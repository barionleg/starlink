/*
*+
*  Name:
*     IRM1_TRMSZ.C

*  Purpose:
*     Obtains the width and height of the terminal screen or window for
*     UNIX.

*  Language:
*     ANSI C

*  Invocation:
*     CALL IRM1_TRMSZ( WIDTH, HEIGHT )

*  Description:
*     This routine obtains the width and height of the terminal screen
*     or window for UNIX.

*  Arguments:
*     WIDTH = INTEGER (Returned)
*        The width of the screen in characters.
*     HEIGHT = INTEGER (Returned)
*        The height of the screen in lines.

*  Building:
*     -  On SUNs this should be compiled with gcc, but with cc on
*     DECstations.  Use the switch "-I/star/include" on both systems.

*  [optional_subroutine_items]...
*  Authors:
*     AJC: Alan Chipperfield (STARLINK)
*     MJC: Malcolm J. Currie (STARLINK)
*     MJB: MArtin Bly (STARLINK)
*     {enter_new_authors_here}

*  History:
*     1992 June 17 (AJC):
*        Original version.
*     1992 June 22 (MJC):
*        Renamed and modified for KAPPA use.  Converted to SST prologue.
*     1993 March 15 (MJC):
*        Cast initscr to int and added endwin call.
*     1993 August 1 (MJC):
*        New version that does not require curses.
*     1993 December 16 (MJB):
*        Borrowed for iras90 IRM
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*- */

/* Global Constants: */
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include <unistd.h>
#include "f77.h"
#include "ems.h"

/*
*.  */

F77_INTEGER_FUNCTION(irm1_trmsz)(INTEGER(width),INTEGER(height))
{

  int retval = 0;
/* Pointers to Arguments:
*/
   GENPTR_INTEGER(width)
   GENPTR_INTEGER(height)

        struct winsize s;

    if (ioctl (STDOUT_FILENO, TIOCGWINSZ, (char *) &s) < 0) {
      retval = 1;
        *height = 80;
        *width = 0;
    } else {
      retval = 0;
            *height = s.ws_row;
            *width = s.ws_col;
    }

        return retval;
}

