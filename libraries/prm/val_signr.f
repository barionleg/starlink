      REAL FUNCTION VAL_SIGNR( BAD, ARG1, ARG2, STATUS )
*+
*  Name:
*     VAL_SIGNR

*  Purpose:
*     Evaluate the REAL Fortran SIGN (sign transfer) function.

*  Language:
*     Starlink Fortran

*  Invocation:
*     RESULT = VAL_SIGNR( BAD, ARG1, ARG2, STATUS )

*  Description:
*     The routine evaluates the Fortran SIGN (sign transfer) function for a pair of
*     arguments of type REAL.  If a numerical error occurs, the value
*     VAL__BADR is returned and a STATUS value is set.

*  Arguments:
*     BAD = LOGICAL (Given)
*        Whether the argument values (ARG1 & ARG2) may be "bad".
*     ARG1, ARG2 = REAL (Given)
*        The two REAL arguments of the Fortran SIGN (sign transfer) function.
*     STATUS = INTEGER (Given & Returned)
*        This should be set to SAI__OK on entry, otherwise the routine
*        returns immediately with the result VAL__BADR.  A STATUS
*        value will be set by this routine if a numerical error occurs.

*  Returned Value:
*     VAL_SIGNR = REAL
*        Returns the evaluated Fortran SIGN (sign transfer) function result as a REAL
*        value.  The value VAL__BADR will be returned under error
*        conditions.

*  Copyright:
*     Copyright (C) 1987, 1991 Science & Engineering Research Council.
*     Copyright (C) 1995 Central Laboratory of the Research Councils.
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
*     R.F. Warren-Smith (STARLINK)
*     {enter_new_authors_here}

*  History:
*     11-AUG-1987 (RFWS):
*        Original version.
*     28-OCT-1991 (RFWS):
*        Added LIB$REVERT call.
*     7-NOV-1991 (RFWS):
*        Changed to use NUM_TRAP.
*     27-SEP-1995 (BKM):
*        Changed LIB$ESTABLISH and LIB$REVERT calls to NUM_HANDL and NUM_REVRT
*     22-FEB-2022 (DSB):
*        Changed error handling to use NUM_TEST
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PRM_PAR'          ! PRM_ public constants
      INCLUDE 'PRM_CONST'        ! PRM_ private constants
      INCLUDE 'PRM_ERR'          ! PRM_ public constants

*  Arguments Given:
      LOGICAL BAD                ! Bad data flag
      REAL ARG1                ! Function argument 1
      REAL ARG2                ! Function argument 2

*  Status:
      INTEGER STATUS             ! Error status

*  External References:
      LOGICAL NUM_TEST           ! Error testing routine

*  Internal References:
      INCLUDE 'NUM_DEC_CVT'      ! Declare NUM_ conversion functions

      INCLUDE 'NUM_DEC_R'      ! Declare NUM_ arithmetic functions

      INCLUDE 'NUM_DEF_CVT'      ! Define NUM_ conversion functions

      INCLUDE 'NUM_DEF_R'      ! Define NUM_ arithmetic functions

*.

*  Check status.  Return the function result VAL__BADR if not OK.
      IF( STATUS .NE. SAI__OK ) THEN
         VAL_SIGNR = VAL__BADR

*  If the bad value flag is set, check the arguments given are not bad.
*  Return VAL__BADR if either is.
      ELSE IF( BAD .AND. ( ( ARG1 .EQ. VAL__BADR ) .OR.
     :                     ( ARG2 .EQ. VAL__BADR ) ) ) THEN
         VAL_SIGNR = VAL__BADR

*  Check if the argument values are acceptable.  If not, return the
*  result VAL__BADR and set a STATUS value.
      ELSE IF( .NOT. ( .TRUE. ) ) THEN
         VAL_SIGNR = VAL__BADR
         STATUS = SAI__OK

*  If the argument values are acceptable...
      ELSE

*  Evaluate the function.
         VAL_SIGNR = NUM_SIGNR( ARG1, ARG2 )

*  If an error handler is established, check if the numerical error
*  flag is set.  If so, return the result VAL__BADR and set STATUS to
*  NUM__FPERR.
         IF( .FALSE. ) THEN
            IF( NUM_TEST() ) THEN
               VAL_SIGNR = VAL__BADR
               STATUS = PRM__FPERR
            ENDIF

         ENDIF
      ENDIF

*  Exit routine.
      END
