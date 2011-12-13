
*+
*  Name:
*    cat0_fiot

*  Copyright:
*    Copyright (C) 2006 Particle Physics & Astronomy Research Council.
*    All Rights Reserved.

*  Licence:
*    This program is free software; you can redistribute it and/or
*    modify it under the terms of the GNU General Public License as
*    published by the Free Software Foundation; either version 2 of
*    the License, or (at your option) any later version.
*
*    This program is distributed in the hope that it will be
*    useful,but WITHOUT ANY WARRANTY; without even the implied
*    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*    PURPOSE. See the GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*    02110-1301, USA

*-
      SUBROUTINE CAT0_FIOB (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      BYTE VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOB (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOB (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOB (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
      SUBROUTINE CAT0_FIOC (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      CHARACTER*(*) VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOC (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOC (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOC (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
      SUBROUTINE CAT0_FIOD (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      DOUBLE PRECISION VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOD (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOD (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOD (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
      SUBROUTINE CAT0_FIOI (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      INTEGER VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOI (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOI (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOI (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
      SUBROUTINE CAT0_FIOL (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      LOGICAL VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOL (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOL (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOL (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
      SUBROUTINE CAT0_FIOR (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      REAL VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOR (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOR (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOR (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
      SUBROUTINE CAT0_FIOW (BCKTYP, IOFLG, CIELM, FI, ELEM, ROWNO,
     :  VALUE, NULFLG, STATUS)
      IMPLICIT NONE
      LOGICAL IOFLG, NULFLG
      INTEGER BCKTYP, CIELM, FI, ELEM, ROWNO, STATUS
      INTEGER*2 VALUE
      INCLUDE 'CAT_PAR'
      INCLUDE 'CAT1_PAR'

      IF (STATUS .NE. CAT__OK) RETURN

      IF (BCKTYP .EQ. CAT1__BKFIT) THEN
         CALL CAT3_FIOW (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKSTL) THEN
         CALL CAT5_FIOW (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      ELSE IF (BCKTYP .EQ. CAT1__BKTST) THEN
         CALL CAT6_FIOW (IOFLG, CIELM, FI, ELEM, ROWNO,
     :     VALUE, NULFLG, STATUS)
      END IF

      END
