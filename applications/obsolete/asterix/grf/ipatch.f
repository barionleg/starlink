*+  IPATCH - patches bad quality pixels in image
      SUBROUTINE IPATCH(STATUS)
*    Description :
*    Deficiencies :
*    Bugs :
*    Authors :
*     BHVAD::RJV
*    History :
*      16 Dec 91 : V1.2-1  now resets BAD flag  (RJV)
*       1 Jul 93 : V1.2-2  GTR used (RJV)
*      28 Oct 93 : V1.7-0  simpler algorithm (RJV)
*      22 Feb 94 : V1.7-1  variance calculation corrected (RJV)
*      24 Feb 94 : V1.7-2  uses BIT_* (RJV)
*      16 Sep 94 : V1.7-3  updates data min/max (RJV)
*    Type definitions :
      IMPLICIT NONE
*    Global constants :
      INCLUDE 'SAE_PAR'
      INCLUDE 'DAT_PAR'
      INCLUDE 'PAR_ERR'
*    Global variables :
      INCLUDE 'IMG_CMN'
*    Status :
      INTEGER STATUS
*    Function declarations :
*    Local constants :
*    Local variables :
*    Version :
      CHARACTER*30 VERSION
      PARAMETER (VERSION = 'IPATCH Version 1.7-3')
*-
      CALL MSG_PRNT(VERSION)

      IF (.NOT.I_OPEN) THEN
        CALL MSG_PRNT('AST_ERR: image processing system not active')
      ELSEIF (.NOT.I_BAD) THEN
        CALL MSG_PRNT('AST_ERR: there are no bad pixels to patch')
      ELSE

*  ensure transformations are correct
        CALL GTR_RESTORE(STATUS)
        CALL GCB_ATTACH('IMAGE',STATUS)
        CALL IMG_2DGCB(STATUS)

        CALL IMG_COPY(.FALSE.,.FALSE.,STATUS)
        I_CAN_UNDO=.FALSE.

        CALL IPATCH_DOIT(%VAL(I_DPTR_W),%VAL(I_VPTR_W),%VAL(I_QPTR_W),
     :                                                          STATUS)

        IF (STATUS.EQ.SAI__OK) THEN

          CALL IMG_SWAP(STATUS)
          CALL IMG_MINMAX(STATUS)

          I_PROC_COUNT=I_PROC_COUNT+1
          I_CAN_UNDO=.TRUE.
          I_LAST_CMD='IPATCH'

        ENDIF

      ENDIF

      END


      SUBROUTINE IPATCH_DOIT(D,V,Q,STATUS)
*    Description :
*    Deficiencies :
*    Bugs :
*    Authors :
*     BHVAD::RJV
*    History :
*
*    Type definitions :
      IMPLICIT NONE
*    Global constants :
      INCLUDE 'SAE_PAR'
      INCLUDE 'DAT_PAR'
      INCLUDE 'QUAL_PAR'
*    Global variables :
      INCLUDE 'IMG_CMN'
*    Import/export :
      REAL D(I_NX,I_NY)
      REAL V(I_NX,I_NY)
      BYTE Q(I_NX,I_NY)
*    Export :
*    Status :
      INTEGER STATUS
*    Function declarations :
      BYTE BIT_ANDUB,BIT_ORUB,BIT_NOTUB
*    Local constants :
*    Local variables :
      INTEGER I,J,II,JJ,I1,I2,J1,J2
      INTEGER NX,NX1,NX2,NY,NY1,NY2
      INTEGER IX,IY
      REAL AX,BX,AY,BY
      REAL DI,DJ
      REAL X(9)
      REAL Y(9)
      REAL Z(9)
      REAL XSIGSQ,YSIGSQ
      BYTE MASK1,MASK2
      LOGICAL ALL
      LOGICAL EDGE
*-
      IF (STATUS.EQ.SAI__OK) THEN

        ALL=.TRUE.

*  create temporary masks to include or exclude previously patched pixels
        MASK1=BIT_ORUB(I_MASK_W,QUAL__PATCHED)
        MASK2=BIT_ANDUB(I_MASK_W,BIT_NOTUB(QUAL__PATCHED))

*  scan current box for bad pixels
        DO J=I_IY1,I_IY2
          DO I=I_IX1,I_IX2

*  found one
            IF (BIT_ANDUB(Q(I,J),MASK2).NE.QUAL__GOOD) THEN

*  see how many in each direction
              I1=I
              I2=I

              EDGE=.FALSE.
              DO WHILE (BIT_ANDUB(Q(I1,J),MASK1).NE.QUAL__GOOD.AND.
     :                               I1.GE.I_IX1.AND..NOT.EDGE)
                IF (I1.EQ.I_IX1) THEN
                  EDGE=.TRUE.
                ELSE
                  I1=I1-1
                ENDIF
              ENDDO
              IF (.NOT.EDGE) THEN
                I1=I1+1
              ENDIF

              EDGE=.FALSE.
              DO WHILE (BIT_ANDUB(Q(I2,J),MASK1).NE.QUAL__GOOD.AND.
     :                               I2.LE.I_IX2.AND..NOT.EDGE)
                IF (I2.EQ.I_IX2) THEN
                  EDGE=.TRUE.
                ELSE
                  I2=I2+1
                ENDIF
              ENDDO
              IF (.NOT.EDGE) THEN
                I2=I2-1
              ENDIF

              J1=J
              J2=J

              EDGE=.FALSE.
              DO WHILE (BIT_ANDUB(Q(I,J1),MASK1).NE.QUAL__GOOD.AND.
     :                               J1.GE.I_IY1.AND..NOT.EDGE)
                IF (J1.EQ.I_IY1) THEN
                  EDGE=.TRUE.
                ELSE
                  J1=J1-1
                ENDIF
              ENDDO
              IF (.NOT.EDGE) THEN
                J1=J1+1
              ENDIF

              EDGE=.FALSE.
              DO WHILE (BIT_ANDUB(Q(I,J2),MASK1).NE.QUAL__GOOD.AND.
     :                               J2.LE.I_IY2.AND..NOT.EDGE)
                IF (J2.EQ.I_IY2) THEN
                  EDGE=.TRUE.
                ELSE
                  J2=J2+1
                ENDIF
              ENDDO
              IF (.NOT.EDGE) THEN
                J2=J2-1
              ENDIF


