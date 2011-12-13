      SUBROUTINE ARDSTAT( STATUS )
*+
*  Name:
*     ARDSTAT

*  Purpose:
*     Calculates the statistics of an NDF within shaped regions.

*  Language:
*     Starlink Fortran 77

*  Type of Module:
*     ADAM A-task

*  Invocation:
*     CALL ARDSTAT( STATUS )

*  Arguments:
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Description:
*     This program calculates the statistics of any data in a 2D NDF
*     that lie within an possibly regular or irregular shaped region.
*
*     Single regions can be given directly as a keyword such as CIRCLE
*     and then the associated radius. More complex regions are read from
*     a file that contains an ARD (SUN/183) description (these can be
*     generated by the ARDGEN or GAIA applications). Note that the
*     calculation is for the data within the regions. If requested
*     and the NDF has a variance component the statistics of that will
*     be reported too.

*  Usage:
*     ARDSTAT IN ARDIN

*  ADAM Parameters:
*     ANGLE = _REAL (Read)
*        Position angle of ellipse or rotbox region.
*     FULL = _LOGICAL (Read)
*        If true then stats of a variance component will be reported too.
*     IN = IMAGE (Read)
*        Input image data set.
*     ONELINE = _LOGICAL (Read)
*        If TRUE then the statistics are reported in one line,
*        rather than one-per line with explanatory text. In this
*        case the order of the results is:
*
*           Mean, Standard Dev, Max, Min, Sum, N pixels
*
*     RADIUS = _REAL (Read)
*        Radius of circle.
*     REGION = FILENAME (Read)
*        ARD description (used when SIMPLE is FALSE). To give
*        a file use the indirection character "^". I.e.
*           ^regions.ard
*        will read in the ARD description in the file "regions.ard".
*     SEMIMAJOR = _REAL (Read)
*        Semimajor axis of ellipse.
*     SEMIMINOR = _REAL (Read)
*        Semiminor axis of ellipse.
*     SHAPE
*        The shape of the region you want statistics for. Must be one of
*        BOX, CIRCLE, ELLIPSE, POLYGON, RECT or ROTBOX.  This is only
*        used when SIMPLE is TRUE.
*     SIDE1 = _REAL (Read)
*        Length of first side of rectangle or rotbox.
*     SIDE2 = _REAL (Read)
*        Length of second side of rectangle or rotbox.
*     SIMPLE = _LOGICAL (Read)
*        If TRUE then a single region is to be used. You define
*        these by specify a shape and any other information (positions,
*        lengths, angles) required to define it.
*        If FALSE then an ARD description is requested using the REGION
*        parameter (this can be stored in a file).
*     VERTICES = _REAL (Read)
*        List of X and Y positions of the vertices of a polygon.
*     XCENTRE = _REAL (Read)
*        X centre of a region.
*     XRANGE = _REAL (Read)
*        Lower and upper X range for a rectangle.
*     YCENTRE = _REAL (Read)
*        Y centre of a region.
*     YRANGE = _REAL (Read)
*        Lower and upper Y range for a rectangle.
*

*  Implementation Deficiencies:
*     - Just uses pixel coordinates (no axes support).

*  Authors:
*     PWD: Peter Draper (STARLINK - Durham University)
*     {enter_new_authors_here}

*  Copyright:
*     Copyright (C) 1996-2005 Central Laboratory of the Research Councils.
*     Copyright (C) 2006 Particle Physics & Astronomy Research Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of the
*     License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied warranty
*     of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

*  History:
*     19-MAR-1996 (PWD):
*        Original version. Just for GAIA not general enough
*        for other purposes.
*     28-APR-2004 (PWD):
*        Added FULL parameter.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE             ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'         ! Standard SAE constants
      INCLUDE 'NDF_PAR'         ! NDF constants
      INCLUDE 'GRP_PAR'         ! GRP constants

*  Status:
      INTEGER STATUS            ! Global status

*  Local variables:
      CHARACTER * ( 7 ) SHAPE   ! Shape of region
      CHARACTER * ( NDF__SZTYP ) TYPE ! NDF data type
      INTEGER DIMS( 2 )         ! Dimensions of input data
      INTEGER EL                ! Number of elements in input data
      INTEGER IDIN              ! Input NDF Identifier
      INTEGER IGRP              ! ARD description
      INTEGER IPIN              ! Pointer to input data
      INTEGER LBND( 2 )         ! Lower bounds of input data
      INTEGER NDIM              ! Number of dimensions
      INTEGER UBND( 2 )         ! Upper bounds of input data
      LOGICAL BAD               ! Whether input data has BAD pixels
      LOGICAL EXISTS            ! Whether variance component exists
      LOGICAL FIXORI            ! Whether to set origin to 1,1.
      LOGICAL FULL              ! Whether to report variance component stats
      LOGICAL ONELIN            ! Whether results are on one line.
      LOGICAL SIMPLE            ! Whether complex ARD descriptions are used
