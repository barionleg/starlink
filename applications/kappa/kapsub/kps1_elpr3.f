      SUBROUTINE KPS1_ELPR3( VAR, ILO, IHI, JLO, JHI, DATIN, VARIN,
     :                       IGRP, REGIND, NBIN, WMEAN, REGVAL, DATOUT,
     :                       VAROUT, NSUM, MASK, STATUS )
*+
*  Name:
*     KPS1_ELPR3

*  Purpose:
*     Finds the integrated data values within ARD regions.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL KPS1_ELPR3( VAR, ILO, IHI, JLO, JHI, DATIN, VARIN, IGRP,
*                      REGIND, NBIN, WMEAN, REGVAL, DATOUT, VAROUT,
*                      NSUM, MASK, STATUS )

*  Description:
*     This subroutine finds the mean (or weighted mean) and variance of the
*     data values within specified sub-regions of the supplied data array,
*     excluding any bad data pixels.  The regions are specified by
*     means of an "ARD description" (see SUN/183).  The pixel mask
*     corresponding to the ARD description is returned, modified so
*     that regions are identified by their bin numbers rather than the
*     arbitrary values returned by ARD_WORK.

*  Arguments:
*     VAR = LOGICAL (Given)
*        It is .TRUE. if input variances are available.
*     ILO = INTEGER (Given)
*        The lower bound of the first axis.
*     IHI = INTEGER (Given)
*        The upper bound of the first axis.
*     JLO = INTEGER (Given)
*        The lower bound of the second axis.
*     JHI = INTEGER (Given)
*        The upper bound of the second axis.
*     DATIN( ILO:IHI, JLO:JHI ) = REAL (Given)
*        The input data array.
*     VARIN( ILO:IHI, JLO:JHI ) = REAL (Given)
*        The input variance array.  It is only used if VAR is TRUE.
*     IGRP = INTEGER (Given)
*        The identifier for a GRP group containing the ARD description.
*     REGIND( * ) = INTEGER (Given)
*        The bin number for each region.  The regions are identified by
*        their ARD_WORK mask value, which is used as the index into the
*        REGIND array.
*     NBIN = INTEGER (Given)
*        The number of bins.
*     WMEAN = LOGICAL (Given)
*        If TRUE, then return the weighted mean values. Otherwise return
*        the simple mean values.
*     REGVAL = INTEGER (Given and Returned)
*        The integer value to use to represent the first region in the
*        ARD description.  It should be no less than 2.  It is returned
*        set to one more than the largest value used in the mask.
*     DATOUT( NBIN ) = REAL (Returned)
*        The mean data value in each bin.  It is set to VAL__BADR if a
*        bin has no data.
*     VAROUT( NBIN ) = REAL (Returned)
*        The variance of each bin.  It is set to VAL__BADR if a bin has
*        no data.  It is Only used if VAR is .TRUE.
*     NSUM( NBIN ) = INTEGER (Returned)
*        The number of data values in each bin.
*     MASK( ILO:IHI, JLO:JHI ) = INTEGER (Returned)
*        Work space to hold the pixel mask generated by ARD_WORK.  This
*        should have at least the number of elements implied by LBND
*        and UBND.  It is modified on returned so that each pixel holds
*        the corresponding bin number, rather than the raw ARD mask
*        value.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Copyright:
*     Copyright (C) 1995, 2002 Central Laboratory of the Research
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
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}