*  now try to find up to 6 good pixels in each direction (3 either side if poss)
              NX=0
              NX1=0
              II=I1-1
              DO WHILE (NX1.LT.6.AND.II.GE.I_IX1)
                IF (BIT_ANDUB(Q(II,J),MASK1).EQ.QUAL__GOOD) THEN
                  NX=NX+1
                  NX1=NX1+1
                  X(NX)=REAL(II)
                  Z(NX)=D(II,J)
                ENDIF
                II=II-1
              ENDDO
              II=I2+1
              NX2=0
              DO WHILE (NX2.LT.6.AND.NX.LT.9.AND.II.LE.I_IX2)
                IF (BIT_ANDUB(Q(II,J),MASK1).EQ.QUAL__GOOD) THEN
                  NX=NX+1
                  NX2=NX2+1
                  X(NX)=REAL(II)
                  Z(NX)=D(II,J)
                ENDIF
                II=II+1
              ENDDO
              IX=MAX(1,NX-5)
              NX=MIN(NX,6)
              CALL IPATCH_FIT(NX,X(IX),Z(IX),AX,BX,XSIGSQ)

              DO II=I1,I2

                NY=0
                NY1=0
                JJ=J1-1
                DO WHILE (NY1.LT.6.AND.JJ.GE.I_IY1)
                  IF (BIT_ANDUB(Q(II,JJ),MASK1).EQ.QUAL__GOOD) THEN
                      NY=NY+1
                      NY1=NY1+1
                      Y(NY)=REAL(JJ)
                      Z(NY)=D(II,JJ)
                  ENDIF
                  JJ=JJ-1
                ENDDO
                JJ=J2+1
                NY2=0
                DO WHILE (NY2.LT.6.AND.NY.LT.9.AND.JJ.LE.I_IY2)
                  IF (BIT_ANDUB(Q(II,JJ),MASK1).EQ.QUAL__GOOD) THEN
                      NY=NY+1
                      NY2=NY2+1
                      Y(NY)=REAL(JJ)
                      Z(NY)=D(II,JJ)
                  ENDIF
                  JJ=JJ+1
                ENDDO
                IY=MAX(1,NY-5)
                NY=MIN(NY,6)
                CALL IPATCH_FIT(NY,Y(IY),Z(IY),AY,BY,YSIGSQ)


                IF (NX.GT.0.AND.NY.GT.0) THEN
                  DI=AX*REAL(II)+BX
                  DJ=AY*REAL(J)+BY
                  D(II,J)=(DI+DJ)/2.0
                  Q(II,J)=QUAL__PATCHED
                  IF (I_VOK) THEN
                    V(II,J)=(XSIGSQ+YSIGSQ)/4.0
                  ENDIF
                ELSEIF (NX.GT.0) THEN
                  D(II,J)=AX*REAL(II)+BX
                  Q(II,J)=QUAL__PATCHED
                  IF (I_VOK) THEN
                    V(II,J)=XSIGSQ
                  ENDIF
                ELSEIF (NY.GT.0) THEN
                  D(II,J)=AY*REAL(J)+BY
                  Q(II,J)=QUAL__PATCHED
                  IF (I_VOK) THEN
                    V(II,J)=YSIGSQ
                  ENDIF
                ELSE
                  CALL MSG_SETI('I',II)
                  CALL MSG_SETI('J',J)
                  CALL MSG_PRNT(
     :             '** insufficient good pixels to patch (^I,^J)')
                ENDIF


              ENDDO


            ENDIF

          ENDDO
        ENDDO

*  adjust quality mask
        I_MASK_W=BIT_ANDUB(I_MASK_W,BIT_NOTUB(QUAL__PATCHED))


      ENDIF

      END





      SUBROUTINE IPATCH_FIT(N,X,Y,A,B,SIGSQ)

      INTEGER N
      REAL X(*),Y(*)
      REAL A,B,SIGSQ

      REAL SX,SXX,SY,SXY,RESSQ,YCALC
      INTEGER I

      IF (N.EQ.0) THEN
        A=0.0
        B=0.0
      ELSE

        SX=0.0
        SXX=0.0
        SY=0.0
        SXY=0.0
        DO I=1,N
          SX=SX+X(I)
          SXX=SXX+X(I)*X(I)
          SY=SY+Y(I)
          SXY=SXY+X(I)*Y(I)
        ENDDO

        A=(REAL(N)*SXY - SX*SY)/(REAL(N)*SXX - SX*SX)
        B=(SY*SXX - SX*SXY)/(REAL(N)*SXX - SX*SX)

*  calculate std.dev.
        IF (N.GT.2) THEN
          RESSQ=0.0
          DO I=1,N
            YCALC=A*X(I)+B
            RESSQ=RESSQ+(YCALC-Y(I))**2
          ENDDO
          SIGSQ=RESSQ/REAL(N-2)
        ELSE
          SIGSQ=0.0
        ENDIF

       ENDIF

       END
