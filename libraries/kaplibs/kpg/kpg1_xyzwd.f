      SUBROUTINE KPG1_XYZWD( DIM1, DIM2, ARRAY, XAXIS, YAXIS, BAD,
     :                         VARWTS, VAR, SIZE, X, Y, Z, W, NGOOD,
     :                         XMIN, XMAX, YMIN, YMAX, STATUS )
*+
*  Name:
*     KPG1_XYZWx
 
*  Purpose:
*     Converts a 2-dimensional array into a list of x-y co-ordinates,
*     values and weights.
 
*  Language:
*     Starlink Fortran 77
 
*  Invocation:
*     CALL KPG1_XYZWx( DIM1, DIM2, ARRAY, XAXIS, YAXIS, BAD, VARWTS,
*                      VAR, SIZE, X, Y, Z, W, NGOOD, XMIN, XMAX, YMIN,
*                      YMAX, STATUS )
 
*  Description:
*     This routine converts a 2-dimensional array into a list of x-y
*     coordinates, data values and weights for each of the good pixels
*     in the array.  The x-y co-ordinates come from the axis arrays
*     supplied.  Bad pixels are ignored if BAD is .TRUE..  If VARWTS is
*     .TRUE., weights are calculated from the reciprocal of the
*     variance for each pixel, otherwise the weights are returned 1.0.
 
*  Arguments:
*     DIM1 = INTEGER (Given)
*        The first dimension of the 2-dimensional array.
*     DIM2 = INTEGER (Given)
*        The second dimension of the 2-dimensional array.
*     ARRAY( DIM1, DIM2 ) = ? (Given)
*        The input data array.
*     XAXIS( DIM1 ) = DOUBLE PRECISION (Given)
*        X axis co-ordinates for the input data array.
*     YAXIS( DIM2 ) = DOUBLE PRECISION (Given)
*        Y axis co-ordinates for the input data array.
*     BAD = LOGICAL (Given)
*        Flag indicating whether bad values are likely to be present.
*     VARWTS = LOGICAL (Given)
*        Flag indicating if the variance array contains valid data.
*     VAR( DIM1, DIM2 ) = ? (Given)
*        An optional array containing the variance of the values
*        in the input data array, used to generate a weight for each
*        element.  It is only used if VARWTS is .TRUE..
*     SIZE = INTEGER (Given)
*        The total number of pixels.
*     X( SIZE ) = DOUBLE PRECISION (Returned)
*        The mean x positions for each pixel
*     Y( SIZE ) = DOUBLE PRECISION (Returned)
*        The mean y positions for each pixel.
*     Z( SIZE ) = DOUBLE PRECISION (Returned)
*        The data value for each pixel.
*     W( SIZE ) = DOUBLE PRECISION (Returned)
*        The weight for each pixel.
*     NGOOD = INTEGER (Returned)
*        The number of good pixels.
*     XMIN = DOUBLE PRECISION (Returned)
*        Minimum x co-ordinate.
*     XMAX = DOUBLE PRECISION (Returned)
*        Maximum x co-ordinate.
*     YMIN = DOUBLE PRECISION (Returned)
*        Minimum y co-ordinate.
*     YMAX = DOUBLE PRECISION (Returned)
*        Maximum y co-ordinate.
*     STATUS = INTEGER (Given and Returned)
*        Global status value.
 
*  Notes:
*     -  There is a routine for each numeric data type: replace "x" in
*     the routine name by D, R, I, W, UW, B or UB as appropriate.  The
*     ARRAY and VAR arrays supplied to the routine must have the data
*     type specified.
*     -  Uses the magic-value method for bad or undefined pixels.
 
*  Authors:
*     SMB: Steven M Beard (ROE)
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}
 
*  History:
*     21-Apr-1993: (SMB)
*        Original version.
*     1995 August 2 (MJC):
*        Converted to generic.  Used a modern-style prologue and coding.
*        Used double-precision axis co-ordinates.
*     {enter_further_changes_here}
 
*  Bugs:
*     {note_new_bugs_here}
 
*-
 
*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing
 
*  Global Constants:
      INCLUDE 'SAE_PAR'          ! SSE global definitions
      INCLUDE 'PRM_PAR'          ! Magic-value definitions
 
*  Arguments Given:
      INTEGER DIM1
      INTEGER DIM2
      DOUBLE PRECISION ARRAY( DIM1, DIM2 )
      DOUBLE PRECISION XAXIS( DIM1 )
      DOUBLE PRECISION YAXIS( DIM2 )
      LOGICAL BAD
      LOGICAL VARWTS
      DOUBLE PRECISION VAR( DIM1, DIM2 )
      INTEGER SIZE
 
*  Arguments Returned:
      DOUBLE PRECISION X( SIZE )
      DOUBLE PRECISION Y( SIZE )
      DOUBLE PRECISION Z( SIZE )
      DOUBLE PRECISION W( SIZE )
      INTEGER NGOOD
      DOUBLE PRECISION XMIN
      DOUBLE PRECISION XMAX
      DOUBLE PRECISION YMIN
      DOUBLE PRECISION YMAX
 
*  Status:
      INTEGER STATUS             ! Global status
 
*  Local constants:
      DOUBLE PRECISION WTOLER    ! Tolerance to prevent extremely large weights
      PARAMETER ( WTOLER = 1.0D-7 ) ! Maximum weight 10 million
 