*  History:
*     27-JAN-1995 (DSB):
*        Original version.
*     1995 March 28 (MJC):
*        Minor stylistic and typographical changes, shortened long
*        lines, and used a modern-style variable declaration.
*     13-MAY-2002 (DSB):
*        Add the WMEAN argument.
*     {enter_further_changes_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PRM_PAR'          ! VAL__ constants

*  Arguments Given:
      LOGICAL VAR
      INTEGER ILO
      INTEGER IHI
      INTEGER JLO
      INTEGER JHI
      REAL    DATIN( ILO:IHI, JLO:JHI )
      REAL    VARIN( ILO:IHI, JLO:JHI )
      INTEGER IGRP
      INTEGER REGIND( * )
      INTEGER NBIN
      LOGICAL WMEAN

*  Arguments Given and Returned:
      INTEGER REGVAL

*  Arguments Returned:
      REAL DATOUT( NBIN )
      REAL VAROUT( NBIN )
      INTEGER NSUM( NBIN )
      INTEGER MASK( ILO:IHI, JLO:JHI )

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      LOGICAL ALLBAD                ! Does the output have any good
                                    ! values?
      INTEGER I                     ! Column count
      INTEGER IBIN                  ! Bin number
      INTEGER J                     ! Row count
      INTEGER LBND( 2 )             ! Low bounds of whole image
      INTEGER LBNDE( 2 )            ! Low bounds of exterior region
      INTEGER LBNDI( 2 )            ! Low bounds of interior region
      INTEGER MASKLO                ! Lowest usable mask value
      INTEGER UBND( 2 )             ! High bounds of whole image
      INTEGER UBNDE( 2 )            ! High bounds of exterior region
      INTEGER UBNDI( 2 )            ! High bounds of interior region
      REAL W                        ! The weight for this data value
*.

*  Check the inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Store the smallest mask value which could correspond to a pixel in
*  one of the required bins.
      MASKLO = REGVAL

*  Store the bounds of the image.
      LBND( 1 ) = ILO
      UBND( 1 ) = IHI
      LBND( 2 ) = JLO
      UBND( 2 ) = JHI

*  Create the pixel mask.  The first (i.e. left-hand) region in the ARD
*  description is assigned the mask value 2.
      CALL ARD_WORK( IGRP, 2, LBND, UBND, VAL__BADR, .FALSE., REGVAL,
     :               MASK, LBNDI, UBNDI, LBNDE, UBNDE, STATUS )

*  Set a flag indicating that no good output DATA values have yet been
*  created.
      ALLBAD = .TRUE.

*  First deal with the cases where input variances are available.
      IF ( VAR ) THEN

*  First do weighted mean...
         IF( WMEAN ) THEN

*  Initialise the bins. DATOUT initially holds the sum of the weighted
*  data values, and VAROUT holds the sum of the weights.
            DO IBIN = 1, NBIN
               DATOUT( IBIN ) = 0.0
               VAROUT( IBIN ) = 0.0
            END DO

*  Loop round every pixel in the region of the mask containing the
*  selected pixels.
            DO J = MAX( JLO, LBNDI( 2 ) ), MIN( JHI, UBNDI( 2 ) )
               DO I = MAX( ILO, LBNDI( 1 ) ), MIN( IHI, UBNDI( 1 ) )

*  Ignore pixels which are not in any of the required bins.
                  IF ( MASK( I, J ) .GE. MASKLO ) THEN

*  Find the profile bin number into which this pixel falls.
                     IBIN = REGIND( MASK( I, J ) )

*  Increment the weighted bin value and the sum of weights if the data
*  and variance values are both good.
                     IF ( DATIN( I, J ) .NE. VAL__BADR .AND.
     :                    VARIN( I, J ) .NE. VAL__BADR .AND.
     :                    VARIN( I, J ) .GT. 0.0 ) THEN

                        W = 1.0/VARIN( I, J )

                        DATOUT( IBIN ) = DATOUT( IBIN )
     :                                   + W*DATIN( I, J )
                        VAROUT( IBIN ) = VAROUT( IBIN ) + W
                     END IF

*  Replace the mask value with the bin index.
                     MASK( I, J ) = IBIN

*  Store zero in all non-used mask pixels.
                  ELSE
                     MASK( I, J ) = 0

                  END IF

               END DO
            END DO

*  Return weighted mean and variance values.  Set to bad values if there were
*  no values in the bin.
            DO IBIN = 1, NBIN
               IF ( VAROUT( IBIN ) .GT. 0.0 ) THEN
                  DATOUT( IBIN ) = DATOUT( IBIN ) / VAROUT( IBIN )
                  VAROUT( IBIN ) = 1.0/VAROUT( IBIN )
                  ALLBAD = .FALSE.
               ELSE
                  DATOUT( IBIN ) = VAL__BADR
                  VAROUT( IBIN ) = VAL__BADR
               END IF

            END DO

* Now do simple means...
         ELSE

*  Initialise the bins
            DO IBIN = 1, NBIN
               DATOUT( IBIN ) = 0.0
               VAROUT( IBIN ) = 0.0
               NSUM( IBIN ) = 0
            END DO

*  Loop round every pixel in the region of the mask containing the
*  selected pixels.
            DO J = MAX( JLO, LBNDI( 2 ) ), MIN( JHI, UBNDI( 2 ) )
               DO I = MAX( ILO, LBNDI( 1 ) ), MIN( IHI, UBNDI( 1 ) )

*  Ignore pixels which are not in any of the required bins.
                  IF ( MASK( I, J ) .GE. MASKLO ) THEN

*  Find the profile bin number into which this pixel falls.
                     IBIN = REGIND( MASK( I, J ) )

*  Increment the bin values if the data and variance values are both
*  good.
                     IF ( DATIN( I, J ) .NE. VAL__BADR .AND.
     :                    VARIN( I, J ) .NE. VAL__BADR ) THEN

                        DATOUT( IBIN ) = DATOUT( IBIN ) + DATIN( I, J )
                        VAROUT( IBIN ) = VAROUT( IBIN ) + VARIN( I, J )
                        NSUM( IBIN ) = NSUM( IBIN ) + 1
                     END IF

*  Replace the mask value with the bin index.
                     MASK( I, J ) = IBIN

*  Store zero in all non-used mask pixels.
                  ELSE
                     MASK( I, J ) = 0

                  END IF

               END DO
            END DO

*  Return mean and variance values.  Set to bad values if there were
*  no values in the bin.
            DO IBIN = 1, NBIN
               IF ( NSUM( IBIN ) .GT. 0 ) THEN
                  DATOUT( IBIN ) = DATOUT( IBIN ) / REAL( NSUM( IBIN ) )
                  VAROUT( IBIN ) = VAROUT( IBIN )
     :                             / REAL( NSUM( IBIN )**2 )
                  ALLBAD = .FALSE.
               ELSE
                  DATOUT( IBIN ) = VAL__BADR
                  VAROUT( IBIN ) = VAL__BADR
               END IF

            END DO

         END IF

*  Now deal with cases where input variances are not available.
      ELSE

*  Initialise the bins
         DO IBIN = 1, NBIN
            DATOUT( IBIN ) = 0.0
            NSUM( IBIN ) = 0
         END DO

*  Loop round every pixel in the region of the mask containing the
*  selected pixels.
         DO J = MAX( JLO, LBNDI( 2 ) ), MIN( JHI, UBNDI( 2 ) )
            DO I = MAX( ILO, LBNDI( 1 ) ), MIN( IHI, UBNDI( 1 ) )

*  Ignore pixels which are not in any of the required bins.
               IF ( MASK( I, J ) .GE. MASKLO ) THEN

*  Find the profile bin number into which this pixel falls.
                  IBIN = REGIND( MASK( I, J ) )

*  Increment the bin values if the data and variance values are both
*  good.
                  IF ( DATIN( I, J ) .NE. VAL__BADR ) THEN

                     DATOUT( IBIN ) = DATOUT( IBIN ) + DATIN( I, J )
                     NSUM( IBIN ) = NSUM( IBIN ) + 1
                  END IF

*  Replace the mask value with the bin index.
                  MASK( I, J ) = IBIN

*  Store zero in all non-used mask pixels.
               ELSE
                  MASK( I, J ) = 0

               END IF

            END DO
         END DO

*  Return the mean values.  Set to the bad value if there were
*  no values in the bin.
         DO IBIN = 1, NBIN
            IF ( NSUM( IBIN ) .GT. 0 ) THEN
               DATOUT( IBIN ) = DATOUT( IBIN ) / REAL( NSUM( IBIN ) )
               ALLBAD = .FALSE.
            ELSE
               DATOUT( IBIN ) = VAL__BADR
            END IF

         END DO

      END IF

*  Report an error if the output would have no good data in it.
      IF ( ALLBAD .AND. STATUS .EQ. SAI__OK ) THEN
         STATUS = SAI__ERROR
         CALL ERR_REP( 'KPS1_ELPR3_ERR', 'The output profile contains '/
     :     /'no good data (maybe no bins overlapped the input image, '/
     :     /'or maybe there were no good data in the input image).',
     :     STATUS )
      END IF

      END
