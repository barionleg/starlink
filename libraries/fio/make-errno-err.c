/*
 *+
 *  Name:
 *    make-errno-err

 *  Type of module:
 *     C source

 *  Purpose:
 *     Generate internal Fortran errno header file

 *  Description:
 *     Generate a fortran-compatible include file containing
 *     the errno codes relevant to the FIO package. This is only
 *     relevant on systems that use errno codes to report file
 *     error conditions. The resulting output file is called ERRNO_PAR

 *  Usage:
 *
 *       ./make-errno-err

*  Copyright:
*     Copyright (C) 2004 Particle Physics & Astronomy Research Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

 *  Authors:
 *     TIMJ: Tim Jenness (JAC, Hawaii)
 *     {enter_new_authors_here}

 *  History:
 *     21-Jul-2004 (TIMJ):
 *        Original version
 *     2-Sep-2004 (TIMJ):
 *        Fix some warnings.

 *-
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#if HAVE_ERRNO_H
# include <errno.h>
#endif

#if HAVE_TIME_H
# include <time.h>
#endif

#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
typedef char bool;
#endif

#if HAVE_STRING_H
# include <string.h>
#endif

#define INCLUDE_FILE  "ERRNO_ERR"

/* internal declarations */
const char* todaysdate(void);
void add_line( FILE * file, char * string, int constant, bool isok);

