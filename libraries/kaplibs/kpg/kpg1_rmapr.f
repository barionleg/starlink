      SUBROUTINE KPG1_RMAPR( EL, INARR, VALMAX, VALMIN, NUMBIN, MAP,
     :                         OUTARR, STATUS )
*+
*  Name:
*     KPG1_RMAPx
 
*  Purpose:
*     Remaps an array's values according to an histogram key.
 
*  Language:
*     Starlink Fortran 77
 
*  Invocation:
*     CALL KPG1_RMAPx( EL, INARR, VALMAX, VALMIN, NUMBIN, MAP, OUTARR,
*                      STATUS )
 
*  Description:
*     This routine remaps all the values in an array onto new values,
*     according to a key.  The key locates element values with
*     respect to an histogram that has been transformed from a linear
*     one, i.e. has equal-sized bins.  For example, in histogram
*     equalisation the bin width varies so as to give approximately
*     equal numbers of values in each bin.
 
*  Arguments:
*     EL = INTEGER (Given)
*        The dimension of the arrays.
*     INARR( EL ) = ? (Given)
*        The array to be remapped.
*     VALMAX = ? (Given)
*        Maximum value data value used to form the linear histogram.
*        Data values greater than this will be set bad in the output
*        array.
*     VALMIN = ? (Given)
*        Minimum value data value used to form the linear histogram.
*        Data values less than this will be set bad in the output
*        array.
*     NUMBIN = INTEGER (Given)
*        Number of bins used in histogram.  A moderately large number of
*        bins is recommended so that there is little artifactual
*        quantisation is introduced, say a few thousand except for
*        byte data.
*     MAP( NUMBIN ) = INTEGER (Given)
*        The key to the transform of the linear histogram.  MAP stores
*        the new bin number for each of the linear bin numbers.  A bin
*        number in the linear histogram is defined by the fractional
*        position between the minimum and maximum data values times the
*        number of bins.
*     OUTARR( EL ) = ? (Returned)
*        The array containing the remapped values.
*     STATUS = INTEGER  (Given)
*        Global status value.
 
*  Notes:
*     -  There is a routine for each numeric data type: replace "x" in
*     the routine name by D, R, I, W, UW, B or UB as appropriate. The
*     arrays and the histogram limits supplied to this routine must
*     have the data type specified.
 
*  Algorithm:
*     -  Define the scale factor for deriving the linear histogram
*     taking care to prevent overflows and underflows.
*     -  For each array element, copy input bad data to the output
*     array, or make bad in the output array any values in the input
*     array that exceed the histogram range.  Otherwise find the
*     bin number in the linear histogram, hence derive its new bin
*     after the transformation, and set the output array value to
*     the corresponding bin value.
 
*  References:
*     Gonzalez, R.C. and Wintz, P., 1977, "Digital Image Processing",
*     Addison-Wesley, pp. 118--126.
 
*  Authors:
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}
 
*  History:
*     1991 November 8 (MJC):
*        Original version based on REMAP.
*     1995 February 21 (MJC):
*        Standardised comment alignment, and sorted the variables.
*     1996 July 3 (MJC):
*        Made to cope with unsigned integer types.
*     {enter_further_changes_here}
 
*  Bugs:
*     {note_new_bugs_here}
 
*-
 
*  Type Definitions:
      IMPLICIT  NONE             ! No default typing allowed
 
*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Global SSE definitions
      INCLUDE 'PRM_PAR'          ! Magic-value constants
 
*  Arguments Given:
      INTEGER EL
      REAL INARR( EL )
      REAL VALMAX
      REAL VALMIN
      INTEGER NUMBIN
      INTEGER MAP( NUMBIN )
 
*  Arguments Returned:
      REAL OUTARR( EL )
 
*  Status:
      INTEGER  STATUS            ! Global status
 
*  Local Variables:
      DOUBLE PRECISION DLOWER    ! Lower limit of histogram
      DOUBLE PRECISION DLRG      ! Maximum difference between the limits
      DOUBLE PRECISION DSML      ! Minimum difference between the limits
      DOUBLE PRECISION DUPPER    ! Upper limit of histogram
      DOUBLE PRECISION DVMAX     ! Maximum data value
      DOUBLE PRECISION DVMIN     ! Minimum data value
      INTEGER I                  ! Counters
      INTEGER NEWBIN             ! Array index to the new transformed
                                 ! histogram bin
      INTEGER OLDBIN             ! Array index to the old linear
                                 ! histogram bin
      DOUBLE PRECISION RNUMB     ! Number of histogram bins
      DOUBLE PRECISION SCALE     ! Scale factor used for calculating
                                 ! the bin number
      DOUBLE PRECISION TEMP      ! Work variable
 
