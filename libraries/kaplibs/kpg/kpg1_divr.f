      SUBROUTINE KPG1_DIVR( BAD, VAR, EL, A, VA, B, VB, C, VC, NBAD,
     :                      STATUS )
*+
*  Name:
*     KPG1_DIVx
 
*  Purpose:
*     Divide two vectorised arrays with optional variance information.
 
*  Language:
*     Starlink Fortran 77
 
*  Invocation:
*     CALL KPG1_DIVx( BAD, VAR, EL, A, VA, B, VB, C, VC, NBAD, STATUS )
 
*  Description:
*     The routine divides one vectorised array by another, with
*     optional variance information. Bad value checking is also
*     performed if required.
 
*  Arguments:
*     BAD = LOGICAL (Given)
*        Whether it is necessary to check for bad values in the input
*        arrays. The routine will execute more rapidly if this checking
*        can be omitted.
*     VAR = LOGICAL (Given)
*        Whether associated variance information is to be processed.
*     EL = INTEGER (Given)
*        Number of array elements to process.
*     A( EL ) = ? (Given)
*        First array of data, to be divided by the second array.
*     VA( EL ) = ? (Given)
*        Variance values associated with the array A. Not used if VAR
*        is set to .FALSE..
*     B( EL ) = ? (Given)
*        Second array of data, to be divided into the first array.
*     VB( EL ) = ? (Given)
*        Variance values associated with the array B. Not used if VAR
*        is set to .FALSE..
*     C( EL ) = ? (Returned)
*        Result of dividing array A by array B.
*     VC( EL ) = ? (Returned)
*        Variance values associated with the array C. Not used if VAR
*        is set to .FALSE..
*     NBAD = INTEGER (Returned)
*        Number of VAL__BADx values in the returned results array (C).
*     STATUS = INTEGER (Given and Returned)
*        The global status.
 
*  Notes:
*     -  There is a routine for each of the data types real and double
*     precision: replace "x" in the routine name by R or D as
*     appropriate.  The arrays passed to this routine should all have
*     the specified data type.
*     -  This routine will NOT handle numerical overflow. 
 
*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}
 
*  History:
*     9-APR-1990 (RFWS):
*        Original version.
*     1991 February 26 (MJC):
*        Fixed bug when VEC routine returns bad status.
*     1996 May 20 (MJC):
*        Replaced LIB$ESTABLISH and LIB$REVERT calls.
*     23-JUN-1998 (DSB):
*        Re-written to avoid dependancy on PRIMDAT error handlers which
*        are not available on Digital UNIX and Linux. Numerical overflow
*        is not longer trapped on Solaris as a result.
*     {enter_further_changes_here}
 
*  Bugs:
*     {note_any_bugs_here}
 
*-
 
*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing
 
*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PRM_PAR'          ! PRIMDAT primitive data constants
 
*  Arguments Given:
      LOGICAL BAD
      LOGICAL VAR
      INTEGER EL
      REAL A( EL )
      REAL VA( EL )
      REAL B( EL )
      REAL VB( EL )
 
*  Arguments Returned:
      REAL C( EL )
      REAL VC( EL )
      INTEGER NBAD
 
*  Status:
      INTEGER STATUS             ! Global status
 
*  Local Variables:
      INTEGER I                  ! Loop counter for array elements
 
*.
 
*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Initialise the number of bad values in the output data array.
      NBAD = 0
 
*  No bad values present:
*  =====================
      IF ( .NOT. BAD ) THEN
 
*  No Variances?
         IF ( .NOT. VAR ) THEN

*  Process each element.
            DO I = 1, EL

*  Check denominator.
               IF( B( I ) .NE. 0.0 ) THEN

*  Store the resulting data value.
                  C( I ) = A( I ) / B( I )

*  If the denominator is zero, store a bad output value and increment the
*  number of errors.
               ELSE 
                  C( I ) = VAL__BADR
                  NBAD = NBAD + 1
               END IF

            END DO 

*  Variances?
         ELSE

*  Process each element.
            DO I = 1, EL

*  Check denominator.
               IF( B( I ) .NE. 0.0 ) THEN

*  Store the resulting data value.
                  C( I ) = A( I ) / B( I )

*  Store the resulting variance.
                  VC( I ) = ( VA( I ) + ( VB( I )*A( I )*A( I ) ) /
     :                                  ( B( I )*B( I ) ) ) /
     :                      ( B( I )*B( I ) )

*  If the denominator is zero, store bad output values and increment the
*  number of errors.
               ELSE 
                  C( I ) = VAL__BADR
                  VC( I ) = VAL__BADR
                  NBAD = NBAD + 1
               END IF

            END DO 

         END IF

*  Bad values present:
*  ==================
      ELSE

*  No Variances?
         IF ( .NOT. VAR ) THEN

*  Process each element.
            DO I = 1, EL

*  Check for bad values and zero denominator.
               IF( A( I ) .NE. VAL__BADR .AND. B( I ) .NE. VAL__BADR 
     :            .AND. B( I ) .NE. 0.0 ) THEN

*  Store the resulting data value.
                  C( I ) = A( I ) / B( I )

*  If either value is bad or the denominator is zero, store a bad output 
*  value and increment the number of errors.
               ELSE 
                  C( I ) = VAL__BADR
                  NBAD = NBAD + 1
               END IF

            END DO 

*  Variances?
         ELSE

*  Process each element.
            DO I = 1, EL

*  Check for bad values and zero denominator.
               IF( A( I ) .NE. VAL__BADR .AND. B( I ) .NE. VAL__BADR 
     :            .AND. B( I ) .NE. 0.0 ) THEN

*  Store the resulting data value.
                  C( I ) = A( I ) / B( I )

*  Check for bad variances. Store the resulting variance if there are none.
                  IF( VA( I ) .NE. VAL__BADR .AND. 
     :                VB( I ) .NE. VAL__BADR ) THEN
                     VC( I ) = ( VA( I ) + ( VB( I )*A( I )*A( I ) ) /
     :                                     ( B( I )*B( I ) ) ) /
     :                         ( B( I )*B( I ) )
                  ELSE
                     VC( I ) = VAL__BADR
                  END IF

*  If the denominator is zero, store bad output values and increment the
*  number of errors.
               ELSE 
                  C( I ) = VAL__BADR
                  VC( I ) = VAL__BADR
                  NBAD = NBAD + 1
               END IF

            END DO 

         END IF

      END IF
 
      END