int main (int argc, char ** argv) {
  char * progname = argv[0];
  FILE * OutputFile;

  /* Open the output file */
  OutputFile = fopen(INCLUDE_FILE,"w");
  if (!OutputFile) {
    fprintf(stderr,"%s: can't open file %s for output\n",
	    progname, INCLUDE_FILE);
    return EXIT_FAILURE;
  }

  /* Start writing the include file. Begin with the header */
        fprintf(OutputFile,
"*+\n"
"*  Name:\n"
"*     %s\n"
"\n"
"*  Type of Module:\n"
"*     Fortran include file.\n"
"\n"
"*  Purpose:\n"
"*     Define private C ERRNO constants for the Fortran FIO system.\n"
"\n"
"*  Description:\n"
"*     This file defines private C ERRNO constants for the\n"
"*     FIO system. Required by fio1_serr.f in some cases (eg linux).\n"
"\n"
"*  Authors:\n"
"*     TIMJ: Tim Jenness (JAC, Hawaii)\n"
"*     %s program\n"
"\n"
"*  History:\n"
"*     22-JUL-2004 (TIMJ):\n"
"*        Original version of C program (via auto-generation).\n"
"*     %s (%s):\n"
"*        Generated\n"
"*     No further changes -- do not edit this file\n"
"\n"
"*-\n"
"\n",
		INCLUDE_FILE, progname, todaysdate(), progname);


	/* Not sure how to generate this code via a C program directly.
	   The code here was done by writing a quick awk script to parse
	   errno.h itself. Clearly better to somehow work out what all
	   the defines are, "EPERM" etc, since the awk approach
	   assumes that these codes (generated from linux) are valid
	   on the host errno system. Easiest thing is to write this
	   program in perl or something and just parse the
	   errno.h directly as part of the build.

	   This is the awk:

grep define /usr/include/asm/errno.h | awk '{print "add_line(OutputFile, \""$2"\",\n#ifdef "$2"\n"$2", 1\n#else\n"$3", 0\n#endif\n);\n"}' > xxx

	*/

	add_line(OutputFile, "EPERM",
#ifdef EPERM
		 EPERM, 1
#else
		 1, 0
#endif
		 );

	add_line(OutputFile, "ENOENT",
#ifdef ENOENT
		 ENOENT, 1
#else
		 2, 0
#endif
		 );

	add_line(OutputFile, "ESRCH",
#ifdef ESRCH
		 ESRCH, 1
#else
		 3, 0
#endif
		 );

	add_line(OutputFile, "EINTR",
#ifdef EINTR
		 EINTR, 1
#else
		 4, 0
#endif
		 );

	add_line(OutputFile, "EIO",
#ifdef EIO
		 EIO, 1
#else
		 5, 0
#endif
		 );

	add_line(OutputFile, "ENXIO",
#ifdef ENXIO
		 ENXIO, 1
#else
		 6, 0
#endif
		 );

	add_line(OutputFile, "E2BIG",
#ifdef E2BIG
		 E2BIG, 1
#else
		 7, 0
#endif
		 );

	add_line(OutputFile, "ENOEXEC",
#ifdef ENOEXEC
		 ENOEXEC, 1
#else
		 8, 0
#endif
		 );

	add_line(OutputFile, "EBADF",
#ifdef EBADF
		 EBADF, 1
#else
		 9, 0
#endif
		 );

	add_line(OutputFile, "ECHILD",
#ifdef ECHILD
		 ECHILD, 1
#else
		 10, 0
#endif
		 );

	add_line(OutputFile, "EAGAIN",
#ifdef EAGAIN
		 EAGAIN, 1
#else
		 11, 0
#endif
		 );

	add_line(OutputFile, "ENOMEM",
#ifdef ENOMEM
		 ENOMEM, 1
#else
		 12, 0
#endif
		 );

	add_line(OutputFile, "EACCES",
#ifdef EACCES
		 EACCES, 1
#else
		 13, 0
#endif
		 );

	add_line(OutputFile, "EFAULT",
#ifdef EFAULT
		 EFAULT, 1
#else
		 14, 0
#endif
		 );

	add_line(OutputFile, "ENOTBLK",
#ifdef ENOTBLK
		 ENOTBLK, 1
#else
		 15, 0
#endif
		 );

	add_line(OutputFile, "EBUSY",
#ifdef EBUSY
		 EBUSY, 1
#else
		 16, 0
#endif
		 );

	add_line(OutputFile, "EEXIST",
#ifdef EEXIST
		 EEXIST, 1
#else
		 17, 0
#endif
		 );

	add_line(OutputFile, "EXDEV",
#ifdef EXDEV
		 EXDEV, 1
#else
		 18, 0
#endif
		 );

	add_line(OutputFile, "ENODEV",
#ifdef ENODEV
		 ENODEV, 1
#else
		 19, 0
#endif
		 );

	add_line(OutputFile, "ENOTDIR",
#ifdef ENOTDIR
		 ENOTDIR, 1
#else
		 20, 0
#endif
		 );

	add_line(OutputFile, "EISDIR",
#ifdef EISDIR
		 EISDIR, 1
#else
		 21, 0
#endif
		 );

	add_line(OutputFile, "EINVAL",
#ifdef EINVAL
		 EINVAL, 1
#else
		 22, 0
#endif
		 );

	add_line(OutputFile, "ENFILE",
#ifdef ENFILE
		 ENFILE, 1
#else
		 23, 0
#endif
		 );

	add_line(OutputFile, "EMFILE",
#ifdef EMFILE
		 EMFILE, 1
#else
		 24, 0
#endif
		 );

	add_line(OutputFile, "ENOTTY",
#ifdef ENOTTY
		 ENOTTY, 1
#else
		 25, 0
#endif
		 );

	add_line(OutputFile, "ETXTBSY",
#ifdef ETXTBSY
		 ETXTBSY, 1
#else
		 26, 0
#endif
		 );

	add_line(OutputFile, "EFBIG",
#ifdef EFBIG
		 EFBIG, 1
#else
		 27, 0
#endif
		 );

	add_line(OutputFile, "ENOSPC",
#ifdef ENOSPC
		 ENOSPC, 1
#else
		 28, 0
#endif
		 );

	add_line(OutputFile, "ESPIPE",
#ifdef ESPIPE
		 ESPIPE, 1
#else
		 29, 0
#endif
		 );

	add_line(OutputFile, "EROFS",
#ifdef EROFS
		 EROFS, 1
#else
		 30, 0
#endif
		 );

	add_line(OutputFile, "EMLINK",
#ifdef EMLINK
		 EMLINK, 1
#else
		 31, 0
#endif
		 );

	add_line(OutputFile, "EPIPE",
#ifdef EPIPE
		 EPIPE, 1
#else
		 32, 0
#endif
		 );

	add_line(OutputFile, "EDOM",
#ifdef EDOM
		 EDOM, 1
#else
		 33, 0
#endif
		 );

	add_line(OutputFile, "ERANGE",
#ifdef ERANGE
		 ERANGE, 1
#else
		 34, 0
#endif
		 );

	add_line(OutputFile, "EDEADLK",
#ifdef EDEADLK
		 EDEADLK, 1
#else
		 35, 0
#endif
		 );

	add_line(OutputFile, "ENAMETOOLONG",
#ifdef ENAMETOOLONG
		 ENAMETOOLONG, 1
#else
		 36, 0
#endif
		 );

	add_line(OutputFile, "ENOLCK",
#ifdef ENOLCK
		 ENOLCK, 1
#else
		 37, 0
#endif
		 );

	add_line(OutputFile, "ENOSYS",
#ifdef ENOSYS
		 ENOSYS, 1
#else
		 38, 0
#endif
		 );

	add_line(OutputFile, "ENOTEMPTY",
#ifdef ENOTEMPTY
		 ENOTEMPTY, 1
#else
		 39, 0
#endif
		 );

	add_line(OutputFile, "ELOOP",
#ifdef ELOOP
		 ELOOP, 1
#else
		 40, 0
#endif
		 );

	add_line(OutputFile, "EWOULDBLOCK",
#ifdef EWOULDBLOCK
		 EWOULDBLOCK, 1
#else
		 11, 0
#endif
		 );

	add_line(OutputFile, "ENOMSG",
#ifdef ENOMSG
		 ENOMSG, 1
#else
		 42, 0
#endif
		 );

	add_line(OutputFile, "EIDRM",
#ifdef EIDRM
		 EIDRM, 1
#else
		 43, 0
#endif
		 );

	add_line(OutputFile, "ECHRNG",
#ifdef ECHRNG
		 ECHRNG, 1
#else
		 44, 0
#endif
		 );

	add_line(OutputFile, "EL2NSYNC",
#ifdef EL2NSYNC
		 EL2NSYNC, 1
#else
		 45, 0
#endif
		 );

	add_line(OutputFile, "EL3HLT",
#ifdef EL3HLT
		 EL3HLT, 1
#else
		 46, 0
#endif
		 );

	add_line(OutputFile, "EL3RST",
#ifdef EL3RST
		 EL3RST, 1
#else
		 47, 0
#endif
		 );

	add_line(OutputFile, "ELNRNG",
#ifdef ELNRNG
		 ELNRNG, 1
#else
		 48, 0
#endif
		 );

	add_line(OutputFile, "EUNATCH",
#ifdef EUNATCH
		 EUNATCH, 1
#else
		 49, 0
#endif
		 );

	add_line(OutputFile, "ENOCSI",
#ifdef ENOCSI
		 ENOCSI, 1
#else
		 50, 0
#endif
		 );

	add_line(OutputFile, "EL2HLT",
#ifdef EL2HLT
		 EL2HLT, 1
#else
		 51, 0
#endif
		 );

	add_line(OutputFile, "EBADE",
#ifdef EBADE
		 EBADE, 1
#else
		 52, 0
#endif
		 );

	add_line(OutputFile, "EBADR",
#ifdef EBADR
		 EBADR, 1
#else
		 53, 0
#endif
		 );

	add_line(OutputFile, "EXFULL",
#ifdef EXFULL
		 EXFULL, 1
#else
		 54, 0
#endif
		 );

	add_line(OutputFile, "ENOANO",
#ifdef ENOANO
		 ENOANO, 1
#else
		 55, 0
#endif
		 );

	add_line(OutputFile, "EBADRQC",
#ifdef EBADRQC
		 EBADRQC, 1
#else
		 56, 0
#endif
		 );

	add_line(OutputFile, "EBADSLT",
#ifdef EBADSLT
		 EBADSLT, 1
#else
		 57, 0
#endif
		 );

	add_line(OutputFile, "EDEADLOCK",
#ifdef EDEADLOCK
		 EDEADLOCK, 1
#else
		 35, 0
#endif
		 );

	add_line(OutputFile, "EBFONT",
#ifdef EBFONT
		 EBFONT, 1
#else
		 59, 0
#endif
		 );

	add_line(OutputFile, "ENOSTR",
#ifdef ENOSTR
		 ENOSTR, 1
#else
		 60, 0
#endif
		 );

	add_line(OutputFile, "ENODATA",
#ifdef ENODATA
		 ENODATA, 1
#else
		 61, 0
#endif
		 );

	add_line(OutputFile, "ETIME",
#ifdef ETIME
		 ETIME, 1
#else
		 62, 0
#endif
		 );

	add_line(OutputFile, "ENOSR",
#ifdef ENOSR
		 ENOSR, 1
#else
		 63, 0
#endif
		 );

	add_line(OutputFile, "ENONET",
#ifdef ENONET
		 ENONET, 1
#else
		 64, 0
#endif
		 );

	add_line(OutputFile, "ENOPKG",
#ifdef ENOPKG
		 ENOPKG, 1
#else
		 65, 0
#endif
		 );

	add_line(OutputFile, "EREMOTE",
#ifdef EREMOTE
		 EREMOTE, 1
#else
		 66, 0
#endif
		 );

	add_line(OutputFile, "ENOLINK",
#ifdef ENOLINK
		 ENOLINK, 1
#else
		 67, 0
#endif
		 );

	add_line(OutputFile, "EADV",
#ifdef EADV
		 EADV, 1
#else
		 68, 0
#endif
		 );

	add_line(OutputFile, "ESRMNT",
#ifdef ESRMNT
		 ESRMNT, 1
#else
		 69, 0
#endif
		 );

	add_line(OutputFile, "ECOMM",
#ifdef ECOMM
		 ECOMM, 1
#else
		 70, 0
#endif
		 );

	add_line(OutputFile, "EPROTO",
#ifdef EPROTO
		 EPROTO, 1
#else
		 71, 0
#endif
		 );

	add_line(OutputFile, "EMULTIHOP",
#ifdef EMULTIHOP
		 EMULTIHOP, 1
#else
		 72, 0
#endif
		 );

	add_line(OutputFile, "EDOTDOT",
#ifdef EDOTDOT
		 EDOTDOT, 1
#else
		 73, 0
#endif
		 );

	add_line(OutputFile, "EBADMSG",
#ifdef EBADMSG
		 EBADMSG, 1
#else
		 74, 0
#endif
		 );

	add_line(OutputFile, "EOVERFLOW",
#ifdef EOVERFLOW
		 EOVERFLOW, 1
#else
		 75, 0
#endif
		 );

	add_line(OutputFile, "ENOTUNIQ",
#ifdef ENOTUNIQ
		 ENOTUNIQ, 1
#else
		 76, 0
#endif
		 );

	add_line(OutputFile, "EBADFD",
#ifdef EBADFD
		 EBADFD, 1
#else
		 77, 0
#endif
		 );

	add_line(OutputFile, "EREMCHG",
#ifdef EREMCHG
		 EREMCHG, 1
#else
		 78, 0
#endif
		 );

	add_line(OutputFile, "ELIBACC",
#ifdef ELIBACC
		 ELIBACC, 1
#else
		 79, 0
#endif
		 );

	add_line(OutputFile, "ELIBBAD",
#ifdef ELIBBAD
		 ELIBBAD, 1
#else
		 80, 0
#endif
		 );

	add_line(OutputFile, "ELIBSCN",
#ifdef ELIBSCN
		 ELIBSCN, 1
#else
		 81, 0
#endif
		 );

	add_line(OutputFile, "ELIBMAX",
#ifdef ELIBMAX
		 ELIBMAX, 1
#else
		 82, 0
#endif
		 );

	add_line(OutputFile, "ELIBEXEC",
#ifdef ELIBEXEC
		 ELIBEXEC, 1
#else
		 83, 0
#endif
		 );

	add_line(OutputFile, "EILSEQ",
#ifdef EILSEQ
		 EILSEQ, 1
#else
		 84, 0
#endif
		 );

	add_line(OutputFile, "ERESTART",
#ifdef ERESTART
		 ERESTART, 1
#else
		 85, 0
#endif
		 );

	add_line(OutputFile, "ESTRPIPE",
#ifdef ESTRPIPE
		 ESTRPIPE, 1
#else
		 86, 0
#endif
		 );

	add_line(OutputFile, "EUSERS",
#ifdef EUSERS
		 EUSERS, 1
#else
		 87, 0
#endif
		 );

	add_line(OutputFile, "ENOTSOCK",
#ifdef ENOTSOCK
		 ENOTSOCK, 1
#else
		 88, 0
#endif
		 );

	add_line(OutputFile, "EDESTADDRREQ",
#ifdef EDESTADDRREQ
		 EDESTADDRREQ, 1
#else
		 89, 0
#endif
		 );

	add_line(OutputFile, "EMSGSIZE",
#ifdef EMSGSIZE
		 EMSGSIZE, 1
#else
		 90, 0
#endif
		 );

	add_line(OutputFile, "EPROTOTYPE",
#ifdef EPROTOTYPE
		 EPROTOTYPE, 1
#else
		 91, 0
#endif
		 );

	add_line(OutputFile, "ENOPROTOOPT",
#ifdef ENOPROTOOPT
		 ENOPROTOOPT, 1
#else
		 92, 0
#endif
		 );

	add_line(OutputFile, "EPROTONOSUPPORT",
#ifdef EPROTONOSUPPORT
		 EPROTONOSUPPORT, 1
#else
		 93, 0
#endif
		 );

	add_line(OutputFile, "ESOCKTNOSUPPORT",
#ifdef ESOCKTNOSUPPORT
		 ESOCKTNOSUPPORT, 1
#else
		 94, 0
#endif
		 );

	add_line(OutputFile, "EOPNOTSUPP",
#ifdef EOPNOTSUPP
		 EOPNOTSUPP, 1
#else
		 95, 0
#endif
		 );

	add_line(OutputFile, "EPFNOSUPPORT",
#ifdef EPFNOSUPPORT
		 EPFNOSUPPORT, 1
#else
		 96, 0
#endif
		 );

	add_line(OutputFile, "EAFNOSUPPORT",
#ifdef EAFNOSUPPORT
		 EAFNOSUPPORT, 1
#else
		 97, 0
#endif
		 );

	add_line(OutputFile, "EADDRINUSE",
#ifdef EADDRINUSE
		 EADDRINUSE, 1
#else
		 98, 0
#endif
		 );

	add_line(OutputFile, "EADDRNOTAVAIL",
#ifdef EADDRNOTAVAIL
		 EADDRNOTAVAIL, 1
#else
		 99, 0
#endif
		 );

	add_line(OutputFile, "ENETDOWN",
#ifdef ENETDOWN
		 ENETDOWN, 1
#else
		 100, 0
#endif
		 );

	add_line(OutputFile, "ENETUNREACH",
#ifdef ENETUNREACH
		 ENETUNREACH, 1
#else
		 101, 0
#endif
		 );

	add_line(OutputFile, "ENETRESET",
#ifdef ENETRESET
		 ENETRESET, 1
#else
		 102, 0
#endif
		 );

	add_line(OutputFile, "ECONNABORTED",
#ifdef ECONNABORTED
		 ECONNABORTED, 1
#else
		 103, 0
#endif
		 );

	add_line(OutputFile, "ECONNRESET",
#ifdef ECONNRESET
		 ECONNRESET, 1
#else
		 104, 0
#endif
		 );

	add_line(OutputFile, "ENOBUFS",
#ifdef ENOBUFS
		 ENOBUFS, 1
#else
		 105, 0
#endif
		 );

	add_line(OutputFile, "EISCONN",
#ifdef EISCONN
		 EISCONN, 1
#else
		 106, 0
#endif
		 );

	add_line(OutputFile, "ENOTCONN",
#ifdef ENOTCONN
		 ENOTCONN, 1
#else
		 107, 0
#endif
		 );

	add_line(OutputFile, "ESHUTDOWN",
#ifdef ESHUTDOWN
		 ESHUTDOWN, 1
#else
		 108, 0
#endif
		 );

	add_line(OutputFile, "ETOOMANYREFS",
#ifdef ETOOMANYREFS
		 ETOOMANYREFS, 1
#else
		 109, 0
#endif
		 );

	add_line(OutputFile, "ETIMEDOUT",
#ifdef ETIMEDOUT
		 ETIMEDOUT, 1
#else
		 110, 0
#endif
		 );

	add_line(OutputFile, "ECONNREFUSED",
#ifdef ECONNREFUSED
		 ECONNREFUSED, 1
#else
		 111, 0
#endif
		 );

	add_line(OutputFile, "EHOSTDOWN",
#ifdef EHOSTDOWN
		 EHOSTDOWN, 1
#else
		 112, 0
#endif
		 );

	add_line(OutputFile, "EHOSTUNREACH",
#ifdef EHOSTUNREACH
		 EHOSTUNREACH, 1
#else
		 113, 0
#endif
		 );

	add_line(OutputFile, "EALREADY",
#ifdef EALREADY
		 EALREADY, 1
#else
		 114, 0
#endif
		 );

	add_line(OutputFile, "EINPROGRESS",
#ifdef EINPROGRESS
		 EINPROGRESS, 1
#else
		 115, 0
#endif
		 );

	add_line(OutputFile, "ESTALE",
#ifdef ESTALE
		 ESTALE, 1
#else
		 116, 0
#endif
		 );

	add_line(OutputFile, "EUCLEAN",
#ifdef EUCLEAN
		 EUCLEAN, 1
#else
		 117, 0
#endif
		 );

	add_line(OutputFile, "ENOTNAM",
#ifdef ENOTNAM
		 ENOTNAM, 1
#else
		 118, 0
#endif
		 );

	add_line(OutputFile, "ENAVAIL",
#ifdef ENAVAIL
		 ENAVAIL, 1
#else
		 119, 0
#endif
		 );

	add_line(OutputFile, "EISNAM",
#ifdef EISNAM
		 EISNAM, 1
#else
		 120, 0
#endif
		 );

	add_line(OutputFile, "EREMOTEIO",
#ifdef EREMOTEIO
		 EREMOTEIO, 1
#else
		 121, 0
#endif
		 );

	add_line(OutputFile, "EDQUOT",
#ifdef EDQUOT
		 EDQUOT, 1
#else
		 122, 0
#endif
		 );

	add_line(OutputFile, "ENOMEDIUM",
#ifdef ENOMEDIUM
		 ENOMEDIUM, 1
#else
		 123, 0
#endif
		 );

	add_line(OutputFile, "EMEDIUMTYPE",
#ifdef EMEDIUMTYPE
		 EMEDIUMTYPE, 1
#else
		 124, 0
#endif
		 );



	/* Tidy up */
	fclose(OutputFile);

        return EXIT_SUCCESS;
}

/* Insert code for a specific errno constant */
void add_line( FILE * file, char * string, int constant, bool isok) {
  char * msg;

  /* Translate the error code to an error message */
#if HAVE_STRERROR
  msg = strerror(constant);
#else
  msg = "<Unknown>";
#endif

  /* the isok argument is used to enable us to fall back
     to a fixed number if we are guessing */
  if (!isok) {
    fprintf(file, "*     %s was not present. Guessing\n", string);
  }
  fprintf(file, "      INTEGER ERRNO__%s    ! %s\n"
	  "      PARAMETER ( ERRNO__%s = %d )\n\n",
	  string, msg, string, constant);

}


/* Stolen from make-prm-par.c */
const char *todaysdate(void)
{
#if HAVE_TIME_H
    static char s[12];
    time_t t = time(0);

    strftime(s, sizeof(s), "%d-%b-%Y", localtime(&t));
    return s;
#else
    return "TODAY";
#endif
}
