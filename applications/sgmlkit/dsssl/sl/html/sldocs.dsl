<!-- -*- mode: sgml; sgml-parent-document: ("sl\.dsl" "CODEGROUP" '("PROGRAMCODE" "CODEGROUP")); -*- -->

<routine>
<routinename>process-html-document
<description>
<p>As this stylesheet's sole direct output, produce a string
which consists of the file-name-root, a colon, and a
space-separated list of the types which the generated HTML has
linked to.

<p>All the rest of the output goes to an entity generated by
<funcname>html-document</>
<returnvalue type=sosofo>A sosofo which produces output to stdout,
then generates an HTML file
<codebody>
(define (process-html-document)
  (make sequence
    (if stream-output			; no info if output to stdout
	(empty-sosofo)
	(let ((extlist (if (and %link-extension-list%
				(not suppress-printable))
			   (apply string-append
				  (map (lambda (l)
					 (string-append (car l) " "))
				       %link-extension-list%))
			   "")))
	  (literal (string-append (root-file-name) ":" extlist ":"))))
    (html-document
     (process-node-list (getdocinfo 'title))
     (process-matching-children 'docbody)
     ;; don't respect suppress-navig here, or we lose the TOC
     )))

<routine>
<description>
These are the document element types.
<codebody>
(element sug (process-html-document))
(element sun (process-html-document))
(element ssn (process-html-document))
(element sc  (process-html-document))
(element sg  (process-html-document))
(element sgp (process-html-document))
(element mud (process-html-document))


<routine>
<description>Flow-object constructors for the head

<codebody>

;(element title
;  (literal (normalise-string (data (current-node)))))
(element title
  (process-children))
;(element displaytitle
;  (process-children))

(element authorlist
  (process-children))

(element otherauthors
  (let ((a (children (current-node))))
    (if (node-list-empty? a)
	(empty-sosofo)
	(node-list-reduce a
			  (lambda (res i)
			    (sosofo-append res
					   (make sequence
					     (process-node-list i)
					     (literal "; "))))
			  (literal "With: ")))))

(element manualtype
   (case (normalize (attribute-string (normalize "type")))
      (("users") (literal "User's Guide"))
      (("programmers") (literal "Programmer's Guide"))
      (("programmers.c") (literal "Programmer's Guide (C version)"))
      (("programmers.fortran") (literal "Programmer's Guide (Fortran version)"))
      (else (process-children))))

(element softwareversion
   (process-children))

(mode make-html-author-links
      (element author
	       (let ((link (cond ((attribute-string (normalize "webpage")
						    (current-node)))
				 ((attribute-string (normalize "email")
						    (current-node))
				  (string-append
				   "mailto:"
				   (attribute-string (normalize "email")
						     (current-node))))
				 (else #f))))
		 (if link
		     (make empty-element
			   gi: "link"
			   attributes: `(("rev" "made")
					 ("href" ,link)
					 ("title" ,(trim-data (current-node)))))
		   (empty-sosofo)))))

;; In the default mode, make author a link to the author details,
;; if either of the webpage or email attributes is present
(element author
  (let* ((webpage (attribute-string (normalize "webpage") (current-node)))
	 (link (cond (webpage)
		     ((attribute-string (normalize "email")
					 (current-node))
		       (string-append "mailto:"
				      (attribute-string (normalize "email")
							(current-node))))
		     (else #f))))
      (if link
	  (make element gi: "a"
		attributes: (list (list "href" link))
		(literal (normalise-string (data (current-node)))))
	  (literal (normalise-string (data (current-node)))))))



<routine>
<description>
Flow-object constructors for the document body

<p>Process the docbody element by creating a `title page', using
information from the docinfo element, then doing <funcname>process-children</>
to format the document content.

<p>The title page is currently configured (pending a re-design) to look
more-or-less as much like the existing star2html output as possible.

<codebody>
(element docbody
   (let* ((title (getdocinfo 'title))
          (rel (document-release-info))
          (vers (caddr rel))
          (dotvers (if vers (string-append "." vers) ""))
	  (dn (getdocnumber (current-node) 'asString))
          (docwordref (if dn
			  (string-append dn dotvers)
			  "Version"))
          (docref (string-append (index-file-name) dotvers))
          (manualtype (getdocinfo 'manualtype))
          (softwareversion (getdocinfo 'softwareversion))
          (copyright (getdocinfo 'copyright))
          (coverimage (getdocinfo 'coverimage))
         )
      (make sequence
         (make element gi: "h1"  attributes: '(("align" "center"))
            (make element gi: "a" attributes: '(("name" "xref_"))
               (process-node-list title)))
         (if softwareversion
             (make element gi: "h2" attributes: '(("align" "center"))
                (process-node-list softwareversion))
             (empty-sosofo))
         (if manualtype
             (make element gi: "h2" attributes: '(("align" "center"))
                (process-node-list manualtype))
             (empty-sosofo))
         (if coverimage
             (make element gi: "div" attributes: '(("align" "center"))
                (process-node-list coverimage)) 
             (empty-sosofo))
         (make empty-element gi: "hr")
         (make element gi: "p"
             (make element gi: "i"
                (if docwordref 
                    (make sequence 
                       (literal docwordref) 
                       (make empty-element gi: "br")) 
                    (empty-sosofo))
                (getdocauthors)
                (make empty-element gi: "br")
                (literal (getdocdate))))
         (if (not suppress-banner)
             (%starlink-banner%)
             (empty-sosofo))
         (if (not suppress-printable)
             (make element gi: "p"
                (make element gi: "a"
                      attributes: (list (list "href" (starlink-hardcopy-server 
                                                      docref)))
                   (make empty-element gi: "img"
                               attributes: 
                                  (list (list "border" "0") 
                                        (list "src" (file-href "hardcopy"))))
                   (literal "Retrieve hardcopy")))
             (empty-sosofo))
         (make empty-element gi: "hr")
	 ;; Include a TOC here, if we're producing single-file output
	 (if (chunking?)
	     (empty-sosofo)
	     (make sequence
	       (make element gi: "h2"
		     (literal "Contents"))
	       (make-contents (current-node) 2 #t)))
         (process-children)
         (process-backmatter)
	 )))
        


