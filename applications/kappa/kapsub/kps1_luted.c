#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "f77.h"
#include "sae_par.h"
#include "ast.h"
#include "mers.h"
#include "star/one.h"
#include <tcl.h>

char *cstring( const char *, int, int * );

F77_SUBROUTINE(kps1_luted)( CHARACTER(CMD), INTEGER(STATUS)
                            TRAIL(CMD) ){
/*
*+
*  Name:
*     kps1_luted

*  Purpose:
*     Activates the main LutEdit tcl script.

*  Language:
*     Starlink C

*  Description:
*     This C function executes the LutEdit Tcl script in a child process.

*  Parameters:
*     CMD = CHARACTER * ( * ) (Given)
*        A string representing the command to be used to execute the
*        lutedit script.
*     STATUS = INTEGER (Given and Returned)
*        The inherited global status.

*  Copyright:
*     Copyright (C) 2001-2002, 2004 Central Laboratory of the Research
*     Councils. All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

*  Authors:
*     DSB: David Berry (STARLINK)
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     19-NOV-2001 (DSB):
*        Original version.
*     3-JUL-2002 (DSB):
*        Replaced use of tmpnam with mkstemp.
*     3-SEP-2004 (TIMJ):
*        Now -Wall clean
*     12-MAY-2011 (TIMJ):
*        Use native errRep rather than F77_CALL(err_rep)
*     {enter_further_changes_here}

*-
*/

   GENPTR_CHARACTER(CMD)
   GENPTR_INTEGER(STATUS)

#define BUFLEN 512
   char buf[BUFLEN];
   char outfile_name[255];
   char *script = NULL;
   int ifd;
   int report;
   char *cmd;
   FILE *fd = NULL;

/* Check the global status. */
   if( *STATUS != SAI__OK ) return;

/* Get a null terminated copy of the command. */
   cmd = cstring( CMD, CMD_length, STATUS );
   if ( cmd ) {

/* Get a unique temporary file name. This file is used to collect
   any standard output from the TCL script. All this complication is needed
   to avoid the warning message generated by the linker on RH 7 Linux
   resulting from the use of the simpler "tmpnam" function. */
   one_strlcpy( outfile_name, "luted_outXXXXXX", sizeof(outfile_name), STATUS );
   ifd = mkstemp( outfile_name );
   if( ifd == -1 ){
      *STATUS = SAI__ERROR;
      errRep( "", "Unable to create a temporary \"luted_out\" file name.",
              STATUS );
      return;
   } else {
      close( ifd );
      remove( outfile_name );
   }

/* Construct the full command for the TCL script (redirecting standard
   output and error to the temporary file chosen above). */
      if( *STATUS == SAI__OK ){
         size_t nbytes = 0;
         nbytes = strlen( cmd )
           + strlen( " 1>" )
           + strlen( outfile_name )
           + strlen( " 2>&1" )
           + 1;
         script = astMalloc( nbytes );

         if( !script ) {
            *STATUS = SAI__ERROR;
            errRep( "", "Failed to allocate memory for full TCL command .",
                    STATUS );
         } else {
            one_strlcpy( script, cmd, nbytes, STATUS );
            one_strlcat( script, " 1>", nbytes, STATUS );
            one_strlcat( script, outfile_name, nbytes, STATUS );
            one_strlcat( script, " 2>&1", nbytes, STATUS );
         }
      }

/* Execute the TCL script. */
      if( *STATUS == SAI__OK ){
         (void) system( script );

/* Attempt to open the file containing the standard output and error from
   the TCL script. */
         fd = fopen( outfile_name, "r" );

/* If succesful, display each non-null line of the file and report an error. */
         if( fd ) {

            report = 0;
            while( fgets( buf, BUFLEN, fd ) ){
               if( strlen( buf ) ) {
                  msgOut( " ", buf, STATUS );
                  report = 1;
               }
            }

            if( report && *STATUS == SAI__OK ){
               *STATUS = SAI__ERROR;
               errRep( "", "Messages received from the TCL script.", STATUS );
            }

            fclose( fd );
            remove( outfile_name );
         }
      }

/* Free the memory holding the TCL script name. */
      if( script ) astFree( script );

/*  Free memory used to hold the null-terminated file name */
      astFree( cmd );

   }
}

char *cstring( const char *fstring, int len, int *STATUS ) {
/*
*+
*  Name:
*     cstring

*  Purpose:
*     Returns a pointer to dynaically allocated memory holding a null
*     terminated copy of an F77 string.

*  Language:
*     Starlink C

*  Description:
*     This function returns a pointer to dynaically allocated memory
*     holding a null terminated copy of an F77 string. The pointer should
*     be freed using free() when no longer needed.

*  Parameters:
*     fstring = const char * (Given)
*        The f77 string.
*     len = int (Given)
*        The length of the f77 string to be stored.
*     STATUS = int * (Given and Returned)
*        The inherited status.

*  Copyright:
*     Copyright (C) 2006 Particle Physics & Astronomy Research Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
*     02111-1307, USA

*-
*/

   char *ret;

   ret = NULL;

/* Check the inherited status. */
   if( *STATUS != SAI__OK ) return ret;

/* Find the length excluding any trailing spaces. */
   len = len - 1;
   while( len && fstring[len] == ' ' ) len--;
   len++;

/* Allocate memory to hold a null-terminated copy of the supplied F77
   string. */
   ret =  astMalloc ( sizeof(*ret)*(size_t) ( len + 1 ) );

/* If successful, copy the string, and append a trailing null character. */
   if ( ret ) {
      memcpy( ret, fstring, len );

      ret[ len ] = 0;

/* Report an error if the memory could not be allocated. */
   } else {
      *STATUS = SAI__ERROR;
      errRepf( "", "Unable to allocate %d bytes of memory. ",
               STATUS, len + 1 );
   }

   return ret;
}
