      SUBROUTINE KPG1_NUMBI( BAD, VABS, ABOVE, EL, INARR, VALUE,
     :                         NUMBER, STATUS )
*+
*  Name:
*     KPG1_NUMBx
 
*  Purpose:
*     Counts the number of elements with values or absolute values
*     above or below a limit.
 
*  Language:
*     Starlink Fortran 77
 
*  Invocation:
*     CALL KPG1_NUMBx( BAD, VABS, ABOVE, EL, INARR, VALUE, NUMBER,
*                      STATUS )
 
*  Description:
*     This routine returns the number of points in the input array
*     that have a value or absolute value greater than or less than
*     the input value.
 
*  Arguments:
*     BAD = LOGICAL (Given)
*        If true testing for bad pixels will be made.  This should not
*        be set to false unless there definitely no bad values within
*        the input array.
*     VABS = LOGICAL (Given)
*        If true the comparison is performed with the absolute value
*        of each array element.
*     ABOVE = LOGICAL (Given)
*        If true the criterion tests for array values greater than the
*        limit; if false the criterion tests for array values less than
*        the limit.
*     EL = INTEGER (Given)
*        The dimension of the input array.
*     INARR( EL ) = ? (Given)
*        The input data array
*     VALUE = ? (Given)
*        Value to test each array value against.
*     NUMBER = INTEGER (Returned)
*        The number of elements of the input array greater than the
*        specified value when VABS = FALSE, or the number of elements
*        of the input array whose absolute values are greater than the
*        specified value when VABS = TRUE.
*     STATUS = INTEGER (Given)
*        Global status value.
 
*  Notes:
*     -  There is a routine for each numeric data type: replace "x" in
*     the routine name by D, R, I, W, UW, B or UB as appropriate. The
*     array and comparison value supplied to the routine must have the
*     data type specified.
 
*  Algorithm:
*     -  The routine loops around the whole input array, incrementing
*     the value of NUMBER by one each time the current pixel has an
*     value greater than the specified VALUE.  There are separate
*     processing paths for the eight states of VABS, ABOVE and BAD in
*     combination.
 
*  Authors:
*     MJC: Malcolm Currie (STARLINK)
*     {enter_new_authors_here}
 
*  History:
*     1991 November 11 (MJC):
*        Original version.
*     1995 February 21 (MJC):
*        Standardised comment alignment, and sorted the variables.
*     {enter_further_changes_here}
 
*  Bugs:
*     {note_any_bugs_here}
 
*-
 
*  Type Definitions:
      IMPLICIT NONE             ! No default typing allowed
 
*  Global Constants:
      INCLUDE 'SAE_PAR'         ! Global environment parameters
      INCLUDE 'PRM_PAR'         ! Magic-value definitions
 
*  Arguemnts Given:
      LOGICAL BAD
      LOGICAL VABS
      LOGICAL ABOVE
      INTEGER EL
      INTEGER INARR( EL )
      INTEGER VALUE
 
*  Arguments Returned:
      INTEGER NUMBER
 
*  Status:
      INTEGER STATUS             ! Global status
 
*  Local Variables:
      INTEGER I                  ! Counter
 
*  Internal References:
      INCLUDE 'NUM_DEC_CVT'      ! Declarations of conversion routines
      INCLUDE 'NUM_DEC_I'      ! Declarations of other routines
      INCLUDE 'NUM_DEF_CVT'      ! Definitions of conversion routines
      INCLUDE 'NUM_DEF_I'      ! Definitions of other routines
 
*.
 
*  Check the inherited status.
      IF ( STATUS .NE. SAI__OK ) RETURN
 
*  Initialise the counter.
      NUMBER = 0
 
*  There 8 possibily processing paths depending on the values of the
*  three input logical arguments.
 
*  Test is greater than the limit.
*  ===============================
      IF ( ABOVE ) THEN
 
*  Compare absolute values.
*  ========================
         IF ( VABS ) THEN
 
            IF ( BAD ) THEN
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test for bad value first.
                  IF ( INARR( I ) .NE. VAL__BADI ) THEN
 
*  Test the absolute array value against input value and check the
*  element's validity. Note no ABS function for byte data so must call
*  NUM routine.  VAL is an order of magnitude slower.
                     IF ( NUM_ABSI( INARR( I ) ) .GT. VALUE ) THEN
 
*  If former is greater than the latter, increment the counter.
                        NUMBER = NUMBER + 1
                     END IF
                  END IF
               END DO
            ELSE
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test the absolute array value against input value.  Note that the
*  ABS function cannot be used for byte data.  If former is greater
*  than the latter, increment the counter.
                  IF ( NUM_ABSI( INARR( I ) ) .GT. VALUE )
     :               NUMBER = NUMBER + 1
               END DO
            END IF
 
*  Compare actual values
*  =====================
         ELSE
            IF ( BAD ) THEN
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test the array value against input value and check the element's
*  validity.
                  IF ( INARR( I ) .GT. VALUE .AND.
     :                 INARR( I ) .NE. VAL__BADI ) THEN
 
*  If former is greater than the latter, increment the counter.
                     NUMBER = NUMBER + 1
                  END IF
 
               END DO
            ELSE
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test the array value against input value.  If former is greater than
*  the latter, increment the counter.
                  IF ( INARR( I ) .GT. VALUE ) NUMBER = NUMBER + 1
               END DO
            END IF
         END IF
 
*  Test is less than the limit.
*  ============================
 
      ELSE
 
*  Compare absolute values.
*  ========================
         IF ( VABS ) THEN
 
            IF ( BAD ) THEN
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test for bad value first.
                  IF ( INARR( I ) .NE. VAL__BADI ) THEN
 
*  Test the absolute array value against input value and check the
*  element's validity.  Note no ABS function for byte data so must call
*  NUM routine.  VAL is an order of magnitude slower.
                     IF ( NUM_ABSI( INARR( I ) ) .LT. VALUE ) THEN
 
*  If former is less than the latter, increment the counter.
                        NUMBER = NUMBER + 1
                     END IF
                  END IF
               END DO
            ELSE
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test the absolute array value against input value.  Note that the
*  ABS function cannot be used for byte data.  If former is less than
*  the latter, increment the counter.
                  IF ( NUM_ABSI( INARR( I ) ) .LT. VALUE )
     :              NUMBER = NUMBER + 1
               END DO
            END IF
 
*  Compare actual values
*  =====================
         ELSE
            IF ( BAD ) THEN
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test the array value against input value and check the element's
*  validity.
                  IF ( INARR( I ) .LT. VALUE .AND.
     :                 INARR( I ) .NE. VAL__BADI ) THEN
 
*  If former is less than the latter, increment the counter.
                     NUMBER = NUMBER + 1
                  END IF
 
               END DO
            ELSE
 
*  Loop around all elements in the input array.
               DO I = 1, EL
 
*  Test the array value against input value.  If former is less than
*  the latter, increment the counter.
                  IF ( INARR( I ) .LT. VALUE ) NUMBER = NUMBER + 1
               END DO
            END IF
         END IF
      END IF
 
*  Return and end.
      END
