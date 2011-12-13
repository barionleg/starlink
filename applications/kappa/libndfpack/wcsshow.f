      SUBROUTINE WCSSHOW( STATUS )
*+
*  Name:
*     WCSSHOW

*  Purpose:
*     Examines the internal structure of an AST Object.

*  Language:
*     Starlink Fortran 77

*  Type of Module:
*     ADAM A-task

*  Invocation:
*     CALL WCSSHOW( STATUS )

*  Arguments:
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Description:
*     This application allows you to examine an AST Object stored in a
*     specified NDF or HDS object, or a catalogue. The structure can be
*     dumped to a text file, or a Graphical User Interface can be used
*     to navigate through the structure (see Parameter LOGFILE). A new
*     FrameSet can also be stored in the WCS component of an NDF (see
*     Parameter NEWWCS). This allows an NDF WCS component to be dumped
*     to a text file, edited, and then restored to the NDF.
*
*     The GUI main window contains the attribute values of the supplied AST
*     Object. Only those associated with the Object's class are displayed
*     initially, but attributes of the Objects parent classes can be
*     displayed by clicking one of the class button to the top left of the
*     window.
*
*     If the Object contains attributes which are themselves AST Objects
*     (such as the Frames within a FrameSet), then new windows can be
*     created to examine these attributes by clicking over the attribute
*     name.

*  Usage:
*     wcsshow ndf object logfile newwcs full quiet

*  ADAM Parameters:
*     CAT = FILENAME (Read)
*        A catalogue containing a positions list such as produced by
*        applications LISTMAKE, CURSOR, etc. If supplied, the WCS
*        Information in the catalogue is displayed. If a null (!) is
*        supplied, the WCS information in the NDF specified by Parameter
*        NDF is displayed. [!]
*     FULL = _INTEGER (Read)
*        This parameter is a three-state flag and takes values of -1, 0 or
*        +1. It controls the amount of information included in the output
*        generated by this application. If FULL is zero, then a modest
*        amount of non-essential but useful information will be included
*        in the output. If FULL is negative, all non-essential information
*        will be suppressed to minimise the amount of output, while if it is
*        positive, the output will include the maximum amount of detailed
*        information about the Object being examined. [current value]
*     LOGFILE = FILENAME (Write)
*        The name of the text file in which to store a dump of the
*        specified AST Object. If a null (!) value is supplied, no log
*        file is created. If a log file is given, the Tk browser window
*        is not produced. [!]
*     NDF = NDF (Read or Update)
*        If an NDF is supplied, then its WCS FrameSet is displayed. If a
*        null (!) value is supplied, then the Parameter OBJECT is used to
*        specify the AST Object to display. Update access is required to
*        the NDF if a value is given for Parameter NEWWCS. Otherwise, only
*        read access is required. Only accessed if a null (!) value is
*        supplied for CAT.
*     NEWWCS = GROUP (Read)
*        A group expression giving a dump of an AST FrameSet which
*        is to be stored as the WCS component in the NDF given by Parameter
*        NDF. The existing WCS component is unchanged if a null value is
*        supplied. The value supplied for this parameter is ignored if a
*        null value is supplied for Parameter NDF. The Base Frame in the
*        FrameSet is assumed to be the GRID Frame. If a value is given for
*        this parameter, then the log file or Tk browser will display the
*        new FrameSet (after being stored in the NDF and retrieved). [!]
*     OBJECT = LITERAL (Read)
*        The HDS object containing the AST Object to display. Only
*        accessed if parameters NDF and CAT are null. It must have an HDS
*        type of WCS, must be scalar, and must contain a single 1-D array
*        component with name DATA and type _CHAR.
*     QUIET = _LOGICAL (Read)
*        If TRUE, then the structure of the AST Object is not displayed
*        (using the Tk GUI). Other functions are unaffected. If a null (!)
*        value is supplied, the value used is TRUE if a non-null value is
*        supplied for Parameter LOGFILE or Parameter NEWWCS, and FALSE
*        otherwise. [!]

