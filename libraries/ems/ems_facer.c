/*+
 *  Name:
 *     emsFacer
 *
 *  Purpose:
*  Fortran callable routine

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

*  Copyright:
*     Copyright (C) 2006 Particle Physics & Astronomy Research Council.
*     All Rights Reserved.

 *  Authors:

 *-
*/
#include "ems.h"                       /* ems_ function prototypes */
#include "ems_sys.h"                   /* EMS internal constants */
#include "f77.h"                       /* CNF macros and prototypes */

#include "ems_f77.h"

F77_SUBROUTINE(ems_facer) ( CHARACTER(token), INTEGER(fstat) TRAIL(token) )
{
   char ctok[EMS__SZNAM+1];      /* Imported token name */

   GENPTR_CHARACTER(token)
   GENPTR_INTEGER(fstat)

   cnfImpn( token, token_length, EMS__SZNAM, ctok );

   emsFacer( ctok, *fstat );

   return;
}