*  Local variables:
      INTEGER I                  ! Loop counter
      INTEGER IX                 ! Loop counter
      INTEGER IY                 ! Loop counter
      DOUBLE PRECISION WMAX      ! Maximum weight found
 
*  Internal References:
      INCLUDE 'NUM_DEC_CVT'    ! NUM declarations for conversions
      INCLUDE 'NUM_DEF_CVT'    ! NUM definitions for conversions
 
*.
 
*  Check the inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN
 
*  Check whether there are likely to be bad values present.  If there
*  are none, used the faster algorithm without any checks.
      IF ( BAD ) THEN
 
*  Bad pixels present - rigorous algorithm.
 
*  Initialise the number of good pixels and the maximum weight, and
*  scan through each pixel in the array in turn.
         NGOOD = 0
         WMAX = 0.0D0
 
         DO IY = 1, DIM2
 
            DO IX = 1, DIM1
 
*  Ensure this is a good pixel. Bad ones are ignored.
               IF ( ARRAY( IX, IY ) .NE. VAL__BADD ) THEN
 
*  Increment the number of good pixels and load up the x, y, and z
*  arrays. (Note that the input arrays are DOUBLE PRECISION but the output
*  arrays are DOUBLE PRECISION, so a conversion is needed).
                  NGOOD = NGOOD + 1
                  X( NGOOD ) = XAXIS( IX )
                  Y( NGOOD ) = YAXIS( IY )
                  Z( NGOOD ) = NUM_DTOD( ARRAY( IX, IY ) )
 
*  If there is a valid variance array then use this to determine a
*  weight for this pixel.
                  IF ( VARWTS ) THEN
 
*  Check the variance is not bad.
                     IF ( VAR( IX, IY ) .NE. VAL__BADD ) THEN
 
*  Set the weight to the reciprocal of the variance.  Use a tolerance
*  to prevent a divide by zero and prevent extremely large weights.
                        W( NGOOD ) = 1.0D0 / MAX( WTOLER,
     :                               NUM_DTOD( VAR( IX, IY ) ) )
                     ELSE
 
*  The variance is bad. Assume it is infinite and set the weight to
*  zero.
                        W( NGOOD ) = 0.0D0
                     END IF
 
*  Keep track of the maximum weight so far.
                     WMAX = MAX( WMAX, W( NGOOD ) )
                  ELSE
 
*  There is no variance.  Set all weights to 1.0D0
                     W( NGOOD ) = 1.0D0
                  END IF
               END IF
            END DO
         END DO
 
*  If variances have been used, and the maximum weight is greater than
*  the tolerance (to avoid a divide by zero), normalise all the weights
*  to the range 0.0-1.0.
         IF ( VARWTS .AND. ( WMAX .GT. WTOLER ) ) THEN
            DO I = 1, NGOOD
               W( I ) = W( I ) / WMAX
            END DO
         END IF
      ELSE
 
*  No bad pixels present - fast alogorithm.
 
*  Initialise the number of good pixels and the maximum weight, and
*  scan through each pixel in the array in turn.
         NGOOD = 0
         WMAX = 0.0D0
 
         DO IY = 1, DIM2
            DO IX = 1, DIM1
 
*  Increment the number of good pixels and load up the x, y, and z
*  arrays. (Note that the input arrays are DOUBLE PRECISION but the output
*  arrays are DOUBLE PRECISION, so a conversion is needed).
               NGOOD = NGOOD + 1
               X( NGOOD ) = XAXIS( IX )
               Y( NGOOD ) = YAXIS( IY )
               Z( NGOOD ) = NUM_DTOD( ARRAY( IX, IY ) )
 
*  If there is a valid variance array then use this to determine a
*  weight for this pixel.
               IF ( VARWTS ) THEN
 
*  Set the weight to the reciprocal of the variance.  Use a tolerance
*  to prevent a divide by zero and prevent extremely large weights.
                  W( NGOOD ) = 1.0D0 / MAX( WTOLER,
     :                         NUM_DTOD( VAR( IX, IY ) ) )
 
*  Keep track of the maximum weight so far.
                  WMAX = MAX( WMAX, W( NGOOD ) )
               ELSE
 
*  There is no variance. Set all weights to 1.0D0
                  W( NGOOD ) = 1.0D0
               END IF
            END DO
         END DO
 
*  If variances have been used, and the maximum weight is greater than
*  the tolerance (to avoid a divide by zero), normalise all the weights
*  to the range 0.0-1.0.
         IF ( VARWTS .AND. ( WMAX .GT. WTOLER ) ) THEN
            DO I = 1, NGOOD
               W( I ) = W( I ) / WMAX
            END DO
         END IF
      END IF
 
*  Assume the upper and lower boundaries of the x and y co-ordinates
*  are the same as the extremes of the axis values.  This is not
*  perfect, an entire row or column at the edge could have been made of
*  bad pixels, but it saves searching through the entire data set
*  again.  Note that the following allows for axes running in either
*  direction.
      XMIN = MIN( XAXIS( 1 ), XAXIS( DIM1 ) )
      XMAX = MAX( XAXIS( 1 ), XAXIS( DIM1 ) )
      YMIN = MIN( YAXIS( 1 ), YAXIS( DIM2 ) )
      YMAX = MAX( YAXIS( 1 ), YAXIS( DIM2 ) )
 
      END