*  Examples:
*     wcsshow m51
*        Displays the WCS component of the NDF m51 in a Tk GUI.
*     wcsshow m51 logfile=m51.ast
*        Dumps the WCS component of the NDF m51 to text file m51.ast.
*     wcsshow m51 newwcs=^m51.ast
*        Reads a FrameSet from the text file m51.ast and stores it in the
*        WCS component of the NDF m51. For instance, the text file m51.ast
*        could be an edited version of the text file created in the
*        previous example.
*     wcsshow object="~/agi_starprog.agi_3800_1.picture(4).more.ast_plot"
*        Displays the AST Plot stored in the AGI database with X windows
*        picture number 4.

*  Copyright:
*     Copyright (C) 1998-1999, 2004 Central Laboratory of the Research Councils.
*     Copyright (C) 2006 Particle Physics & Astronomy Research Council.
*     Copyright (C) 2008 Science and Technology Facilities Council.
*     All Rights Reserved.

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
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     PWD: Peter W. Draper (JAC, Durham University)
*     {enter_new_authors_here}

*  History:
*     2-APR-1998 (DSB):
*        Original version.
*     5-OCT-1998 (DSB):
*        Added Parameter NDF.
*     22-JUN-1999 (DSB):
*        Added Parameter CAT.
*     1-OCT-2004 (TIMJ):
*        Access to KPG_AST restricted to libkpg. Use setter functions.
*     4-OCT-2004 (TIMJ):
*        Use kps1_tkast instead of kpg1_tkast.
*     27-JAN-2006 (DSB):
*        Delete dangling GRP identifiers.
*     3-APR-2008 (PWD):
*        Pass program filename to KPS1_TKAST. Needed so that the
*        self-locating mechanisms of Tcl can be activated.
*     {enter_further_changes_here}

*-
*  Type Definitions:
      IMPLICIT NONE              ! no default typing allowed

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'DAT_PAR'          ! HDS constants
      INCLUDE 'PAR_ERR'          ! PAR error constants
      INCLUDE 'GRP_PAR'          ! GRP constants
      INCLUDE 'NDF_PAR'          ! NDF constants
      INCLUDE 'PRM_PAR'          ! VAL__ constants
      INCLUDE 'AST_PAR'          ! AST constants and function declarations

*  Global Variables:

*  Status:
      INTEGER STATUS

*  External References:
      EXTERNAL KPG1_ASGFR        ! Source function for AST Channel
      EXTERNAL KPG1_ASGFW        ! Sink function for AST Channel

*  Local Variables:
      CHARACTER ACCESS*6
      CHARACTER CNAME*128
      CHARACTER FNAME*512
      CHARACTER LOC*(DAT__SZLOC)
      CHARACTER LOGFNM*(GRP__SZFNM)
      CHARACTER TITLE*255
      INTEGER CHAN
      INTEGER CI
      INTEGER FULL
      INTEGER IAST
      INTEGER IGRP
      INTEGER INDF
      INTEGER SIZE
      INTEGER TLEN
      LOGICAL QUIET
*.

*  Check inherited status.
      IF( STATUS .NE. SAI__OK ) RETURN

*  Begin an AST context.
      CALL AST_BEGIN( STATUS )

*  Attempt to get a group expression using Parameter NEWWCS.
      IGRP = GRP__NOID
      CALL KPG1_GTGRP( 'NEWWCS', IGRP, SIZE, STATUS )

*  Annul the error if a null value was supplied for NEWWCS. Also
*  set up the access required to the NDF.
      IF( STATUS .EQ. SAI__OK ) THEN
         ACCESS = 'UPDATE'

      ELSE IF( STATUS .EQ. PAR__NULL ) THEN
         CALL GRP_DELET( IGRP, STATUS )
         CALL ERR_ANNUL( STATUS )
         ACCESS = 'READ'
      END IF

*  Get the degree of detail required in the output.
      CALL PAR_GET0I( 'FULL', FULL, STATUS )

*  Indicate there is no Object to display yet.
      IAST = AST__NULL

