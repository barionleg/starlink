      SUBROUTINE BDI2_SPGET( NARG, ARGS, OARG, STATUS )
*+
*  Name:
*     BDI2_SPGET

*  Purpose:
*     Service FileItemGet(Spectrum,FITSfile,item) requests from the BDI system

*  Language:
*     Starlink Fortran

*  Invocation:
*     CALL BDI2_SPGET( NARG, ARGS, OARG, STATUS )

*  Description:
*     Service FileItemGet(Spectrum,FITSfile,item) requests from the BDI system

*  Arguments:
*     NARG = INTEGER (given)
*        Number of method arguments
*     ARGS(*) = INTEGER (given)
*        ADI identifier of method arguments
*     OARG = INTEGER (returned)
*        Output data
*     STATUS = INTEGER (given and returned)
*        The global status.

*  Examples:
*     {routine_example_text}
*        {routine_example_description}

*  Pitfalls:
*     {pitfall_description}...

*  Notes:
*     {routine_notes}...

*  Prior Requirements:
*     {routine_prior_requirements}...

*  Side Effects:
*     {routine_side_effects}...

*  Algorithm:
*     {algorithm_description}...

*  Accuracy:
*     {routine_accuracy}

*  Timing:
*     {routine_timing}

*  External Routines Used:
*     {name_of_facility_or_package}:
*        {routine_used}...

*  Implementation Deficiencies:
*     {routine_deficiencies}...

*  References:
*     BDI Subroutine Guide : http://www.sr.bham.ac.uk/asterix-docs/Programmer/Guides/bdi.html

*  Keywords:
*     package:bdi, usage:private

*  Copyright:
*     Copyright (C) University of Birmingham, 1995

*  Authors:
*     DJA: David J. Allan (Jet-X, University of Birmingham)
*     {enter_new_authors_here}

*  History:
*     9 Aug 1995 (DJA):
*        Original version.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'ADI_PAR'
      INCLUDE 'QUAL_PAR'

*  Arguments Given:
      INTEGER                   NARG                    ! # arguments
      INTEGER                   ARGS(*)                 ! Method arguments

*  Arguments Returned:
      INTEGER                   OARG                    ! Returned data

*  Status:
      INTEGER 			STATUS             	! Global status

*  External References:
      EXTERNAL			CHR_LEN
        INTEGER			CHR_LEN

*  Local Variables:
      CHARACTER*20		ITEM
      CHARACTER*40		UNIT			! Unit string

      INTEGER			BCOL			! Binary table column
      INTEGER			SPHDU			! SPECTRUM hdu id

      LOGICAL			ISRATE			! Corrected spectrum?
*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Initialise
      OARG = ADI__NULLID

*  Extract the arguments
      CALL ADI_GET0C( ARGS(3), ITEM, STATUS )

*  Locate the SPECTRUM hdu
      CALL ADI2_FNDHDU( ARGS(2), 'SPECTRUM', SPHDU, STATUS )

*  Switch on the various items
*  Channel axis label
      IF ( ITEM .EQ. 'Axis_1_Label' ) THEN
        CALL ADI_NEWV0C( 'Energy', OARG, STATUS )

*  Channel axis units
      ELSE IF ( ITEM .EQ. 'Axis_1_Units' ) THEN
        CALL ADI_NEWV0C( 'channels', OARG, STATUS )

*  Whole axis
      ELSE IF ( ITEM .EQ. 'Axis_1' ) THEN

*    Create ADI axis description object
        CALL ADI_NEW0( 'BinDSAxis', OARG, STATUS )

*    Copy each component of the axis structure
        CALL ADI_CPUT0C( OARG, 'Label', 'Energy', STATUS )
        CALL ADI_CPUT0C( OARG, 'Units', 'channels', STATUS )

*  Intensity label
      ELSE IF ( ITEM .EQ. 'Label' ) THEN
        CALL ADI_NEWV0C( 'Intensity', OARG, STATUS )

*  Intensity units
      ELSE IF ( ITEM .EQ. 'Units' ) THEN

*    Identify the intensity column
        CALL ADI2_FNDBTC( SPHDU, 'COUNTS', BCOL, STATUS )
        IF ( STATUS .EQ. SAI__OK ) THEN
          CALL ADI2_FNDBTC( SPHDU, 'RATE', BCOL, STATUS )
          ISRATE = .TRUE.
        ELSE
          ISRATE = .FALSE.
        END IF

*    Generate intensity unit keyword name
        CALL ADI2_HGKYIC( SPHDU, 'TUNIT', BCOL, UNIT, STATUS )
        IF ( STATUS .NE. SAI__OK ) THEN
          CALL ERR_ANNUL( STATUS )
          IF ( ISRATE ) THEN
            UNIT = 'count/s'
          ELSE
            UNIT = 'count'
          END IF
        END IF

        CALL ADI_NEWV0C( UNIT(:CHR_LEN(UNIT)), OARG, STATUS )

      END IF

*  Release the SPECTRUM hdu
      CALL ADI_ERASE( SPHDU, STATUS )

*  Report if unable to get data
      IF ( OARG .EQ. ADI__NULLID ) STATUS = SAI__ERROR

