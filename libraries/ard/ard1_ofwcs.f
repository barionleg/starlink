      SUBROUTINE ARD1_OFWCS( AWCS, PAR, UWCS, STATUS )
*+
*  Name:
*     ARD1_OFWCS

*  Purpose:
*     Create a new user FrameSet from a OFFSET statement.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL ARD1_OFWCS( AWCS, PAR, UWCS, STATUS )

*  Description:
*     This routine creates a new user FrameSet (UWCS) from the
*     supplied parameters.

*  Arguments:
*     AWCS = INTEGER (Given)
*        The application FrameSet.
*     PAR( * ) = DOUBLE PRECISION (Given)
*        The statement parameters.
*     UWCS = INTEGER (Given)
*        An AST pointer to the User FrameSet. The Current Frame
*        in this FrameSet is user coords.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Copyright:
*     Copyright (C) 2001 Central Laboratory of the Research Councils.
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
*     DSB: David S. Berry (STARLINK)
*     {enter_new_authors_here}

*  History:
*     6-JUL-2001 (DSB):
*        Original version.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'AST_PAR'          ! AST constants and function declarations
      INCLUDE 'ARD_ERR'          ! ARD error constants
      INCLUDE 'ARD_CONST'        ! ARD private constants

*  Arguments Given:
      INTEGER AWCS
      DOUBLE PRECISION PAR( * )

*  Arguments Returned:
      INTEGER UWCS

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      CHARACTER DOM*30                   ! Frame Domain
      DOUBLE PRECISION INA( ARD__MXDIM ) ! Position A without offset
      DOUBLE PRECISION INB( ARD__MXDIM ) ! Position B without offset
      DOUBLE PRECISION OFFV              ! Offset term
      DOUBLE PRECISION SIDE              ! Distance between window corners
      DOUBLE PRECISION OUTA( ARD__MXDIM )! Position A with offset
      DOUBLE PRECISION OUTB( ARD__MXDIM )! Position B with offset
      INTEGER I                          ! Axis index
      INTEGER IFRM                       ! Index of ARDAPP Frame
      INTEGER FR                         ! Pointer to a Frame
      INTEGER M1                         ! WinMap
      INTEGER NDIM               ! No. of axes in user coords

*.

*  Check the inherited status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Check the Curent Frame in the application FrameSet has domain ARDAPP.
      IF( AST_GETC( AWCS, 'DOMAIN', STATUS ) .NE. 'ARDAPP' ) THEN
         STATUS = ARD__INTER
         CALL MSG_SETC( 'D', AST_GETC( AWCS, 'DOMAIN', STATUS ) )
         CALL ERR_REP( 'ARD1_OFWCS_ERR1', 'ARD1_OFWCS: Current Frame '//
     :                 'in supplied application FrameSet has Domain '//
     :                 '''^D''. This should be ''ARDAPP'' '//
     :                 '(programming error).', STATUS )
         GO TO 999
      END IF

*  Save the number of axes in the current Frame
      NDIM = AST_GETI( AWCS, 'NAXES', STATUS )

*  Locate a Frame in the current User FrameSet with Domain ARDAPP.
      IFRM = AST__NOFRAME
      DO I = 1, AST_GETI( UWCS, 'NFRAME', STATUS )
         FR = AST_GETFRAME( UWCS, I, STATUS )
         DOM = AST_GETC( FR, 'DOMAIN', STATUS )
         CALL AST_ANNUL( FR, STATUS )
         IF( DOM .EQ. 'ARDAPP' ) THEN
            IFRM = I
            GO TO 10
         END IF
      END DO
 10   CONTINUE

*  If no Frame with Domain ARDAPP was found, annull the existing User
*  FrameSet and create a new one containing user and application coordinate
*  Frames connected by a UnitMap.
      IF( IFRM .EQ. AST__NOFRAME ) THEN
         CALL AST_ANNUL( UWCS, STATUS )
         CALL ARD1_COWCS( AWCS, AST__BAD, UWCS, STATUS )
         IFRM = AST_GETI( UWCS, 'BASE', STATUS )
      END IF

*  Create a WinMap from old ARDAPP coords to new ARDAPP coords.
      DO I = 1, NDIM
         OFFV = PAR( I )
         SIDE = MAX( ABS( OFFV ), 1.0D-6 )
         INA( I ) = 0.0
         INB( I ) = -SIDE
         OUTA( I ) = OFFV
         OUTB( I ) = OFFV - SIDE
      END DO

      M1 = AST_WINMAP( NDIM, INA, INB, OUTA, OUTB, ' ', STATUS )

*  Remap the ARDAPP Frame.
      CALL AST_REMAPFRAME( UWCS, IFRM, M1, STATUS )

*  Arrive here if an error occurs.
 999  CONTINUE

*  Annull AST objects.
      CALL AST_ANNUL( M1, STATUS )

      END
