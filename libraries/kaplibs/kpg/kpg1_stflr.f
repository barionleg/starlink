      SUBROUTINE KPG1_STFLR( NDIM, EL, NGOOD, DMIN, MINP, MINC,
     :                         DMAX, MAXP, MAXC, SUM, MEAN, STDEV,
     :                         MEDIAN, MODE, NUMPER, PERCNT, PERVAL,
     :                         IFIL, STATUS )
*+
*  Name:
*     KPG1_STFLx
 
*  Purpose:
*     Write statistics generated by KPG1_STATx and KPG1_HSTAx to a
*     text file.
 
*  Language:
*     Starlink Fortran 77.
 
*  Invocation:
*     CALL KPG1_STFLx( NDIM, EL, NGOOD, DMIN, MINP, MINC,
*                      DMAX, MAXP, MAXC, SUM, MEAN, STDEV,
*                      MEDIAN, MODE, NUMPER, PERCNT, PERVAL,
*                      IFIL, STATUS )
 
*  Description:
*     This routine writes the statistics of pixel data as generated by
*     the routines KPG1_STATx and KPG1_HSTAx to a text file.
 
*  Arguments:
*     NDIM = INTEGER (Given)
*        Number of dimensions of the array whose statistics are being
*        displayed.
*     EL = INTEGER (Given)
*        Total number of pixels in the array.
*     NGOOD = INTEGER (Given)
*        Number of valid pixels which contributed to the statistics.
*     DMIN = DOUBLE PRECISION (Given)
*        Minimum pixel value.
*     MINP( NDIM ) = INTEGER (Given)
*        Pixel indices at which the minimum pixel value was (first)
*        found.
*     MINC( NDIM ) = DOUBLE PRECISION (Given)
*        The co-ordinate values of the centre of the minimum pixel.
*     DMAX = DOUBLE PRECISION (Given)
*        Maximum pixel value.
*     MAXP( NDIM ) = INTEGER (Given)
*        Pixel indices at which the maximum pixel value was (first)
*        found.
*     MAXC( NDIM ) = DOUBLE PRECISION (Given)
*        The co-ordinate values of the centre of the maximum pixel.
*     SUM = DOUBLE PRECISION (Given)
*        Pixel sum.
*     MEAN = DOUBLE PRECISION (Given)
*        Pixel mean value.
*     STDEV = DOUBLE PRECISION (Given)
*        Pixel standard deviation value.
*     MEDIAN = DOUBLE PRECISION (Given)
*        Median value.
*     MODE = DOUBLE PRECISION (Given)
*        Modal value.
*     NUMPER = INTEGER (Given)
*        Number of percentiles values to report.
*     PERCNT( NUMPER ) = REAL (Given)
*        The array of percentiles levels corresponding to the values
*        given by PERVAL.  They should be in the range 0.0 to 100.0,
*        and preferably in ascending order.  If there are none to
*        report, set NUMPER to 1 and pass the bad value in PERCNT( 1 ).
*     PERVAL( NUMPER ) = DOUBLE PRECISION (Given)
*        Percentile values corresponding to the percentile fractions
*        in PERCNT.
*     IFIL = INTEGER (Given)
*        FIO_ file descriptor for the output file.
*     STATUS = INTEGER (Given and Returned)
*        The global status.
 
*  Notes:
*     -  There is a routine for both real and double-precision
*     statistics: replace "x" in the routine name by R or D as
*     appropriate. Note that the two routines have identical argument
*     lists (with floating-point qualitities in double precision) but
*     differ in the precision with which the results are displayed.
*     -  If the value of NGOOD is not positive, then this routine will
*     assume that all the derived statistics (except for the sum) are
*     undefined and will not display them.
*     -  If a statistic is undefined, i.e. has the bad value, it is
*     not reported.  In the case of arrays, the first value is checked.
*     For the co-ordinates and pixel indices of the extreme values both
*     of the first elements of these must be good to display these
*     positions.
 
*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK, RAL)
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}
 
*  History:
*     1991 March 28 (RFWS):
*        Original version, derived from KPG1_STDSx.
*     1996 March 11 (MJC):
*        Redo addition of the ordered statistics and checking for bad
*        values (originally c.1992).
*     {enter_changes_here}
 
*  Bugs:
*     {note_new_bugs_here}
 
*-
 
*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing
 
*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PRM_PAR'          ! VAL__ constants
 
