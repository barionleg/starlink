/*
*+
*  Name:
*     ccdputs

*  Type of Module:
*     C extension to Tcl.

*  Language:
*     ANSI C.

*  Purpose:
*     Allows output from Tcl via the ADAM logging system.

*  Usage:
*     ccdputs ?options? message

*  Description:
*     The message supplied to this command is output via ADAM message
*     system.  According to the flags it may be output to the ADAM
*     message system direct using a MSG_OUT call, or via the CCDPACK 
*     logging system using CCD1_MSG or CCD1_ERREP calls.

*  Notes:
*     This command works by writing commands up the pipe to the parent
*     process, if ccdwish is running in pipes mode.  Therefore its
*     implementation is closely tied to the interprocess communication
*     code in the ccdwish binary.

*  Arguments:
*     options
*        The following options may be submitted:
*           -log
*               This will cause the message to be written via the CCDPACK
*               logging system using the CCD1_MSG call (should not be used 
*               with -error).
*           -error
*               This will cause the message to be written via the CCDPACK
*               logging system using the CCD1_ERREP call (should not be
*               used with -log).
*           -name name
*               If supplied, this gives the name of the message to be
*               passed to the ADAM message system.
* 
*        If neither the -log nor the -error flag is specified, the message
*        will be passed directly to the ADAM message system using a
*        MSG_OUT call.
*     message = string
*        This string will get output as a message.

*  Authors:
*     MBT: Mark Taylor (STARLINK)

*  History:
*     21-SEP-2000 (MBT):
*        Original version.

*-
*/

#include <stdio.h>
#include "sae_par.h"
#include "tcl.h"
#include "cnf.h"
#include "mers.h"
#include "msg_par.h"
#include "ccdtcl.h"

   extern int ccdofd;

/**********************************************************************/
   int CcdputsCmd( ClientData clientData, Tcl_Interp *interp, int objc,
                   Tcl_Obj *CONST objv[] ) {
/**********************************************************************/
      int i;                        /* Loop variable */
      int mleng;                    /* Length of the message */
      int nflag;                    /* Number of flag arguments */
      int nleng;                    /* Length of the name argument */
      int status[ 1 ];              /* The starlink inherited status */
      int stype;                    /* The type of message to be sent. */
      char *flag;                   /* Text of flag argument */
      char *msg;                    /* Text of the message */
      char *name;                   /* Name of the message */
      char *usage;                  /* Usage string */

/* Set usage string. */
      usage = "ccdputs ?-error? ?-log? ?-name name? message";

/* Process flags. */
      stype = CCD_CCDMSG;
      name = " ";
      nleng = 1;
      for ( i = 1; *( flag = Tcl_GetString( objv[ i ] ) ) == '-'; i++ ) {
         if ( ! strcmp( flag, "-log" ) ) {
            stype = CCD_CCDLOG;
         }
         else if ( ! strcmp( flag, "-error" ) ) {
            stype = CCD_CCDERR;
         }
         else if ( ! strcmp( flag, "-name" ) ) {
            name = Tcl_GetStringFromObj( objv[ ++i ], &nleng );
         }
         else {
            Tcl_SetObjResult( interp, Tcl_NewStringObj( usage, -1 ) );
            return TCL_ERROR;
         }
      }
      nflag = i - 1;

/* Check syntax. */
      if ( objc + nflag != 2 ) {
         Tcl_WrongNumArgs( interp, 1, objv, "?options? message" );
         return TCL_ERROR;
      }

/* Get string arguments. */
      msg = Tcl_GetStringFromObj( objv[ 1 + nflag ], &mleng );

/* There are two possibilities: either we are running as a subprocess,
   or we are running free standing.  Find out which. */
      if ( ccdofd >= 0 ) {

/* We are running as a subprocess.  Write the message in an appropriate
   format back up the pipe to the parent. */
         write( ccdofd, &stype, sizeof( int ) );
         write( ccdofd, name, nleng );
         write( ccdofd, "\n", 1 );
         write( ccdofd, msg, mleng + 1 );
      }
      else {

/* We are running freestanding.  Simply output the message to standard 
   output. */
         printf( "%s\n", msg );
      }

/* Set result and exit successfully. */
      Tcl_SetObjResult( interp, Tcl_NewStringObj( "", 0 ) );
      return TCL_OK;
   }


/* $Id$ */