*  Report any errors
      IF ( STATUS .NE. SAI__OK ) CALL AST_REXIT( 'BDI2_SPGET', STATUS )

      END




      SUBROUTINE BDI1_GET( NARG, ARGS, OARG, STATUS )
*+
*  Name:
*     BDI1_GET

*  Purpose:
*     Service FileItemGet requests from the BDI system for HDS files

*  Language:
*     Starlink Fortran

*  Invocation:
*     CALL BDI1_GET( NARG, ARGS, OARG, STATUS )

*  Description:
*     Services BDI get requests for HDS files. In get mode we copy the
*     data from the file into a dynamic ADI object. The BDI top-level
*     makes the decision about the extraction type and other matters.

*  Arguments:
*     NARG = INTEGER (given)
*        Number of method arguments
*     ARGS(*) = INTEGER (given)
*        ADI identifier of method arguments
*     OARG = INTEGER (returned)
*        Output data
*     STATUS = INTEGER (given and returned)
*        The global status.

*  Examples:
*     {routine_example_text}
*        {routine_example_description}

*  Pitfalls:
*     {pitfall_description}...

*  Notes:
*     {routine_notes}...

*  Prior Requirements:
*     {routine_prior_requirements}...

*  Side Effects:
*     {routine_side_effects}...

*  Algorithm:
*     {algorithm_description}...

*  Accuracy:
*     {routine_accuracy}

*  Timing:
*     {routine_timing}

*  External Routines Used:
*     {name_of_facility_or_package}:
*        {routine_used}...

*  Implementation Deficiencies:
*     {routine_deficiencies}...

*  References:
*     BDI Subroutine Guide : http://www.sr.bham.ac.uk/asterix-docs/Programmer/Guides/bdi.html

*  Keywords:
*     package:bdi, usage:private

*  Copyright:
*     Copyright (C) University of Birmingham, 1995

*  Authors:
*     DJA: David J. Allan (Jet-X, University of Birmingham)
*     {enter_new_authors_here}

*  History:
*     9 Aug 1995 (DJA):
*        Original version.
*     8 Nov 1995 (DJA):
*        Handle GET operation for whole axes
*     6 Dec 1995 (DJA):
*       Unified invention scheme
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'ADI_PAR'
      INCLUDE 'DAT_PAR'

*  Arguments Given:
      INTEGER                   NARG, ARGS(*)

*  Arguments Returned:
      INTEGER                   OARG

*  Status:
      INTEGER 			STATUS             	! Global status

*  Local Variables:
      CHARACTER*(DAT__SZLOC)	CLOC			! New component
      CHARACTER*20		ITEM			! Item to get

      INTEGER			ITID			! Invented item id
      INTEGER			NDIM, DIMS(DAT__MXDIM)	! Model object dims
      INTEGER			WBPTR			! Write back address

      LOGICAL			OK			! Data is valid?
      LOGICAL			STRUC			! Object is structure
*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Default return value
      OARG = ADI__NULLID

*  Extract the arguments
      CALL ADI_GET0C( ARGS(3), ITEM, STATUS )

*  Locate object to be got
      CALL BDI1_CFIND( ARGS(1), ARGS(2), ITEM, .FALSE., .FALSE., CLOC,
     :                 NDIM, DIMS, STATUS )

*  Everything ok?
      IF ( (STATUS .EQ. SAI__OK) .AND. (CLOC.NE.DAT__NOLOC) ) THEN

*    Whole axis?
        IF ( (ITEM(:5) .EQ. 'Axis_') .AND.
     :       (INDEX(ITEM(6:),'_').EQ.0) ) THEN

        ELSE

*      Is its data valid?
          CALL DAT_STRUC( CLOC, STRUC, STATUS )
          IF ( STRUC ) THEN
            OK = .TRUE.
          ELSE
            CALL DAT_STATE( CLOC, OK, STATUS )
          END IF
          IF ( OK ) THEN

*        Copy from HDS to ADI
            CALL ADI1_CCH2AT( CLOC, ' ', OARG, ' ', STATUS )

          ELSE
            STATUS = SAI__ERROR
            CALL MSG_SETC( 'IT', ITEM )
            CALL ERR_REP( 'BDI1_GET_1', 'Item ^IT data is invalid',
     :                    STATUS )

          END IF

*      Free the HDS object
          CALL DAT_ANNUL( CLOC, STATUS )

        END IF

      ELSE

*    Try to invent data
        CALL ERR_BEGIN( STATUS )
        CALL BDI1_INVNT( ARGS(1), ARGS(2), ITEM, 'REAL', 'READ',
     :                   ITID, NDIM, DIMS, WBPTR, STATUS )
        CALL ERR_END( STATUS )

*    Invented ok?
        IF ( ITID .NE. ADI__NULLID ) THEN

*      We should store this and re-use it

*      Return to user
          OARG = ITID

        ELSE
          STATUS = SAI__ERROR
          CALL MSG_SETC( 'IT', ITEM )
          CALL ERR_REP( ' ', 'Unable to get item ^IT data from '/
     :                  /'HDS file', STATUS )
        END IF

      END IF

*  Report any errors
      IF ( STATUS .NE. SAI__OK ) CALL AST_REXIT( 'BDI1_GET', STATUS )

      END
