      SUBROUTINE NDF_COPY( INDF1, PLACE, INDF2, STATUS )
*+
*  Name:
*     NDF_COPY

*  Purpose:
*     Copy an NDF to a new location.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     CALL NDF_COPY( INDF1, PLACE, INDF2, STATUS )

*  Description:
*     The routine copies an NDF to a new location and returns an
*     identifier for the resulting new base NDF.

*  Arguments:
*     INDF1 = INTEGER (Given)
*        Identifier for the NDF (or NDF section) to be copied.
*     PLACE = INTEGER (Given and Returned)
*        An NDF placeholder (e.g. generated by the NDF_PLACE routine)
*        which indicates the position in the data system where the new
*        NDF will reside. The placeholder is annulled by this routine,
*        and a value of NDF__NOPL will be returned (as defined in the
*        include file NDF_PAR).
*     INDF2 = INTEGER (Returned)
*        Identifier for the new NDF.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Notes:
*     -  If this routine is called with STATUS set, then a value of
*     NDF__NOID will be returned for the INDF2 argument, although no
*     further processing will occur. The same value will also be
*     returned if the routine should fail for any reason.  In either
*     event the placeholder will still be annulled.  The NDF__NOID
*     constant is defined in the include file NDF_PAR.

*  Copyright:
*     Copyright (C) 1993 Science & Engineering Research Council

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
*     RFWS: R.F. Warren-Smith (STARLINK)
*     {enter_new_authors_here}

*  History:
*     12-OCT-1989 (RFWS):
*        Original, derived from the equivalent ARY_ system routine.
*     17-OCT-1989 (RFWS):
*        Added support for temporary placeholders.
*     18-OCT-1989 (RFWS):
*        Updated prologue and minor change to error message.
*     1-MAR-1990 (RFWS):
*        Corrected incorrect SAVE statement position.
*     4-NOV-1993 (RFWS):
*        Changed to support foreign format files.
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'DAT_PAR'          ! DAT_ public constants
      INCLUDE 'NDF_PAR'          ! NDF_ public constants
      INCLUDE 'NDF_CONST'        ! NDF_ private constants

*  Arguments Given:
      INTEGER INDF1
      INTEGER PLACE

*  Arguments Returned:
      INTEGER INDF2

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      CHARACTER * ( DAT__SZNAM ) EXTN( 1 ) ! Dummy extension list
      INTEGER IACB1              ! Index to input NDF entry in ACB
      INTEGER IACB2              ! Index to output NDF entry in ACB
      INTEGER IPCB               ! Index to placeholder entry in the PCB
      INTEGER TSTAT              ! Temporary status variable
      LOGICAL CPF( NDF__MXCPF )  ! Component propagation flags
      LOGICAL ERASE              ! Whether to erase placeholder object

*  Local Data:
      SAVE CPF
      DATA CPF / NDF__MXCPF * .TRUE. / ! Propagate all NDF components

*.

*  Set an initial value for the INDF2 argument.
      INDF2 = NDF__NOID

*  Save the STATUS value and mark the error stack.
      TSTAT = STATUS
      CALL ERR_MARK

*  Import the NDF placeholder, converting it to a PCB index.
      STATUS = SAI__OK
      IPCB = 0
      CALL NDF1_IMPPL( PLACE, IPCB, STATUS )

*  If there has been no error at all so far, then import the input
*  NDF identifier.
      IF ( ( STATUS .EQ. SAI__OK ) .AND. ( TSTAT .EQ. SAI__OK ) ) THEN
         CALL NDF1_IMPID( INDF1, IACB1, STATUS )

*  Make a copy of the NDF in place of the placeholder object by
*  propagating all its components to the new data structure.
         CALL NDF1_PRP( IACB1, 0, EXTN, CPF, IPCB, IACB2, STATUS )

*  Export an identifier for the new NDF.
         CALL NDF1_EXPID( IACB2, INDF2, STATUS )

*  If an error occurred, then annul any new ACB entry that may have
*  been acquired.
         IF ( STATUS .NE. SAI__OK ) THEN
            CALL NDF1_ANL( IACB2, STATUS )
         END IF
      END IF

*  Annul the placeholder, erasing the associated object if any error has
*  occurred.
      IF ( IPCB .NE. 0 ) THEN
         ERASE = ( STATUS .NE. SAI__OK ) .OR. ( TSTAT .NE. SAI__OK )
         CALL NDF1_ANNPL( ERASE, IPCB, STATUS )
      END IF

*  Reset the PLACE argument.
      PLACE = NDF__NOPL

*  Annul any error if STATUS was previously bad, otherwise let the new
*  error report stand.
      IF ( STATUS .NE. SAI__OK ) THEN
         IF ( TSTAT .NE. SAI__OK ) THEN
            CALL ERR_ANNUL( STATUS )
            STATUS = TSTAT

*  If appropriate, reset the INDF2 argument, report the error context
*  and call the error tracing routine.
         ELSE
            INDF2 = NDF__NOID
            CALL ERR_REP( 'NDF_COPY_ERR',
     :      'NDF_COPY: Error copying an NDF to a new location.',
     :      STATUS )
            CALL NDF1_TRACE( 'NDF_COPY', STATUS )
         END IF
      ELSE
         STATUS = TSTAT
      END IF

*  Release error stack.
      CALL ERR_RLSE

      END