*  Arguments Given:
      INTEGER NDIM
      INTEGER EL
      INTEGER NGOOD
      DOUBLE PRECISION DMIN
      INTEGER MINP( NDIM )
      DOUBLE PRECISION MINC( NDIM )
      DOUBLE PRECISION DMAX
      INTEGER MAXP( NDIM )
      DOUBLE PRECISION MAXC( NDIM )
      DOUBLE PRECISION SUM
      DOUBLE PRECISION MEAN
      DOUBLE PRECISION STDEV
      DOUBLE PRECISION MEDIAN
      DOUBLE PRECISION MODE
      INTEGER NUMPER
      REAL PERCNT( NUMPER )
      DOUBLE PRECISION PERVAL( NUMPER )
 
      INTEGER IFIL
 
*  Status:
      INTEGER  STATUS
 
*  Local Variables:
      CHARACTER * ( 5 ) PCENT    ! Buffer for formatting percentages
      CHARACTER * ( 80 ) BUF1    ! Text buffer
      CHARACTER * ( 80 ) BUF2    ! Text buffer
      INTEGER I                  ! Loop counter for dimensions
      INTEGER NC1                ! No. of characters in BUF1
      INTEGER NC2                ! No. of characters in BUF2
 
*  Internal References:
      INCLUDE 'NUM_DEC_CVT'      ! NUM_ type conversion functions
      INCLUDE 'NUM_DEF_CVT'
 
*.
 
*  Check the inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN
 
*  Display the pixel sum.
      CALL FIO_WRITE( IFIL, ' ', STATUS )
      NC1 = 0
      CALL CHR_PUTC( '         Pixel sum              : ', BUF1, NC1 )
      CALL CHR_PUTR( NUM_DTOR( SUM ), BUF1, NC1 )
      CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
 
*  Display the mean pixel value.
      IF ( NGOOD .GT. 0 ) THEN
         IF ( MEAN .NE. VAL__BADD ) THEN
            NC1 = 0
            CALL CHR_PUTC( '         Pixel mean             : ', BUF1,
     :                     NC1 )
            CALL CHR_PUTR( NUM_DTOR( MEAN ), BUF1, NC1 )
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
         END IF
 
*  Display the standard deviation.
         IF ( STDEV .NE. VAL__BADD ) THEN
            NC1 = 0
            CALL CHR_PUTC( '         Standard deviation     : ', BUF1,
     :                     NC1 )
            CALL CHR_PUTR( NUM_DTOR( STDEV ), BUF1, NC1 )
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
         END IF
 
*  Display the minimum value found.
         IF ( DMIN .NE. VAL__BADD ) THEN
            NC1 = 0
            CALL CHR_PUTC( '         Minimum pixel value    : ', BUF1,
     :                     NC1 )
            CALL CHR_PUTR( NUM_DTOR( DMIN ), BUF1, NC1 )
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
         END IF
 
*  Format the indices and co-ordinates of the centre of the minimum
*  pixel as character strings.
         IF ( MINP( 1 ) .NE. VAL__BADI .AND.
     :        MINC( 1 ) .NE. VAL__BADD ) THEN
            NC1 = 0
            NC2 = 0
            CALL CHR_PUTC( '            At pixel            : (', BUF1,
     :                     NC1 )
            CALL CHR_PUTC( '            Co-ordinate         : (', BUF2,
     :                     NC2 )
            DO 1 I = 1, NDIM
               IF ( I .GT. 1 ) THEN
                  CALL CHR_PUTC( ', ', BUF1, NC1 )
                  CALL CHR_PUTC( ', ', BUF2, NC2 )
               END IF
               CALL CHR_PUTI( MINP( I ), BUF1, NC1 )
               CALL CHR_PUTR( SNGL( MINC( I ) ), BUF2, NC2 )
    1       CONTINUE
            CALL CHR_PUTC( ')', BUF1, NC1 )
            CALL CHR_PUTC( ')', BUF2, NC2 )
 
*  Display the minimum pixel's indices and co-ordinates.
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
            CALL FIO_WRITE( IFIL, BUF2( : NC2 ), STATUS )
         END IF
 
*  Display the maximum pixel value found.
         IF ( DMAX .NE. VAL__BADD ) THEN
            NC1 = 0
            CALL CHR_PUTC( '         Maximum pixel value    : ', BUF1,
     :                     NC1 )
            CALL CHR_PUTR( NUM_DTOR( DMAX ), BUF1, NC1 )
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
         END IF
 
