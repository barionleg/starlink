      SUBROUTINE CCD1_SETRD( INDF, IWCS, SNAME, SINDEX, JFRM, STATUS )
*+
*  Name:
*     CCD1_SETRD

*  Purpose:
*     Read Set membership information from an NDF.

*  Language:
*     Starlink Fortran 77.

*  Invocation:
*     CALL CCD1_SETRD( INDF, IWCS, SNAME, SINDEX, JFRM, STATUS )

*  Description:
*     This routine reads the SET header from the .MORE.CCDPACK
*     extension of an NDF and optionally finds the CCD_SET frame in
*     the WCS component.  If no Set information exists then null
*     values are returned.  It is not an error for the .MORE.CCDPACK
*     extension item to be absent, but error status will be set
*     if it exists and does not contain the expected items.

*  Arguments:
*     INDF = INTEGER (Given)
*        NDF identifier of the NDF to be interrogated.
*     IWCS = INTEGER (Given)
*        AST identifier of the WCS frameset corresponding to the NDF.
*        The frameset will be undisturbed on exit.  If this is
*        supplied as AST__NULL then no attempt will be made to
*        interrogate the WCS frameset.
*     SNAME = CHARACTER * ( * ) (Returned)
*        A name labelling the Set.  This should be the same for all
*        members of the same Set, and no two NDFs with the same
*        SET.INDEX should share the same SET.NAME.  In determining
*        equality of names everything (e.g. case, embedded spaces)
*        apart from leading and trailing spaces is significant.
*        If there is no .MORE.CCDPACK.SET header, an empty string
*        will be returned.
*     SINDEX = INTEGER (Returned)
*        A number indicating rank within the Set.  This should be
*        different for all members of the same Set, i.e. no two
*        NDFs with the same SET.NAME should share the same SET.INDEX.
*        NDFs in different Sets (i.e. with different SET.NAMEs)
*        which share the same SET.INDEX are in some circumstances
*        (i.e. when presented to the same application) considered to
*        be 'similar' to one another, and will typically have been
*        generated by the same amplifier/CCD combination.
*        If there is not .MORE.CCDPACK.SET header, a value of
*        CCD1__BADSI (as defined in CCD1_PAR) will be returned.
*     JFRM = INTEGER (Returned)
*        Index of the frame in the WCS component of the NDF in which
*        the Set is to be considered registered.  If there exists
*        a .MORE.CCDPACK.SET header, this will be the index of the
*        frame within the IWCS frameset which has a domain of CCD_SET.
*        If there is no .MORE.CCDPACK.SET header, a value of
*        AST__NOFRAME will be returned.  If there is a .MORE.CCDPACK.SET
*        header, but no CCD_SET frame, a value of AST__NOFRAME will
*        be returned and no error status will result.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Copyright:
*     Copyright (C) 2001 Central Laboratory of the Research Councils

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
*     MBT: Mark Taylor (STARLINK)
*     {enter_new_authors_here}

*  History:
*     5-FEB-2001 (MBT):
*        Original version.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'AST_PAR'          ! Standard AST constants
      INCLUDE 'DAT_PAR'          ! Standard HDS constants
      INCLUDE 'CCD1_PAR'         ! CCDPACK private constants

*  Arguments Given:
      INTEGER INDF
      INTEGER IWCS

*  Arguments Returned:
      CHARACTER * ( * ) SNAME
      INTEGER SINDEX
      INTEGER JFRM

*  Status:
      INTEGER STATUS             ! Global status

*  Local Variables:
      LOGICAL HASSET             ! Is .MORE.CCDPACK.SET exension present?
      LOGICAL THERE              ! Is HDS item present?
      CHARACTER * ( DAT__SZLOC ) XLOC ! Locator for the .MORE.CCDPACK exension
      CHARACTER * ( DAT__SZLOC ) SLOC ! Locator for the SET item
      CHARACTER * ( DAT__SZLOC ) ILOC ! Locator for item

*.

*  Initialise error return values.
      SNAME = ' '
      SINDEX = CCD1__BADSI
      JFRM = AST__NOFRAME

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Initialise flag indicating existence of SET extension.
      HASSET = .FALSE.

*  Get a locator to the .MORE.CCDPACK extension if it exists.
      CALL NDF_XSTAT( INDF, 'CCDPACK', THERE, STATUS )
      IF ( THERE ) THEN
         CALL NDF_XLOC( INDF, 'CCDPACK', 'READ', XLOC, STATUS )

*  Get a locator to the .MORE.CCDPACK.SET item if it exists.
         CALL DAT_THERE( XLOC, 'SET', THERE, STATUS )
         IF ( THERE ) THEN
            CALL DAT_FIND( XLOC, 'SET', SLOC, STATUS )

*  Record that the .SET exension exists.
            IF ( STATUS .EQ. SAI__OK ) HASSET = .TRUE.

*  Try to read the .MORE.CCDPACK.SET.NAME item.
            CALL DAT_THERE( SLOC, 'NAME', THERE, STATUS )
            IF ( THERE ) THEN
               CALL DAT_FIND( SLOC, 'NAME', ILOC, STATUS )
               CALL DAT_GET0C( ILOC, SNAME, STATUS )
               CALL DAT_ANNUL( ILOC, STATUS )
            ELSE
               STATUS = SAI__ERROR
               CALL NDF_MSG( 'ND', INDF )
               CALL ERR_REP( ' ',
     :'CCD1_SETRD: No NAME item in .MORE.CCDPACK.SET extension of ^ND',
     :                       STATUS )
            END IF

*  Try to read the .MORE.CCDPACK.SET.INDEX item.
            CALL DAT_THERE( SLOC, 'INDEX', THERE, STATUS )
            IF ( THERE ) THEN
               CALL DAT_FIND( SLOC, 'INDEX', ILOC, STATUS )
               CALL DAT_GET0I( ILOC, SINDEX, STATUS )
               CALL DAT_ANNUL( ILOC, STATUS )
            ELSE
               STATUS = SAI__ERROR
               CALL NDF_MSG( 'ND', INDF )
               CALL ERR_REP( ' ',
     :'CCD1_SETRD: No INDEX item in .MORE.CCDPACK.SET extension of ^ND',
     :                       STATUS )
            END IF

*  Release the SET locator.
            CALL DAT_ANNUL( SLOC, STATUS )
         END IF

*  Release the CCDPACK extension locator.
         CALL DAT_ANNUL( XLOC, STATUS )
      END IF

*  If we have got all previous Set membership information and we have
*  been asked to do so, get the frame index of the CCD_SET frame.
      IF ( STATUS .EQ. SAI__OK .AND. HASSET .AND. IWCS .NE. AST__NULL )
     :THEN
         CALL CCD1_FRDM( IWCS, 'CCD_SET', JFRM, STATUS )
      END IF

      END
* $Id$
