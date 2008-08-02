/*
*+
*  Name:
*     err1Print

*  Purpose:
*     Split the text of an error message up for delivery to the user.

*  Language:
*     Starlink ANSI C

*  Invocation:
*     err1Print( const char * text, int * errbel, int * status);

*  Description:
*     The text of the given error message is split into lines of length
*     ERR__SZOUT. Each line in than delivered to the user by a call to
*     err1Prerr

*  Arguments:
*     text = const char * (Given)
*        Text to be output.
*     errbel = int * (Given & Returned)
*        If true, an attempt will be made to ring a terminal bell
*        in addition to flushing the error messages. Will be set to
*        false if the bell was rung.
*     status = int * (Returned)
*        The global status. Returned if status is set bad to this routine,
*        else it is not touched.

*  Copyright:
*     Copyright (C) 2008 Science and Technology Facilities Council.
*     Copyright (C) 1983, 1985, 1987, 1989-1994 Science & Engineering
*     Research Council. Copyright (C) 1997, 1999, 2001 Central 
*     Laboratory of the Research Councils. All Rights Reserved.

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
*     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
*     02111-1307, USA

*  Authors:
*     JRG: Jack Giddings (UCL)
*     SLW: Sid Wright (UCL)
*     BDK: Dennis Kelly (ROE)
*     RFWS: R.F. Warren-Smith (STARLINK)
*     PCTR: P.C.T. Rees (STARLINK)
*     AJC: A.J.Chipperfield (STARLINK)
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     3-JAN-1983 (JRG):
*        Original version.
*     10-MAR-1983 (SLW):
*        Modified to use SAI I/O.
*     10-APR-1985 (BDK):
*        ADAM version.
*     4-NOV-1987 (BDK):
*        Use ERR_SZMSG.
*     7-AUG-1989 (RFWS):
*        Converted to new prologue layout and added comments.
*     12-SEP-1989 (PCTR):
*        Completed code tidy-up.
*     9-APR-1990 (PCTR):
*        Removed unreferenced include file.
*     8-AUG-1991 (PCTR):
*        Re-written to wrap error messages and call ERR1_PRERR.
*     19-AUG-1992 (PCTR):
*        Changed argument list in EMS1_PFORM call.
*     3-SEP-1992 (PCTR):
*        Remove the LSTAT check in the delivery loop.
*     4-OCT-1993 (PCTR):
*        Added bell character behaviour.
*     12-MAY-1994 (AJC):
*        EMS1_PFORM renamed EMS1_RFORM
*     15-AUG-1997 (AJC):
*        Use NEQV to compare ERRBEL
*     21-JUL-1999 (AJC):
*        Add tuning parameters ERRWSZ and ERRSTM
*     26-FEB-2001 (AJC):
*        Change EMS1_RFORM to MSG1_RFORM
*     28-JUL-2008 (TIMJ):
*        Add extra argument so that we do not need ERRBEL common block.
*     30-JUL-2008 (TIMJ):
*        Rewrite in C
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-
*/

#include "sae_par.h"
#include "err_par.h"
#include "merswrap.h"
#include "ems.h"
#include "mers1.h"

#include <string.h>

void err1Print( const char * text, int *errbel, int *status) {


  /*  Local Variables: */
  const char constr[] = "!     ";/* Continuation string */
  int contab;                    /* Length of continuation string */
  int iposn;                     /* Character position for text */
  int leng;                      /* String length */
  int lstat = SAI__OK;           /* Local status */
  int lstart;                    /* Start index of line */
  int oplen;                     /* Output string length */
  char line[ERR__SZMSG+1];       /* Output line of text */


  /* Tuning parameters */
  int errwsz;
  int errstm;

  /* Get tuning parameters */
  err1Gtglbl( &errwsz, &errstm, NULL );

  /*  Get length of text to send. */
  leng = strlen( text );

  /* and length of continuation character */
  contab = strlen( constr );

  /*  Check whether a bell character is to be delivered and initialise the 
   *  output line. */
  if (errbel) {
    strcpy(line, "\a");
    lstart = 1;
    *errbel = 0;
  } else {
    strcpy(line, " ");
    lstart = 0;
  }

  /*  If the text is not blank, then continue. */
  if (leng > 0) {

    /*     Loop to split the line of text into sensible lengths for output, 
     *     then write them to the error stream. First, initialise the 
     *     character pointer. */
    iposn = 0;

    /*     Now output the message in a way determined by the relevant tuning
     *     parameters. */
    if (errstm) {
      if (lstart > 0) err1Prerr( line, &lstat);
      err1Prerr( text, &lstat );
    } else {
      /*     Call MSG1_RFORM to load the first output line and deliver
       *     the result. */
      ems1Rform( text, errwsz - lstart, &iposn, &(line[lstart]), &oplen );
      err1Prerr( line, &lstat );

      /*     Loop to continue the remainder of the message.*/
      while (iposn != 0) {

	/*        Re-initialise the output line for a continuation. */
	strcpy( line, constr );

	/*        Call MSG1_RFORM to load the continuation line and write the
	 *        result. */
	ems1Rform( text, errwsz - contab, &iposn, &(line[contab]), &oplen);
	err1Prerr( line, &lstat );
      }
    }
  } else {

    /*     If there is no text, then send a blank message. */
    err1Prerr( "!!", &lstat );
  }

  /*  Check I/O status and set STATUS if necessary. */
  if (lstat != SAI__OK) {
    *status = lstat;
  }
}


/*
*+
*  Name:
*     ERR1_PRINT

*  Purpose:
*     Split the text of an error message up for delivery to the user.

*  Language:
*     Starlink ANSI C

*  Invocation:
*     CALL ERR1_PRINT( TEXT, ERRBEL, STATUS )

*  Description:
*     The text of the given error message is split into lines of length
*     ERR__SZOUT. Each line in than delivered to the user by a call to
*     ERR1_PRERR.

*  Arguments:
*     TEXT = CHARACTER * ( * ) (Given)
*        Text to be output.
*     ERRBEL = LOGICAL (Given & Returned)
*        If true, an attempt will be made to ring a terminal bell
*        in addition to flushing the error messages. Will be set to
*        false if the bell was rung.
*     STATUS = INTEGER (Returned)
*        The global status.

*/

F77_SUBROUTINE(err1_print)( CHARACTER(TEXT), LOGICAL(ERRBEL), 
			    INTEGER(STATUS) TRAIL(TEXT) );

F77_SUBROUTINE(err1_print)( CHARACTER(TEXT), LOGICAL(ERRBEL), 
			   INTEGER(STATUS) TRAIL(TEXT)) {
  char * text;
  int errbel;
  int status;
  
  GENPTR_CHARACTER(TEXT);

  text = starMallocAtomic( TEXT_length + 1);
  F77_IMPORT_CHARACTER( TEXT, TEXT_length, text );

  F77_IMPORT_INTEGER( *ERRBEL, errbel );
  F77_IMPORT_INTEGER( *STATUS, status );
  err1Print( text, &errbel, &status );
  F77_EXPORT_INTEGER( errbel, *ERRBEL );
  F77_EXPORT_INTEGER( status, *STATUS );
  starFree( text);
}