*  Format the indices and co-ordinates of the centre of the maximum
*  pixel as character strings.
         IF ( MAXP( 1 ) .NE. VAL__BADI .AND.
     :        MAXC( 1 ) .NE. VAL__BADD ) THEN
            NC1 = 0
            NC2 = 0
            CALL CHR_PUTC( '            At pixel            : (', BUF1,
     :                     NC1 )
            CALL CHR_PUTC( '            Co-ordinate         : (', BUF2,
     :                     NC2 )
            DO 2 I = 1, NDIM
               IF ( I .GT. 1 ) THEN
                  CALL CHR_PUTC( ', ', BUF1, NC1 )
                  CALL CHR_PUTC( ', ', BUF2, NC2 )
               END IF
               CALL CHR_PUTI( MAXP( I ), BUF1, NC1 )
               CALL CHR_PUTR( SNGL( MAXC( I ) ), BUF2, NC2 )
    2       CONTINUE
            CALL CHR_PUTC( ')', BUF1, NC1 )
            CALL CHR_PUTC( ')', BUF2, NC2 )
 
*  Display the maximum pixel's indices and co-ordinates.
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
            CALL FIO_WRITE( IFIL, BUF2( : NC2 ), STATUS )
         END IF
 
*  Display the median value.
         IF ( MEDIAN .NE. VAL__BADD ) THEN
            NC1 = 0
            CALL CHR_PUTC( '         Pixel median           : ', BUF1,
     :                     NC1 )
            CALL CHR_PUTR( NUM_DTOR( MEDIAN ), BUF1, NC1 )
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
         END IF
 
*  Display the mode.
         IF ( MODE .NE. VAL__BADD ) THEN
            NC1 = 0
            CALL CHR_PUTC( '         Pixel mode             : ', BUF1,
     :                     NC1 )
            CALL CHR_PUTR( NUM_DTOR( MODE ), BUF1, NC1 )
            CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
         END IF
 
*  Display the percentile values.  To retain alignment, use different
*  formatting for 100.0 percentile.
         IF ( PERCNT( 1 ) .NE. VAL__BADR ) THEN
            DO 3 I = 1, NUMPER
               IF ( PERVAL( I ) .NE. VAL__BADD ) THEN
                  NC1 = 0
                  CALL CHR_PUTC( '         Pixel ' , BUF1, NC1 )
 
                  IF ( PERCNT( I ) .GT. 99.995 ) THEN
                     CALL CHR_PUTC( '100.0', BUF1, NC1 )
                  ELSE
                     WRITE( PCENT, '(F5.2)' ) PERCNT( I )
                     CALL CHR_PUTC( PCENT, BUF1, NC1 )
                  END IF
                  CALL CHR_PUTC( ' percentile : ' , BUF1, NC1 )
 
                  CALL CHR_PUTR( NUM_DTOR( PERVAL( I ) ), BUF1,
     :                             NC1 )
                  CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
               END IF
    3       CONTINUE
         END IF
      END IF
 
*  Show how many pixels are in the NDF.
      NC1 = 0
      CALL CHR_PUTC( '         Total number of pixels : ', BUF1, NC1 )
      CALL CHR_PUTI( EL, BUF1, NC1 )
      CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
 
*  Show how many pixels contributed to the statistics.
      NC1 = 0
      CALL CHR_PUTC( '         Number of pixels used  : ', BUF1, NC1 )
      CALL CHR_PUTI( NGOOD, BUF1, NC1 )
      CALL CHR_PUTC( ' (', BUF1, NC1 )
      WRITE( PCENT, '(F5.1)' ) 100.0 * REAL( NGOOD ) / REAL( EL )
      CALL CHR_LDBLK( PCENT )
      CALL CHR_PUTC( PCENT, BUF1, NC1 )
      CALL CHR_PUTC( '%)', BUF1, NC1 )
      CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
 
*  Show how many pixels did not contribute.
      IF ( NGOOD .LT. EL ) THEN
         NC1 = 0
         CALL CHR_PUTC( '         No. of pixels excluded : ', BUF1,
     :                  NC1 )
         CALL CHR_PUTI( EL - NGOOD, BUF1, NC1 )
         CALL CHR_PUTC( ' (', BUF1, NC1 )
         WRITE( PCENT, '(F5.1)' )
     :          100.0 * REAL( EL - NGOOD ) / REAL( EL )
         CALL CHR_LDBLK( PCENT )
         CALL CHR_PUTC( PCENT, BUF1, NC1 )
         CALL CHR_PUTC( '%)', BUF1, NC1 )
         CALL FIO_WRITE( IFIL, BUF1( : NC1 ), STATUS )
      END IF
      CALL FIO_WRITE( IFIL, ' ', STATUS )
 
      END