*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Get the input NDF.
      CALL NDF_BEGIN
      CALL NDF_ASSOC( 'IN', 'READ', IDIN, STATUS )

*  Obtain the data the data type.
      CALL NDF_TYPE( IDIN, 'DATA', TYPE, STATUS )

*  Map the data component and get the size.
      CALL NDF_MAP( IDIN, 'DATA', TYPE, 'READ', IPIN, EL, STATUS )
      CALL NDF_BOUND( IDIN, 2, LBND, UBND, NDIM, STATUS )
      DIMS( 1 ) = UBND( 1 ) - LBND( 1 ) + 1
      DIMS( 2 ) = UBND( 2 ) - LBND( 2 ) + 1

*  See if we should ignore the origin information (GAIA doesn't handle
*  this).
      CALL PAR_GET0L( 'FIXORIGIN', FIXORI, STATUS )
      IF ( FIXORI ) THEN
         LBND( 1 ) = 1
         LBND( 2 ) = 1
         UBND( 1 ) = DIMS( 1 )
         UBND( 2 ) = DIMS( 2 )
      END IF

*  Does data have BAD pixels?
      CALL NDF_BAD( IDIN, 'DATA', .FALSE., BAD, STATUS )

*  Find out how the user wants to specify the regions to use. This is
*  done either by using a single ARD keyword and then getting the
*  necessary information, or by just supplying the full ARD description
*  (which may of course in read in from a file using the ^ operator).
      CALL PAR_GET0L( 'SIMPLE', SIMPLE, STATUS )
      IF ( .NOT. SIMPLE ) THEN

*  Get the ARD descriptions that define the part of the image to
*  process.
         CALL ARD_GROUP( 'REGION', GRP__NOID, IGRP, STATUS )
      ELSE

*  User supplies shape and its coordinates from a named list.
         CALL PAR_CHOIC( 'SHAPE', 'CIRCLE',
     :                   'BOX,CIRCLE,ELLIPSE,POLYGON,RECT,ROTBOX',
     :                   .FALSE., SHAPE, STATUS )

*  Now convert this information (and also get any addition information
*  such as the position of the centres radii etc.) into the ARD
*  description.
         CALL RTD1_PRSH2( SHAPE, IGRP, STATUS )
      END IF

*  Just output the results in a single line?
      CALL PAR_GET0L( 'ONELINE', ONELIN, STATUS )

*  And calculate the required statistics.
      IF ( STATUS .EQ. SAI__OK ) THEN
         CALL RTD1_ARDST( BAD, ONELIN, 'Data', .TRUE., IGRP, DIMS( 1 ),
     :                    DIMS( 2 ), LBND, UBND, TYPE, IPIN, STATUS )
      END IF

*  If FULL then also do this for the variance component.
      CALL NDF_STATE( IDIN, 'VARIANCE', EXISTS, STATUS )
      IF ( EXISTS ) THEN
         CALL PAR_GET0L( 'FULL', FULL, STATUS )
         IF ( FULL ) THEN
            CALL NDF_MAP( IDIN, 'VARIANCE', TYPE, 'READ', IPIN, EL,
     :                    STATUS )
            CALL NDF_BAD( IDIN, 'VARIANCE', .FALSE., BAD, STATUS )
            IF ( STATUS .EQ. SAI__OK ) THEN
               CALL RTD1_ARDST( BAD, ONELIN, 'Var', .FALSE., IGRP,
     :                          DIMS( 1 ), DIMS( 2 ), LBND, UBND,
     :                          TYPE, IPIN, STATUS )
            END IF
         END IF
      END IF

*  Release all NDFs.
      CALL NDF_END( STATUS )

*  Release the ARD description.
      CALL GRP_DELET( IGRP, STATUS )

*  If an error occurred, then report a contextual message.
      IF ( STATUS .NE. SAI__OK ) THEN
          CALL ERR_REP( 'ARDSTAT_ERR',
     :    'ARDSTAT: Error calculating region statistics.', STATUS )
      END IF

      END