*  Internal References:
      INCLUDE 'NUM_DEC_CVT'      ! NUM declarations for conversions
      INCLUDE 'NUM_DEC_R'      ! NUM declarations for functions
      INCLUDE 'NUM_DEF_CVT'      ! NUM definitions for conversions
      INCLUDE 'NUM_DEF_R'      ! NUM definitions for functions
 
*.
 
*  Check the inherited status.
      IF ( STATUS .NE. SAI__OK ) RETURN
 
*  Define a commonly used variable.
      RNUMB = DBLE( NUMBIN )
 
*  Convert the smallest and largest value, and the histogram range to
*  double precision for later testing.  A half factor prevents
*  overflows.  Also make double-precision copies of the extreme values
*  in the histogram for subsequent tests to avoid problems with
*  unsigned integer data types.
      DSML = NUM_RTOD( VAL__SMLR )
      DLRG = NUM_RTOD( VAL__MAXR )
      DVMIN = NUM_RTOD( VALMIN )
      DVMAX = NUM_RTOD( VALMAX )
      DLOWER = 0.5D0 * DVMIN
      DUPPER = 0.5D0 * DVMAX
 
*  Set the scaling factor, watching for too small a value.  Report an
*  error when the scaling could not be determined.
      IF ( ABS( DUPPER - DLOWER ) .GT. DSML .AND.
     :     ABS( DUPPER - DLOWER ) .LT. DLRG ) THEN
         SCALE = 2.0D0 * ( DUPPER - DLOWER )
      ELSE
 
*  There are no MSG_SETX routines for the one- and two-byte data types.
*  Therefore a messy section of code follows
         STATUS = SAI__ERROR
         IF ( 'R' .EQ. 'I' .OR. 'R' .EQ. 'B' .OR. 'R' .EQ. 'UB'
     :       .OR. 'R' .EQ. 'W' .OR. 'R' .EQ. 'UW' ) THEN
            CALL MSG_SETI( 'MIN', NUM_RTOI( VALMIN ) )
            CALL MSG_SETI( 'MAX', NUM_RTOI( VALMAX ) )
         ELSE IF ( 'R' .EQ. 'R' ) THEN
            CALL MSG_SETR( 'MIN', VALMIN )
            CALL MSG_SETR( 'MAX', VALMAX )
         ELSE IF ( 'R' .EQ. 'D' ) THEN
            CALL MSG_SETD( 'MIN', VALMIN )
            CALL MSG_SETD( 'MAX', VALMAX )
         END IF
         CALL ERR_REP( 'KPG1_RMAPx_RANGE',
     :     'Unable to remap the data.  The histogram range (^MIN '/
     :     /'to ^MAX) is too narrow or too wide.', STATUS )
         GOTO 999
      END IF
 
*  Now perform the transform of the values.
      DO I = 1, EL
 
*  Make a double-precision copy of the data value.
         TEMP = NUM_RTOD( INARR( I ) )
 
*  Test for a valid element.
         IF ( INARR( I ) .EQ. VAL__BADR ) THEN
 
*  Propagate bad data to the output array.
            OUTARR( I ) = INARR( I )
 
         ELSE IF ( TEMP .LT. DVMIN .OR. TEMP .GT. DVMAX ) THEN
 
*  Values outside the remapping range are undefined and so become
*  invalid.
            OUTARR( I ) = VAL__BADR
 
         ELSE
 
*  Find the bin number in the linear histogram for the particular
*  value.
            OLDBIN = INT( ( TEMP - DVMIN ) / SCALE * RNUMB ) + 1
 
*  Check to see that the bin is within range.
            OLDBIN = MIN( NUMBIN, MAX( 1, OLDBIN ) )
 
*  Locate the new bin from the lookup table of keys.
            NEWBIN = MAP( OLDBIN )
 
*  Replace the array value with the value associated with the
*  transformed bin.
            OUTARR( I ) = NUM_DTOR( ( ( DBLE( NEWBIN ) - 0.5 )
     :                    / RNUMB ) * SCALE + DVMIN )
 
         END IF
      END DO
 
  999 CONTINUE
 
      END