*  Initially assume that the Object should be displayed in the GUI.
      QUIET = .FALSE.

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  Attempt to open an input catalogue. Annull the error if a null value
*  is suplied.
      CALL LPG_CATASSOC( 'CAT', 'READ', CI, STATUS )
      IF( STATUS .EQ. PAR__NULL ) THEN
         CALL ERR_ANNUL( STATUS )

*  Otherwise, reset the pointer for the next item of textual information to
*  be read from the catalogue, and get an AST Object from the catalogue.
      ELSE IF( STATUS .EQ. SAI__OK ) THEN
         CALL CAT_RSTXT( CI, STATUS )
         CALL KPG1_RCATW( CI, IAST, STATUS )

*  If no Object was found, report an error.
         IF( IAST .EQ. AST__NULL .AND. STATUS .EQ. SAI__OK ) THEN
            CALL CAT_TIQAC( CI, 'NAME', CNAME, STATUS )
            CALL CAT_TRLSE( CI, STATUS )

            STATUS = SAI__ERROR
            CALL MSG_SETC( 'CAT', CNAME )
            CALL ERR_REP( 'WCSSHOW_ERR1', 'Supplied catalogue '//
     :                    '''^CAT'' contains no WCS information.',
     :                    STATUS )
            GO TO 999
         END IF

*  Release the catalogue identifier.
         CALL CAT_TRLSE( CI, STATUS )

      END IF

*  If no catalogue was supplied, get an NDF identifier.
      IF( IAST .EQ. AST__NULL ) THEN
         CALL LPG_ASSOC( 'NDF', ACCESS, INDF, STATUS )
      ELSE
         INDF = NDF__NOID
      END IF

*  If succesful...
      IF( INDF .NE. NDF__NOID ) THEN

*  If a value was given for NEWWCS, assume the the Object is not to be
*  displayed.
         IF( IGRP .NE. GRP__NOID ) THEN
            QUIET = .TRUE.

*  Create an AST Channel which can be used to read AST Objects from the
*  group. Set the Skip attribute non-zero so that non-AST data which
*  occur between AST Objects are ignored.
            CHAN = AST_CHANNEL( KPG1_ASGFR, AST_NULL, 'SKIP=1', STATUS )

*  Store the identifier for the group containing the dump in common so
*  that it can be used by the source function KPG1_ASGFR. Also initialise
*  the index of the last group element to have been read.
            CALL KPG1_SETASTGRP( IGRP )
            CALL KPG1_SETASTLN( 0 )

*  Attempt to read an Object from the group.
            IAST = AST_READ( CHAN, STATUS )

*  If FrameSet was read...
            IF( IAST .NE. AST__NULL ) THEN
               IF( AST_ISAFRAMESET( IAST, STATUS ) ) THEN

*  Store it in the NDF.
                  CALL NDF_PTWCS( IAST, INDF, STATUS )

*  Report an error if the Object read is not a FrameSet.
               ELSE
                  STATUS = SAI__ERROR
                  CALL MSG_SETC( 'CLASS', AST_GETC( IAST, 'CLASS',
     :                                              STATUS ) )
                  CALL ERR_REP( 'WCSSHOW_ERR', 'An AST ^CLASS has '//
     :                          'been supplied using Parameter '//
     :                          '%NEWWCS. A FrameSet must be supplied.',
     :                          STATUS )
               END IF

*  Annul the pointer to the supplied Object.
               CALL AST_ANNUL( IAST, STATUS )

*  Report an error if no Object was read.
            ELSE IF( STATUS .EQ. SAI__OK ) THEN
               STATUS = SAI__ERROR
               CALL ERR_REP( 'WCSSHOW_ERR', 'Could not read an AST '//
     :                       'Object using Parameter %NEWWCS.', STATUS )
            END IF

*  Delete the group.
            CALL GRP_DELET( IGRP, STATUS )

         END IF

*  Get a pointer to the NDFs WCS FrameSet.
         CALL NDF_GTWCS( INDF, IAST, STATUS )

*  Save the name of the NDF in an MSG token.
         CALL NDF_MSG( 'OBJ', INDF )

*  Annul the NDF identifier.
         CALL NDF_ANNUL( INDF, STATUS )

*  If no NDF or catalogue was supplied, annull any PAR__NULL error and use
*  Parameter OBJECT to get the AST Object to display.
      ELSE IF( IAST .EQ. AST__NULL ) THEN
         IF( STATUS .EQ. PAR__NULL ) CALL ERR_ANNUL( STATUS )

*  Get a locator to the HDS object.
         CALL DAT_ASSOC( 'OBJECT', 'READ', LOC, STATUS )

*  Read an AST Object from the HDS object.
         CALL KPG1_WREAD( LOC, ' ', IAST, STATUS )

*  Save the name of the Object in an MSG token.
         CALL DAT_MSG( 'OBJ', LOC )

*  Annul the HDS locator.
         CALL DAT_ANNUL( LOC, STATUS )

      END IF

*  Abort if an error has occurred, or there is no Object to display.
      IF( STATUS .NE. SAI__OK .OR. IAST .EQ. AST__NULL ) GO TO 999

*  See if a logfile is required.
      CALL PAR_GET0C( 'LOGFILE', LOGFNM, STATUS )

*  If so, assume that the Object is not to be displayed in the Tk GUI, and
*  dump the AST Object to a GRP group.
      IF( STATUS .EQ. SAI__OK ) THEN
         QUIET = .TRUE.

*  Create a group to act as a temporary staging post for the file.
         CALL GRP_NEW( ' ', IGRP, STATUS )

*  Create an AST Channel which can be used to write the Object description
*  to the group.
         CHAN = AST_CHANNEL( AST_NULL, KPG1_ASGFW, ' ', STATUS )
         CALL AST_SETI( CHAN, 'FULL', FULL, STATUS )
         CALL KPG1_SETASTGRP( IGRP )
         CALL KPG1_SETASTGSP( ' ' )

*  Write the OBJECT to the group. Report an error if nothing is
*  written.
         IF( AST_WRITE( CHAN, IAST, STATUS ) .EQ. 0 .AND.
     :       STATUS .EQ. SAI__OK ) THEN
            STATUS = SAI__ERROR
            CALL ERR_REP( 'WCSSHOW_ERR', 'WCSSHOW: Failed to write '//
     :                    'an AST Object to a GRP group (possible '//
     :                    'programming error).', STATUS )
         END IF

*  Write the group contents out to a file.
         CALL GRP_LIST( 'LOGFILE', 0, 0, ' ', IGRP, STATUS )

*  Delete the group.
         CALL GRP_DELET( IGRP, STATUS )

*  If no log file was supplied, annul the error.
      ELSE IF( STATUS .EQ. PAR__NULL ) THEN
         CALL ERR_ANNUL( STATUS )
      END IF

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  See if the Object is to be displayed in the GUI.
      CALL PAR_DEF0L( 'QUIET', QUIET, STATUS )
      CALL PAR_GET0L( 'QUIET', QUIET, STATUS )
      IF( STATUS .EQ. PAR__NULL ) CALL ERR_ANNUL( STATUS )

*  If so (and we have an Object), display it.
      IF( .NOT. QUIET .AND. IAST .NE. AST__NULL ) THEN

*  Display the AST Object in a TK window.
         CALL MSG_SETC( 'OBJ', AST_GETC( IAST, 'CLASS', STATUS ) )
         CALL MSG_LOAD( ' ', '^OBJ', TITLE, TLEN, STATUS )
         CALL GETARG( 0, FNAME )
         CALL KPS1_TKAST( IAST, TITLE( : TLEN ), FULL, FNAME, STATUS )

      END IF

 999  CONTINUE

*  End the AST context.
      CALL AST_END( STATUS )

*  Give a context message if anything went wrong.
      IF( STATUS .NE. SAI__OK ) THEN
         CALL ERR_REP( 'WCSSHOW_ERR', 'Error displaying an AST '//
     :                 'Object.', STATUS )
      END IF

      END
