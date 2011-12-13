      SUBROUTINE ASTLUTMAP( STATUS )
*+
*  Name:
*     ASTLUTMAP

*  Purpose:
*     Create a LutMap.

*  Language:
*     Starlink Fortran 77

*  Type of Module:
*     ADAM A-task

*  Invocation:
*     CALL ASTLUTMAP( STATUS )

*  Arguments:
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Description:
*     This application creates a new LutMap and optionally initialises
*     its attributes. A LutMap is a specialised form of Mapping which
*     transforms 1-dimensional coordinates by using linear interpolation
*     in a lookup table.
*
*     Each input coordinate value is first scaled to give the index of
*     an entry in the table by subtracting a starting value (the input
*     coordinate corresponding to the first table entry) and dividing
*     by an increment (the difference in input coordinate value
*     between adjacent table entries).
*
*     The resulting index will usually contain a fractional part, so
*     the output coordinate value is then generated by interpolating
*     linearly between the appropriate entries in the table. If the
*     index lies outside the range of the table, linear extrapolation
*     is used based on the two nearest entries (i.e. the two entries
*     at the start or end of the table, as appropriate).
*
*     If the lookup table entries increase or decrease monotonically,
*     then the inverse transformation may also be performed.

*  Usage:
*     astlutmap lut start inc options result

*  ADAM Parameters:
*     INC = _DOUBLE (Read)
*        The lookup table spacing (the increment in input coordinate
*        value between successive lookup table entries). This value
*        may be positive or negative, but must not be zero.
*     LUT = GROUP (Read)
*        A comma-separated list of floating point values to be used as the
*        lookup table entries. A text file may be specified by preceeding the
*        name of the file with an up arrow character "^". If the supplied value
*        ends with a minus sign, the user is re-prompted for additional
*        values.
*     OPTIONS = LITERAL (Read)
*        A string containing an optional comma-separated list of attribute
*        assignments to be used for initialising the new LutMap.
*     RESULT = LITERAL (Read)
*        A text file to receive the new LutMap.
*     START = _DOUBLE (Read)
*        The input coordinate value which corresponds to the first
*        lookup table entry.

*  Copyright:
*     Copyright (C) 2001, 2004 Central Laboratory of the Research
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
*     14-FEB-2001 (DSB):
*        Original version.
*     2004 September 1 (TIMJ):
*        Use CNF_PVAL
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-
*  Type Definitions:
      IMPLICIT NONE              ! no default typing allowed

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'AST_PAR'          ! AST constants and function declarations
      INCLUDE 'GRP_PAR'          ! GRP constants
      INCLUDE 'CNF_PAR'          ! For CNF_PVAL function

*  Status:
      INTEGER STATUS

*  Local Variables:
      DOUBLE PRECISION INC
      DOUBLE PRECISION START
      INTEGER IGRP
      INTEGER IPLUT
      INTEGER NLUT
      INTEGER RESULT
      LOGICAL DONE
*.

*  Check inherited status.
      IF( STATUS .NE. SAI__OK ) RETURN

*  Begin an AST context.
      CALL AST_BEGIN( STATUS )

*  Loop until we have a good LUT.
      DONE = .FALSE.
      DO WHILE( .NOT. DONE .AND. STATUS .EQ. SAI__OK )

*  Get a group holding the formated LUT values.
         IGRP = GRP__NOID
         CALL KPG1_GTGRP( 'LUT', IGRP, NLUT, STATUS )

*  Allocate memory to hold the floating point LUT values.
         CALL PSX_CALLOC( NLUT, '_DOUBLE', IPLUT, STATUS )

*  Abort if an error has occurred.
         IF( STATUS .NE. SAI__OK ) GO TO 999

*  Read the values from the group into the memory.
         CALL ATL1_GTOFL( IGRP, NLUT, 1, %VAL( CNF_PVAL( IPLUT ) ),
     :                    STATUS )

*  If an error occurred reading the group, annull the error, cancel the
*  parameter and go round to get a new group. Otherwise, indicate that we
*  can leave the loop.
         IF( STATUS .NE. SAI__OK ) THEN
            CALL ERR_REP( 'ASTLUTMAP_ERR1', 'Please supply a new LUT:',
     :                    STATUS )
            CALL ERR_FLUSH( STATUS )
            CALL GRP_DELET( IGRP, STATUS )
            CALL PAR_CANCL( 'LUT', STATUS )
            CALL PSX_FREE( IPLUT, STATUS )
         ELSE
            DONE = .TRUE.
         END IF

      END DO

*  Get the other parameter values.
      CALL PAR_GET0D( 'START', START, STATUS )

      DONE = .FALSE.
      DO WHILE( .NOT. DONE .AND. STATUS .EQ. SAI__OK )
         CALL PAR_GET0D( 'INC', INC, STATUS )
         IF( INC .EQ. 0.0 .AND. STATUS .EQ. SAI__OK ) THEN
            CALL PAR_CANCL( 'INC', STATUS )
            STATUS = SAI__ERROR
            CALL ERR_REP( 'ASTLUTMAP_ERR2', 'Zero not allowed for '//
     :                    'parameter INC. Please supply a new value.',
     :                    STATUS )
            CALL ERR_FLUSH( STATUS )
         ELSE
            DONE = .TRUE.
         END IF
      END DO

*  Create the required LutMap.
      RESULT = AST_LUTMAP( NLUT, %VAL( CNF_PVAL( IPLUT ) ),
     :                     START, INC, ' ',
     :                     STATUS )

*  Store the required attribute values.
      CALL ATL1_SETOP( 'OPTIONS', RESULT, STATUS )

*  Write the results out to a text file.
      CALL ATL1_PTOBJ( 'RESULT', ' ', RESULT, STATUS )

 999  CONTINUE

*  Free the LUT.
      CALL GRP_DELET( IGRP, STATUS )
      CALL PSX_FREE( IPLUT, STATUS )

*  End the AST context.
      CALL AST_END( STATUS )

*  Give a context message if anything went wrong.
      IF( STATUS .NE. SAI__OK ) THEN
         CALL ERR_REP( 'ASTLUTMAP_ERR', 'Error creating a new '//
     :                 'LutMap.', STATUS )
      END IF

      END
