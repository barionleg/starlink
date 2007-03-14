/*
*class++
*  Name:
*     Mapping

*  Purpose:
*     Inter-relate two coordinate systems.

*  Constructor Function:
*     None.

*  Description:
*     This class provides the basic facilities for transforming a set
*     of coordinates (representing "input" points) to give a new set
*     of coordinates (representing "output" points).  It is used to
*     describe the relationship which exists between two different
*     coordinate systems and to implement operations which make use of
*     this (such as transforming coordinates and resampling grids of
*     data).  However, the Mapping class does not have a constructor
*     function of its own, as it is simply a container class for a
*     family of specialised Mappings which implement particular types
*     of coordinate transformation.

*  Inheritance:
*     The Mapping class inherits from the Object class.

*  Attributes:
*     In addition to those attributes common to all Objects, every
*     Mapping also has the following attributes:
*
*     - Invert: Mapping inversion flag
*     - Nin: Number of input coordinates for a Mapping
*     - Nout: Number of output coordinates for a Mapping
*     - Report: Report transformed coordinates?
*     - TranForward: Forward transformation defined?
*     - TranInverse: Inverse transformation defined?

*  Functions:
c     In addition to those functions applicable to all Objects, the
c     following functions may also be applied to all Mappings:
f     In addition to those routines applicable to all Objects, the
f     following routines may also be applied to all Mappings:
*
c     - astDecompose: Decompose a Mapping into two component Mappings
c     - astTranGrid: Transform a grid of positions
c     - astInvert: Invert a Mapping
c     - astLinearApprox: Calculate a linear approximation to a Mapping
c     - astMapBox: Find a bounding box for a Mapping
c     - astMapSplit: Split a Mapping up into parallel component Mappings
c     - astRate: Calculate the rate of change of a Mapping output
c     - astRebin<X>: Rebin a region of a data grid
f     - astRebinSeq<X>: Rebin a region of a sequence of data grids
c     - astResample<X>: Resample a region of a data grid
c     - astSimplify: Simplify a Mapping
c     - astTran1: Transform 1-dimensional coordinates
c     - astTran2: Transform 2-dimensional coordinates
c     - astTranN: Transform N-dimensional coordinates
c     - astTranP: Transform N-dimensional coordinates held in separate arrays
f     - AST_DECOMPOSE: Decompose a Mapping into two component Mappings
f     - AST_TRANGRID: Transform a grid of positions
f     - AST_INVERT: Invert a Mapping
f     - AST_LINEARAPPOX: Calculate a linear approximation to a Mapping
f     - AST_MAPBOX: Find a bounding box for a Mapping
f     - AST_MAPSPLIT: Split a Mapping up into parallel component Mappings
f     - AST_RATE: Calculate the rate of change of a Mapping output
f     - AST_REBIN<X>: Rebin a region of a data grid
f     - AST_REBINSEQ<X>: Rebin a region of a sequence of data grids
f     - AST_RESAMPLE<X>: Resample a region of a data grid
f     - AST_SIMPLIFY: Simplify a Mapping
f     - AST_TRAN1: Transform 1-dimensional coordinates
f     - AST_TRAN2: Transform 2-dimensional coordinates
f     - AST_TRANN: Transform N-dimensional coordinates

*  Copyright:
*     Copyright (C) 1997-2006 Council for the Central Laboratory of the
*     Research Councils

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public Licence as
*     published by the Free Software Foundation; either version 2 of
*     the Licence, or (at your option) any later version.
*     
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public Licence for more details.
*     
*     You should have received a copy of the GNU General Public Licence
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
*     02111-1307, USA

*  Authors:
*     RFWS: R.F. Warren-Smith (Starlink)
*     MBT: Mark Taylor (Starlink)
*     DSB: David S. Berry (Starlink)

*  History:
*     1-FEB-1996 (RFWS):
*        Original version.
*     29-FEB-1996 (RFWS):
*        Minor improvements to error messages.
*     15-JUL-1996 (RFWS):
*        Support external interface.
*     13-DEC-1996 (RFWS):
*        Added the astMapMerge method.
*     13-DEC-1996 (RFWS):
*        Added the astSimplify method.
*     27-MAY-1997 (RFWS):
*        Improved the astSimplify method to use astMapMerge to
*        simplify a single Mapping where possible.
*     29-MAY-1998 (RFWS):
*        Added the MapBox method.
*     13-NOV-1998 (RFWS):
*        Made default MapBox convergence accuracy larger (i.e. less
*        accurate).
*     10-DEC-1998 (RFWS):
*        First useful implementation of astResample<X>.
*     16-AUG-1999 (RFWS):
*        Fixed bug in SpecialBounds - wrong number of coordinates being used
*        when checking for bad output coordinate values.
*     17-AUG-1999 (RFWS):
*        Improved the convergence security of MapBox (return to older but
*        less efficient setting).
*     24-NOV-2000 (MBT):
*        Fixed bug (function being invoked as wrong type) in AST__UINTERP
*        scheme, and added new AST__BLOCKAVE scheme, in astResample<X>.
*     9-JAN-2001 (DSB):
*        Changed in and out arguments for TranN from type "double (*)[]"
*        to "double *".
*     8-JAN-2003 (DSB):
*        Changed private InitVtab method to protected astInitMappingVtab
*        method.
*     10-JUL-2003 (DSB):
*        Added method astRate.
*     2-SEP-2004 (DSB):
*        Free resources before leaving astRate.
*     31-AUG-2004 (DSB):
*        Make the LinearApprox function protected rather than private,
*        rename it to astLinearApprox, and make the bounds parameters of
*        type double rather than int. Also, correct the size of the fit
*        coefficient array (was "(nin+1)*nout", now is "(nout+1)*nin").
*        Also correct the index of the first gradient coefficient from
*        "fit+nout" to "fit+nin". These errors have probably never been
*        noticed because they make no difference if nin=nout, which is
*        usually the case.
*     6-SEP-2004 (DSB):
*        Make astRate more robust by adding checks for unusal conditions.
*     20-SEP-2004 (DSB):
*        Make the LinearApprox function public and change its interface
*        to be more appropriate for public use. This involved swapping the
*        direction of the fit (the original astLinearApprox fitted the
*        inverse transformation, but the public version now fits the forwrd
*        transformation).
*     4-OCT-2004 (DSB):
*        Modify astMapList to return flag indicating presence of inverted
*        CmpMaps in supplied Mapping.
*     9-NOV-2004 (DSB):
*        Override astEqual method.
*     6-DEC-2004 (DSB):
*        Remove the second derivative estimate from the astRate function
*        since CmpMap has trouble calculating it.
*     17-DEC-2004 (DSB):
*        Added astMapSplit
*     22-APR-2005 (DSB):
*        Modified SpecialBounds to handle cases where some irrelevant
*        output always produces bad values (e.g. a PermMap may do this).
*     30-JUN-2005 (DSB):
*        Added astRebin.
*     7-JUL-2005 (DSB):
*        Make MapSplit public rather than protected.
*     11-AUG-2005 (DSB):
*        Added the AST__CONSERVEFLUX flag (used by astResampleX).
*     17-AUG-2005 (DSB):
*        Added the AST__SOMBCOS kernel.
*     31-AUG-2005 (DSB):
*        Added astRebinSeq.
*     9-SEP-2005 (DSB):
*        Corrected axis indices returned by public interface for astMapSplit.
*     31-JAN-2006 (DSB):
*        Added IsSimple attribute.
*     2-FEB-2006 (DSB):
*        Corrections to prologue of astLinearApprox.
*     16-FEB-2006 (DSB):
*        Some speed optimisations to rebinning code.
*     2-MAR-2006 (DSB):
*        Use HAVE_LONG_DOUBLE in place of AST_LONG_DOUBLE
*     7-MAR-2006 (DSB):
*        Added astTranGrid.
*     14-MAR-2006 (DSB):
*        - The constructor no longer reports an error if the resulting
*	 Mapping cannot transform points in either direction. This is
*	 because it may be possible to simplify such a Mapping and the
*	 simplified Mapping may have defined transformations. E.g. if a
*	 Mapping which has only a forward transformation is combined in
*	 series with its own inverse, the combination CmpMap will simplify 
*        to a UnitMap (usually).
*        - Reset the "issimple" flag when the Invert flag is changed.
*     9-MAY-2006 (DSB):
*        Correct upper bounds for idim in RebinWithblocking. Also, remove
*        the single precision "F" instantiation of the MAKE_REBINSEQ macro.
*        Also correct the "nout = astGetNin" line in the MAKE_REBINSEQ
*        macro to "nout = astGetNout".
*     12-MAY-2006 (DSB):
*        Modify SpecialBounds to include points slightly inside the
*        corners. This is because some Mappings may have singularies at
*        the the edges.
*     17-MAY-2006 (DSB):
*        Correct the "nout = astGetNin" line in the MAKE_RESAMPLE
*        and MAKE_REBIN macros to "nout = astGetNout".
*     7-JUL-2006 (DSB):
*        Change -CHAR_MAX value (used as a "not set" value for boolean
*        attributes) to +CHAR_MAX, since some compilers do not allow
*        chars to have negative values.
*     23-AUG-2006 (DSB):
*        Change the Equal function so that it reports an error when
*        called, rather than using astSimplify to determine if two Mappings 
*        are equal. All concrete Mapping classes should now provide
*        their own implementation of astEqual, avoiding the use of
*        astSimplify. This is so that astSimplify can use astEqual safely
*        (i.e. without danger of entering an infinite loop).
*     24-NOV-2006 (DSB):
*        Allow astRebinSeq to be called with a NULL pointer for the input
*        data array.
*     14-MAR-2007 (DSB):
*        Modify astRebinSeq to allow input variances to be used as weights.
*class--
*/

/* Module Macros. */
/* ============== */
/* Set the name of the class we are implementing. This indicates to the header
   files that define class interfaces that they should make "protected"
   symbols available. */
#define astCLASS Mapping

/* Include files. */
/* ============== */

/* Configuration results  */
/* ---------------------- */
#include "config.h" 

/* Interface definitions. */
/* ---------------------- */
#include "error.h"               /* Error reporting facilities */
#include "memory.h"              /* Memory allocation facilities */
#include "object.h"              /* Base Object class */
#include "pointset.h"            /* Sets of points/coordinates */
#include "channel.h"             /* I/O channels */
#include "mapping.h"             /* Interface definition for this class */
#include "cmpmap.h"              /* Compund Mappings */
#include "unitmap.h"             /* Unit Mappings */
#include "permmap.h"             /* Axis permutations */
#include "pal.h"              /* SLALIB interface */

/* Error code definitions. */
/* ----------------------- */
#include "ast_err.h"             /* AST error codes */

/* C header files. */
/* --------------- */
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Module type definitions. */
/* ======================== */
/* Enum to represent the data type when resampling a grid of data. */
typedef enum DataType {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
   TYPE_LD,
#endif
   TYPE_D,
   TYPE_F,
   TYPE_L,
   TYPE_UL,
   TYPE_I,
   TYPE_UI,
   TYPE_S,
   TYPE_US,
   TYPE_B,
   TYPE_UB
} DataType;

/* Data structure to hold information about a Mapping for use by
   optimisation algorithms. */
typedef struct MapData {
   AstMapping *mapping;          /* Pointer to the Mapping */
   AstPointSet *pset_in;         /* Pointer to input PointSet */
   AstPointSet *pset_out;        /* Pointer to output PointSet */
   double *lbnd;                 /* Pointer to lower constraints on input */
   double *ubnd;                 /* Pointer to upper constraints on input */
   double **ptr_in;              /* Pointer to input PointSet coordinates */
   double **ptr_out;             /* Pointer to output PointSet coordinates */
   int coord;                    /* Index of output coordinate to optimise */
   int forward;                  /* Use forward transformation? */
   int negate;                   /* Negate the output value? */
   int nin;                      /* Number of input coordinates per point */
   int nout;                     /* Number of output coordinates per point */
} MapData;

/* Data structure describing a polynomial function */
#define RATE_ORDER 4             /* The order of polynial used to evalue function derivatives */
typedef struct PN {
   int order;                    /* The order; zero=constant, 1=linear, 2=quadratic */
   double coeff[ RATE_ORDER + 1 ]; /* The coefficients of the polynomail */
   double xlo;                   /* The lower x limit covered by the polynomial */
   double xhi;                   /* The upper x limit covered by the polynomial */
   double y0;                    /* The y offset to be added to the polynomial value */
} PN;

/* Convert from floating point to floating point or integer */
#define CONV(IntType,val) ( ( IntType ) ? (int) ( (val) + (((val)>0)?0.5:-0.5) ) : (val) )

/* Module Variables. */
/* ================= */
/* Define the class virtual function table and its initialisation flag as
   static variables. */
static AstMappingVtab class_vtab; /* Virtual function table */
static int class_init = 0;       /* Virtual function table initialised? */

/* Pointers to parent class methods which are extended by this class. */
static const char *(* parent_getattrib)( AstObject *, const char * );
static int (* parent_testattrib)( AstObject *, const char * );
static void (* parent_clearattrib)( AstObject *, const char * );
static void (* parent_setattrib)( AstObject *, const char * );
static int (* parent_equal)( AstObject *, AstObject * );

/* Pointer to origin (unsimplified) Mapping, only used for reporting
   error messages. */
static AstMapping *unsimplified_mapping;

/* A flag which indicates if the astRate method should be disabled in
   order to improve algorithm speed in cases where the rate value is not
   significant. If astRate is disabled then it always returns a constant
   value of 1.0. */
static int rate_disabled = 0;

/* Prototypes for private member functions. */
/* ======================================== */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
static void InterpolateBlockAverageLD( int, const int[], const int[], const long double [], const long double [], int, const int[], const double *const[], const double[], int, long double, long double *, long double *, int * ); 
static int InterpolateKernel1LD( AstMapping *, int, const int *, const int *, const long double *, const long double *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, long double, long double *, long double * );
static int InterpolateLinearLD( int, const int *, const int *, const long double *, const long double *, int, const int *, const double *const *, int, long double, long double *, long double * );
static int InterpolateNearestLD( int, const int *, const int *, const long double *, const long double *, int, const int *, const double *const *, int, long double, long double *, long double * );
static void SpreadKernel1LD( AstMapping *, int, const int *, const int *, const long double *, const long double *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, long double, int, long double *, long double *, double *);
static void SpreadLinearLD( int, const int *, const int *, const long double *, const long double *, int, const int *, const double *const *, int, long double, int, long double *, long double *, double *);
static void SpreadNearestLD( int, const int *, const int *, const long double *, const long double *, int, const int *, const double *const *, int, long double, int, long double *, long double *, double *);
static int ResampleLD( AstMapping *, int, const int [], const int [], const long double [], const long double [], int, void (*)(), const double [], int, double, int, long double, int, const int [], const int [], const int [], const int [], long double [], long double [] );
static void RebinLD( AstMapping *, double, int, const int [], const int [], const long double [], const long double [], int, const double [], int, double, int, long double, int, const int [], const int [], const int [], const int [], long double [], long double [] );
static void RebinSeqLD( AstMapping *, double, int, const int [], const int [], const long double [], const long double [], int, const double [], int, double, int, long double, int, const int [], const int [], const int [], const int [], long double [], long double [], double [] );
static void ConserveFluxLD( double, int, const int *, long double, long double *, long double * );
#endif

static AstMapping *Simplify( AstMapping * );
static AstPointSet *Transform( AstMapping *, AstPointSet *, int, AstPointSet * );
static PN *FitPN( AstMapping *, double *, int, int, double, double, double * );
static PN *InterpPN( int, double *, double * );
static const char *GetAttrib( AstObject *, const char * );
static double EvaluateDPN( PN *, double );
static double EvaluatePN( PN *, double );
static double J1Bessel( double );
static double LocalMaximum( const MapData *, double, double, double [] );
static double MapFunction( const MapData *, const double [], int * );
static double MatrixDet( int, const double * );
static double MaxD( double, double );
static double NewVertex( const MapData *, int, double, double [], double [], int *, double [] );
static double Random( long int * );
static double Rate( AstMapping *, double *, int, int );
static double UphillSimplex( const MapData *, double, int, const double [], double [], double *, int * );
static int *MapSplit( AstMapping *, int, int *, AstMapping ** );
static int Equal( AstObject *, AstObject * );
static int GetInvert( AstMapping * );
static int GetIsSimple( AstMapping * );
static int GetNin( AstMapping * );
static int GetNout( AstMapping * );
static int GetReport( AstMapping * );
static int GetTranForward( AstMapping * );
static int GetTranInverse( AstMapping * );
static int InterpolateKernel1B( AstMapping *, int, const int *, const int *, const signed char *, const signed char *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, signed char, signed char *, signed char * );
static int InterpolateKernel1D( AstMapping *, int, const int *, const int *, const double *, const double *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, double, double *, double * );
static int InterpolateKernel1F( AstMapping *, int, const int *, const int *, const float *, const float *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, float, float *, float * );
static int InterpolateKernel1I( AstMapping *, int, const int *, const int *, const int *, const int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, int, int *, int * );
static int InterpolateKernel1L( AstMapping *, int, const int *, const int *, const long int *, const long int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, long int, long int *, long int * );
static int InterpolateKernel1S( AstMapping *, int, const int *, const int *, const short int *, const short int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, short int, short int *, short int * );
static int InterpolateKernel1UB( AstMapping *, int, const int *, const int *, const unsigned char *, const unsigned char *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, unsigned char, unsigned char *, unsigned char * );
static int InterpolateKernel1UI( AstMapping *, int, const int *, const int *, const unsigned int *, const unsigned int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, unsigned int, unsigned int *, unsigned int * );
static int InterpolateKernel1UL( AstMapping *, int, const int *, const int *, const unsigned long int *, const unsigned long int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, unsigned long int, unsigned long int *, unsigned long int * );
static int InterpolateKernel1US( AstMapping *, int, const int *, const int *, const unsigned short int *, const unsigned short int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, unsigned short int, unsigned short int *, unsigned short int * );
static int InterpolateLinearB( int, const int *, const int *, const signed char *, const signed char *, int, const int *, const double *const *, int, signed char, signed char *, signed char * );
static int InterpolateLinearD( int, const int *, const int *, const double *, const double *, int, const int *, const double *const *, int, double, double *, double * );
static int InterpolateLinearF( int, const int *, const int *, const float *, const float *, int, const int *, const double *const *, int, float, float *, float * );
static int InterpolateLinearI( int, const int *, const int *, const int *, const int *, int, const int *, const double *const *, int, int, int *, int * );
static int InterpolateLinearL( int, const int *, const int *, const long int *, const long int *, int, const int *, const double *const *, int, long int, long int *, long int * );
static int InterpolateLinearS( int, const int *, const int *, const short int *, const short int *, int, const int *, const double *const *, int, short int, short int *, short int * );
static int InterpolateLinearUB( int, const int *, const int *, const unsigned char *, const unsigned char *, int, const int *, const double *const *, int, unsigned char, unsigned char *, unsigned char * );
static int InterpolateLinearUI( int, const int *, const int *, const unsigned int *, const unsigned int *, int, const int *, const double *const *, int, unsigned int, unsigned int *, unsigned int * );
static int InterpolateLinearUL( int, const int *, const int *, const unsigned long int *, const unsigned long int *, int, const int *, const double *const *, int, unsigned long int, unsigned long int *, unsigned long int * );
static int InterpolateLinearUS( int, const int *, const int *, const unsigned short int *, const unsigned short int *, int, const int *, const double *const *, int, unsigned short int, unsigned short int *, unsigned short int * );
static int InterpolateNearestB( int, const int *, const int *, const signed char *, const signed char *, int, const int *, const double *const *, int, signed char, signed char *, signed char * );
static int InterpolateNearestD( int, const int *, const int *, const double *, const double *, int, const int *, const double *const *, int, double, double *, double * );
static int InterpolateNearestF( int, const int *, const int *, const float *, const float *, int, const int *, const double *const *, int, float, float *, float * );
static int InterpolateNearestI( int, const int *, const int *, const int *, const int *, int, const int *, const double *const *, int, int, int *, int * );
static int InterpolateNearestL( int, const int *, const int *, const long int *, const long int *, int, const int *, const double *const *, int, long int, long int *, long int * );
static int InterpolateNearestS( int, const int *, const int *, const short int *, const short int *, int, const int *, const double *const *, int, short int, short int *, short int * );
static int InterpolateNearestUB( int, const int *, const int *, const unsigned char *, const unsigned char *, int, const int *, const double *const *, int, unsigned char, unsigned char *, unsigned char * );
static int InterpolateNearestUI( int, const int *, const int *, const unsigned int *, const unsigned int *, int, const int *, const double *const *, int, unsigned int, unsigned int *, unsigned int * );
static int InterpolateNearestUL( int, const int *, const int *, const unsigned long int *, const unsigned long int *, int, const int *, const double *const *, int, unsigned long int, unsigned long int *, unsigned long int * );
static int InterpolateNearestUS( int, const int *, const int *, const unsigned short int *, const unsigned short int *, int, const int *, const double *const *, int, unsigned short int, unsigned short int *, unsigned short int * );
static int LinearApprox( AstMapping *, const double *, const double *, double, double * );
static int MapList( AstMapping *, int, int, int *, AstMapping ***, int ** );
static int MapMerge( AstMapping *, int, int, int *, AstMapping ***, int ** );
static int MaxI( int, int );
static int MinI( int, int );
static int ResampleAdaptively( AstMapping *, int, const int *, const int *, const void *, const void *, DataType, int, void (*)(), const double *, int, double, int, const void *, int, const int *, const int *, const int *, const int *, void *, void * );
static int ResampleB( AstMapping *, int, const int [], const int [], const signed char [], const signed char [], int, void (*)(), const double [], int, double, int, signed char, int, const int [], const int [], const int [], const int [], signed char [], signed char [] );
static int ResampleD( AstMapping *, int, const int [], const int [], const double [], const double [], int, void (*)(), const double [], int, double, int, double, int, const int [], const int [], const int [], const int [], double [], double [] );
static int ResampleF( AstMapping *, int, const int [], const int [], const float [], const float [], int, void (*)(), const double [], int, double, int, float, int, const int [], const int [], const int [], const int [], float [], float [] );
static int ResampleI( AstMapping *, int, const int [], const int [], const int [], const int [], int, void (*)(), const double [], int, double, int, int, int, const int [], const int [], const int [], const int [], int [], int [] );
static int ResampleL( AstMapping *, int, const int [], const int [], const long int [], const long int [], int, void (*)(), const double [], int, double, int, long int, int, const int [], const int [], const int [], const int [], long int [], long int [] );
static int ResampleS( AstMapping *, int, const int [], const int [], const short int [], const short int [], int, void (*)(), const double [], int, double, int, short int, int, const int [], const int [], const int [], const int [], short int [], short int [] );
static int ResampleSection( AstMapping *, const double *, int, const int *, const int *, const void *, const void *, DataType, int, void (*)(), const double *, double, int, const void *, int, const int *, const int *, const int *, const int *, void *, void * );
static int ResampleUB( AstMapping *, int, const int [], const int [], const unsigned char [], const unsigned char [], int, void (*)(), const double [], int, double, int, unsigned char, int, const int [], const int [], const int [], const int [], unsigned char [], unsigned char [] );
static int ResampleUI( AstMapping *, int, const int [], const int [], const unsigned int [], const unsigned int [], int, void (*)(), const double [], int, double, int, unsigned int, int, const int [], const int [], const int [], const int [], unsigned int [], unsigned int [] );
static int ResampleUL( AstMapping *, int, const int [], const int [], const unsigned long int [], const unsigned long int [], int, void (*)(), const double [], int, double, int, unsigned long int, int, const int [], const int [], const int [], const int [], unsigned long int [], unsigned long int [] );
static int ResampleUS( AstMapping *, int, const int [], const int [], const unsigned short int [], const unsigned short int [], int, void (*)(), const double [], int, double, int, unsigned short int, int, const int [], const int [], const int [], const int [], unsigned short int [], unsigned short int [] );
static int ResampleWithBlocking( AstMapping *, const double *, int, const int *, const int *, const void *, const void *, DataType, int, void (*)(), const double *, int, const void *, int, const int *, const int *, const int *, const int *, void *, void * );
static int SpecialBounds( const MapData *, double *, double *, double [], double [] );
static int TestAttrib( AstObject *, const char * );
static int TestInvert( AstMapping * );
static int TestReport( AstMapping * );
static void ClearAttrib( AstObject *, const char * );
static void ClearInvert( AstMapping * );
static void ClearReport( AstMapping * );
static void CombinePN( PN *, PN * );
static void ConserveFluxB( double, int, const int *, signed char, signed char *, signed char * );
static void ConserveFluxD( double, int, const int *, double, double *, double * );
static void ConserveFluxF( double, int, const int *, float, float *, float * );
static void ConserveFluxI( double, int, const int *, int, int *, int * );
static void ConserveFluxL( double, int, const int *, long int, long int *, long int * );
static void ConserveFluxS( double, int, const int *, short int, short int *, short int * );
static void ConserveFluxUB( double, int, const int *, unsigned char, unsigned char *, unsigned char * );
static void ConserveFluxUI( double, int, const int *, unsigned int, unsigned int *, unsigned int * );
static void ConserveFluxUL( double, int, const int *, unsigned long int, unsigned long int *, unsigned long int * );
static void ConserveFluxUS( double, int, const int *, unsigned short int, unsigned short int *, unsigned short int * );
static void Copy( const AstObject *, AstObject * );
static void Decompose( AstMapping *, AstMapping **, AstMapping **, int *, int *, int * );
static void Delete( AstObject * );
static void Dump( AstObject *, AstChannel * );
static void FunPN( AstMapping *, double *, int, int, int, double *, double * );
static void Gauss( double, const double [], int, double * );
static void GlobalBounds( MapData *, double *, double *, double [], double [] );
static void InterpolateBlockAverageB( int, const int[], const int[], const signed char [], const signed char [], int, const int[], const double *const[], const double[], int, signed char, signed char *, signed char *, int * ); 
static void InterpolateBlockAverageD( int, const int[], const int[], const double [], const double [], int, const int[], const double *const[], const double[], int, double, double *, double *, int * ); 
static void InterpolateBlockAverageF( int, const int[], const int[], const float [], const float [], int, const int[], const double *const[], const double[], int, float, float *, float *, int * ); 
static void InterpolateBlockAverageI( int, const int[], const int[], const int [], const int [], int, const int[], const double *const[], const double[], int, int, int *, int *, int * ); 
static void InterpolateBlockAverageL( int, const int[], const int[], const long int [], const long int [], int, const int[], const double *const[], const double[], int, long int, long int *, long int *, int * ); 
static void InterpolateBlockAverageS( int, const int[], const int[], const short int [], const short int [], int, const int[], const double *const[], const double[], int, short int, short int *, short int *, int * ); 
static void InterpolateBlockAverageUB( int, const int[], const int[], const unsigned char [], const unsigned char [], int, const int[], const double *const[], const double[], int, unsigned char, unsigned char *, unsigned char *, int * ); 
static void InterpolateBlockAverageUI( int, const int[], const int[], const unsigned int [], const unsigned int [], int, const int[], const double *const[], const double[], int, unsigned int, unsigned int *, unsigned int *, int * ); 
static void InterpolateBlockAverageUL( int, const int[], const int[], const unsigned long int [], const unsigned long int [], int, const int[], const double *const[], const double[], int, unsigned long int, unsigned long int *, unsigned long int *, int * ); 
static void InterpolateBlockAverageUS( int, const int[], const int[], const unsigned short int [], const unsigned short int [], int, const int[], const double *const[], const double[], int, unsigned short int, unsigned short int *, unsigned short int *, int * ); 
static void Invert( AstMapping * );
static void MapBox( AstMapping *, const double [], const double [], int, int, double *, double *, double [], double [] );
static void RebinAdaptively( AstMapping *, int, const int *, const int *, const void *, const void *, DataType, int, const double *, int, double, int, const void *, int, const int *, const int *, const int *, const int *, int, void *, void *, double * );
static void RebinD( AstMapping *, double, int, const int [], const int [], const double [], const double [], int, const double [], int, double, int, double, int, const int [], const int [], const int [], const int [], double [], double [] );
static void RebinF( AstMapping *, double, int, const int [], const int [], const float [], const float [], int, const double [], int, double, int, float, int, const int [], const int [], const int [], const int [], float [], float [] );
static void RebinI( AstMapping *, double, int, const int [], const int [], const int [], const int [], int, const double [], int, double, int, int, int, const int [], const int [], const int [], const int [], int [], int [] );
static void RebinSeqD( AstMapping *, double, int, const int [], const int [], const double [], const double [], int, const double [], int, double, int, double, int, const int [], const int [], const int [], const int [], double [], double [], double [] );
static void RebinSeqF( AstMapping *, double, int, const int [], const int [], const float [], const float [], int, const double [], int, double, int, float, int, const int [], const int [], const int [], const int [], float [], float [], double [] );
static void RebinSeqI( AstMapping *, double, int, const int [], const int [], const int [], const int [], int, const double [], int, double, int, int, int, const int [], const int [], const int [], const int [], int [], int [], double [] );
static void RebinSection( AstMapping *, const double *, int, const int *, const int *, const void *, const void *, DataType, int, const double *, int, const void *, int, const int *, const int *, const int *, const int *, int, void *, void *, double * );
static void RebinWithBlocking( AstMapping *, const double *, int, const int *, const int *, const void *, const void *, DataType, int, const double *, int, const void *, int, const int *, const int *, const int *, const int *, int, void *, void *, double *  );
static void ReportPoints( AstMapping *, int, AstPointSet *, AstPointSet * );
static void SetAttrib( AstObject *, const char * );
static void SetInvert( AstMapping *, int );
static void SetReport( AstMapping *, int );
static void Sinc( double, const double [], int, double * );
static void SincCos( double, const double [], int, double * );
static void SincGauss( double, const double [], int, double * );
static void SincSinc( double, const double [], int, double * );
static void Somb( double, const double [], int, double * );
static void SombCos( double, const double [], int, double * );
static void SpreadKernel1D( AstMapping *, int, const int *, const int *, const double *, const double *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, double, int, double *, double *, double * );
static void SpreadKernel1F( AstMapping *, int, const int *, const int *, const float *, const float *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, float, int, float *, float *, double *);
static void SpreadKernel1I( AstMapping *, int, const int *, const int *, const int *, const int *, int, const int *, const double *const *, void (*)( double, const double *, int, double * ), int, const double *, int, int, int, int *, int *, double * );
static void SpreadLinearD( int, const int *, const int *, const double *, const double *, int, const int *, const double *const *, int, double, int, double *, double *, double *);
static void SpreadLinearF( int, const int *, const int *, const float *, const float *, int, const int *, const double *const *, int, float, int, float *, float *, double *);
static void SpreadLinearI( int, const int *, const int *, const int *, const int *, int, const int *, const double *const *, int, int, int, int *, int *, double *);
static void SpreadNearestD( int, const int *, const int *, const double *, const double *, int, const int *, const double *const *, int, double, int, double *, double *, double *);
static void SpreadNearestF( int, const int *, const int *, const float *, const float *, int, const int *, const double *const *, int, float, int, float *, float *, double *);
static void SpreadNearestI( int, const int *, const int *, const int *, const int *, int, const int *, const double *const *, int, int, int, int *, int *, double *);
static void Tran1( AstMapping *, int, const double [], int, double [] );
static void Tran2( AstMapping *, int, const double [], const double [], int, double [], double [] );
static void TranGrid( AstMapping *, int, const int[], const int[], double, int, int, int, int, double * );
static void TranGridAdaptively( AstMapping *, int, const int[], const int[], const int[], const int[], double, int, int, double *[]  );
static void TranGridSection( AstMapping *, const double *, int, const int *, const int *, const int *, const int *, int, double *[] );
static void TranGridWithBlocking( AstMapping *, const double *, int, const int *, const int *, const int *, const int *, int, double *[]  );
static void TranN( AstMapping *, int, int, int, const double *, int, int, int, double * );
static void TranP( AstMapping *, int, int, const double *[], int, int, double *[] );
static void ValidateMapping( AstMapping *, int, int, int, int, const char * );



/* Member functions. */
/* ================= */
static void ClearAttrib( AstObject *this_object, const char *attrib ) {
/*
*  Name:
*     ClearAttrib

*  Purpose:
*     Clear an attribute value for a Mapping.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void ClearAttrib( AstObject *this, const char *attrib )

*  Class Membership:
*     Mapping member function (over-rides the astClearAttrib protected
*     method inherited from the Object class).

*  Description:
*     This function clears the value of a specified attribute for a
*     Mapping, so that the default value will subsequently be used.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     attrib
*        Pointer to a null terminated string specifying the attribute
*        name.  This should be in lower case with no surrounding white
*        space.
*/

/* Local Variables: */
   AstMapping *this;             /* Pointer to the Mapping structure */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the Mapping structure. */
   this = (AstMapping *) this_object;

/* Check the attribute name and clear the appropriate attribute. */

/* Invert. */
/* ------- */
   if ( !strcmp( attrib, "invert" ) ) {
      astClearInvert( this );

/* Report. */
/* ------- */
   } else if ( !strcmp( attrib, "report" ) ) {
      astClearReport( this );

/* If the name was not recognised, test if it matches any of the
   read-only attributes of this class. If it does, then report an
   error. */
   } else if ( !strcmp( attrib, "nin" ) ||
               !strcmp( attrib, "nout" ) ||
               !strcmp( attrib, "issimple" ) ||
               !strcmp( attrib, "tranforward" ) ||
               !strcmp( attrib, "traninverse" ) ) {
      astError( AST__NOWRT, "astClear: Invalid attempt to clear the \"%s\" "
                "value for a %s.", attrib, astGetClass( this ) );
      astError( AST__NOWRT, "This is a read-only attribute." );

/* If the attribute is still not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      (*parent_clearattrib)( this_object, attrib );
   }
}

static void CombinePN( PN *lo, PN *hi ) {
/*
*  Name:
*     CombinePN

*  Purpose:
*     Combine polynomials "lo" and "hi", both of order N, into a polynomial
*     of order N+1.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void CombinePN( PN *lo, PN *hi )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function combines polynomials "lo" and "hi", both of order N, 
*     into a polynomial of order N+1, and return the new polynomial in 
*     "lo". It is used to implemtn Neville's algorithm for finding an
*     interpolating polynomial. See:
*
*        http://mathworld.wolfram.com/NevillesAlgorithm.html

*  Parameters:
*     lo
*        A polynomial covering the lower x interval. Returned holding the
*        combined higher-order polynomial.
*     hi
*        A polynomial covering the higher x interval. Unchanged on exit.

*/

/* Local Variables: */
   int n, k;
   double f, xlo, xhi, cc[ RATE_ORDER + 1 ];

/* Check the global error status */
   if ( !astOK ) return;

   n = lo->order;
   xlo = lo->xlo;
   xhi = hi->xhi;

   f = 1.0/( xlo - xhi );

   cc[ 0 ] = f*(- xhi*( lo->coeff[0]) + xlo*( hi->coeff[ 0 ] ) );
   for( k = 1; k <= n; k++ ) {
      cc[ k ] = f*( ( lo->coeff[ k - 1 ] ) - xhi*( lo->coeff[ k ] )
                   -( hi->coeff[ k - 1 ] ) + xlo*( hi->coeff[ k ] ) );
   }

   cc[ n + 1 ] = f*( lo->coeff[ n ] - hi->coeff[ n ] );

   lo->order = n + 1;
   lo->xlo = xlo;
   lo->xhi = xhi;
   for( k = 0; k < n+2; k++ ) lo->coeff[ k ] = cc[ k ];

}

/*
*  Name:
*     ConserveFlux<X>

*  Purpose:
*     Scale the output data and variance values produced by ResampleSection 
*     by the given flux conservation factor. 

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void ConserveFlux<X>( double factor, int npoint, const int *offset,
*                           <Xtype> badval, <Xtype> *out, 
*                           <Xtype> *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which scale the supplied resampled data 
*     values by the given flux conservation factor. It also scales any 
*     variances by the square of the factor.

*  Parameters:
*     factor
*        The flux conservation factor. This should be the ratio of the
*        output pixel size to the input pixel size, in the locality of
*        the supplied data values.
*     npoint
*        The number of points at which the input grid was resampled.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each output point, this array should contain the zero-based
*        offset in the output array(s) (i.e. the "out" and,
*        optionally, the "out_var" arrays) at which the resampled
*        output value(s) is stored.
*     badval
*        This parameter specifies the value which is used to identify
*        bad data and/or variance values in the output array(s).
*     out
*        Pointer to an array in which the resampled data is supplied. Note
*        that details of how the output grid maps on to this array
*        (e.g. the storage order, number of dimensions, etc.) is
*        arbitrary and is specified entirely by means of the "offset"
*        array. The "out" array should therefore contain sufficient
*        elements to accommodate the "offset" values supplied.  There
*        is no requirement that all elements of the "out" array should
*        be assigned values, and any which are not addressed by the
*        contents of the "offset" array will be left unchanged.
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, in which variance estimates for
*        the resampled values are supplied. If no output variance estimates 
*        are available, a NULL pointer should be given.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*/
/* Define a macro to implement the function for a specific data
   type. */
#define MAKE_CONSERVEFLUX(X,Xtype) \
static void ConserveFlux##X( double factor, int npoint, const int *offset, \
                             Xtype badval, Xtype *out, Xtype *out_var ) { \
\
/* Local Variables: */ \
   int off_out;                  /* Pixel offset into output array */ \
   int point;                    /* Loop counter for output points */ \
\
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
   for ( point = 0; point < npoint; point++ ) { \
      off_out = offset[ point ]; \
      if( out[ off_out ] != badval ) out[ off_out ] *= factor; \
   } \
\
   if( out_var ) { \
      factor *= factor; \
      for ( point = 0; point < npoint; point++ ) { \
         off_out = offset[ point ]; \
         if( out_var[ off_out ] != badval ) out_var[ off_out ] *= factor; \
      } \
   } \
}


/* Expand the macro above to generate a function for each required 
   data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_CONSERVEFLUX(LD,long double)
#endif
MAKE_CONSERVEFLUX(D,double)
MAKE_CONSERVEFLUX(F,float) 
MAKE_CONSERVEFLUX(L,long int)
MAKE_CONSERVEFLUX(I,int)
MAKE_CONSERVEFLUX(S,short int)
MAKE_CONSERVEFLUX(B,signed char)
MAKE_CONSERVEFLUX(UL,unsigned long int)
MAKE_CONSERVEFLUX(UI,unsigned int)
MAKE_CONSERVEFLUX(US,unsigned short int)
MAKE_CONSERVEFLUX(UB,unsigned char)

/* Undefine the macros used above. */
#undef MAKE_CONSERVEFLUX

static void Decompose( AstMapping *this, AstMapping **map1, AstMapping **map2, 
                       int *series, int *invert1, int *invert2 ) {
/*
*+
*  Name:
*     astDecompose

*  Purpose:
*     Decompose a Mapping into two component Mappings.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     void astDecompose( AstMapping *this, AstMapping **map1, 
*                        AstMapping **map2, int *series, int *invert1, 
*                        int *invert2  ) 

*  Class Membership:
*     Mapping method.

*  Description:
*     This function returns pointers to two Mappings which, when applied
*     either in series or parallel, are equivalent to the supplied Mapping.
*
*     Since the Frame class inherits from the Mapping class, Frames can
*     be considered as special types of Mappings and so this method can
*     be used to decompose either CmpMaps or CmpFrames.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     map1
*        Address of a location to receive a pointer to first component
*        Mapping. 
*     map2
*        Address of a location to receive a pointer to second component
*        Mapping. 
*     series
*        Address of a location to receive a value indicating if the
*        component Mappings are applied in series or parallel. A non-zero
*        value means that the supplied Mapping is equivalent to applying map1 
*        followed by map2 in series. A zero value means that the supplied
*        Mapping is equivalent to applying map1 to the lower numbered axes
*        and map2 to the higher numbered axes, in parallel.
*     invert1
*        The value of the Invert attribute to be used with map1. 
*     invert2
*        The value of the Invert attribute to be used with map2. 

*  Applicability:
*     CmpMap
*        If the supplied Mapping is a CmpMap, then map1 and map2 will be
*        returned holding pointers to the component Mappings used to
*        create the CmpMap, either in series or parallel.
*     Mapping
*        For any class of Mapping other than a CmpMap, map1 will be
*        returned holding a clone of the supplied Mapping pointer, and map2
*        will be returned holding a NULL pointer.
*     CmpFrame
*        If the supplied Mapping is a CmpFrame, then map1 and map2 will be
*        returned holding pointers to the component Frames used to
*        create the CmpFrame. The component Frames are considered to be in
*        applied in parallel.
*     Frame
*        For any class of Frame other than a CmpFrame, map1 will be 
*        returned holding a clone of the supplied Frame pointer, and map2
*        will be returned holding a NULL pointer.

*  Notes:
*     - Any changes made to the component Mappings using the returned
*     pointers will be reflected in the supplied Mapping.
*     - The returned Invert values should be used in preference to the 
*     current values of the Invert attribute in map1 and map2. This is
*     because the attributes may have changed value since the Mappings 
*     were combined.

*  Implementation Notes:
*     - This function implements the basic astDecompose method
*     available via the protected interface to the Frame class. The
*     public interface to this method is provided by the
*     astDecomposeId_ function.

*-
*/

/* Check the global error status. */
   if ( !astOK ) return;

/* The basic Mapping class returns a clone of the supplied Mapping as
   map1 and a NULL pointer as map2. */
   if( map1 ) *map1 = astClone( this );
   if( map2 ) *map2 = NULL;
   if( series ) *series = 1;
   if( invert1 ) *invert1 = astGetInvert( this );
   if( invert2 ) *invert2 = 0;
}

int astRateState_( int disabled ) {
/*
*+
*  Name:
*     astRateState

*  Purpose:
*     Control whether the astRate method is disabled or not.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "mapping.h"
*     int astRateState( int disabled )

*  Class Membership:
*     Mapping member function

*  Description:
*     Some algorithms which use use the astRate method do not actually need
*     to know what the Rate value is. For instance, when the Plot class draws 
*     a border it evaluates the GRAPHICS->Current Mapping hundreds of time.
*     If the Mapping includes a RateMap then this can be very very slow
*     (depending on how the astRate method is implemented). In fact the
*     border drawing algorithm onlyneeds to know if the result is bad or
*     not - the actual value produced by the Mappign does not matter.
*
*     Such algorithms can be speeded up by forcing the astRate method to
*     return a constant value rather than actually doing the numerical
*     differentiation. This can be accomplised by calling this method prior
*     to implementing the algorithm. It should be called at the end in
*     order to re-instate the original disabled flag.

*  Parameters:
*     disabled
*        The new value for the astRate disabled flag.

*  Returned Value:
*     The original value of the astRate disabled flag.

*-
*/
   int result = rate_disabled;
   rate_disabled = disabled;
   return result;
}   

static int Equal( AstObject *this_object, AstObject *that_object ) {
/*
*  Name:
*     Equal

*  Purpose:
*     Test if two Mappings are equivalent.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int Equal( AstObject *this, AstObject *that ) 

*  Class Membership:
*     Mapping member function (over-rides the astEqual protected
*     method inherited from the Object class).

*  Description:
*     This function returns a boolean result (0 or 1) to indicate whether
*     two Mappings are equivalent.
*
*     The implementation provided by this class (the base Mapping class)
*     simply reports an error when called, since all concrete Mapping
*     subclasses should provide their own implementation. 
*
*     Note, sub-class implementations should not use astSimplify (e.g.
*     combining the two Mapping and then simplifying it), since the 
*     astSimplify method for certain classes (e.g. CmpMap) may use 
*     astEqual. Consequently, if astEqual called astSimplify, there would 
*     be possibilities for infinite loops.

*  Parameters:
*     this
*        Pointer to the first Object (a Mapping).
*     that
*        Pointer to the second Object.

*  Returned Value:
*     One if the Frames are equivalent, zero otherwise.

*  Notes:
*     - The two Mappings are considered equivalent if the combination of
*     the first in series with the inverse of the second simplifies to a
*     UnitMap.
*     - A value of zero will be returned if this function is invoked
*     with the global status set, or if it should fail for any reason.
*/

/* Local Variables: */
   int result;                /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Invoke the Equal method inherited from the parent Object class. This checks
   that the Objects are both of the same class (amongst other things). */
   if( (*parent_equal)( this_object, that_object ) ) {

/* Report an error since the concrete sub-class should have over-riden
   this method. */
      astError( AST__INTER, "astEqual(Mapping): The %s class does "
                "not override the abstract astEqual method inherited "
                "from the base Mapping class (internal AST programming "
                "error).", astGetClass( this_object ) );
   }

/* If an error occurred, clear the result value. */
   if ( !astOK ) result = 0;

/* Return the result, */
   return result;
}

static void FunPN( AstMapping *map, double *at, int ax1, int ax2,
                   int n, double *x, double *y ) {
/*
*  Name:
*     FunPN

*  Purpose:
*     Find the value of the function currently being differentiated by the 
*     astRate method.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void FunPN( AstMapping *map, double *at, int ax1, int ax2,
*                 int n, double *x, double *y )

*  Class Membership:
*     Mapping method.

*  Description:
*     This is a service function for the astRate method. It evaluates the
*     function being differentiated at specified axis values.
*
*     This function uses static resources in order to avoid the overhead 
*     of creating new PointSets each time this function is called. These
*     static resources which must be initialised before the first invocation 
*     with a given Mapping, and must be released after the final invocation. 
*     See "ax1".

*  Parameters:
*     map
*        Pointer to a Mapping which yields the value of the function at x.
*        The Mapping may have any number of inputs and outputs; the specific 
*        output representing the function value, f, is specified by ax1 and 
*        the specific input representing the argument, x, is specified by ax2.
*     at
*        A pointer to an array holding axis values at the position at which 
*        the function is to be evaluated. The number of values supplied
*        must equal the number of inputs to the Mapping. The value supplied 
*        for axis "ax2" is ignored (the value of "x" is used for axis "ax2").
*     ax1 
*        The zero-based index of the Mapping output which is to be
*        differentiated. Set this to -1 to allocate, or -2 to release, 
*        the static resources used by this function.
*     ax2 
*        The zero-based index of the Mapping input which is to be varied.
*     n
*        The number of elements in the "x" and "y" arrays. This should not
*        be greater than 2*RATE_ORDER.
*     x
*        The value of the Mapping input specified by ax2 at which the
*        function is to be evaluated. If "ax2" is set to -1, then the
*        supplied value is used as flag indicating if the static resources 
*        used by this function should be initialised (if x >= 0 ) or
*        freed (if x < 0).
*     y
*        An array in which to return the function values at the positions
*        given in "x".

*/
#define MAX_CACHE  5

/* Local Variables: */
   static AstPointSet *pset1_cache[ MAX_CACHE ];
   static AstPointSet *pset2_cache[ MAX_CACHE ];
   static int next_slot;
   static int pset_size[ MAX_CACHE ];

   AstPointSet *pset1;
   AstPointSet *pset2;
   double **ptr1;
   double **ptr2;
   double *oldx;
   double *oldy;
   double *p;
   double xx;
   int i;
   int k;
   int nin;
   int nout;

/* Check the global error status. */
   if ( !astOK ) return;

/* Initialise variables to avoid "used of uninitialised variable"
   messages from dumb compilers. */
   pset2 = NULL;

/* If required, initialise things. */
   if( ax1 == -1 ) {
      for( i = 0; i < MAX_CACHE; i++ ) {
         pset_size[ i ] = 0;
         pset1_cache[ i ] = NULL;
         pset2_cache[ i ] = NULL;
      }
      next_slot = 0;

/* If required, clean up. */
   } else if( ax1 == -2 ) {
      for( i = 0; i < MAX_CACHE; i++ ) {
         pset_size[ i ] = 0;
         if( pset1_cache[ i ] ) pset1_cache[ i ] = astAnnul( pset1_cache[ i ] );
         if( pset2_cache[ i ] ) pset2_cache[ i ] = astAnnul( pset2_cache[ i ] );
      }
      next_slot = 0;

/* Otherwise do the transformations. */
   } else {

/* See if we have already created PointSets of the correct size. */
      pset1 = NULL;
      for( i = 0; i < MAX_CACHE; i++ ) {
         if( pset_size[ i ] == n ) {
            pset1 = pset1_cache[ i ];
            pset2 = pset2_cache[ i ];
            break;
         }
      }

/* If we have not, create new PointSets now. */
      if( pset1 == NULL ) {
         nin = astGetNin( map );
         pset1 = astPointSet( n, nin, "" );
         ptr1 = astGetPoints( pset1 );

         nout = astGetNout( map );
         pset2 = astPointSet( n, nout, "" );
         ptr2 = astGetPoints( pset2 );

/* Store the input position in the input PointSet. */
         for( i = 0; i < nin; i++ ) {
            xx = at[ i ];
            p = ptr1[ i ];
            for( k = 0; k < n; k++, p++ ) *p = xx;
         }

/* Add these new PointSets to the cache, removing any existing 
   PointSets. */
         if( pset_size[ next_slot ] > 0 ) {
            (void) astAnnul( pset1_cache[ next_slot ] );
            (void) astAnnul( pset2_cache[ next_slot ] );
         }
         pset1_cache[ next_slot ] = pset1;
         pset2_cache[ next_slot ] = pset2;
         pset_size[ next_slot ] = n;
         if( ++next_slot == MAX_CACHE ) next_slot = 0;

/* If existing PointSets were found, get there data arrays. */
      } else {
         ptr1 = astGetPoints( pset1 );
         ptr2 = astGetPoints( pset2 );
      }         

/* Store the input X values in the input PointSet data array. */
      oldx = ptr1[ ax2 ];
      ptr1[ ax2 ] = x;

/* Store the output Y values in the output PointSet data array. */
      oldy = ptr2[ ax1 ];
      ptr2[ ax1 ] = y;

/* Transform the positions. */
      (void) astTransform( map, pset1, 1, pset2 );

/* Re-instate the original arrays in the PointSets. */
      ptr1[ ax2 ] = oldx;
      ptr2[ ax1 ] = oldy;
 
   } 

#undef MAX_CACHE 
}

static double EvaluateDPN( PN *pn, double x ) {
/*
*  Name:
*     EvaluateDPN

*  Purpose:
*     Evaluate the gradient of a polynomial at a given x value.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     static double EvaluateDPN( PN *pn, double x ) {

*  Class Membership:
*     Mapping method.

*  Description:
*     This function evaluates the gradient of the supplied polynomial 
*     at the supplied x value.

*  Parameters:
*     pn
*        Pointer to the structure describing the polynomial.
*     x
*        The x value at which to evaluate the polynomial gradient.

*  Returns:
*     The polynomial gradient value.
*/

/* Local Variables: */
   int n, i;
   double ret;

/* Initialise */
   ret = AST__BAD;

/* Check the global error status. */
   if ( !astOK || !pn ) return ret;

   n = pn->order;

   ret = n*pn->coeff[ n ];
   for( i = n - 1; i > 0; i-- ) {
      ret = i*pn->coeff[ i ] + ret*x;
   }

   return ret;

}

static double EvaluatePN( PN *pn, double x ) {
/*
*  Name:
*     EvaluatePN

*  Purpose:
*     Evaluate a polynomial at a given x value.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     static double EvaluatePN( PN *pn, double x ) {

*  Class Membership:
*     Mapping method.

*  Description:
*     This function evaluates the supplied polynomial at the supplied x
*     value.

*  Parameters:
*     pn
*        Pointer to the structure descirbing the polynomial.
*     x
*        The x value at which to evaluate the polynomial.

*  Returns:
*     The polynomial value.
*/

/* Local Variables: */
   int n, i;
   double ret;

/* Initialise */
   ret = AST__BAD;

/* Check the global error status. */
   if ( !astOK || !pn ) return ret;

   n = pn->order;
   ret = pn->coeff[ n ];
   for( i = n - 1; i >= 0; i-- ) {
      ret = pn->coeff[ i ] + ret*x;
   }

   return ret;

}

static PN *FitPN( AstMapping *map, double *at, int ax1, int ax2, double x0, 
                  double h, double *rms ){
/*
*  Name:
*     FitPN

*  Purpose:
*     Fit a polynomial to the function being differentiated and return
*     the RMS residual.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     PN *FitPN( AstMapping *map, double *at, int ax1, int ax2, double x0, 
*                double h, double *rms )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function finds a polynomial which interpolates a set of evenly
*     spaced samples of the function being differentiated by the astRate
*     method. The coefficients of this polynomial are returned, together
*     with the RMS residual between the polynomial and the function at points
*     mid way between the interpolating points.

*  Parameters:
*     map
*        Pointer to a Mapping which yields the value of the function at x.
*        The Mapping may have any number of inputs and outputs; the specific 
*        output representing the function value, f, is specified by ax1 and 
*        the specific input representing the argument, x, is specified by ax2.
*     at
*        A pointer to an array holding axis values at the position at which 
*        the function is to be evaluated. The number of values supplied
*        must equal the number of inputs to the Mapping. The value supplied 
*        for axis "ax2" is ignored (the value of "x" is used for axis "ax2").
*     ax1 
*        The zero-based index of the Mapping output which is to be
*        differentiated. Set this to -1 to allocate, or -2 to release, 
*        the static resources used by this function.
*     ax2 
*        The zero-based index of the Mapping input which is to be varied.
*     x0
*        The central axis value at which the function is to be evaluated. 
*     h
*        The interval over which the fitting is to be performed.
*     rms
*        A pointer to a location at which to return the RMS residual
*        between the returned polynomial and the function, estimated at
*        points mid way between the interpolating points. May be NULL.

*  Returns:
*     The PN structure holding the polynomial coefficients, etc, or NULL
*     if no polynomial can be fitted. The independant variable of the 
*     polynomial is (x-x0) and the dependant variable is (y(x)-y(x0)).
*     The value of y(x0) is stored in the returned PN structure. The 
*     memory used to store the polynomial should be freed using astFree 
*     when no longer needed.
*/

/* Local Variables: */
   double x[ RATE_ORDER + 2 ], y[ RATE_ORDER + 2 ], dh, off, s2, e;   
   PN *ret;
   int i0, i, n;

/* Initialise */
   ret = NULL;

/* Check the global error status. */
   if ( !astOK ) return ret;

/* Store the x values at (RATE_ORDER+1) evenly spaced points over the interval 
   "h" centred on "x0". */
   i0 = RATE_ORDER/2;
   dh = h/RATE_ORDER;

   for( i = 0; i <= RATE_ORDER; i++ ) {
      x[ i ] = x0 + ( i - i0 )*dh;
   }

/* Get the function values at these positions. */
   FunPN( map, at, ax1, ax2, RATE_ORDER + 1, x, y );

/* Convert the x values into x offsets from "x0", and convert the y
   values into y offsets from the central y value. */
   off = y[ i0 ];
   if( off == AST__BAD ) return NULL;
   for( i = 0; i <= RATE_ORDER; i++ ) {
      if( y[ i ] == AST__BAD ) {
         return NULL;
      } else {
         y[ i ] -= off;
         x[ i ] -= x0;
      }
   }

/* Find the polynomial which interpolates these points. */
   ret = InterpPN( RATE_ORDER + 1, x, y );

/* If required, find the rms error between the polynomial and the 
   function at points mid-way between the interpolating points. */
   if( ret ) {
      ret->y0 = off;
      if( rms ) {

/* Store the x values at which to evaluate the function. These are the
   points mid way between the interpolating points (plus one beyond each
   end). */
         dh *= 0.5;
         for( i = 0; i <= RATE_ORDER; i++ ) {
            x[ i ] += x0 - dh;
         }
         x[ RATE_ORDER + 1 ] = x[ RATE_ORDER ] + 2*dh;

/* Evaluate the function at these positions. */
         FunPN( map, at, ax1, ax2, RATE_ORDER + 2, x, y );

/* Loop round evaluating the polynomial fit and incrementing the sum of
   the squared residuals. */
         s2 = 0.0;
         n = 0;
         for( i = 0; i <= RATE_ORDER + 1; i++ ) {
            if( y[ i ] != AST__BAD ) {
               e = EvaluatePN( ret, x[ i ] - x0 ) + off - y[ i ];
               s2 += e*e;
               n++;
            }
         }
   
/* Evaluate the rms residual. */
         if( n > 1 ) {
            *rms = sqrt( s2/( RATE_ORDER + 2 ) );
         } else {
            *rms = AST__BAD;
         }
      }
   }

   return ret;

}

static void Gauss( double offset, const double params[], int flags,
                   double *value ) {
/*
*  Name:
*     Gauss

*  Purpose:
*     1-dimensional Gaussian spreading kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void Gauss( double offset, const double params[], int flags,
*                 double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     spreading kernel. The function used is exp(-k*x*x).

*  Parameters:
*     offset
*        The offset of a pixel from the central output point, measured
*        in pixels.
*     params
*        The first element of this array should give a value for "k"
*        in the exp(-k*x*x) term.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Calculate the result. */
   *value = exp( -params[ 0 ] * offset * offset );
}

static const char *GetAttrib( AstObject *this_object, const char *attrib ) {
/*
*  Name:
*     GetAttrib

*  Purpose:
*     Get the value of a specified attribute for a Mapping.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     const char *GetAttrib( AstObject *this, const char *attrib )

*  Class Membership:
*     Mapping member function (over-rides the protected astGetAttrib
*     method inherited from the Object class).

*  Description:
*     This function returns a pointer to the value of a specified
*     attribute for a Mapping, formatted as a character string.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     attrib
*        Pointer to a null terminated string containing the name of
*        the attribute whose value is required. This name should be in
*        lower case, with all white space removed.

*  Returned Value:
*     Pointer to a null terminated string containing the attribute
*     value.

*  Notes:
*     - The returned string pointer may point at memory allocated
*     within the Mapping, or at static memory. The contents of the
*     string may be over-written or the pointer may become invalid
*     following a further invocation of the same function or any
*     modification of the Mapping. A copy of the string should
*     therefore be made if necessary.
*     - A NULL pointer will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*/

/* Local Constants: */
#define BUFF_LEN 50              /* Max. characters in result buffer */

/* Local Variables: */
   AstMapping *this;             /* Pointer to the Mapping structure */
   const char *result;           /* Pointer value to return */
   int invert;                   /* Invert attribute value */
   int issimple;                 /* IsSimple attribute value */
   int nin;                      /* Nin attribute value */
   int nout;                     /* Nout attribute value */
   int report;                   /* Report attribute value */
   int tran_forward;             /* TranForward attribute value */
   int tran_inverse;             /* TranInverse attribute value */
   static char buff[ BUFF_LEN + 1 ]; /* Buffer for string result */

/* Initialise. */
   result = NULL;

/* Check the global error status. */   
   if ( !astOK ) return result;

/* Obtain a pointer to the Mapping structure. */
   this = (AstMapping *) this_object;

/* Compare "attrib" with each recognised attribute name in turn,
   obtaining the value of the required attribute. If necessary, write
   the value into "buff" as a null terminated string in an appropriate
   format.  Set "result" to point at the result string. */

/* Invert. */
/* ------- */
   if ( !strcmp( attrib, "invert" ) ) {
      invert = astGetInvert( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", invert );
         result = buff;
      }

/* IsSimple. */
/* --------- */
   } else if ( !strcmp( attrib, "issimple" ) ) {
      issimple = astGetIsSimple( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", issimple );
         result = buff;
      }

/* Nin. */
/* ---- */
   } else if ( !strcmp( attrib, "nin" ) ) {
      nin = astGetNin( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", nin );
         result = buff;
      }

/* Nout. */
/* ----- */
   } else if ( !strcmp( attrib, "nout" ) ) {
      nout = astGetNout( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", nout );
         result = buff;
      }

/* Report. */
/* ------- */
   } else if ( !strcmp( attrib, "report" ) ) {
      report = astGetReport( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", report );
         result = buff;
      }

/* TranForward. */
/* ------------ */
   } else if ( !strcmp( attrib, "tranforward" ) ) {
      tran_forward = astGetTranForward( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", tran_forward );
         result = buff;
      }

/* TranInverse. */
/* ------------ */
   } else if ( !strcmp( attrib, "traninverse" ) ) {
      tran_inverse = astGetTranInverse( this );
      if ( astOK ) {
         (void) sprintf( buff, "%d", tran_inverse );
         result = buff;
      }

/* If the attribute name was not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      result = (*parent_getattrib)( this_object, attrib );
   }

/* Return the result. */
   return result;

/* Undefine macros local to this function. */
#undef BUFF_LEN
}

static int GetNin( AstMapping *this ) {
/*
*+
*  Name:
*     astGetNin

*  Purpose:
*     Get the number of input coordinates for a Mapping.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int astGetNin( AstMapping *this )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function returns the number of input coordinate values
*     required per point by a Mapping (i.e. the number of dimensions
*     of the space in which input points reside).

*  Parameters:
*     this
*        Pointer to the Mapping.

*  Returned Value:
*     Number of coordinate values required.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   int invert;                   /* Invert attribute value */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Determine if the Mapping has been inverted. */
   invert = astGetInvert( this );

/* Obtain the Nin value. */
   if ( astOK ) result = invert ? this->nout : this->nin;

/* Return the result. */
   return result;
}

static int GetNout( AstMapping *this ) {
/*
*+
*  Name:
*     astGetNout

*  Purpose:
*     Get the number of output coordinates for a Mapping.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int astGetNout( AstMapping *this )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function returns the number of output coordinate values
*     generated per point by a Mapping (i.e. the number of dimensions
*     of the space in which output points reside).

*  Parameters:
*     this
*        Pointer to the Mapping.

*  Returned Value:
*     Number of coordinate values generated.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   int invert;                   /* Invert attribute value */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Determine if the Mapping has been inverted. */
   invert = astGetInvert( this );

/* Obtain the Nout value. */
   if ( astOK ) result = invert ? this->nin : this->nout;

/* Return the result. */
   return result;
}

static int GetTranForward( AstMapping *this ) {
/*
*+
*  Name:
*     astGetTranForward

*  Purpose:
*     Determine if a Mapping defines a forward coordinate transformation.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int astGetTranForward( AstMapping *this )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function returns a value indicating whether a Mapping is
*     able to perform a coordinate transformation in the "forward"
*     direction.

*  Parameters:
*     this
*        Pointer to the Mapping.

*  Returned Value:
*     Zero if the forward coordinate transformation is not defined, or
*     1 if it is.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   int invert;                   /* Mapping inverted? */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Determine if the Mapping has been inverted. */
   invert = astGetInvert( this );

/* If OK, obtain the result. */
   if ( astOK ) result = invert ? this->tran_inverse : this->tran_forward;

/* Return the result. */
   return result;
}

static int GetTranInverse( AstMapping *this ) {
/*
*+
*  Name:
*     astGetTranInverse

*  Purpose:
*     Determine if a Mapping defines an inverse coordinate transformation.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int astGetTranInverse( AstMapping *this )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function returns a value indicating whether a Mapping is
*     able to perform a coordinate transformation in the "inverse"
*     direction.

*  Parameters:
*     this
*        Pointer to the Mapping.

*  Returned Value:
*     Zero if the inverse coordinate transformation is not defined, or
*     1 if it is.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   int invert;                   /* Mapping inverted? */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Determine if the Mapping has been inverted. */
   invert = astGetInvert( this );

/* If OK, obtain the result. */
   if ( astOK ) result = invert ? this->tran_forward : this->tran_inverse;

/* Return the result. */
   return result;
}

static void GlobalBounds( MapData *mapdata, double *lbnd, double *ubnd,
                          double xl[], double xu[] ) {
/*
*  Name:
*     GlobalBounds

*  Purpose:
*     Estimate global coordinate bounds for a Mapping.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void GlobalBounds( MapData *mapdata, double *lbnd, double *ubnd,
*                        double xl[], double xu[] );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function estimates the global lower and upper bounds of a
*     Mapping function within a constrained region of its input
*     coordinate space. It uses a robust global optimisation algorithm
*     based on the selection of pseudo-random starting positions,
*     followed by the location of local minima and maxima using the
*     downhill (or uphill) simplex method. The algorithm will cope
*     with the case where there are several competing minima (or
*     maxima) with nearly equal values. It attempts to locate the
*     global bounds to full machine precision when possible.

*  Parameters:
*     mapdata
*        Pointer to a MapData structure describing the Mapping
*        function, its coordinate constraints, etc.
*     lbnd
*        Pointer to a double.  On entry, this should contain a
*        previously-obtained upper limit on the global lower bound, or
*        AST__BAD if no such limit is available. On exit, it will be
*        updated with a new estimate of the global lower bound, if a
*        better one has been found.
*     ubnd
*        Pointer to a double.  On entry, this should contain a
*        previously-obtained lower limit on the global upper bound, or
*        AST__BAD if no such limit is available. On exit, it will be
*        updated with a new estimate of the global upper bound, if a
*        better one has been found.
*     xl
*        Pointer to an array of double, with one element for each
*        input coordinate. On entry, if *lbnd is not equal to AST__OK,
*        this should contain the input coordinates of a point at which
*        the Mapping function takes the value *lbnd. On exit, this
*        function returns the position of a (not necessarily unique)
*        input point at which the Mapping function takes the value of
*        the new global lower bound.  This array is not altered if an
*        improved estimate of the global lower bound cannot be found.
*     xu
*        Pointer to an array of double, with one element for each
*        input coordinate. On entry, if *ubnd is not equal to AST__OK,
*        this should contain the input coordinates of a point at which
*        the Mapping function takes the value *ubnd. On exit, this
*        function returns the position of a (not necessarily unique)
*        input point at which the Mapping function takes the value of
*        the new global upper bound.  This array is not altered if an
*        improved estimate of the global upper bound cannot be found.

*  Notes:
*     - The efficiency of this function will usually be improved if
*     previously-obtained estimates of the extrema and their locations
*     are provided.
*     - The values returned via "lbnd", "ubnd", "xl" and "xu" will be
*     set to the value AST__BAD if this function should fail for any
*     reason. Their initial values on entry will not be altered if the
*     function is invoked with the global error status set.
*/

/* Local Constants: */
   const double default_acc = 3.0e-5; /* Default convergence accuracy */
   const int maxiter = 10000;    /* Maximum number of iterations */
   const int minsame = 5;        /* Minimum no. consistent extrema required */
   const int nbatch = 32;        /* No. function samples obtained per batch */

/* Local Variables: */
   AstPointSet *pset_in;         /* Input PointSet for batch transformation */
   AstPointSet *pset_out;        /* Output PointSet for batch transformation */
   double **ptr_in;              /* Pointer to batch input coordinates */
   double **ptr_out;             /* Pointer to batch output coordinates */
   double *active_hi;            /* Estimated upper limits of active region */
   double *active_lo;            /* Estimated lower limits of active region */
   double *sample_hi;            /* Upper limits of sampled region */
   double *sample_lo;            /* Lower limits of sampled region */
   double *sample_width;         /* Nominal widths of sampled region */
   double *x;                    /* Pointer to array of coordinates */
   double acc;                   /* Convergence accuracy for finding maximum */
   double active_width;          /* Estimated width of active region */
   double new_max;               /* Value of new local maximum */
   double new_min;               /* Value of new local minimum */
   double oversize;              /* Over-size factor for sampled region */
   double random;                /* Pseudo-random number */
   int bad;                      /* Transformed position is bad? */
   int batch;                    /* Next element to use in position batch */
   int coord;                    /* Loop counter for coordinates */
   int done_max;                 /* Satisfactory global maximum found? */
   int done_min;                 /* Satisfactory global minimum found? */
   int iter;                     /* Loop counter for iterations */
   int ncall;                    /* Number of Mapping function calls (junk) */
   int ncoord;                   /* Number of coordinates in search space */
   int nmax;                     /* Number of local maxima found */
   int nmin;                     /* Number of local minima found */
   int nsame_max;                /* Number of equivalent local maxima found */
   int nsame_min;                /* Number of equivalent local minima found */
   long int seed = 1776655449;   /* Arbitrary pseudo-random number seed */

/* Check the global error status */
   if ( !astOK ) return;

/* Initialise. */
   done_max = 0;
   done_min = 0;
   ncall = 0;
   nmax = 0;
   nmin = 0;
   nsame_max = 0;
   nsame_min = 0;
   pset_in = NULL;
   pset_out = NULL;
   ptr_in = NULL;
   ptr_out = NULL;
   oversize = 0;
   bad = 0;

/* Extract the number of input coordinates for the Mapping function
   and allocate workspace. */
   ncoord = mapdata->nin;
   active_hi = astMalloc( sizeof( double ) * (size_t) ncoord );
   active_lo = astMalloc( sizeof( double ) * (size_t) ncoord );
   sample_hi = astMalloc( sizeof( double ) * (size_t) ncoord );
   sample_lo = astMalloc( sizeof( double ) * (size_t) ncoord );
   sample_width = astMalloc( sizeof( double ) * (size_t) ncoord );
   x = astMalloc( sizeof( double ) * (size_t) ncoord );
   if ( astOK ) {

/* Calculate the factor by which the size of the region we sample will
   exceed the size of the Mapping function's active region (the region
   where the transformed coordinates are non-bad) in each
   dimension. This is chosen so that the volume ratio will be 2. */
      oversize = pow( 2.0, 1.0 / (double) ncoord );

/* Initialise the limits of the active region to unknown. */
      for ( coord = 0; coord < ncoord; coord++ ) {
         active_lo[ coord ] = DBL_MAX;;
         active_hi[ coord ] = -DBL_MAX;

/* Initialise the nominal widths of the sampled region to be the
   actual widths of the search region times the over-size factor. */
         sample_width[ coord ] = ( mapdata->ubnd[ coord ] -
                                   mapdata->lbnd[ coord ] ) * oversize;

/* Initialise the sampled region to match the search region. */
         sample_lo[ coord ] = mapdata->lbnd[ coord ];
         sample_hi[ coord ] = mapdata->ubnd[ coord ];
      }

/* Set up position buffer. */
/* ======================= */
/* Create two PointSets to act as buffers to hold a complete batch of
   input and output coordinates. Obtain pointers to their coordinate
   arrays. */
      pset_in = astPointSet( nbatch, ncoord, "" );
      pset_out = astPointSet( nbatch, mapdata->nout, "" );
      ptr_in = astGetPoints( pset_in );
      ptr_out = astGetPoints( pset_out );

/* Initialise the next element to be used in the position buffer to
   indicate that the buffer is initially empty. */
      batch = nbatch;
   }

/* Define a macro to fill the position buffer with a set of
   pseudo-random positions and to transform them. */
#define FILL_POSITION_BUFFER {\
\
/* We first generate a suitable volume over which to distribute the\
   batch of pseudo-random positions. Initially, this will be the\
   entire search volume, but if we find that the only non-bad\
   transformed coordinates we obtain are restricted to a small\
   sub-region of this input volume, then we reduce the sampled volume\
   so as to concentrate more on the active region. */\
\
/* Loop through each input coordinate, checking that at least one\
   non-bad transformed point has been obtained. If not, we do not\
   adjust the sampled volume, as we do not yet know where the active\
   region lies. */\
   for ( coord = 0; coord < ncoord; coord++ ) {\
      if ( active_hi[ coord ] >= active_lo[ coord ] ) {\
\
/* Estimate the width of the active region from the range of input\
   coordinates that have so far produced non-bad transformed\
   coordinates. */\
         active_width = active_hi[ coord ] - active_lo[ coord ];\
\
/* If the current width of the sampled volume exceeds this estimate by\
   more than the required factor, then reduce the width of the sampled\
   volume. The rate of reduction is set so that the volume of the\
   sampled region can halve with every fourth batch of positions. */\
         if ( ( active_width * oversize ) < sample_width[ coord ] ) {\
            sample_width[ coord ] /= pow( oversize, 0.25 );\
\
/* If the width of the sampled volume does not exceed that of the\
   known active region by the required factor, then adjust it so that\
   it does. Note that we must continue to sample some points outside\
   the known active region in case we have missed any (in which case\
   the sampled region will expand again to include them). */\
         } else if ( ( active_width * oversize ) > sample_width[ coord ] ) {\
            sample_width[ coord ] = active_width * oversize;\
         }\
\
/* Calculate the lower and upper bounds on the sampled volume, using\
   the new width calculated above and centring it on the active\
   region, as currently known. */\
         sample_lo[ coord ] = ( active_lo[ coord ] + active_hi[ coord ] -\
                                sample_width[ coord ] ) * 0.5;\
         sample_hi[ coord ] = ( active_lo[ coord ] + active_hi[ coord ] +\
                                sample_width[ coord ] ) * 0.5;\
\
/* Ensure that the sampled region does not extend beyond the original\
   search region. */\
         if ( sample_lo[ coord ] < mapdata->lbnd[ coord ] ) {\
            sample_lo[ coord ] = mapdata->lbnd[ coord ];\
         }\
         if ( sample_hi[ coord ] > mapdata->ubnd[ coord ] ) {\
            sample_hi[ coord ] = mapdata->ubnd[ coord ];\
         }\
      }\
   }\
\
/* Having determined the size of the sampled volume, create a batch of\
   pseudo-random positions uniformly distributed within it. */\
   for ( batch = 0; batch < nbatch; batch++ ) {\
      for ( coord = 0; coord < ncoord; coord++ ) {\
         random = Random( &seed );\
         ptr_in[ coord ][ batch ] = sample_lo[ coord ] * random +\
                                    sample_hi[ coord ] * ( 1.0 - random );\
      }\
   }\
\
/* Transform these positions. We process them in a single batch in\
   order to minimise the overheads in doing this. */\
   (void) astTransform( mapdata->mapping, pset_in, mapdata->forward,\
                        pset_out );\
\
/* Indicate that the position buffer is now full. */\
   batch = 0;\
}

/* Fill the position buffer using the above macro. (Note that because
   we do not yet have an estimate of the size of the active region,
   this does not change the sampled region size from our earlier
   initialised values. */
   FILL_POSITION_BUFFER;

/* Iterate. */
/* ======== */
/* Loop to perform up to "maxiter" iterations to estimate the global
   minimum and maximum. */
   for ( iter = 0; astOK && ( iter < maxiter ); iter++ ) {

/* Determine the search accuracy. */
/* ============================== */
/* Decide the accuracy to which local extrema should be found. The
   intention here is to optimise performance, especially where one
   extremum lies near zero and so could potentially be found to
   unnecessarily high precision. If we make a mis-assumption (the code
   below is not fool-proof), we will slow things down for this
   iteration, but the error will be corrected in future iterations
   once better estimates are available. */

/* If we have no current estimate of either global extremum, we assume
   the values we eventually obtain will be of order unity and required
   to the default accuracy. */
      acc = default_acc;

/* If we already have an estimate of both global extrema, we set the
   accuracy level so that the difference between them will be known to
   the default accuracy. */
      if ( ( *lbnd != AST__BAD ) && ( *ubnd != AST__BAD ) ) {
         acc = fabs( *ubnd - *lbnd ) * default_acc;

/* If we have an estimate of only one global extremum, we assume that
   the difference between the two global extrema will eventually be of
   the same order as the estimate we currently have, so long as this
   is not less than unity. */
      } else if ( *lbnd != AST__BAD ) {
         if ( fabs( *lbnd ) > 1.0 ) acc = fabs( *lbnd) * default_acc;
      } else if ( *ubnd != AST__BAD ) {
         if ( fabs( *ubnd ) > 1.0 ) acc = fabs( *ubnd) * default_acc;
      }

/* Search for a new local minimum. */
/* =============================== */
/* If we are still searching for the global minimum, then obtain a set
   of starting coordinates from which to find a new local minimum. */
      if ( !done_min ) {

/* On the first iteration, start searching at the position where the
   best estimate of the global minimum (if any) has previously been
   found. We know that this produces non-bad transformed
   coordinates. */
         bad = 0;
         if ( !iter && ( *lbnd != AST__BAD ) ) {
            for ( coord = 0; coord < ncoord; coord++ ) {
               x[ coord ] = xl[ coord ];
            }

/* Otherwise, if no estimate of the global minimum is available, then
   start searching at the position where the best estimate of the
   global maximum (if any) has been found. This may be a long way from
   a local minimum, but at least it will yield a non-bad value for the
   Mapping function, so some sort of estimate of the global minimum
   will be obtained. This is important in cases where finding the
   active region of the function is the main problem. Note that this
   condition can only occur once, since the global minimum will have
   an estimate on the next iteration. */
         } else if ( ( *lbnd == AST__BAD ) && ( *ubnd != AST__BAD ) ) {
            for ( coord = 0; coord < ncoord; coord++ ) {
               x[ coord ] = xu[ coord ];
            }

/* Having exhausted the above possibilities, we use pseudo-random
   starting positions which are uniformly distributed throughout the
   search volume. First check to see if the buffer containing such
   positions is empty and refill it if necessary. */
         } else {
            if ( batch >= nbatch ) FILL_POSITION_BUFFER;

/* Test the next available set of output (transformed) coordinates in
   the position buffer to see if they are bad. */
            if ( astOK ) {
               for ( coord = 0; coord < mapdata->nout; coord++ ) {
                  bad = ( ptr_out[ coord ][ batch ] == AST__BAD );
                  if ( bad ) break;
               }

/* If not, we have a good starting position for finding a local
   minimum, so extract the corresponding input coordinates. */
               if ( !bad ) {
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     x[ coord ] = ptr_in[ coord ][ batch ];
                  }
               }

/* Increment the position buffer location. */
               batch++;
            }
         }

/* If we do not have a good starting position, we can't do anything
   more on this iteration. A new position will be obtained and tested
   on the next iteration and this (we hope) will eventually identify a
   suitable starting point. */
         if ( astOK && !bad ) {

/* Form estimates of the lower and upper limits of the active region
   from the starting positions used. */
            for ( coord = 0; coord < ncoord; coord++ ) {
               if ( x[ coord ] < active_lo[ coord ] ) {
                  active_lo[ coord ] = x[ coord ];
               }
               if ( x[ coord ] > active_hi[ coord ] ) {
                  active_hi[ coord ] = x[ coord ];
               }
            }

/* Indicate that the Mapping function should be negated (because we
   want a local minimum) and then search for a local maximum in this
   negated function. If the result is non-bad (as it should always be,
   barring an error), then negate it to obtain the value of the local
   minimum found. */
            mapdata->negate = 1;
            new_min = LocalMaximum( mapdata, acc, 0.01, x );
            if ( new_min != AST__BAD ) {
               new_min = -new_min;

/* Update the estimates of the lower and upper bounds of the active
   region to take account of where the minimum was found. */
               for ( coord = 0; coord < ncoord; coord++ ) {
                  if ( x[ coord ] < active_lo[ coord ] ) {
                     active_lo[ coord ] = x[ coord ];
                  }
                  if ( x[ coord ] > active_hi[ coord ] ) {
                     active_hi[ coord ] = x[ coord ];
                  }
               }

/* Count the number of times we successfully locate a local minimum
   (ignoring the fact they might all be the same one). */
               nmin++;

/* Update the global minimum. */
/* ========================== */
/* If this is the first estimate of the global minimum, then set to
   one the count of the number of consecutive iterations where this
   estimate remains unchanged. Store the minimum value and its
   position. */
               if ( *lbnd == AST__BAD ) {
                  nsame_min = 1;
                  *lbnd = new_min;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     xl[ coord ] = x[ coord ];
                  }

/* Otherwise, test if this local minimum is lower than the previous
   estimate of the global minimum. If so, then reset the count of
   unchanged estimates of the global mimimum to one if the difference
   exceeds the accuracy with which the minimum was found (i.e. if we
   have found a significantly different minimum). Otherwise, just
   increment this count (because we have found the same minimum but by
   chance with slightly improved accuracy). Store the new minimum and
   its position. */
               } else if ( new_min < *lbnd ) {
                  nsame_min = ( ( *lbnd - new_min ) > acc ) ? 1 :
                                                              nsame_min + 1;
                  *lbnd = new_min;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     xl[ coord ] = x[ coord ];
                  }

/* If the latest local minimum is no improvement on previous estimates
   of the global minimum, then increment the count of unchanged
   estimates of the global mimimum, but do not save the new one. */
               } else {
                  nsame_min++;
               }

/* Determine if a satisfactory estimate of the global minimum has been
   obtained.  It has if the number of consecutive local minima which
   have not significantly improved the estimate is at least equal to
   "minsame", and at least 30% of the total number of local minima
   found. */
               if ( ( nsame_min >= minsame ) &&
                    ( nsame_min >= (int) ( 0.3f * (float) nmin + 0.5f ) ) ) {
                  done_min = 1;
               }
            }
         }
      }

/* Search for a new local maximum. */
/* =============================== */
/* Now repeat all of the above to find a new local maximum which
   estimates the global maximum. */
      if ( !done_max ) {

/* Choose a suitable starting position, based on one already available
   if appropriate. */
         if ( !iter && ( *ubnd != AST__BAD ) ) {
            for ( coord = 0; coord < ncoord; coord++ ) {
               x[ coord ] = xu[ coord ];
            }

         } else if ( ( *ubnd == AST__BAD ) && ( *lbnd != AST__BAD ) ) {
            for ( coord = 0; coord < ncoord; coord++ ) {
               x[ coord ] = xl[ coord ];
            }

/* Otherwise use a pseudo-random position, refilling the position
   buffer if necessary. Check if the transformed coordinates are
   bad. */
         } else {
            if ( batch >= nbatch ) FILL_POSITION_BUFFER;
            if ( astOK ) {
               for ( coord = 0; coord < mapdata->nout; coord++ ) {
                  bad = ( ptr_out[ coord ][ batch ] == AST__BAD );
                  if ( bad ) break;
               }
               if ( !bad ) {
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     x[ coord ] = ptr_in[ coord ][ batch ];
                  }
               }
               batch++;
            }
         }

/* If the coordinates are OK, update the active region limits. */
         if ( astOK && !bad ) {
            for ( coord = 0; coord < ncoord; coord++ ) {
               if ( x[ coord ] < active_lo[ coord ] ) {
                  active_lo[ coord ] = x[ coord ];
               }
               if ( x[ coord ] > active_hi[ coord ] ) {
                  active_hi[ coord ] = x[ coord ];
               }
            }

/* Find a local maximum in the Mapping function. */
            mapdata->negate = 0;
            new_max = LocalMaximum( mapdata, acc, 0.01, x );
            if ( new_max != AST__BAD ) {

/* Use the result to further update the active region limits. */
               for ( coord = 0; coord < ncoord; coord++ ) {
                  if ( x[ coord ] < active_lo[ coord ] ) {
                     active_lo[ coord ] = x[ coord ];
                  }
                  if ( x[ coord ] > active_hi[ coord ] ) {
                     active_hi[ coord ] = x[ coord ];
                  }
               }

/* Count the number of local maxima found. */
               nmax++;

/* Update the estimate of the global maximum. */
               if ( *ubnd == AST__BAD ) {
                  nsame_max = 1;
                  *ubnd = new_max;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     xu[ coord ] = x[ coord ];
                  }

               } else if ( new_max > *ubnd ) {
                  nsame_max = ( ( new_max - *ubnd ) > acc ) ? 1 :
                                                              nsame_max + 1;
                  *ubnd = new_max;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     xu[ coord ] = x[ coord ];
                  }

               } else {
                  nsame_max++;
               }

/* Test for a satisfactory global maximum estimate. */
               if ( ( nsame_max >= minsame ) &&
                    ( nsame_max >= (int) ( 0.3f * (float) nmax + 0.5 ) ) ) {
                  done_max = 1;
               }
            }
         }
      }

/* Quit iterating once both the global minimum and the global maximum
   have been found. */
      if ( done_min && done_max ) break;
   }

/* Free workspace. */
   active_hi = astFree( active_hi );
   active_lo = astFree( active_lo );
   sample_hi = astFree( sample_hi );
   sample_lo = astFree( sample_lo );
   sample_width = astFree( sample_width );
   x = astFree( x );

/* Annul temporary PointSets. */
   pset_in = astAnnul( pset_in );
   pset_out = astAnnul( pset_out );

/* If the global minimum has been found, attempt to polish the result
   to machine precision by requesting that it be found with an
   accuracy tolerance of zero (subject to the maximum number of
   iterations that LocalMaximum will perform,). */
   if ( astOK ) {
      if ( *lbnd != AST__BAD ) {
         mapdata->negate = 1;
         *lbnd = LocalMaximum( mapdata, 0.0, sqrt( DBL_EPSILON ), xl );
         if ( *lbnd != AST__BAD ) *lbnd = - *lbnd;
      }

/* Similarly polish the estimate of the global maximum. */
      if ( *ubnd != AST__BAD ) {
         mapdata->negate = 0;
         *ubnd = LocalMaximum( mapdata, 0.0, sqrt( DBL_EPSILON ), xu );
      }

/* If either extremum could not be found, then report an error. */
      if ( ( *lbnd == AST__BAD ) || ( *ubnd == AST__BAD ) ) {
         astError( AST__MBBNF, "astMapBox(%s): No valid output coordinates "
                   "(after %d test points).", astGetClass( mapdata->mapping ),
                   2 * maxiter );
      }

/* If an error occurred, then return bad extremum values and
   coordinates. */
      if ( !astOK ) {
         *lbnd = AST__BAD;
         *ubnd = AST__BAD;
         for ( coord = 0; coord < ncoord; coord++ ) {
            xl[ coord ] = AST__BAD;
            xu[ coord ] = AST__BAD;
         }
      }
   }

/* Undefine macros local to this function. */
#undef FILL_POSITION_BUFFER
}

void astInitMappingVtab_(  AstMappingVtab *vtab, const char *name ) {
/*
*+
*  Name:
*     astInitMappingVtab

*  Purpose:
*     Initialise a virtual function table for a Mapping.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "mapping.h"
*     void astInitMappingVtab( AstMappingVtab *vtab, const char *name )

*  Class Membership:
*     Mapping vtab initialiser.

*  Description:
*     This function initialises the component of a virtual function
*     table which is used by the Mapping class.

*  Parameters:
*     vtab
*        Pointer to the virtual function table. The components used by
*        all ancestral classes will be initialised if they have not already
*        been initialised.
*     name
*        Pointer to a constant null-terminated character string which contains
*        the name of the class to which the virtual function table belongs (it 
*        is this pointer value that will subsequently be returned by the Object
*        astClass function).
*-
*/

/* Local Variables: */
   AstObjectVtab *object;        /* Pointer to Object component of Vtab */

/* Check the local error status. */
   if ( !astOK ) return;

/* Initialize the component of the virtual function table used by the
   parent class. */
   astInitObjectVtab( (AstObjectVtab *) vtab, name );

/* Store a unique "magic" value in the virtual function table. This
   will be used (by astIsAMapping) to determine if an object belongs
   to this class.  We can conveniently use the address of the (static)
   class_init variable to generate this unique value. */
   vtab->check = &class_init;

/* Initialise member function pointers. */
/* ------------------------------------ */
/* Store pointers to the member functions (implemented here) that provide
   virtual methods for this class. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
   vtab->ResampleLD = ResampleLD;
   vtab->RebinLD = RebinLD;
   vtab->RebinSeqLD = RebinSeqLD;
#endif
   vtab->ClearInvert = ClearInvert;
   vtab->ClearReport = ClearReport;
   vtab->Decompose = Decompose;
   vtab->GetInvert = GetInvert;
   vtab->GetIsSimple = GetIsSimple;
   vtab->GetNin = GetNin;
   vtab->GetNout = GetNout;
   vtab->GetReport = GetReport;
   vtab->GetTranForward = GetTranForward;
   vtab->GetTranInverse = GetTranInverse;
   vtab->Invert = Invert;
   vtab->LinearApprox = LinearApprox;
   vtab->MapBox = MapBox;
   vtab->MapList = MapList;
   vtab->MapMerge = MapMerge;
   vtab->MapSplit = MapSplit;
   vtab->Rate = Rate;
   vtab->ReportPoints = ReportPoints;
   vtab->RebinD = RebinD;
   vtab->RebinF = RebinF;
   vtab->RebinI = RebinI;
   vtab->RebinSeqD = RebinSeqD;
   vtab->RebinSeqF = RebinSeqF;
   vtab->RebinSeqI = RebinSeqI;
   vtab->ResampleB = ResampleB;
   vtab->ResampleD = ResampleD;
   vtab->ResampleF = ResampleF;
   vtab->ResampleI = ResampleI;
   vtab->ResampleL = ResampleL;
   vtab->ResampleS = ResampleS;
   vtab->ResampleUB = ResampleUB;
   vtab->ResampleUI = ResampleUI;
   vtab->ResampleUL = ResampleUL;
   vtab->ResampleUS = ResampleUS;
   vtab->SetInvert = SetInvert;
   vtab->SetReport = SetReport;
   vtab->Simplify = Simplify;
   vtab->TestInvert = TestInvert;
   vtab->TestReport = TestReport;
   vtab->Tran1 = Tran1;
   vtab->Tran2 = Tran2;
   vtab->TranGrid = TranGrid;
   vtab->TranN = TranN;
   vtab->TranP = TranP;
   vtab->Transform = Transform;

/* Save the inherited pointers to methods that will be extended, and
   replace them with pointers to the new member functions. */
   object = (AstObjectVtab *) vtab;

   parent_clearattrib = object->ClearAttrib;
   object->ClearAttrib = ClearAttrib;
   parent_getattrib = object->GetAttrib;
   object->GetAttrib = GetAttrib;
   parent_setattrib = object->SetAttrib;
   object->SetAttrib = SetAttrib;
   parent_testattrib = object->TestAttrib;
   object->TestAttrib = TestAttrib;
   parent_equal = object->Equal;
   object->Equal = Equal;

/* Declare the destructor, copy constructor and dump function. */
   astSetDelete( vtab, Delete );
   astSetCopy( vtab, Copy );
   astSetDump( vtab, Dump, "Mapping", "Mapping between coordinate systems" );
}

/*
*  Name:
*     InterpolateKernel1<X>

*  Purpose:
*     Resample a data grid, using a 1-d interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int InterpolateKernel1<X>( AstMapping *this, int ndim_in,
*                                const int *lbnd_in, const int *ubnd_in,
*                                const <Xtype> *in, const <Xtype> *in_var,
*                                int npoint, const int *offset,
*                                const double *const *coords,
*                                void (* kernel)( double, const double [], int,
*                                                 double * ),
*                                int neighb, const double *params, int flags,
*                                <Xtype> badval,
*                                <Xtype> *out, <Xtype> *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which resample a rectangular input
*     grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. Each output
*     grid point may be mapped on to a position in the input grid in
*     an arbitrary way. The input and output grids may have any number
*     of dimensions, not necessarily equal.
*
*     Where the positions given do not correspond with a pixel centre
*     in the input grid, interpolation is performed using a weighted
*     sum of the surrounding pixel values. The weights are determined
*     by a separable kernel which is the product of a 1-dimensional
*     kernel function evaluated along each input dimension. A pointer
*     should be supplied to the 1-dimensional kernel function to be
*     used.

*  Parameters:
*     this
*        Pointer to the Mapping being used in the resampling operation
*        (this is only used for constructing error messages).
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input grid, its extent along a particular
*        (i'th) dimension being ubnd_in[i]-lbnd_in[i]+1 (assuming "i"
*        is zero-based). They also define the input grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be resampled (with an element
*        for each pixel in the input grid). The numerical type of
*        these data should match the function used, as given by the
*        suffix on the function name. The storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
*        (i.e. Fortran array storage order).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of points at which the input grid is to be
*        resampled.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each output point, this array should contain the zero-based
*        offset in the output array(s) (i.e. the "out" and,
*        optionally, the "out_var" arrays) at which the resampled
*        output value(s) should be stored.
*     coords
*        An array of pointers to double, with "ndim_in"
*        elements. Element "coords[coord]" should point at the first
*        element of an array of double (with "npoint" elements) which
*        contains the values of coordinate number "coord" for each
*        interpolation point. The value of coordinate number "coord"
*        for interpolation point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices to be
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding output data (and
*        variance) will be set to the value given by "badval".
*     kernel
*        Pointer to the 1-dimensional kernel function to be used.
*     neighb
*        The number of neighbouring pixels in each dimension (on each
*        side of the interpolation position) which are to contribute
*        to the interpolated value. This value should be at least 1.
*     params
*        Pointer to an optional array of parameter values to be passed
*        to the interpolation kernel function. If no parameters are
*        required by this function, then a NULL pointer may be
*        supplied.
*     flags
*        The bitwise OR of a set of flag values which provide
*        additional control over the resampling operation.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned. Note
*        that details of how the output grid maps on to this array
*        (e.g. the storage order, number of dimensions, etc.) is
*        arbitrary and is specified entirely by means of the "offset"
*        array. The "out" array should therefore contain sufficient
*        elements to accommodate the "offset" values supplied.  There
*        is no requirement that all elements of the "out" array should
*        be assigned values, and any which are not addressed by the
*        contents of the "offset" array will be left unchanged.
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given. It is addressed in
*        exactly the same way (via the "offset" array) as the "out"
*        array. The values returned are estimates of the statistical
*        variance of the corresponding values in the "out" array, on
*        the assumption that all errors in input grid values (in the
*        "in" array) are statistically independent and that their
*        variance estimates (in the "in_var" array) may simply be
*        summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.

*  Returned Value:
*     The number of output grid points to which a data value (or a
*     variance value if relevant) equal to "badval" has been assigned
*     because no valid output value could be obtained.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*     - A value of zero will be returned if any of these functions is
*     invoked with the global error status set, or if it should fail
*     for any reason.
*/
/* Define macros to implement the function for a specific data
   type. */
#define MAKE_INTERPOLATE_KERNEL1(X,Xtype,Xfloating,Xfloattype,Xsigned) \
static int InterpolateKernel1##X( AstMapping *this, int ndim_in, \
                                  const int *lbnd_in, const int *ubnd_in, \
                                  const Xtype *in, const Xtype *in_var, \
                                  int npoint, const int *offset, \
                                  const double *const *coords, \
                                  void (* kernel)( double, const double [], \
                                                   int, double * ), \
                                  int neighb, const double *params, \
                                  int flags, Xtype badval, \
                                  Xtype *out, Xtype *out_var ) { \
\
/* Local Variables: */ \
   Xfloattype hi_lim;            /* Upper limit on output values */ \
   Xfloattype lo_lim;            /* Lower limit on output values */ \
   Xfloattype sum;               /* Weighted sum of pixel data values */ \
   Xfloattype sum_var;           /* Weighted sum of pixel variance values */ \
   Xfloattype val;               /* Data value to be assigned to output */ \
   Xfloattype val_var;           /* Variance to be assigned to output */ \
   Xfloattype wtsum;             /* Sum of weight values */ \
   Xfloattype wtsum_sq;          /* Square of sum of weights */ \
   Xtype var;                    /* Variance value */ \
   double **wtptr;               /* Pointer to array of weight pointers */ \
   double **wtptr_last;          /* Array of highest weight pointer values */ \
   double *kval;                 /* Pointer to array of kernel values */ \
   double *wtprod;               /* Accumulated weight value array pointer */ \
   double *xn_max;               /* Pointer to upper limits array (n-d) */ \
   double *xn_min;               /* Pointer to lower limits array (n-d) */ \
   double pixwt;                 /* Weight to apply to individual pixel */ \
   double wt_y;                  /* Value of y-dependent pixel weight */ \
   double x;                     /* x coordinate value */ \
   double xmax;                  /* x upper limit */ \
   double xmin;                  /* x lower limit */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   double ymax;                  /* y upper limit */ \
   double ymin;                  /* y lower limit */ \
   int *hi;                      /* Pointer to array of upper indices */ \
   int *lo;                      /* Pointer to array of lower indices */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int bad_var;                  /* Output variance bad? */ \
   int done;                     /* All pixel indices done? */ \
   int hi_x;                     /* Upper pixel index (x dimension) */ \
   int hi_y;                     /* Upper pixel index (y dimension) */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ii;                       /* Loop counter for dimensions */ \
   int ix;                       /* Pixel index in input grid x dimension */ \
   int ixn;                      /* Pixel index in input grid (n-d) */ \
   int iy;                       /* Pixel index in input grid y dimension */ \
   int kerror;                   /* Error signalled by kernel function? */ \
   int lo_x;                     /* Lower pixel index (x dimension) */ \
   int lo_y;                     /* Lower pixel index (y dimension) */ \
   int off1;                     /* Input pixel offset due to y index */ \
   int off_in;                   /* Offset to input pixel */ \
   int off_out;                  /* Offset to output pixel */ \
   int pixel;                    /* Offset to input pixel containing point */ \
   int point;                    /* Loop counter for output points */ \
   int result;                   /* Result value to return */ \
   int s;                        /* Temporary variable for strides */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int ystride;                  /* Stride along input grid y dimension */ \
\
/* Initialise. */ \
   result = 0; \
\
/* Check the global error status. */ \
   if ( !astOK ) return result; \
\
/* Further initialisation. */ \
   kerror = 0; \
   sum_var = 0; \
   val = 0; \
   val_var = 0; \
   wtsum = 0; \
   bad = 0; \
   bad_var = 0; \
   sum = 0.0; \
\
/* Determine if we are processing bad pixels or variances. */ \
   usebad = flags & AST__USEBAD; \
   usevar = in_var && out_var; \
\
/* Set up limits for checking output values to ensure that they do not \
   overflow the range of the data type being used. */ \
   lo_lim = LO_##X; \
   hi_lim = HI_##X; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   if ( ndim_in == 1 ) { \
\
/* Calculate the coordinate limits of the input grid. */ \
      xmin = (double) lbnd_in[ 0 ] - 0.5; \
      xmax = (double) ubnd_in[ 0 ] + 0.5; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,0) \
            } \
         } \
      } \
\
/* Exit point on error in kernel function */ \
      Kernel_Error_1d: ; \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   } else if ( ndim_in == 2 ) { \
\
/* Allocate workspace. */ \
      kval = astMalloc( sizeof( double ) * (size_t) ( 2 * neighb ) ); \
      if ( astOK ) { \
\
/* Calculate the stride along the y dimension of the input grid. */ \
         ystride = ubnd_in[ 0 ] - lbnd_in[ 0 ] + 1; \
\
/* Calculate the coordinate limits of the input grid in each \
   dimension. */ \
         xmin = (double) lbnd_in[ 0 ] - 0.5; \
         xmax = (double) ubnd_in[ 0 ] + 0.5; \
         ymin = (double) lbnd_in[ 1 ] - 0.5; \
         ymax = (double) ubnd_in[ 1 ] + 0.5; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
         if ( usebad ) { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,0) \
               } \
            } \
         } else { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,0) \
               } \
            } \
         } \
\
/* Exit point on error in kernel function */ \
         Kernel_Error_2d: ; \
      } \
\
/* Free the workspace. */ \
      kval = astFree( kval ); \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
   } else { \
\
/* Allocate workspace. */ \
      hi = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      lo = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      stride = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      xn_max = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      xn_min = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      kval = astMalloc( sizeof( double ) * (size_t) \
                                           ( 2 * neighb * ndim_in ) ); \
      wtprod = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      wtptr = astMalloc( sizeof( double * ) * (size_t) ndim_in ); \
      wtptr_last = astMalloc( sizeof( double * ) * (size_t) ndim_in ); \
      if ( astOK ) { \
\
/* Calculate the stride along each dimension of the input grid. */ \
         for ( s = 1, idim = 0; idim < ndim_in; idim++ ) { \
            stride[ idim ] = s; \
            s *= ubnd_in[ idim ] - lbnd_in[ idim ] + 1; \
\
/* Calculate the coordinate limits of the input grid in each \
   dimension. */ \
            xn_min[ idim ] = (double) lbnd_in[ idim ] - 0.5; \
            xn_max[ idim ] = (double) ubnd_in[ idim ] + 0.5; \
         } \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
         if ( usebad ) { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,0) \
               } \
            } \
         } else { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,0) \
               } \
            } \
         } \
\
/* Exit point on error in kernel function */ \
         Kernel_Error_Nd: ;\
      } \
\
/* Free the workspace. */ \
      hi = astFree( hi ); \
      lo = astFree( lo ); \
      stride = astFree( stride ); \
      xn_max = astFree( xn_max ); \
      xn_min = astFree( xn_min ); \
      kval = astFree( kval ); \
      wtprod = astFree( wtprod ); \
      wtptr = astFree( wtptr ); \
      wtptr_last = astFree( wtptr_last ); \
   } \
\
/* If an error occurred in the kernel function, then report a \
   contextual error message. */ \
   if ( kerror ) { \
      astError( astStatus, "astResample"#X"(%s): Error signalled by " \
                "user-supplied 1-d interpolation kernel.", \
                astGetClass( unsimplified_mapping ) ); \
   } \
\
/* If an error has occurred, clear the returned result. */ \
   if ( !astOK ) result = 0; \
\
/* Return the result. */ \
   return result; \
}

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 1-dimensional
   case. */
#define ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the input grid, or is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If input bad pixels must be detected, then obtain the offset along \
   the input grid x dimension of the input pixel which contains the \
   current coordinate, and calculate this pixel's offset from the \
   start of the input array. */ \
      if ( Usebad ) { \
         pixel = (int) floor( x + 0.5 ) - lbnd_in[ 0 ]; \
\
/* Test if the pixel is bad. */ \
         bad = ( in[ pixel ] == badval ); \
      } \
\
/* If OK, calculate the lowest and highest indices (in the x \
   dimension) of the region of neighbouring pixels that will \
   contribute to the interpolated result. Constrain these values to \
   lie within the input grid. */ \
      if ( !bad ) { \
         ix = (int) floor( x ); \
         lo_x = MaxI( ix - neighb + 1, lbnd_in[ 0 ] ); \
         hi_x = MinI( ix + neighb,     ubnd_in[ 0 ] ); \
\
/* Initialise sums for forming the interpolated result. */ \
         sum = (Xfloattype) 0.0; \
         wtsum = (Xfloattype) 0.0; \
         if ( Usevar ) { \
            sum_var = (Xfloattype) 0.0; \
            bad_var = 0; \
         } \
\
/* Loop to inspect all the contributing pixels, calculating the offset \
   of each pixel from the start of the input array. */ \
         off_in = lo_x - lbnd_in[ 0 ]; \
         for ( ix = lo_x; ix <= hi_x; ix++, off_in++ ) { \
\
/* If necessary, test if the input pixel is bad. If not, calculate its \
   weight by evaluating the kernel function. */ \
            if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
               ( *kernel )( (double) ix - x, params, flags, &pixwt ); \
\
/* Check for errors arising in the kernel function. */ \
               if ( !astOK ) { \
                  kerror = 1; \
                  goto Kernel_Error_1d; \
               } \
\
/* Form the weighted sums required for finding the interpolated \
   value. */ \
               sum += ( (Xfloattype) pixwt ) * ( (Xfloattype) in[ off_in ] ); \
               wtsum += (Xfloattype) pixwt; \
\
/* If a variance estimate is required and it still seems possible to \
   obtain one, then obtain the variance value associated with the \
   current input pixel. */ \
               if ( Usevar ) { \
                  if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                     var = in_var[ off_in ]; \
\
/* If necessary, test if this value is bad (if the data type is \
   signed, also check that it is not negative). */ \
                     if ( Usebad ) bad_var = ( var == badval ); \
                     CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If any bad input variance value is obtained, we cannot generate a \
   valid output variance estimate. Otherwise, form the sum needed to \
   calculate this estimate. */ \
                     if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                        sum_var += ( (Xfloattype) ( pixwt * pixwt ) ) * \
                                   ( (Xfloattype) var ); \
                     } \
                  } \
               } \
            } \
         } \
      } \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 2-dimensional
   case. */
#define ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the input grid, or is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If not, then similarly obtain and test the y coordinate. */ \
      y = coords[ 1 ][ point ]; \
      bad = ( y < ymin ) || ( y >= ymax ) || ( y == AST__BAD ); \
      if ( !bad ) { \
\
/* If input bad pixels must be detected, then obtain the offsets along \
   each input grid dimension of the input pixel which contains the \
   current coordinates, and calculate this pixel's offset from the \
   start of the input array. */ \
         if ( Usebad ) { \
            ix = (int) floor( x + 0.5 ); \
            iy = (int) floor( y + 0.5 ); \
            pixel = ix - lbnd_in[ 0 ] + ystride * ( iy - lbnd_in[ 1 ] ); \
\
/* Test if the pixel is bad. */ \
            bad = ( in[ pixel ] == badval ); \
         } \
\
/* If OK, calculate the lowest and highest indices (in each dimension) \
   of the region of neighbouring pixels that will contribute to the \
   interpolated result. Constrain these values to lie within the input \
   grid. */ \
         if ( !bad ) { \
            ix = (int) floor( x ); \
            lo_x = MaxI( ix - neighb + 1, lbnd_in[ 0 ] ); \
            hi_x = MinI( ix + neighb,     ubnd_in[ 0 ] ); \
            iy = (int) floor( y ); \
            lo_y = MaxI( iy - neighb + 1, lbnd_in[ 1 ] ); \
            hi_y = MinI( iy + neighb,     ubnd_in[ 1 ] ); \
\
/* Loop to evaluate the kernel function along the x dimension, storing \
   the resulting values. The function's argument is the offset of the \
   contributing pixel (along this dimension) from the input \
   position. */ \
            for ( ix = lo_x; ix <= hi_x; ix++ ) { \
               ( *kernel )( (double) ix - x, params, flags, \
                            kval + ix - lo_x ); \
\
/* Check for errors arising in the kernel function. */ \
               if ( !astOK ) { \
                  kerror = 1; \
                  goto Kernel_Error_2d; \
               } \
            } \
\
/* Initialise sums for forming the interpolated result. */ \
            sum = (Xfloattype) 0.0; \
            wtsum = (Xfloattype) 0.0; \
            if ( Usevar ) { \
               sum_var = (Xfloattype) 0.0; \
               bad_var = 0; \
            } \
\
/* Loop over the y index to inspect all the contributing pixels, while \
   keeping track of their offset within the input array. Evaluate the \
   kernel function for each y index value. */ \
            off1 = lo_x - lbnd_in[ 0 ] + ystride * ( lo_y - lbnd_in[ 1 ] ); \
            for ( iy = lo_y; iy <= hi_y; iy++, off1 += ystride ) { \
               ( *kernel )( (double) iy - y, params, flags, &wt_y ); \
\
/* Check for errors arising in the kernel function. */ \
               if ( !astOK ) { \
                  kerror = 1; \
                  goto Kernel_Error_2d; \
               } \
\
/* Loop over the x index, calculating the pixel offset in the input \
   array. */ \
               off_in = off1; \
               for ( ix = lo_x; ix <= hi_x; ix++, off_in++ ) { \
\
/* If necessary, test if the input pixel is bad. If not, calculate its \
   weight as the product of the kernel function's value for the x and \
   y dimensions. */ \
                  if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
                     pixwt = kval[ ix - lo_x ] * wt_y; \
\
/* Form the weighted sums required for finding the interpolated \
   value. */ \
                     sum += ( (Xfloattype) pixwt ) * \
                            ( (Xfloattype) in[ off_in ] ); \
                     wtsum += (Xfloattype) pixwt; \
\
/* If a variance estimate is required and it still seems possible to \
   obtain one, then obtain the variance value associated with the \
   current input pixel. */ \
                     if ( Usevar ) { \
                        if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                           var = in_var[ off_in ]; \
\
/* If necessary, test if this value is bad (if the data type is \
   signed, also check that it is not negative). */ \
                           if ( Usebad ) bad_var = ( var == badval ); \
                           CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If any bad input variance value is obtained, we cannot generate a \
   valid output variance estimate. Otherwise, form the sum needed to \
   calculate this estimate. */ \
                           if ( !( ( Xsigned ) || ( Usebad ) ) || \
                                !bad_var ) { \
                              sum_var += ( (Xfloattype) ( pixwt * pixwt ) ) * \
                                         ( (Xfloattype) var ); \
                           } \
                        } \
                     } \
                  } \
               } \
            } \
         } \
      } \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the n-dimensional
   case. */
#define ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Initialise offsets into the input array. Then loop to obtain each \
   coordinate associated with the current output point. */ \
   pixel = 0; \
   off_in = 0; \
   for ( idim = 0; idim < ndim_in; idim++ ) { \
      xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate lies outside the input grid, or is bad. If \
   either is true, the corresponding output pixel value will be bad, \
   so give up on this point. */ \
      bad = ( xn < xn_min[ idim ] ) || ( xn >= xn_max[ idim ] ) || \
            ( xn == AST__BAD ); \
      if ( bad ) break; \
\
/* If input bad pixels must be detected, then obtain the index along \
   the current input grid dimension of the pixel which contains this \
   coordinate and accumulate the pixel's offset from the start of the \
   input array. */ \
      if ( Usebad ) { \
         pixel += stride[ idim ] * \
                  ( (int) floor( xn + 0.5 ) - lbnd_in[ idim ] ); \
      } \
\
/* Calculate the lowest and highest indices (in the current dimension) \
   of the region of neighbouring pixels that will contribute to the \
   interpolated result. Constrain these values to lie within the input \
   grid. */ \
      ixn = (int) floor( xn ); \
      lo[ idim ] = MaxI( ixn - neighb + 1, lbnd_in[ idim ] ); \
      hi[ idim ] = MinI( ixn + neighb,     ubnd_in[ idim ] ); \
\
/* Accumulate the offset (from the start of the input array) of the \
   contributing pixel which has the lowest index in each dimension. */ \
      off_in += stride[ idim ] * ( lo[ idim ] - lbnd_in[ idim ] ); \
   } \
\
/* Once the input pixel which contains the required coordinates has \
   been identified, test if it is bad, if necessary. */ \
   if ( Usebad ) bad = bad || ( in[ pixel ] == badval ); \
\
/* If OK, loop to evaluate the kernel function which will be used to \
   weight the contributions from surrounding pixels. */ \
   if ( !bad ) { \
      for ( idim = 0; idim < ndim_in; idim++ ) { \
\
/* Set up an array of pointers to locate kernel values (stored in the \
   "kval" array) for each dimension. Initially, each of these pointers \
   locates the first weight value which applies to the contributing \
   pixel with the lowest index in each dimension. */ \
         wtptr[ idim ] = kval + ( 2 * neighb * idim ); \
\
/* Also set up pointers to the last weight value in each dimension. */ \
         wtptr_last[ idim ] = wtptr[ idim ] + ( hi[ idim ] - lo[ idim ] ); \
\
/* Loop to evaluate the kernel function along each dimension, storing \
   the resulting values. The function's argument is the offset of the \
   contributing pixel (along the relevant dimension) from the input \
   point. */ \
         xn = coords[ idim ][ point ]; \
         for ( ixn = lo[ idim ]; ixn <= hi[ idim ]; ixn++ ) { \
            ( *kernel )( (double) ixn - xn, params, flags, \
                         wtptr[ idim ] + ixn - lo[ idim ] ); \
\
/* Check for errors arising in the kernel function. */ \
            if ( !astOK ) { \
               kerror = 1; \
               goto Kernel_Error_Nd; \
            } \
         } \
      } \
\
/* Initialise, and loop over the neighbouring input pixels to obtain \
   an interpolated value. */ \
      sum = (Xfloattype) 0.0; \
      wtsum = (Xfloattype) 0.0; \
      if ( Usevar ) { \
         sum_var = (Xfloattype) 0.0; \
         bad_var = 0; \
      } \
      idim = ndim_in - 1; \
      wtprod[ idim ] = 1.0; \
      done = 0; \
      do { \
\
/* Each contributing pixel is weighted by the product of the kernel \
   weight factors evaluated along each input dimension. However, since \
   we typically only change the index of one dimension at a time, we \
   can avoid forming this product repeatedly by retaining an array of \
   accumulated products for all higher dimensions. We need then only \
   update the lower elements in this array, corresponding to those \
   dimensions whose index has changed. We do this here, "idim" being \
   the index of the most significant dimension to have changed. Note \
   that on the first pass, all dimensions are considered changed, \
   causing this array to be initialised. */ \
         for ( ii = idim; ii >= 1; ii-- ) { \
            wtprod[ ii - 1 ] = wtprod[ ii ] * *( wtptr[ ii ] ); \
         } \
\
/* If necessary, test each pixel which may contribute to the result to \
   see if it is bad. If not, obtain its weight from the accumulated \
   product of weights. Also multiply by the weight for dimension zero, \
   which is not included in the "wtprod" array). */ \
         if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
            pixwt = wtprod[ 0 ] * *( wtptr[ 0 ] ); \
\
/* Form the weighted sums required for finding the interpolated \
   value. */ \
            sum += ( (Xfloattype) pixwt ) * ( (Xfloattype) in[ off_in ] ); \
            wtsum += (Xfloattype) pixwt; \
\
/* If a variance estimate is required and it still seems possible to \
   obtain one, then obtain the variance value associated with the \
   current input pixel. */ \
            if ( Usevar ) { \
               if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                  var = in_var[ off_in ]; \
\
/* If necessary, test if this value is bad (if the data type is \
   signed, also check that it is not negative). */ \
                  if ( Usebad ) bad_var = ( var == badval ); \
                  CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If any bad input variance value is obtained, we cannot generate a \
   valid output variance estimate. Otherwise, form the sum needed to \
   calculate this estimate. */ \
                  if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                     sum_var += ( (Xfloattype) ( pixwt * pixwt ) ) * \
                                ( (Xfloattype) var ); \
                  } \
               } \
            } \
         } \
\
/* Now update the weight value pointers and pixel offset to refer to \
   the next input pixel to be considered. */ \
         idim = 0; \
         do { \
\
/* The first input dimension whose weight value pointer has not yet \
   reached its final value has this pointer incremented, and the pixel \
   offset into the input array is updated accordingly. */ \
            if ( wtptr[ idim ] != wtptr_last[ idim ] ) { \
               wtptr[ idim ]++; \
               off_in += stride[ idim ]; \
               break; \
\
/* Any earlier dimensions (which have reached the final pointer value) \
   have this pointer returned to its lowest value. Again, the pixel \
   offset into the input image is updated accordingly. */ \
            } else { \
               wtptr[ idim ] -= ( hi[ idim ] - lo[ idim ] ); \
               off_in -= stride[ idim ] * \
                               ( hi[ idim ] - lo[ idim ] ); \
               done = ( ++idim == ndim_in ); \
            } \
         } while ( !done ); \
      } while ( !done ); \
   }

/* This subsidiary macro calculates the interpolated output value (and
   variance) from the sums over contributing pixels, checks the
   results for validity, and assigns them to the output array(s). */
#define CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Usebad,Usevar) \
\
/* If the output data value has not yet been flagged as bad, then \
   check that an interpolated value can actually be produced.  First \
   check that the sum of weights is not zero. */ \
   if ( !bad ) { \
      bad = ( wtsum == (Xfloattype) 0.0 ); \
\
/* If OK, calculate the interpolated value. Then, if the output data \
   type is not floating point, check that this value will not overflow \
   the available output range. */ \
      if ( !bad ) { \
         val = sum / wtsum; \
         if ( !( Xfloating ) ) { \
            bad = ( val <= lo_lim ) || ( val >= hi_lim ); \
         } \
      } \
\
/* If no interpolated data value can be produced, then no associated \
   variance will be required either. */ \
      if ( ( Usevar ) && bad ) bad_var = 1; \
   } \
\
/* Now perform similar checks on the output variance value (if \
   required). This time we check that the square of the sum of \
   weights is not zero (since this might underflow before the sum of \
   weights). Again we also check to prevent the result overflowing the \
   output data type. */ \
   if ( ( Usevar ) && !bad_var ) { \
      wtsum_sq = wtsum * wtsum; \
      bad_var = ( wtsum_sq == (Xfloattype) 0.0 ); \
      if ( !bad_var ) { \
         val_var = sum_var / wtsum_sq; \
         if ( !( Xfloating ) ) { \
            bad_var = ( val_var <= lo_lim ) || ( val_var >= hi_lim ); \
         } \
      } \
   } \
\
/* Obtain the pixel offset into the output array. */ \
   off_out = offset[ point ]; \
\
/* Assign a bad output value (and variance) if required and count it. */ \
   if ( bad ) { \
      out[ off_out ] = badval; \
      if ( Usevar ) out_var[ off_out ] = badval; \
      result++; \
\
/* Otherwise, assign the interpolated value. If the output data type \
   is floating point, the result can be stored directly, otherwise we \
   must round to the nearest integer. */ \
   } else { \
      if ( Xfloating ) { \
         out[ off_out ] = (Xtype) val; \
      } else { \
         out[ off_out ] = (Xtype) ( val + ( ( val >= (Xfloattype) 0.0 ) ? \
                                            ( (Xfloattype) 0.5 ) : \
                                            ( (Xfloattype) -0.5 ) ) ); \
      } \
\
/* If a variance estimate is required but none can be obtained, then \
   store a bad output variance value and count it. */ \
      if ( Usevar ) { \
         if ( bad_var ) { \
            out_var[ off_out ] = badval; \
            result++; \
\
/* Otherwise, store the variance estimate, rounding to the nearest \
   integer if necessary. */ \
         } else { \
            if ( Xfloating ) { \
               out_var[ off_out ] = (Xtype) val_var; \
            } else { \
               out_var[ off_out ] = (Xtype) ( val_var + \
                                          ( ( val_var >= (Xfloattype) 0.0 ) ? \
                                            ( (Xfloattype) 0.5 ) : \
                                            ( (Xfloattype) -0.5 ) ) ); \
            } \
         } \
      } \
   }

/* These subsidiary macros define limits for range checking of results
   before conversion to the final data type. For each data type code
   <X>, HI_<X> gives the least positive floating point value which
   just overflows that data type towards plus infinity, while LO_<X>
   gives the least negative floating point value which just overflows
   that data type towards minus infinity. Thus, a floating point value
   must satisfy LO<flt_value<HI if overflow is not to occur when it is
   converted to that data type.

   The data type of each limit should be that of the smallest
   precision floating point type which will accommodate the full range
   of values that the target type may take. */
   
/* If <X> is a floating point type, the limits are not actually used,
   but must be present to permit error-free compilation. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
#define HI_LD ( 0.0L )
#define LO_LD ( 0.0L )
#endif
#define HI_D ( 0.0 )
#define LO_D ( 0.0 )
#define HI_F ( 0.0f )
#define LO_F ( 0.0f )

#if HAVE_LONG_DOUBLE     /* Not normally implemented */
#define HI_L   ( 0.5L + (long double) LONG_MAX )
#define LO_L  ( -0.5L + (long double) LONG_MIN )
#define HI_UL  ( 0.5L + (long double) ULONG_MAX )
#define LO_UL ( -0.5L )
#else
#define HI_L   ( 0.5 + (double) LONG_MAX )
#define LO_L  ( -0.5 + (double) LONG_MIN )
#define HI_UL  ( 0.5 + (double) ULONG_MAX )
#define LO_UL ( -0.5 )
#endif
#define HI_I   ( 0.5 + (double) INT_MAX )
#define LO_I  ( -0.5 + (double) INT_MIN )
#define HI_UI  ( 0.5 + (double) UINT_MAX )
#define LO_UI ( -0.5 )
#define HI_S   ( 0.5f + (float) SHRT_MAX )
#define LO_S  ( -0.5f + (float) SHRT_MIN )
#define HI_US  ( 0.5f + (float) USHRT_MAX )
#define LO_US ( -0.5f )
#define HI_B   ( 0.5f + (float) SCHAR_MAX )
#define LO_B  ( -0.5f + (float) SCHAR_MIN )
#define HI_UB  ( 0.5f + (float) UCHAR_MAX )
#define LO_UB ( -0.5f )

/* This subsidiary macro tests for negative variance values. This
   check is required only for signed data types. */
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
   bad_var = bad_var || ( var < ( (Xtype) 0 ) );

/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_KERNEL1(LD,long double,1,long double,1)
MAKE_INTERPOLATE_KERNEL1(L,long int,0,long double,1)
#else
MAKE_INTERPOLATE_KERNEL1(L,long int,0,double,1)
#endif     
MAKE_INTERPOLATE_KERNEL1(D,double,1,double,1)
MAKE_INTERPOLATE_KERNEL1(F,float,1,float,1)
MAKE_INTERPOLATE_KERNEL1(I,int,0,double,1)
MAKE_INTERPOLATE_KERNEL1(S,short int,0,float,1)
MAKE_INTERPOLATE_KERNEL1(B,signed char,0,float,1)

/* Re-define the macro for testing for negative variances to do
   nothing. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype)

/* Expand the main macro above to generate a function for each
   required unsigned data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_KERNEL1(UL,unsigned long int,0,long double,0)
#else
MAKE_INTERPOLATE_KERNEL1(UL,unsigned long int,0,double,0)
#endif     
MAKE_INTERPOLATE_KERNEL1(UI,unsigned int,0,double,0)
MAKE_INTERPOLATE_KERNEL1(US,unsigned short int,0,float,0)
MAKE_INTERPOLATE_KERNEL1(UB,unsigned char,0,float,0)

/* Undefine the macros used above. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
#undef HI_LD
#undef LO_LD
#endif
#undef HI_D
#undef LO_D
#undef HI_F
#undef LO_F
#undef HI_L
#undef LO_L
#undef HI_UL
#undef LO_UL
#undef HI_I
#undef LO_I
#undef HI_UI
#undef LO_UI
#undef HI_S
#undef LO_S
#undef HI_US
#undef LO_US
#undef HI_B
#undef LO_B
#undef HI_UB
#undef LO_UB
#undef CALC_AND_ASSIGN_OUTPUT
#undef ASSEMBLE_INPUT_ND
#undef ASSEMBLE_INPUT_2D
#undef ASSEMBLE_INPUT_1D
#undef MAKE_INTERPOLATE_KERNEL1

/*
*  Name:
*     InterpolateLinear<X>

*  Purpose:
*     Resample a data grid, using the linear interpolation scheme.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int InterpolateLinear<X>( int ndim_in,
*                               const int *lbnd_in, const int *ubnd_in,
*                               const <Xtype> *in, const <Xtype> *in_var,
*                               int npoint, const int *offset,
*                               const double *const *coords,
*                               int flags, <Xtype> badval,
*                               <Xtype> *out, <Xtype> *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which resample a rectangular input
*     grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. Each output
*     grid point may be mapped on to a position in the input grid in
*     an arbitrary way. Where the positions given do not correspond
*     with a pixel centre in the input grid, the interpolation scheme
*     used is linear interpolation between the centres of the nearest
*     neighbouring pixels in each dimension (there are 2 nearest
*     neighbours in 1 dimension, 4 in 2 dimensions, 8 in 3 dimensions,
*     etc.).

*  Parameters:
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input grid, its extent along a particular
*        (i'th) dimension being ubnd_in[i]-lbnd_in[i]+1 (assuming "i"
*        is zero-based). They also define the input grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be resampled (with an element
*        for each pixel in the input grid). The numerical type of
*        these data should match the function used, as given by the
*        suffix on the function name. The storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
*        (i.e. Fortran array storage order).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of points at which the input grid is to be
*        resampled.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each output point, this array should contain the zero-based
*        offset in the output array(s) (i.e. the "out" and,
*        optionally, the "out_var" arrays) at which the resampled
*        output value(s) should be stored.
*     coords
*        An array of pointers to double, with "ndim_in"
*        elements. Element "coords[coord]" should point at the first
*        element of an array of double (with "npoint" elements) which
*        contains the values of coordinate number "coord" for each
*        interpolation point. The value of coordinate number "coord"
*        for interpolation point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices are
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding output data (and
*        variance) will be set to the value given by "badval".
*     flags
*        The bitwise OR of a set of flag values which control the
*        operation of the function. Currently, only the flag
*        AST__USEBAD is significant and indicates whether there are
*        "bad" (i.e. missing) data in the input array(s) which must be
*        recognised and propagated to the output array(s).  If this
*        flag is not set, all input values are treated literally.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned. Note
*        that details of how the output grid maps on to this array
*        (e.g. the storage order, number of dimensions, etc.) is
*        arbitrary and is specified entirely by means of the "offset"
*        array. The "out" array should therefore contain sufficient
*        elements to accommodate the "offset" values supplied.  There
*        is no requirement that all elements of the "out" array should
*        be assigned values, and any which are not addressed by the
*        contents of the "offset" array will be left unchanged.
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given. It is addressed in
*        exactly the same way (via the "offset" array) as the "out"
*        array. The values returned are estimates of the statistical
*        variance of the corresponding values in the "out" array, on
*        the assumption that all errors in input grid values (in the
*        "in" array) are statistically independent and that their
*        variance estimates (in the "in_var" array) may simply be
*        summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.

*  Returned Value:
*     The number of output grid points to which a data value (or a
*     variance value if relevant) equal to "badval" has been assigned
*     because no valid output value could be obtained.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*     - A value of zero will be returned if any of these functions is
*     invoked with the global error status set, or if it should fail
*     for any reason.
*/
/* Define macros to implement the function for a specific data
   type. */
#define MAKE_INTERPOLATE_LINEAR(X,Xtype,Xfloating,Xfloattype,Xsigned) \
static int InterpolateLinear##X( int ndim_in, \
                                 const int *lbnd_in, const int *ubnd_in, \
                                 const Xtype *in, const Xtype *in_var, \
                                 int npoint, const int *offset, \
                                 const double *const *coords, \
                                 int flags, Xtype badval, \
                                 Xtype *out, Xtype *out_var ) { \
\
/* Local Variables: */ \
   Xfloattype sum;               /* Weighted sum of pixel data values */ \
   Xfloattype sum_var;           /* Weighted sum of pixel variance values */ \
   Xfloattype val;               /* Value to be asigned to output pixel */ \
   Xfloattype wtsum;             /* Sum of weight values */ \
   Xtype var;                    /* Variance value */ \
   double *frac_hi;              /* Pointer to array of weights */ \
   double *frac_lo;              /* Pointer to array of weights */ \
   double *wt;                   /* Pointer to array of weights */ \
   double *wtprod;               /* Array of accumulated weights pointer */ \
   double *xn_max;               /* Pointer to upper limits array (n-d) */ \
   double *xn_min;               /* Pointer to lower limits array (n-d) */ \
   double frac_hi_x;             /* Pixel weight (x dimension) */ \
   double frac_hi_y;             /* Pixel weight (y dimension) */ \
   double frac_lo_x;             /* Pixel weight (x dimension) */ \
   double frac_lo_y;             /* Pixel weight (y dimension) */ \
   double pixwt;                 /* Weight to apply to individual pixel */ \
   double x;                     /* x coordinate value */ \
   double xmax;                  /* x upper limit */ \
   double xmin;                  /* x lower limit */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   double ymax;                  /* y upper limit */ \
   double ymin;                  /* y lower limit */ \
   int *dim;                     /* Pointer to array of pixel indices */ \
   int *hi;                      /* Pointer to array of upper indices */ \
   int *lo;                      /* Pointer to array of lower indices */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int bad_var;                  /* Output variance bad? */ \
   int done;                     /* All pixel indices done? */ \
   int hi_x;                     /* Upper pixel index (x dimension) */ \
   int hi_y;                     /* Upper pixel index (y dimension) */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ii;                       /* Loop counter for weights */ \
   int ix;                       /* Pixel index in input grid x dimension */ \
   int ixn;                      /* Pixel index (n-d) */ \
   int iy;                       /* Pixel index in input grid y dimension */ \
   int lo_x;                     /* Lower pixel index (x dimension) */ \
   int lo_y;                     /* Lower pixel index (y dimension) */ \
   int off_in;                   /* Offset to input pixel */ \
   int off_lo;                   /* Offset to "first" input pixel */ \
   int off_out;                  /* Offset to output pixel */ \
   int pixel;                    /* Offset to input pixel containing point */ \
   int point;                    /* Loop counter for output points */ \
   int result;                   /* Result value to return */ \
   int s;                        /* Temporary variable for strides */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int ystride;                  /* Stride along input grid y dimension */ \
\
/* Initialise. */ \
   result = 0; \
\
/* Check the global error status. */ \
   if ( !astOK ) return result; \
\
/* Initialise variables to avoid "used of uninitialised variable" \
   messages from dumb compilers. */ \
   sum = 0; \
   sum_var = 0; \
   wtsum = 0; \
   bad = 0; \
   bad_var = 0; \
\
/* Determine if we are processing bad pixels or variances. */ \
   usebad = flags & AST__USEBAD; \
   usevar = in_var && out_var; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   if ( ndim_in == 1 ) { \
\
/* Calculate the coordinate limits of the input grid. */ \
      xmin = (double) lbnd_in[ 0 ] - 0.5; \
      xmax = (double) ubnd_in[ 0 ] + 0.5; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      0,0) \
            } \
         } \
      } \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   } else if ( ndim_in == 2 ) { \
\
/* Calculate the stride along the y dimension of the input grid. */ \
      ystride = ubnd_in[ 0 ] - lbnd_in[ 0 ] + 1; \
\
/* Calculate the coordinate limits of the input grid in each \
   dimension. */ \
      xmin = (double) lbnd_in[ 0 ] - 0.5; \
      xmax = (double) ubnd_in[ 0 ] + 0.5; \
      ymin = (double) lbnd_in[ 1 ] - 0.5; \
      ymax = (double) ubnd_in[ 1 ] + 0.5; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                                      0,0) \
            } \
         } \
      } \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
   } else { \
\
/* Allocate workspace. */ \
      dim = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      frac_hi = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      frac_lo = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      hi = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      lo = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      stride = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      wt = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      wtprod = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      xn_max = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      xn_min = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      if ( astOK ) { \
\
/* Calculate the stride along each dimension of the input grid. */ \
         for ( s = 1, idim = 0; idim < ndim_in; idim++ ) { \
            stride[ idim ] = s; \
            s *= ubnd_in[ idim ] - lbnd_in[ idim ] + 1; \
\
/* Calculate the coordinate limits of the input grid in each \
   dimension. */ \
            xn_min[ idim ] = (double) lbnd_in[ idim ] - 0.5; \
            xn_max[ idim ] = (double) ubnd_in[ idim ] + 0.5; \
         } \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
         if ( usebad ) { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype, \
                                         Xsigned,1,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype, \
                                         Xsigned,1,0) \
               } \
            } \
         } else { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype, \
                                         Xsigned,0,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype, \
                                         Xsigned,0,0) \
               } \
            } \
         } \
      } \
\
/* Free the workspace. */ \
      dim = astFree( dim ); \
      frac_hi = astFree( frac_hi ); \
      frac_lo = astFree( frac_lo ); \
      hi = astFree( hi ); \
      lo = astFree( lo ); \
      stride = astFree( stride ); \
      wt = astFree( wt ); \
      wtprod = astFree( wtprod ); \
      xn_max = astFree( xn_max ); \
      xn_min = astFree( xn_min ); \
   } \
\
/* If an error has occurred, clear the returned result. */ \
   if ( !astOK ) result = 0; \
\
/* Return the result. */ \
   return result; \
}

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 1-dimensional
   case. */
#define ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the input grid. Also test if it is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If input bad pixels must be detected, then obtain the offset along \
   the input grid x dimension of the input pixel which contains the \
   current coordinate and calculate this pixel's offset from the start \
   of the input array. */ \
      if ( Usebad ) { \
         pixel = (int) floor( x + 0.5 ) - lbnd_in[ 0 ]; \
\
/* Test if the pixel is bad. */ \
         bad = ( in[ pixel ] == badval ); \
      } \
\
/* If OK, obtain the indices along the input grid x dimension of the \
   two adjacent pixels which will contribute to the interpolated \
   result. Also obtain the fractional weight to be applied to each of \
   these pixels. */ \
      if ( !bad ) { \
         lo_x = (int) floor( x ); \
         hi_x = lo_x + 1; \
         frac_lo_x = (double) hi_x - x; \
         frac_hi_x = 1.0 - frac_lo_x; \
\
/* Obtain the offset within the input array of the first pixel to be \
   used for interpolation (the one with the smaller index). */ \
         off_lo = lo_x - lbnd_in[ 0 ]; \
\
/* Initialise sums for forming the interpolated result. */ \
         sum = (Xfloattype) 0.0; \
         wtsum = (Xfloattype) 0.0; \
         if ( Usevar ) { \
            sum_var = (Xfloattype) 0.0; \
            if ( ( Xsigned ) || ( Usebad ) ) bad_var = 0; \
         } \
\
/* For each of the two pixels which may contribute to the result, \
   test if the pixel index lies within the input grid. Where it does, \
   accumulate the sums required for forming the interpolated \
   result. In each case, we supply the pixel's offset within the input \
   array and the weight to be applied to it. */ \
         if ( lo_x >= lbnd_in[ 0 ] ) { \
            FORM_LINEAR_INTERPOLATION_SUM(off_lo,frac_lo_x,Xtype, \
                                          Xfloattype,Xsigned,Usebad,Usevar) \
         } \
         if ( hi_x <= ubnd_in[ 0 ] ) { \
            FORM_LINEAR_INTERPOLATION_SUM(off_lo + 1,frac_hi_x,Xtype, \
                                          Xfloattype,Xsigned,Usebad,Usevar) \
         } \
      } \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 2-dimensional
   case. */
#define ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the input grid. Also test if it is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If OK, then similarly obtain and test the y coordinate. */ \
      y = coords[ 1 ][ point ]; \
      bad = ( y < ymin ) || ( y >= ymax ) || ( y == AST__BAD ); \
      if ( !bad ) { \
\
/* If input bad pixels must be detected, then obtain the offsets along \
   each input grid dimension of the input pixel which contains the \
   current coordinates. */ \
         if ( Usebad ) { \
            ix = (int) floor( x + 0.5 ); \
            iy = (int) floor( y + 0.5 ); \
\
/* Calculate this pixel's offset from the start of the input array. */ \
            pixel = ix - lbnd_in[ 0 ] + ystride * ( iy - lbnd_in[ 1 ] ); \
\
/* Test if the pixel is bad. */ \
            bad = ( in[ pixel ] == badval ); \
         } \
\
/* If OK, obtain the indices along the input grid x dimension of the \
   two adjacent pixels which will contribute to the interpolated \
   result. Also obtain the fractional weight to be applied to each of \
   these pixels. */ \
         if ( !bad ) { \
            lo_x = (int) floor( x ); \
            hi_x = lo_x + 1; \
            frac_lo_x = (double) hi_x - x; \
            frac_hi_x = 1.0 - frac_lo_x; \
\
/* Repeat this process for the y dimension. */ \
            lo_y = (int) floor( y ); \
            hi_y = lo_y + 1; \
            frac_lo_y = (double) hi_y - y; \
            frac_hi_y = 1.0 - frac_lo_y; \
\
/* Obtain the offset within the input array of the first pixel to be \
   used for interpolation (the one with the smaller index along both \
   dimensions). */ \
            off_lo = lo_x - lbnd_in[ 0 ] + ystride * ( lo_y - lbnd_in[ 1 ] ); \
\
/* Initialise sums for forming the interpolated result. */ \
            sum = (Xfloattype) 0.0; \
            wtsum = (Xfloattype) 0.0; \
            if ( Usevar ) { \
               sum_var = (Xfloattype) 0.0; \
               if ( ( Xsigned ) || ( Usebad ) ) bad_var = 0; \
            } \
\
/* For each of the four pixels which may contribute to the result, \
   test if the pixel indices lie within the input grid. Where they do, \
   accumulate the sums required for forming the interpolated \
   result. In each case, we supply the pixel's offset within the input \
   array and the weight to be applied to it. */ \
            if ( lo_y >= lbnd_in[ 1 ] ) { \
               if ( lo_x >= lbnd_in[ 0 ] ) { \
                  FORM_LINEAR_INTERPOLATION_SUM(off_lo, \
                                                frac_lo_x * frac_lo_y,Xtype, \
                                                Xfloattype, Xsigned, \
                                                Usebad,Usevar) \
               } \
               if ( hi_x <= ubnd_in[ 0 ] ) { \
                  FORM_LINEAR_INTERPOLATION_SUM(off_lo + 1, \
                                                frac_hi_x * frac_lo_y,Xtype, \
                                                Xfloattype,Xsigned, \
                                                Usebad,Usevar) \
               } \
            } \
            if ( hi_y <= ubnd_in[ 1 ] ) { \
               if ( lo_x >= lbnd_in[ 0 ] ) { \
                  FORM_LINEAR_INTERPOLATION_SUM(off_lo + ystride, \
                                                frac_lo_x * frac_hi_y,Xtype, \
                                                Xfloattype,Xsigned, \
                                                Usebad,Usevar) \
               } \
               if ( hi_x <= ubnd_in[ 0 ] ) { \
                  FORM_LINEAR_INTERPOLATION_SUM(off_lo + ystride + 1, \
                                                frac_hi_x * frac_hi_y,Xtype, \
                                                Xfloattype,Xsigned, \
                                                Usebad,Usevar) \
               } \
            } \
         } \
      } \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the n-dimensional
   case. */
#define ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Initialise offsets into the input array. Then loop to obtain each
   coordinate associated with the current output point. */ \
   off_in = 0; \
   if ( Usebad ) pixel = 0; \
   for ( idim = 0; idim < ndim_in; idim++ ) { \
      xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate lies outside the input grid.  Also test if \
   it is bad. If either is true, the corresponding output pixel value \
   will be bad, so give up on this point. */ \
      bad = ( xn < xn_min[ idim ] ) || ( xn >= xn_max[ idim ] ) || \
            ( xn == AST__BAD ); \
      if ( bad ) break; \
\
/* If input bad pixels must be detected, obtain the index along the \
   current input grid dimension of the pixel which contains this \
   coordinate and accumulate the pixel's offset from the start of the \
   input array. */ \
      if ( Usebad ) { \
         pixel += stride[ idim ] * \
                  ( (int) floor( xn + 0.5 ) - lbnd_in[ idim ] ); \
      } \
\
/* Obtain the indices along the current dimension of the input grid of \
   the two (usually adjacent) pixels which will contribute to the \
   output value. If necessary, however, restrict each index to ensure \
   it does not lie outside the input grid. Also calculate the \
   fractional weight to be given to each pixel in order to interpolate \
   linearly between them. */ \
      ixn = (int) floor( xn ); \
      lo[ idim ] = MaxI( ixn, lbnd_in[ idim ] ); \
      hi[ idim ] = MinI( ixn + 1, ubnd_in[ idim ] ); \
      frac_lo[ idim ] = 1.0 - fabs( xn - (double) lo[ idim ] ); \
      frac_hi[ idim ] = 1.0 - fabs( xn - (double) hi[ idim ] ); \
\
/* Store the lower index involved in interpolation along each \
   dimension and accumulate the offset from the start of the input \
   array of the pixel which has these indices. */ \
      dim[ idim ] = lo[ idim ]; \
      off_in += stride[ idim ] * ( lo[ idim ] - lbnd_in[ idim ] ); \
\
/* Also store the fractional weight associated with the lower pixel \
   along each dimension. */ \
      wt[ idim ] = frac_lo[ idim ]; \
   } \
\
/* If the input pixel which contains the required coordinates has \
   been identified, test if it is bad. */ \
   if ( Usebad ) bad = bad || ( in[ pixel ] == badval ); \
\
/* If OK, initialise and loop over adjacent input pixels to obtain an \
   interpolated value. */ \
   if ( !bad ) { \
      sum = (Xfloattype) 0.0; \
      wtsum = (Xfloattype) 0.0; \
      if ( Usevar ) { \
         sum_var = (Xfloattype) 0.0; \
         if ( ( Xsigned ) || ( Usebad ) ) bad_var = 0; \
      } \
      idim = ndim_in - 1; \
      wtprod[ idim ] = 1.0; \
      done = 0; \
      do { \
\
/* Each contributing pixel is weighted by the product of the weights \
   which account for the displacement of its centre from the required \
   position along each dimension. However, since we typically only \
   change the index of one dimension at a time, we can avoid forming \
   this product repeatedly by retaining an array of accumulated weight \
   products for all higher dimensions. We need then only update the \
   lower elements in this array, corresponding to those dimensions \
   whose index has changed. We do this here, "idim" being the index of \
   the most significant dimension to have changed. Note that on the \
   first pass, all dimensions are considered changed, causing this \
   array to be initialised. */ \
         for ( ii = idim; ii >= 1; ii-- ) { \
            wtprod[ ii - 1 ] = wtprod[ ii ] * wt[ ii ]; \
         } \
\
/* Accumulate the sums required for forming the interpolated \
   result. We supply the pixel's offset within the input array and the \
   weight to be applied to it. The pixel weight is formed by including \
   the weight factor for dimension zero, since this is not included in \
   the "wtprod" array. */ \
         FORM_LINEAR_INTERPOLATION_SUM(off_in,wtprod[ 0 ] * wt[ 0 ], \
                                       Xtype,Xfloattype,Xsigned, \
                                       Usebad,Usevar) \
\
/* Now update the indices, offset and weight factors to refer to the \
   next input pixel to be considered. */ \
         idim = 0; \
         do { \
\
/* The first input dimension which still refers to the pixel with the \
   lower of the two possible indices is switched to refer to the other \
   pixel (with the higher index). The offset into the input array and \
   the fractional weight factor for this dimension are also updated \
   accordingly. */ \
            if ( dim[ idim ] != hi[ idim ] ) { \
               dim[ idim ] = hi[ idim ]; \
               off_in += stride[ idim ]; \
               wt[ idim ] = frac_hi[ idim ]; \
               break; \
\
/* Any earlier dimensions (referring to the higher index) are switched \
   back to the lower index, if not already there, before going on to \
   consider the next dimension. (This process is the same as \
   incrementing a binary number and propagating overflows up through \
   successive digits, except that dimensions where the "lo" and "hi" \
   values are the same can only take one value.) The process stops at \
   the first attempt to return the final dimension to the lower \
   index. */ \
            } else { \
               if ( dim[ idim ] != lo[ idim ] ) { \
                  dim[ idim ] = lo[ idim ]; \
                  off_in -= stride[ idim ]; \
                  wt[ idim ] = frac_lo[ idim ]; \
               } \
               done = ( ++idim == ndim_in ); \
            } \
         } while ( !done ); \
      } while ( !done ); \
   }

/* This subsidiary macro adds the contribution from a specified input
   pixel to the accumulated sums for forming the linearly interpolated
   value. */
#define FORM_LINEAR_INTERPOLATION_SUM(off,wt,Xtype,Xfloattype,Xsigned, \
                                      Usebad,Usevar) \
\
/* Obtain the offset of the input pixel to use. */ \
   off_in = ( off ); \
\
/* If necessary, test if this pixel is bad. If not, then obtain the \
   weight to apply to it. */ \
   if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
      pixwt = ( wt ); \
\
/* Increment the weighted sum of pixel values and the sum of weights. */ \
      sum += ( (Xfloattype) in[ off_in ] ) * ( (Xfloattype) pixwt ); \
      wtsum += (Xfloattype) pixwt; \
\
/* If an output variance estimate is to be generated, and it still \
   seems possible to produce one, then obtain the input variance \
   value. */ \
      if ( Usevar ) { \
         if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
            var = in_var[ off_in ]; \
\
/* Test if the variance value is bad (if the data type is signed, also \
   check that it is not negative). */ \
            if ( Usebad ) bad_var = ( var == badval ); \
            CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If OK, increment the weighted sum of variance values. */ \
            if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
               sum_var += ( (Xfloattype) ( pixwt * pixwt ) ) * \
                          ( (Xfloattype) var ); \
            } \
         } \
      } \
   }

/* This subsidiary macro calculates the interpolated output value (and
   variance) from the sums over contributing pixels and assigns them
   to the output array(s). */
#define CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Xsigned, \
                               Usebad,Usevar) \
\
/* Obtain the pixel offset into the output array. */ \
   off_out = offset[ point ]; \
\
/* Assign a bad output value (and variance) if required and count it. */ \
   if ( bad ) { \
      out[ off_out ] = badval; \
      if ( Usevar ) out_var[ off_out ] = badval; \
      result++; \
\
/* Otherwise, calculate the interpolated value. If the output data \
   type is floating point, this result can be stored directly, \
   otherwise we must round to the nearest integer. */ \
   } else { \
      val = sum / wtsum; \
      if ( Xfloating ) { \
         out[ off_out ] = (Xtype) val; \
      } else { \
         out[ off_out ] = (Xtype) ( val + ( ( val >= (Xfloattype) 0.0 ) ? \
                                            ( (Xfloattype) 0.5 ) : \
                                            ( (Xfloattype) -0.5 ) ) ); \
      } \
\
/* If a variance estimate is required but none can be obtained, then \
   store a bad output variance value and count it. */ \
      if ( Usevar ) { \
         if ( ( ( Xsigned ) || ( Usebad ) ) && bad_var ) { \
            out_var[ off_out ] = badval; \
            result++; \
\
/* Otherwise, calculate the variance estimate and store it, rounding \
   to the nearest integer if necessary. */ \
         } else { \
            val = sum_var / ( wtsum * wtsum ); \
            if ( Xfloating ) { \
               out_var[ off_out ] = (Xtype) val; \
            } else { \
               out_var[ off_out ] = (Xtype) ( val + \
                                            ( ( val >= (Xfloattype) 0.0 ) ? \
                                              ( (Xfloattype) 0.5 ) : \
                                              ( (Xfloattype) -0.5 ) ) ); \
            } \
         } \
      } \
   }

/* This subsidiary macro tests for negative variance values in the
   macros above. This check is required only for signed data types. */
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
   bad_var = bad_var || ( var < ( (Xtype) 0 ) );

/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_LINEAR(LD,long double,1,long double,1)
MAKE_INTERPOLATE_LINEAR(L,long int,0,long double,1)
#else
MAKE_INTERPOLATE_LINEAR(L,long int,0,double,1)
#endif     
MAKE_INTERPOLATE_LINEAR(D,double,1,double,1)
MAKE_INTERPOLATE_LINEAR(F,float,1,float,1)
MAKE_INTERPOLATE_LINEAR(I,int,0,double,1)
MAKE_INTERPOLATE_LINEAR(S,short int,0,float,1)
MAKE_INTERPOLATE_LINEAR(B,signed char,0,float,1)

/* Re-define the macro for testing for negative variances to do
   nothing. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype)

/* Expand the main macro above to generate a function for each
   required unsigned data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_LINEAR(UL,unsigned long int,0,long double,0)
#else
MAKE_INTERPOLATE_LINEAR(UL,unsigned long int,0,double,0)
#endif     
MAKE_INTERPOLATE_LINEAR(UI,unsigned int,0,double,0)
MAKE_INTERPOLATE_LINEAR(US,unsigned short int,0,float,0)
MAKE_INTERPOLATE_LINEAR(UB,unsigned char,0,float,0)

/* Undefine the macros uxsed above. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#undef CALC_AND_ASSIGN_OUTPUT
#undef FORM_LINEAR_INTERPOLATION_SUM
#undef ASSEMBLE_INPUT_ND
#undef ASSEMBLE_INPUT_2D
#undef ASSEMBLE_INPUT_1D
#undef MAKE_INTERPOLATE_LINEAR

/*
*  Name:
*     InterpolateNearest<X>

*  Purpose:
*     Resample a data grid, using the nearest-pixel interpolation scheme.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int InterpolateNearest<X>( int ndim_in,
*                                const int *lbnd_in, const int *ubnd_in,
*                                const <Xtype> *in, const <Xtype> *in_var,
*                                int npoint, const int *offset,
*                                const double *const *coords,
*                                int flags, <Xtype> badval,
*                                <Xtype> *out, <Xtype> *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which resample a rectangular input
*     grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. Each output
*     grid point may be mapped on to a position in the input grid in
*     an arbitrary way. Where the positions given do not correspond
*     with a pixel centre in the input grid, the interpolation scheme
*     used is simply to select the nearest pixel (i.e. the one whose
*     bounds contain the supplied position).

*  Parameters:
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input grid, its extent along a particular
*        (i'th) dimension being ubnd_in[i]-lbnd_in[i]+1 (assuming "i"
*        is zero-based). They also define the input grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be resampled (with an element
*        for each pixel in the input grid). The numerical type of
*        these data should match the function used, as given by the
*        suffix on the function name. The storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
*        (i.e. Fortran array storage order).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of points at which the input grid is to be
*        resampled.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each output point, this array should contain the zero-based
*        offset in the output array(s) (i.e. the "out" and,
*        optionally, the "out_var" arrays) at which the resampled
*        output value(s) should be stored.
*     coords
*        An array of pointers to double, with "ndim_in"
*        elements. Element "coords[coord]" should point at the first
*        element of an array of double (with "npoint" elements) which
*        contains the values of coordinate number "coord" for each
*        interpolation point. The value of coordinate number "coord"
*        for interpolation point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices are
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding output data (and
*        variance) will be set to the value given by "badval".
*     flags
*        The bitwise OR of a set of flag values which control the
*        operation of the function. Currently, only the flag
*        AST__USEBAD is significant and indicates whether there are
*        "bad" (i.e. missing) data in the input array(s) which must be
*        recognised and propagated to the output array(s).  If this
*        flag is not set, all input values are treated literally.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned. Note
*        that details of how the output grid maps on to this array
*        (e.g. the storage order, number of dimensions, etc.) is
*        arbitrary and is specified entirely by means of the "offset"
*        array. The "out" array should therefore contain sufficient
*        elements to accommodate the "offset" values supplied.  There
*        is no requirement that all elements of the "out" array should
*        be assigned values, and any which are not addressed by the
*        contents of the "offset" array will be left unchanged.
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given. It is addressed in
*        exactly the same way (via the "offset" array) as the "out"
*        array. The values returned are estimates of the statistical
*        variance of the corresponding values in the "out" array, on
*        the assumption that all errors in input grid values (in the
*        "in" array) are statistically independent and that their
*        variance estimates (in the "in_var" array) may simply be
*        summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.

*  Returned Value:
*     The number of output grid points to which a data value (or a
*     variance value if relevant) equal to "badval" has been assigned
*     because no valid output value could be obtained.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*     - A value of zero will be returned if any of these functions is
*     invoked with the global error status set, or if it should fail
*     for any reason.
*/
/* Define a macro to implement the function for a specific data
   type. */
#define MAKE_INTERPOLATE_NEAREST(X,Xtype,Xsigned) \
static int InterpolateNearest##X( int ndim_in, \
                                  const int *lbnd_in, const int *ubnd_in, \
                                  const Xtype *in, const Xtype *in_var, \
                                  int npoint, const int *offset, \
                                  const double *const *coords, \
                                  int flags, Xtype badval, \
                                  Xtype *out, Xtype *out_var ) { \
\
/* Local Variables: */ \
   Xtype var;                    /* Variance value */ \
   double *xn_max;               /* Pointer to upper limits array (n-d) */ \
   double *xn_min;               /* Pointer to lower limits array (n-d) */ \
   double x;                     /* x coordinate value */ \
   double xmax;                  /* x upper limit */ \
   double xmin;                  /* x lower limit */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   double ymax;                  /* y upper limit */ \
   double ymin;                  /* y lower limit */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ix;                       /* Number of pixels offset in x direction */ \
   int ixn;                      /* Number of pixels offset (n-d) */ \
   int iy;                       /* Number of pixels offset in y direction */ \
   int off_in;                   /* Pixel offset into input array */ \
   int off_out;                  /* Pixel offset into output array */ \
   int point;                    /* Loop counter for output points */ \
   int result;                   /* Returned result value */ \
   int s;                        /* Temporary variable for strides */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int ystride;                  /* Stride along input grid y direction */ \
\
/* Initialise. */ \
   result = 0; \
\
/* Check the global error status. */ \
   if ( !astOK ) return result; \
\
/* Initialise variables to avoid "used of uninitialised variable" \
   messages from dumb compilers. */ \
   bad = 0; \
   off_in = 0; \
\
/* Determine if we are processing bad pixels or variances. */ \
   usebad = flags & AST__USEBAD; \
   usevar = in_var && out_var; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   if ( ndim_in == 1 ) { \
\
/* Calculate the coordinate limits of the input array. */ \
      xmin = (double) lbnd_in[ 0 ] - 0.5; \
      xmax = (double) ubnd_in[ 0 ] + 0.5; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,0,0) \
            } \
         } \
      } \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   } else if ( ndim_in == 2 ) { \
\
/* Calculate the stride along the y dimension of the input grid. */ \
      ystride = ubnd_in[ 0 ] - lbnd_in[ 0 ] + 1; \
\
/* Calculate the coordinate limits of the input array in each \
   dimension. */ \
      xmin = (double) lbnd_in[ 0 ] - 0.5; \
      xmax = (double) ubnd_in[ 0 ] + 0.5; \
      ymin = (double) lbnd_in[ 1 ] - 0.5; \
      ymax = (double) ubnd_in[ 1 ] + 0.5; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,0,0) \
            } \
         } \
      } \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
   } else { \
\
/* Allocate workspace. */ \
      stride = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      xn_max = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      xn_min = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      if ( astOK ) { \
\
/* Calculate the stride along each dimension of the input grid. */ \
         for ( s = 1, idim = 0; idim < ndim_in; idim++ ) { \
            stride[ idim ] = s; \
            s *= ubnd_in[ idim ] - lbnd_in[ idim ] + 1; \
\
/* Calculate the coordinate limits of the input grid in each \
   dimension. */ \
            xn_min[ idim ] = (double) lbnd_in[ idim ] - 0.5; \
            xn_max[ idim ] = (double) ubnd_in[ idim ] + 0.5; \
         } \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
         if ( usebad ) { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,1,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,1,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,1,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,1,0) \
               } \
            } \
         } else { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,0,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,0,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,0,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,0,0) \
               } \
            } \
         } \
      } \
\
/* Free the workspace. */ \
      stride = astFree( stride ); \
      xn_max = astFree( xn_max ); \
      xn_min = astFree( xn_min ); \
   } \
\
/* If an error has occurred, clear the returned result. */ \
   if ( !astOK ) result = 0; \
\
/* Return the result. */ \
   return result; \
}

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 1-dimensional
   case. */
#define ASSEMBLE_INPUT_1D(X,Xtype,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the input grid, or is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If not, then obtain the offset within the input grid of the pixel \
   which contains the current point. */ \
      off_in = (int) floor( x + 0.5 ) - lbnd_in[ 0 ]; \
\
/* If necessary, test if the input pixel is bad. */ \
      if ( Usebad ) bad = ( in[ off_in ] == badval ); \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 2-dimensional
   case. */
#define ASSEMBLE_INPUT_2D(X,Xtype,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the input grid, or is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If not, then similarly obtain and test the y coordinate. */ \
      y = coords[ 1 ][ point ]; \
      bad = ( y < ymin ) || ( y >= ymax ) || ( y == AST__BAD ); \
      if ( !bad ) { \
\
/* Obtain the offsets along each input grid dimension of the input \
   pixel which contains the current point. */ \
         ix = (int) floor( x + 0.5 ) - lbnd_in[ 0 ]; \
         iy = (int) floor( y + 0.5 ) - lbnd_in[ 1 ]; \
\
/* Calculate this pixel's offset from the start of the input array. */ \
         off_in = ix + ystride * iy; \
\
/* If necessary, test if the input pixel is bad. */ \
         if ( Usebad ) bad = ( in[ off_in ] == badval ); \
      } \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the n-dimensional
   case. */
#define ASSEMBLE_INPUT_ND(X,Xtype,Usebad,Usevar) \
\
/* Initialise the offset into the input array. Then loop to obtain \
   each coordinate associated with the current output point. */ \
   off_in = 0; \
   for ( idim = 0; idim < ndim_in; idim++ ) { \
      xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate lies outside the input grid, or is bad. If \
   either is true, the corresponding output pixel value will be bad, \
   so give up on this point. */ \
      bad = ( xn < xn_min[ idim ] ) || ( xn >= xn_max[ idim ] ) || \
            ( xn == AST__BAD ); \
      if ( bad ) break; \
\
/* Obtain the offset along the current input grid dimension of the \
   input pixel which contains the current point. */ \
      ixn = (int) floor( xn + 0.5 ) - lbnd_in[ idim ]; \
\
/* Accumulate this pixel's offset from the start of the input \
   array. */ \
      off_in += ixn * stride[ idim ]; \
   } \
\
/* Once the required input pixel has been located, test if it is \
   bad, if necessary. */ \
   if ( Usebad ) bad = bad || ( in[ off_in ] == badval );

/* This subsidiary macro assigns the output value (and variance) to
   the output array(s). */
#define CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xsigned,Usebad,Usevar) \
\
/* Obtain the pixel offset into the output array. */ \
   off_out = offset[ point ]; \
\
/* If the input data value is bad, assign a bad output value (and \
   variance, if required) and count it. */ \
   if ( bad ) { \
      out[ off_out ] = badval; \
      if ( Usevar ) out_var[ off_out ] = badval; \
      result++; \
\
/* Otherwise, assign the value obtained from the input grid. */ \
   } else { \
      out[ off_out ] = in[ off_in ]; \
\
/* If required, obtain the associated variance value. If necessary, \
   test if it is bad (if the data type is signed, also check that it \
   is not negative). */ \
      if ( Usevar ) { \
         var = in_var[ off_in ]; \
         if ( Usebad ) bad = ( var == badval ); \
         CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If the variance value can be bad, and is, then store a bad value in \
   the output array and count it. Otherwise, store the variance \
   value. */ \
         if ( ( ( Xsigned ) || ( Usebad ) ) && bad ) { \
            out_var[ off_out ] = badval; \
            result++; \
         } else { \
            out_var[ off_out ] = var; \
         } \
      } \
   }

/* This subsidiary macro tests for negative variance values in the
   macros above.  This check is required only for signed data
   types. */
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
   bad = bad || ( var < ( (Xtype) 0 ) );

/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_NEAREST(LD,long double,1)
#endif
MAKE_INTERPOLATE_NEAREST(D,double,1)
MAKE_INTERPOLATE_NEAREST(F,float,1)
MAKE_INTERPOLATE_NEAREST(L,long int,1)
MAKE_INTERPOLATE_NEAREST(I,int,1)
MAKE_INTERPOLATE_NEAREST(S,short int,1)
MAKE_INTERPOLATE_NEAREST(B,signed char,1)

/* Re-define the macro for testing for negative variances to do
   nothing. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype)

/* Expand the main macro above to generate a function for each
   required unsigned data type. */
MAKE_INTERPOLATE_NEAREST(UL,unsigned long int,0)
MAKE_INTERPOLATE_NEAREST(UI,unsigned int,0)
MAKE_INTERPOLATE_NEAREST(US,unsigned short int,0)
MAKE_INTERPOLATE_NEAREST(UB,unsigned char,0)

/* Undefine the macros used above. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#undef CALC_AND_ASSIGN_OUTPUT
#undef ASSEMBLE_INPUT_ND
#undef ASSEMBLE_INPUT_2D
#undef ASSEMBLE_INPUT_1D
#undef MAKE_INTERPOLATE_NEAREST

/*
*  Name:
*     InterpolateBlockAverage<X>

*  Purpose:
*     Resample a data grid, using multidimensional block averaging.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void InterpolateBlockAverage<X>( int ndim_in,
*                                      const int lbnd_in[], 
*                                      const int ubnd_in[],
*                                      const <Xtype> in[], 
*                                      const <Xtype> in_var[],
*                                      int npoint, const int offset[],
*                                      const double *const coords[],
*                                      const double params[], int flags,
*                                      <Xtype> badval, <Xtype> *out, 
*                                      <Xtype> *out_var, int *nbad )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which resample a rectangular input
*     grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. To generate 
*     an output grid pixel, a block average is taken over an ndim-
*     dimensional hypercube of pixels in the input grid.  If variances
*     are being used then the input pixels will be weighted according
*     to the reciprocals of the corresponding variance values, and 
*     input pixels without a valid variance will be ignored;
*     otherwise an unweighted average will be taken over
*     all non-bad pixels in the cube.  The size of the cube over which
*     the average is taken is determined by the first element of the 
*     params array.
*
*     This "interpolation" scheme is appropriate where an input grid 
*     is to be resampled onto a much coarser output grid.

*  Parameters:
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input grid, its extent along a particular
*        (i'th) dimension being ubnd_in[i]-lbnd_in[i]+1 (assuming "i"
*        is zero-based). They also define the input grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be resampled (with an element
*        for each pixel in the input grid). The numerical type of
*        these data should match the function used, as given by the
*        suffix on the function name. The storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
*        (i.e. Fortran array storage order).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of points at which the input grid is to be
*        resampled.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each output point, this array should contain the zero-based
*        offset in the output array(s) (i.e. the "out" and,
*        optionally, the "out_var" arrays) at which the resampled
*        output value(s) should be stored.
*     coords
*        An array of pointers to double, with "ndim_in"
*        elements. Element "coords[coord]" should point at the first
*        element of an array of double (with "npoint" elements) which
*        contains the values of coordinate number "coord" for each
*        interpolation point. The value of coordinate number "coord"
*        for interpolation point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices are
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding output data (and
*        variance) will be set to the value given by "badval".
*     params
*        A pointer to an array of doubles giving further information
*        about how the resampling is to proceed.  Only the first 
*        element is significant; the nearest integer to this gives 
*        the number of pixels on either side of the central input
*        grid pixel to use in each dimension.  Therefore 
*        (1 + 2*params[0])**ndim_in pixels will be averaged over to
*        generate each output pixel.
*     flags
*        The bitwise OR of a set of flag values which control the
*        operation of the function. Currently, only the flag
*        AST__USEBAD is significant and indicates whether there are
*        "bad" (i.e. missing) data in the input array(s) which must be
*        recognised and propagated to the output array(s).  If this
*        flag is not set, all input values are treated literally.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned. Note
*        that details of how the output grid maps on to this array
*        (e.g. the storage order, number of dimensions, etc.) is
*        arbitrary and is specified entirely by means of the "offset"
*        array. The "out" array should therefore contain sufficient
*        elements to accommodate the "offset" values supplied.  There
*        is no requirement that all elements of the "out" array should
*        be assigned values, and any which are not addressed by the
*        contents of the "offset" array will be left unchanged.
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given. It is addressed in
*        exactly the same way (via the "offset" array) as the "out"
*        array. The values returned are estimates of the statistical
*        variance of the corresponding values in the "out" array, on
*        the assumption that all errors in input grid values (in the
*        "in" array) are statistically independent and that their
*        variance estimates (in the "in_var" array) may simply be
*        summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     nbad
*        Pointer to an int in which to return the number of
*        interpolation points at which an output data value (and/or a
*        variance value if relevant) equal to "badval" has been 
*        assigned because no valid interpolated value could be
*        obtained.  The maximum value that will be returned is
*        "npoint" and the minimum is zero (indicating that all output
*        values were successfully obtained).

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*/
/* Define a macro to implement the function for a specific data
   type. */
#define MAKE_INTERPOLATE_BLOCKAVE(X,Xtype,Xfloating,Xfloattype,Xsigned) \
static void InterpolateBlockAverage##X( int ndim_in, \
                                        const int lbnd_in[], \
                                        const int ubnd_in[], \
                                        const Xtype in[], \
                                        const Xtype in_var[], \
                                        int npoint, const int offset[], \
                                        const double *const coords[], \
                                        const double params[], int flags, \
                                        Xtype badval, Xtype *out, \
                                        Xtype *out_var, int *nbad ) { \
\
/* Local Variables: */ \
   Xfloattype hi_lim;            /* Upper limit on output values */ \
   Xfloattype lo_lim;            /* Lower limit on output values */ \
   Xfloattype pixwt;             /* Weight to apply to individual pixel */ \
   Xfloattype sum;               /* Weighted sum of pixel data values */ \
   Xfloattype sum_var;           /* Weighted sum of pixel variance values */ \
   Xfloattype val;               /* Data value to be assigned to output */ \
   Xfloattype val_var;           /* Variance to be assigned to output */ \
   Xfloattype wtsum;             /* Sum of weight values */ \
   Xfloattype wtsum_sq;          /* Square of sum of weights */ \
   Xtype var;                    /* Variance value */ \
   double *xn_max;               /* Pointer to upper limits array (n-d) */ \
   double *xn_min;               /* Pointer to lower limits array (n-d) */ \
   double x;                     /* x coordinate value */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   int *hi;                      /* Pointer to array of upper indices */ \
   int *ixm;                     /* Pointer to array of current indices */ \
   int *lo;                      /* Pointer to array of lower indices */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int bad_var;                  /* Output variance bad? */ \
   int done;                     /* All pixel indices done? */ \
   int hi_x;                     /* Upper pixel index (x dimension) */ \
   int hi_y;                     /* Upper pixel index (y dimension) */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ix;                       /* Pixel index in input grid x dimension */ \
   int ixn;                      /* Pixel index in input grid (n-d) */ \
   int iy;                       /* Pixel index in input grid y dimension */ \
   int lo_x;                     /* Lower pixel index (x dimension) */ \
   int lo_y;                     /* Lower pixel index (y dimension) */ \
   int neighb;                   /* Number of adjacent pixels on each side */ \
   int off1;                     /* Input pixel offset due to y index */ \
   int off_in;                   /* Offset to input pixel */ \
   int off_out;                  /* Offset to output pixel */ \
   int point;                    /* Loop counter for output points */ \
   int s;                        /* Temporary variable for strides */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int ystride;                  /* Stride along input grid y dimension */ \
\
/* Initialise. */ \
   *nbad = 0; \
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
/* Initialise variables to avoid "used of uninitialised variable" \
   messages from dumb compilers. */ \
   val = 0; \
   val_var = 0; \
   sum_var = 0; \
   wtsum = 0; \
   bad = 0; \
   bad_var = 0; \
\
/* Determine if we are processing bad pixels or variances. */ \
   usebad = flags & AST__USEBAD; \
   usevar = in_var && out_var; \
\
/* Set the number of pixels each side of central pixel to use. */ \
   neighb = (int) floor( params[ 0 ] + 0.5 ); \
\
/* Set up limits for checking output values to ensure that they do not \
   overflow the range of the data type being used. */ \
   lo_lim = LO_##X; \
   hi_lim = HI_##X; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   if ( ndim_in == 1 ) { \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,0) \
            } \
         } \
      } \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   } else if ( ndim_in == 2 ) { \
\
/* Calculate the stride along the y dimension of the input grid. */ \
      ystride = ubnd_in[ 0 ] - lbnd_in[ 0 ] + 1; \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
      if ( usebad ) { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,0) \
            } \
         } \
      } else { \
         if ( usevar ) { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,1) \
            } \
         } else { \
            for ( point = 0; point < npoint; point++ ) { \
               ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
               CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,0) \
            } \
         } \
      } \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
   } else { \
\
/* Allocate workspace. */ \
      hi = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      lo = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      stride = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      ixm = astMalloc( sizeof( int ) * (size_t) ndim_in ); \
      xn_max = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      xn_min = astMalloc( sizeof( double ) * (size_t) ndim_in ); \
      if ( astOK ) { \
\
/* Calculate the stride along each dimension of the input grid. */ \
         for ( s = 1, idim = 0; idim < ndim_in; idim++ ) { \
            stride[ idim ] = s; \
            s *= ubnd_in[ idim ] - lbnd_in[ idim ] + 1; \
\
/* Calculate the coordinate limits of the input grid in each \
   dimension. */ \
            xn_min[ idim ] = (double) lbnd_in[ idim ] - 0.5; \
            xn_max[ idim ] = (double) ubnd_in[ idim ] + 0.5; \
         } \
\
/* Identify four cases, according to whether bad pixels and/or \
   variances are being processed. In each case, loop through all the \
   output points to (a) assemble the input data needed to form the \
   interpolated value, and (b) calculate the result and assign it to \
   the output arrays(s). In each case we assign constant values (0 or \
   1) to the "Usebad" and "Usevar" flags so that code for handling bad \
   pixels and variances can be eliminated when not required. */ \
         if ( usebad ) { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,1,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,1,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,1,0) \
               } \
            } \
         } else { \
            if ( usevar ) { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,0,1) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,1) \
               } \
            } else { \
               for ( point = 0; point < npoint; point++ ) { \
                  ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,0,0) \
                  CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,0,0) \
               } \
            } \
         } \
      } \
\
/* Free the workspace. */ \
      hi = astFree( hi ); \
      lo = astFree( lo ); \
      stride = astFree( stride ); \
      ixm = astFree( ixm ); \
      xn_max = astFree( xn_max ); \
      xn_min = astFree( xn_min ); \
   } \
\
/* If an error has occurred, clear the returned result. */ \
   if ( !astOK ) *nbad = 0; \
\
/* Return. */ \
}

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 1-dimensional
   case. */
#define ASSEMBLE_INPUT_1D(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x == AST__BAD ); \
\
/* Note we do not need to check here whether the pixel in this position is \
   bad; if any pixels in the cube are good we can form an average. */ \
\
/* If OK, calculate the lowest and highest indices (in the x \
   dimension) of the region of neighbouring pixels that will \
   contribute to the interpolated result.  Constrain these values to \
   lie within the input grid. */ \
   if ( !bad ) { \
      ix = (int) floor( x ); \
      lo_x = MaxI( ix - neighb + 1, lbnd_in[ 0 ] ); \
      hi_x = MinI( ix + neighb,     ubnd_in[ 0 ] ); \
\
/* Initialise sums for forming the interpolated result. */ \
      sum = (Xfloattype) 0.0; \
      wtsum = (Xfloattype) 0.0; \
      if ( Usevar ) { \
         sum_var = (Xfloattype) 0.0; \
         bad_var = 0; \
      } \
\
/* Loop to inspect all the contributing pixels, calculating the offset \
   of each pixel from the start of the input array. */ \
      off_in = lo_x - lbnd_in[ 0 ]; \
      for ( ix = lo_x; ix <= hi_x; ix++, off_in++ ) { \
\
/* If necessary, test if the input pixel is bad. */ \
         if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
\
/* If we are using variances, then check that the variance is valid; \
   if it is invalid then ignore this pixel altogether. */ \
            if ( Usevar ) { \
               var = in_var[ off_in ]; \
               if ( Usebad ) bad_var = ( var == badval ); \
               CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If variance is valid then accumulate suitably weighted values into \
   the totals. */ \
               if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                  pixwt = (Xfloattype) 1.0 / var; \
                  sum += pixwt * ( (Xfloattype) in[ off_in ] ); \
                  wtsum += pixwt; \
                  sum_var += pixwt; \
               } \
\
/* If we are not using variances, then accumulate values into the \
   totals with a weighting of unity. */ \
            } else { \
               sum += (Xfloattype) in[ off_in ]; \
               wtsum++; \
            } \
         } \
      } \
   }

/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the 2-dimensional
   case. */ 
#define ASSEMBLE_INPUT_2D(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Obtain the x coordinate of the current point and test if it is bad. */ \
   x = coords[ 0 ][ point ]; \
   bad = ( x == AST__BAD ); \
   if ( !bad ) { \
\
/* If not, then similarly obtain and test the y coordinate. */ \
      y = coords[ 1 ][ point ]; \
      bad = ( y == AST__BAD ); \
\
/* Note we do not need to check here whether the pixel in this position is \
   bad; if any pixels in the cube are good we can form an average. */ \
\
/* If OK, calculate the lowest and highest indices (in each dimension) \
   of the region of neighbouring pixels that will contribute to the \
   interpolated result.  Constrain these values to lie within the input \
   grid. */ \
      if ( !bad ) { \
         ix = (int) floor( x ); \
         lo_x = MaxI( ix - neighb + 1, lbnd_in[ 0 ] ); \
         hi_x = MinI( ix + neighb,     ubnd_in[ 0 ] ); \
         iy = (int) floor( y ); \
         lo_y = MaxI( iy - neighb + 1, lbnd_in[ 1 ] ); \
         hi_y = MinI( iy + neighb,     ubnd_in[ 1 ] ); \
\
/* Initialise sums for forming the interpolated result. */ \
         sum = (Xfloattype) 0.0; \
         wtsum = (Xfloattype) 0.0; \
         if ( Usevar ) { \
            sum_var = (Xfloattype) 0.0; \
            bad_var = 0; \
         } \
\
/* Loop to inspect all the contributing pixels, calculating the offset \
   of each pixel from the start of the input array. */ \
         off1 = lo_x - lbnd_in[ 0 ] + ystride * ( lo_y - lbnd_in[ 1 ] ); \
         for ( iy = lo_y; iy <= hi_y; iy++, off1 += ystride ) { \
            off_in = off1; \
            for ( ix = lo_x; ix <= hi_x; ix++, off_in++ ) { \
\
/* If necessary, test if the input pixel is bad. */ \
               if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
\
/* If we are using variances, then check that the variance is valid; \
   if it is invalid then ignore this pixel altogether. */ \
                  if ( Usevar ) { \
                     var = in_var[ off_in ]; \
                     if ( Usebad ) bad_var = ( var == badval ); \
                     CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If variance is valid then accumulate suitably weighted values into \
   the totals. */ \
                     if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                        pixwt = (Xfloattype) 1.0 / var; \
                        sum += pixwt * ( (Xfloattype) in[ off_in ] ); \
                        wtsum += pixwt; \
                        sum_var += pixwt; \
                     } \
\
/* If we are not using variances, then accumulate values into the \
   totals with a weighting of unity. */ \
                  } else { \
                     sum += (Xfloattype) in[ off_in ]; \
                     wtsum++; \
                  } \
               } \
            } \
         } \
      } \
   }
 
/* This subsidiary macro assembles the input data needed in
   preparation for forming the interpolated value in the n-dimensional
   case. */
#define ASSEMBLE_INPUT_ND(X,Xtype,Xfloating,Xfloattype,Xsigned,Usebad,Usevar) \
\
/* Initialise offsets into the input array.  then loop to obtain each \
   coordinate associated with the current output poitn. */ \
   off_in = 0; \
   for ( idim = 0; idim < ndim_in; idim++ ) { \
      xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate is bad. If so give up on this point. */ \
      bad = ( xn == AST__BAD ); \
      if ( bad ) break; \
\
/* Calculate the lowest and highest indices (in the current dimension) \
   of the region of neighbouring pixels that will contribute to the \
   interpolated result. Constrain these values to lie within the input \
   grid. */ \
      ixn = (int) floor( xn ); \
      lo[ idim ] = MaxI( ixn - neighb + 1, lbnd_in[ idim ] ); \
      hi[ idim ] = MinI( ixn + neighb,     ubnd_in[ idim ] ); \
\
/* If the cube has a zero dimension then no data can come from it. */ \
      bad = ( lo[ idim ] > hi[ idim ] ); \
      if ( bad ) break; \
\
/* Accumulate the offset (from the start of the input array) of the \
   contributing pixel which has the lowest index in each dimension. */ \
      off_in += stride[ idim ] * ( lo[ idim ] - lbnd_in[ idim ] ); \
\
/* Initialise an array to keep track of the current position in the \
   input cube. */ \
      ixm[ idim ] = lo[ idim ]; \
   } \
\
/* Note we do not need to check here whether the pixel in this position is \
   bad; if any pixels in the cube are good we can form an average. */ \
\
/* If OK, initialise sums for forming the interpolated result. */ \
   if ( !bad ) { \
      sum = (Xfloattype) 0.0; \
      wtsum= (Xfloattype) 0.0; \
      if ( Usevar ) { \
         sum_var = (Xfloattype) 0.0; \
         bad_var = 0; \
      } \
\
/* Loop to inspect all the contributing pixels, calculating the offset \
   of each pixel from the start of the input array. */ \
      do { \
\
/* If necessary, test if the input pixel is bad. */ \
         if ( !( Usebad ) || ( in[ off_in ] != badval ) ) { \
\
/* If we are using variances, then check that the variance is valid; \
   if it is invalid then ignore this pixel altogether. */ \
            if ( Usevar ) { \
               var = in_var[ off_in ]; \
               if ( Usebad ) bad_var = ( var == badval ); \
               CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
\
/* If variance is valid then accumulate suitably weighted values into \
   the totals. */ \
               if ( !( ( Xsigned ) || ( Usebad ) ) || !bad_var ) { \
                  pixwt = (Xfloattype) 1.0 / var; \
                  sum += pixwt * ( (Xfloattype) in[ off_in ] ); \
                  wtsum += pixwt; \
                  sum_var += pixwt; \
               } \
\
/* If we are not using variances, then accumulate values into the \
   totals with a weighting of unity. */ \
            } else { \
               sum += (Xfloattype) in[ off_in ]; \
               wtsum++; \
            } \
         } \
\
/* Locate the next pixel in the input cube; try incrementing the lowest \
   dimension index first, if that rolls over increment the next  \
   dimension index, and so on. */ \
         for ( idim = 0; idim < ndim_in; idim++ ) { \
            if ( ixm[ idim ] < hi[ idim ] ) { \
               off_in += stride[ idim ]; \
               ixm[ idim ]++; \
               break; \
            } else { \
               off_in -= stride[ idim ] * ( hi[ idim ] - lo[ idim ] ); \
               ixm[ idim ] = lo[ idim ]; \
            } \
         } \
\
/* If the highest dimension index has rolled over, we have done all \
   the pixels in the cube. */ \
         done = ( idim == ndim_in ); \
      } while ( !done ); \
   }

/* This subsidiary macro calculates the interpolated output value (and
   variance) from the sums over contributing pixels, checks the
   results for validity, and assigns them to the output array(s). */
#define CALC_AND_ASSIGN_OUTPUT(X,Xtype,Xfloating,Xfloattype,Usebad,Usevar) \
\
/* If the output data value has not yet been flagged as bad, then \
   check that an interpolated value can actually be produced.  First \
   check that the sum of weights is not zero. */ \
   if ( !bad ) { \
      bad = ( wtsum == (Xfloattype) 0.0 ); \
\
/* If OK, calculate the interpolated value. Then, if the output data \
   type is not floating point, check that this value will not overflow \
   the available output range. */ \
      if ( !bad ) { \
         val = sum / wtsum; \
         if ( !( Xfloating ) ) { \
            bad = ( val <= lo_lim ) || ( val >= hi_lim ); \
         } \
      } \
\
/* If no interpolated data value can be produced, then no associated \
   variance will be required either. */ \
      if ( ( Usevar ) && bad ) bad_var = 1; \
   } \
\
/* Now perform similar checks on the output variance value (if \
   required). This time we check that the square of the sum of \
   weights is not zero (since this might underflow before the sum of \
   weights). Again we also check to prevent the result overflowing the \
   output data type. */ \
   if ( ( Usevar ) && !bad_var ) { \
      wtsum_sq = wtsum * wtsum; \
      bad_var = ( wtsum_sq == (Xfloattype) 0.0 ); \
      if ( !bad_var ) { \
         val_var = sum_var / wtsum_sq; \
         if ( !( Xfloating ) ) { \
            bad_var = ( val_var <= lo_lim ) || ( val_var >= hi_lim ); \
         } \
      } \
   } \
\
/* Obtain the pixel offset into the output array. */ \
   off_out = offset[ point ]; \
\
/* Assign a bad output value (and variance) if required and count it. */ \
   if ( bad ) { \
      out[ off_out ] = badval; \
      if ( Usevar ) out_var[ off_out ] = badval; \
      (*nbad)++; \
\
/* Otherwise, assign the interpolated value. If the output data type \
   is floating point, the result can be stored directly, otherwise we \
   must round to the nearest integer. */ \
   } else { \
      if ( Xfloating ) { \
         out[ off_out ] = (Xtype) val; \
      } else { \
         out[ off_out ] = (Xtype) ( val + ( ( val >= (Xfloattype) 0.0 ) ? \
                                            ( (Xfloattype) 0.5 ) : \
                                            ( (Xfloattype) -0.5 ) ) ); \
      } \
\
/* If a variance estimate is required but none can be obtained, then \
   store a bad output variance value and count it. */ \
      if ( Usevar ) { \
         if ( bad_var ) { \
            out_var[ off_out ] = badval; \
            (*nbad)++; \
\
/* Otherwise, store the variance estimate, rounding to the nearest \
   integer if necessary. */ \
         } else { \
            if ( Xfloating ) { \
               out_var[ off_out ] = (Xtype) val_var; \
            } else { \
               out_var[ off_out ] = (Xtype) ( val_var + \
                                          ( ( val_var >= (Xfloattype) 0.0 ) ? \
                                            ( (Xfloattype) 0.5 ) : \
                                            ( (Xfloattype) -0.5 ) ) ); \
            } \
         } \
      } \
   }

/* These subsidiary macros define limits for range checking of results
   before conversion to the final data type. For each data type code
   <X>, HI_<X> gives the least positive floating point value which
   just overflows that data type towards plus infinity, while LO_<X>
   gives the least negative floating point value which just overflows
   that data type towards minus infinity. Thus, a floating point value
   must satisfy LO<flt_value<HI if overflow is not to occur when it is
   converted to that data type.

   The data type of each limit should be that of the smallest
   precision floating point type which will accommodate the full range
   of values that the target type may take. */

/* If <X> is a floating point type, the limits are not actually used,
   but must be present to permit error-free compilation. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
#define HI_LD ( 0.0L )
#define LO_LD ( 0.0L )
#endif
#define HI_D ( 0.0 )
#define LO_D ( 0.0 )
#define HI_F ( 0.0f )
#define LO_F ( 0.0f )

#if HAVE_LONG_DOUBLE     /* Not normally implemented */
#define HI_L   ( 0.5L + (long double) LONG_MAX )
#define LO_L  ( -0.5L + (long double) LONG_MIN )
#define HI_UL  ( 0.5L + (long double) ULONG_MAX )
#define LO_UL ( -0.5L )
#else
#define HI_L   ( 0.5 + (double) LONG_MAX )
#define LO_L  ( -0.5 + (double) LONG_MIN )
#define HI_UL  ( 0.5 + (double) ULONG_MAX )
#define LO_UL ( -0.5 )
#endif
#define HI_I   ( 0.5 + (double) INT_MAX )
#define LO_I  ( -0.5 + (double) INT_MIN )
#define HI_UI  ( 0.5 + (double) UINT_MAX )
#define LO_UI ( -0.5 )
#define HI_S   ( 0.5f + (float) SHRT_MAX )
#define LO_S  ( -0.5f + (float) SHRT_MIN )
#define HI_US  ( 0.5f + (float) USHRT_MAX )
#define LO_US ( -0.5f )
#define HI_B   ( 0.5f + (float) SCHAR_MAX )
#define LO_B  ( -0.5f + (float) SCHAR_MIN )
#define HI_UB  ( 0.5f + (float) UCHAR_MAX )
#define LO_UB ( -0.5f )

/* This subsidiary macro tests for negative variance values. This
   check is required only for signed data types. */
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype) \
   bad_var = bad_var || ( var < ( (Xtype) 0 ) );

/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_BLOCKAVE(LD,long double,1,long double,1)
MAKE_INTERPOLATE_BLOCKAVE(L,long int,0,long double,1)
#else
MAKE_INTERPOLATE_BLOCKAVE(L,long int,0,double,1)
#endif
MAKE_INTERPOLATE_BLOCKAVE(D,double,1,double,1)
MAKE_INTERPOLATE_BLOCKAVE(F,float,1,float,1)
MAKE_INTERPOLATE_BLOCKAVE(I,int,0,double,1)
MAKE_INTERPOLATE_BLOCKAVE(S,short int,0,float,1)
MAKE_INTERPOLATE_BLOCKAVE(B,signed char,0,float,1)

/* Re-define the macro for testing for negative variances to do
   nothing. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#define CHECK_FOR_NEGATIVE_VARIANCE(Xtype)

/* Expand the main macro above to generate a function for each
   required unsigned data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_INTERPOLATE_BLOCKAVE(UL,unsigned long int,0,long double,0)
#else
MAKE_INTERPOLATE_BLOCKAVE(UL,unsigned long int,0,double,0)
#endif
MAKE_INTERPOLATE_BLOCKAVE(UI,unsigned int,0,double,0)
MAKE_INTERPOLATE_BLOCKAVE(US,unsigned short int,0,float,0)
MAKE_INTERPOLATE_BLOCKAVE(UB,unsigned char,0,float,0)

/* Undefine the macros used above. */
#undef CHECK_FOR_NEGATIVE_VARIANCE
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
#undef HI_LD
#undef LO_LD
#endif
#undef HI_D
#undef LO_D
#undef HI_F
#undef LO_F
#undef HI_L
#undef LO_L
#undef HI_UL
#undef LO_UL
#undef HI_I
#undef LO_I
#undef HI_UI
#undef LO_UI
#undef HI_S
#undef LO_S
#undef HI_US
#undef LO_US
#undef HI_B
#undef LO_B
#undef HI_UB
#undef LO_UB
#undef CALC_AND_ASSIGN_OUTPUT
#undef ASSEMBLE_INPUT_ND
#undef ASSEMBLE_INPUT_2D
#undef ASSEMBLE_INPUT_1D
#undef MAKE_INTERPOLATE_BLOCKAVE


static PN *InterpPN( int np, double *x, double *y ) {
/*
*  Name:
*     InterpPN

*  Purpose:
*     Find a polynomial which interpolates the given points. 

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     static PN *InterpPN( int np, double *x, double *y )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function finds the coefficients of a polynomial which
*     interpolates the supplied positions. The order of the returned
*     polynomial is one less than the the number of supplied points 
*     (thus if 2 points are supplied, the polynomial will be of order 
*     1 - a straight line).

*  Parameters:
*     np
*        The number of points supplied in arrays x and y. This must be no
*        more than (RATE_ORDER+1).
*     x
*        Pointer to a an array of "np" x values. Note the values in this
*        array must increase monotonically.
*     y
*        Pointer to a an array of "np" y values.

*  Returned:
*     Pointer to a structure describing the polynomial, or NULL if no
*     polynomial could be found. The memory used to store the polynomial
*     should be freed using astFree when no longer needed.

*/

/* Local Variables: */
    int i, k;
    PN *pn[ RATE_ORDER + 1 ], *ret;

/* Initialise. */
    ret = NULL;

/* Check the global error status */
   if ( !astOK ) return ret;

/* Check supplied points are good. */
    if( np > RATE_ORDER + 1 ) return NULL;
    for( i = 0; i < np; i++ ) {
       if( x[ i ] == AST__BAD || y[ i ] == AST__BAD ) return NULL;
    }

/* Produce polynomials of order zero. */
    for( i = 0; i < np; i++ ) {
       pn[ i ] = astMalloc( sizeof( PN ) );
       if( !astOK ) return NULL;
       pn[ i ]->order = 0;
       pn[ i ]->coeff[ 0 ] = y[ i ];
       pn[ i ]->xlo = x[ i ];
       pn[ i ]->xhi = x[ i ];
    }

/* Produce polynomials of order "k", from the polynomials of order "k-1".
   This uses Neville's method (see:

      http://mathworld.wolfram.com/NevillesAlgorithm.html

*/
    for( k = 1; k < np; k++ ) {
       for( i = 0; i < np - k; i++ ) {
          CombinePN( pn[i], pn[i+1] );
       }
    }

    for( i = 1; i < np; i++ ) pn[ i ] = astFree( pn[ i ] );

/* If any of the coefficients could not be found return NULL. */
    ret = pn[ 0 ];
    for( i = 0; i < np; i++ ) {
       if( astISNAN( ret->coeff[ i ] ) ) {
          ret = astFree( ret );
          break;
       }
    }

    return ret;
}


static void Invert( AstMapping *this ) {
/*
*++
*  Name:
c     astInvert
f     AST_INVERT

*  Purpose:
*     Invert a Mapping.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astInvert( AstMapping *this )
f     CALL AST_INVERT( THIS, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function inverts a Mapping by reversing the boolean sense
f     This routine inverts a Mapping by reversing the boolean sense
*     of its Invert attribute. If this attribute is zero (the
*     default), the Mapping will transform coordinates in the way
*     specified when it was created. If it is non-zero, the input and
*     output coordinates will be inter-changed so that the direction
*     of the Mapping is reversed. This will cause it to display the
*     inverse of its original behaviour.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping.
f     STATUS = INTEGER (Given and Returned)
f        The global status.
*--
*/

/* Local Variables: */
   int invert;                   /* New Invert attribute value */

/* Check the global error status. */
   if ( !astOK ) return;

/* Determine the new Invert attribute value. */
   invert = !astGetInvert( this );

/* Clear the old value. */
   astClearInvert( this );

/* If the resulting default value is not the one required, then set a
   new value explicitly. */
   if ( astGetInvert( this ) != invert ) astSetInvert( this, invert );
}

static double J1Bessel( double x ) {
/*
*  Name:
*     J1Bessel

*  Purpose:
*     Calculates the first-order Bessel function of the first kind.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double J1Bessel( double x )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of the first-order Bessel function
*     of the first kind.

*  Parameters:
*     x
*        The argument for J1.

*  Returned Value:
*     The calculated J1(x) value.

*  Notes:
*     - The algorithm is taken from the SCUBA routine SCULIB_BESSJ1, by 
*     J.Lightfoot.
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   static double p1 = 1.0;
   static double p2 = 0.183105E-2;
   static double p3 = -0.3516396496E-4;
   static double p4 = 0.2457520174E-5;
   static double p5 = -0.240337019E-6;

   static double q1 = 0.04687499995;
   static double q2 = -0.2002690873E-3;
   static double q3 = 0.8449199096E-5;
   static double q4 = -0.88228987E-6;
   static double q5 = 0.105787412E-6;

   static double r1 = 72362614232.0;
   static double r2 = -7895059235.0;
   static double r3 = 242396853.1;
   static double r4 = -2972611.439;
   static double r5 = 15704.48260;
   static double r6 = -30.16036606;

   static double s1 = 144725228442.0;
   static double s2 = 2300535178.0;
   static double s3 = 18583304.74;
   static double s4 = 99447.43394;
   static double s5 = 376.9991397;
   static double s6 = 1.0;

   double ax;
   double xx;
   double z;
   double y;
   double value;
   int s;

/* Calculate the value */
   ax = fabs( x );
   if( ax < 8.0 ) {
      y = x*x;
      value = x*( r1 + y*( r2 + y*( r3 + y*( r4 + y*( r5 + y*r6 ) ) ) ) ) /
                       ( s1 + y*( s2 + y*( s3 + y*( s4 + y*( s5 + y*s6 ) ) ) ) );
   } else {
      s = ( x >= 0.0 ) ? 1 : -1;
      z = 8.0 / ax;
      y = z*z;
      xx = ax - 2.356194491;
      value = sqrt ( 0.636619772/ax )*( cos( xx )*( p1 + y*( p2 + y*
        ( p3 + y*( p4 + y*p5 ) ) ) )-z*sin( xx )*( q1 + y*( q2 + y*( q3 + y*
        ( q4 + y*q5 ) ) ) ) )*s;
   }

   return value;

}

static int LinearApprox( AstMapping *this, const double *lbnd, 
                         const double *ubnd, double tol, double *fit ) {
/*
*++
*  Name:
c     astLinearApprox
f     AST_LINEARAPPROX

*  Purpose:
*     Obtain a linear approximation to a Mapping, if appropriate.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     int astLinearApprox( AstMapping *this, const double *lbnd, 
c                          const double *ubnd, double tol, double *fit )
f     RESULT = AST_LINEARAPPROX( THIS, LBND, UBND, TOL, FIT, STATUS )

*  Class Membership:
*     Mapping function.

*  Description:
*     This function tests the forward coordinate transformation
*     implemented by a Mapping over a given range of input coordinates. If
*     the transformation is found to be linear to a specified level of
*     accuracy, then an array of fit coefficients is returned. These
*     may be used to implement a linear approximation to the Mapping's
*     forward transformation within the specified range of output coordinates. 
*     If the transformation is not sufficiently linear, no coefficients 
*     are returned.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping. 
c     lbnd
f     LBND( * ) = DOUBLE PRECISION (Given)
c        Pointer to an array of doubles
f        An array
*        containing the lower bounds of a box defined within the input 
*        coordinate system of the Mapping. The number of elements in this 
*        array should equal the value of the Mapping's Nin attribute. This
*        box should specify the region over which linearity is required.
c     ubnd
f     UBND( * ) = DOUBLE PRECISION (Given)
c        Pointer to an array of doubles
f        An array
*        containing the upper bounds of the box specifying the region over
*        which linearity is required.
c     tol
f     TOL = DOUBLE PRECISION (Given)
*        The maximum permitted deviation from linearity, expressed as
*        a positive Cartesian displacement in the output coordinate
*        space of the Mapping. If a linear fit to the forward
*        transformation of the Mapping deviates from the true transformation 
*        by more than this amount at any point which is tested, then no fit
*        coefficients will be returned.
c     fit
f     FIT( * ) = DOUBLE PRECISION (Returned)
c        Pointer to an array of doubles 
f        An array
*        in which to return the co-efficients of the linear
*        approximation to the specified transformation. This array should
*        have at least "( Nin + 1 ) * Nout", elements. The first Nout elements 
*        hold the constant offsets for the transformation outputs. The 
*        remaining elements hold the gradients. So if the Mapping has 2 inputs 
*        and 3 outputs the linear approximation to the forward transformation 
*        is:
*
c           X_out = fit[0] + fit[3]*X_in + fit[4]*Y_in 
f           X_out = fit(1) + fit(4)*X_in + fit(5)*Y_in 
*
c           Y_out = fit[1] + fit[5]*X_in + fit[6]*Y_in 
f           Y_out = fit(2) + fit(6)*X_in + fit(7)*Y_in 
*
c           Z_out = fit[2] + fit[7]*X_in + fit[8]*Y_in
f           Z_out = fit(3) + fit(8)*X_in + fit(9)*Y_in
*
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Returned Value:
c     astLinearApprox()
f     AST_LINEARAPPOX = LOGICAL
*        If the forward transformation is sufficiently linear,
c        a non-zero value is returned. Otherwise zero is returned 
f        .TRUE is returned. Otherwise .FALSE. is returned
*        and the fit co-efficients are set to AST__BAD.

*  Notes:
*     - This function fits the Mapping's forward transformation. To fit
*     the inverse transformation, the Mapping should be inverted using
c     astInvert
f     AST_INVERT
*     before invoking this function.
c     - A value of zero
f     - A value of .FALSE.
*     will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*--

*  Implementation Deficiencies:
*     Sub-classes which implement linear mappings should probably
*     over-ride this function to get better accuracy and faster execution, 
*     but currently they do not.

*/

/* Local Variables: */
   AstPointSet *pset_in_f;       /* PointSet for input fitting points */
   AstPointSet *pset_in_t;       /* PointSet for input test points */
   AstPointSet *pset_out_f;      /* PointSet for output fitting points */
   AstPointSet *pset_out_t;      /* PointSet for output test points */
   double **ptr_in_f;            /* Input coordinate array pointers */
   double **ptr_in_t;            /* Input coordinate array pointers */
   double **ptr_out_f;           /* Output coordinate array pointers */
   double **ptr_out_t;           /* Output coordinate array pointers */
   double *grad;                 /* Pointer to matrix of gradients */
   double *zero;                 /* Pointer to array of zero point values */
   double diff;                  /* Difference in coordinate values */
   double err;                   /* Sum of squared error */
   double frac;                  /* Fraction of input coordinate range */
   double in1;                   /* Input coordinate value */
   double in2;                   /* Input coordinate value */
   double indiff;                /* Difference in input coordinate values */
   double out1;                  /* Output coordinate value */
   double out2;                  /* Output coordinate value */
   double x0;                    /* Coordinate of grid centre */
   double y;                     /* Output coordinate (transformed) */
   double yfit;                  /* Coordinate resulting from fit */
   double z;                     /* Sum for calculating zero points */
   int *vertex;                  /* Pointer to flag array for vertices */
   int coord_in;                 /* Loop counter for input coordinates */
   int coord_out;                /* Loop counter for output coordinates. */
   int done;                     /* All vertices visited? */
   int face1;                    /* Index of first face coordinates */
   int face2;                    /* Index of second face coordinates */
   int face;                     /* Loop counter for faces */
   int ii;                       /* Index into gradient matrix */
   int linear;                   /* Mapping is linear? */
   int nc;                       /* Number of coeffs in fit */
   int ndim_in;                  /* Number of Mapping inputs */
   int ndim_out;                 /* Number of Mapping outputs */
   int npoint;                   /* Number of test points required */
   int point;                    /* Counter for points */
   int result;                   /* Returned flag */

/* Initialise. */
   result = 0;
   
/* Check the global error status. */
   if ( !astOK ) return result;

/* Further initialisation. */
   linear = 1;
   grad = NULL;
   zero = NULL;

/* Get the number of Mapping output and inputs. */
   ndim_in = astGetNin( this );
   ndim_out = astGetNout( this );

/* Store the number of coefficients in the fit.*/
   nc = ( ndim_in + 1 ) * ndim_out;

/* Create a PointSet to hold input coordinates and obtain a pointer
   to its coordinate arrays. */
   pset_in_f = astPointSet( 2 * ndim_in, ndim_in, "" );
   ptr_in_f = astGetPoints( pset_in_f );
   if ( astOK ) {

/* Set up and transform an initial set of points. */
/* ---------------------------------------------- */
/* Loop to set up input coordinates at the centre of each face of the
   input grid, storing them in the PointSet created above. */
      point = 0;
      for ( face = 0; face < ( 2 * ndim_in ); face++ ) {
         for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
            ptr_in_f[ coord_in ][ point ] =
               0.5 * ( lbnd[ coord_in ] + ubnd[ coord_in ] );
         }
         ptr_in_f[ face / 2 ][ point ] = ( face % 2 ) ?
                                          ubnd[ face / 2 ] : lbnd[ face / 2 ];
         point++;
      }
   }

/* Transform these coordinates into the output grid's coordinate system
   and obtain an array of pointers to the resulting coordinate
   data. */
   pset_out_f = astTransform( this, pset_in_f, 1, NULL );
   ptr_out_f = astGetPoints( pset_out_f );
   if ( astOK ) {

/* Fit a linear approximation to the points. */
/* ----------------------------------------- */
/* Obtain pointers to the locations in the fit coefficients array
   where the gradients and zero points should be stored. */
      grad = fit + ndim_out;
      zero = fit;

/* On the assumption that the transformation applied above is
   approximately linear, loop to determine the matrix of gradients and
   the zero points which describe it. */
      ii = 0;
      for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
         z = 0.0;
         for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {

/* Find the indices of opposite faces in each input dimension. */
            face1 = 2 * coord_in;
            face2 = face1 + 1;

/* Obtain the input and output coordinates at these face centres. */
            in1 = ptr_in_f[ coord_in ][ face1 ];
            in2 = ptr_in_f[ coord_in ][ face2 ];
            out1 = ptr_out_f[ coord_out ][ face1 ];
            out2 = ptr_out_f[ coord_out ][ face2 ];

/* Check whether any transformed coordinates are bad. If so, the
   transformation cannot be linear, so give up trying to fit it. */
            if ( ( out1 == AST__BAD ) || ( out2 == AST__BAD ) ) {
               linear = 0;
               break;
            }

/* If possible, determine the gradient along this dimension, storing
   it in the appropriate element of the gradient matrix. */
            indiff = in2 - in1;
            if ( indiff != 0.0 ) {
               grad[ ii++ ] = ( out2 - out1 ) / indiff;
            } else {
               grad[ ii++ ] = 0.0;
            }

/* Accumulate the sum used to determine the zero point. */
            z += ( out1 + out2 );
         }

/* Also quit the outer loop if a linear fit cannot be obtained. */
         if ( !linear ) break;

/* Determine the average zero point from all dimensions. */
         zero[ coord_out ] = z / (double) ( 2 * ndim_in );
      }

/* If a linear fit was obtained, its zero points will be appropriate
   to an input coordinate system with an origin at the centre of the
   input grid (we assume this to simplify the calculations above). To
   correct for this, we transform the actual input coordinates of the
   grid's centre through the matrix of gradients and subtract the
   resulting coordinates from the zero point values. The zero points
   are then correct for the actual output and input coordinate systems
   we are using. */
      if ( linear ) {
         ii = 0;
         for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
            for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
               x0 = 0.5 * ( lbnd[ coord_in ] + ubnd[ coord_in ] );
               zero[ coord_out ] -= grad[ ii++ ] * x0;
            }
         }
      }
   }

/* Annul the pointers to the PointSets used above. */
   pset_out_f = astAnnul( pset_out_f );
   pset_in_f = astAnnul( pset_in_f );

/* Calculate the number of test points required. */
/* --------------------------------------------- */
/* If we have obtained a linear fit above, it will (by construction)
   be exact at the centre of each face of the input grid. However, it
   may not fit anywhere else. We therefore set up some test points to
   determine if it is an adequate approximation elsewhere. */
   if ( astOK && linear ) {

/* Calculate the number of test points required to place one at each
   vertex of the grid. */
      npoint = 1;
      for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
         npoint *= 2;
      }

/* Now calculate the total number of test points required, also
   allowing one at the centre, one at half the distance to each face,
   and one at half the distance to each vertex. */
      npoint = 1 + 2 * ( ndim_in + npoint );

/* Set up test points in the input coordinate system. */
/* --------------------------------------------------- */   
/* Create a PointSet to hold the test coordinates and obtain an array
   of pointers to its coordinate data. */
      pset_in_t = astPointSet( npoint, ndim_in, "" );
      ptr_in_t = astGetPoints( pset_in_t );
      if ( astOK ) {

/* If the input array is 1-dimensional, the face and vertex positions
   calculated below will co-incide. Therefore, we simply distribute
   the required number of test points uniformly throughout the input
   coordinate range (avoiding the end-points, where the fit has been
   obtained). The coordinates are stored in the PointSet created
   above. */
         if ( ndim_in == 1 ) {
            for ( point = 0; point < npoint; point++ ) {
               frac = ( (double) ( point + 1 ) ) / (double) ( npoint + 1 );
               ptr_in_t[ 0 ][ point ] = ( 1.0 - frac ) * lbnd[ 0 ] +
                                         frac * ubnd[ 0 ];
            }

/* Otherwise, generate one point at the grid centre. */
         } else {
            point = 0;
            for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
               ptr_in_t[ coord_in ][ point ] =
                  0.5 * ( lbnd[ coord_in ] + ubnd[ coord_in ] );
            }
            point++;

/* Similarly generate a point half way between the grid centre and the
   centre of each face. */
            for ( face = 0; face < ( 2 * ndim_in ); face++ ) {
               for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                  ptr_in_t[ coord_in ][ point ] =
                     0.5 * ( lbnd[ coord_in ] + ubnd[ coord_in ] );
               }
               ptr_in_t[ face / 2 ][ point ] =
                  0.5 * ( ( ( ( face % 2 ) ? ubnd[ face / 2 ] :
                                             lbnd[ face / 2 ] ) ) +
                          ptr_in_t[ face / 2 ][ 0 ] );
               point++;
            }

/* Allocate workspace and initialise flags for identifying the
   vertices. */
            vertex = astMalloc( sizeof( int ) * (size_t) ndim_in );
            if ( astOK ) {
               for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                  vertex[ coord_in ] = 0;
               }

/* Now loop to visit each input grid vertex. */
               done = 0;
               do {

/* Generate a test point at each vertex. */
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     ptr_in_t[ coord_in ][ point ] = vertex[ coord_in ] ?
                                                       ubnd[ coord_in ] :
                                                       lbnd[ coord_in ];

/* Also place one half way between the grid centre and each vertex. */
                     ptr_in_t[ coord_in ][ point + 1 ] =
                        0.5 * ( ptr_in_t[ coord_in ][ point ] +
                                ptr_in_t[ coord_in ][ 0 ] );
                  }
                  point += 2;

/* Now update the array of vertex flags to identify the next vertex. */
                  coord_in = 0;
                  do {

/* The least significant dimension which does not have its upper bound
   as one of the vertex coordinates is changed to use its upper bound
   in the next vertex. */
                     if ( !vertex[ coord_in ] ) {
                        vertex[ coord_in ] = 1;
                        break;

/* Any less significant dimensions whose upper bounds are already
   being used are changed to use their lower bounds in the next
   vertex. */
                     } else {
                        vertex[ coord_in ] = 0;

/* All vertices have been visited when the most significant dimension
   is changed back to using its lower bound. */
                        done = ( ++coord_in == ndim_in );
                     }
                  } while ( !done );
               } while ( !done );
            }

/* Free the workspace used for vertex flags. */
            vertex = astFree( vertex );
         }

/* Transform the test points. */
/* -------------------------- */
/* Use the Mapping to transform the test points into the output grid's
   coordinate system, obtaining a pointer to the resulting arrays of
   output coordinates. */
         pset_out_t = astTransform( this, pset_in_t, 1, NULL );
         ptr_out_t = astGetPoints( pset_out_t );

/* Test the linear fit for accuracy. */
/* --------------------------------- */
/* If OK so far, then loop to use this fit to transform each test
   point and compare the result with the result of applying the
   Mapping. */
         if ( astOK ) {
            for ( point = 0; point < npoint; point++ ) {

/* Initialise the fitting error for the current point. */
               err = 0.0;

/* Obtain each output coordinate (produced by using the Mapping) in
   turn and check that it is not bad. If it is, then the
   transformation is not linear, so give up testing the fit. */
               ii = 0;
               for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                  y = ptr_out_t[ coord_out ][ point ];
                  if ( y == AST__BAD ) {
                     linear = 0;
                     break;
                  }

/* Apply the fitted transformation to the input coordinates to obtain
   the approximate output coordinate value. */
                  yfit = zero[ coord_out ];
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     yfit += grad[ ii++ ] * ptr_in_t[ coord_in ][ point ];
                  }

/* Form the sum of squared differences between the Mapping's
   transformation and the fit. */
                  diff = ( y - yfit );
                  err += diff * diff;
               }

/* Quit the outer loop if the Mapping is found to be non-linear. */
               if ( !linear ) break;

/* Test if the Cartesian distance between the true output coordinate
   and the approximate one exceeds the accuracy tolerance. If this
   happens for any test point, we declare the Mapping non-linear and
   give up. */
               if ( sqrt( err ) > tol ) {
                  linear = 0;
                  break;
               }
            }
         }

/* Annul the pointers to the PointSets used above. */
         pset_out_t = astAnnul( pset_out_t );
      }
      pset_in_t = astAnnul( pset_in_t );
   }

/* If an error occurred, or the Mapping was found to be non-linear,
   then set the coefficients to AST_BAD. Otherwise, set the returned flag
   to indicate that the fit was succesful. */
   if ( !astOK || !linear ) {
      for( ii = 0; ii < nc; ii++ ) fit[ ii ] = AST__BAD;
   } else {
      result = 1;
   }

/* Return the result. */
   return result;
}

static double LocalMaximum( const MapData *mapdata, double acc, double fract,
                            double x[] ) {
/*
*  Name:
*     LocalMaximum

*  Purpose:
*     Find a local maximum in a Mapping function.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double LocalMaximum( const MapData *mapdata, double acc, double fract,
*                          double x[] );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function finds a local maximum in the Mapping function
*     supplied.  It employs the modified simplex method (as
*     implemented by UphillSimplex), but repeatedly re-starts the
*     simplex algorithm and tests for convergence of successive
*     maxima, so as to further improve robustness on difficult
*     problems.

*  Parameters:
*     mapdata
*        Pointer to a MapData structure describing the Mapping
*        function, its coordinate constraints, etc.
*     acc
*        The required accuracy with which the maximum is to be found.
*     fract
*        A value between 0.0 and 1.0 which determines the initial step
*        length along each coordinate axis. It should be given as a
*        fraction of the difference between the upper and lower
*        constraint values for each axis (as specified in the
*        "mapdata" structure).
*     x
*        Pointer to an array of double containing the coordinates of
*        an initial estimate of the position of the maximum. On exit,
*        this will be updated to contain the best estimate of the
*        maximum's position, as found by this function.

*  Returned Value:
*     The best estimate of the Mapping function's maximum value.

*  Notes:
*     - A value of AST__BAD will be returned, and no useful
*     information about a solution will be produced, if this function
*     is invoked with the global error status set or if it should fail
*     for any reason.
*/

/* Local Constants: */
   const int maxcall = 1500;     /* Maximum number of function evaluations */
   const int maxiter = 5;        /* Maximum number of iterations */

/* Local Variables: */
   double *dx;                   /* Pointer to array of step lengths */
   double err;                   /* Simplex error estimate */
   double maximum;               /* Simplex maximum value */
   double middle;                /* Middle coordinate between bounds */
   double result;                /* Result value to return */
   int coord;                    /* Loop counter for coordinates */
   int done;                     /* Iterations complete? */
   int iter;                     /* Loop counter for iterations */
   int ncall;                    /* Number of function calls (junk) */

/* Initialise. */
   result = AST__BAD;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Further initialise to avoid compiler warnings. */
   err = 0.0;

/* Allocate workspace. */
   dx = astMalloc( sizeof( double ) * (size_t) mapdata->nin );

/* Perform iterations to repeatedly identify a local maximum. */
   for ( iter = 0; astOK && ( iter < maxiter ); iter++ ) {

/* Set up initial step lengths along each coordinate axis, adjusting
   their signs to avoid placing points outside the coordinate
   constraints (i.e. step away from the closer boundary on each
   axis). */
      for ( coord = 0; coord < mapdata->nin; coord++ ) {
         middle = 0.5 * ( mapdata->lbnd[ coord ] + mapdata->ubnd[ coord ] );
         dx[ coord ] = fract * ( mapdata->ubnd[ coord ] -
                                 mapdata->lbnd[ coord ] );
         if ( x[ coord ] > middle ) dx[ coord ] = -dx[ coord ];
      }

/* Find an approximation to a local maximum using the simplex method
   and check for errors. */
      maximum = UphillSimplex( mapdata, acc, maxcall, dx, x, &err, &ncall );
      if ( astOK ) {

/* Use this maximum value if no previous maximum has been found. */
         if ( result == AST__BAD ) {
            result = maximum;

/* Otherwise use it only if it improves on the previous maximum. */
         } else if ( maximum >= result ) {

/* We iterate, re-starting the simplex algorithm from its previous
   best position so as to guard against premature false
   convergence. Iterations continue until the improvement in the
   maximum is no greater than the required accuracy (and the simplex
   algorithm itself has converged to the required accuracy). Note when
   iterations should cease. */
            done = ( ( ( maximum - result ) <= acc ) && ( err <= acc ) );

/* Store the best maximum and quit iterating if appropriate. */
            result = maximum;
            if ( done ) break;
         }

/* Otherwise, decrement the initial step size for the next iteration. */
         fract /= 1000.0;
      }
   }
   
/* Free the workspace. */
   dx = astFree( dx );

/* If an error occurred, clear the result value. */
   if ( !astOK ) result = AST__BAD;

/* return the result. */
   return result;
}

static void MapBox( AstMapping *this,
                    const double lbnd_in[], const double ubnd_in[],
                    int forward, int coord_out,
                    double *lbnd_out, double *ubnd_out,
                    double xl[], double xu[] ) {
/*
*+
*  Name:
*     astMapBox

*  Purpose:
*     Find a bounding box for a Mapping.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     void astMapBox( AstMapping *this,
*                     const double lbnd_in[], const double ubnd_in[],
*                     int forward, int coord_out,
*                     double *lbnd_out, double *ubnd_out,
*                     double xl[], double xu[] );

*  Class Membership:
*     Mapping method.

*  Description:
*     This function allows you to find the "bounding box" which just
*     encloses another box after it has been transformed by a Mapping
*     (using either its forward or inverse transformation). A typical
*     use might be to calculate the size which an image would have
*     after being transformed by the Mapping.
*
*     The function works on one dimension at a time. When supplied
*     with the lower and upper bounds of a rectangular region (box) of
*     input coordinate space, it finds the lowest and highest values
*     taken by a nominated output coordinate within that
*     region. Optionally, it also returns the input coordinates where
*     these bounding values are attained. It should be used repeatedly
*     if the extent of the bounding box is required in more than one
*     dimension.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     lbnd_in
*        Pointer to an array of double, with one element for each
*        Mapping input coordinate. This should contain the lower bound
*        of the input box in each dimension.
*     ubnd_in
*        Pointer to an array of double, with one element for each
*        Mapping input coordinate. This should contain the upper bound
*        of the input box in each dimension.
*
*        Note that it is permissible for the lower bound to exceed the
*        corresponding upper bound, as the values will simply be
*        swapped before use.
*     forward
*        If this value is non-zero, then the Mapping's forward
*        transformation will be used to transform the input
*        box. Otherwise, its inverse transformation will be used.
*
*        (If the inverse transformation is selected, then references
*        to "input" and "output" coordinates in this description
*        should be transposed. For example, the size of the "lbnd_in"
*        and "ubnd_in" arrays should match the number of output
*        coordinates, as given by the Mapping's Nout attribute.)
*     coord_out
*        The (zero-based) index of the output coordinate for which the
*        lower and upper bounds are required.
*     lbnd_out
*        Pointer to a double in which to return the lowest value taken
*        by the nominated output coordinate within the specified
*        region of input coordinate space.
*     ubnd_out
*        Pointer to a double in which to return the highest value
*        taken by the nominated output coordinate within the specified
*        region of input coordinate space.
*     xl
*        An optional pointer to an array of double, with one element
*        for each Mapping input coordinate. If given, this array will
*        be filled with the coordinates of an input point (although
*        not necessarily a unique one) for which the nominated output
*        coordinate takes the lower bound value returned in
*        "*lbnd_out".
*
*        If these coordinates are not required, a NULL pointer may be
*        supplied.
*     xu
*        An optional pointer to an array of double, with one element
*        for each Mapping input coordinate. If given, this array will
*        be filled with the coordinates of an input point (although
*        not necessarily a unique one) for which the nominated output
*        coordinate takes the upper bound value returned in
*        "*ubnd_out".
*
*        If these coordinates are not required, a NULL pointer may be
*        supplied.

*  Notes:
*     - Any input points which are transformed by the Mapping to give
*     output coordinates containing the value AST__BAD are regarded as
*     invalid and are ignored, They will make no contribution to
*     determining the output bounds, even although the nominated
*     output coordinate might still have a valid value at such points.
*     - An error will occur if the required output bounds cannot be
*     found. Typically, this might occur if all the input points which
*     the function considers turn out to be invalid (see above). The
*     number of points considered before generating such an error is
*     quite large, however, so this is unlikely to occur by accident
*     unless valid points are restricted to a very small subset of the
*     input coordinate space.
*     - The values returned via "lbnd_out", "ubnd_out", "xl" and "xu"
*     will be set to the value AST__BAD if this function should fail
*     for any reason. Their initial values on entry will not be
*     altered if the function is invoked with the global error status
*     set.
*-

*  Implementation Notes:
*     - This function implements the basic astMapBox method available
*     via the protected interface to the Mapping class. The public
*     interface to this method is provided by the astMapBoxId_
*     function.
*/

/* Local Variables: */
   MapData mapdata;              /* Structure to describe Mapping function */
   double *x_l;                  /* Pointer to coordinate workspace */
   double *x_u;                  /* Pointer to coordinate workspace */
   double lbnd;                  /* Required lower bound */
   double ubnd;                  /* Required upper bound */
   int coord;                    /* Loop counter for coordinates. */
   int nin;                      /* Effective number of input coordinates */
   int nout;                     /* Effective number of output coordinates */
   int refine;                   /* Can bounds be refined? */

/* Check the global error status. */
   if ( !astOK ) return;

/* Initialisation to avoid compiler warnings. */
   lbnd = AST__BAD;
   ubnd = AST__BAD;

/* Obtain the effective numbers of input and output coordinates for
   the Mapping, taking account of which transformation is to be
   used. */
   nin = forward ? astGetNin( this ) : astGetNout( this );
   nout = forward ? astGetNout( this ) : astGetNin( this );

/* Check that the output coordinate index supplied is valid and report
   an error if it is not. Use public (one-based) coordinate numbering
   in the error message. */
   if ( astOK ) {
      if ( ( coord_out < 0 ) || ( coord_out >= nout ) ) {
         astError( AST__BADCI, "astMapBox(%s): Output coordinate index (%d) "
                   "invalid - it should be in the range 1 to %d.",
                   astGetClass( this ), coord_out + 1, nout );
      }
   }

/* Initialise a MapData structure to describe the Mapping function
   whose limits are to be found.  Since it may be evaluated many
   times, we attempt to simplify the Mapping supplied. */
   if ( astOK ) {
      mapdata.mapping = astSimplify( this );

/* Store the number of input/output coordinates and the index of the
   output coordinate in which we are interested. */
      mapdata.nin = nin;
      mapdata.nout = nout;
      mapdata.coord = coord_out;

/* Note which Mapping transformation is being used. */
      mapdata.forward = forward;

/* Store pointers to arrays which will contain the input coordinate
   bounds. */
      mapdata.lbnd = astMalloc( sizeof( double ) * (size_t) nin );
      mapdata.ubnd = astMalloc( sizeof( double ) * (size_t) nin );

/* Create PointSets for passing coordinate data to and from the
   Mapping. */
      mapdata.pset_in = astPointSet( 1, nin, "" );
      mapdata.pset_out = astPointSet( 1, nout, "" );

/* Obtain pointers to these PointSets' coordinate arrays. */
      mapdata.ptr_in = astGetPoints( mapdata.pset_in );
      mapdata.ptr_out = astGetPoints( mapdata.pset_out );

/* Allocate workspace for the returned input coordinates. */
      x_l = astMalloc( sizeof( double ) * (size_t) nin );
      x_u = astMalloc( sizeof( double ) * (size_t) nin );
      if ( astOK ) {

/* Initialise the output bounds and corresponding input coordinates to
   "unknown". */
         for ( coord = 0; coord < nin; coord++ ) {
            x_l[ coord ] = AST__BAD;
            x_u[ coord ] = AST__BAD;

/* Initialise the input bounds, ensuring they are the correct way
   around (if not already supplied this way). */
            mapdata.lbnd[ coord ] = ( lbnd_in[ coord ] < ubnd_in[ coord ] ) ?
                                      lbnd_in[ coord ] : ubnd_in[ coord ];
            mapdata.ubnd[ coord ] = ( ubnd_in[ coord ] > lbnd_in[ coord ] ) ?
                                      ubnd_in[ coord ] : lbnd_in[ coord ];
         }

/* First examine a set of special input points to obtain an initial
   estimate of the required output bounds. Do this only so long as the
   number of points involved is not excessive. */
         if ( nin <= 12 ) {
            refine = SpecialBounds( &mapdata, &lbnd, &ubnd, x_l, x_u );
         } else {
            refine = 1;
         }

/* Then attempt to refine this estimate using a global search
   algorithm. */
         if( refine ) GlobalBounds( &mapdata, &lbnd, &ubnd, x_l, x_u );

/* If an error occurred, generate a contextual error message. */
         if ( !astOK ) {
            astError( astStatus, "Unable to find a bounding box for a %s.",
                      astGetClass( this ) );
         }
      }

/* Return the output bounds and, if required, the input coordinate
   values which correspond with them. */
      if ( astOK ) {
         *lbnd_out = lbnd;
         *ubnd_out = ubnd;
         for ( coord = 0; coord < nin; coord++ ) {
            if ( xl ) xl[ coord ] = x_l[ coord ];
            if ( xu ) xu[ coord ] = x_u[ coord ];
         }
      }

/* Annul the simplified Mapping pointer and the temporary
   PointSets. Also free the workspace. */
      mapdata.mapping = astAnnul( mapdata.mapping );
      mapdata.lbnd = astFree( mapdata.lbnd );
      mapdata.ubnd = astFree( mapdata.ubnd );
      mapdata.pset_in = astAnnul( mapdata.pset_in );
      mapdata.pset_out = astAnnul( mapdata.pset_out );
      x_l = astFree( x_l );
      x_u = astFree( x_u );
   }
      
/* If an error occurred, then return bad bounds values and
   coordinates. */
   if ( !astOK ) {
      *lbnd_out = AST__BAD;
      *ubnd_out = AST__BAD;
      for ( coord = 0; coord < nin; coord++ ) {
         if ( xl ) xl[ coord ] = AST__BAD;
         if ( xu ) xu[ coord ] = AST__BAD;
      }
   }
}

static double MapFunction( const MapData *mapdata, const double in[],
                           int *ncall ) {
/*
*  Name:
*     MapFunction

*  Purpose:
*     Return the value of a selected transformed coordinate.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double MapFunction( const MapData *mapdata, const double in[],
*                         int *ncall );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function takes a set of input coordinates and applies a
*     Mapping's coordinate transformation to them. It then returns the
*     value of one of the transformed coordinates.
*
*     It is provided for use by optimisation functions (e.g. those
*     used for finding bounding boxes). The Mapping to be used and
*     associated parameters (such as constraints on the range of input
*     coordinates and the index of the output coordinate to be
*     returned) are supplied in a MapData structure. The value
*     returned will be negated if the "negate" component of this
*     structure is non-zero.
*
*     The value AST__BAD will be returned by this function if the
*     input coordinates lie outside the constrained range given in
*     the MapData structure, or if any of the transformed output
*     coordinates is bad.

*  Parameters:
*     mapdata
*        Pointer to a MapData structure which describes the Mapping to
*        be used.
*     in
*        A double array containing the input coordinates of a single point.
*     ncall
*        Pointer to an int containing a count of the number of times
*        the Mapping's coordinate transformation has been used. This
*        value will be updated to reflect any use made by this
*        function. Normally, this means incrementing the value by 1,
*        but this will be omitted if the input coordinates supplied
*        are outside the constrained range so that no transformation
*        is performed.

*  Returned Value:
*     The selected output coordinate value, or AST__BAD, as appropriate.

*  Notes:
*     - A value of AST__BAD will be returned if this function is
*     invoked with the global error status set, or if it should fail
*     for any reason.
*/

/* Local Variables: */
   double result;                /* Result to be returned */
   int bad;                      /* Output coordinates invalid? */
   int coord_in;                 /* Loop counter for input coordinates */
   int coord_out;                /* Loop counter for output coordinates */
   int outside;                  /* Input point outside bounds? */

/* Initialise. */
   result = AST__BAD;

/* Check the global error status. */
   if ( !astOK ) return result;

/* See if the input point lies outside the required bounds. */
   outside = 0;
   for ( coord_in = 0; coord_in < mapdata->nin; coord_in++ ) {
      if ( ( in[ coord_in ] < mapdata->lbnd[ coord_in ] ) ||
           ( in[ coord_in ] > mapdata->ubnd[ coord_in ] ) ) {
         outside = 1;
         break;
      }

/* Also store the input coordinates in the memory associated with the
   Mapping's input PointSet. */
      mapdata->ptr_in[ coord_in ][ 0 ] = in[ coord_in ];
   }

/* If the input coordinates are within bounds, transform them, using the
   PointSets identified in the "mapdata" structure. */
   if ( !outside ) {
      (void) astTransform( mapdata->mapping, mapdata->pset_in,
                           mapdata->forward, mapdata->pset_out );

/* Increment the number of calls to astTransform and check the error
   status. */
      ( *ncall )++;
      if ( astOK ) {

/* If OK, test if any of the output coordinates is bad. */
         bad = 0;
         for ( coord_out = 0; coord_out < mapdata->nout; coord_out++ ) {
            if ( mapdata->ptr_out[ coord_out ][ 0 ] == AST__BAD ) {
               bad = 1;
               break;
            }
         }

/* If not, then extract the required output coordinate, negating it if
   necessary. */
         if ( !bad ) {
            result = mapdata->ptr_out[ mapdata->coord ][ 0 ];
            if ( mapdata->negate ) result = -result;
         }
      }
   }

/* Return the result. */
   return result;
}

static int MapList( AstMapping *this, int series, int invert, int *nmap,
                     AstMapping ***map_list, int **invert_list ) {
/*
*+
*  Name:
*     astMapList

*  Purpose:
*     Decompose a Mapping into a sequence of simpler Mappings.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int astMapList( AstMapping *this, int series, int invert, int *nmap,
*                      AstMapping ***map_list, int **invert_list )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function decomposes a Mapping (which, in derived classes,
*     may be a compound Mapping) into a sequence of simpler Mappings
*     which may be applied in sequence to achieve the same effect. The
*     Mapping is decomposed as far as possible, but it is not
*     guaranteed that this will necessarily yield any more than one
*     Mapping, which may actually be the original one supplied.
*
*     This function is provided to support both the simplification of
*     compound Mappings, and the analysis of Mapping structure so that
*     particular forms can be recognised.

*  Parameters:
*     this
*        Pointer to the Mapping to be decomposed (the Mapping is not
*        actually modified by this function).
*     series
*        If this value is non-zero, an attempt will be made to
*        decompose the Mapping into a sequence of equivalent Mappings
*        which can be applied in series (i.e. one after the other). If
*        it is zero, the decomposition will instead yield Mappings
*        which can be applied in parallel (i.e. on successive sub-sets
*        of the input/output coordinates).
*     invert
*        The value to which the Mapping's Invert attribute is to be
*        (notionally) set before performing the
*        decomposition. Normally, the value supplied here will be the
*        actual Invert value obtained from the Mapping (e.g. using
*        astGetInvert).  Sometimes, however, when a Mapping is
*        encapsulated within another structure, that structure may
*        retain an Invert value (in order to prevent external
*        interference) which should be used instead.
*
*        Note that the actual Invert value of the Mapping supplied is
*        not used (or modified) by this function.
*     nmap
*        The address of an int which holds a count of the number of
*        individual Mappings in the decomposition. On entry, this
*        should count the number of Mappings already in the
*        "*map_list" array (below). On exit, it is updated to include
*        any new Mappings appended by this function.
*     map_list
*        Address of a pointer to an array of Mapping pointers. On
*        entry, this array pointer should either be NULL (if no
*        Mappings have yet been obtained) or should point at a
*        dynamically allocated array containing Mapping pointers
*        ("*nmap" in number) which have been obtained from a previous
*        invocation of this function.
*
*        On exit, the dynamic array will be enlarged to contain any
*        new Mapping pointers that result from the decomposition
*        requested. These pointers will be appended to any previously
*        present, and the array pointer will be updated as necessary
*        to refer to the enlarged array (any space released by the
*        original array will be freed automatically).
*
*        The new Mapping pointers returned will identify a sequence of
*        Mappings which, when applied in order, will perform a forward
*        transformation equivalent to that of the original Mapping
*        (after its Invert flag has first been set to the value
*        requested above). The Mappings should be applied in series or
*        in parallel according to the type of decomposition requested.
*
*        All the Mapping pointers returned by this function should be
*        annulled by the caller, using astAnnul, when no longer
*        required. The dynamic array holding these pointers should
*        also be freed, using astFree.
*     invert_list
*        Address of a pointer to an array of int. On entry, this array
*        pointer should either be NULL (if no Mappings have yet been
*        obtained) or should point at a dynamically allocated array
*        containing Invert attribute values ("*nmap" in number) which
*        have been obtained from a previous invocation of this
*        function.
*
*        On exit, the dynamic array will be enlarged to contain any
*        new Invert attribute values that result from the
*        decomposition requested. These values will be appended to any
*        previously present, and the array pointer will be updated as
*        necessary to refer to the enlarged array (any space released
*        by the original array will be freed automatically).
*
*        The new Invert values returned identify the values which must
*        be assigned to the Invert attributes of the corresponding
*        Mappings (whose pointers are in the "*map_list" array) before
*        they are applied. Note that these values may differ from the
*        actual Invert attribute values of these Mappings, which are
*        not relevant.
*
*        The dynamic array holding these values should be freed by the
*        caller, using astFree, when no longer required.

*  Returned Value:
*     A non-zero value is returned if the supplied Mapping contained any
*     inverted CmpMaps.

*  Notes:
*     - It is unspecified to what extent the original Mapping and the
*     individual (decomposed) Mappings are
*     inter-dependent. Consequently, the individual Mappings cannot be
*     modified without risking modification of the original.
*     - If this function is invoked with the global error status set,
*     or if it should fail for any reason, then the *nmap value, the
*     list of Mapping pointers and the list of Invert values will all
*     be returned unchanged.
*- 
*/

/* Check the global error status. */
   if ( !astOK ) return 0;

/* Since we are dealing with a basic Mapping, only one new Mapping
   pointer will be returned. Extend the dynamic arrays to accommodate
   this Mapping. */
   *map_list = astGrow( *map_list, *nmap + 1, sizeof( AstMapping * ) );
   *invert_list = astGrow( *invert_list, *nmap + 1, sizeof( int ) );
   if ( astOK ) {

/* Return the invert flag value for the Mapping and a clone of the
   Mapping pointer. */
      ( *invert_list )[ *nmap ] = ( invert != 0 );
      ( *map_list )[ *nmap ] = astClone( this );

/* If OK, return the new Mapping count. */
      if ( astOK ) ( *nmap )++;
   }

   return 0;
}

static int MapMerge( AstMapping *this, int where, int series, int *nmap,
                     AstMapping ***map_list, int **invert_list ) {
/*
*+
*  Name:
*     astMapMerge

*  Purpose:
*     Simplify a sequence of Mappings.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int astMapMerge( AstMapping *this, int where, int series, int *nmap,
*                      AstMapping ***map_list, int **invert_list )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function attempts to simplify a sequence of Mappings by
*     merging a nominated Mapping in the sequence with its neighbours,
*     so as to shorten the sequence if possible.
*
*     In many cases, simplification will not be possible and the
*     function will return -1 to indicate this, without further
*     action.
*
*     In most cases of interest, however, this function will either
*     attempt to replace the nominated Mapping with one which it
*     considers simpler, or to merge it with the Mappings which
*     immediately precede it or follow it in the sequence (both will
*     normally be considered). This is sufficient to ensure the
*     eventual simplification of most Mapping sequences by repeated
*     application of this function.
*
*     In some cases, the function may attempt more elaborate
*     simplification, involving any number of other Mappings in the
*     sequence. It is not restricted in the type or scope of
*     simplification it may perform, but will normally only attempt
*     elaborate simplification in cases where a more straightforward
*     approach is not adequate.

*  Parameters:
*     this
*        Pointer to the nominated Mapping which is to be merged with
*        its neighbours. This should be a cloned copy of the Mapping
*        pointer contained in the array element "(*map_list)[where]"
*        (see below). This pointer will not be annulled, and the
*        Mapping it identifies will not be modified by this function.
*     where
*        Index in the "*map_list" array (below) at which the pointer
*        to the nominated Mapping resides.
*     series
*        A non-zero value indicates that the sequence of Mappings to
*        be simplified will be applied in series (i.e. one after the
*        other), whereas a zero value indicates that they will be
*        applied in parallel (i.e. on successive sub-sets of the
*        input/output coordinates).
*     nmap
*        Address of an int which counts the number of Mappings in the
*        sequence. On entry this should be set to the initial number
*        of Mappings. On exit it will be updated to record the number
*        of Mappings remaining after simplification.
*     map_list
*        Address of a pointer to a dynamically allocated array of
*        Mapping pointers (produced, for example, by the astMapList
*        method) which identifies the sequence of Mappings. On entry,
*        the initial sequence of Mappings to be simplified should be
*        supplied.
*
*        On exit, the contents of this array will be modified to
*        reflect any simplification carried out. Any form of
*        simplification may be performed. This may involve any of: (a)
*        removing Mappings by annulling any of the pointers supplied,
*        (b) replacing them with pointers to new Mappings, (c)
*        inserting additional Mappings and (d) changing their order.
*
*        The intention is to reduce the number of Mappings in the
*        sequence, if possible, and any reduction will be reflected in
*        the value of "*nmap" returned. However, simplifications which
*        do not reduce the length of the sequence (but improve its
*        execution time, for example) may also be performed, and the
*        sequence might conceivably increase in length (but normally
*        only in order to split up a Mapping into pieces that can be
*        more easily merged with their neighbours on subsequent
*        invocations of this function).
*
*        If Mappings are removed from the sequence, any gaps that
*        remain will be closed up, by moving subsequent Mapping
*        pointers along in the array, so that vacated elements occur
*        at the end. If the sequence increases in length, the array
*        will be extended (and its pointer updated) if necessary to
*        accommodate any new elements.
*
*        Note that any (or all) of the Mapping pointers supplied in
*        this array may be annulled by this function, but the Mappings
*        to which they refer are not modified in any way (although
*        they may, of course, be deleted if the annulled pointer is
*        the final one).
*     invert_list
*        Address of a pointer to a dynamically allocated array which,
*        on entry, should contain values to be assigned to the Invert
*        attributes of the Mappings identified in the "*map_list"
*        array before they are applied (this array might have been
*        produced, for example, by the astMapList method). These
*        values will be used by this function instead of the actual
*        Invert attributes of the Mappings supplied, which are
*        ignored.
*
*        On exit, the contents of this array will be updated to
*        correspond with the possibly modified contents of the
*        "*map_list" array.  If the Mapping sequence increases in
*        length, the "*invert_list" array will be extended (and its
*        pointer updated) if necessary to accommodate any new
*        elements.

*  Returned Value:
*     If simplification was possible, the function returns the index
*     in the "map_list" array of the first element which was
*     modified. Otherwise, it returns -1 (and makes no changes to the
*     arrays supplied).

*  Notes:
*     - A value of -1 will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* This is the default method which is inherited by all Mappings which
   do not explicitly provide their own simplification method. Return
   -1 to indicate that no simplification is provided. */
   return -1;
}

static int *MapSplit( AstMapping *this, int nin, int *in, AstMapping **map ){
/*
*+
*  Name:
*     astMapSplit

*  Purpose:
*     Create a Mapping representing a subset of the inputs of an existing
*     Mapping.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     int *astMapSplit( AstMapping *this, int nin, int *in, AstMapping **map )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function creates a new Mapping by picking specified inputs from 
*     an existing Mapping. This is only possible if the specified inputs
*     correspond to some subset of the Mapping outputs. That is, there
*     must exist a subset of the Mapping outputs for which each output
*     depends only on the selected Mapping inputs, and not on any of the
*     inputs which have not been selected. If this condition is not met
*     by the supplied Mapping, then a NULL Mapping is returned.

*  Parameters:
*     this
*        Pointer to the Mapping to be split (the Mapping is not
*        actually modified by this function).
*     nin
*        The number of inputs to pick from "this".
*     in
*        Pointer to an array of indices (zero based) for the inputs which
*        are to be picked. This array should have "nin" elements. If "Nin"
*        is the number of inputs of the supplied Mapping, then each element 
*        should have a value in the range zero to Nin-1.
*     map
*        Address of a location at which to return a pointer to the new
*        Mapping. This Mapping will have "nin" inputs (the number of
*        outputs may be differetn to "nin"). A NULL pointer will be
*        returned if the supplied Mapping has no subset of outputs which 
*        depend only on the selected inputs.

*  Returned Value:
*     A pointer to a dynamically allocated array of ints. The number of
*     elements in this array will equal the number of outputs for the 
*     returned Mapping. Each element will hold the index of the
*     corresponding output in the supplied Mapping. The array should be
*     freed using astFree when no longer needed. A NULL pointer will
*     be returned if no output Mapping can be created.

*  Notes:
*     - If this function is invoked with the global error status set,
*     or if it should fail for any reason, then NULL values will be
*     returned as the function value and for the "map" pointer.
*- 

*  Implementation Notes:
*     - This function implements the basic astMapSplit method available
*     via the protected interface to the Mapping class. The public
*     interface to this method is provided by the astMapSplitId_
*     function.
*/

/* Local Variables: */
   AstCmpMap *rmap;           /* Unsimplified result mapping */
   AstPermMap *pm;            /* PermMap which rearranges the inputs */
   int *outperm;              /* PermMap output axis permutation array */   
   int *result;               /* Pointer to returned array */
   int iin;                   /* Input index */
   int iout;                  /* Output index */
   int mapnin;                /* Number of Mapping inputs */
   int nout;                  /* No of outputs */
   int ok;                    /* Can the supplied "in" array be used? */
   int perm;                  /* Are the inputs permuted? */

/* Initialise */
   result = NULL;
   *map = NULL;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Verify the input axis indices.*/
   mapnin = astGetNin( this );
   for( iin = 0; iin < nin; iin++ ){
      if( in[ iin ] < 0 || in[ iin ] >= mapnin ) {
         astError( AST__AXIIN, "astMapSplit(%s): One of the supplied Mapping "
                   "input indices has value %d which is invalid; it should "
                   "be in the range 1 to %d.", astGetClass( this ), 
                   in[ iin ] + 1, mapnin );
         break;
      }
   }

/* Since we are dealing with a basic Mapping, we can only create the
   required output Mapping if all inputs are being selected. */
   if( nin == mapnin ) {

/* The inputs may have been selected in a different order to that in
   which they occur in the supplied Mapping. We therefore create a
   PermMap which rearranges the inputs into the order they have in the
   supplied Mapping. The supplied "in" array can act as the PermMap's
   "inperm" array. Allocate memory for the "outperm" array. */
      outperm = astMalloc( sizeof(int)*(size_t) nin );
      if( astOK ) {

/* Store the input index for each output in the outperm array and check that 
   each input has been selected once and only once. Also set a flag
   indicating if a PermMap is needed. */
         perm = 0;
         ok = 1;
         for( iout = 0; iout < nin; iout++ ) outperm[ iout ] = -1;
         for( iin = 0; iin < nin; iin++ ) {
            iout = in[ iin ];
            if( outperm[ iout ] != -1 ) {
               ok = 0;
               break;
            } else {
               outperm[ iout ] = iin;
            }
         }
         for( iout = 0; iout < nin; iout++ ) {
            if( outperm[ iout ] == -1 ) {
               ok = 0;
               break;
            } else if( outperm[ iout ] != iout ) {
               perm = 1;
            }
         }
         if( ok ) {

/* Allocate the array to hold the returned output indices. */
            nout = astGetNout( this );
            result = astMalloc( sizeof(int)*(size_t) nout );
            if( astOK ) {

/* The outputs are copied from the supplied Mapping. */
               for( iout = 0; iout < nout; iout++ ) result[ iout ] = iout;

/* If the inputs are to be permuted, create the PermMap. */
               if( perm ) {
                  pm = astPermMap( nin, in, nin, outperm, NULL, "" );

/* The returned Mapping is a series CmpMap containing this PermMap
   followed by the supplied Mapping. */
                  rmap = astCmpMap( pm, this, 1, "" );
                  *map = astSimplify( rmap );
                  rmap = astAnnul( rmap );

/* Annul the PermMap pointer. */
                  pm = astAnnul( pm );

/* If no input permutation is needed, the resturned Mapping is just the
   supplied Mapping. */
               } else {
                  *map = astClone( this );
               }
            }
         }

/* Free resources. */
         outperm = astFree( outperm );
      }
   }

/* Free resources if an error has occurred. */
   if( !astOK ) {
      result = astFree( result );
      *map = astAnnul( *map );
   }

/* Return the list of output indices. */
   return result;
}

static double MatrixDet( int ndim, const double *matrix ){
/*
*  Name:
*     MatrixDet

*  Purpose:
*     Return the determinant of a square matrix.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double MatrixDet( int ndim, const double *matrix )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function returns the determinant of the supplied square matrix.

*  Parameters:
*     ndim
*        The number of rows and columns in the matrix.
*     matrix
*        The matrix element values. The first row of "ndim" elements
*        should be supplied first, followed by the second row, etc.

*  Returned Value:
*     The determinant.
*/

/* Local Variables: */
   double result;
   double *a;
   double *y;
   int *iw;
   int i;
   int jf;

/* Initialise */
   result = AST__BAD;

/* Check the global error status. */
   if ( !astOK ) return result;

   if( ndim == 1 ) {
      result = matrix[ 0 ];

   } else if( ndim == 2 ) {
      result = matrix[ 0 ]*matrix[ 3 ] - matrix[ 1 ]*matrix[ 2 ];

   } else {
      a = astStore( NULL, matrix, sizeof( double )*(size_t) (ndim*ndim) );
      iw = astMalloc( sizeof( int )*(size_t) ndim );
      y = astMalloc( sizeof( double )*(size_t) ndim );
      if( y ) {
         for( i = 0; i < ndim; i++ ) y[ i ] = 1.0;
         palSlaDmat( ndim, a, y, &result, &jf, iw );
      }
      y = astFree( y );
      iw = astFree( iw );
      a = astFree( a );
   }

   return result;
}

static double MaxD( double a, double b ) {
/*
*  Name:
*     MaxD

*  Purpose:
*     Return the maximum of two double values.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double MaxD( double a, double b )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function returns the maximum of two double values.

*  Parameters:
*     a
*        The first value.
*     b
*        The second value.

*  Returned Value:
*     The maximum.
*/

/* Return the larger value. */
   return ( a > b ) ? a : b;
}

static int MaxI( int a, int b ) {
/*
*  Name:
*     MaxI

*  Purpose:
*     Return the maximum of two integer values.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int MaxI( int a, int b )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function returns the maximum of two integer values.

*  Parameters:
*     a
*        The first value.
*     b
*        The second value.

*  Returned Value:
*     The maximum.
*/

/* Return the larger value. */
   return ( a > b ) ? a : b;
}

static int MinI( int a, int b ) {
/*
*  Name:
*     MinI

*  Purpose:
*     Return the minimum of two integer values.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int MinI( int a, int b )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function returns the minimum of two integer values.

*  Parameters:
*     a
*        The first value.
*     b
*        The second value.

*  Returned Value:
*     The minimum.
*/

/* Return the smaller value. */
   return ( a < b ) ? a : b;
}

static double NewVertex( const MapData *mapdata, int lo, double scale,
                         double x[], double f[], int *ncall, double xnew[] ) {
/*
*  Name:
*     NewVertex

*  Purpose:
*     Locate a new vertex for a simplex.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double NewVertex( const MapData *mapdata, int lo, double scale,
*                       double x[], double f[], int *ncall, double xnew[] );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function is provided for use during optimisation of a
*     Mapping function using the simplex method. It generates the
*     coordinates of a new simplex vertex and evaluates the Mapping
*     function at that point.  If the function's value is better then
*     (i.e. larger than) the value at the previously worst vertex,
*     then it is used to replace that vertex.

*  Parameters:
*     mapdata
*        Pointer to a MapData structure which describes the Mapping
*        function to be used.
*     lo
*        The (zero-based) index of the simplex vertex which initially
*        has the worst (lowest) value.
*     scale
*        The scale factor to be used to generate the new vertex. The
*        distance of the worst vertex from the centre of the face
*        opposite it is scaled by this factor to give the new vertex
*        position. Negative factors result in reflection through this
*        opposite face.
*     x
*        An array of double containing the coordinates of the vertices
*        of the simplex. The coordinates of the first vertex are
*        stored first, then those of the second vertex, etc. This
*        array will be updated by this function if the new vertex is
*        used to replace an existing one.
*     f
*        An array of double containing the Mapping function values at
*        each vertex of the simplex. This array will be updated by
*        this function if the new vertex is used to replace an
*        existing one.
*     ncall
*        Pointer to an int containing a count of the number of times
*        the Mapping function has been invoked. This value will be
*        updated to reflect the actions of this function.
*     xnew
*        An array of double with one element for each input coordinate
*        of the Mapping function. This is used as workspace.

*  Returned Value:
*     The Mapping function value at the new vertex. This value is
*     returned whether or not the new vertex replaces an existing one.

*  Notes:
*     - A value of AST__BAD will be returned by this function if it is
*     invoked with the global error status set, or if it should fail
*     for any reason.
*     - A value of AST__BAD will also be returned if the new vertex
*     lies outside the constrained range of input coordinates
*     associated with the Mapping function (as specified in the
*     MapData structure supplied) or if any of the transformed output
*     coordinates produced by the underlying Mapping is bad. In either
*     case the new vertex will not be used to replace an existing one.
*/

/* Local Variables: */
   double fnew;                  /* Function value at new vertex */
   double xface;                 /* Coordinate of centre of magnification */
   int coord;                    /* Loop counter for coordinates */
   int ncoord;                   /* Number of coordinates */
   int nvertex;                  /* Number of simplex vertices */
   int vertex;                   /* Loop counter for vertices */

/* Initialise. */
   fnew = AST__BAD;

/* Check the global error status. */
   if ( !astOK ) return fnew;
   
/* Obtain the number of Mapping input coordinates from the MapData
   structure and calculate the number of simplex vertices. */
   ncoord = mapdata->nin;
   nvertex = ncoord + 1;

/* Loop to obtain each coordinate of the new vertex. */
   for ( coord = 0; coord < ncoord; coord++ ) {

/* Loop over all vertices except the lowest one and average their
   coordinates. This gives the coordinate of the centre of the face
   opposite the lowest vertex, which will act as the centre of
   magnification. */
      xface = 0.0;
      for ( vertex = 0; vertex < nvertex; vertex++ ) {
         if ( vertex != lo ) {

/* Divide each coordinate by the number of vertices as the sum is
   accumulated in order to minimise the risk of overflow. */
            xface += x[ vertex * ncoord + coord ] /
                     ( (double ) ( nvertex - 1 ) );
         }
      }

/* Magnify the lowest vertex's distance from this point by the
   required factor to give the coordinates of the new vertex. */
      xnew[ coord ] = xface + ( x[ lo * ncoord + coord ] - xface ) * scale;
   }

/* Evaluate the Mapping function at the new vertex. */
   fnew = MapFunction( mapdata, xnew, ncall );
 
/* If the result is not bad and exceeds the previous value at the
   lowest vertex, then replace the lowest vertex with this new one. */
   if ( astOK && ( fnew != AST__BAD ) && ( fnew > f[ lo ] ) ) {
      for ( coord = 0; coord < ncoord; coord++ ) {
         x[ lo * ncoord + coord ] = xnew[ coord ];
      }
      f[ lo ] = fnew;
   }

/* Return the value at the new vertex. */
   return fnew;
}

static double Random( long int *seed ) {
/*
*  Name:
*     Random

*  Purpose:
*     Return a pseudo-random value in the range 0 to 1.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double Random( long int *seed );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function returns a pseudo-random double value from a PDF
*     uniformly distributed in the range 0 to 1. It also updates a
*     seed value so that a sequence of pseudo-random values may be
*     obtained with successive invocations.

*  Parameters:
*     seed
*        Pointer to a long int which should initially contain a
*        non-zero seed value. This will be updated with a new seed
*        which may be supplied on the next invocation in order to
*        obtain a different pseudo-random value.

*  Returned Value:
*     The pseudo-random value.
*/

/* Local Variables: */
   long int i;                   /* Temporary storage */

/* This a basic random number generator using constants given in
   Numerical Recipes (Press et al.). */
   i = *seed / 127773;
   *seed = ( *seed - i * 127773 ) * 16807 - i * 2836;
   if ( *seed < 0 ) *seed += 2147483647;

/* Return the result as a double value in the range 0 to 1. */
   return ( (double) ( *seed - 1 ) ) / (double) 2147483646;
}

static double Rate( AstMapping *this, double *at, int ax1, int ax2 ){
/*
*+
*  Name:
*     astRate

*  Purpose:
*     Calculate the rate of change of a Mapping output.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     result = astRate( AstMapping *this, double *at, int ax1, int ax2 )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function evaluates the rate of change of a specified output of 
*     the supplied Mapping with respect to a specified input, at a 
*     specified input position. 
*
*     The result is estimated by interpolating the function using a
*     fourth order polynomial in the neighbourhood of the specified
*     position. The size of the neighbourhood used is chosen to minimise
*     the RMS residual per unit length between the interpolating
*     polynomial and the supplied Mapping function.

*  Parameters:
*     this
*        Pointer to the Mapping to be applied.
*     at
*        The address of an array holding the axis values at the position 
*        at which the rate of change is to be evaluated. The number of 
*        elements in this array should equal the number of inputs to the 
*        Mapping.
*     ax1
*        The index of the Mapping output for which the rate of change is to 
*        be found (output numbering starts at 0 for the first output).
*     ax2
*        The index of the Mapping input which is to be varied in order to
*        find the rate of change (input numbering starts at 0 for the first 
*        input).

*  Returned Value:
*     astRate()
*        The rate of change of Mapping output "ax1" with respect to input 
*        "ax2", evaluated at "at", or AST__BAD if the value cannot be 
*        calculated.

*  Notes:
*     - A value of AST__BAD will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-

*  Implementation Notes:
*     - This function implements the basic astRate method available
*     via the protected interface to the Mapping class. The public
*     interface to this method is provided by the astRateId_
*     function.
*/

/* Local Variables: */
#define MXY 100
   double x0, h, s1, s2, sp, r, dh, ed2, ret, rms, h0, x[MXY], y[MXY];
   int nin, nout, i, ixy, fitted, fitok;
   PN *fit;

/* Initialise */
   ret = AST__BAD;

/* Check the global error status. */
   if ( !astOK ) return ret;

/* Obtain the numbers of input and output coordinates for the Mapping. */
   nin = astGetNin( this );
   nout = astGetNout( this );

/* Validate the output index. */
   if ( astOK && ( ax1 < 0 || ax1 >= nout ) ) {
      astError( AST__AXIIN, "astRate(%s): The supplied Mapping output "
                "index (%d) is invalid; it should be in the range 1 to %d.", 
                astGetClass( this ), ax1 + 1, nout );
   }

/* Validate the input index. */
   if ( astOK && ( ax2 < 0 || ax2 >= nin ) ) {
      astError( AST__AXIIN, "astRate(%s): The supplied Mapping input "
                "index (%d) is invalid; it should be in the range 1 to %d.", 
                astGetClass( this ), ax2 + 1, nin );
   }

/* Check the Mapping has a forward transformation. */
   if ( astOK && !astGetTranForward( this ) ) {
      astError( AST__NODEF, "astRate(%s): The supplied Mapping does not "
                "have a defined forward transformation.", 
                astGetClass( this ) );
   }

/* Save the central value on the Mapping input which is to be varied. */
   x0 = at[ ax2 ];

/* If it is bad, return bad values. */
   if( astOK && x0 != AST__BAD ) {

/* The required derivative is estimated by fitting a polynomial to the
  function over a small range of length "h" centred on the supplied value
  "x0", and then finding the derivative of this fitting polynomial at "x0".
  The bulk of the problem lies in choosing a suitable value for the
  length "h". If "h" is too large, the function is not well fitted by a
  polynomial and so the returned value is inaccurate. If "h" is too small,
  then the returned value is dominated by rounding errors and so is again
  inaccurate. The process tries a series of different values for "h". The
  initial estimate is formed as a fixed small fraction of the supplied 
  "x0", or 1.0 if "x0" is zero. */
      h = ( x0 != 0.0 ) ? DBL_EPSILON*1.0E10*x0 : 1.0;

/* We next find a more reliable estimate based on the probable accuracy
   of the calculated function values and the rate of change of the
   derivative of the function in the region of "x0". Find a polynomial fit 
   to the function over this initial interval. The independant variable 
   of this fit is (x-x0) and the dependant variable is (y(x)-y(x0). */
      fit = FitPN( this, at, ax1, ax2, x0, h, NULL );
      if( !fit ) return AST__BAD;

/* We need an estimate of how much the derivative may typically change
   over this interval. The more the derivative changes, the smaller we
   should make "h" in order to produce a good fit to the polynomial.
   Calculate the rate of change of the polynomial (i.e the derivative
   estimate) at a set of 5 evenly spaced points over the interval "h"
   and then find the standard deviation of the derivative estimates
   divided by the interval size. At the same time form an estimate of 
   the RMS polynomial value over the step ("sp"). */  
      s1 = 0.0;
      s2 = 0.0;
      sp = 0.0;
      dh = h/4.0;
      for( i = -(RATE_ORDER/2); i < (RATE_ORDER+1)/2; i++ ) {
         r = EvaluateDPN( fit, i*dh );
         s1 += r;
         s2 += r*r;
         r = EvaluatePN( fit, i*dh ) + fit->y0;
         sp += r*r;
      }
      s2 /= RATE_ORDER;
      s1 /= RATE_ORDER;
      ed2 = s2 - s1*s1;
      if( ed2 > 0 ) {
         ed2 = sqrt( ed2 )/h;
      } else {
         ed2 = 0.0;
      }
      sp = sqrt( sp/RATE_ORDER );
   
      fit = astFree( fit );

/* If the derivative estimate does not change significantly over the interval, 
   return it. */
      if( ed2 <= 1.0E-10*fabs( s1/h ) ) {
         ret = s1;
 
      } else {

/* Otherwise, we find a better estimate for the step size by assuming a
   fixed relative error in the function value, and a second derivative 
   based on the "ed2" value found above. The total error in the
   derivative estimate is assumed to be of the form:

     (a/h)**2 + (d2*h)**2

   where "a" is the accuracy with which the function can be evaluated 
   (assumed to be 1.0E5*DBL_EPSILON*sp) and f2 is the second derivative. 
   The value of "h" below is the value which minimises the above
   total error expression. */

         h = sqrt( fabs( 1.0E5*DBL_EPSILON*sp/ed2 ) );

/* It turns out that the error in the derivative (i.e. the residual
   between the true derivative value and the derivative of the fitting
   polynomial at the "x0" value), is generally equal to the RMS error
   between the fitting polynomial and the function value, divided by the
   step size. This error is high for very small step sizes because of
   rounding error and is also high for large step sizes becase the function
   is not well fitted by a polynomial. In between there is a minimum
   which corresponds to the optimal step size. It also turns out that the
   error in the derivative is a monotonic increasing function for step
   sizes above the optimal step size. We find the optimal step size by
   working our way down this monotonic function, in powers of ten, until 
   the first increase in error is encountered. 

   Starting at the step size found above, note log10( normalised rms error of
   fit ) at increasing step sizes until the rms error exceeds the 0.2 of
   the rms function value. Each new step size is a factor 10 times the previous 
   step size. Once the step size is so large rgat we cannot fit the
   polynomial, break out of the loop. */
         h0 = h;
         ixy = 0;
         rms = 0.1*sp - 1;
         fitted = 0;
         fitok = 1;
         while( rms < 0.2*sp && ixy < MXY && ( !fitted || fitok ) ) {
            fit = FitPN( this, at, ax1, ax2, x0, h0, &rms );
            if( fit ) {
               fitted = 1;
               fitok = 1;

/* If we come across an exact fit, use it to determine the returned
   values and break. */
               if( rms == 0.0 ) { 
                  ret = fit->coeff[ 1 ];
                  fit = astFree( fit );
                  break;

               } else {
                  if( fit->coeff[ 1 ] != 0.0 ) {
                     y[ ixy ] = log10( rms/( h0*fabs( fit->coeff[ 1 ] ) ) );
                  } else {
                     y[ ixy ] = AST__BAD;
                  }
                  fit = astFree( fit );
               }
            } else {
               fitok = 0;
               y[ ixy ] = AST__BAD;
            }
            x[ ixy ] = ixy;
            ixy++;
            h0 *= 10.0;
         }

/* If we found a step size which gave zero rms error, use it. Otherwise, run 
   down from the largest step size to the smallest looking for the first step 
   size at which the error increases rather than decreasing. */
         if( ret == AST__BAD ) {
            h0 = AST__BAD;
            ixy--;
            while( --ixy > 0 ){
               if( y[ ixy - 1 ] != AST__BAD && 
                   y[ ixy ] != AST__BAD &&
                   y[ ixy + 1 ] != AST__BAD ) {         
                  if( y[ ixy - 1 ] > y[ ixy ] ) {         
                     h0 = x[ ixy ];
         
                     x[ 0 ] = x[ ixy - 1 ];
                     x[ 1 ] = x[ ixy ];
                     x[ 2 ] = x[ ixy + 1 ];
         
                     y[ 0 ] = y[ ixy - 1 ];
                     y[ 1 ] = y[ ixy ];
                     y[ 2 ] = y[ ixy + 1 ];
         
                     break;
                  }
               }
            }

/* If no minimum could be found in the above loop, continue decreasing
   the step size below the value set above until a minimum is found. */
            if( h0 == AST__BAD ) {
               h0 = h;
               ixy = 0;
               while( y[ 0 ] < y[ 1 ] ) {
      
                  h0 *= 0.1;
                  ixy--;
                  fit = FitPN( this, at, ax1, ax2, x0, h0, &rms );
                  if( fit ) {
                     x[ 2 ] = x[ 1 ];
                     x[ 1 ] = x[ 0 ];
                     y[ 2 ] = y[ 1 ];
                     y[ 1 ] = y[ 0 ];

                     if( rms == 0.0 ) { 
                        ret = fit->coeff[ 1 ];
                        fit = astFree( fit );
                        break;

                     } else if( fit->coeff[ 1 ] != 0.0 ) {
                        x[ 0 ] = ixy;
                        y[ 0 ] = log10( rms/( h0*fabs( fit->coeff[ 1 ] ) ) );

                     } else {
                        h0 *= 10.0;
                        x[ 0 ] = AST__BAD;
                        break;
                     }
                     fit = astFree( fit );
                  } else {
                     h0 *= 10.0;
                     x[ 0 ] = AST__BAD;
                     break;
                  }
               }
            }

/* If we have found a error which is lower than either of its
   neighbouring errors, fit a quadratic through the three points and find
   the power of ten which correspnds to the minimum of the function. */
            if( ret == AST__BAD ) {
               if( x[ 0 ] != AST__BAD ) {
                  fit = InterpPN( 3, x, y );
                  if( fit ){
                     if( fit->coeff[ 2 ] > 0.0 ) {
                        h0 = h*pow( 10.0, -0.5*fit->coeff[ 1 ]/fit->coeff[ 2 ] );
                     }
                     fit = astFree( fit );
                  }
               }

/* Use the best estimate of h to calculate the returned derivatives. */
               fit = FitPN( this, at, ax1, ax2, x0, h0, &rms );
               if( fit ) {         
                  ret = fit->coeff[ 1 ];
                  fit = astFree( fit );
               }
            }
         }
      }
   }

/* Free resources */
   FunPN( NULL, NULL, -2, 0, 0, NULL, NULL );

/* Return the result. */
   return ret;
#undef MXY
}

/*
*++
*  Name:
c     astRebin<X>
f     AST_REBIN<X>

*  Purpose:
*     Rebin a region of a data grid.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astRebin<X>( AstMapping *this, double wlim, int ndim_in,
c                      const int lbnd_in[], const int ubnd_in[],
c                      const <Xtype> in[], const <Xtype> in_var[],
c                      int spread, const double params[], int flags,
c                      double tol, int maxpix,
c                      <Xtype> badval, int ndim_out,
c                      const int lbnd_out[], const int ubnd_out[],
c                      const int lbnd[], const int ubnd[],
c                      <Xtype> out[], <Xtype> out_var[] );
f     CALL AST_REBIN<X>( THIS, WLIM, NDIM_IN, LBND_IN, UBND_IN, IN, IN_VAR,
f                        SPREAD, PARAMS, FLAGS,
f                        TOL, MAXPIX, BADVAL,
f                        NDIM_OUT, LBND_OUT, UBND_OUT,
f                        LBND, UBND, OUT, OUT_VAR, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
*     This is a set of functions for rebinning gridded data (e.g. an
*     image) under the control of a geometrical transformation, which
*     is specified by a Mapping.  The functions operate on a pair of
*     data grids (input and output), each of which may have any number
*     of dimensions. Rebinning may be restricted to a specified
*     region of the input grid. An associated grid of error estimates
*     associated with the input data may also be supplied (in the form
*     of variance values), so as to produce error estimates for the
*     rebined output data. Propagation of missing data (bad pixels)
*     is supported.
*
*     Note, if you will be rebining a sequence of input arrays and then 
*     co-adding them into a single array, the alternative 
c     astRebinSeq<X> functions
f     AST_REBINSEQ<X> routines
*     will in general be more efficient.
*
*     You should use a rebinning function which matches the numerical
*     type of the data you are processing by replacing <X> in
c     the generic function name astRebin<X> by an appropriate 1- or
f     the generic function name AST_REBIN<X> by an appropriate 1- or
*     2-character type code. For example, if you are rebinning data
c     with type "float", you should use the function astRebinF (see
f     with type REAL, you should use the function AST_REBINR (see
*     the "Data Type Codes" section below for the codes appropriate to
*     other numerical types).
*
*     Rebinning of the grid of input data is performed by transforming
*     the coordinates of the centre of each input grid element (or pixel)
*     into the coordinate system of the output grid. The input pixel
*     value is then divided up and assigned to the output pixels in the
*     neighbourhood of the central output coordinates. A choice of
*     schemes are provided for determining how each input pixel value is
*     divided up between the output pixels. In general, each output pixel 
*     may be assigned values from more than one input pixel. All 
*     contributions to a given output pixel are summed to produce the
*     final output pixel value. Output pixels can be set to the supplied
*     bad value if they receive contributions from an insufficient number
*     of input pixels. This is controlled by the 
c     "wlim" parameter.
f     WLIM argument.
*
*     Input pixel coordinates are transformed into the coordinate
*     system of the output grid using the forward transformation of the
*     Mapping which is supplied. This means that geometrical features
*     in the input data are subjected to the Mapping's forward
*     transformation as they are transferred from the input to the
*     output grid.
*
*     In practice, transforming the coordinates of every pixel of a
*     large data grid can be time-consuming, especially if the Mapping
*     involves complicated functions, such as sky projections. To
*     improve performance, it is therefore possible to approximate
*     non-linear Mappings by a set of linear transformations which are
*     applied piece-wise to separate sub-regions of the data. This
*     approximation process is applied automatically by an adaptive
*     algorithm, under control of an accuracy criterion which
*     expresses the maximum tolerable geometrical distortion which may
*     be introduced, as a fraction of a pixel.
*     
*     This algorithm first attempts to approximate the Mapping with a
*     linear transformation applied over the whole region of the
*     input grid which is being used. If this proves to be
*     insufficiently accurate, the input region is sub-divided into
*     two along its largest dimension and the process is repeated
*     within each of the resulting sub-regions. This process of
*     sub-division continues until a sufficiently good linear
*     approximation is found, or the region to which it is being
*     applied becomes too small (in which case the original Mapping is
*     used directly).

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to a Mapping, whose forward transformation will be
*        used to transform the coordinates of pixels in the input
*        grid into the coordinate system of the output grid. 
*
*        The number of input coordinates used by this Mapping (as
*        given by its Nin attribute) should match the number of input
c        grid dimensions given by the value of "ndim_in"
f        grid dimensions given by the value of NDIM_IN
*        below. Similarly, the number of output coordinates (Nout
*        attribute) should match the number of output grid dimensions
c        given by "ndim_out".
f        given by NDIM_OUT.
c     wlim
f     WLIM = DOUBLE PRECISION (Given)
*        Gives the required number of input pixel values which must contribute
*        to an output pixel in order for the output pixel value to be
*        considered valid. If the sum of the input pixel weights contributing 
*        to an output pixel is less than the supplied
c        "wlim"
f        WLIM
*        value, then the output pixel value is returned set to the
*        supplied bad value.
c     ndim_in
f     NDIM_IN = INTEGER (Given)
*        The number of dimensions in the input grid. This should be at
*        least one.
c     lbnd_in
f     LBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the input grid along each dimension.
c     ubnd_in
f     UBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the input grid along each dimension.
*
c        Note that "lbnd_in" and "ubnd_in" together define the shape
f        Note that LBND_IN and UBND_IN together define the shape
*        and size of the input grid, its extent along a particular
c        (j'th) dimension being ubnd_in[j]-lbnd_in[j]+1 (assuming the
c        index "j" to be zero-based). They also define
f        (J'th) dimension being UBND_IN(J)-LBND_IN(J)+1. They also define
*        the input grid's coordinate system, each pixel having unit
*        extent along each dimension with integral coordinate values
*        at its centre.
c     in
f     IN( * ) = <Xtype> (Given)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        input grid, containing the input data to be rebined.  The
*        numerical type of this array should match the 1- or
*        2-character type code appended to the function name (e.g. if
c        you are using astRebinF, the type of each array element
c        should be "float").
f        you are using AST_REBINR, the type of each array element
f        should be REAL).
*
*        The storage order of data within this array should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
c     in_var
f     IN_VAR( * ) = <Xtype> (Given)
c        An optional pointer to a second array with the same size and
c        type as the "in" array. If given, this should contain a set
c        of non-negative values which represent estimates of the
c        statistical variance associated with each element of the "in"
c        array. If this array is supplied (together with the
c        corresponding "out_var" array), then estimates of the
c        variance of the rebined output data will be calculated.
c
c        If no input variance estimates are being provided, a NULL
c        pointer should be given.
f        An optional second array with the same size and type as the
f        IN array. If the AST__USEVAR flag is set via the FLAGS
f        argument (below), this array should contain a set of
f        non-negative values which represent estimates of the
f        statistical variance associated with each element of the IN
f        array. Estimates of the variance of the rebined output data
f        will then be calculated.
f
f        If the AST__USEVAR flag is not set, no input variance
f        estimates are required and this array will not be used. A
f        dummy (e.g. one-element) array may then be supplied.
c     spread
f     SPREAD = INTEGER (Given)
c        This parameter specifies the scheme to be used for dividing
f        This argument specifies the scheme to be used for dividing
*        each input data value up amongst the corresponding output pixels.
*        It may be used to select
*        from a set of pre-defined schemes by supplying one of the
*        values described in the "Pixel Spreading Schemes"
*        section below.  If a value of zero is supplied, then the
*        default linear spreading scheme is used (equivalent to
*        supplying the value AST__LINEAR).
c     params
f     PARAMS( * ) = DOUBLE PRECISION (Given)
c        An optional pointer to an array of double which should contain
f        An optional array which should contain
*        any additional parameter values required by the pixel
*        spreading scheme. If such parameters are required, this
*        will be noted in the "Pixel Spreading Schemes"
*        section below.
*
c        If no additional parameters are required, this array is not
c        used and a NULL pointer may be given.
f        If no additional parameters are required, this array is not
f        used. A dummy (e.g. one-element) array may then be supplied.
c     flags
f     FLAGS = INTEGER (Given)
c        The bitwise OR of a set of flag values which may be used to
f        The sum of a set of flag values which may be used to
*        provide additional control over the rebinning operation. See
*        the "Control Flags" section below for a description of the
*        options available.  If no flag values are to be set, a value
*        of zero should be given.
c     tol
f     TOL = DOUBLE PRECISION (Given)
*        The maximum tolerable geometrical distortion which may be
*        introduced as a result of approximating non-linear Mappings
*        by a set of piece-wise linear transformations. This should be
*        expressed as a displacement in pixels in the output grid's
*        coordinate system.
*
*        If piece-wise linear approximation is not required, a value
*        of zero may be given. This will ensure that the Mapping is
*        used without any approximation, but may increase execution
*        time.
*        
*        If the value is too high, discontinuities between the linear
*        approximations used in adjacent panel will be higher, and may
*        cause the edges of the panel to be visible when viewing the output 
*        image at high contrast. If this is a problem, reduce the
*        tolerance value used.
c     maxpix
f     MAXPIX = INTEGER (Given)
*        A value which specifies an initial scale size (in pixels) for
*        the adaptive algorithm which approximates non-linear Mappings
*        with piece-wise linear transformations. Normally, this should
*        be a large value (larger than any dimension of the region of
*        the input grid being used). In this case, a first attempt to
*        approximate the Mapping by a linear transformation will be
*        made over the entire input region.
*
*        If a smaller value is used, the input region will first be
c        divided into sub-regions whose size does not exceed "maxpix"
f        divided into sub-regions whose size does not exceed MAXPIX
*        pixels in any dimension. Only at this point will attempts at
*        approximation commence.
*
*        This value may occasionally be useful in preventing false
*        convergence of the adaptive algorithm in cases where the
*        Mapping appears approximately linear on large scales, but has
*        irregularities (e.g. holes) on smaller scales. A value of,
*        say, 50 to 100 pixels can also be employed as a safeguard in
*        general-purpose software, since the effect on performance is
*        minimal.
*
*        If too small a value is given, it will have the effect of
*        inhibiting linear approximation altogether (equivalent to
c        setting "tol" to zero). Although this may degrade
f        setting TOL to zero). Although this may degrade
*        performance, accurate results will still be obtained.
c     badval
f     BADVAL = <Xtype> (Given)
*        This argument should have the same type as the elements of
c        the "in" array. It specifies the value used to flag missing
f        the IN array. It specifies the value used to flag missing
*        data (bad pixels) in the input and output arrays.
*
c        If the AST__USEBAD flag is set via the "flags" parameter,
f        If the AST__USEBAD flag is set via the FLAGS argument,
c        then this value is used to test for bad pixels in the "in"
c        (and "in_var") array(s).
f        then this value is used to test for bad pixels in the IN
f        (and IN_VAR) array(s).
*
*        In all cases, this value is also used to flag any output
c        elements in the "out" (and "out_var") array(s) for which
f        elements in the OUT (and OUT_VAR) array(s) for which
*        rebined values could not be obtained (see the "Propagation
*        of Missing Data" section below for details of the
*        circumstances under which this may occur). 
c     ndim_out
f     NDIM_OUT = INTEGER (Given)
*        The number of dimensions in the output grid. This should be
*        at least one. It need not necessarily be equal to the number
*        of dimensions in the input grid.
c     lbnd_out
f     LBND_OUT( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the output grid along each dimension.
c     ubnd_out
f     UBND_OUT( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the output grid along each dimension.
*
c        Note that "lbnd_out" and "ubnd_out" together define the
f        Note that LBND_OUT and UBND_OUT together define the
*        shape, size and coordinate system of the output grid in the
c        same way as "lbnd_in" and "ubnd_in" define the shape, size
f        same way as LBND_IN and UBND_IN define the shape, size
*        and coordinate system of the input grid.
c     lbnd
f     LBND( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the first pixel in the region
*        of the input grid which is to be included in the rebined output
*        array.
c     ubnd
f     UBND( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the last pixel in the region of
*        the input grid which is to be included in the rebined output
*        array.
*
c        Note that "lbnd" and "ubnd" together define the shape and
f        Note that LBND and UBND together define the shape and
*        position of a (hyper-)rectangular region of the input grid
*        which is to be included in the rebined output array. This region
*        should lie wholly within the extent of the input grid (as
c        defined by the "lbnd_in" and "ubnd_in" arrays). Regions of
f        defined by the LBND_IN and UBND_IN arrays). Regions of
*        the input grid lying outside this region will not be used.
c     out
f     OUT( * ) = <Xtype> (Returned)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        output grid, in which the rebined data values will be
*        returned. The numerical type of this array should match that
c        of the "in" array, and the data storage order should be such
f        of the IN array, and the data storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
c     out_var
f     OUT_VAR( * ) = <Xtype> (Returned)
c        An optional pointer to an array with the same type and size
c        as the "out" array. If given, this array will be used to
c        return variance estimates for the rebined data values. This
c        array will only be used if the "in_var" array has also been
c        supplied.
f        An optional array with the same type and size as the OUT
f        array. If the AST__USEVAR flag is set via the FLAGS argument,
f        this array will be used to return variance estimates for the
f        rebined data values.
*
*        The output variance values will be calculated on the
*        assumption that errors on the input data values are
*        statistically independent and that their variance estimates
*        may simply be summed (with appropriate weighting factors)
*        when several input pixels contribute to an output data
*        value. If this assumption is not valid, then the output error
*        estimates may be biased. In addition, note that the
*        statistical errors on neighbouring output data values (as
*        well as the estimates of those errors) may often be
*        correlated, even if the above assumption about the input data
*        is correct, because of the pixel spreading schemes
*        employed.
*
c        If no output variance estimates are required, a NULL pointer
c        should be given.
f        If the AST__USEVAR flag is not set, no output variance
f        estimates will be calculated and this array will not be
f        used. A dummy (e.g. one-element) array may then be supplied.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Data Type Codes:
*     To select the appropriate rebinning function, you should
c     replace <X> in the generic function name astRebin<X> with a
f     replace <X> in the generic function name AST_REBIN<X> with a
*     1- or 2-character data type code, so as to match the numerical
*     type <Xtype> of the data you are processing, as follows:
c     - D: double
c     - F: float
c     - I: int
f     - D: DOUBLE PRECISION
f     - R: REAL
f     - I: INTEGER
*
c     For example, astRebinD would be used to process "double"
c     data, while astRebinI would be used to process "int"
c     data, etc.
f     For example, AST_REBIND would be used to process DOUBLE
f     PRECISION data, while AST_REBINI would be used to process
f     integer data (stored in an INTEGER array), etc.
*
*     Note that, unlike 
c     astResample<X>, the astRebin<X>
f     AST_RESAMPLE<X>, the AST_REBIN<X>
*     set of functions does not yet support unsigned integer data types
*     or integers of different sizes.

*  Pixel Spreading Schemes:
*     The pixel spreading scheme specifies the Point Spread Function (PSF) 
*     applied to each input pixel value as it is copied into the output 
*     array. It can be thought of as the inverse of the sub-pixel 
*     interpolation schemes used by the
c     astResample<X> 
f     AST_RESAMPLE<X>
*     group of functions. That is, in a sub-pixel interpolation scheme the 
*     kernel specifies the weight to assign to each input pixel when
*     forming the weighted mean of the input pixels, whereas the kernel in a 
*     pixel spreading scheme specifies the fraction of the input data value 
*     which is to be assigned to each output pixel. As for interpolation, the 
*     choice of suitable pixel spreading scheme involves stricking a balance 
*     between schemes which tend to degrade sharp features in the data by 
*     smoothing them, and those which attempt to preserve sharp features but 
*     which often tend to introduce unwanted artifacts. See the
c     astResample<X> 
f     AST_RESAMPLE<X>
*     documentation for further discussion.
*
*     The binning algorithm used has the ability to introduce artifacts
*     not seen when using a resampling algorithm. Particularly, when
*     viewing the output image at high contrast, systems of curves lines
*     covering the entire image may be visible. These are caused by a
*     beating effect between the input pixel positions and the output pixels
*     position, and their nature and strength depend critically upon the
*     nature of the Mapping and the spreading function being used. In
*     general, the nearest neighbour spreading function demonstrates this
*     effect more clearly than the other functions, and for this reason
*     should be used with caution.
*
*     The following values (defined in the 
c     "ast.h" header file)
f     AST_PAR include file)
*     may be assigned to the 
c     "spread" 
f     SPREAD 
*     parameter. See the 
c     astResample<X> 
f     AST_RESAMPLE<X>
*     documentation for details of these schemes including the use of the 
c     "fspread" and "params" parameters:
f     FSPREAD and PARAMS arguments:
*
*     - AST__NEAREST
*     - AST__LINEAR
*     - AST__SINC
*     - AST__SINCSINC
*     - AST__SINCCOS
*     - AST__SINCGAUSS
*     - AST__SOMBCOS
*
*     In addition, the following schemes can be used with 
f     AST_REBIN<X> but not with AST_RESAMPLE<X>:
c     astRebin<X> but not with astResample<X>:
*
*     - AST__GAUSS: This scheme uses a kernel of the form exp(-k*x*x), with k 
*     a positive constant determined by the full-width at half-maximum (FWHM).
*     The FWHM should be supplied in units of output pixels by means of the 
c     "params[1]"
f     PARAMS(2) 
*     value and should be at least 0.1. The 
c     "params[0]" 
f     PARAMS(1)
*     value should be used to specify at what point the Gaussian is truncated 
*     to zero. This should be given as a number of output pixels on either 
*     side of the central output point in each dimension (the nearest integer 
*     value is used).

*  Control Flags:
c     The following flags are defined in the "ast.h" header file and
f     The following flags are defined in the AST_PAR include file and
*     may be used to provide additional control over the rebinning
*     process. Having selected a set of flags, you should supply the
c     bitwise OR of their values via the "flags" parameter:
f     sum of their values via the FLAGS argument:
*
*     - AST__USEBAD: Indicates that there may be bad pixels in the
*     input array(s) which must be recognised by comparing with the
c     value given for "badval" and propagated to the output array(s).
f     value given for BADVAL and propagated to the output array(s).
*     If this flag is not set, all input values are treated literally
c     and the "badval" value is only used for flagging output array
f     and the BADVAL value is only used for flagging output array
*     values.
f     - AST__USEVAR: Indicates that variance information should be
f     processed in order to provide estimates of the statistical error
f     associated with the rebined values. If this flag is not set,
f     no variance processing will occur and the IN_VAR and OUT_VAR
f     arrays will not be used. (Note that this flag is only available
f     in the Fortran interface to AST.)

*  Propagation of Missing Data:
*     Instances of missing data (bad pixels) in the output grid are
c     identified by occurrences of the "badval" value in the "out"
f     identified by occurrences of the BADVAL value in the OUT
*     array. These are produced if the sum of the weights of the 
*     contributing input pixels is less than 
c     "wlim".
f     WLIM.
*
*     An input pixel is considered bad (and is consequently ignored) if 
*     its
c     data value is equal to "badval" and the AST__USEBAD flag is
c     set via the "flags" parameter.
f     data value is equal to BADVAL and the AST__USEBAD flag is
f     set via the FLAGS argument.
*
*     In addition, associated output variance estimates (if
c     calculated) may be declared bad and flagged with the "badval"
c     value in the "out_var" array for similar reasons.
f     calculated) may be declared bad and flagged with the BADVAL
f     value in the OUT_VAR array for similar reasons.
*--
*/
/* Define a macro to implement the function for a specific data
   type. */
#define MAKE_REBIN(X,Xtype,IntType) \
static void Rebin##X( AstMapping *this, double wlim, int ndim_in, \
                     const int lbnd_in[], const int ubnd_in[], \
                     const Xtype in[], const Xtype in_var[], \
                     int spread, const double params[], int flags, \
                     double tol, int maxpix, Xtype badval, \
                     int ndim_out, const int lbnd_out[], \
                     const int ubnd_out[], const int lbnd[], \
                     const int ubnd[], Xtype out[], Xtype out_var[] ) { \
\
/* Local Variables: */ \
   AstMapping *simple;           /* Pointer to simplified Mapping */ \
   Xtype *d;                     /* Pointer to next output data value */ \
   Xtype *v;                     /* Pointer to next output variance value */ \
   double *w;                    /* Pointer to next weight value */ \
   double *work;                 /* Pointer to weight array */ \
   int idim;                     /* Loop counter for coordinate dimensions */ \
   int ipix_out;                 /* Index into output array */ \
   int nin;                      /* Number of Mapping input coordinates */ \
   int nout;                     /* Number of Mapping output coordinates */ \
   int npix;                     /* Number of pixels in input region */ \
   int npix_out;                 /* Number of pixels in output array */ \
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
/* Obtain values for the Nin and Nout attributes of the Mapping. */ \
   nin = astGetNin( this ); \
   nout = astGetNout( this ); \
\
/* If OK, check that the number of input grid dimensions matches the \
   number required by the Mapping and is at least 1. Report an error \
   if necessary. */ \
   if ( astOK && ( ( ndim_in != nin ) || ( ndim_in < 1 ) ) ) { \
      astError( AST__NGDIN, "astRebin"#X"(%s): Bad number of input grid " \
                "dimensions (%d).", astGetClass( this ), ndim_in ); \
      if ( ndim_in != nin ) { \
         astError( AST__NGDIN, "The %s given requires %d coordinate value%s " \
                   "to specify an input position.", \
                   astGetClass( this ), nin, ( nin == 1 ) ? "" : "s" ); \
      } \
   } \
\
/* If OK, also check that the number of output grid dimensions matches \
   the number required by the Mapping and is at least 1. Report an \
   error if necessary. */ \
   if ( astOK && ( ( ndim_out != nout ) || ( ndim_out < 1 ) ) ) { \
      astError( AST__NGDIN, "astRebin"#X"(%s): Bad number of output grid " \
                "dimensions (%d).", astGetClass( this ), ndim_out ); \
      if ( ndim_out != nout ) { \
         astError( AST__NGDIN, "The %s given generates %s%d coordinate " \
                   "value%s for each output position.", astGetClass( this ), \
                   ( nout < ndim_out ) ? "only " : "", nout, \
                   ( nout == 1 ) ? "" : "s" ); \
      } \
   } \
\
/* Check that the lower and upper bounds of the input grid are \
   consistent. Report an error if any pair is not. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_in; idim++ ) { \
         if ( lbnd_in[ idim ] > ubnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astRebin"#X"(%s): Lower bound of " \
                      "input grid (%d) exceeds corresponding upper bound " \
                      "(%d).", astGetClass( this ), \
                      lbnd_in[ idim ], ubnd_in[ idim ] ); \
            astError( AST__GBDIN, "Error in input dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* Check that the positional accuracy tolerance supplied is valid and \
   report an error if necessary. */ \
   if ( astOK && ( tol < 0.0 ) ) { \
      astError( AST__PATIN, "astRebin"#X"(%s): Invalid positional " \
                "accuracy tolerance (%.*g pixel).", \
                astGetClass( this ), DBL_DIG, tol ); \
      astError( AST__PATIN, "This value should not be less than zero." ); \
   } \
\
/* Check that the initial scale size in pixels supplied is valid and \
   report an error if necessary. */ \
   if ( astOK && ( maxpix < 0 ) ) { \
      astError( AST__SSPIN, "astRebin"#X"(%s): Invalid initial scale " \
                "size in pixels (%d).", astGetClass( this ), maxpix ); \
      astError( AST__SSPIN, "This value should not be less than zero." ); \
   } \
\
/* Check that the lower and upper bounds of the output grid are \
   consistent. Report an error if any pair is not. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         if ( lbnd_out[ idim ] > ubnd_out[ idim ] ) { \
            astError( AST__GBDIN, "astRebin"#X"(%s): Lower bound of " \
                      "output grid (%d) exceeds corresponding upper bound " \
                      "(%d).", astGetClass( this ), \
                      lbnd_out[ idim ], ubnd_out[ idim ] ); \
            astError( AST__GBDIN, "Error in output dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* Similarly check the bounds of the input region. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         if ( lbnd[ idim ] > ubnd[ idim ] ) { \
            astError( AST__GBDIN, "astRebin"#X"(%s): Lower bound of " \
                      "input region (%d) exceeds corresponding upper " \
                      "bound (%d).", astGetClass( this ), \
                      lbnd[ idim ], ubnd[ idim ] ); \
\
/* Also check that the input region lies wholly within the input \
   grid. */ \
         } else if ( lbnd[ idim ] < lbnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astRebin"#X"(%s): Lower bound of " \
                      "input region (%d) is less than corresponding " \
                      "bound of input grid (%d).", astGetClass( this ), \
                      lbnd[ idim ], lbnd_in[ idim ] ); \
         } else if ( ubnd[ idim ] > ubnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astRebin"#X"(%s): Upper bound of " \
                      "input region (%d) exceeds corresponding " \
                      "bound of input grid (%d).", astGetClass( this ), \
                      ubnd[ idim ], ubnd_in[ idim ] ); \
         } \
\
/* Say which dimension produced the error. */ \
         if ( !astOK ) { \
            astError( AST__GBDIN, "Error in output dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* If OK, loop to determine how many input pixels are to be binned. */ \
   simple = NULL; \
   npix = 1; \
   npix_out = 1; \
   unsimplified_mapping = this; \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_in; idim++ ) { \
         npix *= ubnd[ idim ] - lbnd[ idim ] + 1; \
      } \
\
/* Loop to determine how many pixels the output array contains. */ \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         npix_out *= ubnd_out[ idim ] - lbnd_out[ idim ] + 1; \
      } \
\
/* If there are sufficient pixels to make it worthwhile, simplify the \
   Mapping supplied to improve performance. Otherwise, just clone the \
   Mapping pointer. Note we have already saved a pointer to the original \
   Mapping so that lower-level functions can use it if they need to report \
   an error. */ \
      if ( npix > 1024 ) { \
         simple = astSimplify( this ); \
      } else { \
         simple = astClone( this ); \
      } \
   } \
\
/* Report an error if the forward transformation of this simplified \
   Mapping is not defined. */ \
   if ( !astGetTranForward( simple ) && astOK ) { \
      astError( AST__TRNND, "astRebin"#X"(%s): An forward coordinate " \
                "transformation is not defined by the %s supplied.", \
                astGetClass( unsimplified_mapping ), \
                astGetClass( unsimplified_mapping ) ); \
   } \
\
/* If required, allocate work array to hold the sum of the weights \
   contributing to each output pixel, and initialise it to zero. */ \
   if( wlim > 0.0 ) { \
      work = astMalloc( sizeof( double )*(size_t) npix_out ); \
      if( work ) { \
         w = work; \
         for( ipix_out = 0; ipix_out < npix_out; ipix_out++ ) *(w++) = 0.0; \
      } \
   } else { \
      work = NULL; \
   } \
\
/* Initialise the output arrays to hold zeros. */ \
   d = out; \
   if( out_var ) { \
      v = out_var; \
      for( ipix_out = 0; ipix_out < npix_out; ipix_out++, d++, v++ ) { \
         *d = 0; \
         *v = 0; \
      } \
   } else { \
      for( ipix_out = 0; ipix_out < npix_out; ipix_out++, d++ ) { \
         *d = 0; \
      } \
   } \
\
/* Perform the rebinning. Note that we pass all gridded data, the \
   spread function and the bad pixel value by means of pointer \
   types that obscure the underlying data type. This is to avoid \
   having to replicate functions unnecessarily for each data \
   type. However, we also pass an argument that identifies the data \
   type we have obscured. */ \
   RebinAdaptively( simple, ndim_in, lbnd_in, ubnd_in, \
                    (const void *) in, (const void *) in_var, \
                    TYPE_##X, spread, \
                    params, flags, tol, maxpix, \
                    (const void *) &badval, \
                    ndim_out, lbnd_out, ubnd_out, \
                    lbnd, ubnd, npix_out, \
                    (void *) out, (void *) out_var, work ); \
\
/* If required set output pixels bad if they have a total weight less \
   than "wlim". */ \
   if( work ) { \
      w = work; \
      d = out; \
      if( out_var ) { \
         v = out_var; \
         for( ipix_out = 0; ipix_out < npix_out; ipix_out++, d++, w++, v++ ) { \
            if( *w < wlim ) { \
               *d = badval; \
               *v = badval; \
            } \
         } \
      } else { \
         for( ipix_out = 0; ipix_out < npix_out; ipix_out++, d++, w++ ) { \
            if( *w < wlim ) *d = badval; \
         } \
      } \
\
/* Free the work array. */ \
      work = astFree( work ); \
   } \
\
/* Annul the pointer to the simplified/cloned Mapping. */ \
   simple = astAnnul( simple ); \
\
}

/* Expand the above macro to generate a function for each required
   data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_REBIN(LD,long double,0)
#endif
MAKE_REBIN(D,double,0)
MAKE_REBIN(F,float,0) 
MAKE_REBIN(I,int,1)

/* Undefine the macro. */
#undef MAKE_REBIN

static void RebinAdaptively( AstMapping *this, int ndim_in,
                            const int *lbnd_in, const int *ubnd_in,
                            const void *in, const void *in_var,
                            DataType type, int spread, 
                            const double *params, int flags, double tol,
                            int maxpix, const void *badval_ptr,
                            int ndim_out, const int *lbnd_out,
                            const int *ubnd_out, const int *lbnd,
                            const int *ubnd, int npix_out, void *out, 
                            void *out_var, double *work ){
/*
*  Name:
*     RebinAdaptively

*  Purpose:
*     Rebin a section of a data grid adaptively.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void RebinAdaptively( AstMapping *this, int ndim_in,
*                          const int *lbnd_in, const int *ubnd_in,
*                          const void *in, const void *in_var,
*                          DataType type, int spread, 
*                          const double *params, int flags, double tol,
*                          int maxpix, const void *badval_ptr,
*                          int ndim_out, const int *lbnd_out,
*                          const int *ubnd_out, const int *lbnd,
*                          const int *ubnd, int npix_out, void *out, 
*                          void *out_var, double *work )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function rebins a specified section of a rectangular grid of 
*     data (with any number of dimensions) into another rectangular grid 
*     (with a possibly different number of dimensions). The coordinate 
*     transformation used to convert input pixel coordinates into positions 
*     in the output grid is given by the forward transformation of the 
*     Mapping which is supplied.  Any pixel spreading scheme may be specified 
*     for distributing the flux of an input pixel amongst the output
*     pixels.
*
*     This function is very similar to RebinWithBlocking and RebinSection 
*     which lie below it in the calling hierarchy. However, this function 
*     also attempts to adapt to the Mapping supplied and to sub-divide the 
*     section being rebinned into smaller sections within which a linear 
*     approximation to the Mapping may be used.  This reduces the number of 
*     Mapping evaluations, thereby improving efficiency particularly when
*     complicated Mappings are involved.

*  Parameters:
*     this
*        Pointer to a Mapping, whose forward transformation may be
*        used to transform the coordinates of pixels in the input
*        grid into associated positions in the output grid.
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     in
*        Pointer to the input array of data to be rebinned (with one
*        element for each pixel in the input grid). The numerical type
*        of these data should match the "type" value (below). The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and data type as the "in" array),
*        which represent estimates of the statistical variance
*        associated with each element of the "in" array. If this
*        second array is given (along with the corresponding "out_var"
*        array), then estimates of the variance of the rebinned data
*        will also be returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     type
*        A value taken from the "DataType" enum, which specifies the
*        data type of the input and output arrays containing the
*        gridded data (and variance) values.
*     spread
*        A value selected from a set of pre-defined macros to identify
*        which pixel spread function should be used.
*     params
*        Pointer to an optional array of parameters that may be passed
*        to the pixel spread algorithm, if required. If no parameters
*        are required, a NULL pointer should be supplied.
*     flags
*        The bitwise OR of a set of flag values which provide additional 
*        control over the resampling operation.
*     tol
*        The maximum permitted positional error in transforming input
*        pixel positions into the output grid in order to rebin
*        it. This should be expressed as a displacement in pixels in
*        the output grid's coordinate system. If the Mapping's forward
*        transformation can be approximated by piecewise linear functions 
*        to this accuracy, then such functions may be used instead of the 
*        Mapping in order to improve performance. Otherwise, every input 
*        pixel position will be transformed individually using the Mapping.
*
*        If linear approximation is not required, a "tol" value of
*        zero may be given. This will ensure that the Mapping is used
*        without any approximation.
*     maxpix
*        A value which specifies the largest scale size on which to
*        search for non-linearities in the Mapping supplied. This
*        value should be expressed as a number of pixels in the input
*        grid. The function will break the input section specified
*        into smaller sub-sections (if necessary), each no larger than
*        "maxpix" pixels in any dimension, before it attempts to
*        approximate the Mapping by a linear function over each
*        sub-section.
* 
*        If the value given is larger than the largest dimension of
*        the input section (the normal recommendation), the function
*        will initially search for non-linearity on a scale determined
*        by the size of the input section.  This is almost always
*        satisfactory. Very occasionally, however, a Mapping may
*        appear linear on this scale but nevertheless have smaller
*        irregularities (e.g. "holes") in it.  In such cases, "maxpix"
*        may be set to a suitably smaller value so as to ensure this
*        non-linearity is not overlooked. Typically, a value of 50 to
*        100 pixels might be suitable and should have little effect on
*        performance.
*
*        If too small a value is given, however, it will have the
*        effect of preventing linear approximation occurring at all
*        (equivalent to setting "tol" to zero).  Although this may
*        degrade performance, accurate results will still be obtained.
*     badval_ptr
*        If the AST__USEBAD flag is set (above), this parameter is a
*        pointer to a value which is used to identify bad data and/or
*        variance values in the input array(s). The referenced value's
*        data type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which rebinned values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output data grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output data grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output data grid in the same way as "lbnd_in"
*        and "ubnd_in" define the shape and size of the input grid
*        (see above).
*     lbnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the first pixel in the
*        section of the input data grid which is to be rebinned.
*     ubnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the last pixel in the
*        section of the input data grid which is to be rebinned.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the input grid which is to be rebinned. This section 
*        should lie wholly within the extent of the input grid (as defined 
*        by the "lbnd_out" and "ubnd_out" arrays). Regions of the input 
*        grid lying outside this section will be ignored.
*     npix_out
*        The number of pixels in the output array.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the rebinned data will be returned.  The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the rebinned values may be returned. This array will only be
*        used if the "in_var" array has been given.
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     work
*        An optional pointer to a double array with the same size as 
*        the "out" array. The contents of this array (if supplied) are
*        incremented by the accumulated weights assigned to each output pixel.
*        If no accumulated weights are required, a NULL pointer should be 
*        given.

*/
                      
/* Local Variables: */
   double *flbnd;                /* Array holding floating point lower bounds */
   double *fubnd;                /* Array holding floating point upper bounds */
   double *linear_fit;           /* Pointer to array of fit coefficients */
   int *hi;                      /* Pointer to array of section upper bounds */
   int *lo;                      /* Pointer to array of section lower bounds */
   int coord_in;                 /* Loop counter for input coordinates */
   int dim;                      /* Output section dimension size */
   int dimx;                     /* Dimension with maximum section extent */
   int divide;                   /* Sub-divide the output section? */
   int i;                        /* Loop count */
   int isLinear;                 /* Is the transformation linear? */
   int mxdim;                    /* Largest output section dimension size */
   int npix;                     /* Number of pixels in output section */
   int npoint;                   /* Number of points for obtaining a fit */
   int nvertex;                  /* Number of vertices of output section */
   int toobig;                   /* Section too big (must sub-divide)? */
   int toosmall;                 /* Section too small to sub-divide? */

/* Check the global error status. */
   if ( !astOK ) return;

/* Further initialisation. */
   npix = 1;
   mxdim = 0;
   dimx = 1;
   nvertex = 1;

/* Loop through the input grid dimensions. */
   for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {

/* Obtain the extent in each dimension of the input section which is
   to be rebinned, and calculate the total number of pixels it contains. */
      dim = ubnd[ coord_in ] - lbnd[ coord_in ] + 1;
      npix *= dim;

/* Find the maximum dimension size of this input section and note which 
   dimension has this size. */
      if ( dim > mxdim ) {
         mxdim = dim;
         dimx = coord_in;
      }

/* Calculate how many vertices the output section has. */
      nvertex *= 2;
   }
   
/* Calculate how many sample points will be needed (by the astLinearApprox 
   function) to obtain a linear fit to the Mapping's forward transformation. */
   npoint = 1 + 4 * ndim_in + 2 * nvertex;

/* If the number of pixels in the input section is not at least 4
   times this number, we will probably not save significant time by
   attempting to obtain a linear fit, so note that the input section
   is too small. */
   toosmall = ( npix < ( 4 * npoint ) );

/* Note if the maximum dimension of the input section exceeds the
   user-supplied scale factor. */
   toobig = ( maxpix < mxdim );

/* Assume the Mapping is significantly non-linear before deciding
   whether to sub-divide the output section. */
   linear_fit = NULL;

/* If the output section is too small to be worth obtaining a linear
   fit, or if the accuracy tolerance is zero, we will not
   sub-divide. This means that the Mapping will be used to transform
   each pixel's coordinates and no linear approximation will be
   used. */
   if ( toosmall || ( tol == 0.0 ) ) {
      divide = 0;

/* Otherwise, if the largest input section dimension exceeds the
   scale length given, we will sub-divide. This offers the possibility
   of obtaining a linear approximation to the Mapping over a reduced
   range of input coordinates (which will be handled by a recursive
   invocation of this function). */
   } else if ( toobig ) {
      divide = 1;

/* If neither of the above apply, then attempt to fit a linear
   approximation to the Mapping's forward transformation over the
   range of coordinates covered by the input section. We need to 
   temporarily copy the integer bounds into floating point arrays to 
   use astLinearApprox. */
   } else {

/* Allocate memory for floating point bounds and for the coefficient array */
      flbnd = astMalloc( sizeof( double )*(size_t) ndim_in );
      fubnd = astMalloc( sizeof( double )*(size_t) ndim_in );
      linear_fit = astMalloc( sizeof( double )*
                              (size_t) ( ndim_out*( ndim_in + 1 ) ) );
      if( astOK ) {

/* Copy the bounds into these arrays */
         for( i = 0; i < ndim_in; i++ ) {
            flbnd[ i ] = (double) lbnd[ i ];
            fubnd[ i ] = (double) ubnd[ i ];
         }

/* Get the linear approximation to the forward transformation. */
         isLinear = astLinearApprox( this, flbnd, fubnd, tol, linear_fit );

/* Free the coeff array if the inverse transformation is not linear. */
         if( !isLinear ) linear_fit = astFree( linear_fit );

      } else {
         linear_fit = astFree( linear_fit );
      }

/* Free resources */
      flbnd = astFree( flbnd );
      fubnd = astFree( fubnd );

/* If a linear fit was obtained, we will use it and therefore do not
   wish to sub-divide further. Otherwise, we sub-divide in the hope
   that this may result in a linear fit next time. */
      divide = !linear_fit;
   }

/* If no sub-division is required, perform rebinning (in a
   memory-efficient manner, since the section we are rebinning might
   still be very large). This will use the linear fit, if obtained
   above. */
   if ( astOK ) {
      if ( !divide ) {
         RebinWithBlocking( this, linear_fit,
                            ndim_in, lbnd_in, ubnd_in,
                            in, in_var, type, spread, 
                            params, flags, badval_ptr,
                            ndim_out, lbnd_out, ubnd_out,
                            lbnd, ubnd, npix_out, out, out_var, work );

/* Otherwise, allocate workspace to perform the sub-division. */
      } else {
         lo = astMalloc( sizeof( int ) * (size_t) ndim_in );
         hi = astMalloc( sizeof( int ) * (size_t) ndim_in );
         if ( astOK ) {

/* Initialise the bounds of a new input section to match the original
   input section. */
            for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
               lo[ coord_in ] = lbnd[ coord_in ];
               hi[ coord_in ] = ubnd[ coord_in ];
            }

/* Replace the upper bound of the section's largest dimension with the
   mid-point of the section along this dimension, rounded downwards. */
            hi[ dimx ] =
               (int) floor( 0.5 * (double) ( lbnd[ dimx ] + ubnd[ dimx ] ) );

/* Rebin the resulting smaller section using a recursive invocation
   of this function. */
            RebinAdaptively( this, ndim_in, lbnd_in, ubnd_in,
                             in, in_var, type, spread, 
                             params, flags, tol, maxpix,
                             badval_ptr, ndim_out,
                             lbnd_out, ubnd_out,
                             lo, hi, npix_out, out, out_var, work );

/* Now set up a second section which covers the remaining half of the
   original input section. */
            lo[ dimx ] = hi[ dimx ] + 1;
            hi[ dimx ] = ubnd[ dimx ];

/* If this section contains pixels, resample it in the same way,
   summing the returned values. */
            if ( lo[ dimx ] <= hi[ dimx ] ) {
               RebinAdaptively( this, ndim_in, lbnd_in, ubnd_in,
                                in, in_var, type, spread, 
                                params, flags, tol, maxpix,
                                badval_ptr,  ndim_out,
                                lbnd_out, ubnd_out,
                                lo, hi, npix_out, out, out_var, work );
            }
         }

/* Free the workspace. */
         lo = astFree( lo );
         hi = astFree( hi );
      }
   }

/* If coefficients for a linear fit were obtained, then free the space
   they occupy. */
   if ( linear_fit ) linear_fit = astFree( linear_fit );
}

static void RebinSection( AstMapping *this, const double *linear_fit,
                         int ndim_in,
                         const int *lbnd_in, const int *ubnd_in,
                         const void *in, const void *in_var,
                         DataType type, int spread,
                         const double *params, int flags,
                         const void *badval_ptr, int ndim_out,
                         const int *lbnd_out, const int *ubnd_out,
                         const int *lbnd, const int *ubnd, int npix_out,
                         void *out, void *out_var, double *work ) {
/*
*  Name:
*     RebinSection

*  Purpose:
*     Rebin a section of a data grid.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void RebinSection( AstMapping *this, const double *linear_fit,
*                       int ndim_in, const int *lbnd_in, const int *ubnd_in,
*                       const void *in, const void *in_var,
*                       DataType type, int spread,
*                       const double *params, int flags,
*                       const void *badval_ptr, int ndim_out,
*                       const int *lbnd_out, const int *ubnd_out,
*                       const int *lbnd, const int *ubnd, int npix_out,
*                       void *out, void *out_var, double *work )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function rebins a specified section of a rectangular grid of 
*     data (with any number of dimensions) into another rectangular grid 
*     (with a possibly different number of dimensions). The coordinate 
*     transformation used to convert input pixel coordinates into positions 
*     in the output grid is given by the forward transformation of the 
*     Mapping which is supplied or, alternatively, by a linear approximation 
*     fitted to a Mapping's forward transformation. Any pixel spreading scheme 
*     may be specified for distributing the flux of an input pixel amongst 
*     the output pixels.

*  Parameters:
*     this
*        Pointer to a Mapping, whose forward transformation may be
*        used to transform the coordinates of pixels in the input
*        grid into associated positions in the output grid.
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     linear_fit
*        Pointer to an optional array of double which contains the
*        coefficients of a linear fit which approximates the above
*        Mapping's forward coordinate transformation. If this is
*        supplied, it will be used in preference to the above Mapping
*        when transforming coordinates. This may be used to enhance
*        performance in cases where evaluation of the Mapping's
*        forward transformation is expensive. If no linear fit is
*        available, a NULL pointer should be supplied.
*
*        The way in which the fit coefficients are stored in this
*        array and the number of array elements are as defined by the
*        astLinearApprox function.
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     in
*        Pointer to the input array of data to be rebinned (with one
*        element for each pixel in the input grid). The numerical type
*        of these data should match the "type" value (below). The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and data type as the "in" array),
*        which represent estimates of the statistical variance
*        associated with each element of the "in" array. If this
*        second array is given (along with the corresponding "out_var"
*        array), then estimates of the variance of the rebinned data
*        will also be returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     type
*        A value taken from the "DataType" enum, which specifies the
*        data type of the input and output arrays containing the
*        gridded data (and variance) values.
*     spread
*        A value selected from a set of pre-defined macros to identify
*        which pixel spread function should be used.
*     params
*        Pointer to an optional array of parameters that may be passed
*        to the pixel spread algorithm, if required. If no parameters
*        are required, a NULL pointer should be supplied.
*     flags
*        The bitwise OR of a set of flag values which provide additional 
*        control over the resampling operation.
*     badval_ptr
*        If the AST__USEBAD flag is set (above), this parameter is a
*        pointer to a value which is used to identify bad data and/or
*        variance values in the input array(s). The referenced value's
*        data type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which rebinned values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output data grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output data grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output data grid in the same way as "lbnd_in"
*        and "ubnd_in" define the shape and size of the input grid
*        (see above).
*     lbnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the first pixel in the
*        section of the input data grid which is to be rebinned.
*     ubnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the last pixel in the
*        section of the input data grid which is to be rebinned.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the input grid which is to be rebinned. This section 
*        should lie wholly within the extent of the input grid (as defined 
*        by the "lbnd_out" and "ubnd_out" arrays). Regions of the input 
*        grid lying outside this section will be ignored.
*     npix_out
*        The number of pixels in the output array.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the rebinned data will be returned.  The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the rebinned values may be returned. This array will only be
*        used if the "in_var" array has been given.
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     work
*        An optional pointer to a double array with the same size as 
*        the "out" array. The contents of this array (if supplied) are
*        incremented by the accumulated weights assigned to each output pixel.
*        If no accumulated weights are required, a NULL pointer should be 
*        given.

*  Notes:
*     - This function does not take steps to limit memory usage if the
*     grids supplied are large. To resample large grids in a more
*     memory-efficient way, the ResampleWithBlocking function should
*     be used.
*/

/* Local Variables: */
   AstPointSet *pset_in;         /* Input PointSet for transformation */
   AstPointSet *pset_out;        /* Output PointSet for transformation */
   const double *grad;           /* Pointer to gradient matrix of linear fit */
   const double *zero;           /* Pointer to zero point array of fit */
   double **ptr_in;              /* Pointer to input PointSet coordinates */
   double **ptr_out;             /* Pointer to output PointSet coordinates */
   double *accum;                /* Pointer to array of accumulated sums */
   double x1;                    /* Interim x coordinate value */
   double xx1;                   /* Initial x coordinate value */
   double y1;                    /* Interim y coordinate value */
   double yy1;                   /* Initial y coordinate value */
   int *dim;                     /* Pointer to array of output pixel indices */
   int *offset;                  /* Pointer to array of output pixel offsets */
   int *stride;                  /* Pointer to array of output grid strides */
   int coord_in;                 /* Loop counter for input dimensions */
   int coord_out;                /* Loop counter for output dimensions */
   int done;                     /* All pixel indices done? */
   int i1;                       /* Interim offset into "accum" array */
   int i2;                       /* Final offset into "accum" array */
   int idim;                     /* Loop counter for dimensions */
   int ix;                       /* Loop counter for output x coordinate */
   int iy;                       /* Loop counter for output y coordinate */
   int neighb;                   /* Number of neighbouring pixels */
   int npoint;                   /* Number of output points (pixels) */
   int off1;                     /* Interim pixel offset into output array */
   int off2;                     /* Interim pixel offset into output array */
   int off;                      /* Final pixel offset into output array */
   int point;                    /* Counter for output points (pixels ) */
   int s;                        /* Temporary variable for strides */
   const double *par;            /* Pointer to parameter array */
   double fwhm;                  /* Full width half max. of gaussian */
   double lpar[ 1 ];             /* Local parameter array */
   void (* kernel)( double, const double [], int, double * ); /* Kernel fn. */

/* Check the global error status. */
   if ( !astOK ) return;

/* Further initialisation. */
   pset_in = NULL;
   ptr_in = NULL;
   ptr_out = NULL;
   pset_out = NULL;
   neighb = 0;
   kernel = NULL;

/* Calculate the number of input points, as given by the product of
   the input grid dimensions. */
   for ( npoint = 1, coord_in = 0; coord_in < ndim_in; coord_in++ ) {
      npoint *= ubnd[ coord_in ] - lbnd[ coord_in ] + 1;
   }

/* Allocate workspace. */
   offset = astMalloc( sizeof( int ) * (size_t) npoint );
   stride = astMalloc( sizeof( int ) * (size_t) ndim_in );
   if ( astOK ) {

/* Calculate the stride for each input grid dimension. */
      off = 0;
      s = 1;
      for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
         stride[ coord_in ] = s;
         s *= ubnd_in[ coord_in ] - lbnd_in[ coord_in ] + 1;
      }

/* A linear fit to the Mapping is available. */
/* ========================================= */
      if ( linear_fit ) {

/* If a linear fit to the Mapping has been provided, then obtain
   pointers to the array of gradients and zero-points comprising the
   fit. */
         grad = linear_fit + ndim_out;
         zero = linear_fit;

/* Create a PointSet to hold the output grid coordinates and obtain an
   array of pointers to its coordinate data. */
         pset_out = astPointSet( npoint, ndim_out, "" );
         ptr_out = astGetPoints( pset_out );
         if ( astOK ) {

/* Initialise the count of input points. */
            point = 0;

/* Handle the 1-dimensional case optimally. */
/* ---------------------------------------- */
            if ( ( ndim_in == 1 ) && ( ndim_out == 1 ) ) {

/* Loop through the pixels of the input grid and transform their x
   coordinates into the output grid's coordinate system using the
   linear fit supplied. Store the results in the PointSet created
   above. */
               off = lbnd[ 0 ] - lbnd_in[ 0 ];
               xx1 = zero[ 0 ] + grad[ 0 ] * (double) lbnd[ 0 ];

               for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                  ptr_out[ 0 ][ point ] = xx1;
                  xx1 += grad[ 0 ];
                  offset[ point++ ] = off++;
               }

/* Handle the 2-dimensional case optimally. */
/* ---------------------------------------- */
            } else if ( ( ndim_in == 2 ) && ( ndim_out == 2 ) ) {

/* Loop through the range of y coordinates in the input grid and
   calculate interim values of the output coordinates using the linear
   fit supplied. */
               x1 = zero[ 0 ] + grad[ 1 ] * (double) ( lbnd[ 1 ] - 1 );
               y1 = zero[ 1 ] + grad[ 3 ] * (double) ( lbnd[ 1 ] - 1 );
               off1 = stride[ 1 ] * ( lbnd[ 1 ] - lbnd_in[ 1 ] - 1 ) - lbnd_in[ 0 ];
               for ( iy = lbnd[ 1 ]; iy <= ubnd[ 1 ]; iy++ ) {
                  x1 += grad[ 1 ];
                  y1 += grad[ 3 ];

/* Also calculate an interim pixel offset into the input array. */
                  off1 += stride[ 1 ];

/* Now loop through the range of input x coordinates and calculate
   the final values of the input coordinates, storing the results in
   the PointSet created above. */
                  xx1 = x1 + grad[ 0 ] * (double) lbnd[ 0 ];
                  yy1 = y1 + grad[ 2 ] * (double) lbnd[ 0 ];
                  off = off1 + lbnd[ 0 ];
                  for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                     ptr_out[ 0 ][ point ] = xx1;
                     xx1 += grad[ 0 ];
                     ptr_out[ 1 ][ point ] = yy1;
                     yy1 += grad[ 2 ];

/* Also calculate final pixel offsets into the input array. */
                     offset[ point++ ] = off++;
                  }
               }

/* Handle other numbers of dimensions. */
/* ----------------------------------- */               
            } else {

/* Allocate workspace. */
               accum = astMalloc( sizeof( double ) *
                                 (size_t) ( ndim_in * ndim_out ) );
               dim = astMalloc( sizeof( int ) * (size_t) ndim_in );
               if ( astOK ) {

/* Initialise an array of pixel indices for the input grid which refer to the 
   first pixel which we will rebin. Also calculate the offset of this pixel 
   within the input array. */
                  off = 0;
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     dim[ coord_in ] = lbnd[ coord_in ];
                     off += stride[ coord_in ] *
                            ( dim[ coord_in ] - lbnd_in[ coord_in ] );
                  }

/* To calculate each output grid coordinate we must perform a matrix
   multiply on the input grid coordinates (using the gradient matrix)
   and then add the zero points. However, since we will usually only
   be altering one input coordinate at a time (the least
   significant), we can avoid the full matrix multiply by accumulating
   partial sums for the most significant input coordinates and only
   altering those sums which need to change each time. The zero points
   never change, so we first fill the "most significant" end of the
   "accum" array with these. */
                  for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                     accum[ ( coord_out + 1 ) * ndim_in - 1 ] =
                                                              zero[ coord_out ];
                  }
                  coord_in = ndim_in - 1;

/* Now loop to process each input pixel. */
                  for ( done = 0; !done; point++ ) {

/* To generate the output coordinate that corresponds to the current
   input pixel, we work down from the most significant dimension
   whose index has changed since the previous pixel we considered
   (given by "coord_in"). For each affected dimension, we accumulate
   in "accum" the matrix sum (including the zero point) for that
   dimension and all higher input dimensions. We must accumulate a
   separate set of sums for each output coordinate we wish to
   produce. (Note that for the first pixel we process, all dimensions
   are considered "changed", so we start by initialising the whole
   "accum" array.) */
                     for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
/*
                        ptr_out[ coord_out ][ point ] = zero[ coord_out ];
                        for ( idim = 0; idim < ndim_in; idim++ ) {
                           ptr_out[ coord_out ][ point ] += 
                                 grad[ idim + coord_out*ndim_in ] *
                                 dim[ idim ];
                        }
*/

                        i1 = coord_out * ndim_in;
                        for ( idim = coord_in; idim >= 1; idim-- ) {
                           i2 = i1 + idim;
                           accum[ i2 - 1 ] = accum[ i2 ] +
                                             dim[ idim ] * grad[ i2 ];
                        }

/* The output coordinate for each dimension is given by the accumulated
   sum for input dimension zero (giving the sum over all input
   dimensions). We do not store this in the "accum" array, but assign
   the result directly to the coordinate array of the PointSet created
   earlier. */
                        ptr_out[ coord_out ][ point ] = accum[ i1 ] +
                                                      dim[ 0 ] * grad[ i1 ];
                     }

/* Store the offset of the current pixel in the input array. */
                     offset[ point ] = off;

/* Now update the array of pixel indices to refer to the next input pixel. */
                     coord_in = 0;
                     do {

/* The least significant index which currently has less than its maximum 
   value is incremented by one. The offset into the input array is updated 
   accordingly. */
                        if ( dim[ coord_in ] < ubnd[ coord_in ] ) {
                           dim[ coord_in ]++;
                           off += stride[ coord_in ];
                           break;

/* Any less significant indices which have reached their maximum value
   are returned to their minimum value and the input pixel offset is
   decremented appropriately. */
                        } else {
                           dim[ coord_in ] = lbnd[ coord_in ];
                           off -= stride[ coord_in ] *
                                  ( ubnd[ coord_in ] - lbnd[ coord_in ] );

/* All the output pixels have been processed once the most significant
   pixel index has been returned to its minimum value. */
                           done = ( ++coord_in == ndim_in );
                        }
                     } while ( !done );
                  }
               }

/* Free the workspace. */
               accum = astFree( accum );
               dim = astFree( dim );
            }
         }

/* No linear fit to the Mapping is available. */
/* ========================================== */
      } else {

/* Create a PointSet to hold the coordinates of the input pixels and
   obtain a pointer to its coordinate data. */
         pset_in = astPointSet( npoint, ndim_in, "" );
         ptr_in = astGetPoints( pset_in );
         if ( astOK ) {

/* Initialise the count of input points. */
            point = 0;

/* Handle the 1-dimensional case optimally. */
/* ---------------------------------------- */
            if ( ndim_in == 1 ) {

/* Loop through the required range of input x coordinates, assigning
   the coordinate values to the PointSet created above. Also store a
   pixel offset into the input array. */
               for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                  ptr_in[ 0 ][ point ] = (double) ix;
                  offset[ point++ ] = ix - lbnd_in[ 0 ];
               }

/* Handle the 2-dimensional case optimally. */
/* ---------------------------------------- */
            } else if ( ndim_in == 2 ) {

/* Loop through the required range of input y coordinates,
   calculating an interim pixel offset into the input array. */
               off1 = stride[ 1 ] * ( lbnd[ 1 ] - lbnd_in[ 1 ] - 1 ) 
                      - lbnd_in[ 0 ];
               for ( iy = lbnd[ 1 ]; iy <= ubnd[ 1 ]; iy++ ) {
                  off1 += stride[ 1 ];

/* Loop through the required range of input x coordinates, assigning
   the coordinate values to the PointSet created above. Also store a
   final pixel offset into the input array. */
                  off2 = off1 + lbnd[ 0 ];
                  for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                     ptr_in[ 0 ][ point ] = (double) ix;
                     ptr_in[ 1 ][ point ] = (double) iy;
                     offset[ point++ ] = off2++;
                  }
               }

/* Handle other numbers of dimensions. */
/* ----------------------------------- */
            } else {

/* Allocate workspace. */
               dim = astMalloc( sizeof( int ) * (size_t) ndim_in );
               if ( astOK ) {

/* Initialise an array of pixel indices for the input grid which
   refer to the first pixel to be rebinned. Also calculate the offset 
   of this pixel within the input array. */
                  off = 0;
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     dim[ coord_in ] = lbnd[ coord_in ];
                     off += stride[ coord_in ] *
                            ( dim[ coord_in ] - lbnd_in[ coord_in ] );
                  }

/* Loop to generate the coordinates of each input pixel. */
                  for ( done = 0; !done; point++ ) {

/* Copy each pixel's coordinates into the PointSet created above. */
                     for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                        ptr_in[ coord_in ][ point ] =
                                                     (double) dim[ coord_in ];
                     }

/* Store the offset of the pixel in the input array. */
                     offset[ point ] = off;

/* Now update the array of pixel indices to refer to the next input
   pixel. */
                     coord_in = 0;
                     do {

/* The least significant index which currently has less than its
   maximum value is incremented by one. The offset into the input
   array is updated accordingly. */
                        if ( dim[ coord_in ] < ubnd[ coord_in ] ) {
                           dim[ coord_in ]++;
                           off += stride[ coord_in ];
                           break;

/* Any less significant indices which have reached their maximum value
   are returned to their minimum value and the input pixel offset is
   decremented appropriately. */
                        } else {
                           dim[ coord_in ] = lbnd[ coord_in ];
                           off -= stride[ coord_in ] *
                                  ( ubnd[ coord_in ] - lbnd[ coord_in ] );

/* All the input pixels have been processed once the most significant
   pixel index has been returned to its minimum value. */
                           done = ( ++coord_in == ndim_in );
                        }
                     } while ( !done );
                  }
               }

/* Free the workspace. */
               dim = astFree( dim );
            }

/* When all the input pixel coordinates have been generated, use the
   Mapping's forward transformation to generate the output coordinates
   from them. Obtain an array of pointers to the resulting coordinate
   data. */
            pset_out = astTransform( this, pset_in, 1, NULL );
            ptr_out = astGetPoints( pset_out );
         }

/* Annul the PointSet containing the input coordinates. */
         pset_in = astAnnul( pset_in );
      }
   }


/* Rebin the input grid. */
/* ------------------------ */
   if( astOK ) {

/* Identify the pixel spreading scheme to be used. */
/* Nearest pixel. */
/* -------------- */
      switch ( spread ) {
         case AST__NEAREST:

/* Define a macro to use a "case" statement to invoke the
   nearest-pixel spreading function appropriate to a given data
   type. */
#define CASE_NEAREST(X,Xtype) \
               case ( TYPE_##X ): \
                  SpreadNearest##X( ndim_out, lbnd_out, ubnd_out, \
                                    (Xtype *) in, (Xtype *) in_var, \
                                    npoint, offset, \
                                    (const double *const *) ptr_out, \
                                    flags, *( (Xtype *) badval_ptr ), \
                                    npix_out, \
                                    (Xtype *) out, (Xtype *) out_var, work ); \
                  break;
       
/* Use the above macro to invoke the appropriate function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_NEAREST(LD,long double)
#endif
               CASE_NEAREST(D,double)
               CASE_NEAREST(F,float)
               CASE_NEAREST(I,int)

               case ( TYPE_L ): break;
               case ( TYPE_B ): break;
               case ( TYPE_S ): break;
               case ( TYPE_UL ): break;
               case ( TYPE_UI ): break;
               case ( TYPE_US ): break;
               case ( TYPE_UB ): break;
            }
            break;

/* Undefine the macro. */
#undef CASE_NEAREST
               
/* Linear spreading. */
/* ----------------- */
/* Note this is also the default if zero is given. */
         case AST__LINEAR:
         case 0:

/* Define a macro to use a "case" statement to invoke the linear
   spreading function appropriate to a given data type. */
#define CASE_LINEAR(X,Xtype) \
               case ( TYPE_##X ): \
                  SpreadLinear##X( ndim_out, lbnd_out, ubnd_out,\
                                   (Xtype *) in, (Xtype *) in_var, \
                                   npoint, offset, \
                                   (const double *const *) ptr_out, \
                                   flags, *( (Xtype *) badval_ptr ), \
                                   npix_out, \
                                   (Xtype *) out, (Xtype *) out_var, work ); \
                  break;

/* Use the above macro to invoke the appropriate function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_LINEAR(LD,long double)
#endif
               CASE_LINEAR(D,double)
               CASE_LINEAR(F,float)
               CASE_LINEAR(I,int)

               case ( TYPE_L ): break;
               case ( TYPE_B ): break;
               case ( TYPE_S ): break;
               case ( TYPE_UL ): break;
               case ( TYPE_UI ): break;
               case ( TYPE_US ): break;
               case ( TYPE_UB ): break;
            }
            break;

/* Undefine the macro. */
#undef CASE_LINEAR

/* Spreading using a 1-d kernel. */
/* ----------------------------- */
         case AST__SINC:
         case AST__SINCCOS:
         case AST__SINCGAUSS:
         case AST__GAUSS:
         case AST__SINCSINC:
         case AST__SOMB:
         case AST__SOMBCOS:

/* Obtain a pointer to the appropriate 1-d kernel function (either
   internal or user-defined) and set up any parameters it may
   require. */
            par = NULL;
            switch ( spread ) {

/* sinc(pi*x) */
/* ---------- */
/* Assign the kernel function. */
               case AST__SINC:
                  kernel = Sinc;

/* Calculate the number of neighbouring pixels to use. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) {
                     neighb = 2;
                  } else {
                     neighb = MaxI( 1, neighb );
                  }
                  break;

/* somb(pi*x) */
/* ---------- */
/* Assign the kernel function. */
               case AST__SOMB:
                  kernel = Somb;

/* Calculate the number of neighbouring pixels to use. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) {
                     neighb = 2;
                  } else {
                     neighb = MaxI( 1, neighb );
                  }
                  break;

/* sinc(pi*x)*cos(k*pi*x) */
/* ---------------------- */
/* Assign the kernel function. */
               case AST__SINCCOS:
                  kernel = SincCos;

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 0.5 / MaxD( 1.0, params[ 1 ] );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, the number will be calculated automatically below. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = INT_MAX;

/* Calculate the maximum number of neighbouring pixels required by the
   width of the kernel, and use this value if preferable. */
                  neighb = MinI( neighb,
                                 (int) ceil( MaxD( 1.0, params[ 1 ] ) ) );
                  break;

/* sinc(pi*x)*exp(-k*x*x) */
/* ---------------------- */
/* Assign the kernel function. */
               case AST__SINCGAUSS:
                  kernel = SincGauss;

/* Constrain the full width half maximum of the gaussian factor. */
                  fwhm = MaxD( 0.1, params[ 1 ] );

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 4.0 * log( 2.0 ) / ( fwhm * fwhm );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, use the number of neighbouring pixels required by the width
   of the kernel (out to where the gaussian term falls to 1% of its
   peak value). */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = (int) ceil( sqrt( -log( 0.01 ) /
                                                                lpar[ 0 ] ) );
                  break;

/* exp(-k*x*x) */
/* ----------- */
/* Assign the kernel function. */
               case AST__GAUSS:
                  kernel = Gauss;

/* Constrain the full width half maximum of the gaussian. */
                  fwhm = MaxD( 0.1, params[ 1 ] );

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 4.0 * log( 2.0 ) / ( fwhm * fwhm );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, use the number of neighbouring pixels required by the width
   of the kernel (out to where the gaussian term falls to 1% of its
   peak value). */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = (int) ceil( sqrt( -log( 0.01 ) /
                                                                lpar[ 0 ] ) );
                  break;

/* somb(pi*x)*cos(k*pi*x) */
/* ---------------------- */
/* Assign the kernel function. */
               case AST__SOMBCOS:
                  kernel = SombCos;

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 0.5 / MaxD( 1.0, params[ 1 ] );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, the number will be calculated automatically below. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = INT_MAX;

/* Calculate the maximum number of neighbouring pixels required by the
   width of the kernel, and use this value if preferable. */
                  neighb = MinI( neighb,
                                 (int) ceil( MaxD( 1.0, params[ 1 ] ) ) );
                  break;

/* sinc(pi*x)*sinc(k*pi*x) */
/* ----------------------- */
/* Assign the kernel function. */
               case AST__SINCSINC:
                  kernel = SincSinc;

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 0.5 / MaxD( 1.0, params[ 1 ] );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, the number will be calculated automatically below. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = INT_MAX;

/* Calculate the maximum number of neighbouring pixels required by the
   width of the kernel, and use this value if preferable. */
                  neighb = MinI( neighb,
                                 (int) ceil( MaxD( 1.0, params[ 1 ] ) ) );
                  break;
            }

/* Define a macro to use a "case" statement to invoke the 1-d kernel
   interpolation function appropriate to a given data type, passing it
   the pointer to the kernel function obtained above. */
#define CASE_KERNEL1(X,Xtype) \
               case ( TYPE_##X ): \
                  SpreadKernel1##X( this, ndim_out, lbnd_out, ubnd_out, \
                                         (Xtype *) in, (Xtype *) in_var, \
                                         npoint, offset, \
                                         (const double *const *) ptr_out, \
                                         kernel, neighb, par, flags, \
                                         *( (Xtype *) badval_ptr ), \
                                         npix_out, \
                                         (Xtype *) out, (Xtype *) out_var, work ); \
                  break;

/* Use the above macro to invoke the appropriate function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_KERNEL1(LD,long double)
#endif
               CASE_KERNEL1(D,double)
               CASE_KERNEL1(F,float)
               CASE_KERNEL1(I,int)

               case ( TYPE_L ): break;
               case ( TYPE_B ): break;
               case ( TYPE_S ): break;
               case ( TYPE_UL ): break;
               case ( TYPE_UI ): break;
               case ( TYPE_US ): break;
               case ( TYPE_UB ): break;
            }
            break;

/* Undefine the macro. */
#undef CASE_KERNEL1

/* Error: invalid pixel spreading scheme specified. */
/* ------------------------------------------------ */
         default:

/* Define a macro to report an error message appropriate to a given
   data type. */
#define CASE_ERROR(X) \
            case TYPE_##X: \
               astError( AST__SISIN, "astRebin"#X"(%s): Invalid " \
                         "pixel spreading scheme (%d) specified.", \
                         astGetClass( unsimplified_mapping ), spread ); \
               break;
                                 
/* Use the above macro to report an appropriate error message. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_ERROR(LD)
#endif
               CASE_ERROR(D)
               CASE_ERROR(F)
               CASE_ERROR(I)

               case ( TYPE_L ): break;
               case ( TYPE_B ): break;
               case ( TYPE_S ): break;
               case ( TYPE_UL ): break;
               case ( TYPE_UI ): break;
               case ( TYPE_US ): break;
               case ( TYPE_UB ): break;
            }
            break;

/* Undefine the macro. */
#undef CASE_ERROR
      }
   }

/* Annul the PointSet used to hold output coordinates. */
   pset_out = astAnnul( pset_out );

/* Free the workspace. */
   offset = astFree( offset );
   stride = astFree( stride );
}

/*
*++
*  Name:
c     astRebinSeq<X>
f     AST_REBINSEQ<X>

*  Purpose:
*     Rebin a region of a sequence of data grids.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astRebinSeq<X>( AstMapping *this, double wlim, int ndim_in,
c                          const int lbnd_in[], const int ubnd_in[],
c                          const <Xtype> in[], const <Xtype> in_var[],
c                          int spread, const double params[], int flags,
c                          double tol, int maxpix, <Xtype> badval, 
c                          int ndim_out, const int lbnd_out[], 
c                          const int ubnd_out[], const int lbnd[], 
c                          const int ubnd[], <Xtype> out[], <Xtype> out_var[], 
c                          double weights[] );
f     CALL AST_REBINSEQ<X>( THIS, WLIM, NDIM_IN, LBND_IN, UBND_IN, IN, IN_VAR,
f                           SPREAD, PARAMS, FLAGS, TOL, MAXPIX, BADVAL,
f                           NDIM_OUT, LBND_OUT, UBND_OUT, LBND, UBND, OUT, 
f                           OUT_VAR, WEIGHTS, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
*     This set of 
c     functions is identical to astRebin<X>
f     routines is identical to AST_REBIN<X>
*     except that the rebinned input data is added into the supplied
*     output arrays, rather than simply over-writing the contents of the 
*     output arrays. Thus, by calling this 
c     function
f     routine
*     repeatedly, a sequence of input arrays can be rebinned and accumulated 
*     into a single output array, effectively forming a mosaic of the
*     input data arrays.
*
*     In addition, the weights associated with each output pixel are
*     returned. The weight of an output pixel indicates the number of input 
*     pixels which have been accumulated in that output pixel. If the entire
*     value of an input pixel is assigned to a single output pixel, then the 
*     weight of that output pixel is incremented by one. If some fraction of 
*     the value of an input pixel is assigned to an output pixel, then the 
*     weight of that output pixel is incremented by the fraction used.
*
*     The start of a new sequence is indicated by specifying the 
*     AST__REBININIT flag via the
c     "flags" parameter.
f     FLAGS argument.
*     This causes the supplied arrays to be filled with zeros before the 
*     rebinned input data is added into them. Subsequenct invocations
*     within the same sequence should omit the AST__REBININIT flag.
*
*     The last call in a sequence is indicated by specifying the AST__REBINEND 
*     flag. This causes the output data and variance arrays to be normalised 
*     before being returned. This normalisation consists of dividing the data 
*     array by the weights array, and can eliminate artifacts which may be 
*     introduced into the rebinned data as a consequence of aliasing between 
*     the input and output grids. However, it can also result in small changes to 
*     the total pixel value in any given area of the output array. In addition to
*     normalisation of the output data values, any output variances are also 
*     appropriately normalised, and any output data values with weight less 
*     than 
c     "wlim" are set to "badval".
f     WLIM are set to BADVAL.
*
*     An option is available to generate output variances based on the
*     spread of data values contributing to each output pixel (see the
*     AST__GENVAR flag).

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to a Mapping, whose forward transformation will be
*        used to transform the coordinates of pixels in the input
*        grid into the coordinate system of the output grid. 
*
*        The number of input coordinates used by this Mapping (as
*        given by its Nin attribute) should match the number of input
c        grid dimensions given by the value of "ndim_in"
f        grid dimensions given by the value of NDIM_IN
*        below. Similarly, the number of output coordinates (Nout
*        attribute) should match the number of output grid dimensions
c        given by "ndim_out".
f        given by NDIM_OUT.
c     wlim
f     WLIM = DOUBLE PRECISION (Given)
*        This value is only used if the AST__REBINEND flag is specified
*        via the 
c        "flags" parameter.
f        FLAGS argument.
*        It gives the required number of input pixel values which must 
*        contribute to an output pixel (i.e. the output pixel weight) in 
*        order for the output pixel value to be considered valid. If the sum 
*        of the input pixel weights contributing to an output pixel is less 
*        than the supplied
c        "wlim"
f        WLIM
*        value, then the output pixel value is returned set to the
*        supplied bad value. If the supplied value is less than 1.0E-10
*        then 1.0E-10 is used instead.
c     ndim_in
f     NDIM_IN = INTEGER (Given)
*        The number of dimensions in the input grid. This should be at
*        least one.
c     lbnd_in
f     LBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the input grid along each dimension.
c     ubnd_in
f     UBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the input grid along each dimension.
*
c        Note that "lbnd_in" and "ubnd_in" together define the shape
f        Note that LBND_IN and UBND_IN together define the shape
*        and size of the input grid, its extent along a particular
c        (j'th) dimension being ubnd_in[j]-lbnd_in[j]+1 (assuming the
c        index "j" to be zero-based). They also define
f        (J'th) dimension being UBND_IN(J)-LBND_IN(J)+1. They also define
*        the input grid's coordinate system, each pixel having unit
*        extent along each dimension with integral coordinate values
*        at its centre.
c     in
f     IN( * ) = <Xtype> (Given)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        input grid, containing the input data to be rebined.  The
*        numerical type of this array should match the 1- or
*        2-character type code appended to the function name (e.g. if
c        you are using astRebinSeqF, the type of each array element
c        should be "float").
f        you are using AST_REBINSEQR, the type of each array element
f        should be REAL).
*
*        The storage order of data within this array should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
c        If a NULL pointer is supplied for "in", then no data is added to
c        the output arrays, but any initialisation or normalisation
c        requested by "flags" is still performed.
c     in_var
f     IN_VAR( * ) = <Xtype> (Given)
c        An optional pointer to a second array with the same size and
c        type as the "in" array. If given, this should contain a set
c        of non-negative values which represent estimates of the
c        statistical variance associated with each element of the "in"
c        array. If this array is supplied (together with the
c        corresponding "out_var" array), then estimates of the
c        variance of the rebined output data will be calculated.
c
c        If no input variance estimates are being provided, a NULL
c        pointer should be given.
f        An optional second array with the same size and type as the
f        IN array. If the AST__USEVAR flag is set via the FLAGS
f        argument (below), this array should contain a set of
f        non-negative values which represent estimates of the
f        statistical variance associated with each element of the IN
f        array. Estimates of the variance of the rebined output data
f        will then be calculated.
f
f        If the AST__USEVAR flag is not set, no input variance
f        estimates are required and this array will not be used. A
f        dummy (e.g. one-element) array may then be supplied.
c     spread
f     SPREAD = INTEGER (Given)
c        This parameter specifies the scheme to be used for dividing
f        This argument specifies the scheme to be used for dividing
*        each input data value up amongst the corresponding output pixels.
*        It may be used to select
*        from a set of pre-defined schemes by supplying one of the
*        values described in the "Pixel Spreading Schemes"
*        section in the descrioption of the 
c        astRebin<X> functions.
f        AST_REBIN<X> routines.
*        If a value of zero is supplied, then the default linear spreading 
*        scheme is used (equivalent to supplying the value AST__LINEAR).
c     params
f     PARAMS( * ) = DOUBLE PRECISION (Given)
c        An optional pointer to an array of double which should contain
f        An optional array which should contain
*        any additional parameter values required by the pixel
*        spreading scheme. If such parameters are required, this
*        will be noted in the "Pixel Spreading Schemes" section in the 
*        descrioption of the 
c        astRebin<X> functions.
f        AST_REBIN<X> routines.
*
c        If no additional parameters are required, this array is not
c        used and a NULL pointer may be given.
f        If no additional parameters are required, this array is not
f        used. A dummy (e.g. one-element) array may then be supplied.
c     flags
f     FLAGS = INTEGER (Given)
c        The bitwise OR of a set of flag values which may be used to
f        The sum of a set of flag values which may be used to
*        provide additional control over the rebinning operation. See
*        the "Control Flags" section below for a description of the
*        options available.  If no flag values are to be set, a value
*        of zero should be given.
c     tol
f     TOL = DOUBLE PRECISION (Given)
*        The maximum tolerable geometrical distortion which may be
*        introduced as a result of approximating non-linear Mappings
*        by a set of piece-wise linear transformations. This should be
*        expressed as a displacement in pixels in the output grid's
*        coordinate system.
*
*        If piece-wise linear approximation is not required, a value
*        of zero may be given. This will ensure that the Mapping is
*        used without any approximation, but may increase execution
*        time.
*        
*        If the value is too high, discontinuities between the linear
*        approximations used in adjacent panel will be higher, and may
*        cause the edges of the panel to be visible when viewing the output 
*        image at high contrast. If this is a problem, reduce the
*        tolerance value used.
c     maxpix
f     MAXPIX = INTEGER (Given)
*        A value which specifies an initial scale size (in pixels) for
*        the adaptive algorithm which approximates non-linear Mappings
*        with piece-wise linear transformations. Normally, this should
*        be a large value (larger than any dimension of the region of
*        the input grid being used). In this case, a first attempt to
*        approximate the Mapping by a linear transformation will be
*        made over the entire input region.
*
*        If a smaller value is used, the input region will first be
c        divided into sub-regions whose size does not exceed "maxpix"
f        divided into sub-regions whose size does not exceed MAXPIX
*        pixels in any dimension. Only at this point will attempts at
*        approximation commence.
*
*        This value may occasionally be useful in preventing false
*        convergence of the adaptive algorithm in cases where the
*        Mapping appears approximately linear on large scales, but has
*        irregularities (e.g. holes) on smaller scales. A value of,
*        say, 50 to 100 pixels can also be employed as a safeguard in
*        general-purpose software, since the effect on performance is
*        minimal.
*
*        If too small a value is given, it will have the effect of
*        inhibiting linear approximation altogether (equivalent to
c        setting "tol" to zero). Although this may degrade
f        setting TOL to zero). Although this may degrade
*        performance, accurate results will still be obtained.
c     badval
f     BADVAL = <Xtype> (Given)
*        This argument should have the same type as the elements of
c        the "in" array. It specifies the value used to flag missing
f        the IN array. It specifies the value used to flag missing
*        data (bad pixels) in the input and output arrays.
*
c        If the AST__USEBAD flag is set via the "flags" parameter,
f        If the AST__USEBAD flag is set via the FLAGS argument,
c        then this value is used to test for bad pixels in the "in"
c        (and "in_var") array(s).
f        then this value is used to test for bad pixels in the IN
f        (and IN_VAR) array(s).
*
*        In all cases, this value is also used to flag any output
c        elements in the "out" (and "out_var") array(s) for which
f        elements in the OUT (and OUT_VAR) array(s) for which
*        rebined values could not be obtained (see the "Propagation
*        of Missing Data" section below for details of the
*        circumstances under which this may occur). 
c     ndim_out
f     NDIM_OUT = INTEGER (Given)
*        The number of dimensions in the output grid. This should be
*        at least one. It need not necessarily be equal to the number
*        of dimensions in the input grid.
c     lbnd_out
f     LBND_OUT( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the output grid along each dimension.
c     ubnd_out
f     UBND_OUT( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the output grid along each dimension.
*
c        Note that "lbnd_out" and "ubnd_out" together define the
f        Note that LBND_OUT and UBND_OUT together define the
*        shape, size and coordinate system of the output grid in the
c        same way as "lbnd_in" and "ubnd_in" define the shape, size
f        same way as LBND_IN and UBND_IN define the shape, size
*        and coordinate system of the input grid.
c     lbnd
f     LBND( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the first pixel in the region
*        of the input grid which is to be included in the rebined output
*        array.
c     ubnd
f     UBND( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the last pixel in the region of
*        the input grid which is to be included in the rebined output
*        array.
*
c        Note that "lbnd" and "ubnd" together define the shape and
f        Note that LBND and UBND together define the shape and
*        position of a (hyper-)rectangular region of the input grid
*        which is to be included in the rebined output array. This region
*        should lie wholly within the extent of the input grid (as
c        defined by the "lbnd_in" and "ubnd_in" arrays). Regions of
f        defined by the LBND_IN and UBND_IN arrays). Regions of
*        the input grid lying outside this region will not be used.
c     out
f     OUT( * ) = <Xtype> (Given and Returned)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        output grid. The rebined data values will be added into the
*        original contents of this array. The numerical type of this array 
*        should match that of the
c        "in" array, and the data storage order should be such
f        IN array, and the data storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
c     out_var
f     OUT_VAR( * ) = <Xtype> (Given and Returned)
*        An optional 
c        pointer to an 
*        array with the same type and size as the 
c        "out" 
f        OUT
*        array. If given, variance estimates for the rebined data values will 
*        be added into this array.
f        This array will only be used if the 
f        AST__USEVAR flag is set via the FLAGS argument.
*
*        The output variance values will be calculated on the
*        assumption that errors on the input data values are
*        statistically independent and that their variance estimates
*        may simply be summed (with appropriate weighting factors)
*        when several input pixels contribute to an output data
*        value. If this assumption is not valid, then the output error
*        estimates may be biased. In addition, note that the
*        statistical errors on neighbouring output data values (as
*        well as the estimates of those errors) may often be
*        correlated, even if the above assumption about the input data
*        is correct, because of the pixel spreading schemes
*        employed.
*
c        If no output variance estimates are required, a NULL pointer
c        should be given.
f        If the AST__USEVAR flag is not set, no output variance
f        estimates will be calculated and this array will not be
f        used. A dummy (e.g. one-element) array may then be supplied.
c     weights
f     WEIGHTS( * ) = DOUBLE PRECISION (Given and Returned)
c        Pointer to an array of double, 
f        An array 
*        with one or two elements for each pixel in the output grid,
*        depending on whether or not the AST__GENVAR flag has been supplied 
*        via the
c        "flags" parameter. 
f        FLAGS parameter.
*        If AST__GENVAR has not been specified then the array should have
*        one element for each output pixel, and it will be used to
*        accumulate the weight associated with each output pixel.
*        If AST__GENVAR has been specified then the array should have
*        two elements for each output pixel. The first half of the array
*        is again used to accumulate the weight associated with each output 
*        pixel, and the second half is used to accumulate the square of
*        the weights. In each half, the data storage order should be such that 
*        the index of the first grid dimension varies most rapidly and that of 
*        the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Data Type Codes:
*     To select the appropriate rebinning function, you should
c     replace <X> in the generic function name astRebinSeq<X> with a
f     replace <X> in the generic function name AST_REBINSEQ<X> with a
*     1- or 2-character data type code, so as to match the numerical
*     type <Xtype> of the data you are processing, as follows:
c     - D: double
c     - F: float
c     - I: int
f     - D: DOUBLE PRECISION
f     - R: REAL
f     - I: INTEGER
*
c     For example, astRebinSeqD would be used to process "double"
c     data, while astRebinSeqI would be used to process "int"
c     data, etc.
f     For example, AST_REBIND would be used to process DOUBLE
f     PRECISION data, while AST_REBINI would be used to process
f     integer data (stored in an INTEGER array), etc.
*
*     Note that, unlike 
c     astResample<X>, the astRebinSeq<X>
f     AST_RESAMPLE<X>, the AST_REBINSEQ<X>
*     set of functions does not yet support unsigned integer data types
*     or integers of different sizes.

*  Control Flags:
c     The following flags are defined in the "ast.h" header file and
f     The following flags are defined in the AST_PAR include file and
*     may be used to provide additional control over the rebinning
*     process. Having selected a set of flags, you should supply the
c     bitwise OR of their values via the "flags" parameter:
f     sum of their values via the FLAGS argument:
*
*     - AST__REBININIT: Used to mark the first call in a sequence. It indicates
*     that the supplied 
c     "out", "out_var" and "weights" 
f     OUT, OUT_VAR and WEIGHTS
*     arrays should be filled with zeros (thus over-writing any supplied
*     values) before adding the rebinned input data into them. This flag
*     should be used when rebinning the first input array in a sequence.
*     - AST__REBINEND: Used to mark the last call in a sequence. It causes
*     each value in the 
c     "out" and "out_var" 
f     OUT and OUT_VAR 
*     arrays to be divided by a normalisation factor before being 
*     returned. The normalisation factor for each output data value is just the 
*     corresponding value from the weights array. The normalisation factor 
*     for each output variance value is the square of the data value 
*     normalisation factor. It also causes output data values to be set
*     bad if the corresponding weight is less than the value supplied for
c     parameter "wlim".
f     argument WLIM.
*     It also causes any temporary values stored in the output variance array 
*     (see flag AST__GENVAR below) to be converted into usable variance values.
*     - AST__USEBAD: Indicates that there may be bad pixels in the
*     input array(s) which must be recognised by comparing with the
c     value given for "badval" and propagated to the output array(s).
f     value given for BADVAL and propagated to the output array(s).
*     If this flag is not set, all input values are treated literally
c     and the "badval" value is only used for flagging output array
f     and the BADVAL value is only used for flagging output array
*     values.
f     - AST__USEVAR: Indicates that variance information should be
f     processed in order to provide estimates of the statistical error
f     associated with the rebined values. If this flag is not set,
f     no variance processing will occur and the IN_VAR and OUT_VAR
f     arrays will not be used. (Note that this flag is only available
f     in the Fortran interface to AST.)
*     - AST__GENVAR: Indicates that the supplied output variance array should 
*     be used as a work array to accumulate the temporary values needed to 
*     generate output variances based on the spread of input data values 
*     contributing to each output pixel. If this flag is given, any supplied 
*     input variances are ignored. If the generation of such output variances 
*     is required, this flag should be used on every invocation of this
c     function
f     routine
*     within a sequence. In addition, the AST__REBINEND flag should be
*     given on the last invocation within the sequence (this causes the
*     temporary values accumulated in the variance array to be replaced with 
*     the final required variances).
*     - AST__VARWGT: Indicates that the input data should be weighted by
*     the reciprocal of the input variances. Otherwise, all input data are 
*     given equal weight.

*  Propagation of Missing Data:
*     Instances of missing data (bad pixels) in the output grid are
c     identified by occurrences of the "badval" value in the "out"
f     identified by occurrences of the BADVAL value in the OUT
*     array. These are only produced if the AST__REBINEND flag is
*     specified and a pixel has zero weight.
*
*     An input pixel is considered bad (and is consequently ignored) if 
*     its
c     data value is equal to "badval" and the AST__USEBAD flag is
c     set via the "flags" parameter.
f     data value is equal to BADVAL and the AST__USEBAD flag is
f     set via the FLAGS argument.
*
*     In addition, associated output variance estimates (if
c     calculated) may be declared bad and flagged with the "badval"
c     value in the "out_var" array for similar reasons.
f     calculated) may be declared bad and flagged with the BADVAL
f     value in the OUT_VAR array for similar reasons.
*--
*/
/* Define a macro to implement the function for a specific data
   type. */
#define MAKE_REBINSEQ(X,Xtype,IntType) \
static void RebinSeq##X( AstMapping *this, double wlim, int ndim_in, \
                     const int lbnd_in[], const int ubnd_in[], \
                     const Xtype in[], const Xtype in_var[], \
                     int spread, const double params[], int flags, \
                     double tol, int maxpix, Xtype badval, \
                     int ndim_out, const int lbnd_out[], \
                     const int ubnd_out[], const int lbnd[], \
                     const int ubnd[], Xtype out[], Xtype out_var[], \
                     double weights[] ) { \
\
/* Local Variables: */ \
   AstMapping *simple;           /* Pointer to simplified Mapping */ \
   Xtype *d;                     /* Pointer to next output data value */ \
   Xtype *v;                     /* Pointer to next output variance value */ \
   double *w;                    /* Pointer to next weight value */ \
   double a;                     /* Mean data value at output pixel */ \
   double sw;                    /* Sum of weights at output pixel */ \
   int i;                        /* Loop counter for output pixels */ \
   int idim;                     /* Loop counter for coordinate dimensions */ \
   int ipix_out;                 /* Index into output array */ \
   int nin;                      /* Number of Mapping input coordinates */ \
   int nout;                     /* Number of Mapping output coordinates */ \
   int npix;                     /* Number of pixels in input region */ \
   int npix_out;                 /* Number of pixels in output array */ \
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
/* Obtain values for the Nin and Nout attributes of the Mapping. */ \
   nin = astGetNin( this ); \
   nout = astGetNout( this ); \
\
/* If OK, check that the number of input grid dimensions matches the \
   number required by the Mapping and is at least 1. Report an error \
   if necessary. */ \
   if ( astOK && ( ( ndim_in != nin ) || ( ndim_in < 1 ) ) ) { \
      astError( AST__NGDIN, "astRebinSeq"#X"(%s): Bad number of input grid " \
                "dimensions (%d).", astGetClass( this ), ndim_in ); \
      if ( ndim_in != nin ) { \
         astError( AST__NGDIN, "The %s given requires %d coordinate value%s " \
                   "to specify an input position.", \
                   astGetClass( this ), nin, ( nin == 1 ) ? "" : "s" ); \
      } \
   } \
\
/* If OK, also check that the number of output grid dimensions matches \
   the number required by the Mapping and is at least 1. Report an \
   error if necessary. */ \
   if ( astOK && ( ( ndim_out != nout ) || ( ndim_out < 1 ) ) ) { \
      astError( AST__NGDIN, "astRebinSeq"#X"(%s): Bad number of output grid " \
                "dimensions (%d).", astGetClass( this ), ndim_out ); \
      if ( ndim_out != nout ) { \
         astError( AST__NGDIN, "The %s given generates %s%d coordinate " \
                   "value%s for each output position.", astGetClass( this ), \
                   ( nout < ndim_out ) ? "only " : "", nout, \
                   ( nout == 1 ) ? "" : "s" ); \
      } \
   } \
\
/* Check that the lower and upper bounds of the input grid are \
   consistent. Report an error if any pair is not. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_in; idim++ ) { \
         if ( lbnd_in[ idim ] > ubnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astRebinSeq"#X"(%s): Lower bound of " \
                      "input grid (%d) exceeds corresponding upper bound " \
                      "(%d).", astGetClass( this ), \
                      lbnd_in[ idim ], ubnd_in[ idim ] ); \
            astError( AST__GBDIN, "Error in input dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* Check that the positional accuracy tolerance supplied is valid and \
   report an error if necessary. */ \
   if ( astOK && ( tol < 0.0 ) ) { \
      astError( AST__PATIN, "astRebinSeq"#X"(%s): Invalid positional " \
                "accuracy tolerance (%.*g pixel).", \
                astGetClass( this ), DBL_DIG, tol ); \
      astError( AST__PATIN, "This value should not be less than zero." ); \
   } \
\
/* Check that the initial scale size in pixels supplied is valid and \
   report an error if necessary. */ \
   if ( astOK && ( maxpix < 0 ) ) { \
      astError( AST__SSPIN, "astRebinSeq"#X"(%s): Invalid initial scale " \
                "size in pixels (%d).", astGetClass( this ), maxpix ); \
      astError( AST__SSPIN, "This value should not be less than zero." ); \
   } \
\
/* Check that the lower and upper bounds of the output grid are \
   consistent. Report an error if any pair is not. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         if ( lbnd_out[ idim ] > ubnd_out[ idim ] ) { \
            astError( AST__GBDIN, "astRebinSeq"#X"(%s): Lower bound of " \
                      "output grid (%d) exceeds corresponding upper bound " \
                      "(%d).", astGetClass( this ), \
                      lbnd_out[ idim ], ubnd_out[ idim ] ); \
            astError( AST__GBDIN, "Error in output dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* Similarly check the bounds of the input region. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         if ( lbnd[ idim ] > ubnd[ idim ] ) { \
            astError( AST__GBDIN, "astRebinSeq"#X"(%s): Lower bound of " \
                      "input region (%d) exceeds corresponding upper " \
                      "bound (%d).", astGetClass( this ), \
                      lbnd[ idim ], ubnd[ idim ] ); \
\
/* Also check that the input region lies wholly within the input \
   grid. */ \
         } else if ( lbnd[ idim ] < lbnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astRebinSeq"#X"(%s): Lower bound of " \
                      "input region (%d) is less than corresponding " \
                      "bound of input grid (%d).", astGetClass( this ), \
                      lbnd[ idim ], lbnd_in[ idim ] ); \
         } else if ( ubnd[ idim ] > ubnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astRebinSeq"#X"(%s): Upper bound of " \
                      "input region (%d) exceeds corresponding " \
                      "bound of input grid (%d).", astGetClass( this ), \
                      ubnd[ idim ], ubnd_in[ idim ] ); \
         } \
\
/* Say which dimension produced the error. */ \
         if ( !astOK ) { \
            astError( AST__GBDIN, "Error in output dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* If OK, loop to determine how many input pixels are to be binned. */ \
   simple = NULL; \
   npix = 1; \
   npix_out = 1; \
   unsimplified_mapping = this; \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_in; idim++ ) { \
         npix *= ubnd[ idim ] - lbnd[ idim ] + 1; \
      } \
\
/* Loop to determine how many pixels the output array contains. */ \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         npix_out *= ubnd_out[ idim ] - lbnd_out[ idim ] + 1; \
      } \
\
/* If there are sufficient pixels to make it worthwhile, simplify the \
   Mapping supplied to improve performance. Otherwise, just clone the \
   Mapping pointer. Note we have already saved a pointer to the original \
   Mapping so that lower-level functions can use it if they need to report \
   an error. */ \
      if ( npix > 1024 ) { \
         simple = astSimplify( this ); \
      } else { \
         simple = astClone( this ); \
      } \
   } \
\
/* Report an error if the forward transformation of this simplified \
   Mapping is not defined. */ \
   if ( !astGetTranForward( simple ) && astOK ) { \
      astError( AST__TRNND, "astRebinSeq"#X"(%s): An forward coordinate " \
                "transformation is not defined by the %s supplied.", \
                astGetClass( unsimplified_mapping ), \
                astGetClass( unsimplified_mapping ) ); \
   } \
\
/* If required, initialise the output arrays to hold zeros. */ \
   if( flags & AST__REBININIT ) { \
      w = weights; \
      d = out; \
      if( out_var ) { \
         v = out_var; \
         for( ipix_out = 0; ipix_out < npix_out; ipix_out++, d++, v++, w++ ) { \
            *d = 0; \
            *v = 0; \
            *w = 0; \
         } \
      } else { \
         for( ipix_out = 0; ipix_out < npix_out; ipix_out++, d++, w++ ) { \
            *d = 0; \
            *w = 0; \
         } \
      } \
      if( flags & AST__GENVAR ) { \
         w = weights + npix_out; \
         for( ipix_out = 0; ipix_out < npix_out; ipix_out++, w++ ) *w = 0; \
      } \
   } \
\
/* Perform the rebinning. Note that we pass all gridded data, the \
   spread function and the bad pixel value by means of pointer \
   types that obscure the underlying data type. This is to avoid \
   having to replicate functions unnecessarily for each data \
   type. However, we also pass an argument that identifies the data \
   type we have obscured. */ \
   if( in ) { \
      RebinAdaptively( simple, ndim_in, lbnd_in, ubnd_in, \
                       (const void *) in, (const void *) in_var, \
                       TYPE_##X, spread, \
                       params, flags, tol, maxpix, \
                       (const void *) &badval, \
                       ndim_out, lbnd_out, ubnd_out, \
                       lbnd, ubnd, npix_out, \
                       (void *) out, (void *) out_var, weights ); \
   } \
\
/* If required, finalise the sequence. */ \
   if( flags & AST__REBINEND ) { \
\
/* Ensure "wlim" is not zero. */ \
      if( wlim < 1.0E-10 ) wlim = 1.0E-10; \
\
/* If required create the output variances from the spread of the input \
   data values.*/ \
      if( flags & AST__GENVAR ) { \
         for( i = 0; i < npix_out; i++ ) { \
            if( weights[ i ] >= wlim ) { \
               sw = weights[ i ]; \
               a = out[ i ]/sw; \
               out_var[ i ] = ( out_var[ i ]/sw - a*a )*weights[ i + npix_out ]; \
               if( out_var[ i ] < 0.0 ) out_var[ i ] = badval; \
            } else { \
               out_var[ i ] = badval; \
            } \
         } \
      } \
\
/* Normalise the returned data and variance arrays. */ \
      for( i = 0; i < npix_out; i++ ) { \
         if( weights[ i ] >= wlim && out[ i ] != badval ) { \
            out[ i ] /= weights[ i ]; \
         } else { \
            out[ i ] = badval; \
         } \
      } \
      if( out_var ) { \
         for( i = 0; i < npix_out; i++ ) { \
            if( weights[ i ] >= wlim && out_var[ i ] != badval ) { \
               out_var[ i ] /= ( weights[ i ]*weights[ i ] ); \
            } else { \
               out_var[ i ] = badval; \
            } \
         } \
      } \
   } \
\
/* Annul the pointer to the simplified/cloned Mapping. */ \
   simple = astAnnul( simple ); \
\
}

/* Expand the above macro to generate a function for each required
   data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_REBINSEQ(LD,long double,0)
#endif
MAKE_REBINSEQ(D,double,0)  
MAKE_REBINSEQ(F,float,0)
MAKE_REBINSEQ(I,int,1)

/* Undefine the macro. */
#undef MAKE_REBIN

static void RebinWithBlocking( AstMapping *this, const double *linear_fit,
                              int ndim_in,
                              const int *lbnd_in, const int *ubnd_in,
                              const void *in, const void *in_var,
                              DataType type, int spread,
                              const double *params, int flags,
                              const void *badval_ptr, int ndim_out,
                              const int *lbnd_out, const int *ubnd_out,
                              const int *lbnd, const int *ubnd, int npix_out,
                              void *out, void *out_var, double *work ) {
/*
*  Name:
*     RebinWithBlocking

*  Purpose:
*     Rebin a section of a data grid in a memory-efficient way.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void RebinWithBlocking( AstMapping *this, const double *linear_fit,
*                            int ndim_in,
*                            const int *lbnd_in, const int *ubnd_in,
*                            const void *in, const void *in_var,
*                            DataType type, int spread,
*                            const double *params, int flags,
*                            const void *badval_ptr, int ndim_out,
*                            const int *lbnd_out, const int *ubnd_out,
*                            const int *lbnd, const int *ubnd, int npix_out,
*                            void *out, void *out_var, double *work )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function rebins a specified section of a rectangular grid of 
*     data (with any number of dimensions) into another rectangular grid 
*     (with a possibly different number of dimensions). The coordinate 
*     transformation used to convert input pixel coordinates into positions 
*     in the output grid is given by the forward transformation of the 
*     Mapping which is supplied.  Any pixel spreading scheme may be specified 
*     for distributing the flux of an input pixel amongst the output
*     pixels.
*
*     This function is very similar to RebinSection, except that in
*     order to limit memory usage and to ensure locality of reference,
*     it divides the input grid up into "blocks" which have a limited
*     extent along each input dimension. Each block, which will not
*     contain more than a pre-determined maximum number of pixels, is
*     then passed to RebinSection for resampling.

*  Parameters:
*     this
*        Pointer to a Mapping, whose forward transformation may be
*        used to transform the coordinates of pixels in the input
*        grid into associated positions in the output grid.
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     linear_fit
*        Pointer to an optional array of double which contains the
*        coefficients of a linear fit which approximates the above
*        Mapping's forward coordinate transformation. If this is
*        supplied, it will be used in preference to the above Mapping
*        when transforming coordinates. This may be used to enhance
*        performance in cases where evaluation of the Mapping's
*        forward transformation is expensive. If no linear fit is
*        available, a NULL pointer should be supplied.
*
*        The way in which the fit coefficients are stored in this
*        array and the number of array elements are as defined by the
*        astLinearApprox function.
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     in
*        Pointer to the input array of data to be rebinned (with one
*        element for each pixel in the input grid). The numerical type
*        of these data should match the "type" value (below). The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and data type as the "in" array),
*        which represent estimates of the statistical variance
*        associated with each element of the "in" array. If this
*        second array is given (along with the corresponding "out_var"
*        array), then estimates of the variance of the rebinned data
*        will also be returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     type
*        A value taken from the "DataType" enum, which specifies the
*        data type of the input and output arrays containing the
*        gridded data (and variance) values.
*     spread
*        A value selected from a set of pre-defined macros to identify
*        which pixel spread function should be used.
*     params
*        Pointer to an optional array of parameters that may be passed
*        to the pixel spread algorithm, if required. If no parameters
*        are required, a NULL pointer should be supplied.
*     flags
*        The bitwise OR of a set of flag values which provide additional 
*        control over the resampling operation.
*     badval_ptr
*        If the AST__USEBAD flag is set (above), this parameter is a
*        pointer to a value which is used to identify bad data and/or
*        variance values in the input array(s). The referenced value's
*        data type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which rebinned values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output data grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output data grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output data grid in the same way as "lbnd_in"
*        and "ubnd_in" define the shape and size of the input grid
*        (see above).
*     lbnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the first pixel in the
*        section of the input data grid which is to be rebinned.
*     ubnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the last pixel in the
*        section of the input data grid which is to be rebinned.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the input grid which is to be rebinned. This section 
*        should lie wholly within the extent of the input grid (as defined 
*        by the "lbnd_out" and "ubnd_out" arrays). Regions of the input 
*        grid lying outside this section will be ignored.
*     npix_out
*        The number of pixels in the output array.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the rebinned data will be returned.  The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the rebinned values may be returned. This array will only be
*        used if the "in_var" array has been given.
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     work
*        An optional pointer to a double array with the same size as 
*        the "out" array. The contents of this array (if supplied) are
*        incremented by the accumulated weights assigned to each output pixel.
*        If no accumulated weights are required, a NULL pointer should be 
*        given.

*/

/* Local Constants: */
   const int mxpix = 2 * 1024;   /* Maximum number of pixels in a block (this
                                    relatively small number seems to give best
                                    performance) */

/* Local Variables: */
   int *dim_block;               /* Pointer to array of block dimensions */
   int *lbnd_block;              /* Pointer to block lower bound array */
   int *ubnd_block;              /* Pointer to block upper bound array */
   int dim;                      /* Dimension size */
   int done;                     /* All blocks rebinned? */
   int hilim;                    /* Upper limit on maximum block dimension */
   int idim;                     /* Loop counter for dimensions */
   int lolim;                    /* Lower limit on maximum block dimension */
   int mxdim_block;              /* Maximum block dimension */
   int npix;                     /* Number of pixels in block */

/* Check the global error status. */
   if ( !astOK ) return;

/* Allocate workspace. */
   lbnd_block = astMalloc( sizeof( int ) * (size_t) ndim_in );
   ubnd_block = astMalloc( sizeof( int ) * (size_t) ndim_in );
   dim_block = astMalloc( sizeof( int ) * (size_t) ndim_in );
   if ( astOK ) {

/* Find the optimum block size. */
/* ---------------------------- */
/* We first need to find the maximum extent which a block of input
   pixels may have in each dimension. We determine this by taking the
   input grid extent in each dimension and then limiting the maximum
   dimension size until the resulting number of pixels is sufficiently
   small. This approach allows the block shape to approximate (or
   match) the input grid shape when appropriate. */

/* First loop to calculate the total number of input pixels and the
   maximum input dimension size. */
      npix = 1;
      mxdim_block = 0;
      for ( idim = 0; idim < ndim_in; idim++ ) {
         dim = ubnd[ idim ] - lbnd[ idim ] + 1;
         npix *= dim;
         if ( mxdim_block < dim ) mxdim_block = dim;
      }

/* If the number of input pixels is too large for a single block, we
   perform iterations to determine the optimum upper limit on a
   block's dimension size. Initialise the limits on this result. */
      if ( npix > mxpix ) {
         lolim = 1;
         hilim = mxdim_block;

/* Loop to perform a binary chop, searching for the best result until
   the lower and upper limits on the result converge to adjacent
   values. */
         while ( ( hilim - lolim ) > 1 ) {

/* Form a new estimate from the mid-point of the previous limits. */
            mxdim_block = ( hilim + lolim ) / 2;

/* See how many pixels a block contains if its maximum dimension is
   limited to this new value. */
            for ( npix = 1, idim = 0; idim < ndim_in; idim++ ) {
               dim = ubnd[ idim ] - lbnd[ idim ] + 1;
               npix *= ( dim < mxdim_block ) ? dim : mxdim_block;
            }

/* Update the appropriate limit, according to whether the number of
   pixels is too large or too small. */
            *( ( npix <= mxpix ) ? &lolim : &hilim ) = mxdim_block;
         }

/* When iterations have converged, obtain the maximum limit on the
   dimension size of a block which results in no more than the maximum
   allowed number of pixels per block. However, ensure that all block
   dimensions are at least 2. */
            mxdim_block = lolim;
      }
      if ( mxdim_block < 2 ) mxdim_block = 2;

/* Calculate the block dimensions by applying this limit to the output
   grid dimensions. */
      for ( idim = 0; idim < ndim_in; idim++ ) {
         dim = ubnd[ idim ] - lbnd[ idim ] + 1;
         dim_block[ idim ] = ( dim < mxdim_block ) ? dim : mxdim_block;

/* Also initialise the lower and upper bounds of the first block of
   output grid pixels to be rebinned, ensuring that this does not
   extend outside the grid itself. */
         lbnd_block[ idim ] = lbnd[ idim ];
         ubnd_block[ idim ] = MinI( lbnd[ idim ] + dim_block[ idim ] - 1,
                                    ubnd[ idim ] );
      }

/* Rebin each block of input pixels. */
/* --------------------------------- */
/* Loop to generate the extent of each block of input pixels and to
   resample them. */
      done = 0;
      while ( !done && astOK ) {

/* Rebin the current block, accumulating the sum of bad pixels produced. */
         RebinSection( this, linear_fit,
                       ndim_in, lbnd_in, ubnd_in,
                       in, in_var, type, spread, params,
                       flags, badval_ptr,
                       ndim_out, lbnd_out, ubnd_out,
                       lbnd_block, ubnd_block, npix_out, out, out_var, work );

/* Update the block extent to identify the next block of input pixels. */
         idim = 0;
         do {

/* We find the least significant dimension where the upper bound of
   the block has not yet reached the upper bound of the region of the
   input grid which we are rebinning. The block's position is then
   incremented by one block extent along this dimension, checking that
   the resulting extent does not go outside the region being rebinned. */
            if ( ubnd_block[ idim ] < ubnd[ idim ] ) {
               lbnd_block[ idim ] = MinI( lbnd_block[ idim ] +
                                          dim_block[ idim ], ubnd[ idim ] );
               ubnd_block[ idim ] = MinI( lbnd_block[ idim ] +
                                          dim_block[ idim ] - 1,
                                          ubnd[ idim ] );
               break;

/* If any less significant dimensions are found where the upper bound
   of the block has reached its maximum value, we reset the block to
   its lowest position. */
            } else {
               lbnd_block[ idim ] = lbnd[ idim ];
               ubnd_block[ idim ] = MinI( lbnd[ idim ] + dim_block[ idim ] - 1,
                                          ubnd[ idim ] );

/* All the blocks have been processed once the position along the most
   significant dimension has been reset. */
               done = ( ++idim == ndim_in );
            }
         } while ( !done );
      }
   }

/* Free the workspace. */
   lbnd_block = astFree( lbnd_block );
   ubnd_block = astFree( ubnd_block );
   dim_block = astFree( dim_block );
}

static void ReportPoints( AstMapping *this, int forward,
                          AstPointSet *in_points, AstPointSet *out_points ) {
/*
*+
*  Name:
*     astReportPoints

*  Purpose:
*     Report the effect of transforming a set of points using a Mapping.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     void astReportPoints( AstMapping *this, int forward,
*                           AstPointSet *in_points, AstPointSet *out_points )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function reports the coordinates of a set of points before
*     and after being transformed by a Mapping, by writing them to
*     standard output.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     forward
*        A non-zero value indicates that the Mapping's forward
*        coordinate transformation has been applied, while a zero
*        value indicates the inverse transformation.
*     in_points
*        Pointer to a PointSet which is associated with the
*        coordinates of a set of points before the Mapping was
*        applied.
*     out_points
*        Pointer to a PointSet which is associated with the
*        coordinates of the same set of points after the Mapping has
*        been applied.

*  Notes:
*     - This method is provided as a development and debugging aid to
*     be invoked when coordinates are transformed by public Mapping
*     methods and under control of the "Report" Mapping attribute.
*     - Derived clases may over-ride this method in order to change
*     the way in which coordinates are formatted, etc.
*-
*/

/* Local Variables: */
   double **ptr_in;              /* Pointer to array of input data pointers */
   double **ptr_out;             /* Pointer to array of output data pointers */
   int coord;                    /* Loop counter for coordinates */
   int ncoord_in;                /* Number of input coordinates per point */
   int ncoord_out;               /* Number of output coordinates per point */
   int npoint;                   /* Number of points to report */
   int npoint_in;                /* Number of input points */
   int npoint_out;               /* Number of output points */
   int point;                    /* Loop counter for points */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain the numbers of points and coordinates associated with each
   PointSet. */ 
   npoint_in = astGetNpoint( in_points );
   npoint_out = astGetNpoint( out_points );
   ncoord_in = astGetNcoord( in_points );
   ncoord_out = astGetNcoord( out_points );

/* Obtain the pointers that give access to the coordinate data
   associated with each PointSet. */
   ptr_in = astGetPoints( in_points );
   ptr_out = astGetPoints( out_points );

/* In the event that both PointSets don't contain equal numbers of
   points (this shouldn't actually happen), simply use the minimum
   number. */
   npoint = ( npoint_in < npoint_out ) ? npoint_in : npoint_out;

/* Loop to report the effect of the Mapping on each point in turn. */
   for ( point = 0; point < npoint; point++ ) {

/* Report the input coordinates (in parentheses and separated by
   commas). Replace coordinate values of AST__BAD with the string
   "<bad>" to indicate missing values. */
      printf( "(" );
      for ( coord = 0; coord < ncoord_in; coord++ ) {
         if ( ptr_in[ coord ][ point ] == AST__BAD ) {
            printf( "%s<bad>", coord ? ", " : "" );
         } else {
            printf( "%s%.*g", coord ? ", " : "",
                              DBL_DIG, ptr_in[ coord ][ point ] );
         }
      }

/* Similarly report the output coordinates. */
      printf( ") --> (" );
      for ( coord = 0; coord < ncoord_out; coord++ ) {
         if ( ptr_out[ coord ][ point ] == AST__BAD ) {
            printf( "%s<bad>", coord ? ", " : "" );
         } else {
            printf( "%s%.*g", coord ? ", " : "",
                              DBL_DIG, ptr_out[ coord ][ point ] );
         }
      }
      printf( ")\n" );
   }
}

/*
*++
*  Name:
c     astResample<X>
f     AST_RESAMPLE<X>

*  Purpose:
*     Resample a region of a data grid.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     int astResample<X>( AstMapping *this, int ndim_in,
c                         const int lbnd_in[], const int ubnd_in[],
c                         const <Xtype> in[], const <Xtype> in_var[],
c                         int interp, void (* finterp)(),
c                         const double params[], int flags,
c                         double tol, int maxpix,
c                         <Xtype> badval, int ndim_out,
c                         const int lbnd_out[], const int ubnd_out[],
c                         const int lbnd[], const int ubnd[],
c                         <Xtype> out[], <Xtype> out_var[] );
f     RESULT = AST_RESAMPLE<X>( THIS, NDIM_IN, LBND_IN, UBND_IN, IN, IN_VAR,
f                               INTERP, FINTERP, PARAMS, FLAGS,
f                               TOL, MAXPIX, BADVAL,
f                               NDIM_OUT, LBND_OUT, UBND_OUT,
f                               LBND, UBND, OUT, OUT_VAR, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
*     This is a set of functions for resampling gridded data (e.g. an
*     image) under the control of a geometrical transformation, which
*     is specified by a Mapping.  The functions operate on a pair of
*     data grids (input and output), each of which may have any number
*     of dimensions. Resampling may be restricted to a specified
*     region of the output grid. An associated grid of error estimates
*     associated with the input data may also be supplied (in the form
*     of variance values), so as to produce error estimates for the
*     resampled output data. Propagation of missing data (bad pixels)
*     is supported.
*
*     You should use a resampling function which matches the numerical
*     type of the data you are processing by replacing <X> in
c     the generic function name astResample<X> by an appropriate 1- or
f     the generic function name AST_RESAMPLE<X> by an appropriate 1- or
*     2-character type code. For example, if you are resampling data
c     with type "float", you should use the function astResampleF (see
f     with type REAL, you should use the function AST_RESAMPLER (see
*     the "Data Type Codes" section below for the codes appropriate to
*     other numerical types).
*
*     Resampling of the grid of input data is performed by
*     transforming the coordinates of the centre of each output grid
*     element (or pixel) into the coordinate system of the input grid.
*     Since the resulting coordinates will not, in general, coincide
*     with the centre of an input pixel, sub-pixel interpolation is
*     performed between the neighbouring input pixels. This produces a
*     resampled value which is then assigned to the output pixel. A
*     choice of sub-pixel interpolation schemes is provided, but you
*     may also implement your own.
*
*     This algorithm samples the input data value, it does not integrate 
*     it. Thus total data value in the input image will not, in general,
*     be conserved. However, an option is provided (see the "Control Flags"
*     section below) which can produce approximate flux conservation by 
*     scaling the output values using the ratio of the output pixel size
*     to the input pixel size. However, if accurate flux conservation is
*     important to you, consder using the
c     astRebin<X> or astRebinSeq<X> family of functions
f     AST_REBIN<X> or AST_REBINSEQ<X> family of routines
*     instead.
*
*     Output pixel coordinates are transformed into the coordinate
*     system of the input grid using the inverse transformation of the
*     Mapping which is supplied. This means that geometrical features
*     in the input data are subjected to the Mapping's forward
*     transformation as they are transferred from the input to the
*     output grid (although the Mapping's forward transformation is
*     not explicitly used).
*
*     In practice, transforming the coordinates of every pixel of a
*     large data grid can be time-consuming, especially if the Mapping
*     involves complicated functions, such as sky projections. To
*     improve performance, it is therefore possible to approximate
*     non-linear Mappings by a set of linear transformations which are
*     applied piece-wise to separate sub-regions of the data. This
*     approximation process is applied automatically by an adaptive
*     algorithm, under control of an accuracy criterion which
*     expresses the maximum tolerable geometrical distortion which may
*     be introduced, as a fraction of a pixel.
*     
*     This algorithm first attempts to approximate the Mapping with a
*     linear transformation applied over the whole region of the
*     output grid which is being used. If this proves to be
*     insufficiently accurate, the output region is sub-divided into
*     two along its largest dimension and the process is repeated
*     within each of the resulting sub-regions. This process of
*     sub-division continues until a sufficiently good linear
*     approximation is found, or the region to which it is being
*     applied becomes too small (in which case the original Mapping is
*     used directly).

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to a Mapping, whose inverse transformation will be
*        used to transform the coordinates of pixels in the output
*        grid into the coordinate system of the input grid. This
*        yields the positions which are used to obtain resampled
*        values by sub-pixel interpolation within the input grid.
*
*        The number of input coordinates used by this Mapping (as
*        given by its Nin attribute) should match the number of input
c        grid dimensions given by the value of "ndim_in"
f        grid dimensions given by the value of NDIM_IN
*        below. Similarly, the number of output coordinates (Nout
*        attribute) should match the number of output grid dimensions
c        given by "ndim_out".
f        given by NDIM_OUT.
c     ndim_in
f     NDIM_IN = INTEGER (Given)
*        The number of dimensions in the input grid. This should be at
*        least one.
c     lbnd_in
f     LBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the input grid along each dimension.
c     ubnd_in
f     UBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the input grid along each dimension.
*
c        Note that "lbnd_in" and "ubnd_in" together define the shape
f        Note that LBND_IN and UBND_IN together define the shape
*        and size of the input grid, its extent along a particular
c        (j'th) dimension being ubnd_in[j]-lbnd_in[j]+1 (assuming the
c        index "j" to be zero-based). They also define
f        (J'th) dimension being UBND_IN(J)-LBND_IN(J)+1. They also define
*        the input grid's coordinate system, each pixel having unit
*        extent along each dimension with integral coordinate values
*        at its centre.
c     in
f     IN( * ) = <Xtype> (Given)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        input grid, containing the input data to be resampled.  The
*        numerical type of this array should match the 1- or
*        2-character type code appended to the function name (e.g. if
c        you are using astResampleF, the type of each array element
c        should be "float").
f        you are using AST_RESAMPLER, the type of each array element
f        should be REAL).
*
*        The storage order of data within this array should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
c     in_var
f     IN_VAR( * ) = <Xtype> (Given)
c        An optional pointer to a second array with the same size and
c        type as the "in" array. If given, this should contain a set
c        of non-negative values which represent estimates of the
c        statistical variance associated with each element of the "in"
c        array. If this array is supplied (together with the
c        corresponding "out_var" array), then estimates of the
c        variance of the resampled output data will be calculated.
c
c        If no input variance estimates are being provided, a NULL
c        pointer should be given.
f        An optional second array with the same size and type as the
f        IN array. If the AST__USEVAR flag is set via the FLAGS
f        argument (below), this array should contain a set of
f        non-negative values which represent estimates of the
f        statistical variance associated with each element of the IN
f        array. Estimates of the variance of the resampled output data
f        will then be calculated.
f
f        If the AST__USEVAR flag is not set, no input variance
f        estimates are required and this array will not be used. A
f        dummy (e.g. one-element) array may then be supplied.
c     interp
f     INTERP = INTEGER (Given)
c        This parameter specifies the scheme to be used for sub-pixel
f        This argument specifies the scheme to be used for sub-pixel
*        interpolation within the input grid. It may be used to select
*        from a set of pre-defined schemes by supplying one of the
*        values described in the "Sub-Pixel Interpolation Schemes"
*        section below.  If a value of zero is supplied, then the
*        default linear interpolation scheme is used (equivalent to
*        supplying the value AST__LINEAR).
*
*        Alternatively, you may supply a value which indicates that
c        you will provide your own function to perform sub-pixel
c        interpolation by means of the "finterp " parameter. Again, see
f        you will provide your own routine to perform sub-pixel
f        interpolation by means of the FINTERP argument. Again, see
*        the "Sub-Pixel Interpolation Schemes" section below for
*        details.
c     finterp
f     FINTERP = SUBROUTINE (Given)
c        If the value given for the "interp" parameter indicates that
c        you will provide your own function for sub-pixel
c        interpolation, then a pointer to that function should be
c        given here. For details of the interface which the function
c        should have (several are possible, depending on the value of
c        "interp"), see the "Sub-Pixel Interpolation Schemes" section
c        below.
f        If the value given for the INTERP argument indicates that you
f        will provide your own routine for sub-pixel interpolation,
f        then the name of that routine should be given here (the name
f        should also appear in a Fortran EXTERNAL statement in the
f        routine which invokes AST_RESAMPLE<X>). For details of the
f        interface which the routine should have (several are
f        possible, depending on the value of INTERP), see the
f        "Sub-Pixel Interpolation Schemes" section below.
*
c        If the "interp" parameter has any other value, corresponding
c        to one of the pre-defined interpolation schemes, then this
c        function will not be used and you may supply a NULL pointer.
f        If the INTERP argument has any other value, corresponding to
f        one of the pre-defined interpolation schemes, then this
f        routine will not be used and you may supply the null routine
f        AST_NULL here (note only one underscore).  No EXTERNAL
f        statement is required for this routine, so long as the AST_PAR
f        include file has been used.
c     params
f     PARAMS( * ) = DOUBLE PRECISION (Given)
c        An optional pointer to an array of double which should contain
f        An optional array which should contain
*        any additional parameter values required by the sub-pixel
*        interpolation scheme. If such parameters are required, this
*        will be noted in the "Sub-Pixel Interpolation Schemes"
c        section below (you may also use this array to pass values
c        to your own interpolation function).
f        section below (you may also use this array to pass values
f        to your own interpolation routine).
*
c        If no additional parameters are required, this array is not
c        used and a NULL pointer may be given.
f        If no additional parameters are required, this array is not
f        used. A dummy (e.g. one-element) array may then be supplied.
c     flags
f     FLAGS = INTEGER (Given)
c        The bitwise OR of a set of flag values which may be used to
f        The sum of a set of flag values which may be used to
*        provide additional control over the resampling operation. See
*        the "Control Flags" section below for a description of the
*        options available.  If no flag values are to be set, a value
*        of zero should be given.
c     tol
f     TOL = DOUBLE PRECISION (Given)
*        The maximum tolerable geometrical distortion which may be
*        introduced as a result of approximating non-linear Mappings
*        by a set of piece-wise linear transformations. This should be
*        expressed as a displacement in pixels in the input grid's
*        coordinate system.
*
*        If piece-wise linear approximation is not required, a value
*        of zero may be given. This will ensure that the Mapping is
*        used without any approximation, but may increase execution
*        time.
c     maxpix
f     MAXPIX = INTEGER (Given)
*        A value which specifies an initial scale size (in pixels) for
*        the adaptive algorithm which approximates non-linear Mappings
*        with piece-wise linear transformations. Normally, this should
*        be a large value (larger than any dimension of the region of
*        the output grid being used). In this case, a first attempt to
*        approximate the Mapping by a linear transformation will be
*        made over the entire output region.
*
*        If a smaller value is used, the output region will first be
c        divided into sub-regions whose size does not exceed "maxpix"
f        divided into sub-regions whose size does not exceed MAXPIX
*        pixels in any dimension. Only at this point will attempts at
*        approximation commence.
*
*        This value may occasionally be useful in preventing false
*        convergence of the adaptive algorithm in cases where the
*        Mapping appears approximately linear on large scales, but has
*        irregularities (e.g. holes) on smaller scales. A value of,
*        say, 50 to 100 pixels can also be employed as a safeguard in
*        general-purpose software, since the effect on performance is
*        minimal.
*
*        If too small a value is given, it will have the effect of
*        inhibiting linear approximation altogether (equivalent to
c        setting "tol" to zero). Although this may degrade
f        setting TOL to zero). Although this may degrade
*        performance, accurate results will still be obtained.
c     badval
f     BADVAL = <Xtype> (Given)
*        This argument should have the same type as the elements of
c        the "in" array. It specifies the value used to flag missing
f        the IN array. It specifies the value used to flag missing
*        data (bad pixels) in the input and output arrays.
*
c        If the AST__USEBAD flag is set via the "flags" parameter,
f        If the AST__USEBAD flag is set via the FLAGS argument,
c        then this value is used to test for bad pixels in the "in"
c        (and "in_var") array(s).
f        then this value is used to test for bad pixels in the IN
f        (and IN_VAR) array(s).
*
*        In all cases, this value is also used to flag any output
c        elements in the "out" (and "out_var") array(s) for which
f        elements in the OUT (and OUT_VAR) array(s) for which
*        resampled values could not be obtained (see the "Propagation
*        of Missing Data" section below for details of the
c        circumstances under which this may occur). The astResample<X>
f        circumstances under which this may occur). The AST_RESAMPLE<X>
*        function return value indicates whether any such values have
*        been produced.
c     ndim_out
f     NDIM_OUT = INTEGER (Given)
*        The number of dimensions in the output grid. This should be
*        at least one. It need not necessarily be equal to the number
*        of dimensions in the input grid.
c     lbnd_out
f     LBND_OUT( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the output grid along each dimension.
c     ubnd_out
f     UBND_OUT( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the output grid along each dimension.
*
c        Note that "lbnd_out" and "ubnd_out" together define the
f        Note that LBND_OUT and UBND_OUT together define the
*        shape, size and coordinate system of the output grid in the
c        same way as "lbnd_in" and "ubnd_in" define the shape, size
f        same way as LBND_IN and UBND_IN define the shape, size
*        and coordinate system of the input grid.
c     lbnd
f     LBND( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the first pixel in the region
*        of the output grid for which a resampled value is to be
*        calculated.
c     ubnd
f     UBND( NDIM_OUT ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_out" elements,
f        An array
*        containing the coordinates of the last pixel in the region of
*        the output grid for which a resampled value is to be
*        calculated.
*
c        Note that "lbnd" and "ubnd" together define the shape and
f        Note that LBND and UBND together define the shape and
*        position of a (hyper-)rectangular region of the output grid
*        for which resampled values should be produced. This region
*        should lie wholly within the extent of the output grid (as
c        defined by the "lbnd_out" and "ubnd_out" arrays). Regions of
f        defined by the LBND_OUT and UBND_OUT arrays). Regions of
*        the output grid lying outside this region will not be
*        modified.
c     out
f     OUT( * ) = <Xtype> (Returned)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        output grid, into which the resampled data values will be
*        returned. The numerical type of this array should match that
c        of the "in" array, and the data storage order should be such
f        of the IN array, and the data storage order should be such
*        that the index of the first grid dimension varies most
*        rapidly and that of the final dimension least rapidly
c        (i.e. Fortran array indexing is used).
f        (i.e. normal Fortran array storage order).
c     out_var
f     OUT_VAR( * ) = <Xtype> (Returned)
c        An optional pointer to an array with the same type and size
c        as the "out" array. If given, this array will be used to
c        return variance estimates for the resampled data values. This
c        array will only be used if the "in_var" array has also been
c        supplied.
f        An optional array with the same type and size as the OUT
f        array. If the AST__USEVAR flag is set via the FLAGS argument,
f        this array will be used to return variance estimates for the
f        resampled data values.
*
*        The output variance values will be calculated on the
*        assumption that errors on the input data values are
*        statistically independent and that their variance estimates
*        may simply be summed (with appropriate weighting factors)
*        when several input pixels contribute to an output data
*        value. If this assumption is not valid, then the output error
*        estimates may be biased. In addition, note that the
*        statistical errors on neighbouring output data values (as
*        well as the estimates of those errors) may often be
*        correlated, even if the above assumption about the input data
*        is correct, because of the sub-pixel interpolation schemes
*        employed.
*
c        If no output variance estimates are required, a NULL pointer
c        should be given.
f        If the AST__USEVAR flag is not set, no output variance
f        estimates will be calculated and this array will not be
f        used. A dummy (e.g. one-element) array may then be supplied.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Returned Value:
c     astResample<X>()
f     AST_RESAMPLE<X> = INTEGER
*        The number of output pixels to which a data value (or a
c        variance value if relevant) equal to "badval" has been
f        variance value if relevant) equal to BADVAL has been
*        assigned because no valid resampled value could be obtained.
*        Thus, in the absence of any error, a returned value of zero
*        indicates that all the required output pixels received valid
*        resampled data values (and variances).

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.

*  Data Type Codes:
*     To select the appropriate resampling function, you should
c     replace <X> in the generic function name astResample<X> with a
f     replace <X> in the generic function name AST_RESAMPLE<X> with a
*     1- or 2-character data type code, so as to match the numerical
*     type <Xtype> of the data you are processing, as follows:
c     - D: double
c     - F: float
c     - L: long int
c     - UL: unsigned long int
c     - I: int
c     - UI: unsigned int
c     - S: short int
c     - US: unsigned short int
c     - B: byte (signed char)
c     - UB: unsigned byte (unsigned char)
f     - D: DOUBLE PRECISION
f     - R: REAL
f     - I: INTEGER
f     - UI: INTEGER (treated as unsigned)
f     - S: INTEGER*2 (short integer)
f     - US: INTEGER*2 (short integer, treated as unsigned)
f     - B: BYTE (treated as signed)
f     - UB: BYTE (treated as unsigned)
*
c     For example, astResampleD would be used to process "double"
c     data, while astResampleS would be used to process "short int"
c     data, etc.
f     For example, AST_RESAMPLED would be used to process DOUBLE
f     PRECISION data, while AST_RESAMPLES would be used to process
f     short integer data (stored in an INTEGER*2 array), etc.
f
f     For compatibility with other Starlink facilities, the codes W
f     and UW are provided as synonyms for S and US respectively (but
f     only in the Fortran interface to AST).

*  Sub-Pixel Interpolation Schemes:
*     There is no such thing as a perfect sub-pixel interpolation
*     scheme and, in practice, all resampling will result in some
*     degradation of gridded data.  A range of schemes is therefore
*     provided, from which you can choose the one which best suits
*     your needs.
*
*     In general, a balance must be struck between schemes which tend
*     to degrade sharp features in the data by smoothing them, and
*     those which attempt to preserve sharp features. The latter will
*     often tend to introduce unwanted oscillations, typically visible
*     as "ringing" around sharp features and edges, especially if the
*     data are under-sampled (i.e. if the sharpest features are less
*     than about two pixels across). In practice, a good interpolation
*     scheme is likely to be a compromise and may exhibit some aspects
*     of both these features.
*
*     For under-sampled data, some interpolation schemes may appear to
*     preserve data resolution because they transform single input
*     pixels into single output pixels, rather than spreading their
*     data between several output pixels. While this may look
*     better cosmetically, it can result in a geometrical shift of
*     sharp features in the data. You should beware of this if you
*     plan to use such features (e.g.) for image alignment.
*
*     The following are two easy-to-use sub-pixel interpolation
*     schemes which are generally applicable. They are selected by
c     supplying the appropriate value (defined in the "ast.h" header
c     file) via the "interp" parameter. In these cases, the "finterp"
c     and "params" parameters are not used:
f     supplying the appropriate value (defined in the AST_PAR include
f     file) via the INTERP argument. In these cases, the FINTERP
f     and PARAMS arguments are not used:
*
*     - AST__NEAREST: This is the simplest possible scheme, in which
*     the value of the input pixel with the nearest centre to the
*     interpolation point is used. This is very quick to execute and
*     will preserve single-pixel features in the data, but may
*     displace them by up to half their width along each dimension. It
*     often gives a good cosmetic result, so is useful for quick-look
*     processing, but is unsuitable if accurate geometrical
*     transformation is required.
*     - AST__LINEAR: This is the default scheme, which uses linear
*     interpolation between the nearest neighbouring pixels in the
*     input grid (there are two neighbours in one dimension, four
*     neighbours in two dimensions, eight in three dimensions,
*     etc.). It is superior to the nearest-pixel scheme (above) in not
*     displacing features in the data, yet it still executes fairly
*     rapidly. It is generally a safe choice if you do not have any
*     particular reason to favour another scheme, since it cannot
*     introduce oscillations. However, it does introduce some spatial
*     smoothing which varies according to the distance of the
*     interpolation point from the neighbouring pixels. This can
*     degrade the shape of sharp features in the data in a
*     position-dependent way. It may also show in the output variance
*     grid (if used) as a pattern of stripes or fringes.
*
*     An alternative set of interpolation schemes is based on forming
*     the interpolated value from the weighted sum of a set of
*     surrounding pixel values (not necessarily just the nearest
*     neighbours). This approach has its origins in the theory of
*     digital filtering, in which interpolated values are obtained by
*     conceptually passing the sampled data (represented by a grid of
*     delta functions) through a linear filter which implements a
*     convolution. Because the convolution kernel is continuous, the
*     convolution yields a continuous function which may then be
*     evaluated at fractional pixel positions. The (possibly
*     multi-dimensional) kernel is usually regarded as "separable" and
*     formed from the product of a set of identical 1-dimensional
*     kernel functions, evaluated along each dimension. Different
*     interpolation schemes are then distinguished by the choice of
*     this 1-dimensional interpolation kernel. The number of
*     surrounding pixels which contribute to the result may also be
*     varied.
*
*     From a practical standpoint, it is useful to divide the weighted
*     sum of pixel values by the sum of the weights when determining
*     the interpolated value.  Strictly, this means that a true
*     convolution is no longer being performed. However, the
*     distinction is rarely important in practice because (for
*     slightly subtle reasons) the sum of weights is always
*     approximately constant for good interpolation kernels. The
*     advantage of this technique, which is used here, is that it can
*     easily accommodate missing data and tends to minimise unwanted
*     oscillations at the edges of the data grid.
*
*     In the following schemes, which are based on a 1-dimensional
c     interpolation kernel, the first element of the "params" array
f     interpolation kernel, the first element of the PARAMS array
*     should be used to specify how many pixels are to contribute to the
*     interpolated result on either side of the interpolation point in
*     each dimension (the nearest integer value is used). Execution time
*     increases rapidly with this number. Typically, a value of 2 is
*     appropriate and the minimum value used will be 1 (i.e. two pixels
*     altogether, one on either side of the interpolation point).
c     A value of zero or less may be given for "params[0]"
f     A value of zero or less may be given for PARAMS(1)
*     to indicate that a suitable number of pixels should be calculated
*     automatically.
*
c     In each of these cases, the "finterp" parameter is not used:
f     In each of these cases, the FINTERP argument is not used:
*
*     - AST__SINC: This scheme uses a sinc(pi*x) kernel, where x is the
*     pixel offset from the interpolation point and sinc(z)=sin(z)/z. This
*     sometimes features as an "optimal" interpolation kernel in books on
*     image processing. Its supposed optimality depends on the assumption
*     that the data are band-limited (i.e. have no spatial frequencies above
*     a certain value) and are adequately sampled. In practice, astronomical
*     data rarely meet these requirements. In addition, high spatial
*     frequencies are often present due (e.g.) to image defects and cosmic
*     ray events. Consequently, substantial ringing can be experienced with
*     this kernel. The kernel also decays slowly with distance, so that
*     many surrounding pixels are required, leading to poor performance.
*     Abruptly truncating it, by using only a few neighbouring pixels,
c     improves performance and may reduce ringing (if "params[0]" is set to
f     improves performance and may reduce ringing (if PARAMS(1) is set to
*     zero, then only two pixels will be used on either side). However, a
*     more gradual truncation, as implemented by other kernels, is generally
*     to be preferred. This kernel is provided mainly so that you can
*     convince yourself not to use it!
*     - AST__SINCSINC: This scheme uses an improved kernel, of the form
*     sinc(pi*x).sinc(k*pi*x), with k a constant, out to the point where
*     sinc(k*pi*x) goes to zero, and zero beyond. The second sinc() factor
*     provides an "envelope" which gradually rolls off the normal sinc(pi*x)
*     kernel at large offsets. The width of this envelope is specified by
*     giving the number of pixels offset at which it goes to zero by means
c     of the "params[1]" value, which should be at least 1.0 (in addition,
c     setting "params[0]" to zero will select the number of contributing
f     of the PARAMS(2) value, which should be at least 1.0 (in addition,
f     setting PARAMS(1) to zero will select the number of contributing
*     pixels so as to utilise the full width of the kernel, out to where it
c     reaches zero). The case given by "params[0]=2, params[1]=2" is typically
f     reaches zero). The case given by PARAMS(1)=2, PARAMS(2)=2 is typically
*     a good choice and is sometimes known as the Lanczos kernel. This is a
*     valuable general-purpose interpolation scheme, intermediate in its
*     visual effect on images between the AST__NEAREST and AST__LINEAR
*     schemes. Although the kernel is slightly oscillatory, ringing is
*     adequately suppressed if the data are well sampled.
*     - AST__SINCCOS: This scheme uses a kernel of the form
*     sinc(pi*x).cos(k*pi*x), with k a constant, out to the point where
*     cos(k*pi*x) goes to zero, and zero beyond. As above, the cos() factor
*     provides an envelope which gradually rolls off the sinc() kernel
*     at large offsets. The width of this envelope is specified by giving
*     the number of pixels offset at which it goes to zero by means
c     of the "params[1]" value, which should be at least 1.0 (in addition,
c     setting "params[0]" to zero will select the number of contributing
f     of the PARAMS(2) value, which should be at least 1.0 (in addition,
f     setting PARAMS(1) to zero will select the number of contributing
*     pixels so as to utilise the full width of the kernel, out to where it
*     reaches zero). This scheme gives similar results to the
*     AST__SINCSINC scheme, which it resembles.
*     - AST__SINCGAUSS: This scheme uses a kernel of the form
*     sinc(pi*x).exp(-k*x*x), with k a positive constant. Here, the sinc()
*     kernel is rolled off using a Gaussian envelope which is specified by
c     giving its full-width at half-maximum (FWHM) by means of the "params[1]"
c     value, which should be at least 0.1 (in addition, setting "params[0]"
f     giving its full-width at half-maximum (FWHM) by means of the PARAMS(2)
f     value, which should be at least 0.1 (in addition, setting PARAMS(1)
*     to zero will select the number of contributing pixels so as to utilise
*     the width of the kernel out to where the envelope declines to 1% of its
*     maximum value). On astronomical images and spectra, good results are
*     often obtained by approximately matching the FWHM of the
c     envelope function, given by "params[1]", to the point spread function
f     envelope function, given by PARAMS(2), to the point spread function
*     of the input data. However, there does not seem to be any theoretical
*     reason for this.
*     - AST__SOMB: This scheme uses a somb(pi*x) kernel (a "sombrero"
*     function), where x is the pixel offset from the interpolation point 
*     and somb(z)=2*J1(z)/z  (J1 is a Bessel function of the first kind of
*     order 1). It is similar to the AST__SINC kernel, and has the same 
*     parameter usage.
*     - AST__SOMBCOS: This scheme uses a kernel of the form 
*     somb(pi*x).cos(k*pi*x), with k a constant, out to the point where
*     cos(k*pi*x) goes to zero, and zero beyond. It is similar to the 
*     AST__SINCCOS kernel, and has the same parameter usage.
*
*     In addition, the following schemes are provided which are not based
*     on a 1-dimensional kernel:
*
*     - AST__BLOCKAVE: This scheme simply takes an average of all the
*     pixels on the input grid in a cube centred on the interpolation
*     point.  The number of pixels in the cube is determined by the
c     value of the first element of the "params" array, which gives
f     value of the first element of the PARAMS array, which gives
*     the number of pixels in each dimension on either side of the
c     central point.  Hence a block of (2 * params[0])^ndim_in
f     central point.  Hence a block of (2 * PARAMS(1))**NDIM_IN
*     pixels in the input grid will be examined to determine the
*     value of the output pixel.  If the variance is not being used
c     (var_in or var_out = NULL) then all valid pixels in this cube
f     (USEVAR = .FALSE.) then all valid pixels in this cube
*     will be averaged in to the result with equal weight.
*     If variances are being used, then each input pixel will be 
*     weighted proportionally to the reciprocal of its variance; any
*     pixel without a valid variance will be discarded.  This scheme
*     is suitable where the output grid is much coarser than the 
*     input grid; if the ratio of pixel sizes is R then a suitable
c     value of params[0] may be R/2.
f     value of PARAMS(1) may be R/2.
*
c     Finally, supplying the following values for "interp" allows you
c     to implement your own sub-pixel interpolation scheme by means of
c     your own function. You should supply a pointer to this function
c     via the "finterp" parameter:
f     Finally, supplying the following values for INTERP allows you to
f     implement your own sub-pixel interpolation scheme by means of
f     your own routine. You should supply the name of this routine via
f     the FINTERP argument:
*
c     - AST__UKERN1: In this scheme, you supply a function to evaluate
c     your own 1-dimensional interpolation kernel, which is then used
c     to perform sub-pixel interpolation (as described above). The
c     function you supply should have the same interface as the
c     fictitious astUkern1 function (q.v.).  In addition, a value
c     should be given via "params[0]" to specify the number of
c     neighbouring pixels which are to contribute to each interpolated
c     value (in the same way as for the pre-defined interpolation
c     schemes described above). Other elements of the "params" array
c     are available to pass values to your interpolation function.
f     - AST__UKERN1: In this scheme, you supply a routine to evaluate
f     your own 1-dimensional interpolation kernel, which is then used
f     to perform sub-pixel interpolation (as described above). The
f     routine you supply should have the same interface as the
f     fictitious AST_UKERN1 routine (q.v.).  In addition, a value
f     should be given via PARAMS(1) to specify the number of
f     neighbouring pixels which are to contribute to each interpolated
f     value (in the same way as for the pre-defined interpolation
f     schemes described above). Other elements of the PARAMS array
f     are available to pass values to your interpolation routine.
*
c     - AST__UINTERP: This is a completely general scheme, in which
c     your interpolation function has access to all of the input
c     data. This allows you to implement any interpolation algorithm
c     you choose, which could (for example) be non-linear, or
c     adaptive. In this case, the astResample<X> functions play no
c     role in the sub-pixel interpolation process and simply handle
c     the geometrical transformation of coordinates and other
c     housekeeping. The function you supply should have the same
c     interface as the fictitious astUinterp function (q.v.). In this
c     case, the "params" parameter is not used by astResample<X>, but
c     is available to pass values to your interpolation function.
f     - AST__UINTERP: This is a completely general scheme, in which
f     your interpolation routine has access to all of the input
f     data. This allows you to implement any interpolation algorithm
f     you choose, which could (for example) be non-linear, or
f     adaptive. In this case, the AST_RESAMPLE<X> functions play no
f     role in the sub-pixel interpolation process and simply handle
f     the geometrical transformation of coordinates and other
f     housekeeping. The routine you supply should have the same
f     interface as the fictitious AST_UINTERP routine (q.v.). In this
f     case, the PARAMS argument is not used by AST_RESAMPLE<X>, but
f     is available to pass values to your interpolation routine.

*  Control Flags:
c     The following flags are defined in the "ast.h" header file and
f     The following flags are defined in the AST_PAR include file and
*     may be used to provide additional control over the resampling
*     process. Having selected a set of flags, you should supply the
c     bitwise OR of their values via the "flags" parameter:
f     sum of their values via the FLAGS argument:
*
*     - AST__URESAMP1, 2, 3 & 4: A set of four flags which are
*     reserved for your own use. They may be used to pass private
c     information to any sub-pixel interpolation function which you
f     information to any sub-pixel interpolation routine which you
*     implement yourself. They are ignored by all the pre-defined
*     interpolation schemes.
*     - AST__USEBAD: Indicates that there may be bad pixels in the
*     input array(s) which must be recognised by comparing with the
c     value given for "badval" and propagated to the output array(s).
f     value given for BADVAL and propagated to the output array(s).
*     If this flag is not set, all input values are treated literally
c     and the "badval" value is only used for flagging output array
f     and the BADVAL value is only used for flagging output array
*     values.
f     - AST__USEVAR: Indicates that variance information should be
f     processed in order to provide estimates of the statistical error
f     associated with the resampled values. If this flag is not set,
f     no variance processing will occur and the IN_VAR and OUT_VAR
f     arrays will not be used. (Note that this flag is only available
f     in the Fortran interface to AST.)
*     - AST__CONSERVEFLUX: Indicates that the output pixel values should
*     be scaled in such a way as to preserve (approximately) the total data 
*     value in a feature on the sky. Without this flag, each output pixel 
*     value represents an instantaneous sample of the input data values at 
*     the corresponding input position. This is appropriate if the input
*     data represents the spatial density of some quantity (e.g. surface
*     brightness in Janskys per square arc-second) because the output
*     pixel values will have the same normalisation and units as the
*     input pixel values. However, if the input data values represent
*     flux (or some other physical quantity) per pixel, then the 
*     AST__CONSERVEFLUX flag could be used. This causes each output
*     pixel value to be scaled by the ratio of the output pixel size to
*     the input pixel size. 
*
*     This flag can only be used if the Mapping is succesfully approximated 
*     by one or more linear transformations. Thus an error will be reported 
*     if it used when the 
c     "tol" parameter
f     TOL argument
*     is set to zero (which stops the use of linear approximations), or
*     if the Mapping is too non-linear to be approximated by a piece-wise
*     linear transformation. The ratio of output to input pixel size is 
*     evaluated once for each panel of the piece-wise linear approximation to 
*     the Mapping, and is assumed to be constant for all output pixels in the 
*     panel. The scaling factors for adjacent panels will in general
*     differ slightly, and so the joints between panels may be visible when 
*     viewing the output image at high contrast. If this is a problem,
*     reduce the value of the 
c     "tol" parameter
f     TOL argument
*     until the difference between adjacent panels is sufficiently small
*     to be insignificant. 
*
*     Flux conservation can only be approximate when using a resampling
*     algorithm. For accurate flux conservation use the 
c     astRebin<X> or astRebinSeq<X> function
f     AST_REBIN<X> or AST_REBINSEQ<X> routine
*     instead.

*  Propagation of Missing Data:
*     Instances of missing data (bad pixels) in the output grid are
c     identified by occurrences of the "badval" value in the "out"
f     identified by occurrences of the BADVAL value in the OUT
*     array. These may be produced if any of the following happen:
*
*     - The input position (the transformed position of the output
*     pixel's centre) lies outside the boundary of the grid of input
*     pixels.
*     - The input position lies inside the boundary of a bad input
*     pixel. In this context, an input pixel is considered bad if its
c     data value is equal to "badval" and the AST__USEBAD flag is
c     set via the "flags" parameter.
f     data value is equal to BADVAL and the AST__USEBAD flag is
f     set via the FLAGS argument.
*     (Positions which have half-integral coordinate values, and
*     therefore lie on a pixel boundary, are regarded as lying within
*     the pixel with the larger, i.e. more positive, index.)
*     - The set of neighbouring input pixels (excluding those which
*     are bad) is unsuitable for calculating an interpolated
*     value. Whether this is true may depend on the sub-pixel
*     interpolation scheme in use.
*     - The interpolated value lies outside the range which can be
c     represented using the data type of the "out" array.
f     represented using the data type of the OUT array.
*
*     In addition, associated output variance estimates (if
c     calculated) may be declared bad and flagged with the "badval"
c     value in the "out_var" array under any of the following
f     calculated) may be declared bad and flagged with the BADVAL
f     value in the OUT_VAR array under any of the following
*     circumstances:
*
c     - The associated resampled data value (in the "out" array) is bad.
f     - The associated resampled data value (in the OUT array) is bad.
*     - The set of neighbouring input pixels which contributed to the
*     output data value do not all have valid variance estimates
*     associated with them. In this context, an input variance
*     estimate may be regarded as bad either because it has the value
c     "badval" (and the AST__USEBAD flag is set), or because it is
f     BADVAL (and the AST__USEBAD flag is set), or because it is
*     negative.
*     - The set of neighbouring input pixels for which valid variance
*     values are available is unsuitable for calculating an overall
*     variance value. Whether this is true may depend on the sub-pixel
*     interpolation scheme in use.
*     - The variance value lies outside the range which can be
c     represented using the data type of the "out_var" array.
f     represented using the data type of the OUT_VAR array.
*--
*/
/* Define a macro to implement the function for a specific data
   type. */
#define MAKE_RESAMPLE(X,Xtype) \
static int Resample##X( AstMapping *this, int ndim_in, \
                        const int lbnd_in[], const int ubnd_in[], \
                        const Xtype in[], const Xtype in_var[], \
                        int interp, void (* finterp)(), \
                        const double params[], int flags, double tol, \
                        int maxpix, Xtype badval, \
                        int ndim_out, const int lbnd_out[], \
                        const int ubnd_out[], const int lbnd[], \
                        const int ubnd[], Xtype out[], Xtype out_var[] ) { \
\
/* Local Variables: */ \
   AstMapping *simple;           /* Pointer to simplified Mapping */ \
   int idim;                     /* Loop counter for coordinate dimensions */ \
   int nin;                      /* Number of Mapping input coordinates */ \
   int nout;                     /* Number of Mapping output coordinates */ \
   int npix;                     /* Number of pixels in output region */ \
   int result;                   /* Result value to return */ \
\
/* Initialise. */ \
   result = 0; \
\
/* Check the global error status. */ \
   if ( !astOK ) return result; \
\
/* Obtain values for the Nin and Nout attributes of the Mapping. */ \
   nin = astGetNin( this ); \
   nout = astGetNout( this ); \
\
/* If OK, check that the number of input grid dimensions matches the \
   number required by the Mapping and is at least 1. Report an error \
   if necessary. */ \
   if ( astOK && ( ( ndim_in != nin ) || ( ndim_in < 1 ) ) ) { \
      astError( AST__NGDIN, "astResample"#X"(%s): Bad number of input grid " \
                "dimensions (%d).", astGetClass( this ), ndim_in ); \
      if ( ndim_in != nin ) { \
         astError( AST__NGDIN, "The %s given requires %d coordinate value%s " \
                   "to specify an input position.", \
                   astGetClass( this ), nin, ( nin == 1 ) ? "" : "s" ); \
      } \
   } \
\
/* If OK, also check that the number of output grid dimensions matches \
   the number required by the Mapping and is at least 1. Report an \
   error if necessary. */ \
   if ( astOK && ( ( ndim_out != nout ) || ( ndim_out < 1 ) ) ) { \
      astError( AST__NGDIN, "astResample"#X"(%s): Bad number of output grid " \
                "dimensions (%d).", astGetClass( this ), ndim_out ); \
      if ( ndim_out != nout ) { \
         astError( AST__NGDIN, "The %s given generates %s%d coordinate " \
                   "value%s for each output position.", astGetClass( this ), \
                   ( nout < ndim_out ) ? "only " : "", nout, \
                   ( nout == 1 ) ? "" : "s" ); \
      } \
   } \
\
/* Check that the lower and upper bounds of the input grid are \
   consistent. Report an error if any pair is not. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_in; idim++ ) { \
         if ( lbnd_in[ idim ] > ubnd_in[ idim ] ) { \
            astError( AST__GBDIN, "astResample"#X"(%s): Lower bound of " \
                      "input grid (%d) exceeds corresponding upper bound " \
                      "(%d).", astGetClass( this ), \
                      lbnd_in[ idim ], ubnd_in[ idim ] ); \
            astError( AST__GBDIN, "Error in input dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* Check that the positional accuracy tolerance supplied is valid and \
   report an error if necessary. */ \
   if ( astOK && ( tol < 0.0 ) ) { \
      astError( AST__PATIN, "astResample"#X"(%s): Invalid positional " \
                "accuracy tolerance (%.*g pixel).", \
                astGetClass( this ), DBL_DIG, tol ); \
      astError( AST__PATIN, "This value should not be less than zero." ); \
   } \
\
/* Check that the initial scale size in pixels supplied is valid and \
   report an error if necessary. */ \
   if ( astOK && ( maxpix < 0 ) ) { \
      astError( AST__SSPIN, "astResample"#X"(%s): Invalid initial scale " \
                "size in pixels (%d).", astGetClass( this ), maxpix ); \
      astError( AST__SSPIN, "This value should not be less than zero." ); \
   } \
\
/* Check that the lower and upper bounds of the output grid are \
   consistent. Report an error if any pair is not. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         if ( lbnd_out[ idim ] > ubnd_out[ idim ] ) { \
            astError( AST__GBDIN, "astResample"#X"(%s): Lower bound of " \
                      "output grid (%d) exceeds corresponding upper bound " \
                      "(%d).", astGetClass( this ), \
                      lbnd_out[ idim ], ubnd_out[ idim ] ); \
            astError( AST__GBDIN, "Error in output dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* Similarly check the bounds of the output region. */ \
   if ( astOK ) { \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         if ( lbnd[ idim ] > ubnd[ idim ] ) { \
            astError( AST__GBDIN, "astResample"#X"(%s): Lower bound of " \
                      "output region (%d) exceeds corresponding upper " \
                      "bound (%d).", astGetClass( this ), \
                      lbnd[ idim ], ubnd[ idim ] ); \
\
/* Also check that the output region lies wholly within the output \
   grid. */ \
         } else if ( lbnd[ idim ] < lbnd_out[ idim ] ) { \
            astError( AST__GBDIN, "astResample"#X"(%s): Lower bound of " \
                      "output region (%d) is less than corresponding " \
                      "bound of output grid (%d).", astGetClass( this ), \
                      lbnd[ idim ], lbnd_out[ idim ] ); \
         } else if ( ubnd[ idim ] > ubnd_out[ idim ] ) { \
            astError( AST__GBDIN, "astResample"#X"(%s): Upper bound of " \
                      "output region (%d) exceeds corresponding " \
                      "bound of output grid (%d).", astGetClass( this ), \
                      ubnd[ idim ], ubnd_out[ idim ] ); \
         } \
\
/* Say which dimension produced the error. */ \
         if ( !astOK ) { \
            astError( AST__GBDIN, "Error in output dimension %d.", \
                      idim + 1 ); \
            break; \
         } \
      } \
   } \
\
/* If we are conserving flux, check "tol" is not zero. */ \
   if( ( flags & AST__CONSERVEFLUX ) && astOK ) { \
      if( tol == 0.0 ) { \
         astError( AST__CNFLX, "astResample"#X"(%s): Flux conservation was " \
                   "requested but cannot be performed because zero tolerance " \
                   "was also specified.", astGetClass( this ) ); \
\
/* Also check "nin" and "nout" are equal. */ \
      } else if( nin != nout ) { \
         astError( AST__CNFLX, "astResample"#X"(%s): Flux conservation was " \
                "requested but cannot be performed because the Mapping " \
                "has different numbers of inputs and outputs.", \
                astGetClass( this ) ); \
      } \
   } \
\
/* If OK, loop to determine how many pixels require resampled values. */ \
   simple = NULL; \
   if ( astOK ) { \
      npix = 1; \
      for ( idim = 0; idim < ndim_out; idim++ ) { \
         npix *= ubnd[ idim ] - lbnd[ idim ] + 1; \
      } \
\
/* If there are sufficient pixels to make it worthwhile, simplify the \
   Mapping supplied to improve performance. Otherwise, just clone the \
   Mapping pointer. Note we save a pointer to the original Mapping so \
   that lower-level functions can use it if they need to report an \
   error. */ \
      unsimplified_mapping = this; \
      if ( npix > 1024 ) { \
         simple = astSimplify( this ); \
      } else { \
         simple = astClone( this ); \
      } \
   } \
\
/* Report an error if the inverse transformation of this simplified \
   Mapping is not defined. */ \
   if ( !astGetTranInverse( simple ) && astOK ) { \
      astError( AST__TRNND, "astResample"#X"(%s): An inverse coordinate " \
                "transformation is not defined by the %s supplied.", \
                astGetClass( unsimplified_mapping ), \
                astGetClass( unsimplified_mapping ) ); \
   } \
\
/* Perform the resampling. Note that we pass all gridded data, the \
   interpolation function and the bad pixel value by means of pointer \
   types that obscure the underlying data type. This is to avoid \
   having to replicate functions unnecessarily for each data \
   type. However, we also pass an argument that identifies the data \
   type we have obscured. */ \
   result = ResampleAdaptively( simple, ndim_in, lbnd_in, ubnd_in, \
                                (const void *) in, (const void *) in_var, \
                                TYPE_##X, interp, finterp, \
                                params, flags, tol, maxpix, \
                                (const void *) &badval, \
                                ndim_out, lbnd_out, ubnd_out, \
                                lbnd, ubnd, \
                                (void *) out, (void *) out_var ); \
\
/* Annul the pointer to the simplified/cloned Mapping. */ \
   simple = astAnnul( simple ); \
\
/* If an error occurred, clear the returned result. */ \
   if ( !astOK ) result = 0; \
\
/* Return the result. */ \
   return result; \
}

/* Expand the above macro to generate a function for each required
   data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_RESAMPLE(LD,long double)
#endif
MAKE_RESAMPLE(D,double)
MAKE_RESAMPLE(F,float)
MAKE_RESAMPLE(L,long int)
MAKE_RESAMPLE(UL,unsigned long int)
MAKE_RESAMPLE(I,int)
MAKE_RESAMPLE(UI,unsigned int)
MAKE_RESAMPLE(S,short int)
MAKE_RESAMPLE(US,unsigned short int)
MAKE_RESAMPLE(B,signed char)
MAKE_RESAMPLE(UB,unsigned char)

/* Undefine the macro. */
#undef MAKE_RESAMPLE

static int ResampleAdaptively( AstMapping *this, int ndim_in,
                               const int *lbnd_in, const int *ubnd_in,
                               const void *in, const void *in_var,
                               DataType type, int interp, void (* finterp)(),
                               const double *params, int flags, double tol,
                               int maxpix, const void *badval_ptr,
                               int ndim_out, const int *lbnd_out,
                               const int *ubnd_out, const int *lbnd,
                               const int *ubnd, void *out, void *out_var ) {
/*
*  Name:
*     ResampleAdaptively

*  Purpose:
*     Resample a section of a data grid adaptively.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int ResampleAdaptively( AstMapping *this, int ndim_in,
*                             const int *lbnd_in, const int *ubnd_in,
*                             const void *in, const void *in_var,
*                             DataType type, int interp, void (* finterp)(),
*                             const double *params, int flags, double tol,
*                             int maxpix, const void *badval_ptr,
*                             int ndim_out, const int *lbnd_out,
*                             const int *ubnd_out, const int *lbnd,
*                             const int *ubnd, void *out, void *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function resamples a rectangular grid of data (with any
*     number of dimensions) into a specified section of another
*     rectangular grid (with a possibly different number of
*     dimensions). The coordinate transformation used to convert
*     output pixel coordinates into positions in the input grid is
*     given by the inverse transformation of the Mapping which is
*     supplied.  Any pixel interpolation scheme may be specified for
*     interpolating between the pixels of the input grid.
*
*     This function is very similar to ResampleWithBlocking and
*     ResampleSection which lie below it in the calling
*     hierarchy. However, this function also attempts to adapt to the
*     Mapping supplied and to sub-divide the section being resampled
*     into smaller sections within which a linear approximation to the
*     Mapping may be used.  This reduces the number of Mapping
*     evaluations, thereby improving efficiency particularly when
*     complicated Mappings are involved.

*  Parameters:
*     this
*        Pointer to a Mapping, whose inverse transformation may be
*        used to transform the coordinates of pixels in the output
*        grid into associated positions in the input grid, from which
*        the output pixel values should be derived (by interpolation
*        if necessary).
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     in
*        Pointer to the input array of data to be resampled (with one
*        element for each pixel in the input grid). The numerical type
*        of these data should match the "type" value (below). The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and data type as the "in" array),
*        which represent estimates of the statistical variance
*        associated with each element of the "in" array. If this
*        second array is given (along with the corresponding "out_var"
*        array), then estimates of the variance of the resampled data
*        will also be returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     type
*        A value taken from the "DataType" enum, which specifies the
*        data type of the input and output arrays containing the
*        gridded data (and variance) values.
*     interp
*        A value selected from a set of pre-defined macros to identify
*        which sub-pixel interpolation algorithm should be used.
*     finterp
*        If "interp" is set to a value which requires a user-supplied
*        function, then a pointer to that function shoild be given
*        here. Otherwise, this value is not used and may be a NULL
*        pointer.
*     params
*        Pointer to an optional array of parameters that may be passed
*        to the interpolation algorithm, if required. If no parameters
*        are required, a NULL pointer should be supplied.
*     flags
*        The bitwise OR of a set of flag values which provide
*        additional control over the resampling operation.
*     tol
*        The maximum permitted positional error in transforming output
*        pixel positions into the input grid in order to resample
*        it. This should be expressed as a displacement in pixels in
*        the input grid's coordinate system. If the Mapping's inverse
*        transformation can be approximated by piecewise linear
*        functions to this accuracy, then such functions may be used
*        instead of the Mapping in order to improve
*        performance. Otherwise, every output pixel position will be
*        transformed individually using the Mapping.
*
*        If linear approximation is not required, a "tol" value of
*        zero may be given. This will ensure that the Mapping is used
*        without any approximation.
*     maxpix
*        A value which specifies the largest scale size on which to
*        search for non-linearities in the Mapping supplied. This
*        value should be expressed as a number of pixels in the output
*        grid. The function will break the output section specified
*        into smaller sub-sections (if necessary), each no larger than
*        "maxpix" pixels in any dimension, before it attempts to
*        approximate the Mapping by a linear function over each
*        sub-section.
* 
*        If the value given is larger than the largest dimension of
*        the output section (the normal recommendation), the function
*        will initially search for non-linearity on a scale determined
*        by the size of the output section.  This is almost always
*        satisfactory. Very occasionally, however, a Mapping may
*        appear linear on this scale but nevertheless have smaller
*        irregularities (e.g. "holes") in it.  In such cases, "maxpix"
*        may be set to a suitably smaller value so as to ensure this
*        non-linearity is not overlooked. Typically, a value of 50 to
*        100 pixels might be suitable and should have little effect on
*        performance.
*
*        If too small a value is given, however, it will have the
*        effect of preventing linear approximation occurring at all
*        (equivalent to setting "tol" to zero).  Although this may
*        degrade performance, accurate results will still be obtained.
*     badval_ptr
*        If the AST__USEBAD flag is set (above), this parameter is a
*        pointer to a value which is used to identify bad data and/or
*        variance values in the input array(s). The referenced value's
*        data type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output data grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output data grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output data grid in the same way as "lbnd_in"
*        and "ubnd_in" define the shape and size of the input grid
*        (see above).
*     lbnd
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the first pixel in the
*        section of the output data grid for which a value is
*        required.
*     ubnd
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the last pixel in the
*        section of the output data grid for which a value is
*        required.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the output grid for which resampled values are
*        required. This section should lie wholly within the extent of
*        the output grid (as defined by the "lbnd_out" and "ubnd_out"
*        arrays). Regions of the output grid lying outside this section
*        will not be modified.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned.  The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given.
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.

*  Returned Value:
*     The number of output grid points to which a data value (or a
*     variance value if relevant) equal to "badval" has been assigned
*     because no valid output value could be obtained.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*/
                      
/* Local Variables: */
   double *flbnd;                /* Array holding floating point lower bounds */
   double *fubnd;                /* Array holding floating point upper bounds */
   double *linear_fit;           /* Pointer to array of fit coefficients */
   int *hi;                      /* Pointer to array of section upper bounds */
   int *lo;                      /* Pointer to array of section lower bounds */
   int coord_out;                /* Loop counter for output coordinates */
   int dim;                      /* Output section dimension size */
   int dimx;                     /* Dimension with maximum section extent */
   int divide;                   /* Sub-divide the output section? */
   int i;                        /* Loop count */
   int isLinear;                 /* Is the transformation linear? */
   int mxdim;                    /* Largest output section dimension size */
   int npix;                     /* Number of pixels in output section */
   int npoint;                   /* Number of points for obtaining a fit */
   int nvertex;                  /* Number of vertices of output section */
   int result;                   /* Result value to return */
   int toobig;                   /* Section too big (must sub-divide)? */
   int toosmall;                 /* Section too small to sub-divide? */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Further initialisation. */
   npix = 1;
   mxdim = 0;
   dimx = 1;
   nvertex = 1;

/* Loop through the output grid dimensions. */
   for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {

/* Obtain the extent in each dimension of the output section which is
   to receive resampled values, and calculate the total number of
   pixels it contains. */
      dim = ubnd[ coord_out ] - lbnd[ coord_out ] + 1;
      npix *= dim;

/* Find the maximum dimension size of this output section and note
   which dimension has this size. */
      if ( dim > mxdim ) {
         mxdim = dim;
         dimx = coord_out;
      }

/* Calculate how many vertices the output section has. */
      nvertex *= 2;
   }
   
/* Calculate how many sample points will be needed (by the
   astLinearApprox function) to obtain a linear fit to the Mapping's
   inverse transformation. */
   npoint = 1 + 4 * ndim_out + 2 * nvertex;

/* If the number of pixels in the output section is not at least 4
   times this number, we will probably not save significant time by
   attempting to obtain a linear fit, so note that the output section
   is too small. */
   toosmall = ( npix < ( 4 * npoint ) );

/* Note if the maximum dimension of the output section exceeds the
   user-supplied scale factor. */
   toobig = ( maxpix < mxdim );

/* Assume the Mapping is significantly non-linear before deciding
   whether to sub-divide the output section. */
   linear_fit = NULL;

/* If the output section is too small to be worth obtaining a linear
   fit, or if the accuracy tolerance is zero, we will not
   sub-divide. This means that the Mapping will be used to transform
   each pixel's coordinates and no linear approximation will be
   used. */
   if ( toosmall || ( tol == 0.0 ) ) {
      divide = 0;

/* Otherwise, if the largest output section dimension exceeds the
   scale length given, we will sub-divide. This offers the possibility
   of obtaining a linear approximation to the Mapping over a reduced
   range of output coordinates (which will be handled by a recursive
   invocation of this function). */
   } else if ( toobig ) {
      divide = 1;

/* If neither of the above apply, then attempt to fit a linear
   approximation to the Mapping's inverse transformation over the
   range of coordinates covered by the output section. We need to 
   temporarily copy the integer bounds into floating point arrays to 
   use astLinearApprox. */
   } else {

/* Allocate memory for floating point bounds and for the coefficient array */
      flbnd = astMalloc( sizeof( double )*(size_t) ndim_out );
      fubnd = astMalloc( sizeof( double )*(size_t) ndim_out );
      linear_fit = astMalloc( sizeof( double )*
                              (size_t) ( ndim_in*( ndim_out + 1 ) ) );
      if( astOK ) {

/* Copy the bounds into these arrays */
         for( i = 0; i < ndim_out; i++ ) {
            flbnd[ i ] = (double) lbnd[ i ];
            fubnd[ i ] = (double) ubnd[ i ];
         }

/* Get the linear approximation to the inverse transformation. The
   astLinearApprox function fits the forward transformation so temporarily
   invert the Mapping in order to get a fit to the inverse transformation. */
         astInvert( this );
         isLinear = astLinearApprox( this, flbnd, fubnd, tol, linear_fit );
         astInvert( this );

/* Free the coeff array if the inverse transformation is not linear. */
         if( !isLinear ) linear_fit = astFree( linear_fit );

      } else {
         linear_fit = astFree( linear_fit );
      }

/* Free resources */
      flbnd = astFree( flbnd );
      fubnd = astFree( fubnd );

/* If a linear fit was obtained, we will use it and therefore do not
   wish to sub-divide further. Otherwise, we sub-divide in the hope
   that this may result in a linear fit next time. */
      divide = !linear_fit;
   }

/* If no sub-division is required, perform resampling (in a
   memory-efficient manner, since the section we are resampling might
   still be very large). This will use the linear fit, if obtained
   above. */
   if ( astOK ) {
      if ( !divide ) {
         result = ResampleWithBlocking( this, linear_fit,
                                        ndim_in, lbnd_in, ubnd_in,
                                        in, in_var, type, interp, finterp,
                                        params, flags, badval_ptr,
                                        ndim_out, lbnd_out, ubnd_out,
                                        lbnd, ubnd, out, out_var );

/* Otherwise, allocate workspace to perform the sub-division. */
      } else {
         lo = astMalloc( sizeof( int ) * (size_t) ndim_out );
         hi = astMalloc( sizeof( int ) * (size_t) ndim_out );
         if ( astOK ) {

/* Initialise the bounds of a new output section to match the original
   output section. */
            for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
               lo[ coord_out ] = lbnd[ coord_out ];
               hi[ coord_out ] = ubnd[ coord_out ];
            }

/* Replace the upper bound of the section's largest dimension with the
   mid-point of the section along this dimension, rounded
   downwards. */
            hi[ dimx ] =
               (int) floor( 0.5 * (double) ( lbnd[ dimx ] + ubnd[ dimx ] ) );

/* Resample the resulting smaller section using a recursive invocation
   of this function. */
            result = ResampleAdaptively( this, ndim_in, lbnd_in, ubnd_in,
                                         in, in_var, type, interp, finterp,
                                         params, flags, tol, maxpix,
                                         badval_ptr, ndim_out,
                                         lbnd_out, ubnd_out,
                                         lo, hi, out, out_var );

/* Now set up a second section which covers the remaining half of the
   original output section. */
            lo[ dimx ] = hi[ dimx ] + 1;
            hi[ dimx ] = ubnd[ dimx ];

/* If this section contains pixels, resample it in the same way,
   summing the returned values. */
            if ( lo[ dimx ] <= hi[ dimx ] ) {
               result += ResampleAdaptively( this, ndim_in, lbnd_in, ubnd_in,
                                             in, in_var, type, interp, finterp,
                                             params, flags, tol, maxpix,
                                             badval_ptr,  ndim_out,
                                             lbnd_out, ubnd_out,
                                             lo, hi, out, out_var );
            }
         }

/* Free the workspace. */
         lo = astFree( lo );
         hi = astFree( hi );
      }
   }

/* If coefficients for a linear fit were obtained, then free the space
   they occupy. */
   if ( linear_fit ) linear_fit = astFree( linear_fit );

/* If an error occurred, clear the returned result. */
   if ( !astOK ) result = 0;

/* Return the result. */
   return result;
}

static int ResampleSection( AstMapping *this, const double *linear_fit,
                            int ndim_in,
                            const int *lbnd_in, const int *ubnd_in,
                            const void *in, const void *in_var,
                            DataType type, int interp, void (* finterp)(),
                            const double *params, double factor, int flags,
                            const void *badval_ptr, int ndim_out,
                            const int *lbnd_out, const int *ubnd_out,
                            const int *lbnd, const int *ubnd,
                            void *out, void *out_var ) {
/*
*  Name:
*     ResampleSection

*  Purpose:
*     Resample a section of a data grid.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int ResampleSection( AstMapping *this, const double *linear_fit,
*                          int ndim_in, const int *lbnd_in, const int *ubnd_in,
*                          const void *in, const void *in_var,
*                          DataType type, int interp, void (* finterp)(),
*                          const double *params, double factor, int flags,
*                          const void *badval_ptr, int ndim_out,
*                          const int *lbnd_out, const int *ubnd_out,
*                          const int *lbnd, const int *ubnd,
*                          void *out, void *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function resamples a rectangular grid of data (with any
*     number of dimensions) into a specified section of another
*     rectangular grid (with a possibly different number of
*     dimensions). The coordinate transformation used is given by the
*     inverse transformation of the Mapping which is supplied or,
*     alternatively, by a linear approximation fitted to a Mapping's
*     inverse transformation. Any pixel interpolation scheme may be
*     specified for interpolating between the pixels of the input
*     grid.

*  Parameters:
*     this
*        Pointer to a Mapping, whose inverse transformation may be
*        used to transform the coordinates of pixels in the output
*        grid into associated positions in the input grid, from which
*        the output pixel values should be derived (by interpolation
*        if necessary).
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     linear_fit
*        Pointer to an optional array of double which contains the
*        coefficients of a linear fit which approximates the above
*        Mapping's inverse coordinate transformation. If this is
*        supplied, it will be used in preference to the above Mapping
*        when transforming coordinates. This may be used to enhance
*        performance in cases where evaluation of the Mapping's
*        inverse transformation is expensive. If no linear fit is
*        available, a NULL pointer should be supplied.
*
*        The way in which the fit coefficients are stored in this
*        array and the number of array elements are as defined by the
*        astLinearApprox function.
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     in
*        Pointer to the input array of data to be resampled (with one
*        element for each pixel in the input grid). The numerical type
*        of these data should match the "type" value (below). The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and data type as the "in" array),
*        which represent estimates of the statistical variance
*        associated with each element of the "in" array. If this
*        second array is given (along with the corresponding "out_var"
*        array), then estimates of the variance of the resampled data
*        will also be returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     type
*        A value taken from the "DataType" enum, which specifies the
*        data type of the input and output arrays containing the
*        gridded data (and variance) values.
*     interp
*        A value selected from a set of pre-defined macros to identify
*        which sub-pixel interpolation algorithm should be used.
*     finterp
*        If "interp" is set to a value which requires a user-supplied
*        function, then a pointer to that function shoild be given
*        here. Otherwise, this value is not used and may be a NULL
*        pointer.
*     params
*        Pointer to an optional array of parameters that may be passed
*        to the interpolation algorithm, if required. If no parameters
*        are required, a NULL pointer should be supplied.
*     factor
*        A factor by which to scale the resampled output data values before
*        returning them. If flux is being conserved this should be set to
*        the ratio of the output pixel size to the input pixel size in the 
*        section. Otherwise it should be set to 1.0.
*     flags
*        The bitwise OR of a set of flag values which provide
*        additional control over the resampling operation.
*     badval_ptr
*        If the AST__USEBAD flag is set (above), this parameter is a
*        pointer to a value which is used to identify bad data and/or
*        variance values in the input array(s). The referenced value's
*        data type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output data grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output data grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output data grid in the same way as "lbnd_in"
*        and "ubnd_in" define the shape and size of the input grid
*        (see above).
*     lbnd
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the first pixel in the
*        section of the output data grid for which a value is
*        required.
*     ubnd
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the last pixel in the
*        section of the output data grid for which a value is
*        required.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the output grid for which resampled values are
*        required. This section should lie wholly within the extent of
*        the output grid (as defined by the "lbnd_out" and "ubnd_out"
*        arrays). Regions of the output grid lying outside this section
*        will not be modified.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned.  The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given.
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.

*  Returned Value:
*       The number of output grid points to which a data value (or a
*       variance value if relevant) equal to "badval" has been
*       assigned because no valid output value could be obtained.

*  Notes:
*     - This function does not take steps to limit memory usage if the
*     grids supplied are large. To resample large grids in a more
*     memory-efficient way, the ResampleWithBlocking function should
*     be used.
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*/

/* Local Variables: */
   AstPointSet *pset_in;         /* Input PointSet for transformation */
   AstPointSet *pset_out;        /* Output PointSet for transformation */
   const double *grad;           /* Pointer to gradient matrix of linear fit */
   const double *par;            /* Pointer to parameter array */
   const double *zero;           /* Pointer to zero point array of fit */
   double **ptr_in;              /* Pointer to input PointSet coordinates */
   double **ptr_out;             /* Pointer to output PointSet coordinates */
   double *accum;                /* Pointer to array of accumulated sums */
   double fwhm;                  /* Full width half max. of gaussian */
   double lpar[ 1 ];             /* Local parameter array */
   double x1;                    /* Interim x coordinate value */
   double y1;                    /* Interim y coordinate value */
   int *dim;                     /* Pointer to array of output pixel indices */
   int *offset;                  /* Pointer to array of output pixel offsets */
   int *stride;                  /* Pointer to array of output grid strides */
   int conserve;                 /* Conserve flux? */
   int coord_in;                 /* Loop counter for input dimensions */
   int coord_out;                /* Loop counter for output dimensions */
   int done;                     /* All pixel indices done? */
   int i1;                       /* Interim offset into "accum" array */
   int i2;                       /* Final offset into "accum" array */
   int idim;                     /* Loop counter for dimensions */
   int ix;                       /* Loop counter for output x coordinate */
   int iy;                       /* Loop counter for output y coordinate */
   int nbad;                     /* Number of pixels assigned a bad value */
   int neighb;                   /* Number of neighbouring pixels */
   int npoint;                   /* Number of output points (pixels) */
   int off1;                     /* Interim pixel offset into output array */
   int off;                      /* Final pixel offset into output array */
   int point;                    /* Counter for output points (pixels ) */
   int result;                   /* Result value to be returned */
   int s;                        /* Temporary variable for strides */
   int usevar;                   /* Process variance array? */
   void (* gifunc)();            /* General interpolation function */
   void (* kernel)( double, const double [], int, double * ); /* Kernel fn. */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Further initialisation. */
   pset_in = NULL;
   ptr_in = NULL;
   neighb = 0;
   gifunc = NULL;
   kernel = NULL;

/* See if we are conserving flux */
   conserve = flags & AST__CONSERVEFLUX; \

/* Calculate the number of output points, as given by the product of
   the output grid dimensions. */
   for ( npoint = 1, coord_out = 0; coord_out < ndim_out; coord_out++ ) {
      npoint *= ubnd[ coord_out ] - lbnd[ coord_out ] + 1;
   }

/* Allocate workspace. */
   offset = astMalloc( sizeof( int ) * (size_t) npoint );
   stride = astMalloc( sizeof( int ) * (size_t) ndim_out );
   if ( astOK ) {

/* Calculate the stride for each output grid dimension. */
      off = 0;
      s = 1;
      for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
         stride[ coord_out ] = s;
         s *= ubnd_out[ coord_out ] - lbnd_out[ coord_out ] + 1;
      }

/* A linear fit to the Mapping is available. */
/* ========================================= */
      if ( linear_fit ) {

/* If a linear fit to the Mapping has been provided, then obtain
   pointers to the array of gradients and zero-points comprising the
   fit. */
         grad = linear_fit + ndim_in;
         zero = linear_fit;

/* Create a PointSet to hold the input grid coordinates and obtain an
   array of pointers to its coordinate data. */
         pset_in = astPointSet( npoint, ndim_in, "" );
         ptr_in = astGetPoints( pset_in );
         if ( astOK ) {

/* Initialise the count of output points. */
            point = 0;

/* Handle the 1-dimensional case optimally. */
/* ---------------------------------------- */
            if ( ( ndim_in == 1 ) && ( ndim_out == 1 ) ) {

/* Loop through the pixels of the output grid and transform their x
   coordinates into the input grid's coordinate system using the
   linear fit supplied. Store the results in the PointSet created
   above. */
               for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                  ptr_in[ 0 ][ point ] = zero[ 0 ] + grad[ 0 ] * (double) ix;

/* Calculate the offset of each pixel within the output array. */
                  offset[ point ] = ix - lbnd_out[ 0 ];
                  point++;
               }

/* Handle the 2-dimensional case optimally. */
/* ---------------------------------------- */
            } else if ( ( ndim_in == 2 ) && ( ndim_out == 2 ) ) {

/* Loop through the range of y coordinates in the output grid and
   calculate interim values of the input coordinates using the linear
   fit supplied. */
               for ( iy = lbnd[ 1 ]; iy <= ubnd[ 1 ]; iy++ ) {
                  x1 = zero[ 0 ] + grad[ 1 ] * (double) iy;
                  y1 = zero[ 1 ] + grad[ 3 ] * (double) iy;

/* Also calculate an interim pixel offset into the output array. */
                  off1 = stride[ 1 ] * ( iy - lbnd_out[ 1 ] ) - lbnd_out[ 0 ];

/* Now loop through the range of output x coordinates and calculate
   the final values of the input coordinates, storing the results in
   the PointSet created above. */
                  for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                     ptr_in[ 0 ][ point ] = x1 + grad[ 0 ] * (double) ix;
                     ptr_in[ 1 ][ point ] = y1 + grad[ 2 ] * (double) ix;

/* Also calculate final pixel offsets into the output array. */
                     offset[ point ] = off1 + ix;
                     point++;
                  }
               }

/* Handle other numbers of dimensions. */
/* ----------------------------------- */               
            } else {

/* Allocate workspace. */
               accum = astMalloc( sizeof( double ) *
                                 (size_t) ( ndim_in * ndim_out ) );
               dim = astMalloc( sizeof( int ) * (size_t) ndim_out );
               if ( astOK ) {

/* Initialise an array of pixel indices for the output grid which
   refer to the first pixel for which we require a value. Also
   calculate the offset of this pixel within the output array. */
                  off = 0;
                  for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                     dim[ coord_out ] = lbnd[ coord_out ];
                     off += stride[ coord_out ] *
                            ( dim[ coord_out ] - lbnd_out[ coord_out ] );
                  }

/* To calculate each input grid coordinate we must perform a matrix
   multiply on the output grid coordinates (using the gradient matrix)
   and then add the zero points. However, since we will usually only
   be altering one output coordinate at a time (the least
   significant), we can avoid the full matrix multiply by accumulating
   partial sums for the most significant output coordinates and only
   altering those sums which need to change each time. The zero points
   never change, so we first fill the "most significant" end of the
   "accum" array with these. */
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     accum[ ( coord_in + 1 ) * ndim_out - 1 ] =
                                                              zero[ coord_in ];
                  }
                  coord_out = ndim_out - 1;

/* Now loop to process each output pixel. */
                  for ( done = 0; !done; point++ ) {

/* To generate the input coordinate that corresponds to the current
   output pixel, we work down from the most significant dimension
   whose index has changed since the previous pixel we considered
   (given by "coord_out"). For each affected dimension, we accumulate
   in "accum" the matrix sum (including the zero point) for that
   dimension and all higher output dimensions. We must accumulate a
   separate set of sums for each input coordinate we wish to
   produce. (Note that for the first pixel we process, all dimensions
   are considered "changed", so we start by initialising the whole
   "accum" array.) */
                     for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                        i1 = coord_in * ndim_out;
                        for ( idim = coord_out; idim >= 1; idim-- ) {
                           i2 = i1 + idim;
                           accum[ i2 - 1 ] = accum[ i2 ] +
                                             dim[ idim ] * grad[ i2 ];
                        }

/* The input coordinate for each dimension is given by the accumulated
   sum for output dimension zero (giving the sum over all output
   dimensions). We do not store this in the "accum" array, but assign
   the result directly to the coordinate array of the PointSet created
   earlier. */
                        ptr_in[ coord_in ][ point ] = accum[ i1 ] +
                                                      dim[ 0 ] * grad[ i1 ];
                     }

/* Store the offset of the current pixel in the output array. */
                     offset[ point ] = off;

/* Now update the array of pixel indices to refer to the next output
   pixel. */
                     coord_out = 0;
                     do {

/* The least significant index which currently has less than its
   maximum value is incremented by one. The offset into the output
   array is updated accordingly. */
                        if ( dim[ coord_out ] < ubnd[ coord_out ] ) {
                           dim[ coord_out ]++;
                           off += stride[ coord_out ];
                           break;

/* Any less significant indices which have reached their maximum value
   are returned to their minimum value and the output pixel offset is
   decremented appropriately. */
                        } else {
                           dim[ coord_out ] = lbnd[ coord_out ];
                           off -= stride[ coord_out ] *
                                  ( ubnd[ coord_out ] - lbnd[ coord_out ] );

/* All the output pixels have been processed once the most significant
   pixel index has been returned to its minimum value. */
                           done = ( ++coord_out == ndim_out );
                        }
                     } while ( !done );
                  }
               }

/* Free the workspace. */
               accum = astFree( accum );
               dim = astFree( dim );
            }
         }

/* No linear fit to the Mapping is available. */
/* ========================================== */
      } else {

/* If flux conseravtion was requested, report an error, since we can only
   conserve flux if a linear approximation is available. */
         if( conserve && astOK ) { 
            astError( AST__CNFLX, "astResampleSection(%s): Flux conservation "
                "was requested but cannot be performed because either the Mapping "
                "is too non-linear, or the requested tolerance is too small.",
                 astGetClass( this ) ); 
         } 

/* Create a PointSet to hold the coordinates of the output pixels and
   obtain a pointer to its coordinate data. */
         pset_out = astPointSet( npoint, ndim_out, "" );
         ptr_out = astGetPoints( pset_out );
         if ( astOK ) {

/* Initialise the count of output points. */
            point = 0;

/* Handle the 1-dimensional case optimally. */
/* ---------------------------------------- */
            if ( ndim_out == 1 ) {

/* Loop through the required range of output x coordinates, assigning
   the coordinate values to the PointSet created above. Also store a
   pixel offset into the output array. */
               for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                  ptr_out[ 0 ][ point ] = (double) ix;
                  offset[ point ] = ix - lbnd_out[ 0 ];

/* Increment the count of output pixels. */
                  point++;
               }

/* Handle the 2-dimensional case optimally. */
/* ---------------------------------------- */
            } else if ( ndim_out == 2 ) {

/* Loop through the required range of output y coordinates,
   calculating an interim pixel offset into the output array. */
               for ( iy = lbnd[ 1 ]; iy <= ubnd[ 1 ]; iy++ ) {
                  off1 = stride[ 1 ] * ( iy - lbnd_out[ 1 ] ) - lbnd_out[ 0 ];

/* Loop through the required range of output x coordinates, assigning
   the coordinate values to the PointSet created above. Also store a
   final pixel offset into the output array. */
                  for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                     ptr_out[ 0 ][ point ] = (double) ix;
                     ptr_out[ 1 ][ point ] = (double) iy;
                     offset[ point ] = off1 + ix;

/* Increment the count of output pixels. */
                     point++;
                  }
               }

/* Handle other numbers of dimensions. */
/* ----------------------------------- */
            } else {

/* Allocate workspace. */
               dim = astMalloc( sizeof( int ) * (size_t) ndim_out );
               if ( astOK ) {

/* Initialise an array of pixel indices for the output grid which
   refer to the first pixel for which we require a value. Also
   calculate the offset of this pixel within the output array. */
                  off = 0;
                  for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                     dim[ coord_out ] = lbnd[ coord_out ];
                     off += stride[ coord_out ] *
                            ( dim[ coord_out ] - lbnd_out[ coord_out ] );
                  }

/* Loop to generate the coordinates of each output pixel. */
                  for ( done = 0; !done; point++ ) {

/* Copy each pixel's coordinates into the PointSet created above. */
                     for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                        ptr_out[ coord_out ][ point ] =
                                                     (double) dim[ coord_out ];
                     }

/* Store the offset of the pixel in the output array. */
                     offset[ point ] = off;

/* Now update the array of pixel indices to refer to the next output
   pixel. */
                     coord_out = 0;
                     do {

/* The least significant index which currently has less than its
   maximum value is incremented by one. The offset into the output
   array is updated accordingly. */
                        if ( dim[ coord_out ] < ubnd[ coord_out ] ) {
                           dim[ coord_out ]++;
                           off += stride[ coord_out ];
                           break;

/* Any less significant indices which have reached their maximum value
   are returned to their minimum value and the output pixel offset is
   decremented appropriately. */
                        } else {
                           dim[ coord_out ] = lbnd[ coord_out ];
                           off -= stride[ coord_out ] *
                                  ( ubnd[ coord_out ] - lbnd[ coord_out ] );

/* All the output pixels have been processed once the most significant
   pixel index has been returned to its minimum value. */
                           done = ( ++coord_out == ndim_out );
                        }
                     } while ( !done );
                  }
               }

/* Free the workspace. */
               dim = astFree( dim );
            }

/* When all the output pixel coordinates have been generated, use the
   Mapping's inverse transformation to generate the input coordinates
   from them. Obtain an array of pointers to the resulting coordinate
   data. */
            pset_in = astTransform( this, pset_out, 0, NULL );
            ptr_in = astGetPoints( pset_in );
         }

/* Annul the PointSet containing the output coordinates. */
         pset_out = astAnnul( pset_out );
      }
   }

/* Resample the input grid. */
/* ------------------------ */
/* Determine if a variance array is to be processed. */
   usevar = ( in_var && out_var );

/* If the input coordinates have been produced successfully, identify
   the input grid resampling method to be used. */
   if ( astOK ) {

/* Nearest pixel. */
/* -------------- */
      switch ( interp ) {
         case AST__NEAREST:

/* Define a macro to use a "case" statement to invoke the
   nearest-pixel interpolation function appropriate to a given data
   type. */
#define CASE_NEAREST(X,Xtype) \
               case ( TYPE_##X ): \
                  result = \
                  InterpolateNearest##X( ndim_in, lbnd_in, ubnd_in, \
                                         (Xtype *) in, (Xtype *) in_var, \
                                         npoint, offset, \
                                         (const double *const *) ptr_in, \
                                         flags, *( (Xtype *) badval_ptr ), \
                                         (Xtype *) out, (Xtype *) out_var ); \
                  break;
       
/* Use the above macro to invoke the appropriate function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_NEAREST(LD,long double)
#endif
               CASE_NEAREST(D,double)
               CASE_NEAREST(F,float)
               CASE_NEAREST(L,long int)
               CASE_NEAREST(UL,unsigned long int)
               CASE_NEAREST(I,int)
               CASE_NEAREST(UI,unsigned int)
               CASE_NEAREST(S,short int)
               CASE_NEAREST(US,unsigned short int)
               CASE_NEAREST(B,signed char)
               CASE_NEAREST(UB,unsigned char)
            }
            break;

/* Undefine the macro. */
#undef CASE_NEAREST
               
/* Linear interpolation. */
/* --------------------- */
/* Note this is also the default if zero is given. */
         case AST__LINEAR:
         case 0:

/* Define a macro to use a "case" statement to invoke the linear
   interpolation function appropriate to a given data type. */
#define CASE_LINEAR(X,Xtype) \
               case ( TYPE_##X ): \
                  result = \
                  InterpolateLinear##X( ndim_in, lbnd_in, ubnd_in,\
                                        (Xtype *) in, (Xtype *) in_var, \
                                        npoint, offset, \
                                        (const double *const *) ptr_in, \
                                        flags, *( (Xtype *) badval_ptr ), \
                                        (Xtype *) out, (Xtype *) out_var ); \
                  break;

/* Use the above macro to invoke the appropriate function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_LINEAR(LD,long double)
#endif
               CASE_LINEAR(D,double)
               CASE_LINEAR(F,float)
               CASE_LINEAR(L,long int)
               CASE_LINEAR(UL,unsigned long int)
               CASE_LINEAR(I,int)
               CASE_LINEAR(UI,unsigned int)
               CASE_LINEAR(S,short int)
               CASE_LINEAR(US,unsigned short int)
               CASE_LINEAR(B,signed char)
               CASE_LINEAR(UB,unsigned char)
            }
            break;

/* Undefine the macro. */
#undef CASE_LINEAR

/* Interpolation using a 1-d kernel. */
/* --------------------------------- */
         case AST__SINC:
         case AST__SINCCOS:
         case AST__SINCGAUSS:
         case AST__SINCSINC:
         case AST__SOMB:
         case AST__SOMBCOS:
         case AST__UKERN1:       /* User-supplied 1-d kernel function */

/* Obtain a pointer to the appropriate 1-d kernel function (either
   internal or user-defined) and set up any parameters it may
   require. */
            par = NULL;
            switch ( interp ) {

/* sinc(pi*x) interpolation. */
/* ------------------------- */
/* Assign the kernel function. */
               case AST__SINC:
                  kernel = Sinc;

/* Calculate the number of neighbouring pixels to use. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) {
                     neighb = 2;
                  } else {
                     neighb = MaxI( 1, neighb );
                  }
                  break;

/* sinc(pi*x)*cos(k*pi*x) interpolation. */
/* ------------------------------------- */
/* Assign the kernel function. */
               case AST__SINCCOS:
                  kernel = SincCos;

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 0.5 / MaxD( 1.0, params[ 1 ] );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, the number will be calculated automatically below. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = INT_MAX;

/* Calculate the maximum number of neighbouring pixels required by the
   width of the kernel, and use this value if preferable. */
                  neighb = MinI( neighb,
                                 (int) ceil( MaxD( 1.0, params[ 1 ] ) ) );
                  break;

/* somb(pi*x) interpolation. */
/* ------------------------- */
/* Assign the kernel function. */
               case AST__SOMB:
                  kernel = Somb;

/* Calculate the number of neighbouring pixels to use. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) {
                     neighb = 2;
                  } else {
                     neighb = MaxI( 1, neighb );
                  }
                  break;

/* somb(pi*x)*cos(k*pi*x) interpolation. */
/* ------------------------------------- */
/* Assign the kernel function. */
               case AST__SOMBCOS:
                  kernel = SombCos;

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 0.5 / MaxD( 1.0, params[ 1 ] );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, the number will be calculated automatically below. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = INT_MAX;

/* Calculate the maximum number of neighbouring pixels required by the
   width of the kernel, and use this value if preferable. */
                  neighb = MinI( neighb,
                                 (int) ceil( MaxD( 1.0, params[ 1 ] ) ) );
                  break;

/* sinc(pi*x)*exp(-k*x*x) interpolation. */
/* ------------------------------------- */
/* Assign the kernel function. */
               case AST__SINCGAUSS:
                  kernel = SincGauss;

/* Constrain the full width half maximum of the gaussian factor. */
                  fwhm = MaxD( 0.1, params[ 1 ] );

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 4.0 * log( 2.0 ) / ( fwhm * fwhm );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, use the number of neighbouring pixels required by the width
   of the kernel (out to where the gaussian term falls to 1% of its
   peak value). */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = (int) ceil( sqrt( -log( 0.01 ) /
                                                                lpar[ 0 ] ) );
                  break;

/* sinc(pi*x)*sinc(k*pi*x) interpolation. */
/* -------------------------------------- */
/* Assign the kernel function. */
               case AST__SINCSINC:
                  kernel = SincSinc;

/* Store the required value of "k" in a local parameter array and pass
   this array to the kernel function. */
                  lpar[ 0 ] = 0.5 / MaxD( 1.0, params[ 1 ] );
                  par = lpar;

/* Obtain the number of neighbouring pixels to use. If this is zero or
   less, the number will be calculated automatically below. */
                  neighb = (int) floor( params[ 0 ] + 0.5 );
                  if ( neighb <= 0 ) neighb = INT_MAX;

/* Calculate the maximum number of neighbouring pixels required by the
   width of the kernel, and use this value if preferable. */
                  neighb = MinI( neighb,
                                 (int) ceil( MaxD( 1.0, params[ 1 ] ) ) );
                  break;

/* User-supplied kernel. */
/* --------------------- */
/* Assign the kernel function. */
               case AST__UKERN1:
                  kernel = (void (*)( double, const double [],
                                      int, double * )) finterp;

/* Calculate the number of neighbouring pixels to use. */
                  neighb = MaxI( 1, (int) floor( params[ 0 ] + 0.5 ) );

/* Pass a pointer to the "params" array. */
                  par = params;
                  break;
            }

/* Define a macro to use a "case" statement to invoke the 1-d kernel
   interpolation function appropriate to a given data type, passing it
   the pointer to the kernel function obtained above. */
#define CASE_KERNEL1(X,Xtype) \
               case ( TYPE_##X ): \
                  result = \
                  InterpolateKernel1##X( this, ndim_in, lbnd_in, ubnd_in, \
                                         (Xtype *) in, (Xtype *) in_var, \
                                         npoint, offset, \
                                         (const double *const *) ptr_in, \
                                         kernel, neighb, par, flags, \
                                         *( (Xtype *) badval_ptr ), \
                                         (Xtype *) out, (Xtype *) out_var ); \
                  break;

/* Use the above macro to invoke the appropriate function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_KERNEL1(LD,long double)
#endif
               CASE_KERNEL1(D,double)
               CASE_KERNEL1(F,float)
               CASE_KERNEL1(L,long int)
               CASE_KERNEL1(UL,unsigned long int)
               CASE_KERNEL1(I,int)
               CASE_KERNEL1(UI,unsigned int)
               CASE_KERNEL1(S,short int)
               CASE_KERNEL1(US,unsigned short int)
               CASE_KERNEL1(B,signed char)
               CASE_KERNEL1(UB,unsigned char)
            }
            break;

/* Undefine the macro. */
#undef CASE_KERNEL1

/* General sub-pixel interpolation function. */
/* ----------------------------------------- */
         case AST__BLOCKAVE:
         case AST__UINTERP:

/* Define a macro to use a "case" statement to invoke the general 
   sub-pixel interpolation function appropriate to a given type and
   the selected value of the interp variable. */
#define CASE_GINTERP(X,Xtype) \
               case ( TYPE_##X ): \
\
/* Obtain a pointer to the appropriate general interpolation function \
   (either internal or user-defined) and set up any parameters it may \
   require. */ \
                  switch ( interp ) { \
\
/* Block averaging interpolation. */ \
/* ------------------------------ */ \
                     case AST__BLOCKAVE: \
                        gifunc = (void (*)()) InterpolateBlockAverage##X; \
                        break; \
\
/* User-supplied sub-pixel interpolation function. */ \
/* ----------------------------------------------- */ \
                     case AST__UINTERP: \
                        gifunc = (void (*)()) finterp; \
                        break; \
                  } \
\
/* Invoke the general interpolation function.  It has to be cast to the \
   right type (i.e. a function with the correctly typed arguments) \
   to prevent default promotion (to int or double) of its arguments. \
   The cast here corresponds to the declaration of 
   ast_resample_uinterp##Xtype. */ \
                  ( *( (void (*)( int, const int[], const int[], \
                                  const Xtype[], \
                                  const Xtype[], \
                                  int, const int[], \
                                  const double *const[], \
                                  const double[], int, \
                                  Xtype, \
                                  Xtype *, \
                                  Xtype *, \
                                  int * )) \
                       gifunc ) )( ndim_in, lbnd_in, ubnd_in, \
                                   (Xtype *) in, \
                                   (Xtype *) ( usevar ? in_var : NULL ), \
                                   npoint, offset, \
                                   (const double *const *) ptr_in, \
                                   params, flags, \
                                   *( (Xtype *) badval_ptr ), \
                                   (Xtype *) out, \
                                   (Xtype *) ( usevar ? out_var : NULL ), \
                                   &nbad ); \
                  if ( astOK ) { \
                     result += nbad; \
                  } else { \
                     astError( astStatus, "astResample"#X"(%s): Error " \
                               "signalled by user-supplied sub-pixel " \
                               "interpolation function.", \
                               astGetClass( unsimplified_mapping ) ); \
                  } \
                  break;

/* Use the above macro to invoke the function. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_GINTERP(LD,long double)
#endif
               CASE_GINTERP(D,double)
               CASE_GINTERP(F,float)
               CASE_GINTERP(L,long int)
               CASE_GINTERP(UL,unsigned long int)
               CASE_GINTERP(I,int)
               CASE_GINTERP(UI,unsigned int)
               CASE_GINTERP(S,short int)
               CASE_GINTERP(US,unsigned short int)
               CASE_GINTERP(B,signed char)
               CASE_GINTERP(UB,unsigned char)
            }
            break;

/* Undefine the macro. */
#undef CASE_GINTERP

/* Error: invalid interpolation scheme specified. */
/* ---------------------------------------------- */
         default:

/* Define a macro to report an error message appropriate to a given
   data type. */
#define CASE_ERROR(X) \
            case TYPE_##X: \
               astError( AST__SISIN, "astResample"#X"(%s): Invalid " \
                         "sub-pixel interpolation scheme (%d) specified.", \
                         astGetClass( unsimplified_mapping ), interp ); \
               break;
                                 
/* Use the above macro to report an appropriate error message. */
            switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
               CASE_ERROR(LD)
#endif
               CASE_ERROR(D)
               CASE_ERROR(F)
               CASE_ERROR(L)
               CASE_ERROR(UL)
               CASE_ERROR(I)
               CASE_ERROR(UI)
               CASE_ERROR(S)
               CASE_ERROR(US)
               CASE_ERROR(B)
               CASE_ERROR(UB)
            }
            break;

/* Undefine the macro. */
#undef CASE_ERROR
      }
   }

/* Now scale the output values to conserve flux if required. */
   if( conserve ) {

/* Define a macro to use a "case" statement to invoke the function 
   appropriate to a given data type. These simply multiple the output data
   value by the factor, and the output variance by the square of the
   factor. */
#define CASE_CONSERVE(X,Xtype) \
      case ( TYPE_##X ): \
         ConserveFlux##X( factor, npoint, offset, *( (Xtype *) badval_ptr ), \
                          (Xtype *) out, \
                          (Xtype *) ( usevar ? out_var : NULL ) ); \
         break;
       
/* Use the above macro to invoke the appropriate function. */
      switch ( type ) {
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
         CASE_CONSERVE(LD,long double)
#endif
         CASE_CONSERVE(D,double)
         CASE_CONSERVE(F,float)
         CASE_CONSERVE(L,long int)
         CASE_CONSERVE(UL,unsigned long int)
         CASE_CONSERVE(I,int)
         CASE_CONSERVE(UI,unsigned int)
         CASE_CONSERVE(S,short int)
         CASE_CONSERVE(US,unsigned short int)
         CASE_CONSERVE(B,signed char)
         CASE_CONSERVE(UB,unsigned char)
      }

/* Undefine the macro. */
#undef CASE_CONSERVE
   }               

/* Annul the PointSet used to hold input coordinates. */
   pset_in = astAnnul( pset_in );

/* Free the workspace. */
   offset = astFree( offset );
   stride = astFree( stride );

/* If an error occurred, clear the returned value. */
   if ( !astOK ) result = 0;

/* Return the result. */
   return result;
}

static int ResampleWithBlocking( AstMapping *this, const double *linear_fit,
                                 int ndim_in,
                                 const int *lbnd_in, const int *ubnd_in,
                                 const void *in, const void *in_var,
                                 DataType type, int interp, void (* finterp)(),
                                 const double *params, int flags,
                                 const void *badval_ptr, int ndim_out,
                                 const int *lbnd_out, const int *ubnd_out,
                                 const int *lbnd, const int *ubnd,
                                 void *out, void *out_var ) {
/*
*  Name:
*     ResampleWithBlocking

*  Purpose:
*     Resample a section of a data grid in a memory-efficient way.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int ResampleWithBlocking( AstMapping *this, const double *linear_fit,
*                               int ndim_in,
*                               const int *lbnd_in, const int *ubnd_in,
*                               const void *in, const void *in_var,
*                               DataType type, int interp, void (* finterp)(),
*                               const double *params, int flags,
*                               const void *badval_ptr, int ndim_out,
*                               const int *lbnd_out, const int *ubnd_out,
*                               const int *lbnd, const int *ubnd,
*                               void *out, void *out_var )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function resamples a rectangular grid of data (with any
*     number of dimensions) into a specified section of another
*     rectangular grid (with a possibly different number of
*     dimensions). The coordinate transformation used is given by the
*     inverse transformation of the Mapping which is supplied or,
*     alternatively, by a linear approximation fitted to a Mapping's
*     inverse transformation. Any pixel interpolation scheme may be
*     specified for interpolating between the pixels of the input
*     grid.
*
*     This function is very similar to ResampleSection, except that in
*     order to limit memory usage and to ensure locality of reference,
*     it divides the output grid up into "blocks" which have a limited
*     extent along each output dimension. Each block, which will not
*     contain more than a pre-determined maximum number of pixels, is
*     then passed to ResampleSection for resampling.

*  Parameters:
*     this
*        Pointer to a Mapping, whose inverse transformation may be
*        used to transform the coordinates of pixels in the output
*        grid into associated positions in the input grid, from which
*        the output pixel values should be derived (by interpolation
*        if necessary).
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     linear_fit
*        Pointer to an optional array of double which contains the
*        coefficients of a linear fit which approximates the above
*        Mapping's inverse coordinate transformation. If this is
*        supplied, it will be used in preference to the above Mapping
*        when transforming coordinates. This may be used to enhance
*        performance in cases where evaluation of the Mapping's
*        inverse transformation is expensive. If no linear fit is
*        available, a NULL pointer should be supplied.
*
*        The way in which the fit coefficients are stored in this
*        array and the number of array elements are as defined by the
*        astLinearApprox function.
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     in
*        Pointer to the input array of data to be resampled (with one
*        element for each pixel in the input grid). The numerical type
*        of these data should match the "type" value (below). The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and data type as the "in" array),
*        which represent estimates of the statistical variance
*        associated with each element of the "in" array. If this
*        second array is given (along with the corresponding "out_var"
*        array), then estimates of the variance of the resampled data
*        will also be returned.
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     type
*        A value taken from the "DataType" enum, which specifies the
*        data type of the input and output arrays containing the
*        gridded data (and variance) values.
*     interp
*        A value selected from a set of pre-defined macros to identify
*        which sub-pixel interpolation algorithm should be used.
*     finterp
*        If "interp" is set to a value which requires a user-supplied
*        function, then a pointer to that function shoild be given
*        here. Otherwise, this value is not used and may be a NULL
*        pointer.
*     params
*        Pointer to an optional array of parameters that may be passed
*        to the interpolation algorithm, if required. If no parameters
*        are required, a NULL pointer should be supplied.
*     flags
*        The bitwise OR of a set of flag values which provide
*        additional control over the resampling operation.
*     badval_ptr
*        If the AST__USEBAD flag is set (above), this parameter is a
*        pointer to a value which is used to identify bad data and/or
*        variance values in the input array(s). The referenced value's
*        data type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output data grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output data grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output data grid in the same way as "lbnd_in"
*        and "ubnd_in" define the shape and size of the input grid
*        (see above).
*     lbnd
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the first pixel in the
*        section of the output data grid for which a value is
*        required.
*     ubnd
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the last pixel in the
*        section of the output data grid for which a value is
*        required.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the output grid for which resampled values are
*        required. This section should lie wholly within the extent of
*        the output grid (as defined by the "lbnd_out" and "ubnd_out"
*        arrays). Regions of the output grid lying outside this section
*        will not be modified.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the resampled data will be returned.  The
*        storage order should be such that the coordinate of the first
*        dimension varies most rapidly and that of the final dimension
*        least rapidly (i.e. Fortran array storage order is used).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the resampled values may be returned. This array will only be
*        used if the "in_var" array has been given.
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.

*  Returned Value:
*     The number of output grid points to which a data value (or a
*     variance value if relevant) equal to "badval" has been assigned
*     because no valid output value could be obtained.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*/

/* Local Constants: */
   const int mxpix = 2 * 1024;   /* Maximum number of pixels in a block (this
                                    relatively small number seems to give best
                                    performance) */

/* Local Variables: */
   double factor;                /* Flux conservation factor */
   int *dim_block;               /* Pointer to array of block dimensions */
   int *lbnd_block;              /* Pointer to block lower bound array */
   int *ubnd_block;              /* Pointer to block upper bound array */
   int dim;                      /* Dimension size */
   int done;                     /* All blocks resampled? */
   int hilim;                    /* Upper limit on maximum block dimension */
   int idim;                     /* Loop counter for dimensions */
   int lolim;                    /* Lower limit on maximum block dimension */
   int mxdim_block;              /* Maximum block dimension */
   int npix;                     /* Number of pixels in block */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Allocate workspace. */
   lbnd_block = astMalloc( sizeof( int ) * (size_t) ndim_out );
   ubnd_block = astMalloc( sizeof( int ) * (size_t) ndim_out );
   dim_block = astMalloc( sizeof( int ) * (size_t) ndim_out );
   if ( astOK ) {

/* Find the optimum block size. */
/* ---------------------------- */
/* We first need to find the maximum extent which a block of output
   pixels may have in each dimension. We determine this by taking the
   output grid extent in each dimension and then limiting the maximum
   dimension size until the resulting number of pixels is sufficiently
   small. This approach allows the block shape to approximate (or
   match) the output grid shape when appropriate. */

/* First loop to calculate the total number of output pixels and the
   maximum output dimension size. */
      npix = 1;
      mxdim_block = 0;
      for ( idim = 0; idim < ndim_out; idim++ ) {
         dim = ubnd[ idim ] - lbnd[ idim ] + 1;
         npix *= dim;
         if ( mxdim_block < dim ) mxdim_block = dim;
      }

/* If the number of output pixels is too large for a single block, we
   perform iterations to determine the optimum upper limit on a
   block's dimension size. Initialise the limits on this result. */
      if ( npix > mxpix ) {
         lolim = 1;
         hilim = mxdim_block;

/* Loop to perform a binary chop, searching for the best result until
   the lower and upper limits on the result converge to adjacent
   values. */
         while ( ( hilim - lolim ) > 1 ) {

/* Form a new estimate from the mid-point of the previous limits. */
            mxdim_block = ( hilim + lolim ) / 2;

/* See how many pixels a block contains if its maximum dimension is
   limited to this new value. */
            for ( npix = 1, idim = 0; idim < ndim_out ; idim++ ) {
               dim = ubnd[ idim ] - lbnd[ idim ] + 1;
               npix *= ( dim < mxdim_block ) ? dim : mxdim_block;
            }

/* Update the appropriate limit, according to whether the number of
   pixels is too large or too small. */
            *( ( npix <= mxpix ) ? &lolim : &hilim ) = mxdim_block;
         }

/* When iterations have converged, obtain the maximum limit on the
   dimension size of a block which results in no more than the maximum
   allowed number of pixels per block. However, ensure that all block
   dimensions are at least 2. */
            mxdim_block = lolim;
      }
      if ( mxdim_block < 2 ) mxdim_block = 2;

/* Calculate the block dimensions by applying this limit to the output
   grid dimensions. */
      for ( idim = 0; idim < ndim_out ; idim++ ) {
         dim = ubnd[ idim ] - lbnd[ idim ] + 1;
         dim_block[ idim ] = ( dim < mxdim_block ) ? dim : mxdim_block;

/* Also initialise the lower and upper bounds of the first block of
   output grid pixels to be resampled, ensuring that this does not
   extend outside the grid itself. */
         lbnd_block[ idim ] = lbnd[ idim ];
         ubnd_block[ idim ] = MinI( lbnd[ idim ] + dim_block[ idim ] - 1,
                                    ubnd[ idim ] );
      }

/* Determine the flux conservation constant if needed. */
/* --------------------------------------------------- */
   if( ( flags & AST__CONSERVEFLUX ) && linear_fit ) {
      factor = MatrixDet( ndim_in, linear_fit + ndim_in );
   } else {
      factor = 1.0;
   }

/* Resample each block of output pixels. */
/* ------------------------------------- */
/* Loop to generate the extent of each block of output pixels and to
   resample them. */
      done = 0;
      while ( !done && astOK ) {

/* Resample the current block, accumulating the sum of bad pixels
   produced. */
         result += ResampleSection( this, linear_fit,
                                    ndim_in, lbnd_in, ubnd_in,
                                    in, in_var, type, interp, finterp, params,
                                    factor, flags, badval_ptr,
                                    ndim_out, lbnd_out, ubnd_out,
                                    lbnd_block, ubnd_block, out, out_var );

/* Update the block extent to identify the next block of output
   pixels. */
         idim = 0;
         do {

/* We find the least significant dimension where the upper bound of
   the block has not yet reached the upper bound of the region of the
   output grid which we are resampling. The block's position is then
   incremented by one block extent along this dimension, checking that
   the resulting extent does not go outside the region being
   resampled. */
            if ( ubnd_block[ idim ] < ubnd[ idim ] ) {
               lbnd_block[ idim ] = MinI( lbnd_block[ idim ] +
                                          dim_block[ idim ], ubnd[ idim ] );
               ubnd_block[ idim ] = MinI( lbnd_block[ idim ] +
                                          dim_block[ idim ] - 1,
                                          ubnd[ idim ] );
               break;

/* If any less significant dimensions are found where the upper bound
   of the block has reached its maximum value, we reset the block to
   its lowest position. */
            } else {
               lbnd_block[ idim ] = lbnd[ idim ];
               ubnd_block[ idim ] = MinI( lbnd[ idim ] + dim_block[ idim ] - 1,
                                          ubnd[ idim ] );

/* All the blocks have been processed once the position along the most
   significant dimension has been reset. */
               done = ( ++idim == ndim_out );
            }
         } while ( !done );
      }
   }

/* Free the workspace. */
   lbnd_block = astFree( lbnd_block );
   ubnd_block = astFree( ubnd_block );
   dim_block = astFree( dim_block );

/* If an error occurred, clear the returned value. */
   if ( !astOK ) result = 0;

/* Return the result. */
   return result;
}

static void SetAttrib( AstObject *this_object, const char *setting ) {
/*
*  Name:
*     astSetAttrib

*  Purpose:
*     Set an attribute value for a Mapping.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SetAttrib( AstObject *this, const char *setting )

*  Class Membership:
*     Mapping member function (over-rides the astSetAttrib protected
*     method inherited from the Object class).

*  Description:
*     This function assigns an attribute value for a Mapping, the
*     attribute and its value being specified by means of a string of
*     the form:
*
*        "attribute= value "
*
*     Here, "attribute" specifies the attribute name and should be in
*     lower case with no white space present. The value to the right
*     of the "=" should be a suitable textual representation of the
*     value to be assigned and this will be interpreted according to
*     the attribute's data type.  White space surrounding the value is
*     only significant for string attributes.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     setting
*        Pointer to a null terminated string specifying the new attribute
*        value.
*/

/* Local Variables: */
   AstMapping *this;             /* Pointer to the Mapping structure */
   int invert;                   /* Invert attribute value */
   int len;                      /* Length of setting string */
   int nc;                       /* Number of characters read by astSscanf */
   int report;                   /* Report attribute value */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the Mapping structure. */
   this = (AstMapping *) this_object;

/* Obtain the length of the setting string. */
   len = (int) strlen( setting );

/* Test for each recognised attribute in turn, using "astSscanf" to parse
   the setting string and extract the attribute value (or an offset to
   it in the case of string values). In each case, use the value set
   in "nc" to check that the entire string was matched. Once a value
   has been obtained, use the appropriate method to set it. */

/* Invert. */
/* ------- */
   if ( nc = 0,
        ( 1 == astSscanf( setting, "invert= %d %n", &invert, &nc ) )
        && ( nc >= len ) ) {
      astSetInvert( this, invert );

/* Report. */
/* ------- */
   } else if ( nc = 0,
        ( 1 == astSscanf( setting, "report= %d %n", &report, &nc ) )
        && ( nc >= len ) ) {
      astSetReport( this, report );

/* Define a macro to see if the setting string matches any of the
   read-only attributes of this class. */
#define MATCH(attrib) \
        ( nc = 0, ( 0 == astSscanf( setting, attrib "=%*[^\n]%n", &nc ) ) && \
                  ( nc >= len ) )

/* If the attribute was not recognised, use this macro to report an error
   if a read-only attribute has been specified. */
   } else if ( MATCH( "nin" ) ||
        MATCH( "nout" ) ||
        MATCH( "issimple" ) ||
        MATCH( "tranforward" ) ||
        MATCH( "traninverse" ) ) {
      astError( AST__NOWRT, "astSet: The setting \"%s\" is invalid for a %s.",
                setting, astGetClass( this ) );
      astError( AST__NOWRT, "This is a read-only attribute." );

/* If the attribute is still not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      (*parent_setattrib)( this_object, setting );
   }

/* Undefine macros local to this function. */
#undef MATCH
}

static void Sinc( double offset, const double params[], int flags,
                  double *value ) {
/*
*  Name:
*     Sinc

*  Purpose:
*     1-dimensional sinc(pi*x) interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void Sinc( double offset, const double params[], int flags,
*                double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. The function used is sinc(pi*x), where
*     sinc(z)=sin(z)/z.

*  Parameters:
*     offset
*        The offset of a pixel from the interpolation point, measured
*        in pixels.
*     params
*        Not used.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   static double pi;             /* Value of pi */
   static int init = 0;          /* Initialisation flag */

/* On the first invocation, initialise a local value for pi. Do this
   only once. */
   if ( !init ) {
      pi = acos( -1.0 );
      init = 1;
   }

/* Scale the offset. */
   offset *= pi;

/* Evaluate the function. */
   *value = ( offset != 0.0 ) ? ( sin( offset ) / offset ) : 1.0;
}

static void SincCos( double offset, const double params[], int flags,
                     double *value ) {
/*
*  Name:
*     SincCos

*  Purpose:
*     1-dimensional sinc(pi*x)*cos(k*pi*x) interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SincCos( double offset, const double params[], int flags,
*                   double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. The function used is sinc(pi*x)*cos(k*pi*x)
*     out to the point where cos(k*pi*x) = 0, and zero beyond. Here,
*     sinc(z)=sin(z)/z.

*  Parameters:
*     offset
*        The offset of a pixel from the interpolation point, measured
*        in pixels.
*     params
*        The first element of this array should give a value for "k"
*        in the cos(k*pi*x) term.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   double offset_k;              /* Scaled offset */
   static double halfpi;         /* Value of pi/2 */
   static double pi;             /* Value of pi */
   static int init = 0;          /* Initialisation flag */

/* On the first invocation, initialise local values for pi and
   pi/2. Do this only once. */
   if ( !init ) {
      pi = acos( -1.0 );
      halfpi = 0.5 * pi;
      init = 1;
   }

/* Multiply the offset by pi and remove its sign. */
   offset = pi * fabs( offset );

/* Find the offset scaled by the "k" factor. */
   offset_k = offset * params[ 0 ];

/* If the cos(k*pi*x) term has not reached zero, calculate the
   result. */
   if ( offset_k < halfpi ) {
      *value = ( ( offset != 0.0 ) ? ( sin( offset ) / offset ) : 1.0 ) *
               cos( offset_k );

/* Otherwise, the result is zero. */
   } else {
      *value = 0.0;
   }
}

static void SincGauss( double offset, const double params[], int flags,
                       double *value ) {
/*
*  Name:
*     SincGauss

*  Purpose:
*     1-dimensional sinc(pi*x)*exp(-k*x*x) interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SincGauss( double offset, const double params[], int flags,
*                     double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. The function used is sinc(pi*x)*exp(-k*x*x),
*     where sinc(z)=sin(z)/z.

*  Parameters:
*     offset
*        The offset of a pixel from the interpolation point, measured
*        in pixels.
*     params
*        The first element of this array should give a value for "k"
*        in the exp(-k*x*x) term.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   double offset_pi;             /* Offset multiplied by pi */
   static double pi;             /* Value of pi */
   static int init = 0;          /* Initialisation flag */

/* On the first invocation, initialise a local value for pi. Do this
   only once. */
   if ( !init ) {
      pi = acos( -1.0 );
      init = 1;
   }

/* Find the offset scaled by pi. */
   offset_pi = pi * offset;

/* Calculate the result. */
   *value = ( ( offset_pi != 0.0 ) ? ( sin( offset_pi ) / offset_pi ) : 1.0 ) *
            exp( -params[ 0 ] * offset * offset );
}

static void SincSinc( double offset, const double params[], int flags,
                      double *value ) {
/*
*  Name:
*     SincSinc

*  Purpose:
*     1-dimensional sinc(pi*x)*sinc(k*pi*x) interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SincSinc( double offset, const double params[], int flags,
*                    double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. The function used is sinc(pi*x)*sinc(k*pi*x),
*     out to the point where sinc(k*pi*x)=0, and zero beyond. Here,
*     sinc(z)=sin(z)/z.

*  Parameters:
*     offset
*        The offset of a pixel from the interpolation point, measured
*        in pixels.
*     params
*        The first element of this array should give a value for "k"
*        in the sinc(k*pi*x) term.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   double offset_k;              /* Scaled offset */
   static double halfpi;         /* Value of pi/2 */
   static double pi;             /* Value of pi */
   static int init = 0;          /* Initialisation flag */

/* On the first invocation, initialise local values for pi and
   pi/2. Do this only once. */
   if ( !init ) {
      pi = acos( -1.0 );
      halfpi = 0.5 * pi;
      init = 1;
   }

/* Multiply the offset by pi and remove its sign. */
   offset = pi * fabs( offset );

/* Find the offset scaled by the "k" factor. */
   offset_k = offset * params[ 0 ];
   
/* If the sinc(k*pi*x) term has not reached zero, calculate the
   result. */
   if ( offset_k < halfpi ) {
      *value = ( ( offset != 0.0 ) ? ( sin( offset ) / offset ) : 1.0 ) *
               ( ( offset_k != 0.0 ) ? ( sin( offset_k ) / offset_k ) : 1.0 );

/* Otherwise, the result is zero. */
   } else {
      *value = 0.0;
   }
}

static AstMapping *Simplify( AstMapping *this ) {
/*
*++
*  Name:
c     astSimplify
f     AST_SIMPLIFY

*  Purpose:
*     Simplify a Mapping.

*  Type:
*     Public function.

*  Synopsis:
c     #include "mapping.h"
c     AstMapping *astSimplify( AstMapping *this )
f     RESULT = AST_SIMPLIFY( THIS, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function simplifies a Mapping (which may be a compound
*     Mapping such as a CmpMap) to eliminate redundant computational
*     steps, or to merge separate steps which can be performed more
*     efficiently in a single operation.
*
*     As a simple example, a Mapping which multiplied coordinates by
*     5, and then multiplied the result by 10, could be simplified to
*     a single step which multiplied by 50. Similarly, a Mapping which
*     multiplied by 5, and then divided by 5, could be reduced to a
*     simple copying operation.
*
*     This function should typically be applied to Mappings which have
*     undergone substantial processing or have been formed by merging
*     other Mappings. It is of potential benefit, for example, in
*     reducing execution time if applied before using a Mapping to
*     transform a large number of coordinates.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the original Mapping.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Returned Value:
c     astSimplify()
f     AST_SIMPLIFY = INTEGER
*        A new pointer to the (possibly simplified) Mapping.

*  Applicability:
*     Mapping
*        This function applies to all Mappings.
*     FrameSet
*        If the supplied Mapping is a FrameSet, the returned Mapping
*        will be a copy of the supplied FrameSet in which all the
*        inter-Frame Mappings have been simplified.

*  Notes:
*     - This function can safely be applied even to Mappings which
*     cannot be simplified. If no simplification is possible, it
c     behaves exactly like astClone and returns a pointer to the
f     behaves exactly like AST_CLONE and returns a pointer to the
*     original Mapping.
*     - The Mapping returned by this function may not be independent
*     of the original (even if simplification was possible), and
*     modifying it may therefore result in indirect modification of
*     the original. If a completely independent result is required, a
c     copy should be made using astCopy.
f     copy should be made using AST_COPY.
*     - A null Object pointer (AST__NULL) will be returned if this
c     function is invoked with the AST error status set, or if it
f     function is invoked with STATUS set to an error value, or if it
*     should fail for any reason.
*--
*/

/* Local Variables: */
   AstMapping **map_list;        /* Pointer to array of Mapping pointers */
   AstMapping *map;              /* Cloned pointer to nominated Mapping */
   AstMapping *result;           /* Pointer to result Mapping */
   int *invert_list;             /* Pointer to array of invert flags */
   int imap;                     /* Loop counter for Mappings */
   int modified;                 /* Index of first modified element */
   int nmap;                     /* Number of Mappings */
   int simpler;                  /* Simplification achieved? */

/* Initialise. */
   result = NULL;

/* Check the inherited status. */
   if ( !astOK ) return result;

/* Initialise dynamic arrays of Mapping pointers and associated invert
   flags. */
   nmap = 0;
   map_list = NULL;
   invert_list = NULL;

/* Build a Mapping list to contain this Mapping (the list should only
   have 1 element). */
   astMapList( this, 1, astGetInvert( this ), &nmap, &map_list, &invert_list );

/* Pass the list repeatedly to the "astMapMerge" method for
   simplification. */
   simpler = 0;
   while ( astOK ) {
      map = astClone( map_list[ 0 ] );
      modified = astMapMerge( map, 0, 1, &nmap, &map_list, &invert_list );
      map = astAnnul( map );

/* Quit looping if the number of Mappings increases above 1, or if no
   further change occurs. Note if any simplification was achieved. */
      if ( ( nmap > 1 ) || ( modified < 0 ) ) break;
      simpler = 1;
   }

/* Check whether simplification has occurred. If not, simply clone the
   original Mapping pointer. This is what will normally happen for
   Mapping classes which inherit the default (null) "astMapMerge"
   method from this class and do not define one of their own. */
   if ( astOK ) {
      if ( !simpler || ( nmap > 1 ) ) {
         result = astClone( this );

/* If simplification occurred, test if the resulting Mapping has the
   Invert attribute value we want. If so, we can simply clone a
   pointer to it. */
      } else {
         if ( invert_list[ 0 ] == astGetInvert( map_list[ 0 ] ) ) {
            result = astClone( map_list[ 0 ] );

/* If not, we must make a copy. */
         } else {
            result = astCopy( map_list[ 0 ] );

/* Either clear the copy's Invert attribute, or set it to 1, as
   required. */
            if ( invert_list[ 0 ] ) {
               astSetInvert( result, 1 );
            } else {
               astClearInvert( result );
            }
         }
      }
   }

/* Loop to annul all the pointers in the Mapping list. */
   for ( imap = 0; imap < nmap; imap++ ) {
      map_list[ imap ] = astAnnul( map_list[ imap ] );
   }

/* Free the dynamic arrays. */
   map_list = astFree( map_list );
   invert_list = astFree( invert_list );

/* If an error occurred, annul the returned Mapping. */
   if ( !astOK ) result = astAnnul( result );

/* Return the result. */
   return result;   
}

static void Somb( double offset, const double params[], int flags,
                  double *value ) {
/*
*  Name:
*     Somb

*  Purpose:
*     1-dimensional somb(pi*x) interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void Somb( double offset, const double params[], int flags,
*                double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. The function used is somb(pi*x), where
*     somb(z)=2*J1(z)/z  (J1 is a Bessel function of the first kind of
*     order 1). 

*  Parameters:
*     offset
*        The offset of a pixel from the interpolation point, measured
*        in pixels.
*     params
*        Not used.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   static double pi;             /* Value of pi */
   static int init = 0;          /* Initialisation flag */

/* On the first invocation, initialise a local value for pi. Do this
   only once. */
   if ( !init ) {
      pi = acos( -1.0 );
      init = 1;
   }

/* Scale the offset. */
   offset *= pi;

/* Evaluate the function. */
   *value = ( offset != 0.0 ) ? ( 2.0*J1Bessel( offset ) / offset ) : 1.0;
}

static void SombCos( double offset, const double params[], int flags,
                     double *value ) {
/*
*  Name:
*     SombCos

*  Purpose:
*     1-dimensional somb(pi*x)*cos(k*pi*x) interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SombCos( double offset, const double params[], int flags,
*                   double *value )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. The function used is somb(pi*x)*cos(k*pi*x)
*     out to the point where cos(k*pi*x) = 0, and zero beyond. Here,
*     somb(z)=2*J1(z)/z  (J1 is a Bessel function of the first kind of
*     order 1). 

*  Parameters:
*     offset
*        The offset of a pixel from the interpolation point, measured
*        in pixels.
*     params
*        The first element of this array should give a value for "k"
*        in the cos(k*pi*x) term.
*     flags
*        Not used.
*     value
*        Pointer to a double to receive the calculated kernel value.

*  Notes:
*     - This function does not perform error checking and does not
*     generate errors.
*/

/* Local Variables: */
   double offset_k;              /* Scaled offset */
   static double halfpi;         /* Value of pi/2 */
   static double pi;             /* Value of pi */
   static int init = 0;          /* Initialisation flag */

/* On the first invocation, initialise local values for pi and
   pi/2. Do this only once. */
   if ( !init ) {
      pi = acos( -1.0 );
      halfpi = 0.5 * pi;
      init = 1;
   }

/* Multiply the offset by pi and remove its sign. */
   offset = pi * fabs( offset );

/* Find the offset scaled by the "k" factor. */
   offset_k = offset * params[ 0 ];

/* If the cos(k*pi*x) term has not reached zero, calculate the
   result. */
   if ( offset_k < halfpi ) {
      *value = ( ( offset != 0.0 ) ? ( J1Bessel( offset ) / offset ) : 1.0 ) *
               cos( offset_k );

/* Otherwise, the result is zero. */
   } else {
      *value = 0.0;
   }
}

static int SpecialBounds( const MapData *mapdata, double *lbnd, double *ubnd,
                          double xl[], double xu[] ) {
/*
*  Name:
*     SpecialBounds

*  Purpose:
*     Estimate coordinate bounds using special points.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int SpecialBounds( const MapData *mapdata, double *lbnd, double *ubnd,
*                        double xl[], double xu[] );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function makes a rough estimate of the lower and upper
*     bounds of a Mapping function over a constrained region of its
*     input coordinate space by transforming a set of special test
*     points. The points used lie at the corners of the constrained
*     region, at the centre of each of its faces, at its centroid, and
*     (if within the coordinate constraints) the origin.
*
*     In many practical cases, the true extrema may actually lie at
*     one or other of these points, in which case the true bounds will
*     be found. In other cases, this function only provides an
*     approximate limit on each bound (there is no way of telling if
*     this is the case, however). In either case, having these initial
*     estimates can speed subsequent searches to find the global
*     extrema as well as making that search more secure

*  Parameters:
*     mapdata
*        Pointer to a MapData structure describing the Mapping
*        function, its coordinate constraints, etc.
*     lbnd
*        Pointer to a double.  On entry, this should contain a
*        previously-obtained upper limit on the lower bound, or
*        AST__BAD if no such limit is available. On exit, it will be
*        updated with a new estimate of the lower bound, if a better
*        one has been found.
*     ubnd
*        Pointer to a double.  On entry, this should contain a
*        previously-obtained lower limit on the upper bound, or
*        AST__BAD if no such limit is available. On exit, it will be
*        updated with a new estimate of the upper bound, if a better
*        one has been found.
*     xl
*        Pointer to an array of double, with one element for each
*        input coordinate, in which to return the position of a (not
*        necessarily unique) input point at which the lower output
*        bound is reached. This array is not altered if an improved
*        estimate of the lower bound cannot be found.
*     xu
*        Pointer to an array of double, with one element for each
*        input coordinate, in which to return the position of a (not
*        necessarily unique) input point at which the upper output
*        bound is reached. This array is not altered if an improved
*        estimate of the upper bound cannot be found.

*  Returned:
*     A flag indicating if the returned values can be refined.

*/

/* Local Variables: */
   AstPointSet *pset_in;         /* PointSet for input coordinates */
   AstPointSet *pset_out;        /* PointSet for output coordinates */
   double **ptr_in;              /* Pointer to input coordinates */
   double **ptr_out;             /* Pointer to output coordinates */
   double *sxl;                  /* Secondary xl values */
   double *sxu;                  /* Secondary xu values */
   double f;                     /* Output coordinate value */
   double slbnd;                 /* Secondary lbnd value */
   double subnd;                 /* Secondary lbnd value */
   int *limit;                   /* Workspace for lower/upper limit flags */
   int bad;                      /* Output coordinate bad? */
   int coord;                    /* Loop counter for coordinates */
   int done;                     /* All corners done? */
   int face;                     /* Loop counter for faces */
   int ic;                       /* Index of corner */
   int icen;                     /* Index of centroid point */
   int ncorner;                  /* Number of corners */
   int ncoord;                   /* Number of input coordinates */
   int npoint;                   /* Number of points */
   int origin;                   /* Origin lies within bounds? */
   int point;                    /* Loop counter for points */
   int result;                   /* Returned flag */

/* Initialise */
   result = 1;

/* Initialise variables to avoid "used of uninitialised variable"
   messages from dumb compilers. */
   pset_out = NULL;

/* Obtain the number of coordinate axes and calculate the number of
   points required in order to place one at every corner of the
   constrained region of the coordinate space. */
   ncoord = mapdata->nin;
   for ( npoint = 1, coord = 0; coord < ncoord; coord++ ) npoint *= 2;

/* Also include a second point at each corner,offset slightly from the
   corner towards the centroid */
   ncorner = npoint;
   npoint *= 2;

/* Also include placing one at the centre of every face and one at the
   centroid of the constrained coordinate space. */
   npoint += 2 * ncoord + 1;
   
/* Determine if the origin lies within the bounds. If so, include it
   as a further point. */
   origin = 1;
   for ( coord = 0; coord < ncoord; coord++ ) {
      if ( ( mapdata->lbnd[ coord ] > 0.0 ) ||
           ( mapdata->ubnd[ coord ] < 0.0 ) ) {
         origin = 0;
         break;
      }
   }
   if ( origin ) npoint++;

/* Initialise secondary bounds to be the supplied primary bounds */
   slbnd = *lbnd;
   subnd = *ubnd;

/* Create workspace for ssecondary xl xu values */
   sxl = astMalloc( sizeof(double)*(size_t) ncoord );
   sxu = astMalloc( sizeof(double)*(size_t) ncoord );

/* Create a PointSet to hold the coordinates and obtain a pointer to
   its coordinate values. Also allocate workspace for calculating the
   corner coordinates. */
   pset_in = astPointSet( npoint, ncoord, "" );
   ptr_in = astGetPoints( pset_in );
   limit = astMalloc( sizeof( int ) * (size_t) ncoord );
   if ( astOK ) {
   
/* Initialise the workspace. */
      for ( coord = 0; coord < ncoord; coord++ ) limit[ coord ] = 0;

/* Loop to visit every corner. */
      point = 0;
      done = 0;
      do {

/* At each corner, translate the contents of the "limit" array
   (containing zeros and ones) into the lower or upper bound on the
   corresponding axis. This gives the coordinates of the corner, which
   we store in the input PointSet. */
         for ( coord = 0; coord < ncoord; coord++ ) {
            ptr_in[ coord ][ point ] = limit[ coord ] ?
                                       mapdata->ubnd[ coord ] :
                                       mapdata->lbnd[ coord ];
         }

/* Increment the count of points (i.e. corners). */
         point++;
      
/* Now update the limit array to identify the next corner. */
         coord = 0;
         do {

/* Flip the first zero found to become a one. This gives a new
   corner. */
            if ( !limit[ coord ] ) {
               limit[ coord ] = 1;
               break;

/* However, first flip any previous ones to become zeros and then
   examine the next element. We have processed all corners once the
   array is entirely filled with ones. */
            } else {
               limit[ coord ] = 0;
               done = ( ++coord == ncoord );
            }
         } while ( !done );
      } while ( !done );

/* Once the corners have been processed, loop to consider the centre
   of each face. */
      for ( face = 0; face < ( 2 * ncoord ); face++ ) {

/* First calculate the centroid value for each coordinate.  Then set
   one of these coordinates to the bound where the face lies. */
         for ( coord = 0; coord < ncoord; coord++ ) {
            ptr_in[ coord ][ point ] = 0.5 * ( mapdata->lbnd[ coord ] +
                                               mapdata->ubnd[ coord ] );
         }
         ptr_in[ face / 2 ][ point ] = ( face % 2 ) ?
                                       mapdata->lbnd[ face / 2 ] :
                                       mapdata->ubnd[ face / 2 ];

/* Increment the count of points. */
         point++;
      }

/* Place a point at the centroid of the constrained coordinate
   space. */
      for ( coord = 0; coord < ncoord; coord++ ) {
         ptr_in[ coord ][ point ] = 0.5 * ( mapdata->lbnd[ coord ] +
                                            mapdata->ubnd[ coord ] );
      }
      icen = point++;

/* Add a set of positions which are offset slightly from each corner
   towards the centroid. */
      for ( ic = 0; ic < ncorner; ic++ ) {
         for ( coord = 0; coord < ncoord; coord++ ) {
            ptr_in[ coord ][ point ] = 0.999*ptr_in[ coord ][ ic ] + 
                                       0.001*ptr_in[ coord ][ icen ];
         }
         point++;
      }

/* Finally, add the origin, if it lies within the constraints. */
      if ( origin ) {
         for ( coord = 0; coord < ncoord; coord++ ) {
            ptr_in[ coord ][ point ] = 0.0;
         }
      }

/* Once all the input coordinates have been calculated, transform them
   and obtain a pointer to the resulting coordinate values. */
      pset_out = astTransform( mapdata->mapping, pset_in, mapdata->forward,
                               NULL );
      ptr_out = astGetPoints( pset_out );
      if ( astOK ) {

/* Loop through each point and test if any of its transformed
   coordinates is bad. */
         for ( point = 0; point < npoint; point++ ) {
            bad = 0;
            for ( coord = 0; coord < mapdata->nout; coord++ ) {
               if ( ptr_out[ coord ][ point ] == AST__BAD ) {
                  bad = 1;
                  break;
               }
            }

/* If so, we ignore the point. Otherwise, extract the required
   coordinate. */
            f = ptr_out[ mapdata->coord ][ point ];
            if ( !bad ) {

/* Use this to update the lower and upper bounds we are seeking. If
   either bound is updated, also store the coordinates of the
   corresponding input point. */
               if ( ( *lbnd == AST__BAD ) || ( f < *lbnd ) ) {
                  *lbnd = f;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     xl[ coord ] = ptr_in[ coord ][ point ];
                  }
               }
               if ( ( *ubnd == AST__BAD ) || ( f > *ubnd ) ) {
                  *ubnd = f;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     xu[ coord ] = ptr_in[ coord ][ point ];
                  }
               }

/* If this point has a bad coord value, it may still be useful if the
   required coord value is not bad. In this case, extract the required
   coordinate. */
            } else if ( f != AST__BAD ) {

/* Use this to update secondary lower and upper bounds we are seeking.
   These will be returned if no primary values are found via the previous
   code block. */
               if ( ( slbnd == AST__BAD ) || ( f < slbnd ) ) {
                  slbnd = f;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     sxl[ coord ] = ptr_in[ coord ][ point ];
                  }
               }
               if ( ( subnd == AST__BAD ) || ( f > subnd ) ) {
                  subnd = f;
                  for ( coord = 0; coord < ncoord; coord++ ) {
                     sxu[ coord ] = ptr_in[ coord ][ point ];
                  }
               }
            }
         }

/* If no primary values could be found, use secondary values. */
         if( *lbnd == AST__BAD && *ubnd == AST__BAD ) {
            *lbnd = slbnd;
            *ubnd = subnd;
            for ( coord = 0; coord < ncoord; coord++ ) {
               xu[ coord ] = sxu[ coord ];
               xl[ coord ] = sxl[ coord ];
            }
            result = ( slbnd == AST__BAD || subnd == AST__BAD );
         }
      }
   }

/* Free workspace */
   sxl = astFree( sxl );
   sxu = astFree( sxu );

/* Annul the temporary PointSets and free the workspace. */
   pset_in = astAnnul( pset_in );
   pset_out = astAnnul( pset_out );
   limit = astFree( limit );

   return result;
}

/*
*  Name:
*     SpreadKernel1<X>

*  Purpose:
*     Rebin a data grid, using a 1-d interpolation kernel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SpreadKernel1<X>( AstMapping *this, int ndim_out,
*                           const int *lbnd_out, const int *ubnd_out,
*                           const <Xtype> *in, const <Xtype> *in_var,
*                           int npoint, const int *offset, 
*                           const double *const *coords,
*                           void (* kernel)( double, const double [], int,
*                                            double * ),
*                           int neighb, const double *params, int flags,
*                           <Xtype> badval, int npix_out, 
*                           <Xtype> *out, <Xtype> *out_var, double *work )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which rebins a rectangular region of an 
*     input grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. Each input
*     grid point may be mapped on to a position in the output grid in
*     an arbitrary way. The input and output grids may have any number
*     of dimensions, not necessarily equal.
*
*     Where the input positions given do not correspond with a pixel centre
*     in the output grid, the each input pixel value is spread out between the
*     surrounding output pixels using weights determined by a separable kernel 
*     which is the product of a 1-dimensional kernel function evaluated along 
*     each output dimension. A pointer should be supplied to the 1-dimensional 
*     kernel function to be used.

*  Parameters:
*     this
*        Pointer to the Mapping being used in the rebinning operation
*        (this is only used for constructing error messages).
*     ndim_out
*        The number of dimensions in the output grid. This should be at
*        least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output grid, its extent along a particular
*        (i'th) dimension being ubnd_out[i]-lbnd_out[i]+1 (assuming "i"
*        is zero-based). They also define the output grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be rebinned. The numerical type
*        of these data should match the function used, as given by the
*        suffix on the function name. Note that details of how the input 
*        grid maps on to this array (e.g. the storage order, number of 
*        dimensions, etc.) is arbitrary and is specified entirely by means 
*        of the "offset" array. The "in" array should therefore contain 
*        sufficient elements to accommodate the "offset" values supplied.  
*        There is no requirement that all elements of the "in" array 
*        should be rebinned, and any which are not addressed by the
*        contents of the "offset" array will be ignored.
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned. It is addressed in exactly the same way (via the 
*        "offset" array) as the "in" array. 
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of input points which are to be rebinned.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each input point, this array should contain the zero-based
*        offset in the input array(s) (i.e. the "in" and, optionally, 
*        the "in_var" arrays) from which the value to be rebinned should
*        be obtained.
*     coords
*        An array of pointers to double, with "ndim_out" elements. 
*        Element "coords[coord]" should point at the first element of 
*        an array of double (with "npoint" elements) which contains the 
*        values of coordinate number "coord" for each point being
*        rebinned. The value of coordinate number "coord" for 
*        rebinning point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices are
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding input data (and
*        variance) value will be ignored.
*     kernel
*        Pointer to the 1-dimensional kernel function to be used.
*     neighb
*        The number of neighbouring pixels in each dimension (on each
*        side of the interpolation position) which are to receive
*        contributions from the input pixel value. This value should be at 
*        least 1.
*     params
*        Pointer to an optional array of parameter values to be passed
*        to the kernel function. If no parameters are required by this 
*        function, then a NULL pointer may be supplied.
*     flags
*        The bitwise OR of a set of flag values which control the
*        operation of the function. These are chosend from:
*
*        - AST__USEBAD: indicates whether there are "bad" (i.e. missing) data 
*        in the input array(s) which must be recognised.  If this flag is not 
*        set, all input values are treated literally.
*        - AST__GENVAR: Indicates that any input variances are to be
*        ignored, and that the output variances should be generated from
*        the spread of values contributing to each output pixel.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     npix_out
*        Number of pixels in output array.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the rebinned data will be returned. The 
*        storage order should be such that the index of the first grid
*        dimension varies most rapidly and that of the final dimension 
*        least rapidly (i.e. Fortran array storage order).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the rebinned values may be returned. This array will only be
*        used if the "in_var" array has been given. The values returned 
*        are estimates of the statistical variance of the corresponding 
*        values in the "out" array, on the assumption that all errors in 
*        input grid values (in the "in" array) are statistically independent 
*        and that their variance estimates (in the "in_var" array) may 
*        simply be summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     work
*        A pointer to an array with the same data type and size as the "out" 
*        array which is used as work space. The values in the supplied
*        array are incremented on exit by the sum of the weights used
*        with each output pixel.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*/
/* Define macros to implement the function for a specific data
   type. */
#define MAKE_SPREAD_KERNEL1(X,Xtype,IntType) \
static void SpreadKernel1##X( AstMapping *this, int ndim_out, \
                              const int *lbnd_out, const int *ubnd_out, \
                              const Xtype *in, const Xtype *in_var, \
                              int npoint, const int *offset, \
                              const double *const *coords, \
                              void (* kernel)( double, const double [], \
                                               int, double * ), \
                              int neighb, const double *params, \
                              int flags, Xtype badval, int npix_out, \
                              Xtype *out, Xtype *out_var, double *work ) { \
\
/* Local Variables: */ \
   Xtype in_val;                 /* Input pixel value */ \
   Xtype c; \
   double error; \
   double **wtptr;               /* Pointer to array of weight pointers */ \
   double *filter;               /* Pointer to Nd array of filter values */ \
   double *kstart;               /* Pointer to next kernel value */ \
   double *kval;                 /* Pointer to 1d array of kernel values */ \
   double *wtprod;               /* Accumulated weight value array pointer */ \
   double **wtptr_last;          /* Array of highest weight pointer values */ \
   double *xnl;                  /* Pointer to previous ofset array (n-d) */ \
   double xxn; \
   double xxl;                   /* Previous X offset */ \
   double yyl;                   /* Previous Y offset */ \
   double pixwt;                 /* Weight to apply to individual pixel */ \
   double x;                     /* x coordinate value */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   double sum;                   /* Sum of all filter values */ \
   double wgt;                   /* Weight for input value */ \
   double xx;                    /* X offset */ \
   double yy;                    /* Y offset */ \
   double *kp;                   /* Pointer to next weight values */ \
   int *hi;                      /* Pointer to array of upper indices */ \
   int *lo;                      /* Pointer to array of lower indices */ \
   int *jhi;                     /* Pointer to array of filter upper indices */ \
   int *jlo;                     /* Pointer to array of filter lower indices */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int done;                     /* All pixel indices done? */ \
   int hi_ix;                    /* Upper output pixel index (x dimension) */ \
   int hi_iy;                    /* Upper output pixel index (y dimension) */ \
   int hi_jx;                    /* Upper filter pixel index (x dimension) */ \
   int hi_jy;                    /* Upper filter pixel index (y dimension) */ \
   int lo_ix;                    /* Lower output pixel index (x dimension) */ \
   int lo_iy;                    /* Lower output pixel index (y dimension) */ \
   int lo_jx;                    /* Lower filter pixel index (x dimension) */ \
   int lo_jy;                    /* Lower filter pixel index (y dimension) */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ii;                       /* Loop counter for dimensions */ \
   int ix;                       /* Pixel index in output grid x dimension */ \
   int jx;                       /* Pixel index in filter grid x dimension */ \
   int ixn;                      /* Pixel index in input grid (n-d) */ \
   int ixn0;                     /* First pixel index in input grid (n-d) */ \
   int iy;                       /* Pixel index in output grid y dimension */ \
   int jy;                       /* Pixel index in filter grid y dimension */ \
   int kerror;                   /* Error signalled by kernel function? */ \
   int nb2;                      /* The total number of neighbouring pixels */ \
   int nf;                       /* Number of pixels in filter array */ \
   int off1;                     /* Input pixel offset due to y index */ \
   int off_in;                   /* Offset to input pixel */ \
   int off_out;                  /* Offset to output pixel */ \
   int point;                    /* Loop counter for output points */ \
   int s;                        /* Temporary variable for strides */ \
   int genvar;                   /* Generate output variances? */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int varwgt;                   /* Use input variances as weights? */ \
   int ystride;                  /* Stride along input grid y dimension */ \
   int jxn; \
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
/* Further initialisation. */ \
   kerror = 0; \
   sum = 0.0; \
   bad = 0; \
\
/* Find the total number of pixels in the filter used to spread a single \
   input pixel into the output image. */ \
   nb2 = 2*neighb; \
   nf = 1; \
   for ( idim = 0; idim < ndim_out; idim++ ) nf *= nb2; \
\
/* Allocate workspace to hold the filter values. */ \
   filter = astMalloc( sizeof( double ) * (size_t) nf ); \
   if ( astOK ) { \
\
/* Determine if we are processing bad pixels or variances. */ \
      usebad = flags & AST__USEBAD; \
      genvar = ( flags & AST__GENVAR ) && out_var && work; \
      varwgt = ( flags & AST__VARWGT ) && in_var && work; \
      usevar = !genvar && in_var && out_var; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
      if ( ndim_out == 1 ) { \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
         if( varwgt ) { \
            if ( usebad ) { \
               if ( usevar ) { \
                  KERNEL_1D(X,Xtype,1,1,0,IntType,1) \
               } else if ( genvar ) { \
                  KERNEL_1D(X,Xtype,1,0,1,IntType,1) \
               } else { \
                  KERNEL_1D(X,Xtype,1,0,0,IntType,1) \
               } \
            } else { \
               if ( usevar ) { \
                  KERNEL_1D(X,Xtype,0,1,0,IntType,1) \
               } else if ( genvar ) { \
                  KERNEL_1D(X,Xtype,0,0,1,IntType,1) \
               } else { \
                  KERNEL_1D(X,Xtype,0,0,0,IntType,1) \
               } \
            } \
         } else { \
            if ( usebad ) { \
               if ( usevar ) { \
                  KERNEL_1D(X,Xtype,1,1,0,IntType,0) \
               } else if ( genvar ) { \
                  KERNEL_1D(X,Xtype,1,0,1,IntType,0) \
               } else { \
                  KERNEL_1D(X,Xtype,1,0,0,IntType,0) \
               } \
            } else { \
               if ( usevar ) { \
                  KERNEL_1D(X,Xtype,0,1,0,IntType,0) \
               } else if ( genvar ) { \
                  KERNEL_1D(X,Xtype,0,0,1,IntType,0) \
               } else { \
                  KERNEL_1D(X,Xtype,0,0,0,IntType,0) \
               } \
            } \
         } \
\
/* Exit point on error in kernel function */ \
         Kernel_SError_1d: ; \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
      } else if ( ndim_out == 2 ) { \
\
/* Calculate the stride along the y dimension of the output grid. */ \
         ystride = ubnd_out[ 0 ] - lbnd_out[ 0 ] + 1; \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
         if( varwgt ) { \
            if ( usebad ) { \
               if ( usevar ) { \
                  KERNEL_2D(X,Xtype,1,1,0,IntType,1) \
               } else if ( genvar ) { \
                  KERNEL_2D(X,Xtype,1,0,1,IntType,1) \
               } else { \
                  KERNEL_2D(X,Xtype,1,0,0,IntType,1) \
               } \
            } else { \
               if ( usevar ) { \
                  KERNEL_2D(X,Xtype,0,1,0,IntType,1) \
               } else if ( genvar ) { \
                  KERNEL_2D(X,Xtype,0,0,1,IntType,1) \
               } else { \
                  KERNEL_2D(X,Xtype,0,0,0,IntType,1) \
               } \
            } \
         } else { \
            if ( usebad ) { \
               if ( usevar ) { \
                  KERNEL_2D(X,Xtype,1,1,0,IntType,0) \
               } else if ( genvar ) { \
                  KERNEL_2D(X,Xtype,1,0,1,IntType,0) \
               } else { \
                  KERNEL_2D(X,Xtype,1,0,0,IntType,0) \
               } \
            } else { \
               if ( usevar ) { \
                  KERNEL_2D(X,Xtype,0,1,0,IntType,0) \
               } else if ( genvar ) { \
                  KERNEL_2D(X,Xtype,0,0,1,IntType,0) \
               } else { \
                  KERNEL_2D(X,Xtype,0,0,0,IntType,0) \
               } \
            } \
         } \
\
/* Exit point on error in kernel function */ \
         Kernel_SError_2d: ; \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
      } else { \
\
/* Allocate workspace. */ \
         hi = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
         lo = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
         jhi = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
         jlo = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
         stride = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
         xnl = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
         kval = astMalloc( sizeof( double ) * (size_t) \
                                           ( nb2 * ndim_out ) ); \
         wtprod = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
         wtptr = astMalloc( sizeof( double * ) * (size_t) ndim_out ); \
         wtptr_last = astMalloc( sizeof( double * ) * (size_t) ndim_out ); \
         if ( astOK ) { \
\
/* Calculate the stride along each dimension of the output grid. */ \
            for ( s = 1, idim = 0; idim < ndim_out; idim++ ) { \
               stride[ idim ] = s; \
               s *= ubnd_out[ idim ] - lbnd_out[ idim ] + 1; \
               xnl[ idim ] = AST__BAD; \
            } \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
            if( varwgt ) { \
               if ( usebad ) { \
                  if ( usevar ) { \
                     KERNEL_ND(X,Xtype,1,1,0,IntType,1) \
                  } else if ( genvar ) { \
                     KERNEL_ND(X,Xtype,1,0,1,IntType,1) \
                  } else { \
                     KERNEL_ND(X,Xtype,1,0,0,IntType,1) \
                  } \
               } else { \
                  if ( usevar ) { \
                     KERNEL_ND(X,Xtype,0,1,0,IntType,1) \
                  } else if ( genvar ) { \
                     KERNEL_ND(X,Xtype,0,0,1,IntType,1) \
                  } else { \
                     KERNEL_ND(X,Xtype,0,0,0,IntType,1) \
                  } \
               } \
            } else { \
               if ( usebad ) { \
                  if ( usevar ) { \
                     KERNEL_ND(X,Xtype,1,1,0,IntType,0) \
                  } else if ( genvar ) { \
                     KERNEL_ND(X,Xtype,1,0,1,IntType,0) \
                  } else { \
                     KERNEL_ND(X,Xtype,1,0,0,IntType,0) \
                  } \
               } else { \
                  if ( usevar ) { \
                     KERNEL_ND(X,Xtype,0,1,0,IntType,0) \
                  } else if ( genvar ) { \
                     KERNEL_ND(X,Xtype,0,0,1,IntType,0) \
                  } else { \
                     KERNEL_ND(X,Xtype,0,0,0,IntType,0) \
                  } \
               } \
            } \
\
/* Exit point on error in kernel function */ \
            Kernel_SError_Nd: ;\
         } \
\
/* Free the workspace. */ \
         hi = astFree( hi ); \
         lo = astFree( lo ); \
         jhi = astFree( jhi ); \
         jlo = astFree( jlo ); \
         stride = astFree( stride ); \
         xnl = astFree( xnl ); \
         kval = astFree( kval ); \
         wtprod = astFree( wtprod ); \
         wtptr = astFree( wtptr ); \
         wtptr_last = astFree( wtptr_last ); \
      } \
      filter = astFree( filter ); \
   }\
\
/* If an error occurred in the kernel function, then report a \
   contextual error message. */ \
   if ( kerror ) { \
      astError( astStatus, "astRebin"#X"(%s): Error signalled by " \
                "user-supplied 1-d interpolation kernel.", \
                astGetClass( unsimplified_mapping ) ); \
   } \
\
}




#define KERNEL_1D(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* We do not yet have a previous filter position. */ \
   xxl = AST__BAD; \
\
/* Loop round all input points which are to be rebinned. */ \
   for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
      off_in = offset[ point ]; \
      in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
      if ( Usebad ) { \
         bad = ( in_val == badval ); \
         if ( Usevar || Varwgt ) { \
            bad = bad || ( in_var[ off_in ] == badval ) \
                      || ( in_var[ off_in ] <= 0.0 ); \
         } \
      } else { \
         if ( Usevar || Varwgt ) { \
            bad = ( in_var[ off_in ] <= 0.0 ); \
         } else { \
            bad = 0; \
         } \
      } \
\
/* Obtain the x coordinate of the current point and test if it is bad. */ \
      x = coords[ 0 ][ point ]; \
      bad = bad || ( x == AST__BAD ); \
\
/* If OK, calculate the lowest and highest indices (in the x \
   dimension) of the region of neighbouring pixels that will \
   contribute to the interpolated result. Constrain these values to \
   lie within the output grid. */ \
      if ( !bad ) { \
         ix = (int) floor( x ) - neighb + 1; \
         lo_ix = MaxI( ix, lbnd_out[ 0 ] ); \
         hi_ix = MinI( ix + nb2 - 1, ubnd_out[ 0 ] ); \
\
/* Skip to the next input point if the current input point makes no \
   contribution to any output pixel. */ \
         if( lo_ix <= hi_ix ) { \
\
/* Convert these output indices to the corresponding indices \
   within a box [ 0, 2*neighb ] holding the kernel values. */ \
            lo_jx = lo_ix - ix; \
            hi_jx = hi_ix - ix; \
\
/* Use the kernel function to fill the work array with weights for all output \
   pixels whether or not they fall within the output array. At the same \
   time find the sum of all the factors. */ \
            xx = (double) ix - x; \
            if( xx != xxl ) { \
               xxl = xx; \
\
               sum = 0.0; \
               for ( jx = 0; jx < nb2; jx++ ) { \
                  ( *kernel )( xx, params, flags, &pixwt ); \
\
/* Check for errors arising in the kernel function. */ \
                  if ( !astOK ) { \
                     kerror = 1; \
                     goto Kernel_SError_1d; \
                  } \
\
/* Store the kernel factor and increment the sum of all factors. */ \
                  filter[ jx ] = pixwt; \
                  sum += pixwt; \
                  xx += 1.0; \
               } \
\
/* Ensure we do not divide by zero. */ \
               if( sum == 0.0 ) sum = 1.0; \
            } \
\
/* If we are using the input data variances as weights, calculate the \
   total weight, incorporating the normalisation factor for the kernel. */ \
            if( Varwgt ) { \
               wgt = 1.0/(sum*in_var[ off_in ]); \
\
/* If we are not using input variances as weights, the weight is just the \
   kernel normalisation factor. */ \
            } else { \
               wgt = 1.0/sum; \
            } \
\
/* Loop round all the output pixels which receive contributions from this \
   input pixel, calculating the offset of each pixel from the start of the \
   input array. */ \
            off_out = lo_ix - lbnd_out[ 0 ]; \
            for ( jx = lo_jx; jx <= hi_jx; jx++, off_out++ ) { \
\
/* Retrieve the weight for the current output pixel and normalise it. */ \
               pixwt = wgt*filter[ jx ]; \
\
/* Update the output pixel with the required fraction of the input pixel \
   value. */ \
               c = CONV(IntType,in_val*pixwt); \
               out[ off_out ] += c; \
               if( work ) work[ off_out ] += pixwt; \
\
               if ( Usevar ) { \
                  out_var[ off_out ] += CONV(IntType,in_var[ off_in ]*pixwt*pixwt); \
               } else if ( Genvar && pixwt != 0.0 ) { \
                  out_var[ off_out ] += c*c/pixwt; \
                  work[ off_out + npix_out ] += pixwt*pixwt; \
               } \
\
            } \
         } \
      } \
   }




#define KERNEL_2D(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* We do not yet have a previous filter position. */ \
   xxl = AST__BAD; \
   yyl = AST__BAD; \
\
/* Loop round all input points which are to be rebinned. */ \
   for( point = 0; point < npoint; point++ ) { \
      error = 0.0; \
\
/* Obtain the input data value which is to be added into the output array. */ \
      off_in = offset[ point ]; \
      in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
      if ( Usebad ) { \
         bad = ( in_val == badval ); \
         if ( Usevar || Varwgt ) { \
            bad = bad || ( in_var[ off_in ] == badval ) \
                      || ( in_var[ off_in ] <= 0.0 ); \
         } \
      } else { \
         if ( Usevar || Varwgt ) { \
            bad = ( in_var[ off_in ] <= 0.0 ); \
         } else { \
            bad = 0; \
         } \
      } \
\
/* Obtain the x coordinate of the current point and test if it is bad. */ \
      x = coords[ 0 ][ point ]; \
      bad = bad || ( x == AST__BAD ); \
      if ( !bad ) { \
\
/* Similarly obtain and test the y coordinate. */ \
         y = coords[ 1 ][ point ]; \
         bad = ( y == AST__BAD ); \
         if ( !bad ) { \
\
/* If OK, calculate the lowest and highest indices (in each dimension) \
   of the region of neighbouring output pixels which will receive \
   contributions from the current input pixel. Constrain these values \
   to lie within the input grid. */ \
            ix = (int) floor( x ) - neighb + 1; \
            lo_ix = MaxI( ix, lbnd_out[ 0 ] ); \
            hi_ix = MinI( ix + nb2 - 1, ubnd_out[ 0 ] ); \
            iy = (int) floor( y ) - neighb + 1; \
            lo_iy = MaxI( iy, lbnd_out[ 1 ] ); \
            hi_iy = MinI( iy + nb2 - 1, ubnd_out[ 1 ] ); \
\
/* Skip to the next input point if the current input point makes no \
   contribution to any output pixel. */ \
            if( lo_ix <= hi_ix && lo_iy <= hi_iy ) { \
\
/* Convert these output indices to the corresponding indices \
   within a box [ 0:2*neighb, 0:2*neighb ] holding the kernel values. */ \
               lo_jx = lo_ix - ix; \
               hi_jx = hi_ix - ix; \
               lo_jy = lo_iy - iy; \
               hi_jy = hi_iy - iy; \
\
/* Loop to evaluate the kernel function along the y dimension, storing \
   the resulting weight values in all elements of each associated row \
   in the kvar array. The function's argument is the offset of the \
   output pixel (along this dimension) from the central output \
   position. */ \
               yy = (double) iy - y; \
               xx = (double) ix - x; \
               if( xx != xxl || yy != yyl ) { \
                  xxl = xx; \
                  yyl = yy; \
\
                  kp = filter; \
                  for ( jy = 0; jy < nb2; jy++ ) { \
                     ( *kernel )( yy, params, flags, &pixwt ); \
\
/* Check for errors arising in the kernel function. */ \
                     if ( !astOK ) { \
                        kerror = 1; \
                        goto Kernel_SError_2d; \
                     } \
\
/* Store the kernel factor in all elements of the current row. */ \
                     for( jx = 0; jx < nb2; jx++ ) *(kp++) = pixwt; \
\
/* Move on to the next row. */ \
                     yy += 1.0; \
                  } \
\
/* Loop to evaluate the kernel function along the x dimension, multiplying \
   the resulting weight values by the values already stored in the the \
   associated column in the kvar array. The function's argument is the \
   offset of the output pixel (along this dimension) from the central output \
   position. Also form the total data sum in the filter array. */ \
                  sum = 0.0; \
                  for ( jx = 0; jx < nb2; jx++ ) { \
                     ( *kernel )( xx, params, flags, &pixwt ); \
\
/* Check for errors arising in the kernel function. */ \
                     if ( !astOK ) { \
                        kerror = 1; \
                        goto Kernel_SError_2d; \
                     } \
\
/* Multiply the kernel factor by all elements of the current column. */ \
                     kp = filter + jx; \
                     for( jy = 0; jy < nb2; jy++, kp += nb2 ) { \
                        *kp *= pixwt; \
                        sum += *kp; \
                     } \
\
/* Move on to the next column. */ \
                     xx += 1.0; \
                  } \
\
/* Ensure we do not divide by zero. */ \
                  if( sum == 0.0 ) sum = 1.0; \
               } \
\
/* If we are using the input data variances as weights, calculate the \
   total weight, incorporating the normalisation factor for the kernel. */ \
               if( Varwgt ) { \
                  wgt = 1.0/(sum*in_var[ off_in ]); \
\
/* If we are not using input variances as weights, the weight is just the \
   kernel normalisation factor. */ \
               } else { \
                  wgt = 1.0/sum; \
               } \
\
/* Find the offset into the output array at the first modified output pixel \
   in the first modified row. */ \
               off1 = lo_ix - lbnd_out[ 0 ] + ystride * ( lo_iy - lbnd_out[ 1 ] ); \
\
/* Loop over the affected output rows again. */ \
               for ( jy = lo_jy; jy <= hi_jy; jy++, off1 += ystride ) { \
\
/* Save the offset of the first output pixel to be modified in the \
   current row. */ \
                  off_out = off1; \
\
/* Get a pointer to the first weight value which will be used. */ \
                  kp = filter + lo_jx + jy*nb2; \
\
/* Loop over the affected output columns again. */ \
                  for ( jx = lo_jx; jx <= hi_jx; jx++, off_out++, kp++ ) { \
\
/* Calculate the weight for this output pixel and normalise it. */ \
                     pixwt = wgt*( *kp ); \
\
/* Update the output pixel with the required fraction of the input pixel \
   value. */ \
                     c = CONV(IntType,in_val*pixwt); \
                     out[ off_out ] += c; \
                     if( work ) work[ off_out ] += pixwt; \
                     if ( Usevar ) { \
                        out_var[ off_out ] += CONV(IntType,in_var[ off_in ]*pixwt*pixwt); \
                     } else if ( Genvar && pixwt != 0.0 ) { \
                        out_var[ off_out ] += c*c/pixwt; \
                        work[ off_out + npix_out ] += pixwt*pixwt; \
                     } \
                  } \
               } \
            } \
         } \
      } \
   }




#define KERNEL_ND(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* We do not yet have a normalising factor */ \
   sum = AST__BAD; \
\
/* Loop round all input points which are to be rebinned. */ \
   for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
      off_in = offset[ point ]; \
      in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
      if ( Usebad ) { \
         bad = ( in_val == badval ); \
         if ( Usevar || Varwgt ) { \
            bad = bad || ( in_var[ off_in ] == badval ) \
                      || ( in_var[ off_in ] <= 0.0 ); \
         } \
      } else { \
         if ( Usevar || Varwgt ) { \
            bad = ( in_var[ off_in ] <= 0.0 ); \
         } else { \
            bad = 0; \
         } \
      } \
\
/* Initialise offsets into the output array. Then loop to obtain each \
   coordinate associated with the current output point. Set a flag \
   indicating if any output pixel will be modified. */ \
      if( !bad ) { \
         off_out = 0; \
         for ( idim = 0; idim < ndim_out; idim++ ) { \
            xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate is bad. If true, the corresponding output pixel \
   value will be bad, so give up on this point. */ \
            bad = ( xn == AST__BAD ); \
            if ( bad ) break; \
\
/* Calculate the lowest and highest indices (in the current dimension) \
   of the region of neighbouring output pixels that will be modified. \
   Constrain these values to lie within the output grid. */ \
            ixn = (int) floor( xn ); \
            ixn0 = ixn - neighb + 1; \
            lo[ idim ] = MaxI( ixn0, lbnd_out[ idim ] ); \
            hi[ idim ] = MinI( ixn + neighb, ubnd_out[ idim ] ); \
            jlo[ idim ] = lo[ idim ] - ixn0; \
            jhi[ idim ] = hi[ idim ] - ixn0; \
\
/* Check there is some overlap with the output array on this axis. */ \
            if( lo[ idim ] > hi[ idim ] ) { \
               bad = 1; \
               break; \
            } \
\
/* Accumulate the offset (from the start of the output array) of the \
   modified output pixel which has the lowest index in each dimension. */ \
            off_out += stride[ idim ] * ( lo[ idim ] - lbnd_out[ idim ] ); \
\
/* Set up an array of pointers to locate the first filter pixel (stored in the \
   "kval" array) for each dimension. */ \
            wtptr[ idim ] = kval + nb2*idim; \
            wtptr_last[ idim ] = wtptr[ idim ] + nb2 - 1; \
\
/* Loop to evaluate the kernel function along each dimension, storing \
   the resulting values. The function's argument is the offset of the \
   output pixel (along the relevant dimension) from the central output \
   point. */ \
            xxn = (double) ( ixn - neighb + 1 ) - xn; \
            if( xxn != xnl[ idim ] ) { \
               sum = AST__BAD; \
               xnl[ idim ] = xxn; \
               for ( jxn = 0; jxn < nb2; jxn++ ) { \
                  ( *kernel )( xxn, params, flags, wtptr[ idim ] + jxn ); \
\
/* Check for errors arising in the kernel function. */ \
                  if ( !astOK ) { \
                     kerror = 1; \
                     goto Kernel_SError_Nd; \
                  } \
\
/* Increment the kernel position. */ \
                  xxn += 1.0; \
               } \
            } \
         } \
\
/* If OK... */ \
         if ( !bad ) { \
\
/* We only need to modify the normalising factor if the weight values \
   have changed. */ \
            if( sum == AST__BAD ) { \
\
/* The kernel value to use for each output pixel is the product of the \
   kernel values for each individual axis at that point. To conserve \
   flux we need to make sure that the sum of these kernel products is unity. \
   So loop over the values now to find the total sum of all kernel values. */ \
               idim = ndim_out - 1; \
               wtprod[ idim ] = 1.0; \
               done = 0; \
               sum = 0; \
               do { \
\
/* Each modified output pixel has a weight equal to the product of the kernel \
   weight factors evaluated along each input dimension. However, since \
   we typically only change the index of one dimension at a time, we \
   can avoid forming this product repeatedly by retaining an array of \
   accumulated products for all higher dimensions. We need then only \
   update the lower elements in this array, corresponding to those \
   dimensions whose index has changed. We do this here, "idim" being \
   the index of the most significant dimension to have changed. Note \
   that on the first pass, all dimensions are considered changed, \
   causing this array to be initialised. */ \
                  for ( ii = idim; ii >= 1; ii-- ) { \
                     wtprod[ ii - 1 ] = wtprod[ ii ] * *( wtptr[ ii ] ); \
                  } \
\
/* Obtain the weight of each pixel from the accumulated product of \
   weights. Also multiply by the weight for dimension zero, which is not \
   included in the "wtprod" array). Increment the sum of all weights. */ \
                  sum += wtprod[ 0 ] * *( wtptr[ 0 ] ); \
\
/* Now update the weight value pointers and pixel offset to refer to \
   the next output pixel to be considered. */ \
                  idim = 0; \
                  do { \
\
/* The first input dimension whose weight value pointer has not yet \
   reached its final value has this pointer incremented. */ \
                     if ( wtptr[ idim ] != wtptr_last[ idim ] ) { \
                        wtptr[ idim ]++; \
                        break; \
\
/* Any earlier dimensions (which have reached the final pointer value) \
   have this pointer returned to its lowest value. */ \
                     } else { \
                        wtptr[ idim ] -= nb2 - 1; \
                        done = ( ++idim == ndim_out ); \
                     } \
                  } while ( !done ); \
               } while ( !done ); \
\
/* Ensure we do not divide by zero. */ \
               if( sum == 0.0 ) sum = 1.0; \
            } \
\
/* Re-initialise the weights pointers to refer to the first and last \
   filter pixels which overlaps the output array. */ \
            kstart = kval; \
            for ( idim = 0; idim < ndim_out; idim++ ) { \
               wtptr[ idim ] = kstart + jlo[ idim ]; \
               wtptr_last[ idim ] = kstart + jhi[ idim ]; \
               kstart += nb2; \
            } \
\
/* If we are using the input data variances as weights, calculate the \
   total weight, incorporating the normalisation factor for the kernel. */ \
            if( Varwgt ) { \
               wgt = 1.0/(sum*in_var[ off_in ]); \
\
/* If we are not using input variances as weights, the weight is just the \
   kernel normalisation factor. */ \
            } else { \
               wgt = 1.0/sum; \
            } \
\
/* Initialise, and loop over the neighbouring output pixels to divide up \
   the input pixel value between them. */ \
            idim = ndim_out - 1; \
            wtprod[ idim ] = 1.0; \
            done = 0; \
            do { \
\
/* Each modified output pixel has a weight equal to the product of the kernel \
   weight factors evaluated along each input dimension. However, since \
   we typically only change the index of one dimension at a time, we \
   can avoid forming this product repeatedly by retaining an array of \
   accumulated products for all higher dimensions. We need then only \
   update the lower elements in this array, corresponding to those \
   dimensions whose index has changed. We do this here, "idim" being \
   the index of the most significant dimension to have changed. Note \
   that on the first pass, all dimensions are considered changed, \
   causing this array to be initialised. */ \
               for ( ii = idim; ii >= 1; ii-- ) { \
                  wtprod[ ii - 1 ] = wtprod[ ii ] * *( wtptr[ ii ] ); \
               } \
\
/* Obtain the weight of each pixel from the accumulated \
   product of weights. Also multiply by the weight for dimension zero, \
   which is not included in the "wtprod" array). */ \
               pixwt = ( wtprod[ 0 ] * *( wtptr[ 0 ] ) )*wgt; \
\
/* Update the output pixel with the required fraction of the input pixel \
   value. */ \
               c = CONV(IntType,in_val*pixwt); \
               out[ off_out ] += c; \
               if( work ) work[ off_out ] += pixwt; \
               if ( Usevar ) { \
                  out_var[ off_out ] += CONV(IntType,in_var[ off_in ]*pixwt*pixwt); \
               } else if ( Genvar && pixwt != 0.0 ) { \
                  out_var[ off_out ] += c*c/pixwt; \
                  work[ off_out + npix_out ] += pixwt*pixwt; \
               } \
\
/* Now update the weight value pointers and pixel offset to refer to \
   the next output pixel to be considered. */ \
               idim = 0; \
               do { \
\
/* The first input dimension whose weight value pointer has not yet \
   reached its final value has this pointer incremented, and the pixel \
   offset into the input array is updated accordingly. */ \
                  if ( wtptr[ idim ] != wtptr_last[ idim ] ) { \
                     wtptr[ idim ]++; \
                     off_out += stride[ idim ]; \
                     break; \
\
/* Any earlier dimensions (which have reached the final pointer value) \
   have this pointer returned to its lowest value. Again, the pixel \
   offset into the input image is updated accordingly. */ \
                  } else { \
                     wtptr[ idim ] -= ( hi[ idim ] - lo[ idim ] ); \
                     off_out -= stride[ idim ] * \
                                     ( hi[ idim ] - lo[ idim ] ); \
                     done = ( ++idim == ndim_out ); \
                  } \
               } while ( !done ); \
            } while ( !done ); \
         } \
      } \
   }


/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_SPREAD_KERNEL1(LD,long double,0)
#endif     
MAKE_SPREAD_KERNEL1(D,double,0) 
MAKE_SPREAD_KERNEL1(F,float,0)
MAKE_SPREAD_KERNEL1(I,int,1)

/* Undefine the macros used above. */
#undef KERNEL_ND
#undef KERNEL_2D
#undef KERNEL_1D
#undef MAKE_SPREAD_KERNEL1

/*
*  Name:
*     SpreadLinear<X>

*  Purpose:
*     Rebin a data grid, using the linear spreading scheme.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SpreadLinear<X>( int ndim_out,
*                           const int *lbnd_out, const int *ubnd_out,
*                           const <Xtype> *in, const <Xtype> *in_var,
*                           int npoint, const int *offset, 
*                           const double *const *coords,
*                           int flags, <Xtype> badval, int npix_out,
*                           <Xtype> *out, <Xtype> *out_var, double *work ) 

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which rebins a rectangular region of an 
*     input grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. Each input
*     grid point may be mapped on to a position in the output grid in
*     an arbitrary way. Where the positions given do not correspond
*     with a pixel centre in the input grid, the spreading scheme
*     used divides the input pixel value up linearly between the 
*     nearest neighbouring output pixels in each dimension (there are 2 
*     nearest neighbours in 1 dimension, 4 in 2 dimensions, 8 in 3 
*     dimensions, etc.).

*  Parameters:
*     ndim_out
*        The number of dimensions in the output grid. This should be at
*        least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output grid, its extent along a particular
*        (i'th) dimension being ubnd_out[i]-lbnd_out[i]+1 (assuming "i"
*        is zero-based). They also define the output grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be rebinned. The numerical type
*        of these data should match the function used, as given by the
*        suffix on the function name. Note that details of how the input 
*        grid maps on to this array (e.g. the storage order, number of 
*        dimensions, etc.) is arbitrary and is specified entirely by means 
*        of the "offset" array. The "in" array should therefore contain 
*        sufficient elements to accommodate the "offset" values supplied.  
*        There is no requirement that all elements of the "in" array 
*        should be rebinned, and any which are not addressed by the
*        contents of the "offset" array will be ignored.
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned. It is addressed in exactly the same way (via the 
*        "offset" array) as the "in" array. 
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of input points which are to be rebinned.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each input point, this array should contain the zero-based
*        offset in the input array(s) (i.e. the "in" and, optionally, 
*        the "in_var" arrays) from which the value to be rebinned should
*        be obtained.
*     coords
*        An array of pointers to double, with "ndim_out" elements. 
*        Element "coords[coord]" should point at the first element of 
*        an array of double (with "npoint" elements) which contains the 
*        values of coordinate number "coord" for each point being
*        rebinned. The value of coordinate number "coord" for 
*        rebinning point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices are
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding input data (and
*        variance) value will be ignored.
*        The bitwise OR of a set of flag values which control the
*        operation of the function. These are chosend from:
*
*        - AST__USEBAD: indicates whether there are "bad" (i.e. missing) data 
*        in the input array(s) which must be recognised.  If this flag is not 
*        set, all input values are treated literally.
*        - AST__GENVAR: Indicates that any input variances are to be
*        ignored, and that the output variances should be generated from
*        the spread of values contributing to each output pixel.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     npix_out
*        Number of pixels in output array.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the rebinned data will be returned. The 
*        storage order should be such that the index of the first grid
*        dimension varies most rapidly and that of the final dimension 
*        least rapidly (i.e. Fortran array storage order).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the rebinned values may be returned. This array will only be
*        used if the "in_var" array has been given. The values returned 
*        are estimates of the statistical variance of the corresponding 
*        values in the "out" array, on the assumption that all errors in 
*        input grid values (in the "in" array) are statistically independent 
*        and that their variance estimates (in the "in_var" array) may 
*        simply be summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     work
*        An optional pointer to a double array with the same size as 
*        the "out" array. The contents of this array (if supplied) are
*        incremented by the accumulated weights assigned to each output pixel.
*        If no accumulated weights are required, a NULL pointer should be 
*        given.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*/
/* Define macros to implement the function for a specific data
   type. */
#define MAKE_SPREAD_LINEAR(X,Xtype,IntType) \
static void SpreadLinear##X( int ndim_out, \
                            const int *lbnd_out, const int *ubnd_out, \
                            const Xtype *in, const Xtype *in_var, \
                            int npoint, const int *offset, \
                            const double *const *coords, \
                            int flags, Xtype badval, int npix_out, \
                            Xtype *out, Xtype *out_var, double *work ) { \
\
/* Local Variables: */ \
   Xtype c;                      /* Contribution to output value */ \
   Xtype in_val;                 /* Input value */ \
   double *frac_hi;              /* Pointer to array of weights */ \
   double *frac_lo;              /* Pointer to array of weights */ \
   double *wt;                   /* Pointer to array of weights */ \
   double *wtprod;               /* Array of accumulated weights pointer */ \
   double *xn_max;               /* Pointer to upper limits array (n-d) */ \
   double *xn_min;               /* Pointer to lower limits array (n-d) */ \
   double f;                     /* Total pixel weight */ \
   double frac_hi_x;             /* Pixel weight (x dimension) */ \
   double frac_hi_y;             /* Pixel weight (y dimension) */ \
   double frac_lo_x;             /* Pixel weight (x dimension) */ \
   double frac_lo_y;             /* Pixel weight (y dimension) */ \
   double wgt;                   /* Weight for input value */ \
   double x;                     /* x coordinate value */ \
   double xmax;                  /* x upper limit */ \
   double xmin;                  /* x lower limit */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   double ymax;                  /* y upper limit */ \
   double ymin;                  /* y lower limit */ \
   int *dim;                     /* Pointer to array of pixel indices */ \
   int *hi;                      /* Pointer to array of upper indices */ \
   int *lo;                      /* Pointer to array of lower indices */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int bad_var;                  /* Output variance bad? */ \
   int done;                     /* All pixel indices done? */ \
   int genvar;                   /* Generate output variances? */ \
   int hi_x;                     /* Upper pixel index (x dimension) */ \
   int hi_y;                     /* Upper pixel index (y dimension) */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ii;                       /* Loop counter for weights */ \
   int ixn;                      /* Pixel index (n-d) */ \
   int lo_x;                     /* Lower pixel index (x dimension) */ \
   int lo_y;                     /* Lower pixel index (y dimension) */ \
   int off;                      /* Total offset to input pixel */ \
   int off_in;                   /* Offset to input pixel */ \
   int off_lo;                   /* Offset to "first" input pixel */ \
   int off_out;                  /* Offset to output pixel */ \
   int point;                    /* Loop counter for output points */ \
   int s;                        /* Temporary variable for strides */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int varwgt;                   /* Use input variances as weights? */ \
   int ystride;                  /* Stride along input grid y dimension */ \
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
/* Initialise variables to avoid "used of uninitialised variable" \
   messages from dumb compilers. */ \
   bad = 0; \
   bad_var = 0; \
\
/* Determine if we are processing bad pixels or variances. */ \
   usebad = flags & AST__USEBAD; \
   genvar = ( flags & AST__GENVAR ) && out_var && work; \
   varwgt = ( flags & AST__VARWGT ) && in_var && work; \
   usevar = !genvar && in_var && out_var; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   if ( ndim_out == 1 ) { \
\
/* Calculate the coordinate limits of the input grid. */ \
      xmin = (double) lbnd_out[ 0 ] - 0.5; \
      xmax = (double) ubnd_out[ 0 ] + 0.5; \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
      if( varwgt ) { \
         if ( usebad ) { \
            if ( usevar ) { \
               LINEAR_1D(X,Xtype,1,1,0,IntType,1) \
            } else if ( genvar ) { \
               LINEAR_1D(X,Xtype,1,0,1,IntType,1) \
            } else { \
               LINEAR_1D(X,Xtype,1,0,0,IntType,1) \
            } \
         } else { \
            if ( usevar ) { \
               LINEAR_1D(X,Xtype,0,1,0,IntType,1) \
            } else if ( genvar ) { \
               LINEAR_1D(X,Xtype,0,0,1,IntType,1) \
            } else { \
               LINEAR_1D(X,Xtype,0,0,0,IntType,1) \
            } \
         } \
      } else { \
         if ( usebad ) { \
            if ( usevar ) { \
               LINEAR_1D(X,Xtype,1,1,0,IntType,0) \
            } else if ( genvar ) { \
               LINEAR_1D(X,Xtype,1,0,1,IntType,0) \
            } else { \
               LINEAR_1D(X,Xtype,1,0,0,IntType,0) \
            } \
         } else { \
            if ( usevar ) { \
               LINEAR_1D(X,Xtype,0,1,0,IntType,0) \
            } else if ( genvar ) { \
               LINEAR_1D(X,Xtype,0,0,1,IntType,0) \
            } else { \
               LINEAR_1D(X,Xtype,0,0,0,IntType,0) \
            } \
         } \
      } \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   } else if ( ndim_out == 2 ) { \
\
/* Calculate the stride along the y dimension of the output grid. */ \
      ystride = ubnd_out[ 0 ] - lbnd_out[ 0 ] + 1; \
\
/* Calculate the coordinate limits of the output grid in each \
   dimension. */ \
      xmin = (double) lbnd_out[ 0 ] - 0.5; \
      xmax = (double) ubnd_out[ 0 ] + 0.5; \
      ymin = (double) lbnd_out[ 1 ] - 0.5; \
      ymax = (double) ubnd_out[ 1 ] + 0.5; \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
      if( varwgt ) { \
         if ( usebad ) { \
            if ( usevar ) { \
               LINEAR_2D(X,Xtype,1,1,0,IntType,1) \
            } else if ( genvar ) { \
               LINEAR_2D(X,Xtype,1,0,1,IntType,1) \
            } else { \
               LINEAR_2D(X,Xtype,1,0,0,IntType,1) \
            } \
         } else { \
            if ( usevar ) { \
               LINEAR_2D(X,Xtype,0,1,0,IntType,1) \
            }else if ( genvar ) { \
               LINEAR_2D(X,Xtype,0,0,1,IntType,1) \
            } else { \
               LINEAR_2D(X,Xtype,0,0,0,IntType,1) \
            } \
         } \
      } else { \
         if ( usebad ) { \
            if ( usevar ) { \
               LINEAR_2D(X,Xtype,1,1,0,IntType,0) \
            } else if ( genvar ) { \
               LINEAR_2D(X,Xtype,1,0,1,IntType,0) \
            } else { \
               LINEAR_2D(X,Xtype,1,0,0,IntType,0) \
            } \
         } else { \
            if ( usevar ) { \
               LINEAR_2D(X,Xtype,0,1,0,IntType,0) \
            }else if ( genvar ) { \
               LINEAR_2D(X,Xtype,0,0,1,IntType,0) \
            } else { \
               LINEAR_2D(X,Xtype,0,0,0,IntType,0) \
            } \
         } \
      } \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
   } else { \
\
/* Allocate workspace. */ \
      dim = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
      frac_hi = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      frac_lo = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      hi = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
      lo = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
      stride = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
      wt = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      wtprod = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      xn_max = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      xn_min = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      if ( astOK ) { \
\
/* Calculate the stride along each dimension of the output grid. */ \
         for ( s = 1, idim = 0; idim < ndim_out; idim++ ) { \
            stride[ idim ] = s; \
            s *= ubnd_out[ idim ] - lbnd_out[ idim ] + 1; \
\
/* Calculate the coordinate limits of the output grid in each \
   dimension. */ \
            xn_min[ idim ] = (double) lbnd_out[ idim ] - 0.5; \
            xn_max[ idim ] = (double) ubnd_out[ idim ] + 0.5; \
         } \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
         if( varwgt ) { \
            if ( usebad ) { \
               if ( usevar ) { \
                  LINEAR_ND(X,Xtype,1,1,0,IntType,1) \
               } else if ( genvar ) { \
                  LINEAR_ND(X,Xtype,1,0,1,IntType,1) \
               } else { \
                  LINEAR_ND(X,Xtype,1,0,0,IntType,1) \
               } \
            } else { \
               if ( usevar ) { \
                  LINEAR_ND(X,Xtype,0,1,0,IntType,1) \
               } else if ( genvar ) { \
                  LINEAR_ND(X,Xtype,0,0,1,IntType,1) \
               } else { \
                  LINEAR_ND(X,Xtype,0,0,0,IntType,1) \
               } \
            } \
         } else { \
            if ( usebad ) { \
               if ( usevar ) { \
                  LINEAR_ND(X,Xtype,1,1,0,IntType,0) \
               } else if ( genvar ) { \
                  LINEAR_ND(X,Xtype,1,0,1,IntType,0) \
               } else { \
                  LINEAR_ND(X,Xtype,1,0,0,IntType,0) \
               } \
            } else { \
               if ( usevar ) { \
                  LINEAR_ND(X,Xtype,0,1,0,IntType,0) \
               } else if ( genvar ) { \
                  LINEAR_ND(X,Xtype,0,0,1,IntType,0) \
               } else { \
                  LINEAR_ND(X,Xtype,0,0,0,IntType,0) \
               } \
            } \
         } \
      } \
\
/* Free the workspace. */ \
      dim = astFree( dim ); \
      frac_hi = astFree( frac_hi ); \
      frac_lo = astFree( frac_lo ); \
      hi = astFree( hi ); \
      lo = astFree( lo ); \
      stride = astFree( stride ); \
      wt = astFree( wt ); \
      wtprod = astFree( wtprod ); \
      xn_max = astFree( xn_max ); \
      xn_min = astFree( xn_min ); \
   } \
\
}






#define LINEAR_1D(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* Loop round all input points which are to be rebinned. */ \
   for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
      off_in = offset[ point ]; \
      in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
      if ( Usebad ) { \
         bad = ( in_val == badval ); \
         if ( Usevar || Varwgt ) { \
            bad = bad || ( in_var[ off_in ] == badval ) \
                      || ( in_var[ off_in ] <= 0.0 ); \
         } \
      } else { \
         if ( Usevar || Varwgt ) { \
            bad = ( in_var[ off_in ] <= 0.0 ); \
         } else { \
            bad = 0; \
         } \
      } \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the output grid. Also test if it is bad. */ \
      x = coords[ 0 ][ point ]; \
      bad = bad || ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
\
/* If OK, obtain the indices along the output grid x dimension of the \
   two adjacent output pixels which will receive contributions from the \
   input pixel. Also obtain the fractional weight to be applied to each of \
   these pixels. */ \
      if ( !bad ) { \
         lo_x = (int) floor( x ); \
         hi_x = lo_x + 1; \
         frac_lo_x = (double) hi_x - x; \
         frac_hi_x = 1.0 - frac_lo_x; \
\
/* Obtain the offset within the output array of the first pixel to be \
   updated (the one with the smaller index). */ \
         off_lo = lo_x - lbnd_out[ 0 ]; \
\
/* If we are using the input data variances as weights, calculate the \
   weight, and scale the fractions of each input pixel by the weight. */ \
         if( Varwgt ) { \
            wgt = 1.0/in_var[ off_in ]; \
            frac_lo_x *= wgt; \
            frac_hi_x *= wgt; \
         } \
\
/* For each of the two pixels which may be updated, test if the pixel index \
   lies within the output grid. Where it does, update the output pixel \
   with the required fraction of the input pixel value. */ \
         if ( lo_x >= lbnd_out[ 0 ] ) { \
            c = CONV(IntType,in_val*frac_lo_x); \
            out[ off_lo ] += c; \
            if( work ) work[ off_lo ] += frac_lo_x; \
            if ( Usevar ) { \
               out_var[ off_lo ] += CONV(IntType,in_var[ off_in ]*frac_lo_x*frac_lo_x); \
            } else if ( Genvar && frac_lo_x != 0.0 ) { \
               out_var[ off_lo ] += c*c/frac_lo_x; \
               work[ off_lo + npix_out ] += frac_lo_x*frac_lo_x; \
            } \
         } \
         if ( hi_x <= ubnd_out[ 0 ] ) { \
            c = CONV(IntType,in_val*frac_hi_x); \
            out[ off_lo + 1 ] += c; \
            if( work ) work[ off_lo + 1 ] += frac_hi_x; \
            if ( Usevar ) { \
               out_var[ off_lo + 1 ] += CONV(IntType,in_var[ off_in ]*frac_hi_x*frac_hi_x); \
            } else if( Genvar && frac_hi_x != 0.0 ) { \
               out_var[ off_lo + 1 ] += c*c/frac_hi_x; \
               work[ off_lo + 1 + npix_out ] += frac_hi_x*frac_hi_x; \
            } \
         } \
      } \
   }




#define LINEAR_2D(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* Loop round all input points which are to be rebinned. */ \
   for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
      off_in = offset[ point ]; \
      in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
      if ( Usebad ) { \
         bad = ( in_val == badval ); \
         if ( Usevar || Varwgt ) { \
            bad = bad || ( in_var[ off_in ] == badval ) \
                      || ( in_var[ off_in ] <= 0.0 ); \
         } \
      } else { \
         if ( Usevar || Varwgt ) { \
            bad = ( in_var[ off_in ] <= 0.0 ); \
         } else { \
            bad = 0; \
         } \
      } \
\
/* Obtain the x coordinate of the current point and test if it lies \
   outside the output grid. Also test if it is bad. */ \
      y = coords[ 1 ][ point ]; \
      bad = ( y < ymin ) || ( y >= ymax ) || ( y == AST__BAD ); \
      if ( !bad ) { \
\
/* Similarly obtain and test the y coordinate. */ \
         x = coords[ 0 ][ point ]; \
         bad = bad || ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
         if ( !bad ) { \
\
\
/* If OK, obtain the indices along the output grid x dimension of the \
   two adjacent pixels which recieve contributions from the input pixel. \
   Also obtain the fractional weight to be applied to each of \
   these pixels. */ \
            lo_x = (int) floor( x ); \
            hi_x = lo_x + 1; \
            frac_lo_x = (double) hi_x - x; \
            frac_hi_x = 1.0 - frac_lo_x; \
\
/* Repeat this process for the y dimension. */ \
            lo_y = (int) floor( y ); \
            hi_y = lo_y + 1; \
            frac_lo_y = (double) hi_y - y; \
            frac_hi_y = 1.0 - frac_lo_y; \
\
/* If we are using the input data variances as weights, calculate the \
   weight, and scale the fractions of each input pixel by the weight. \
   Since the product of two fractions is always used ot scale the input \
   data values, we use the square root of the reciprocal of the variance \
   as the weight (so that when the product of two fractions is taken, \
   the square roots multiply together to give the required 1/variance \
   weight).  */ \
            if( Varwgt ) { \
               wgt = 1.0/sqrt( in_var[ off_in ] ); \
               frac_lo_x *= wgt; \
               frac_hi_x *= wgt; \
               frac_lo_y *= wgt; \
               frac_hi_y *= wgt; \
            } \
\
/* Obtain the offset within the output array of the first pixel to be \
   updated (the one with the smaller index along both dimensions). */ \
            off_lo = lo_x - lbnd_out[ 0 ] + ystride * ( lo_y - lbnd_out[ 1 ] ); \
\
/* For each of the four pixels which may be updated, test if the pixel indices \
   lie within the output grid. Where they do, update the output pixel \
   with the required fraction of the input pixel value. */ \
            if ( lo_y >= lbnd_out[ 1 ] ) { \
               if ( lo_x >= lbnd_out[ 0 ] ) { \
                  f = frac_lo_x * frac_lo_y; \
                  c = CONV(IntType,in_val*f); \
                  out[ off_lo ] += c; \
                  if( work ) work[ off_lo ] += f; \
                  if ( Usevar ) { \
                     out_var[ off_lo ] += CONV(IntType,in_var[ off_in ]*f*f); \
                  } else if ( Genvar && f != 0.0 ) { \
                     out_var[ off_lo ] += c*c/f; \
                     work[ off_lo + npix_out ] += f*f; \
                  } \
               } \
               if ( hi_x <= ubnd_out[ 0 ] ) { \
                  f = frac_hi_x * frac_lo_y; \
                  off = off_lo + 1; \
                  c = CONV(IntType,in_val*f); \
                  out[ off ] += c; \
                  if( work ) work[ off ] += f; \
                  if ( Usevar ) { \
                     out_var[ off ] += CONV(IntType,in_var[ off_in ]*f*f); \
                  } else if ( Genvar && f != 0.0 ) { \
                     out_var[ off ] += c*c/f; \
                     work[ off + npix_out ] += f*f; \
                  } \
               } \
            } \
            if ( hi_y <= ubnd_out[ 1 ] ) { \
               if ( lo_x >= lbnd_out[ 0 ] ) { \
                  f = frac_lo_x * frac_hi_y; \
                  off = off_lo + ystride; \
                  c = CONV(IntType,in_val*f); \
                  out[ off ] += c; \
                  if( work ) work[ off ] += f; \
                  if ( Usevar ) { \
                     out_var[ off ] += CONV(IntType,in_var[ off_in ]*f*f ); \
                  } else if ( Genvar && f != 0.0 ) { \
                     out_var[ off ] += c*c/f; \
                     work[ off + npix_out ] += f*f; \
                  } \
               } \
               if ( hi_x <= ubnd_out[ 0 ] ) { \
                  f = frac_hi_x * frac_hi_y; \
                  off = off_lo + ystride + 1; \
                  c = CONV(IntType,in_val*f); \
                  out[ off ] += c; \
                  if( work ) work[ off ] += f; \
                  if ( Usevar ) { \
                     out_var[ off ] += CONV(IntType,in_var[ off_in ]*f*f); \
                  } else if ( Genvar && f != 0.0 ) { \
                     out_var[ off ] += c*c/f; \
                     work[ off + npix_out ] += f*f; \
                  } \
               } \
            } \
         } \
      } \
   }


#define LINEAR_ND(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* Loop round all input points which are to be rebinned. */ \
   for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
      off_in = offset[ point ]; \
      in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
      if ( Usebad ) { \
         bad = ( in_val == badval ); \
         if ( Usevar || Varwgt ) { \
            bad = bad || ( in_var[ off_in ] == badval ) \
                      || ( in_var[ off_in ] <= 0.0 ); \
         } \
      } else { \
         if ( Usevar || Varwgt ) { \
            bad = ( in_var[ off_in ] <= 0.0 ); \
         } else { \
            bad = 0; \
         } \
      } \
\
/* Initialise offsets into the output array. Then loop to obtain each \
   coordinate associated with the current output point. */ \
      if( !bad ) { \
         off_out = 0; \
         for ( idim = 0; idim < ndim_out; idim++ ) { \
            xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate lies outside the output grid.  Also test if \
   it is bad. If either is true, the corresponding output pixel value \
   will be bad, so give up on this point. */ \
            bad = ( xn < xn_min[ idim ] ) || ( xn >= xn_max[ idim ] ) || \
                  ( xn == AST__BAD ); \
            if ( bad ) break; \
\
/* Obtain the indices along the current dimension of the output grid of \
   the two (usually adjacent) pixels which will be updated. If necessary, \
   however, restrict each index to ensure it does not lie outside the \
   input grid. Also calculate the fractional weight to be given to each \
   pixel in order to divide the input value linearly between them. */ \
            ixn = (int) floor( xn ); \
            lo[ idim ] = MaxI( ixn, lbnd_out[ idim ] ); \
            hi[ idim ] = MinI( ixn + 1, ubnd_out[ idim ] ); \
            frac_lo[ idim ] = 1.0 - fabs( xn - (double) lo[ idim ] ); \
            frac_hi[ idim ] = 1.0 - fabs( xn - (double) hi[ idim ] ); \
\
/* Store the lower index involved in spreading along each \
   dimension and accumulate the offset from the start of the output \
   array of the pixel which has these indices. */ \
            dim[ idim ] = lo[ idim ]; \
            off_out += stride[ idim ] * ( lo[ idim ] - lbnd_out[ idim ] ); \
\
/* Also store the fractional weight associated with the lower pixel \
   along each dimension. */ \
            wt[ idim ] = frac_lo[ idim ]; \
         } \
\
/* If we are using the input data variances as weights, calculate the \
   weight, and scale the fractions of each input pixel by the weight. */ \
         if( Varwgt ) { \
            wgt = pow( in_var[ off_in ], -1.0/(double)ndim_out ); \
            for ( idim = 0; idim < ndim_out; idim++ ) { \
               frac_lo[ idim ] *= wgt; \
               frac_hi[ idim ] *= wgt; \
               wt[ idim ] = frac_lo[ idim ]; \
            } \
         } \
\
/* If OK, loop over adjacent output pixels to divide up the input value. */ \
         if ( !bad ) { \
            idim = ndim_out - 1; \
            wtprod[ idim ] = 1.0; \
            done = 0; \
            do { \
\
/* Each pixel pixel to be updated has a total weight equal to the product \
   of the weights which account for the displacement of its centre from \
   the required position along each dimension. However, since we typically \
   only change the index of one dimension at a time, we can avoid forming \
   this product repeatedly by retaining an array of accumulated weight \
   products for all higher dimensions. We need then only update the \
   lower elements in this array, corresponding to those dimensions \
   whose index has changed. We do this here, "idim" being the index of \
   the most significant dimension to have changed. Note that on the \
   first pass, all dimensions are considered changed, causing this \
   array to be initialised. */ \
               for ( ii = idim; ii >= 1; ii-- ) { \
                  wtprod[ ii - 1 ] = wtprod[ ii ] * wt[ ii ]; \
               } \
\
/* Update the relevent output pixel. The pixel weight is formed by including \
   the weight factor for dimension zero, since this is not included in \
   the "wtprod" array. */ \
               f = wtprod[ 0 ] * wt[ 0 ]; \
               c = CONV(IntType,in_val*f); \
               out[ off_out ] += c; \
               if( work ) work[ off_out ] += f; \
               if ( Usevar ) { \
                  out_var[ off_out ] += CONV(IntType,in_var[ off_in ]*f*f); \
               } else if ( Genvar && f != 0.0 ) { \
                  out_var[ off_out ] += c*c/f; \
                  work[ off_out + npix_out ] += f*f; \
               } \
\
/* Now update the indices, offset and weight factors to refer to the \
   next output pixel to be updated. */ \
               idim = 0; \
               do { \
\
/* The first input dimension which still refers to the pixel with the \
   lower of the two possible indices is switched to refer to the other \
   pixel (with the higher index). The offset into the output array and \
   the fractional weight factor for this dimension are also updated \
   accordingly. */ \
                  if ( dim[ idim ] != hi[ idim ] ) { \
                     dim[ idim ] = hi[ idim ]; \
                     off_out += stride[ idim ]; \
                     wt[ idim ] = frac_hi[ idim ]; \
                     break; \
\
/* Any earlier dimensions (referring to the higher index) are switched \
   back to the lower index, if not already there, before going on to \
   consider the next dimension. (This process is the same as \
   incrementing a binary number and propagating overflows up through \
   successive digits, except that dimensions where the "lo" and "hi" \
   values are the same can only take one value.) The process stops at \
   the first attempt to return the final dimension to the lower \
   index. */ \
                  } else { \
                     if ( dim[ idim ] != lo[ idim ] ) { \
                        dim[ idim ] = lo[ idim ]; \
                        off_out -= stride[ idim ]; \
                        wt[ idim ] = frac_lo[ idim ]; \
                     } \
                     done = ( ++idim == ndim_out ); \
                  } \
               } while ( !done ); \
            } while ( !done ); \
         } \
      } \
   }

/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_SPREAD_LINEAR(LD,long double,0)
#endif     
MAKE_SPREAD_LINEAR(D,double,0)
MAKE_SPREAD_LINEAR(F,float,0)
MAKE_SPREAD_LINEAR(I,int,1)

/* Undefine the macros used above. */
#undef LINEAR_1D
#undef LINEAR_2D
#undef LINEAR_ND
#undef MAKE_SPREAD_LINEAR

/*
*  Name:
*     SpreadNearest<X>

*  Purpose:
*     Rebin a data grid, using the nearest-pixel spreading scheme.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void SpreadNearest<X>( int ndim_out,
*                           const int *lbnd_out, const int *ubnd_out,
*                           const <Xtype> *in, const <Xtype> *in_var,
*                           int npoint, const int *offset, 
*                           const double *const *coords,
*                           int flags, <Xtype> badval, int npix_out,
*                           <Xtype> *out, <Xtype> *out_var, double *work ) 

*  Class Membership:
*     Mapping member function.

*  Description:
*     This is a set of functions which rebins a rectangular region of an 
*     input grid of data (and, optionally, associated statistical variance
*     values) so as to place them into a new output grid. Each input
*     grid point may be mapped on to a position in the output grid in
*     an arbitrary way. Where the positions given do not correspond
*     with a pixel centre in the output grid, the spreading scheme
*     used is simply to select the nearest pixel (i.e. the one whose
*     bounds contain the supplied position).

*  Parameters:
*     ndim_out
*        The number of dimensions in the output grid. This should be at
*        least one.
*     lbnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the output grid along each dimension.
*     ubnd_out
*        Pointer to an array of integers, with "ndim_out" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the output grid along each dimension.
*
*        Note that "lbnd_out" and "ubnd_out" together define the shape
*        and size of the output grid, its extent along a particular
*        (i'th) dimension being ubnd_out[i]-lbnd_out[i]+1 (assuming "i"
*        is zero-based). They also define the output grid's coordinate
*        system, with each pixel being of unit extent along each
*        dimension with integral coordinate values at its centre.
*     in
*        Pointer to the array of data to be rebinned. The numerical type
*        of these data should match the function used, as given by the
*        suffix on the function name. Note that details of how the input 
*        grid maps on to this array (e.g. the storage order, number of 
*        dimensions, etc.) is arbitrary and is specified entirely by means 
*        of the "offset" array. The "in" array should therefore contain 
*        sufficient elements to accommodate the "offset" values supplied.  
*        There is no requirement that all elements of the "in" array 
*        should be rebinned, and any which are not addressed by the
*        contents of the "offset" array will be ignored.
*     in_var
*        An optional pointer to a second array of positive numerical
*        values (with the same size and type as the "in" array), which
*        represent estimates of the statistical variance associated
*        with each element of the "in" array. If this second array is
*        given (along with the corresponding "out_var" array), then
*        estimates of the variance of the resampled data will also be
*        returned. It is addressed in exactly the same way (via the 
*        "offset" array) as the "in" array. 
*
*        If no variance estimates are required, a NULL pointer should
*        be given.
*     npoint
*        The number of input points which are to be rebinned.
*     offset
*        Pointer to an array of integers with "npoint" elements. For
*        each input point, this array should contain the zero-based
*        offset in the input array(s) (i.e. the "in" and, optionally, 
*        the "in_var" arrays) from which the value to be rebinned should
*        be obtained.
*     coords
*        An array of pointers to double, with "ndim_out" elements. 
*        Element "coords[coord]" should point at the first element of 
*        an array of double (with "npoint" elements) which contains the 
*        values of coordinate number "coord" for each point being
*        rebinned. The value of coordinate number "coord" for 
*        rebinning point number "point" is therefore given by
*        "coords[coord][point]" (assuming both indices are
*        zero-based).  If any point has a coordinate value of AST__BAD
*        associated with it, then the corresponding input data (and
*        variance) value will be ignored.
*     flags
*        The bitwise OR of a set of flag values which control the
*        operation of the function. These are chosend from:
*
*        - AST__USEBAD: indicates whether there are "bad" (i.e. missing) data 
*        in the input array(s) which must be recognised.  If this flag is not 
*        set, all input values are treated literally.
*        - AST__GENVAR: Indicates that any input variances are to be
*        ignored, and that the output variances should be generated from
*        the spread of values contributing to each output pixel.
*     badval
*        If the AST__USEBAD flag is set in the "flags" value (above),
*        this parameter specifies the value which is used to identify
*        bad data and/or variance values in the input array(s). Its
*        numerical type must match that of the "in" (and "in_var")
*        arrays. The same value will also be used to flag any output
*        array elements for which resampled values could not be
*        obtained.  The output arrays(s) may be flagged with this
*        value whether or not the AST__USEBAD flag is set (the
*        function return value indicates whether any such values have
*        been produced).
*     npix_out
*        Number of pixels in output array.
*     out
*        Pointer to an array with the same data type as the "in"
*        array, into which the rebinned data will be returned. The 
*        storage order should be such that the index of the first grid
*        dimension varies most rapidly and that of the final dimension 
*        least rapidly (i.e. Fortran array storage order).
*     out_var
*        An optional pointer to an array with the same data type and
*        size as the "out" array, into which variance estimates for
*        the rebinned values may be returned. This array will only be
*        used if the "in_var" array has been given. The values returned 
*        are estimates of the statistical variance of the corresponding 
*        values in the "out" array, on the assumption that all errors in 
*        input grid values (in the "in" array) are statistically independent 
*        and that their variance estimates (in the "in_var" array) may 
*        simply be summed (with appropriate weighting factors).
*
*        If no output variance estimates are required, a NULL pointer
*        should be given.
*     work
*        A pointer to an array with the same data type and size as the "out" 
*        array which is used as work space. The values in the supplied
*        array are incremented on exit by the sum of the weights used
*        with each output pixel.

*  Notes:
*     - There is a separate function for each numerical type of
*     gridded data, distinguished by replacing the <X> in the function
*     name by the appropriate 1- or 2-character suffix.
*/
/* Define a macro to implement the function for a specific data type. */
#define MAKE_SPREAD_NEAREST(X,Xtype,IntType) \
static void SpreadNearest##X( int ndim_out, \
                             const int *lbnd_out, const int *ubnd_out, \
                             const Xtype *in, const Xtype *in_var, \
                             int npoint, const int *offset, \
                             const double *const *coords, \
                             int flags, Xtype badval, int npix_out, \
                             Xtype *out, Xtype *out_var, double *work ) { \
\
/* Local Variables: */ \
   Xtype c;                      /* Contribution to output value */ \
   Xtype in_val;                 /* Input data value */ \
   double *xn_max;               /* Pointer to upper limits array (n-d) */ \
   double *xn_min;               /* Pointer to lower limits array (n-d) */ \
   double cwgt;                  /* Product of input value and weight */ \
   double wgt;                   /* Weight for input value */ \
   double x;                     /* x coordinate value */ \
   double xmax;                  /* x upper limit */ \
   double xmin;                  /* x lower limit */ \
   double xn;                    /* Coordinate value (n-d) */ \
   double y;                     /* y coordinate value */ \
   double ymax;                  /* y upper limit */ \
   double ymin;                  /* y lower limit */ \
   int *stride;                  /* Pointer to array of dimension strides */ \
   int bad;                      /* Output pixel bad? */ \
   int genvar;                   /* Generate output variances? */ \
   int idim;                     /* Loop counter for dimensions */ \
   int ix;                       /* Number of pixels offset in x direction */ \
   int ixn;                      /* Number of pixels offset (n-d) */ \
   int iy;                       /* Number of pixels offset in y direction */ \
   int off_in;                   /* Pixel offset into input array */ \
   int off_out;                  /* Pixel offset into output array */ \
   int point;                    /* Loop counter for output points */ \
   int s;                        /* Temporary variable for strides */ \
   int usebad;                   /* Use "bad" input pixel values? */ \
   int usevar;                   /* Process variance array? */ \
   int varwgt;                   /* Use input variances as weights? */ \
   int ystride;                  /* Stride along input grid y direction */ \
\
/* Check the global error status. */ \
   if ( !astOK ) return; \
\
/* Determine if we are processing bad pixels or variances. */ \
   usebad = flags & AST__USEBAD; \
   genvar = ( flags & AST__GENVAR ) && out_var && work; \
   varwgt = ( flags & AST__VARWGT ) && in_var && work; \
   usevar = !genvar && in_var && out_var; \
\
/* Handle the 1-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   if ( ndim_out == 1 ) { \
\
/* Calculate the coordinate limits of the output array. */ \
      xmin = (double) lbnd_out[ 0 ] - 0.5; \
      xmax = (double) ubnd_out[ 0 ] + 0.5; \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
      if( varwgt ) { \
         if ( usebad ) { \
            if ( usevar ) { \
               NEAR_1D(X,Xtype,1,1,0,IntType,1) \
            } else if ( genvar ) { \
               NEAR_1D(X,Xtype,1,0,1,IntType,1) \
            } else { \
               NEAR_1D(X,Xtype,1,0,0,IntType,1) \
            } \
         } else { \
            if ( usevar ) { \
               NEAR_1D(X,Xtype,0,1,0,IntType,1) \
            } else if ( genvar ) { \
               NEAR_1D(X,Xtype,0,0,1,IntType,1) \
            } else { \
               NEAR_1D(X,Xtype,0,0,0,IntType,1) \
            } \
         } \
      } else { \
         if ( usebad ) { \
            if ( usevar ) { \
               NEAR_1D(X,Xtype,1,1,0,IntType,0) \
            } else if ( genvar ) { \
               NEAR_1D(X,Xtype,1,0,1,IntType,0) \
            } else { \
               NEAR_1D(X,Xtype,1,0,0,IntType,0) \
            } \
         } else { \
            if ( usevar ) { \
               NEAR_1D(X,Xtype,0,1,0,IntType,0) \
            } else if ( genvar ) { \
               NEAR_1D(X,Xtype,0,0,1,IntType,0) \
            } else { \
               NEAR_1D(X,Xtype,0,0,0,IntType,0) \
            } \
         } \
      } \
\
/* Handle the 2-dimensional case optimally. */ \
/* ---------------------------------------- */ \
   } else if ( ndim_out == 2 ) { \
\
/* Calculate the stride along the y dimension of the output grid. */ \
      ystride = ubnd_out[ 0 ] - lbnd_out[ 0 ] + 1; \
\
/* Calculate the coordinate limits of the output array in each \
   dimension. */ \
      xmin = (double) lbnd_out[ 0 ] - 0.5; \
      xmax = (double) ubnd_out[ 0 ] + 0.5; \
      ymin = (double) lbnd_out[ 1 ] - 0.5; \
      ymax = (double) ubnd_out[ 1 ] + 0.5; \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
      if( varwgt ) { \
         if ( usebad ) { \
            if ( usevar ) { \
               NEAR_2D(X,Xtype,1,1,0,IntType,1) \
            } else if ( genvar ) { \
               NEAR_2D(X,Xtype,1,0,1,IntType,1) \
            } else { \
               NEAR_2D(X,Xtype,1,0,0,IntType,1) \
            } \
         } else { \
            if ( usevar ) { \
               NEAR_2D(X,Xtype,0,1,0,IntType,1) \
            } else if ( genvar ) { \
               NEAR_2D(X,Xtype,0,0,1,IntType,1) \
            } else { \
               NEAR_2D(X,Xtype,0,0,0,IntType,1) \
            } \
         } \
      } else { \
         if ( usebad ) { \
            if ( usevar ) { \
               NEAR_2D(X,Xtype,1,1,0,IntType,0) \
            } else if ( genvar ) { \
               NEAR_2D(X,Xtype,1,0,1,IntType,0) \
            } else { \
               NEAR_2D(X,Xtype,1,0,0,IntType,0) \
            } \
         } else { \
            if ( usevar ) { \
               NEAR_2D(X,Xtype,0,1,0,IntType,0) \
            } else if ( genvar ) { \
               NEAR_2D(X,Xtype,0,0,1,IntType,0) \
            } else { \
               NEAR_2D(X,Xtype,0,0,0,IntType,0) \
            } \
         } \
      } \
\
/* Handle other numbers of dimensions. */ \
/* ----------------------------------- */ \
   } else { \
\
/* Allocate workspace. */ \
      stride = astMalloc( sizeof( int ) * (size_t) ndim_out ); \
      xn_max = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      xn_min = astMalloc( sizeof( double ) * (size_t) ndim_out ); \
      if ( astOK ) { \
\
/* Calculate the stride along each dimension of the output grid. */ \
         for ( s = 1, idim = 0; idim < ndim_out; idim++ ) { \
            stride[ idim ] = s; \
            s *= ubnd_out[ idim ] - lbnd_out[ idim ] + 1; \
\
/* Calculate the coordinate limits of the output grid in each \
   dimension. */ \
            xn_min[ idim ] = (double) lbnd_out[ idim ] - 0.5; \
            xn_max[ idim ] = (double) ubnd_out[ idim ] + 0.5; \
         } \
\
/* Identify eight cases, according to whether bad pixels and/or variances \
   are being processed and/or used. In each case we assign constant values \
   (0 or 1) to the "Usebad", "Usevar" and "Varwgt" flags so that code for \
   handling bad pixels and variances can be eliminated by the compiler's \
   optimisation system when not required. */ \
         if( varwgt ) { \
            if ( usebad ) { \
               if ( usevar ) { \
                  NEAR_ND(X,Xtype,1,1,0,IntType,1) \
               } else if ( genvar ) { \
                  NEAR_ND(X,Xtype,1,0,1,IntType,1) \
               } else { \
                  NEAR_ND(X,Xtype,1,0,0,IntType,1) \
               } \
            } else { \
               if ( usevar ) { \
                  NEAR_ND(X,Xtype,0,1,0,IntType,1) \
               } else if ( genvar ) { \
                  NEAR_ND(X,Xtype,0,0,1,IntType,1) \
               } else { \
                  NEAR_ND(X,Xtype,0,0,0,IntType,1) \
               } \
            } \
         } else { \
            if ( usebad ) { \
               if ( usevar ) { \
                  NEAR_ND(X,Xtype,1,1,0,IntType,0) \
               } else if ( genvar ) { \
                  NEAR_ND(X,Xtype,1,0,1,IntType,0) \
               } else { \
                  NEAR_ND(X,Xtype,1,0,0,IntType,0) \
               } \
            } else { \
               if ( usevar ) { \
                  NEAR_ND(X,Xtype,0,1,0,IntType,0) \
               } else if ( genvar ) { \
                  NEAR_ND(X,Xtype,0,0,1,IntType,0) \
               } else { \
                  NEAR_ND(X,Xtype,0,0,0,IntType,0) \
               } \
            } \
         } \
      } \
\
/* Free the workspace. */ \
      stride = astFree( stride ); \
      xn_max = astFree( xn_max ); \
      xn_min = astFree( xn_min ); \
   } \
\
}





#define NEAR_1D(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* Loop round all input points which are to be rebinned. */ \
            for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
               off_in = offset[ point ]; \
               in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
               if ( Usebad ) { \
                  bad = ( in_val == badval ); \
                  if ( Usevar || Varwgt ) { \
                     bad = bad || ( in_var[ off_in ] == badval ) \
                               || ( in_var[ off_in ] <= 0.0 ); \
                  } \
               } else { \
                  if ( Usevar || Varwgt ) { \
                     bad = ( in_var[ off_in ] <= 0.0 ); \
                  } else { \
                     bad = 0; \
                  } \
               } \
\
/* Obtain the output x coordinate corresponding to the centre of the \
   current input pixel and test if it lies outside the output grid, or \
   is bad. */ \
               x = coords[ 0 ][ point ]; \
               bad = bad || ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
               if ( !bad ) { \
\
/* If not, then obtain the offset within the output grid of the pixel \
   which contains the current input point. */ \
                  off_out = (int) floor( x + 0.5 ) - lbnd_out[ 0 ]; \
\
/* Note the input data value. */ \
                  c = CONV(IntType,in_val); \
\
/* If we are using the input data variances as weights, calculate the \
   weight. */ \
                  if( Varwgt ) { \
                     wgt = 1.0/in_var[ off_in ]; \
\
/* Increment the value of this output pixel by the weighted input pixel \
   value, and increment the sum of the weights. */ \
                     cwgt = c*wgt; \
                     out[ off_out ] += CONV(IntType, cwgt ); \
                     work[ off_out ] += wgt; \
\
/* If output variances are being calculated on the basis of the input \
   variances, then we also store the sum of the weights in "out_var". */ \
                     if( Usevar ) { \
                        out_var[ off_out ] += CONV(IntType,wgt); \
\
/* If output variances are being calculated on the basis of the spread of \
   input values, we need the sum of the squared weighted data values, the \
   sum of the weights (already in the first half of the "work" array), and \
   the sum of the squared weights. */ \
                     } else if( Genvar ) { \
                        out_var[ off_out ] += CONV(IntType,cwgt*cwgt); \
                        work[ off_out + npix_out ] += wgt*wgt; \
                     } \
\
/* Now deal with cases where we are not using the input data varainces as \
   weights. */ \
                  } else { \
\
/* Increment the value of this output pixel by the input pixel value, and \
   increment the sum of the weights by 1. */ \
                     out[ off_out ] += c; \
                     if( work ) work[ off_out ] += 1.0; \
\
/* If output variances are being calculated on the basis of the input \
   variances, then we increment the variance of this output pixel by the \
   variance of the input pixel. */ \
                     if ( Usevar ) { \
                        out_var[ off_out ] += CONV(IntType,in_var[ off_in ]); \
\
/* If output variances are being calculated on the basis of the spread of \
   input values, we need the sum of the squared data values, and the number \
   of values summed. */ \
                     } else if( Genvar ) { \
                        out_var[ off_out ] += c*c; \
                        work[ off_out + npix_out ] += 1.0; \
                     } \
                  } \
               } \
            }






#define NEAR_2D(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* Loop round all input points which are to be rebinned. */ \
            for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
               off_in = offset[ point ]; \
               in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad (or zero). */ \
               if ( Usebad ) { \
                  bad = ( in_val == badval ); \
                  if ( Usevar || Varwgt ) { \
                     bad = bad || ( in_var[ off_in ] == badval ) \
                               || ( in_var[ off_in ] <= 0.0 ); \
                  } \
               } else { \
                  if ( Usevar || Varwgt ) { \
                     bad = ( in_var[ off_in ] <= 0.0 ); \
                  } else { \
                     bad = 0; \
                  } \
               } \
\
/* Obtain the output y coordinate corresponding to the centre of the \
   current input pixel and test if it lies outside the output grid, or \
   is bad. */ \
               y = coords[ 1 ][ point ]; \
               bad = bad || ( y < ymin ) || ( y >= ymax ) || ( y == AST__BAD ); \
               if ( !bad ) { \
\
/* Obtain the output x coordinate corresponding to the centre of the \
   current input pixel and test if it lies outside the output grid, or \
   is bad. */ \
                  x = coords[ 0 ][ point ]; \
                  bad = bad || ( x < xmin ) || ( x >= xmax ) || ( x == AST__BAD ); \
                  if ( !bad ) { \
\
/* Obtain the offsets along each output grid dimension of the output \ 
   pixel which is to receive the input pixel value. */ \
                     ix = (int) floor( x + 0.5 ) - lbnd_out[ 0 ]; \
                     iy = (int) floor( y + 0.5 ) - lbnd_out[ 1 ]; \
\
/* Calculate this pixel's offset from the start of the output array. */ \
                     off_out = ix + ystride * iy; \
\
/* Note the input data value. */ \
                     c = CONV(IntType,in_val); \
\
/* If we are using the input data variances as weights, calculate the \
   weight. */ \
                     if( Varwgt ) { \
                        wgt = 1.0/in_var[ off_in ]; \
\
/* Increment the value of this output pixel by the weighted input pixel \
   value, and increment the sum of the weights. */ \
                        cwgt = c*wgt; \
                        out[ off_out ] += CONV(IntType, cwgt ); \
                        work[ off_out ] += wgt; \
\
/* If output variances are being calculated on the basis of the input \
   variances, then we also store the sum of the weights in "out_var". */ \
                        if( Usevar ) { \
                           out_var[ off_out ] += CONV(IntType,wgt); \
\
/* If output variances are being calculated on the basis of the spread of \
   input values, we need the sum of the squared weighted data values, the \
   sum of the weights (already in the first half of the "work" array), and \
   the sum of the squared weights. */ \
                        } else if( Genvar ) { \
                           out_var[ off_out ] += CONV(IntType,cwgt*cwgt); \
                           work[ off_out + npix_out ] += wgt*wgt; \
                        } \
\
/* Now deal with cases where we are not using the input data varainces as \
   weights. */ \
                     } else { \
\
/* Increment the value of this output pixel by the value of the input pixel. */ \
                        out[ off_out ] += c; \
                        if( work ) work[ off_out ] += 1.0; \
\
/* If required, also increment the variance of this output pixel by the \
   variance of the input pixel. */ \
                        if ( Usevar ) { \
                           out_var[ off_out ] += CONV(IntType,in_var[ off_in ]); \
\
/* Alternatively, if generating output variances from the spread of \
   input values, form the required sum.*/ \
                        } else if( Genvar ) { \
                           out_var[ off_out ] += c*c; \
                           work[ off_out + npix_out ] += 1.0; \
                        } \
                     } \
                  } \
               } \
            }



#define NEAR_ND(X,Xtype,Usebad,Usevar,Genvar,IntType,Varwgt) \
\
/* Loop round all input points which are to be rebinned. */ \
            for( point = 0; point < npoint; point++ ) { \
\
/* Obtain the input data value which is to be added into the output array. */ \
               off_in = offset[ point ]; \
               in_val = in[ off_in ]; \
\
/* If necessary, test if the input data value or variance is bad. */ \
               if ( Usebad ) { \
                  bad = ( in_val == badval ); \
                  if ( Usevar || Varwgt ) { \
                     bad = bad || ( in_var[ off_in ] == badval ) \
                               || ( in_var[ off_in ] <= 0.0 ); \
                  } \
               } else { \
                  if ( Usevar || Varwgt ) { \
                     bad = ( in_var[ off_in ] <= 0.0 ); \
                  } else { \
                     bad = 0; \
                  } \
               } \
\
               if( !bad ) { \
\
/* Initialise the offset into the output array. Then loop to obtain \
   each coordinate associated with the current output point. */ \
                  off_out = 0; \
                  for ( idim = 0; idim < ndim_out; idim++ ) { \
                     xn = coords[ idim ][ point ]; \
\
/* Test if the coordinate lies outside the output grid, or is bad. If \
   either is true, the corresponding input pixel value will be ignored, \
   so give up on this point. */ \
                     bad = ( xn < xn_min[ idim ] ) || ( xn >= xn_max[ idim ] ) || \
                           ( xn == AST__BAD ); \
                     if ( bad ) break; \
\
/* Obtain the offset along the current output grid dimension of the \
   output pixel which is to receive the input pixel value. */ \
                     ixn = (int) floor( xn + 0.5 ) - lbnd_out[ idim ]; \
\
/* Accumulate this pixel's offset from the start of the output array. */ \
                     off_out += ixn * stride[ idim ]; \
                  } \
                  if( !bad ) { \
\
/* Note the input data value. */ \
                     c = CONV(IntType,in_val); \
\
/* If we are using the input data variances as weights, calculate the \
   weight. */ \
                     if( Varwgt ) { \
                        wgt = 1.0/in_var[ off_in ]; \
\
/* Increment the value of this output pixel by the weighted input pixel \
   value, and increment the sum of the weights. */ \
                        cwgt = c*wgt; \
                        out[ off_out ] += CONV(IntType, cwgt ); \
                        work[ off_out ] += wgt; \
\
/* If output variances are being calculated on the basis of the input \
   variances, then we also store the sum of the weights in "out_var". */ \
                        if( Usevar ) { \
                           out_var[ off_out ] += CONV(IntType,wgt); \
\
/* If output variances are being calculated on the basis of the spread of \
   input values, we need the sum of the squared weighted data values, the \
   sum of the weights (already in the first half of the "work" array), and \
   the sum of the squared weights. */ \
                        } else if( Genvar ) { \
                           out_var[ off_out ] += CONV(IntType,cwgt*cwgt); \
                           work[ off_out + npix_out ] += wgt*wgt; \
                        } \
\
/* Now deal with cases where we are not using the input data varainces as \
   weights. */ \
                     } else { \
\
/* Increment the value of this output pixel by the value of the input pixel. */ \
                        out[ off_out ] += c; \
                        if( work ) work[ off_out ] += 1.0; \
\
/* If required, also increment the variance of this output pixel by the \
   variance of the input pixel. */ \
                        if ( Usevar ) { \
                           out_var[ off_out ] += CONV(IntType,in_var[ off_in ]); \
\
/* Alternatively, if generating output variances from the spread of \
   input values, form the required sum.*/ \
                        } else if( Genvar ) { \
                           out_var[ off_out ] += c*c; \
                           work[ off_out + npix_out ] += 1.0; \
                        } \
                     } \
                  } \
               } \
            }






/* Expand the main macro above to generate a function for each
   required signed data type. */
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_SPREAD_NEAREST(LD,long double,0)
#endif

MAKE_SPREAD_NEAREST(D,double,0)
MAKE_SPREAD_NEAREST(F,float,0)
MAKE_SPREAD_NEAREST(I,int,1)

/* Undefine the macros used above. */
#undef NEAR_ND
#undef NEAR_2D
#undef NEAR_1D
#undef MAKE_SPREAD_NEAREST






static int TestAttrib( AstObject *this_object, const char *attrib ) {
/*
*  Name:
*     TestAttrib

*  Purpose:
*     Test if a specified attribute value is set for a Mapping.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     int TestAttrib( AstObject *this, const char *attrib )

*  Class Membership:
*     Mapping member function (over-rides the astTestAttrib protected
*     method inherited from the Object class).

*  Description:
*     This function returns a boolean result (0 or 1) to indicate whether
*     a value has been set for one of a Mapping's attributes.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     attrib
*        Pointer to a null terminated string specifying the attribute
*        name.  This should be in lower case with no surrounding white
*        space.

*  Returned Value:
*     One if a value has been set, otherwise zero.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global status set, or if it should fail for any reason.
*/

/* Local Variables: */
   AstMapping *this;             /* Pointer to the Mapping structure */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Obtain a pointer to the Mapping structure. */
   this = (AstMapping *) this_object;

/* Check the attribute name and test the appropriate attribute. */

/* Invert. */
/* ------- */
   if ( !strcmp( attrib, "invert" ) ) {
      result = astTestInvert( this );

/* Report. */
/* ------- */
   } else if ( !strcmp( attrib, "report" ) ) {
      result = astTestReport( this );

/* If the name is not recognised, test if it matches any of the
   read-only attributes of this class. If it does, then return
   zero. */
   } else if ( !strcmp( attrib, "nin" ) ||
        !strcmp( attrib, "issimple" ) ||
        !strcmp( attrib, "nout" ) ||
        !strcmp( attrib, "tranforward" ) ||
        !strcmp( attrib, "traninverse" ) ) {
      result = 0;

/* If the attribute is still not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      result = (*parent_testattrib)( this_object, attrib );
   }

/* Return the result, */
   return result;
}

static void Tran1( AstMapping *this, int npoint, const double xin[],
                   int forward, double xout[] ) {
/*
*++
*  Name:
c     astTran1
f     AST_TRAN1

*  Purpose:
*     Transform 1-dimensional coordinates.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astTran1( AstMapping *this, int npoint, const double xin[],
c                    int forward, double xout[] )
f     CALL AST_TRAN1( THIS, NPOINT, XIN, FORWARD, XOUT, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function applies a Mapping to transform the coordinates of
f     This routine applies a Mapping to transform the coordinates of
*     a set of points in one dimension.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping to be applied.
c     npoint
f     NPOINT = INTEGER (Given)
*        The number of points to be transformed.
c     xin
f     XIN( NPOINT ) = DOUBLE PRECISION (Given)
c        An array of "npoint" coordinate values for the input
f        An array of coordinate values for the input
*        (untransformed) points.
c     forward
f     FORWARD = LOGICAL (Given)
c        A non-zero value indicates that the Mapping's forward
c        coordinate transformation is to be applied, while a zero
c        value indicates that the inverse transformation should be
c        used.
f        A .TRUE. value indicates that the Mapping's forward
f        coordinate transformation is to be applied, while a .FALSE.
f        value indicates that the inverse transformation should be
f        used.
c     xout
f     XOUT( NPOINT ) = DOUBLE PRECISION (Returned)
c        An array (with "npoint" elements) into which the
f        An array into which the
*        coordinates of the output (transformed) points will be written.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
*     - The Mapping supplied must have the value 1 for both its Nin
*     and Nout attributes.
*--
*/

/* Local Variables: */
   AstPointSet *in_points;       /* Pointer to input PointSet */
   AstPointSet *out_points;      /* Pointer to output PointSet */
   const double *in_ptr[ 1 ];    /* Array of input data pointers */
   double *out_ptr[ 1 ];         /* Array of output data pointers */

/* Check the global error status. */
   if ( !astOK ) return;

/* Validate the Mapping and numbers of points/coordinates. */
   ValidateMapping( this, forward, npoint, 1, 1, "astTran1" );

/* Set up pointers to the input and output coordinate arrays. */
   if ( astOK ) {
      in_ptr[ 0 ] = xin;
      out_ptr[ 0 ] = xout;

/* Create PointSets to describe the input and output points. */
      in_points = astPointSet( npoint, 1, "" );
      out_points = astPointSet( npoint, 1, "" );

/* Associate the data pointers with the PointSets (note we must
   explicitly remove the "const" qualifier from the input data here,
   although they will not be modified).  */
      astSetPoints( in_points, (double **) in_ptr );
      astSetPoints( out_points, out_ptr );

/* Apply the required transformation to the coordinates. */
      (void) astTransform( this, in_points, forward, out_points );

/* If the Mapping's Report attribute is set, report the effect the
   Mapping has had on the coordinates. */
      if ( astGetReport( this ) ) astReportPoints( this, forward,
                                                   in_points, out_points );

/* Delete the two PointSets. */
      in_points = astDelete( in_points );
      out_points = astDelete( out_points );
   }
}

static void Tran2( AstMapping *this,
                   int npoint, const double xin[], const double yin[],
                   int forward, double xout[], double yout[] ) {
/*
*++
*  Name:
c     astTran2
f     AST_TRAN2

*  Purpose:
*     Transform 2-dimensional coordinates.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astTran2( AstMapping *this,
c                    int npoint, const double xin[], const double yin[],
c                    int forward, double xout[], double yout[] )
f     CALL AST_TRAN2( THIS, NPOINT, XIN, YIN, FORWARD, XOUT, YOUT, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function applies a Mapping to transform the coordinates of
f     This routine applies a Mapping to transform the coordinates of
*     a set of points in two dimensions.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping to be applied.
c     npoint
f     NPOINT = INTEGER (Given)
*        The number of points to be transformed.
c     xin
f     XIN( NPOINT ) = DOUBLE PRECISION (Given)
c        An array of "npoint" X-coordinate values for the input
f        An array of X-coordinate values for the input
*        (untransformed) points.
c     yin
f     YIN( NPOINT ) = DOUBLE PRECISION (Given)
c        An array of "npoint" Y-coordinate values for the input
f        An array of Y-coordinate values for the input
*        (untransformed) points.
c     forward
f     FORWARD = LOGICAL (Given)
c        A non-zero value indicates that the Mapping's forward
c        coordinate transformation is to be applied, while a zero
c        value indicates that the inverse transformation should be
c        used.
f        A .TRUE. value indicates that the Mapping's forward
f        coordinate transformation is to be applied, while a .FALSE.
f        value indicates that the inverse transformation should be
f        used.
c     xout
f     XOUT( NPOINT ) = DOUBLE PRECISION (Returned)
c        An array (with "npoint" elements) into which the
f        An array into which the
*        X-coordinates of the output (transformed) points will be written.
c     yout
f     YOUT( NPOINT ) = DOUBLE PRECISION (Returned)
c        An array (with "npoint" elements) into which the
f        An array into which the
*        Y-coordinates of the output (transformed) points will be written.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
*     - The Mapping supplied must have the value 2 for both its Nin
*     and Nout attributes.
*--
*/

/* Local Variables: */
   AstPointSet *in_points;       /* Pointer to input PointSet */
   AstPointSet *out_points;      /* Pointer to output PointSet */
   const double *in_ptr[ 2 ];    /* Array of input data pointers */
   double *out_ptr[ 2 ];         /* Array of output data pointers */

/* Check the global error status. */
   if ( !astOK ) return;

/* Validate the Mapping and the numbers of points/coordinates. */
   ValidateMapping( this, forward, npoint, 2, 2, "astTran2" );

/* Set up pointers to the input and output coordinate arrays. */
   if ( astOK ) {
      in_ptr[ 0 ] = xin;
      in_ptr[ 1 ] = yin;
      out_ptr[ 0 ] = xout;
      out_ptr[ 1 ] = yout;

/* Create PointSets to describe the input and output points. */
      in_points = astPointSet( npoint, 2, "" );
      out_points = astPointSet( npoint, 2, "" );

/* Associate the data pointers with the PointSets (note we must
   explicitly remove the "const" qualifier from the input data here,
   although they will not be modified).  */
      astSetPoints( in_points, (double **) in_ptr );
      astSetPoints( out_points, out_ptr );

/* Apply the required transformation to the coordinates. */
      (void) astTransform( this, in_points, forward, out_points );

/* If the Mapping's Report attribute is set, report the effect the
   Mapping has had on the coordinates. */
      if ( astGetReport( this ) ) astReportPoints( this, forward,
                                                   in_points, out_points );

/* Delete the two PointSets. */
      in_points = astDelete( in_points );
      out_points = astDelete( out_points );
   }
}

static void TranGrid( AstMapping *this, int ncoord_in, const int lbnd[], 
                      const int ubnd[], double tol, int maxpix, int forward, 
                      int ncoord_out, int outdim, double *out ) {
/*
*++
*  Name:
c     astTranGrid 
f     AST_TRANGRID

*  Purpose:
*     Transform a grid of positions

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astTranGrid( AstMapping *this, int ncoord_in,
c                       const int lbnd[], const int ubnd[],
c                       double tol, int maxpix, int forward,
c                       int ncoord_out, int outdim, double *out );
f     CALL AST_TRANGRID( THIS, NCOORD_IN, LBND, UBND, TOL, MAXPIX, 
f                        FORWARD, NCOORD_OUT, OUTDIM, OUT, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function uses the supplied Mapping to transforms a regular square 
*     grid of points covering a specified box. It attempts to do this
*     quickly by first approximating the Mapping with a linear transformation 
*     applied over the whole region of the input grid which is being used. 
*     If this proves to be insufficiently accurate, the input region is 
*     sub-divided into two along its largest dimension and the process is 
*     repeated within each of the resulting sub-regions. This process of
*     sub-division continues until a sufficiently good linear approximation 
*     is found, or the region to which it is being applied becomes too small 
*     (in which case the original Mapping is used directly).

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping to be applied.
c     ncoord_in
f     NCOORD_IN = INTEGER (Given)
*        The number of coordinates being supplied for each box corner
*        (i.e. the number of dimensions of the space in which the
*        input points reside).
c     lbnd
f     LBND( NCOORD_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ncoord_in" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the input grid along each dimension.
c     ubnd
f     UBND( NCOORD_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ncoord_in" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the input grid along each dimension.
*
c        Note that "lbnd" and "ubnd" together define the shape
f        Note that LBND and UBND together define the shape
*        and size of the input grid, its extent along a particular
c        (j'th) dimension being ubnd[j]-lbnd[j]+1 (assuming the
c        index "j" to be zero-based). They also define
f        (J'th) dimension being UBND(J)-LBND(J)+1. They also define
*        the input grid's coordinate system, each pixel having unit
*        extent along each dimension with integral coordinate values
*        at its centre.
c     tol
f     TOL = DOUBLE PRECISION (Given)
*        The maximum tolerable geometrical distortion which may be
*        introduced as a result of approximating non-linear Mappings
*        by a set of piece-wise linear transformations. This should be
*        expressed as a displacement within the output coordinate system
*        of the Mapping.
*
*        If piece-wise linear approximation is not required, a value
*        of zero may be given. This will ensure that the Mapping is
*        used without any approximation, but may increase execution
*        time.
*        
*        If the value is too high, discontinuities between the linear
*        approximations used in adjacent panel will be higher. If this 
*        is a problem, reduce the tolerance value used.
c     maxpix
f     MAXPIX = INTEGER (Given)
*        A value which specifies an initial scale size (in input grid points) 
*        for the adaptive algorithm which approximates non-linear Mappings
*        with piece-wise linear transformations. Normally, this should
*        be a large value (larger than any dimension of the region of
*        the input grid being used). In this case, a first attempt to
*        approximate the Mapping by a linear transformation will be
*        made over the entire input region.
*
*        If a smaller value is used, the input region will first be
c        divided into sub-regions whose size does not exceed "maxpix"
f        divided into sub-regions whose size does not exceed MAXPIX
*        grid points in any dimension. Only at this point will attempts
*        at approximation commence.
*
*        This value may occasionally be useful in preventing false
*        convergence of the adaptive algorithm in cases where the
*        Mapping appears approximately linear on large scales, but has
*        irregularities (e.g. holes) on smaller scales. A value of,
*        say, 50 to 100 grid points can also be employed as a safeguard 
*        in general-purpose software, since the effect on performance is
*        minimal.
*
*        If too small a value is given, it will have the effect of
*        inhibiting linear approximation altogether (equivalent to
c        setting "tol" to zero). Although this may degrade
f        setting TOL to zero). Although this may degrade
*        performance, accurate results will still be obtained.
c     forward
f     FORWARD = LOGICAL (Given)
c        A non-zero value indicates that the Mapping's forward
c        coordinate transformation is to be applied, while a zero
c        value indicates that the inverse transformation should be
c        used.
f        A .TRUE. value indicates that the Mapping's forward
f        coordinate transformation is to be applied, while a .FALSE.
f        value indicates that the inverse transformation should be
f        used.
c     ncoord_out
f     NCOORD_OUT = INTEGER (Given)
*        The number of coordinates being generated by the Mapping for
*        each output point (i.e. the number of dimensions of the
*        space in which the output points reside). This need not be
c        the same as "ncoord_in".
f        the same as NCOORD_IN.
c     outdim
f     OUTDIM = INTEGER (Given)
c        The number of elements along the second dimension of the "out"
f        The number of elements along the first dimension of the OUT
*        array (which will contain the output coordinates). The value
*        given should not be less than the number of points in the grid.
c     out
f     OUT( OUTDIM, NCOORD_OUT ) = DOUBLE PRECISION (Returned)
c        The address of the first element in a 2-dimensional array of 
c        shape "[ncoord_out][outdim]", into
c        which the coordinates of the output (transformed) points will
c        be written. These will be stored such that the value of
c        coordinate number "coord" for output point number "point"
c        will be found in element "out[coord][point]".
f        An array into which the coordinates of the output
f        (transformed) points will be written. These will be stored
f        such that the value of coordinate number COORD for output
f        point number POINT will be found in element OUT(POINT,COORD).
*        The points are ordered such that the first axis of the input
*        grid changes most rapidly. For example, if the input grid is 
*        2-dimensional and extends from (2,-1) to (3,1), the output
*        points will be stored in the order (2,-1), (3, -1), (2,0), (3,0),
*        (2,1), (3,1).
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
c     - If the forward coordinate transformation is being applied, the
c     Mapping supplied must have the value of "ncoord_in" for its Nin
c     attribute and the value of "ncoord_out" for its Nout attribute. If
c     the inverse transformation is being applied, these values should
c     be reversed.
f     - If the forward coordinate transformation is being applied, the
f     Mapping supplied must have the value of NCOORD_IN for its Nin
f     attribute and the value of NCOORD_OUT for its Nout attribute. If
f     the inverse transformation is being applied, these values should
f     be reversed.
*--
*/

/* Local Variables: */ 
   AstMapping *simple;           /* Pointer to simplified Mapping */ 
   double **out_ptr;             /* Pointer to array of output data pointers */
   int coord;                    /* Loop counter for coordinates */
   int idim;                     /* Loop counter for coordinate dimensions */ 
   int npoint;                   /* Number of points in the grid */ 

/* Check the global error status. */ 
   if ( !astOK ) return; 

/* Calculate the number of points in the grid, and check that the lower and 
   upper bounds of the input grid are consistent. Report an error if any 
   pair is not. */ 
   npoint = 1;
   for ( idim = 0; idim < ncoord_in; idim++ ) { 
      if ( lbnd[ idim ] > ubnd[ idim ] ) { 
         astError( AST__GBDIN, "astTranGrid(%s): Lower bound of " 
                   "input grid (%d) exceeds corresponding upper bound " 
                   "(%d).", astGetClass( this ), 
                   lbnd[ idim ], ubnd[ idim ] ); 
         astError( AST__GBDIN, "Error in input dimension %d.", 
                   idim + 1 ); 
         break; 
      } else {
         npoint *= ubnd[ idim ] - lbnd[ idim ] + 1; 
      }
   } 

/* Validate the mapping and numbers of points/coordinates. */
   ValidateMapping( this, forward, npoint, ncoord_in, ncoord_out,
                    "astTranGrid" );

/* Check that the positional accuracy tolerance supplied is valid and 
   report an error if necessary. */ 
   if ( astOK && ( tol < 0.0 ) ) { 
      astError( AST__PATIN, "astTranGrid(%s): Invalid positional " 
                "accuracy tolerance (%.*g pixel).", 
                astGetClass( this ), DBL_DIG, tol ); 
      astError( AST__PATIN, "This value should not be less than zero." ); 
   } 

/* Check that the initial scale size in grid points supplied is valid and 
   report an error if necessary. */ 
   if ( astOK && ( maxpix < 0 ) ) { 
      astError( AST__SSPIN, "astTranGrid(%s): Invalid initial scale " 
                "size in grid points (%d).", astGetClass( this ), maxpix ); 
      astError( AST__SSPIN, "This value should not be less than zero." ); 
   } 

/* Validate the output array dimension argument. */
   if ( astOK && ( outdim < npoint ) ) {
      astError( AST__DIMIN, "astTranGrid(%s): The output array dimension value "
                "(%d) is invalid.", astGetClass( this ), outdim );
      astError( AST__DIMIN, "This should not be less than the number of "
                "grid points being transformed (%d).", npoint );
   }

/* If there are sufficient pixels to make it worthwhile, simplify the 
   Mapping supplied to improve performance. Otherwise, just clone the 
   Mapping pointer. Note we save a pointer to the original Mapping so 
   that lower-level functions can use it if they need to report an error. */ 
   simple = NULL; 
   unsimplified_mapping = this; 
   if ( astOK ) { 
      if ( npoint > 1024 ) { 
         simple = astSimplify( this ); 

/* Report an error if the required transformation of this simplified 
   Mapping is not defined. */ 
         if( astOK ) {
            if ( forward && !astGetTranForward( simple ) ) { 
               astError( AST__TRNND, "astTranGrid(%s): A forward coordinate " 
                         "transformation is not defined by the %s supplied.", 
                         astGetClass( unsimplified_mapping ), 
                         astGetClass( unsimplified_mapping ) ); 
            } else if ( !forward && !astGetTranInverse( simple ) ) { 
               astError( AST__TRNND, "astTranGrid(%s): An inverse coordinate " 
                         "transformation is not defined by the %s supplied.", 
                         astGetClass( unsimplified_mapping ), 
                         astGetClass( unsimplified_mapping ) ); 
            }
         } 

      } else { 
         simple = astClone( this ); 
      } 

/* Allocate memory to hold the array of output data pointers. */
      out_ptr = astMalloc( sizeof( double * ) * (size_t) ncoord_out );

/* Initialise the output data pointers to point into the "out" array. */
      if ( astOK ) {
         for ( coord = 0; coord < ncoord_out; coord++ ) {
            out_ptr[ coord ] = out + coord * outdim;
         }

/* If required, temporarily invert the Mapping. */
         if( !forward ) astInvert( simple );

/* Perform the transformation. */
         TranGridAdaptively( simple, ncoord_in, lbnd, ubnd, lbnd, ubnd, tol, 
                             maxpix, ncoord_out, out_ptr );

/* If required, uninvert the Mapping. */
         if( !forward ) astInvert( simple );

      }

/* Free the memory used for the data pointers. */
      out_ptr = astFree( out_ptr );

/* Annul the pointer to the simplified/cloned Mapping. */ 
      simple = astAnnul( simple ); 
   }
}

static void TranGridAdaptively( AstMapping *this, int ncoord_in, 
                                const int *lbnd_in, const int *ubnd_in,
                                const int lbnd[], const int ubnd[], 
                                double tol, int maxpix, int ncoord_out, 
                                double *out[] ){
/*
*  Name:
*     TranGridAdaptively

*  Purpose:
*     Transform grid positions adaptively.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void TranGridAdaptively( AstMapping *this, int ncoord_in, 
*                              const int *lbnd_in, const int *ubnd_in,
*                              const int lbnd[], const int ubnd[], 
*                              double tol, int maxpix, int ncoord_out, 
*                              double *out[] )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function transforms grid points within a specified section of a 
*     rectangular grid (with any number of dimensions) using the forward
*     transformation of the specified Mapping.
*
*     This function is very similar to TranGridWithBlocking and TranGridSection
*     which lie below it in the calling hierarchy. However, this function 
*     also attempts to adapt to the Mapping supplied and to sub-divide the 
*     section being transformed into smaller sections within which a linear 
*     approximation to the Mapping may be used.  This reduces the number of 
*     Mapping evaluations, thereby improving efficiency particularly when
*     complicated Mappings are involved.

*  Parameters:
*     this
*        Pointer to the Mapping to be applied. The forward transformation
*        is used.
*     ncoord_in
*        The number of coordinates being supplied for each box corner
*        (i.e. the number of dimensions of the space in which the
*        input points reside).
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the whole input grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     lbnd
*        Pointer to an array of integers, with "ncoord_in" elements,
*        containing the coordinates of the centre of the first pixel
*        in the input grid along each dimension.
*     ubnd
*        Pointer to an array of integers, with "ncoord_in" elements,
*        containing the coordinates of the centre of the last pixel in
*        the input grid along each dimension.
*
*        Note that "lbnd" and "ubnd" together define the shape
*        and size of the input grid, its extent along a particular
*        (j'th) dimension being ubnd[j]-lbnd[j]+1 (assuming the
*        index "j" to be zero-based). They also define
*        the input grid's coordinate system, each pixel having unit
*        extent along each dimension with integral coordinate values
*        at its centre.
*     tol
*        The maximum tolerable geometrical distortion which may be
*        introduced as a result of approximating non-linear Mappings
*        by a set of piece-wise linear transformations. This should be
*        expressed as a displacement in pixels in the output grid's
*        coordinate system.
*
*        If piece-wise linear approximation is not required, a value
*        of zero may be given. This will ensure that the Mapping is
*        used without any approximation, but may increase execution
*        time.
*        
*        If the value is too high, discontinuities between the linear
*        approximations used in adjacent panel will be higher. If this 
*        is a problem, reduce the tolerance value used.
*     maxpix
*        A value which specifies an initial scale size (in grid points) 
*        for the adaptive algorithm which approximates non-linear Mappings
*        with piece-wise linear transformations. Normally, this should
*        be a large value (larger than any dimension of the region of
*        the input grid being used). In this case, a first attempt to
*        approximate the Mapping by a linear transformation will be
*        made over the entire input region.
*
*        If a smaller value is used, the input region will first be
*        divided into sub-regions whose size does not exceed "maxpix"
*        grid points in any dimension. Only at this point will attempts
*        at approximation commence.
*
*        This value may occasionally be useful in preventing false
*        convergence of the adaptive algorithm in cases where the
*        Mapping appears approximately linear on large scales, but has
*        irregularities (e.g. holes) on smaller scales. A value of,
*        say, 50 to 100 grid points can also be employed as a safeguard 
*        in general-purpose software, since the effect on performance is
*        minimal.
*
*        If too small a value is given, it will have the effect of
*        inhibiting linear approximation altogether (equivalent to
*        setting "tol" to zero). Although this may degrade
*        performance, accurate results will still be obtained.
*     ncoord_out
*        The number of dimensions of the space in which the output points 
*        reside.
*     out
*        Pointer to an array with "ndim_out" elements. Element [i] of
*        this array is a pointer to an array in which to store the 
*        transformed values for output axis "i". The points are ordered 
*        such that the first axis of the input grid changes most rapidly. 
*        For example, if the input grid is 2-dimensional and extends from 
*        (2,-1) to (3,1), the output points will be stored in the order 
*        (2,-1), (3, -1), (2,0), (3,0), (2,1), (3,1).

*/
                      
/* Local Variables: */
   double *flbnd;                /* Array holding floating point lower bounds */
   double *fubnd;                /* Array holding floating point upper bounds */
   double *linear_fit;           /* Pointer to array of fit coefficients */
   int *hi;                      /* Pointer to array of section upper bounds */
   int *lo;                      /* Pointer to array of section lower bounds */
   int coord_in;                 /* Loop counter for input coordinates */
   int dim;                      /* Output section dimension size */
   int dimx;                     /* Dimension with maximum section extent */
   int divide;                   /* Sub-divide the output section? */
   int i;                        /* Loop count */
   int isLinear;                 /* Is the transformation linear? */
   int mxdim;                    /* Largest output section dimension size */
   int npix;                     /* Number of pixels in output section */
   int npoint;                   /* Number of points for obtaining a fit */
   int nvertex;                  /* Number of vertices of output section */
   int toobig;                   /* Section too big (must sub-divide)? */
   int toosmall;                 /* Section too small to sub-divide? */

/* Check the global error status. */
   if ( !astOK ) return;

/* Further initialisation. */
   npix = 1;
   mxdim = 0;
   dimx = 1;
   nvertex = 1;

/* Loop through the input grid dimensions. */
   for ( coord_in = 0; coord_in < ncoord_in; coord_in++ ) {

/* Obtain the extent in each dimension of the input section which is
   to be rebinned, and calculate the total number of pixels it contains. */
      dim = ubnd[ coord_in ] - lbnd[ coord_in ] + 1;
      npix *= dim;

/* Find the maximum dimension size of this input section and note which 
   dimension has this size. */
      if ( dim > mxdim ) {
         mxdim = dim;
         dimx = coord_in;
      }

/* Calculate how many vertices the output section has. */
      nvertex *= 2;
   }
   
/* Calculate how many sample points will be needed (by the astLinearApprox 
   function) to obtain a linear fit to the Mapping's forward transformation. */
   npoint = 1 + 4 * ncoord_in + 2 * nvertex;

/* If the number of pixels in the input section is not at least 4
   times this number, we will probably not save significant time by
   attempting to obtain a linear fit, so note that the input section
   is too small. */
   toosmall = ( npix < ( 4 * npoint ) );

/* Note if the maximum dimension of the input section exceeds the
   user-supplied scale factor. */
   toobig = ( maxpix < mxdim );

/* Assume the Mapping is significantly non-linear before deciding
   whether to sub-divide the output section. */
   linear_fit = NULL;

/* If the output section is too small to be worth obtaining a linear
   fit, or if the accuracy tolerance is zero, we will not
   sub-divide. This means that the Mapping will be used to transform
   each pixel's coordinates and no linear approximation will be
   used. */
   if ( toosmall || ( tol == 0.0 ) ) {
      divide = 0;

/* Otherwise, if the largest input section dimension exceeds the
   scale length given, we will sub-divide. This offers the possibility
   of obtaining a linear approximation to the Mapping over a reduced
   range of input coordinates (which will be handled by a recursive
   invocation of this function). */
   } else if ( toobig ) {
      divide = 1;

/* If neither of the above apply, then attempt to fit a linear
   approximation to the forward transformation of the Mapping over 
   the range of coordinates covered by the input section. We need to 
   temporarily copy the integer bounds into floating point arrays to 
   use astLinearApprox. */
   } else {

/* Allocate memory for floating point bounds and for the coefficient array */
      flbnd = astMalloc( sizeof( double )*(size_t) ncoord_in );
      fubnd = astMalloc( sizeof( double )*(size_t) ncoord_in );
      linear_fit = astMalloc( sizeof( double )*
                              (size_t) ( ncoord_out*( ncoord_in + 1 ) ) );
      if( astOK ) {

/* Copy the bounds into these arrays */
         for( i = 0; i < ncoord_in; i++ ) {
            flbnd[ i ] = (double) lbnd[ i ];
            fubnd[ i ] = (double) ubnd[ i ];
         }

/* Get the linear approximation to the forward transformation. */
         isLinear = astLinearApprox( this, flbnd, fubnd, tol, linear_fit );

/* Free the coeff array if the inverse transformation is not linear. */
         if( !isLinear ) linear_fit = astFree( linear_fit );

      } else {
         linear_fit = astFree( linear_fit );
      }

/* Free resources */
      flbnd = astFree( flbnd );
      fubnd = astFree( fubnd );

/* If a linear fit was obtained, we will use it and therefore do not
   wish to sub-divide further. Otherwise, we sub-divide in the hope
   that this may result in a linear fit next time. */
      divide = !linear_fit;
   }

/* If no sub-division is required, perform the transformation (in a
   memory-efficient manner, since the section we are rebinning might
   still be very large). This will use the linear fit, if obtained
   above. */
   if ( astOK ) {
      if ( !divide ) {
         TranGridWithBlocking( this, linear_fit, ncoord_in, lbnd_in,
                               ubnd_in, lbnd, ubnd, ncoord_out, out );

/* Otherwise, allocate workspace to perform the sub-division. */
      } else {
         lo = astMalloc( sizeof( int ) * (size_t) ncoord_in );
         hi = astMalloc( sizeof( int ) * (size_t) ncoord_in );
         if ( astOK ) {

/* Initialise the bounds of a new input section to match the original
   input section. */
            for ( coord_in = 0; coord_in < ncoord_in; coord_in++ ) {
               lo[ coord_in ] = lbnd[ coord_in ];
               hi[ coord_in ] = ubnd[ coord_in ];
            }

/* Replace the upper bound of the section's largest dimension with the
   mid-point of the section along this dimension, rounded downwards. */
            hi[ dimx ] =
               (int) floor( 0.5 * (double) ( lbnd[ dimx ] + ubnd[ dimx ] ) );

/* Rebin the resulting smaller section using a recursive invocation
   of this function. */
            TranGridAdaptively( this, ncoord_in, lbnd_in, ubnd_in, lo, hi, 
                                tol, maxpix, ncoord_out, out );

/* Now set up a second section which covers the remaining half of the
   original input section. */
            lo[ dimx ] = hi[ dimx ] + 1;
            hi[ dimx ] = ubnd[ dimx ];

/* If this section contains pixels, transform it in the same way. */
            if ( lo[ dimx ] <= hi[ dimx ] ) {
               TranGridAdaptively( this, ncoord_in, lbnd_in, ubnd_in, lo, hi, 
                                   tol, maxpix, ncoord_out, out );
            }
         }

/* Free the workspace. */
         lo = astFree( lo );
         hi = astFree( hi );
      }
   }

/* If coefficients for a linear fit were obtained, then free the space
   they occupy. */
   if ( linear_fit ) linear_fit = astFree( linear_fit );
}

static void TranGridSection( AstMapping *this, const double *linear_fit,
                             int ndim_in, const int *lbnd_in,  
                             const int *ubnd_in, const int *lbnd, 
                             const int *ubnd, int ndim_out, double *out[] ){
/*
*  Name:
*     TranGridSection

*  Purpose:
*     Transform grid points within a section of a rectangular grid.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void TranGridSection( AstMapping *this, const double *linear_fit,
*                           int ndim_in, const int *lbnd_in,  
*                           const int *ubnd_in, const int *lbnd, 
*                           const int *ubnd, int ndim_out, double *out[] )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function transforms grid points within a specified section of a 
*     rectangular grid (with any number of dimensions) using a specified
*     Mapping or, alternatively, a linear approximation fitted to the 
*     Mapping's forward transformation. 

*  Parameters:
*     this
*        Pointer to a Mapping, whose forward transformation may be
*        used to transform the coordinates of points in the input
*        grid.
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     linear_fit
*        Pointer to an optional array of double which contains the
*        coefficients of a linear fit which approximates the above
*        Mapping's forward coordinate transformation. If this is
*        supplied, it will be used in preference to the above Mapping
*        when transforming coordinates. This may be used to enhance
*        performance in cases where evaluation of the Mapping's
*        forward transformation is expensive. If no linear fit is
*        available, a NULL pointer should be supplied.
*
*        The way in which the fit coefficients are stored in this
*        array and the number of array elements are as defined by the
*        astLinearApprox function.
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input data grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input data grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the input data grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     lbnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the first pixel in the
*        section of the input data grid which is to be rebinned.
*     ubnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the last pixel in the
*        section of the input data grid which is to be rebinned.
*
*        Note that "lbnd" and "ubnd" define the shape and position of
*        the section of the input grid which is to be rebinned. This section 
*        should lie wholly within the extent of the input grid (as defined 
*        by the "lbnd_out" and "ubnd_out" arrays). Regions of the input 
*        grid lying outside this section will be ignored.
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     out
*        Pointer to an array with "ndim_out" elements. Element [i] of
*        this array is a pointer to an array in which to store the 
*        transformed values for output axis "i". The points are ordered 
*        such that the first axis of the input grid changes most rapidly. 
*        For example, if the input grid is 2-dimensional and extends from 
*        (2,-1) to (3,1), the output points will be stored in the order 
*        (2,-1), (3, -1), (2,0), (3,0), (2,1), (3,1).

*  Notes:
*     - This function does not take steps to limit memory usage if the
*     grids supplied are large. To resample large grids in a more
*     memory-efficient way, the ResampleWithBlocking function should
*     be used.
*/

/* Local Variables: */
   AstPointSet *pset_in;         /* Input PointSet for transformation */
   AstPointSet *pset_out;        /* Output PointSet for transformation */
   const double *grad;           /* Pointer to gradient matrix of linear fit */
   const double *zero;           /* Pointer to zero point array of fit */
   double **ptr_in;              /* Pointer to input PointSet coordinates */
   double **ptr_out;             /* Pointer to output PointSet coordinates */
   double *accum;                /* Pointer to array of accumulated sums */
   double x1;                    /* Interim x coordinate value */
   double xx1;                   /* Initial x coordinate value */
   double y1;                    /* Interim y coordinate value */
   double yy1;                   /* Initial y coordinate value */
   int *dim;                     /* Pointer to array of output pixel indices */
   int *offset;                  /* Pointer to array of output pixel offsets */
   int *stride;                  /* Pointer to array of output grid strides */
   int coord_in;                 /* Loop counter for input dimensions */
   int coord_out;                /* Loop counter for output dimensions */
   int done;                     /* All pixel indices done? */
   int i1;                       /* Interim offset into "accum" array */
   int i2;                       /* Final offset into "accum" array */
   int idim;                     /* Loop counter for dimensions */
   int ix;                       /* Loop counter for output x coordinate */
   int iy;                       /* Loop counter for output y coordinate */
   int neighb;                   /* Number of neighbouring pixels */
   int npoint;                   /* Number of output points (pixels) */
   int off1;                     /* Interim pixel offset into output array */
   int off2;                     /* Interim pixel offset into output array */
   int off;                      /* Final pixel offset into output array */
   int point;                    /* Counter for output points (pixels ) */
   int s;                        /* Temporary variable for strides */

/* Check the global error status. */
   if ( !astOK ) return;

/* Further initialisation. */
   pset_in = NULL;
   ptr_in = NULL;
   ptr_out = NULL;
   pset_out = NULL;
   neighb = 0;

/* Calculate the number of input points, as given by the product of
   the input grid dimensions. */
   for ( npoint = 1, coord_in = 0; coord_in < ndim_in; coord_in++ ) {
      npoint *= ubnd[ coord_in ] - lbnd[ coord_in ] + 1;
   }

/* Allocate workspace. */
   offset = astMalloc( sizeof( int ) * (size_t) npoint );
   stride = astMalloc( sizeof( int ) * (size_t) ndim_in );
   if ( astOK ) {

/* Calculate the stride for each input grid dimension. */
      off = 0;
      s = 1;
      for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
         stride[ coord_in ] = s;
         s *= ubnd_in[ coord_in ] - lbnd_in[ coord_in ] + 1;
      }

/* A linear fit to the Mapping is available. */
/* ========================================= */
      if ( linear_fit ) {

/* If a linear fit to the Mapping has been provided, then obtain
   pointers to the array of gradients and zero-points comprising the
   fit. */
         grad = linear_fit + ndim_out;
         zero = linear_fit;

/* Create a PointSet to hold the output grid coordinates and obtain an
   array of pointers to its coordinate data. */
         pset_out = astPointSet( npoint, ndim_out, "" );
         ptr_out = astGetPoints( pset_out );
         if ( astOK ) {

/* Initialise the count of input points. */
            point = 0;

/* Handle the 1-dimensional case optimally. */
/* ---------------------------------------- */
            if ( ( ndim_in == 1 ) && ( ndim_out == 1 ) ) {

/* Loop through the pixels of the input grid and transform their x
   coordinates into the output grid's coordinate system using the
   linear fit supplied. Store the results in the PointSet created
   above. */
               off = lbnd[ 0 ] - lbnd_in[ 0 ];
               xx1 = zero[ 0 ] + grad[ 0 ] * (double) lbnd[ 0 ];

               for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                  ptr_out[ 0 ][ point ] = xx1;
                  xx1 += grad[ 0 ];
                  offset[ point++ ] = off++;
               }

/* Handle the 2-dimensional case optimally. */
/* ---------------------------------------- */
            } else if ( ( ndim_in == 2 ) && ( ndim_out == 2 ) ) {

/* Loop through the range of y coordinates in the input grid and
   calculate interim values of the output coordinates using the linear
   fit supplied. */
               x1 = zero[ 0 ] + grad[ 1 ] * (double) ( lbnd[ 1 ] - 1 );
               y1 = zero[ 1 ] + grad[ 3 ] * (double) ( lbnd[ 1 ] - 1 );
               off1 = stride[ 1 ] * ( lbnd[ 1 ] - lbnd_in[ 1 ] - 1 ) - lbnd_in[ 0 ];
               for ( iy = lbnd[ 1 ]; iy <= ubnd[ 1 ]; iy++ ) {
                  x1 += grad[ 1 ];
                  y1 += grad[ 3 ];

/* Also calculate an interim pixel offset into the input array. */
                  off1 += stride[ 1 ];

/* Now loop through the range of input x coordinates and calculate
   the final values of the input coordinates, storing the results in
   the PointSet created above. */
                  xx1 = x1 + grad[ 0 ] * (double) lbnd[ 0 ];
                  yy1 = y1 + grad[ 2 ] * (double) lbnd[ 0 ];
                  off = off1 + lbnd[ 0 ];
                  for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                     ptr_out[ 0 ][ point ] = xx1;
                     xx1 += grad[ 0 ];
                     ptr_out[ 1 ][ point ] = yy1;
                     yy1 += grad[ 2 ];

/* Also calculate final pixel offsets into the input array. */
                     offset[ point++ ] = off++;
                  }
               }

/* Handle other numbers of dimensions. */
/* ----------------------------------- */               
            } else {

/* Allocate workspace. */
               accum = astMalloc( sizeof( double ) *
                                 (size_t) ( ndim_in * ndim_out ) );
               dim = astMalloc( sizeof( int ) * (size_t) ndim_in );
               if ( astOK ) {

/* Initialise an array of pixel indices for the input grid which refer to the 
   first pixel which we will rebin. Also calculate the offset of this pixel 
   within the input array. */
                  off = 0;
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     dim[ coord_in ] = lbnd[ coord_in ];
                     off += stride[ coord_in ] *
                            ( dim[ coord_in ] - lbnd_in[ coord_in ] );
                  }

/* To calculate each output grid coordinate we must perform a matrix
   multiply on the input grid coordinates (using the gradient matrix)
   and then add the zero points. However, since we will usually only
   be altering one input coordinate at a time (the least
   significant), we can avoid the full matrix multiply by accumulating
   partial sums for the most significant input coordinates and only
   altering those sums which need to change each time. The zero points
   never change, so we first fill the "most significant" end of the
   "accum" array with these. */
                  for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                     accum[ ( coord_out + 1 ) * ndim_in - 1 ] =
                                                              zero[ coord_out ];
                  }
                  coord_in = ndim_in - 1;

/* Now loop to process each input pixel. */
                  for ( done = 0; !done; point++ ) {

/* To generate the output coordinate that corresponds to the current
   input pixel, we work down from the most significant dimension
   whose index has changed since the previous pixel we considered
   (given by "coord_in"). For each affected dimension, we accumulate
   in "accum" the matrix sum (including the zero point) for that
   dimension and all higher input dimensions. We must accumulate a
   separate set of sums for each output coordinate we wish to
   produce. (Note that for the first pixel we process, all dimensions
   are considered "changed", so we start by initialising the whole
   "accum" array.) */
                     for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
                        i1 = coord_out * ndim_in;
                        for ( idim = coord_in; idim >= 1; idim-- ) {
                           i2 = i1 + idim;
                           accum[ i2 - 1 ] = accum[ i2 ] +
                                             dim[ idim ] * grad[ i2 ];
                        }

/* The output coordinate for each dimension is given by the accumulated
   sum for input dimension zero (giving the sum over all input
   dimensions). We do not store this in the "accum" array, but assign
   the result directly to the coordinate array of the PointSet created
   earlier. */
                        ptr_out[ coord_out ][ point ] = accum[ i1 ] +
                                                      dim[ 0 ] * grad[ i1 ];
                     }

/* Store the offset of the current pixel in the input array. */
                     offset[ point ] = off;

/* Now update the array of pixel indices to refer to the next input pixel. */
                     coord_in = 0;
                     do {

/* The least significant index which currently has less than its maximum 
   value is incremented by one. The offset into the input array is updated 
   accordingly. */
                        if ( dim[ coord_in ] < ubnd[ coord_in ] ) {
                           dim[ coord_in ]++;
                           off += stride[ coord_in ];
                           break;

/* Any less significant indices which have reached their maximum value
   are returned to their minimum value and the input pixel offset is
   decremented appropriately. */
                        } else {
                           dim[ coord_in ] = lbnd[ coord_in ];
                           off -= stride[ coord_in ] *
                                  ( ubnd[ coord_in ] - lbnd[ coord_in ] );

/* All the output pixels have been processed once the most significant
   pixel index has been returned to its minimum value. */
                           done = ( ++coord_in == ndim_in );
                        }
                     } while ( !done );
                  }
               }

/* Free the workspace. */
               accum = astFree( accum );
               dim = astFree( dim );
            }
         }

/* No linear fit to the Mapping is available. */
/* ========================================== */
      } else {

/* Create a PointSet to hold the coordinates of the input pixels and
   obtain a pointer to its coordinate data. */
         pset_in = astPointSet( npoint, ndim_in, "" );
         ptr_in = astGetPoints( pset_in );
         if ( astOK ) {

/* Initialise the count of input points. */
            point = 0;

/* Handle the 1-dimensional case optimally. */
/* ---------------------------------------- */
            if ( ndim_in == 1 ) {

/* Loop through the required range of input x coordinates, assigning
   the coordinate values to the PointSet created above. Also store a
   pixel offset into the input array. */
               for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                  ptr_in[ 0 ][ point ] = (double) ix;
                  offset[ point++ ] = ix - lbnd_in[ 0 ];
               }

/* Handle the 2-dimensional case optimally. */
/* ---------------------------------------- */
            } else if ( ndim_in == 2 ) {

/* Loop through the required range of input y coordinates,
   calculating an interim pixel offset into the input array. */
               off1 = stride[ 1 ] * ( lbnd[ 1 ] - lbnd_in[ 1 ] - 1 ) 
                      - lbnd_in[ 0 ];
               for ( iy = lbnd[ 1 ]; iy <= ubnd[ 1 ]; iy++ ) {
                  off1 += stride[ 1 ];

/* Loop through the required range of input x coordinates, assigning
   the coordinate values to the PointSet created above. Also store a
   final pixel offset into the input array. */
                  off2 = off1 + lbnd[ 0 ];
                  for ( ix = lbnd[ 0 ]; ix <= ubnd[ 0 ]; ix++ ) {
                     ptr_in[ 0 ][ point ] = (double) ix;
                     ptr_in[ 1 ][ point ] = (double) iy;
                     offset[ point++ ] = off2++;
                  }
               }

/* Handle other numbers of dimensions. */
/* ----------------------------------- */
            } else {

/* Allocate workspace. */
               dim = astMalloc( sizeof( int ) * (size_t) ndim_in );
               if ( astOK ) {

/* Initialise an array of pixel indices for the input grid which
   refer to the first pixel to be rebinned. Also calculate the offset 
   of this pixel within the input array. */
                  off = 0;
                  for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                     dim[ coord_in ] = lbnd[ coord_in ];
                     off += stride[ coord_in ] *
                            ( dim[ coord_in ] - lbnd_in[ coord_in ] );
                  }

/* Loop to generate the coordinates of each input pixel. */
                  for ( done = 0; !done; point++ ) {

/* Copy each pixel's coordinates into the PointSet created above. */
                     for ( coord_in = 0; coord_in < ndim_in; coord_in++ ) {
                        ptr_in[ coord_in ][ point ] =
                                                     (double) dim[ coord_in ];
                     }

/* Store the offset of the pixel in the input array. */
                     offset[ point ] = off;

/* Now update the array of pixel indices to refer to the next input
   pixel. */
                     coord_in = 0;
                     do {

/* The least significant index which currently has less than its
   maximum value is incremented by one. The offset into the input
   array is updated accordingly. */
                        if ( dim[ coord_in ] < ubnd[ coord_in ] ) {
                           dim[ coord_in ]++;
                           off += stride[ coord_in ];
                           break;

/* Any less significant indices which have reached their maximum value
   are returned to their minimum value and the input pixel offset is
   decremented appropriately. */
                        } else {
                           dim[ coord_in ] = lbnd[ coord_in ];
                           off -= stride[ coord_in ] *
                                  ( ubnd[ coord_in ] - lbnd[ coord_in ] );

/* All the input pixels have been processed once the most significant
   pixel index has been returned to its minimum value. */
                           done = ( ++coord_in == ndim_in );
                        }
                     } while ( !done );
                  }
               }

/* Free the workspace. */
               dim = astFree( dim );
            }

/* When all the input pixel coordinates have been generated, use the
   Mapping's forward transformation to generate the output coordinates
   from them. Obtain an array of pointers to the resulting coordinate
   data. */
            pset_out = astTransform( this, pset_in, 1, NULL );
            ptr_out = astGetPoints( pset_out );
         }

/* Annul the PointSet containing the input coordinates. */
         pset_in = astAnnul( pset_in );
      }
   }

/* Copy the output coordinates into the correct positions within the
   supplied "out" array. */
/* ================================================================= */
   if( astOK ) {
      for ( coord_out = 0; coord_out < ndim_out; coord_out++ ) {
         for ( point = 0; point < npoint; point++ ) {
            out[ coord_out ][ offset[ point ] ] = ptr_out[ coord_out ][ point ];
         }
      }
   }

/* Annul the PointSet used to hold output coordinates. */
   pset_out = astAnnul( pset_out );

/* Free the workspace. */
   offset = astFree( offset );
   stride = astFree( stride );
}

static void TranGridWithBlocking( AstMapping *this, const double *linear_fit,
                                  int ndim_in, const int *lbnd_in, 
                                  const int *ubnd_in, const int *lbnd, 
                                  const int *ubnd, int ndim_out, 
                                  double *out[] ){
/*
*  Name:
*     TranGridWithBlocking

*  Purpose:
*     Transforms positions in a section of a grid in a memory-efficient way.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void TranGridWithBlocking( AstMapping *this, const double *linear_fit,
*                                int ndim_in, const int *lbnd_in, 
*                                const int *ubnd_in, const int *lbnd, 
*                                const int *ubnd, int ndim_out, 
*                                double *out[] )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function transforms positions within a specified section of a 
*     rectangular grid (with any number of dimensions) using the forward
*     transformation of the supplied Mapping.
*
*     This function is very similar to TranGridSection, except that in
*     order to limit memory usage and to ensure locality of reference,
*     it divides the input grid up into "blocks" which have a limited
*     extent along each input dimension. Each block, which will not
*     contain more than a pre-determined maximum number of pixels, is
*     then passed to TranGridSection for transformation.

*  Parameters:
*     this
*        Pointer to a Mapping, whose forward transformation may be
*        used to transform the coordinates of pixels in the input
*        grid into associated positions in the output grid.
*
*        The number of input coordintes for the Mapping (Nin
*        attribute) should match the value of "ndim_in" (below), and
*        the number of output coordinates (Nout attribute) should
*        match the value of "ndim_out".
*     linear_fit
*        Pointer to an optional array of double which contains the
*        coefficients of a linear fit which approximates the above
*        Mapping's forward coordinate transformation. If this is
*        supplied, it will be used in preference to the above Mapping
*        when transforming coordinates. This may be used to enhance
*        performance in cases where evaluation of the Mapping's
*        forward transformation is expensive. If no linear fit is
*        available, a NULL pointer should be supplied.
*
*        The way in which the fit coefficients are stored in this
*        array and the number of array elements are as defined by the
*        astLinearApprox function.
*     ndim_in
*        The number of dimensions in the input grid. This should be at
*        least one.
*     lbnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the first
*        pixel in the input grid along each dimension.
*     ubnd_in
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the centre of the last
*        pixel in the input grid along each dimension.
*
*        Note that "lbnd_in" and "ubnd_in" together define the shape
*        and size of the whole input grid, its extent along a
*        particular (i'th) dimension being (ubnd_in[i] - lbnd_in[i] +
*        1). They also define the input grid's coordinate system, with
*        each pixel being of unit extent along each dimension with
*        integral coordinate values at its centre.
*     lbnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the first pixel in the
*        section of the input data grid which is to be transformed.
*     ubnd
*        Pointer to an array of integers, with "ndim_in" elements.
*        This should give the coordinates of the last pixel in the
*        section of the input data grid which is to be transformed.
*
*        Note that "lbnd" and "ubnd" define the shape and position of the 
*        section of the input grid which is to be transformed. 
*     ndim_out
*        The number of dimensions in the output grid. This should be
*        at least one.
*     out
*        Pointer to an array with "ndim_out" elements. Element [i] of
*        this array is a pointer to an array in which to store the 
*        transformed values for output axis "i". The points are ordered 
*        such that the first axis of the input grid changes most rapidly. 
*        For example, if the input grid is 2-dimensional and extends from 
*        (2,-1) to (3,1), the output points will be stored in the order 
*        (2,-1), (3, -1), (2,0), (3,0), (2,1), (3,1).

*/

/* Local Constants: */
   const int mxpix = 2 * 1024;   /* Maximum number of pixels in a block (this
                                    relatively small number seems to give best
                                    performance) */

/* Local Variables: */
   int *dim_block;               /* Pointer to array of block dimensions */
   int *lbnd_block;              /* Pointer to block lower bound array */
   int *ubnd_block;              /* Pointer to block upper bound array */
   int dim;                      /* Dimension size */
   int done;                     /* All blocks rebinned? */
   int hilim;                    /* Upper limit on maximum block dimension */
   int idim;                     /* Loop counter for dimensions */
   int lolim;                    /* Lower limit on maximum block dimension */
   int mxdim_block;              /* Maximum block dimension */
   int npix;                     /* Number of pixels in block */

/* Check the global error status. */
   if ( !astOK ) return;

/* Allocate workspace. */
   lbnd_block = astMalloc( sizeof( int ) * (size_t) ndim_in );
   ubnd_block = astMalloc( sizeof( int ) * (size_t) ndim_in );
   dim_block = astMalloc( sizeof( int ) * (size_t) ndim_in );
   if ( astOK ) {

/* Find the optimum block size. */
/* ---------------------------- */
/* We first need to find the maximum extent which a block of input
   pixels may have in each dimension. We determine this by taking the
   input grid extent in each dimension and then limiting the maximum
   dimension size until the resulting number of pixels is sufficiently
   small. This approach allows the block shape to approximate (or
   match) the input grid shape when appropriate. */

/* First loop to calculate the total number of input pixels and the
   maximum input dimension size. */
      npix = 1;
      mxdim_block = 0;
      for ( idim = 0; idim < ndim_in; idim++ ) {
         dim = ubnd[ idim ] - lbnd[ idim ] + 1;
         npix *= dim;
         if ( mxdim_block < dim ) mxdim_block = dim;
      }

/* If the number of input pixels is too large for a single block, we
   perform iterations to determine the optimum upper limit on a
   block's dimension size. Initialise the limits on this result. */
      if ( npix > mxpix ) {
         lolim = 1;
         hilim = mxdim_block;

/* Loop to perform a binary chop, searching for the best result until
   the lower and upper limits on the result converge to adjacent
   values. */
         while ( ( hilim - lolim ) > 1 ) {

/* Form a new estimate from the mid-point of the previous limits. */
            mxdim_block = ( hilim + lolim ) / 2;

/* See how many pixels a block contains if its maximum dimension is
   limited to this new value. */
            for ( npix = 1, idim = 0; idim < ndim_in; idim++ ) {
               dim = ubnd[ idim ] - lbnd[ idim ] + 1;
               npix *= ( dim < mxdim_block ) ? dim : mxdim_block;
            }

/* Update the appropriate limit, according to whether the number of
   pixels is too large or too small. */
            *( ( npix <= mxpix ) ? &lolim : &hilim ) = mxdim_block;
         }

/* When iterations have converged, obtain the maximum limit on the
   dimension size of a block which results in no more than the maximum
   allowed number of pixels per block. However, ensure that all block
   dimensions are at least 2. */
            mxdim_block = lolim;
      }
      if ( mxdim_block < 2 ) mxdim_block = 2;

/* Calculate the block dimensions by applying this limit to the output
   grid dimensions. */
      for ( idim = 0; idim < ndim_in; idim++ ) {
         dim = ubnd[ idim ] - lbnd[ idim ] + 1;
         dim_block[ idim ] = ( dim < mxdim_block ) ? dim : mxdim_block;

/* Also initialise the lower and upper bounds of the first block of
   output grid pixels to be rebinned, ensuring that this does not
   extend outside the grid itself. */
         lbnd_block[ idim ] = lbnd[ idim ];
         ubnd_block[ idim ] = MinI( lbnd[ idim ] + dim_block[ idim ] - 1,
                                    ubnd[ idim ] );
      }

/* Transform each block of input grid positions. */
/* --------------------------------------------- */
/* Loop to generate the extent of each block of input grid positions and to
   transform them. */
      done = 0;
      while ( !done && astOK ) {

/* Rebin the current block, accumulating the sum of bad pixels produced. */
         TranGridSection( this, linear_fit, ndim_in, lbnd_in, ubnd_in,
                          lbnd_block, ubnd_block, ndim_out, out );

/* Update the block extent to identify the next block of input pixels. */
         idim = 0;
         do {

/* We find the least significant dimension where the upper bound of
   the block has not yet reached the upper bound of the region of the
   input grid which we are rebinning. The block's position is then
   incremented by one block extent along this dimension, checking that
   the resulting extent does not go outside the region being rebinned. */
            if ( ubnd_block[ idim ] < ubnd[ idim ] ) {
               lbnd_block[ idim ] = MinI( lbnd_block[ idim ] +
                                          dim_block[ idim ], ubnd[ idim ] );
               ubnd_block[ idim ] = MinI( lbnd_block[ idim ] +
                                          dim_block[ idim ] - 1,
                                          ubnd[ idim ] );
               break;

/* If any less significant dimensions are found where the upper bound
   of the block has reached its maximum value, we reset the block to
   its lowest position. */
            } else {
               lbnd_block[ idim ] = lbnd[ idim ];
               ubnd_block[ idim ] = MinI( lbnd[ idim ] + dim_block[ idim ] - 1,
                                          ubnd[ idim ] );

/* All the blocks have been processed once the position along the most
   significant dimension has been reset. */
               done = ( ++idim == ndim_out );
            }
         } while ( !done );
      }
   }

/* Free the workspace. */
   lbnd_block = astFree( lbnd_block );
   ubnd_block = astFree( ubnd_block );
   dim_block = astFree( dim_block );
}

static void TranN( AstMapping *this, int npoint,
                   int ncoord_in, int indim, const double *in,
                   int forward,
                   int ncoord_out, int outdim, double *out ) {
/*
*++
*  Name:
c     astTranN
f     AST_TRANN

*  Purpose:
*     Transform N-dimensional coordinates.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astTranN( AstMapping *this, int npoint,
c                    int ncoord_in, int indim, const double *in,
c                    int forward,
c                    int ncoord_out, int outdim, double *out )
f     CALL AST_TRANN( THIS, NPOINT,
f                     NCOORD_IN, INDIM, IN,
f                     FORWARD, NCOORD_OUT, OUTDIM, OUT, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function applies a Mapping to transform the coordinates of
f     This routine applies a Mapping to transform the coordinates of
*     a set of points in an arbitrary number of dimensions. It is the
*     appropriate routine to use if the coordinates are not purely 1-
*     or 2-dimensional and are stored in a single array (which they
*     need not fill completely).
c
c     If the coordinates are not stored in a single array, then the
c     astTranP function might be more suitable.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping to be applied.
c     npoint
f     NPOINT = INTEGER (Given)
*        The number of points to be transformed.
c     ncoord_in
f     NCOORD_IN = INTEGER (Given)
*        The number of coordinates being supplied for each input point
*        (i.e. the number of dimensions of the space in which the
*        input points reside).
c     indim
f     INDIM = INTEGER (Given)
c        The number of elements along the second dimension of the "in"
f        The number of elements along the first dimension of the IN
*        array (which contains the input coordinates). This value is
*        required so that the coordinate values can be correctly
*        located if they do not entirely fill this array. The value
c        given should not be less than "npoint".
f        given should not be less than NPOINT.
c     in
f     IN( INDIM, NCOORD_IN ) = DOUBLE PRECISION (Given)
c        The address of the first element in a 2-dimensional array of 
c        shape "[ncoord_in][indim]",
c        containing the coordinates of the input (untransformed)
c        points. These should be stored such that the value of
c        coordinate number "coord" for input point number "point" is
c        found in element "in[coord][point]".
f        An array containing the coordinates of the input
f        (untransformed) points. These should be stored such that the
f        value of coordinate number COORD for input point number POINT
f        is found in element IN(POINT,COORD).
c     forward
f     FORWARD = LOGICAL (Given)
c        A non-zero value indicates that the Mapping's forward
c        coordinate transformation is to be applied, while a zero
c        value indicates that the inverse transformation should be
c        used.
f        A .TRUE. value indicates that the Mapping's forward
f        coordinate transformation is to be applied, while a .FALSE.
f        value indicates that the inverse transformation should be
f        used.
c     ncoord_out
f     NCOORD_OUT = INTEGER (Given)
*        The number of coordinates being generated by the Mapping for
*        each output point (i.e. the number of dimensions of the
*        space in which the output points reside). This need not be
c        the same as "ncoord_in".
f        the same as NCOORD_IN.
c     outdim
f     OUTDIM = INTEGER (Given)
c        The number of elements along the second dimension of the "out"
f        The number of elements along the first dimension of the OUT
*        array (which will contain the output coordinates). This value
*        is required so that the coordinate values can be correctly
*        located if they will not entirely fill this array. The value
c        given should not be less than "npoint".
f        given should not be less than NPOINT.
c     out
f     OUT( OUTDIM, NCOORD_OUT ) = DOUBLE PRECISION (Returned)
c        The address of the first element in a 2-dimensional array of 
c        shape "[ncoord_out][outdim]", into
c        which the coordinates of the output (transformed) points will
c        be written. These will be stored such that the value of
c        coordinate number "coord" for output point number "point"
c        will be found in element "out[coord][point]".
f        An array into which the coordinates of the output
f        (transformed) points will be written. These will be stored
f        such that the value of coordinate number COORD for output
f        point number POINT will be found in element OUT(POINT,COORD).
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
c     - If the forward coordinate transformation is being applied, the
c     Mapping supplied must have the value of "ncoord_in" for its Nin
c     attribute and the value of "ncoord_out" for its Nout attribute. If
c     the inverse transformation is being applied, these values should
c     be reversed.
f     - If the forward coordinate transformation is being applied, the
f     Mapping supplied must have the value of NCOORD_IN for its Nin
f     attribute and the value of NCOORD_OUT for its Nout attribute. If
f     the inverse transformation is being applied, these values should
f     be reversed.
*--
*/

/* Local Variables: */
   AstPointSet *in_points;       /* Pointer to input PointSet */
   AstPointSet *out_points;      /* Pointer to output PointSet */
   const double **in_ptr;        /* Pointer to array of input data pointers */
   double **out_ptr;             /* Pointer to array of output data pointers */
   int coord;                    /* Loop counter for coordinates */

/* Check the global error status. */
   if ( !astOK ) return;

/* Validate the mapping and numbers of points/coordinates. */
   ValidateMapping( this, forward, npoint, ncoord_in, ncoord_out, "astTranN" );

/* Also validate the input array dimension argument. */
   if ( astOK && ( indim < npoint ) ) {
      astError( AST__DIMIN, "astTranN(%s): The input array dimension value "
                "(%d) is invalid.", astGetClass( this ), indim );
      astError( AST__DIMIN, "This should not be less than the number of "
                "points being transformed (%d).", npoint );
   }

/* Similarly, validate the output array dimension argument. */
   if ( astOK && ( outdim < npoint ) ) {
      astError( AST__DIMIN, "astTranN(%s): The output array dimension value "
                "(%d) is invalid.", astGetClass( this ), outdim );
      astError( AST__DIMIN, "This should not be less than the number of "
                "points being transformed (%d).", npoint );
   }

/* Allocate memory to hold the arrays of input and output data
   pointers. */
   if ( astOK ) {
      in_ptr = (const double **) astMalloc( sizeof( const double * ) *
                                            (size_t) ncoord_in );
      out_ptr = astMalloc( sizeof( double * ) * (size_t) ncoord_out );


#ifdef DEBUG
      { int i, ns;
        ns = ncoord_out*outdim;
        for( i = 0; i < ns; i++ ) out[ i ] = 0.0;
      }
#endif


/* Initialise the input data pointers to locate the coordinate data in
   the "in" array. */
      if ( astOK ) {
         for ( coord = 0; coord < ncoord_in; coord++ ) {
            in_ptr[ coord ] = in + coord * indim;
         }

/* Similarly initialise the output data pointers to point into the
   "out" array. */
         for ( coord = 0; coord < ncoord_out; coord++ ) {
            out_ptr[ coord ] = out + coord * outdim;
         }

/* Create PointSets to describe the input and output points. */
         in_points = astPointSet( npoint, ncoord_in, "" );
         out_points = astPointSet( npoint, ncoord_out, "" );

/* Associate the data pointers with the PointSets (note we must
   explicitly remove the "const" qualifier from the input data here,
   although they will not be modified).  */
         astSetPoints( in_points, (double **) in_ptr );
         astSetPoints( out_points, out_ptr );

/* Apply the required transformation to the coordinates. */
         (void) astTransform( this, in_points, forward, out_points );

/* If the Mapping's Report attribute is set, report the effect the
   Mapping has had on the coordinates. */
         if ( astGetReport( this ) ) astReportPoints( this, forward,
                                                      in_points, out_points );

/* Delete the two PointSets. */
         in_points = astDelete( in_points );
         out_points = astDelete( out_points );
      }

/* Free the memory used for the data pointers. */
      in_ptr = (const double **) astFree( (void *) in_ptr );
      out_ptr = astFree( out_ptr );
   }
}

static void TranP( AstMapping *this, int npoint,
                   int ncoord_in, const double *ptr_in[],
                   int forward, int ncoord_out, double *ptr_out[] ) {
/*
c++
*  Name:
*     astTranP

*  Purpose:
*     Transform N-dimensional coordinates held in separate arrays.

*  Type:
*     Public virtual function.

*  Synopsis:
*     #include "mapping.h"
*     void astTranP( AstMapping *this, int npoint,
*                    int ncoord_in, const double *ptr_in[],
*                    int forward, int ncoord_out, double *ptr_out[] )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function applies a Mapping to transform the coordinates of
*     a set of points in an arbitrary number of dimensions. It is the
*     appropriate routine to use if the coordinates are not purely 1-
*     or 2-dimensional and are stored in separate arrays, since each
*     coordinate array is located by supplying a separate pointer to
*     it.
*
*     If the coordinates are stored in a single (2-dimensional) array,
*     then the astTranN function might be more suitable.

*  Parameters:
*     this
*        Pointer to the Mapping to be applied.
*     npoint
*        The number of points to be transformed.
*     ncoord_in
*        The number of coordinates being supplied for each input point
*        (i.e. the number of dimensions of the space in which the
*        input points reside).
*     ptr_in
*        An array of pointers to double, with "ncoord_in"
*        elements. Element "ptr_in[coord]" should point at the first
*        element of an array of double (with "npoint" elements) which
*        contain the values of coordinate number "coord" for each
*        input (untransformed) point. The value of coordinate number
*        "coord" for input point number "point" is therefore given by
*        "ptr_in[coord][point]" (assuming both indices are
*        zero-based).
*     forward
*        A non-zero value indicates that the Mapping's forward
*        coordinate transformation is to be applied, while a zero
*        value indicates that the inverse transformation should be
*        used.
*     ncoord_out
*        The number of coordinates being generated by the Mapping for
*        each output point (i.e. the number of dimensions of the space
*        in which the output points reside). This need not be the same
*        as "ncoord_in".
*     ptr_out
*        An array of pointers to double, with "ncoord_out"
*        elements. Element "ptr_out[coord]" should point at the first
*        element of an array of double (with "npoint" elements) into
*        which the values of coordinate number "coord" for each output
*        (transformed) point will be written.  The value of coordinate
*        number "coord" for output point number "point" will therefore
*        be found in "ptr_out[coord][point]".

*  Notes:
*     - If the forward coordinate transformation is being applied, the
*     Mapping supplied must have the value of "ncoord_in" for its Nin
*     attribute and the value of "ncoord_out" for its Nout
*     attribute. If the inverse transformation is being applied, these
*     values should be reversed.
*     - This routine is not available in the Fortran 77 interface to
*     the AST library.
c--
*/

/* Local Variables: */
   AstPointSet *in_points;       /* Pointer to input PointSet */
   AstPointSet *out_points;      /* Pointer to output PointSet */

/* Check the global error status. */
   if ( !astOK ) return;

/* Validate the Mapping and number of points/coordinates. */
   ValidateMapping( this, forward, npoint, ncoord_in, ncoord_out, "astTranP" );

/* Create PointSets to describe the input and output points. */
   if ( astOK ) {
      in_points = astPointSet( npoint, ncoord_in, "" );
      out_points = astPointSet( npoint, ncoord_out, "" );

/* Associate the data pointers with the PointSets (note we must
   explicitly remove the "const" qualifier from the input data here,
   although they will not be modified).  */
      astSetPoints( in_points, (double **) ptr_in );
      astSetPoints( out_points, ptr_out );

/* Apply the required transformation to the coordinates. */
      (void) astTransform( this, in_points, forward, out_points );

/* If the Mapping's Report attribute is set, report the effect the
   Mapping has had on the coordinates. */
      if ( astGetReport( this ) ) astReportPoints( this, forward,
                                                   in_points, out_points );

/* Delete the two PointSets. */
      in_points = astDelete( in_points );
      out_points = astDelete( out_points );
   }
}

static AstPointSet *Transform( AstMapping *this, AstPointSet *in,
                               int forward, AstPointSet *out ) {
/*
*+
*  Name:
*     astTransform

*  Purpose:
*     Transform a set of points.

*  Type:
*     Protected virtual function.

*  Synopsis:
*     #include "mapping.h"
*     AstPointSet *astTransform( AstMapping *this, AstPointSet *in,
*                                int forward, AstPointSet *out )

*  Class Membership:
*     Mapping method.

*  Description:
*     This function takes a Mapping and a set of points encapsulated
*     in a PointSet, and applies either the forward or inverse
*     coordinate transformation (if defined by the Mapping) to the
*     points.

*  Parameters:
*     this
*        Pointer to the Mapping. The nature of the coordinate
*        transformation will depend on the class of Mapping
*        supplied. Note that there is no constructor for the Mapping
*        class itself, so this object should be from a derived class.
*     in
*        Pointer to the PointSet holding the input coordinate data.
*     forward
*        A non-zero value indicates that the forward coordinate
*        transformation should be applied, while a zero value requests
*        the inverse transformation.
*     out
*        Pointer to a PointSet which will hold the transformed
*        (output) coordinate values. A NULL value may also be given,
*        in which case a new PointSet will be created by this
*        function.

*  Returned Value:
*     Pointer to the output (possibly new) PointSet.

*  Notes:
*     - An error will result if the Mapping supplied does not define
*     the requested coordinate transformation (either forward or
*     inverse).
*     - The number of coordinate values per point in the input
*     PointSet must match the number of input coordinates for the
*     Mapping being applied (or number of output coordinates if the
*     inverse transformation is requested).
*     - If an output PointSet is supplied, it must have space for
*     sufficient number of points and coordinate values per point to
*     accommodate the result (e.g. the number of Mapping output
*     coordinates, or number of input coordinates if the inverse
*     transformation is requested). Any excess space will be ignored.
*     - A null pointer will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   AstPointSet *result;          /* Pointer to output PointSet */
   int def;                      /* Coordinate transformation defined? */
   int ncoord_in;                /* Number of input PointSet coordinates */
   int ncoord_out;               /* Number of coordinates in output PointSet */
   int nin;                      /* Number of input Mapping coordinates */
   int nout;                     /* Number of output Mapping coordinates */
   int npoint;                   /* Number of points to transform */
   int npoint_out;               /* Number of points in output PointSet */

/* Check the global error status. */
   if ( !astOK ) return NULL;

/* Initialise. */
   result = NULL;

/* Determine if a coordinate transformation is defined for the requested
   direction. */
   def = forward ? astGetTranForward( this ) : astGetTranInverse( this );

/* Report an error if the transformation is not defined. */
   if ( astOK && !def ) {
      astError( AST__TRNND, "astTransform(%s): %s coordinate transformation "
                "is not defined by the %s supplied.", astGetClass( this ),
                forward ? "A forward" : "An inverse", astGetClass( this ) );
   }

/* Obtain the effective number of input and output coordinate values for the
   transformation to be performed, taking account of the transformation
   direction required. Note we use Mapping methods to obtain these values, as
   this will take account of whether the Mapping has been inverted. */
   nin = forward ? astGetNin( this ) : astGetNout( this );
   nout = forward ? astGetNout( this ) : astGetNin( this );

/* Obtain the number of input points to transform and the number of coordinate
   values per input point. */
   npoint = astGetNpoint( in );
   ncoord_in = astGetNcoord( in );

/* If OK, check that the number of input coordinates matches the number
   required by the mapping. Report an error if these numbers do not match. */
   if ( astOK && ( ncoord_in != nin ) ) {
      astError( AST__NCPIN, "astTransform(%s): Bad number of coordinate "
                "values (%d) in input %s.", astGetClass( this ), ncoord_in,
                astGetClass( in ) );
      astError( AST__NCPIN, "The %s given requires %d coordinate value(s) for "
                "each input point.", astGetClass( this ), nin );
   }

/* If still OK, and a non-NULL pointer has been given for the output PointSet,
   then obtain the number of points and number of coordinates per point for
   this PointSet. */
   if ( astOK && out ) {
      npoint_out = astGetNpoint( out );
      ncoord_out = astGetNcoord( out );

/* Check that the dimensions of this PointSet are adequate to accommodate the
   output coordinate values and report an error if they are not. */
      if ( astOK ) {
         if ( npoint_out < npoint ) {
            astError( AST__NOPTS, "astTransform(%s): Too few points (%d) in "
                      "output %s.", astGetClass( this ), npoint_out,
                      astGetClass( out ) );
            astError( AST__NOPTS, "The %s needs space to hold %d transformed "
                      "point(s).", astGetClass( this ), npoint );
         } else if ( ncoord_out < nout ) {
            astError( AST__NOCTS, "astTransform(%s): Too few coordinate "
                      "values per point (%d) in output %s.",
                      astGetClass( this ), ncoord_out, astGetClass( out ) );
            astError( AST__NOCTS, "The %s supplied needs space to store %d "
                      "coordinate value(s) per transformed point.",
                      astGetClass( this ), nout );
         }
      }
   }

/* If all the validation stages are passed successfully, and a NULL output
   pointer was given, then create a new PointSet to encapsulate the output
   coordinate data. */
   if ( astOK ) {
      if ( !out ) {
         result = astPointSet( npoint, nout, "" );

/* Otherwise, use the PointSet supplied. */
      } else {
         result = out;
      }
   }

/* Return a pointer to the output PointSet. Note that we do not actually
   transform (or even copy) the coordinates. This is left for derived classes
   to implement. */
   return result;
}

/*
*++
*  Name:
c     astUinterp
f     AST_UINTERP

*  Purpose:
*     Perform sub-pixel interpolation on a grid of data.

*  Type:
*     Fictitious function.

*  Synopsis:
c     #include "mapping.h"
c     void astUinterp( int ndim_in, const int lbnd_in[], const int ubnd_in[],
c                      const <Xtype> in[], const <Xtype> in_var[],
c                      int npoint, const int offset[],
c                      const double *const coords[], const double params[],
c                      int flags, <Xtype> badval,
c                      <Xtype> out[], <Xtype> out_var[], int *nbad )
f     CALL AST_UINTERP( NDIM_IN, LBND_IN, UBND_IN, IN, IN_VAR,
f                       NPOINT, OFFSET, COORDS, PARAMS, FLAGS, BADVAL,
f                       OUT, OUT_VAR, NBAD, STATUS )

*  Class Membership:
*     Mapping member function.

*  Description:
c     This is a fictitious function which does not actually
c     exist. Instead, this description constitutes a template so that
c     you may implement a function with this interface for yourself
c     (and give it any name you wish). A pointer to such a function
c     may be passed via the "finterp" parameter of the astResample<X>
c     functions (q.v.) in order to perform sub-pixel interpolation
c     during resampling of gridded data (you must also set the
c     "interp" parameter of astResample<X> to the value
c     AST__UINTERP). This allows you to use your own interpolation
c     algorithm in addition to those which are pre-defined.
f     This is a fictitious routine which does not actually
f     exist. Instead, this description constitutes a template so that
f     you may implement a routine with this interface for yourself
f     (and give it any name you wish). Such a routine
f     may be passed via the FINTERP argument of the AST_RESAMPLE<X>
f     functions (q.v.) in order to perform sub-pixel interpolation
f     during resampling of gridded data (you must also set the
f     INTERP argument of AST_RESAMPLE<X> to the value
f     AST__UINTERP). This allows you to use your own interpolation
f     algorithm in addition to those which are pre-defined.
*
c     The function interpolates an input grid of data (and,
f     The routine interpolates an input grid of data (and,
*     optionally, processes associated statistical variance estimates)
*     at a specified set of points.

*  Parameters:
c     ndim_in
f     NDIM_IN = INTEGER (Given)
*        The number of dimensions in the input grid. This will be at
*        least one.
c     lbnd_in
f     LBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the first pixel
*        in the input grid along each dimension.
c     ubnd_in
f     UBND_IN( NDIM_IN ) = INTEGER (Given)
c        Pointer to an array of integers, with "ndim_in" elements,
f        An array
*        containing the coordinates of the centre of the last pixel in
*        the input grid along each dimension.
*
c        Note that "lbnd_in" and "ubnd_in" together define the shape,
f        Note that LBND_IN and UBND_IN together define the shape,
*        size and coordinate system of the input grid in the same
c        way as they do in astResample<X>.
f        way as they do in AST_RESAMPLE<X>.
c     in
f     IN( * ) = <Xtype> (Given)
c        Pointer to an array, with one element for each pixel in the
f        An array, with one element for each pixel in the
*        input grid, containing the input data. This will be the same
c        array as was passed to astResample<X> via the "in" parameter.
f        array as was passed to AST_RESAMPLE<X> via the IN argument.
*        The numerical type of this array should match that of the
*        data being processed.
c     in_var
f     IN_VAR( * ) = <Xtype> (Given)
c        Pointer to an optional second array with the same size and
c        type as the "in" array. If given, this will contain the set
c        of variance values associated with the input data and will be
c        the same array as was passed to astResample<X> via the
c        "in_var" parameter.
f        An optional second array with the same size and type as the
f        IN array. This will only be given if the AST__USEVAR flag is
f        set via the FLAGS argument (below). If given, it will contain
f        the set of variance values associated with the input data and
f        will be the same array as was passed to AST_RESAMPLE<X> via
f        the IN_VAR argument.
*
c        If no variance values are being processed, this will be a
c        NULL pointer.
f        If the AST__USEVAR flag is not set, then no variance values
f        are being processed. In this case, this array of variance
f        values may be a dummy (e.g. one-element) array and should not
f        be used.
c     npoint
f     NPOINT = INTEGER (Given)
*        The number of points at which the input grid is to be
*        interpolated. This will be at least one.
c     offset
f     OFFSET( NPOINT ) = INTEGER (Given)
c        Pointer to an array of integers with "npoint" elements. For
c        each interpolation point, this will contain the zero-based
c        index in the "out" (and "out_var") array(s) at which the
c        interpolated value (and its variance, if required) should be
c        stored. For example, the interpolated value for point number
c        "point" should be stored in "out[offset[point]]" (assuming
c        the index "point" is zero-based).
f        For each interpolation point, this array will contain the
f        offset from the start of the OUT (and OUT_VAR) array(s) at
f        which the interpolated value (and its variance, if required)
f        should be stored. For example, the interpolated value for
f        point number POINT should be stored in OUT(1+OFFSET(POINT)).
c     coords
f     COORDS( NPOINT, NDIM_IN ) = DOUBLE PRECISION (Given)
c        An array of pointers to double, with "ndim_in"
c        elements. Element "coords[coord]" will point at the first
c        element of an array of double (with "npoint" elements) which
c        contains the values of coordinate number "coord" for each
c        interpolation point. The value of coordinate number "coord"
c        for interpolation point number "point" is therefore given by
c        "coords[coord][point]" (assuming both indices are
c        zero-based).
f        A 2-dimensional array containing the coordinates of the
f        points at which interpolation should be performed. These will
f        be stored so that coordinate number COORD for interpolation
f        point number POINT is found in element COORDS(POINT,COORD).
*
*        If any interpolation point has any of its coordinates equal
c        to the value AST__BAD (as defined in the "ast.h" header
f        to the value AST__BAD (as defined in the AST_PAR include
*        file), then the corresponding output data (and variance)
c        should be set to the value given by "badval" (see below).
f        should be set to the value given by BADVAL (see below).
c     params
f     PARAMS( * ) = DOUBLE PRECISION (Given)
c        This will be a pointer to the same array as was given via the
c        "params" parameter of astResample<X>. You may use this to
f        This will be the same array as was given via the
f        PARAMS argument of AST_RESAMPLE<X>. You may use this to
*        pass any additional parameter values required by your
*        interpolation algorithm.
c     flags
f     FLAGS = INTEGER (Given)
c        This will be the same value as was given via the "flags"
c        parameter of astResample<X>. You may test this value to
f        This will be the same value as was given via the FLAGS
f        argument of AST_RESAMPLE<X>. You may test this value to
*        provide additional control over the operation of your
*        resampling algorithm. Note that the special flag values
*        AST__URESAMP1, 2, 3 & 4 are reserved for you to use for your
*        own purposes and will not clash with other pre-defined flag
c        values (see astResample<X>).
f        values (see AST_RESAMPLE<X>).
c     badval
f     BADVAL = <Xtype> (Given)
c        This will be the same value as was given via the "badval"
c        parameter of astResample<X>, and will have the same numerical
c        type as the data being processed (i.e. as elements of the
c        "in" array).  It should be used to test for bad pixels in the
c        input grid (but only if the AST__USEBAD flag is set via the
c        "flags" parameter) and for identifying bad output values in
c        the "out" (and "out_var") array(s).
f        This will be the same value as was given for the BADVAL
f        argument of AST_RESAMPLE<X>, and will have the same numerical
f        type as the data being processed (i.e. as elements of the IN
f        array).  It should be used to test for bad pixels in the
f        input grid (but only if the AST__USEBAD flag is set via the
f        FLAGS argument) and for identifying bad output values in the
f        OUT (and OUT_VAR) array(s).
c     out
f     OUT( * ) = <Xtype> (Returned)
c        Pointer to an array with the same numerical type as the "in"
f        An array with the same numerical type as the IN
*        array, into which the interpolated data values should be
*        returned.  Note that details of the storage order and number
*        of dimensions of this array are not required, since the
c        "offset" array contains all necessary information about where
f        OFFSET array contains all necessary information about where
*        each returned value should be stored.
*
c        In general, not all elements of this array (or the "out_var"
f        In general, not all elements of this array (or the OUT_VAR
*        array below) may be used in any particular invocation of the
c        function. Those which are not used should be returned
f        routine. Those which are not used should be returned
*        unchanged.
c     out_var
f     OUT_VAR( * ) = <Xtype> (Returned)
c        Pointer to an optional array with the same type and size as
c        the "out" array, into which variance estimates for the
c        resampled values should be returned.  This array will only be
c        given if the "in_var" array has also been given.
f        An optional array with the same type and size as the OUT
f        array, into which variance estimates for the resampled values
f        should be returned. This array will only be given if the
f        AST__USEVAR flag is set via the FLAGS argument.
*
c        If given, it is addressed in exactly the same way (via the
c        "offset" array) as the "out" array. The values returned
c        should be estimates of the statistical variance of the
c        corresponding values in the "out" array, on the assumption
c        that all errors in input data values are statistically
c        independent and that their variance estimates may simply be
c        summed (with appropriate weighting factors).
f        If given, it is addressed in exactly the same way (via the
f        OFFSET array) as the OUT array. The values returned should be
f        estimates of the statistical variance of the corresponding
f        values in the OUT array, on the assumption that all errors in
f        input data values are statistically independent and that
f        their variance estimates may simply be summed (with
f        appropriate weighting factors).
*
c        If no output variance estimates are required, a NULL pointer
c        will be given.
f        If the AST__USEVAR flag is not set, then variance values are
f        not being processed.  In this case, this array may be a dummy
f        (e.g. one-element) array and should not be used.
c     nbad
f     NBAD = INTEGER (Returned)
c        Pointer to an int in which to return the number of interpolation
c        points at
f        This should return the number of interpolation points at
*        which an output data value (and/or a variance value if
c        relevant) equal to "badval" has been assigned because no
f        relevant) equal to BADVAL has been assigned because no
*        valid interpolated value could be obtained.  The maximum
c        value that should be returned is "npoint", and the minimum is
f        value that should be returned is NPOINT, and the minimum is
*        zero (indicating that all output values were successfully
*        obtained).
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
*     - The data type <Xtype> indicates the numerical type of the data
c     being processed, as for astResample<X>.
f     being processed, as for AST_RESAMPLE<X>.
c     - This function will typically be invoked more than once for each
c     invocation of astResample<X>.
f     - This routine will typically be invoked more than once for each
f     invocation of AST_RESAMPLE<X>.
c     - If an error occurs within this function, it should use
c     astSetStatus to set the AST error status to an error value.
c     This will cause an immediate return from astResample<X>. The error
c     value AST__UINER is available for this purpose, but other values may
c     also be used (e.g. if you wish to distinguish different types of
c     error).
f     - If an error occurs within this routine, it should set the
f     STATUS argument to an error value before returning. This will
f     cause an immediate return from AST_RESAMPLE<X>. The error value
f     AST__UINER is available for this purpose, but other values may also
f     be used (e.g. if you wish to distinguish different types of error).
f     The AST__UINER error value is defined in the AST_ERR include file.
*--
*/
/* Note the above is just a description to act as a template. The
   function does not actually exist. */

/*
*++
*  Name:
c     astUkern1
f     AST_UKERN1

*  Purpose:
*     1-dimensional sub-pixel interpolation kernel.

*  Type:
*     Fictitious function.

*  Synopsis:
c     #include "mapping.h"
c     void astUkern1( double offset, const double params[], int flags,
c                     double *value )
f     CALL AST_UKERN1( OFFSET, PARAMS, FLAGS, VALUE, STATUS )

*  Class Membership:
*     Mapping member function.

*  Description:
c     This is a fictitious function which does not actually
c     exist. Instead, this description constitutes a template so that
c     you may implement a function with this interface for yourself
c     (and give it any name you wish). A pointer to such a function
c     may be passed via the "finterp" parameter of the astResample<X>
c     functions (q.v.) in order to supply a 1-dimensional
c     interpolation kernel to the algorithm which performs sub-pixel
c     interpolation during resampling of gridded data (you must also
c     set the "interp" parameter of astResample<X> to the value
c     AST__UKERN1). This allows you to use your own interpolation
c     kernel in addition to those which are pre-defined.
f     This is a fictitious routine which does not actually
f     exist. Instead, this description constitutes a template so that
f     you may implement a routine with this interface for yourself
f     (and give it any name you wish). Such a routine
f     may be passed via the FINTERP argument of the AST_RESAMPLE<X>
f     functions (q.v.) in order to supply a 1-dimensional
f     interpolation kernel to the algorithm which performs sub-pixel
f     interpolation during resampling of gridded data (you must also
f     set the INTERP argument of AST_RESAMPLE<X> to the value
f     AST__UKERN1). This allows you to use your own interpolation
f     kernel in addition to those which are pre-defined.
*
c     The function calculates the value of a 1-dimensional sub-pixel
f     The routine calculates the value of a 1-dimensional sub-pixel
*     interpolation kernel. This determines how the weight given to
*     neighbouring pixels in calculating an interpolated value depends
*     on the pixel's offset from the interpolation point. In more than
*     one dimension, the weight assigned to a pixel is formed by
*     evaluating this 1-dimensional kernel using the offset along each
*     dimension in turn. The product of the returned values is then
*     used as the pixel weight.

*  Parameters:
c     offset
f     OFFSET = DOUBLE PRECISION (Given)
*        This will be the offset of the pixel from the interpolation
*        point, measured in pixels. This value may be positive or
*        negative, but for most practical interpolation schemes its
*        sign should be ignored.
c     params
f     PARAMS( * ) = DOUBLE PRECISION (Given)
c        This will be a pointer to the same array as was given via the
c        "params" parameter of astResample<X>. You may use this to
f        This will be the same array as was given via the
f        PARAMS argument of AST_RESAMPLE<X>. You may use this to
*        pass any additional parameter values required by your kernel,
c        but note that "params[0]" will already have been used to specify
f        but note that PARAMS(1) will already have been used to specify
*        the number of neighbouring pixels which contribute to the
*        interpolated value.
c     flags
f     FLAGS = INTEGER (Given)
c        This will be the same value as was given via the "flags"
c        parameter of astResample<X>. You may test this value to
f        This will be the same value as was given via the FLAGS
f        argument of AST_RESAMPLE<X>. You may test this value to
*        provide additional control over the operation of your
c        function. Note that the special flag values AST__URESAMP1, 2,
f        routine. Note that the special flag values AST__URESAMP1, 2,
*        3 & 4 are reserved for you to use for your own purposes and
*        will not clash with other pre-defined flag
c        values (see astResample<X>).
f        values (see AST_RESAMPLE<X>).
c     value
f     VALUE = DOUBLE PRECISION (Returned)
c        Pointer to a double to receive the calculated kernel value,
f        The calculated kernel value,
*        which may be positive or negative.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
*     - Not all functions make good interpolation kernels. In general,
*     acceptable kernels tend to be symmetrical about zero, to have a
*     positive peak (usually unity) at zero, and to evaluate to zero
*     whenever the pixel offset has any other integral value (this
*     ensures that the interpolated values pass through the original
*     data). An interpolation kernel may or may not have regions with
*     negative values. You should consult a good book on image
*     processing for more details.
c     - If an error occurs within this function, it should use
c     astSetStatus to set the AST error status to an error value.
c     This will cause an immediate return from astResample<X>. The error
c     value AST__UK1ER is available for this purpose, but other values may
c     also be used (e.g. if you wish to distinguish different types of
c     error).
f     - If an error occurs within this routine, it should set the
f     STATUS argument to an error value before returning. This will
f     cause an immediate return from AST_RESAMPLE<X>. The error value
f     AST__UK1ER is available for this purpose, but other values may also
f     be used (e.g. if you wish to distinguish different types of error).
f     The AST__UK1ER error value is defined in the AST_ERR include file.
*--
*/
/* Note the above is just a description to act as a template. The
   function does not actually exist. */

static double UphillSimplex( const MapData *mapdata, double acc, int maxcall,
                             const double dx[], double xmax[], double *err,
                             int *ncall ) {
/*
*  Name:
*     UphillSimplex

*  Purpose:
*     Find a function maximum using a modification of the simplex method.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     double UphillSimplex( const MapData *mapdata, double acc, int maxcall,
*                           const double dx[], double xmax[], double *err,
*                           int *ncall );

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function applies a modification of the simplex method to
*     find a local maximum in the value returned by a Mapping
*     function. The modification used allows the method to cope with
*     coordinate constraints and (equivalently) regions where the
*     function returns "bad" values. The method is robust and not
*     susceptible to overflow, so is suitable for applying to Mapping
*     functions of unknown form.

*  Parameters:
*     mapdata
*        Pointer to a MapData structure which describes the Mapping
*        function, its coordinate constraints, etc.
*     acc
*        The accuracy required in the value of the maximum.
*     maxcall
*        The maximum number of Mapping function evaluations to use.
*     dx
*        Pointer to an array of double containing an offset along each
*        input coordinate for the Mapping function supplied. These
*        offsets will be used to construct the initial simplex
*        (i.e. they are the initial "step lengths" for each
*        coordinate) and may be positive or negative.
*     xmax
*        Pointer to an array of double which contains the coordinates
*        of an initial estimate of the location of the maximum. On
*        exit, this will be updated to contain the best estimate of
*        the location of the maximum as generated by this function.
*     err
*        Pointer to a double in which to return an estimate of the
*        error in the value of the maximum found. For normal
*        convergence, this should be no larger than "acc". However, if
*        the maximum number of Mapping function evaluations is
*        reached, the returned value may be larger than this, although
*        it should still be valid. In such cases, re-starting the
*        algorithm at the new location returned in "xmax" may be
*        advisable.
*     ncall
*        Pointer to an int in which the number of Mapping function
*        evaluations will be returned.

*  Returned Value:
*     An estimate of the Mapping function value at the local maximum.

*  Notes:
*     - The function may return before the requested accuracy has been
*     met and before all Mapping function evaluations have been
*     made. This signifies that an excessive number of function values
*     have been needed outside the coordinate constraints. This is
*     only likely if the function is unable to make progress near such
*     a constraint, in which case the algorithm should probably be
*     re-started.
*     - A value of AST__BAD will be returned if no maximum could be
*     found.  This means that all the Mapping function evaluations
*     performed returned a value of AST__BAD.
*     - A value of AST__BAD will also be returned and no useful
*     information about a solution will be produced if this routine is
*     invoked with the global error status set, or if it should fail
*     for any reason.
*/

/* Local Constants: */
   const double factor = 3.0;    /* Simplex contraction/expansion factor */

/* Local Variables: */
   double *f;                    /* Pointer to array of function values */
   double *x;                    /* Pointer to array of vertex coordinates */
   double *xnew;                 /* Pointer to workspace array */
   double fnew;                  /* New function value */
   double fsave;                 /* Saved function value */
   double offset;                /* Coordinate difference between vertices */
   double range;                 /* Range of simplex values */
   double result;                /* Value to return */
   double tmp;                   /* Temporary store for coordinate */
   int coord;                    /* Loop counter for coordinates */
   int hi;                       /* Index of best vertex */
   int lo;                       /* Index of worst vertex */
   int ncalla;                   /* Number of function calls attempted */
   int ncoord;                   /* Number of function dimensions */
   int nextlo;                   /* Index of second worst vertex */
   int nvertex;                  /* Number of simplex vertices */
   int vertex;                   /* Loop counter for vertices */

/* Initialise. */
   result = AST__BAD;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Further initialisation. */
   *err = DBL_MAX;
   *ncall = 0;

/* Obtain the number of input coordinates for the Mapping function and
   calculate the number of simplex vertices. */
   ncoord = mapdata->nin;
   nvertex = ncoord + 1;

/* Allocate workspace. */
   f = astMalloc( sizeof( double ) * (size_t) nvertex );
   x = astMalloc( sizeof( double ) * (size_t) ( ncoord * nvertex ) );
   xnew = astMalloc( sizeof( double ) * (size_t) ncoord );
   if ( astOK ) {

/* Loop to set up an initial simplex. */
      for ( vertex = 0; vertex < nvertex; vertex++ ) {
         for ( coord = 0; coord < ncoord; coord++ ) {
            tmp = xmax[ coord ];

/* Displace each point (except the first) the required amount along
   one of the axes to generate the coordinates of the simplex
   vertices. */
            if ( coord == ( vertex - 1 ) ) tmp += dx[ coord ];
            x[ vertex * ncoord + coord ] = tmp;
         }

/* Evaluate the Mapping function at each vertex. */
         f[ vertex ] = MapFunction( mapdata, &x[ vertex * ncoord ], ncall );
         if ( f[ vertex ] == AST__BAD ) f[ vertex ] = -DBL_MAX;
      }

/* Initialise the number of times we attempt to call the Mapping
   function (not necessarily the same as the number of times it was
   actually called, which is stored in *ncall). */
      ncalla = nvertex;

/* Loop until convergence is reached or an error occurs. */
      while( astOK ) {

/* Initialise the index of the lowest vertex of the simplex, the next
   lowest vertex and the highest vertex. */
         lo = ( f[ 0 ] < f[ 1 ] ) ? 0 : 1;
         nextlo = 1 - lo;
         hi = 0;

/* Loop to inspect each vertex and update these values. Ensure that in
   the case of equal vertices, the first one is taken to be the
   highest. This makes the maximisation stable (so that if no better
   maximum can be found, the original position is returned rather than
   a nearby position that yields the same function value). */
         for ( vertex = 0; vertex < nvertex; vertex++ ) {
            if ( f[ vertex ] <= f[ lo ] ) {
               nextlo = lo;
               lo = vertex;
            } else if ( ( f[ vertex ] <= f[ nextlo ] ) && ( vertex != lo ) ) {
               nextlo = vertex;
            }
            if ( f[ vertex ] > f[ hi ] ) hi = vertex;
         }

/* Estimate the error on the result as the difference between the
   highest and lowest simplex vertices. */
         if ( ( f[ hi ] == -DBL_MAX ) || ( f[ lo ] == -DBL_MAX ) ) {
            range = DBL_MAX;
         } else {
            range = f[ hi ] - f[ lo ];
         }

/* Test for convergence. Ideally, the accuracy criterion should have
   been met. However, also quit if the maximum number of Mapping
   function evaluations has been reached, or the number of points at
   which function values have been requested reaches three times this
   limit (this latter number will typically be larger because points
   lying outside the coordinate constraints do not result in the
   Mapping function being evaluated). */
         if ( range <= fabs( acc ) ||
              ( *ncall >= maxcall ) || ( ncalla >= ( 3 * maxcall ) ) ) {

/* If quitting, return the coordinates and function value at the best
   simplex vertex, and the error estimate. */
            for ( coord = 0; coord < ncoord; coord++ ) {
               xmax[ coord ] = x[ hi * ncoord + coord ];
            }
            result = ( f[ hi ] == -DBL_MAX ) ? AST__BAD : f[ hi ];
            *err = range;
            break;
         }

/* If performing another iteration, first try reflecting the worst
   vertex through the opposite face of the simplex. Check for
   errors. */
         fnew = NewVertex( mapdata, lo, -1.0, x, f, ncall, xnew );
         ncalla++;
         if ( astOK ) {

/* If this results in a point lying in a forbiddden region (either
   outside the coordinate constraints or where the Mapping function
   yields bad coordinate values), then we must make a departure from
   the standard simplex algorithm. This is because the inability to
   make forward progress in this case can cause the simplex to
   repeatedly contract about each face (except one) in turn. This
   mechanism normally results in lateral contraction as the simplex
   attempts to squeeze through a narrow gap which is impeding
   progress. However, in this case there is no gap to get through, so
   the lateral contraction can eventually make the simplex become
   degenerate (due to rounding). This prevents it from expanding
   laterally again and exploring the region adjacent to the constraint
   boundary once it has become small enough. */
            if ( fnew == AST__BAD ) {

/* To overcome this, we instead contract the worst simplex vertex
   towards the best vertex (this has the cumulative effect of
   contracting the simplex without changing its shape). First find the
   offset in each coordinate between these two vertices. */
               for ( coord = 0; coord < ncoord; coord++ ) {
                  offset = x[ lo * ncoord + coord ] - x[ hi * ncoord + coord ];

/* Scale the offset to obtain the new coordinate. */
                  x[ lo * ncoord + coord ] = x[ hi * ncoord + coord ] +
                                             offset / factor;

/* If the distance between the two vertices has not decreased, we are
   in a region where rounding errors prevent them approaching each
   other any more closely, so simply set them equal. */
                  if ( fabs( x[ lo * ncoord + coord ] -
                             x[ hi * ncoord + coord ] ) >= fabs( offset ) ) {
                     x[ lo * ncoord + coord ] = x[ hi * ncoord + coord ];
                  }
               }

/* Evaluate the Mapping function at the new vertex. */
               f[ lo ] = MapFunction( mapdata, &x[ lo * ncoord ], ncall );
               if ( f[ lo ] == AST__BAD ) f[ lo ] = -DBL_MAX;
               ncalla++;

/* We now return to the standard simplex algorithm. If the new vertex
   is a new maximum, then see if more of the same is even better by
   trying to expand the best vertex away from the opposite face. */
            } else if ( fnew >= f[ hi ] ) {
               fnew = NewVertex( mapdata, lo, factor, x, f, ncall, xnew );
               ncalla++;

/* Otherwise, if the new vertex was no improvement on the second
   worst, then try contracting the worst vertex towards the opposite
   face. */
            } else if ( fnew <= f[ nextlo ] ) {
               fsave = f[ lo ];
               fnew = NewVertex( mapdata, lo, 1.0 / factor, x, f, ncall, xnew );
               ncalla++;

/* If this didn't result in any improvement, then contract the entire
   simplex towards the best vertex. Use the same approach as earlier
   to protect against rounding so that all the simplex vertices will
   eventually coalesce if this process is repeated enough times. */
               if ( astOK && ( fnew <= fsave ) ) {
                  for ( vertex = 0; vertex < nvertex; vertex++ ) {
                     if ( vertex != hi ) {
                        for ( coord = 0; coord < ncoord; coord++ ) {
                           offset = x[ vertex * ncoord + coord ] -
                                    x[ hi * ncoord + coord ];
                           x[ vertex * ncoord + coord ] =
                               x[ hi * ncoord + coord ] + offset / factor;
                           if ( fabs( x[ vertex * ncoord + coord ] -
                                      x[ hi * ncoord + coord ] ) >=
                                fabs( offset ) ) {
                              x[ vertex * ncoord + coord ] =
                                 x[ hi * ncoord + coord ];
                           }
                        }

/* Evaluate the Mapping function at each new vertex. */
                        f[ vertex ] = MapFunction( mapdata,
                                                   &x[ vertex * ncoord ],
                                                   ncall );
                        if ( f[ vertex ] == AST__BAD ) f[ vertex ] = -DBL_MAX;
                        ncalla++;
                     }
                  }
               }
            }
         }
      }
   }

/* Free workspace. */
   f = astFree( f );
   x = astFree( x );
   xnew = astFree( xnew );

/* If an error occurred, clear the returned result. */
   if ( !astOK ) result = AST__BAD;

/* Return the result. */
   return result;
}

static void ValidateMapping( AstMapping *this, int forward,
                             int npoint, int ncoord_in, int ncoord_out,
                             const char *method ) {
/*
*  Name:
*     ValidateMapping

*  Purpose:
*     Validate a Mapping for use to transform coordinates.

*  Type:
*     Private function.

*  Synopsis:
*     #include "mapping.h"
*     void ValidateMapping( AstMapping *this, int forward,
*                           int npoint, int ncoord_in, int ncoord_out,
*                           const char *method )

*  Class Membership:
*     Mapping member function.

*  Description:
*     This function checks that a Mapping is suitable for transforming
*     a set of points. It also checks that the number of points and
*     the number of coordinate values per point is valid. If an error
*     is detected, the global error status is set and an error report
*     made. Otherwise, the function returns without further action.

*  Parameters:
*     this
*        Pointer to the Mapping.
*     forward
*        A non-zero value indicates that the forward coordinate
*        transformation is to be checked, while a zero value requests
*        the inverse transformation.
*     npoint
*        The number of points being transformed.
*     ncoord_in
*        The number of coordinates associated with each input point.
*     ncoord_out
*        The number of coordinates associated with each output point.
*     method
*        Pointer to a null terminated character string containing the
*        name of the method which invoked this function to validate a
*        Mapping. This is used solely for constructing error messages.
*/

/* Local Variables: */
   int nin;                    /* Mapping Nin attribute value */
   int nout;                   /* Mapping Nout attribute value */

/* Check the global error status. */
   if ( !astOK ) return;

/* Report an error if the requested transformation is not defined. */
   if ( !( forward ? astGetTranForward( this ) : astGetTranInverse( this ) )
        && astOK ) {
      astError( AST__TRNND, "%s(%s): %s coordinate transformation "
                "is not defined by the %s supplied.", method,
                astGetClass( this ),
                ( forward ? "A forward" : "An inverse" ),
                astGetClass( this ) );
   }

/* Obtain the effective values of the Nin and Nout attributes for the
   Mapping. */
   nin = forward ? astGetNin( this ) : astGetNout( this );
   nout = forward ? astGetNout( this ) : astGetNin( this );

/* If OK, check that the number of input coordinates matches the
   number required by the Mapping. Report an error if these numbers do
   not match. */
   if ( astOK && ( ncoord_in != nin ) ) {
      astError( AST__NCPIN, "%s(%s): Bad number of input coordinate values "
                "(%d).", method, astGetClass( this ), ncoord_in );
      astError( AST__NCPIN, "The %s given requires %d coordinate value%s for "
                "each input point.", astGetClass( this ), nin,
                ( nin == 1 ) ? "" : "s" );
   }

/* If OK, also check that the number of output coordinates matches the
   number required by the Mapping. Report an error if these numbers do
   not match. */
   if ( astOK && ( ncoord_out != nout ) ) {
      astError( AST__NCPIN, "%s(%s): Bad number of output coordinate values "
                "(%d).", method, astGetClass( this ), ncoord_out );
      astError( AST__NCPIN, "The %s given generates %s%d coordinate value%s "
                "for each output point.", astGetClass( this ),
                ( nout < ncoord_out ) ? "only " : "", nout,
                ( nout == 1 ) ? "" : "s" );
   }

/* Check that the number of points being transformed is not negative
   and report an error if necessary. */
   if ( astOK && ( npoint < 0 ) ) {
      astError( AST__NPTIN, "%s(%s): Number of points to be transformed (%d) "
                "is invalid.", method, astGetClass( this ), npoint );
   }
}

/* Functions which access class attributes. */
/* ---------------------------------------- */
/* Implement member functions to access the attributes associated with
   this class using the macros defined for this purpose in the
   "object.h" file. */
/*
*att++
*  Name:
*     Invert

*  Purpose:
*     Mapping inversion flag.

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean).

*  Description:
*     This attribute controls which one of a Mapping's two possible
*     coordinate transformations is considered the "forward"
*     transformation (the other being the "inverse"
*     transformation). If the attribute value is zero (the default),
*     the Mapping's behaviour will be the same as when it was first
*     created. However, if it is non-zero, its two transformations
*     will be inter-changed, so that the Mapping displays the inverse
*     of its original behaviour.
*
*     Inverting the boolean sense of the Invert attribute will cause
*     the values of a Mapping's Nin and Nout attributes to be
*     interchanged. The values of its TranForward and TranInverse
*     attributes will also be interchanged. This operation may be
c     performed with the astInvert function.
f     performed with the AST_INVERT routine.

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     UnitMap
*        The value of the Invert attribute has no effect on the
*        behaviour of a UnitMap.
*     FrameSet
*        Inverting the boolean sense of the Invert attribute for a
*        FrameSet will cause its base and current Frames (and its Base
*        and Current attributes) to be interchanged. This, in turn,
*        may affect other properties and attributes of the FrameSet
*        (such as Nin, Nout, Naxes, TranForward, TranInverse,
*        etc.). The Invert attribute of a FrameSet is not itself
*        affected by selecting a new base or current Frame.
*att--
*/
/* This ia a boolean value (0 or 1) with a value of CHAR_MAX when
   undefined but yielding a default of zero. */
astMAKE_CLEAR(Mapping,Invert,invert,CHAR_MAX)
astMAKE_GET(Mapping,Invert,int,0,( ( this->invert == CHAR_MAX ) ?
                                   0 : this->invert ))
astMAKE_SET(Mapping,Invert,int,invert,( this->issimple=0,(value!=0) ))
astMAKE_TEST(Mapping,Invert,( this->invert != CHAR_MAX ))

/*
*att++
*  Name:
*     IsSimple

*  Purpose:
*     Has the Mapping been simplified?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean), read-only.

*  Description:
*     This attribute indicates whether a Mapping has been simplified
*     by the 
c     astSimplify
f     AST_SIMPLIFY
*     method. If the IsSimple value is non-zero, then the Mapping has
*     been simplified and so there is nothing to be gained by simplifying
*     it again. Indeed, the
c     astSimplify
f     AST_SIMPLIFY
*     method will immediately return the Mapping unchanged if the IsSimple 
*     attribute indicates that the Mapping has already been simplified.

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     Frame
*        All classes of Frame return zero for the IsSimple attribute.
*        This is because changes can be made to a Frame which affect the
*        Mapping represented by the Frame, and so there can be no
*        guarantee that the Mapping may not need re-simplifying. Most
*        non-Frame Mappings, on the other hand, are immutable and so when
*        they are simplified it is certain that they weill remain in a
*        simple state.

*att--
*/
astMAKE_GET(Mapping,IsSimple,int,0,this->issimple)

/*
*att++
*  Name:
*     Nin

*  Purpose:
*     Number of input coordinates for a Mapping.

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer, read-only.

*  Description:
*     This attribute gives the number of coordinate values required to
*     specify an input point for a Mapping (i.e. the number of
*     dimensions of the space in which the Mapping's input points
*     reside).

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     CmpMap
*        If a CmpMap's component Mappings are joined in series, then
*        its Nin attribute is equal to the Nin attribute of the first
*        component (or to the Nout attribute of the second component
*        if the the CmpMap's Invert attribute is non-zero).
*
*        If a CmpMap's component Mappings are joined in parallel, then
*        its Nin attribute is given by the sum of the Nin attributes
*        of each component (or to the sum of their Nout attributes if
*        the CmpMap's Invert attribute is non-zero).
*     Frame
*        The Nin attribute for a Frame is always equal to the number
*        of Frame axes (Naxes attribute).
*     FrameSet
*        The Nin attribute of a FrameSet is equal to the number of
*        axes (Naxes attribute) of its base Frame (as specified by the
*        FrameSet's Base attribute). The Nin attribute value may
*        therefore change if a new base Frame is selected.
*att-- 
*/

/*
*att++
*  Name:
*     Nout

*  Purpose:
*     Number of output coordinates for a Mapping.

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer, read-only.

*  Description:
*     This attribute gives the number of coordinate values generated
*     by a Mapping to specify each output point (i.e. the number of
*     dimensions of the space in which the Mapping's output points
*     reside).

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     CmpMap
*        If a CmpMap's component Mappings are joined in series, then
*        its Nout attribute is equal to the Nout attribute of the
*        second component (or to the Nin attribute of the first
*        component if the the CmpMap's Invert attribute is non-zero).
*
*        If a CmpMap's component Mappings are joined in parallel, then
*        its Nout attribute is given by the sum of the Nout attributes
*        of each component (or to the sum of their Nin attributes if
*        the CmpMap's Invert attribute is non-zero).
*     Frame
*        The Nout attribute for a Frame is always equal to the number
*        of Frame axes (Naxes attribute).
*     FrameSet
*        The Nout attribute of a FrameSet is equal to the number of
*        FrameSet axes (Naxes attribute) which, in turn, is equal to
*        the Naxes attribute of the FrameSet's current Frame (as
*        specified by the Current attribute). The Nout attribute value
*        may therefore change if a new current Frame is selected.
*att--
*/

/*
*att++
*  Name:
*     Report

*  Purpose:
*     Report transformed coordinates?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean).

*  Description:
*     This attribute controls whether coordinate values are reported
*     whenever a Mapping is used to transform a set of points. If its
*     value is zero (the default), no report is made. However, if it
*     is non-zero, the coordinates of each point are reported (both
*     before and after transformation) by writing them to standard
*     output.
*
*     This attribute is provided as an aid to debugging, and to avoid
*     having to report values explicitly in simple programs.

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     CmpMap
*        When applied to a compound Mapping (CmpMap), only the Report
*        attribute of the CmpMap, and not those of its component
*        Mappings, is used.  Coordinate information is never reported
*        for the component Mappings individually, only for the
*        complete CmpMap.
*     Frame
*        When applied to any Frame, the formatting capabilities of the
c        Frame (as provided by the astFormat function) will be used to
f        Frame (as provided by the AST_FORMAT function) will be used to
*        format the reported coordinates.
*     FrameSet
*        When applied to any FrameSet, the formatting capabilities of
*        the base and current Frames will be used (as above) to
*        individually format the input and output coordinates, as
*        appropriate. The Report attribute of a FrameSet is not itself
*        affected by selecting a new base or current Frame, but the
*        resulting formatting capabilities may be.

*  Notes:
*     - Unlike most other attributes, the value of the Report
*     attribute is not transferred when a Mapping is copied. Instead,
*     its value is undefined (and therefore defaults to zero) in any
*     copy. Similarly, it becomes undefined in any external
c     representation of a Mapping produced by the astWrite function.
f     representation of a Mapping produced by the AST_WRITE routine.
*att--
*/
/* This ia a boolean value (0 or 1) with a value of CHAR_MAX when
   undefined but yielding a default of zero. */
astMAKE_CLEAR(Mapping,Report,report,CHAR_MAX)
astMAKE_GET(Mapping,Report,int,0,( ( this->report == CHAR_MAX ) ?
                                   0 : this->report ))
astMAKE_SET(Mapping,Report,int,report,( value != 0 ))
astMAKE_TEST(Mapping,Report,( this->report != CHAR_MAX ))

/*
*att++
*  Name:
*     TranForward

*  Purpose:
*     Forward transformation defined?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean), read-only.

*  Description:
*     This attribute indicates whether a Mapping is able to transform
*     coordinates in the "forward" direction (i.e. converting input
*     coordinates into output coordinates). If this attribute is
*     non-zero, the forward transformation is available. Otherwise, it
*     is not.

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     CmpMap
*        The TranForward attribute value for a CmpMap is given by the
*        boolean AND of the value for each component Mapping.
*     FrameSet
*        The TranForward attribute of a FrameSet applies to the
*        transformation which converts between the FrameSet's base
*        Frame and its current Frame (as specified by the Base and
*        Current attributes). This value is given by the boolean AND
*        of the TranForward values which apply to each of the
*        individual sub-Mappings required to perform this conversion.
*        The TranForward attribute value for a FrameSet may therefore
*        change if a new Base or Current Frame is selected.

*  Notes:
*     - An error will result if a Mapping with a TranForward value of
*     zero is used to transform coordinates in the forward direction.
*att--
*/

/*
*att++
*  Name:
*     TranInverse

*  Purpose:
*     Inverse transformation defined?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean), readonly.

*  Description:
*     This attribute indicates whether a Mapping is able to transform
*     coordinates in the "inverse" direction (i.e. converting output
*     coordinates back into input coordinates). If this attribute is
*     non-zero, the inverse transformation is available. Otherwise, it
*     is not.

*  Applicability:
*     Mapping
*        All Mappings have this attribute.
*     CmpMap
*        The TranInverse attribute value for a CmpMap is given by the
*        boolean AND of the value for each component Mapping.
*     FrameSet
*        The TranInverse attribute of a FrameSet applies to the
*        transformation which converts between the FrameSet's current
*        Frame and its base Frame (as specified by the Current and
*        Base attributes). This value is given by the boolean AND of
*        the TranInverse values which apply to each of the individual
*        sub-Mappings required to perform this conversion.
*        The TranInverse attribute value for a FrameSet may therefore
*        change if a new Base or Current Frame is selected.

*  Notes:
*     - An error will result if a Mapping with a TranInverse value of
*     zero is used to transform coordinates in the inverse direction.
*att--
*/

/* Copy constructor. */
/* ----------------- */
static void Copy( const AstObject *objin, AstObject *objout ) {
/*
*  Name:
*     Copy

*  Purpose:
*     Copy constructor for Mapping objects.

*  Type:
*     Private function.

*  Synopsis:
*     void Copy( const AstObject *objin, AstObject *objout )

*  Description:
*     This function implements the copy constructor for Mapping objects.

*  Parameters:
*     objin
*        Pointer to the Mapping to be copied.
*     objout
*        Pointer to the Mapping being constructed.

*  Notes:
*     - This constructor exists simply to ensure that the "Report"
*     attribute is cleared in any copy made of a Mapping.
*/

/* Local Variables: */
   AstMapping *out;              /* Pointer to output Mapping */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the output Mapping. */
   out = (AstMapping *) objout;

/* Clear the output Report attribute. */
   out->report = CHAR_MAX;
}

/* Destructor. */
/* ----------- */
static void Delete( AstObject *obj ) {
/*
*  Name:
*     Delete

*  Purpose:
*     Destructor for Mapping objects.

*  Type:
*     Private function.

*  Synopsis:
*     void Delete( AstObject *obj )

*  Description:
*     This function implements the destructor for Mapping objects.

*  Parameters:
*     obj
*        Pointer to the Mapping to be deleted.

*  Notes:
*     - This destructor does nothing and exists only to maintain a
*     one-to-one correspondence between destructors and copy
*     constructors.
*/

/* Return without action. */
}

/* Dump function. */
/* -------------- */
static void Dump( AstObject *this_object, AstChannel *channel ) {
/*
*  Name:
*     Dump

*  Purpose:
*     Dump function for Mapping objects.

*  Type:
*     Private function.

*  Synopsis:
*     void Dump( AstObject *this, AstChannel *channel )

*  Description:
*     This function implements the Dump function which writes out data
*     for the Mapping class to an output Channel.

*  Parameters:
*     this
*        Pointer to the Mapping whose data are being written.
*     channel
*        Pointer to the Channel to which the data are being written.
*/

/* Local Variables: */
   AstMapping *this;             /* Pointer to the Mapping structure */
   int invert;                   /* Mapping inverted? */
   int ival;                     /* Integer value */
   int set;                      /* Attribute value set? */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the Mapping structure. */
   this = (AstMapping *) this_object;

/* Write out values representing the instance variables for the
   Mapping class.  Accompany these with appropriate comment strings,
   possibly depending on the values being written.*/

/* In the case of attributes, we first use the appropriate (private)
   Test...  member function to see if they are set. If so, we then use
   the (private) Get... function to obtain the value to be written
   out.

   For attributes which are not set, we use the astGet... method to
   obtain the value instead. This will supply a default value
   (possibly provided by a derived class which over-rides this method)
   which is more useful to a human reader as it corresponds to the
   actual default attribute value.  Since "set" will be zero, these
   values are for information only and will not be read back. */

/* Determine if the Mapping is inverted. The output values
   (e.g. number of input and output coordinates) will refer to the
   Mapping ***before*** this inversion flag is applied, but we need it
   when using (e.g.) the astGetNin/astGetNout methods to determine
   which one will return the required value. */
   invert = astGetInvert( this );

/* (NB. there is a subtle point here that dictates the extent to which
   this inversion flag can be used... All use of methods (such as
   astGetInvert, which might be over-ridden by derived classes) must
   be restricted to determining the values of "unset" output
   quantities only (below). This is because when re-loading the
   Mapping, the derived classes will not have been loaded at the point
   when these values are re-read - hence any value whose
   interpretation depends on these methods cannot be reliably
   recovered.) */

/* Nin. */
/* ---- */
/* Use the instance variable directly to avoid the effect of the
   Invert attribute on the private member function. Treat zero as the
   default. */
   set = ( this->nin != 0 );
   ival = set ? this->nin : ( !invert ? astGetNin( this ) :
                                        astGetNout( this ) );
   astWriteInt( channel, "Nin", set, 0, ival,
                "Number of input coordinates" );

/* Nout. */
/* ----- */
/* Use the instance variable directly. Treat zero as the default. */
   set = ( this->nout != this->nin );
   ival = set ? this->nout : ( !invert ? astGetNout( this ) :
                                         astGetNin( this ) );
   astWriteInt( channel, "Nout", set, 0, ival,
                "Number of output coordinates" );

/* IsSimple. */
/* --------- */
   ival = astGetIsSimple( this );
   astWriteInt( channel, "IsSimp", ival, 0, ival,
                ival ? "Mapping has been simplified" : 
                       "Mapping has not been simplified" );

/* Invert. */
/* ------- */
   set = TestInvert( this );
   ival = set ? GetInvert( this ) : astGetInvert( this );
   astWriteInt( channel, "Invert", set, 0, ival,
                ival ? "Mapping inverted" :
                       "Mapping not inverted" );

/* TranForward. */
/* ------------ */
/* Use the instance variable directly. Treat 1 as the default. */
   set = ( this->tran_forward == 0 );
   ival = set ? this->tran_forward : ( !invert ? astGetTranForward( this ) :
                                                 astGetTranInverse( this ) );
   astWriteInt( channel, "Fwd", set, 0, ival,
                ival ? "Forward transformation defined" :
                       "Forward transformation not defined" );

/* TranInverse. */
/* ------------ */
/* Use the instance variable directly. Treat 1 as the default. */
   set = ( this->tran_inverse == 0 );
   ival = set ? this->tran_inverse : ( !invert ? astGetTranInverse( this ) :
                                                 astGetTranForward( this ) );
   astWriteInt( channel, "Inv", set, 0, ival,
                ival ? "Inverse transformation defined" :
                       "Inverse transformation not defined" );

/* Report. */
/* ------- */
   set = TestReport( this );
   ival = set ? GetReport( this ) : astGetReport( this );
   astWriteInt( channel, "Report", set, 0, ival,
                ival ? "Report coordinate transformations" :
                       "Don't report coordinate transformations" );
}

/* Standard class functions. */
/* ========================= */
/* Implement the astIsAMapping and astCheckMapping functions using the macros
   defined for this purpose in the "object.h" header file. */
astMAKE_ISA(Mapping,Object,check,&class_init)
astMAKE_CHECK(Mapping)

AstMapping *astInitMapping_( void *mem, size_t size, int init,
                             AstMappingVtab *vtab, const char *name,
                             int nin, int nout,
                             int tran_forward, int tran_inverse ) {
/*
*+
*  Name:
*     astInitMapping

*  Purpose:
*     Initialise a Mapping.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "mapping.h"
*     AstMapping *astInitMapping( void *mem, size_t size, int init,
*                                 AstMappingVtab *vtab, const char *name,
*                                 int nin, int nout,
*                                 int tran_forward, int tran_inverse )

*  Class Membership:
*     Mapping initialiser.

*  Description:
*     This function is provided for use by class implementations to initialise
*     a new Mapping object. It allocates memory (if necessary) to accommodate
*     the Mapping plus any additional data associated with the derived class.
*     It then initialises a Mapping structure at the start of this memory. If
*     the "init" flag is set, it also initialises the contents of a virtual
*     function table for a Mapping at the start of the memory passed via the
*     "vtab" parameter.

*  Parameters:
*     mem
*        A pointer to the memory in which the Mapping is to be initialised.
*        This must be of sufficient size to accommodate the Mapping data
*        (sizeof(Mapping)) plus any data used by the derived class. If a value
*        of NULL is given, this function will allocate the memory itself using
*        the "size" parameter to determine its size.
*     size
*        The amount of memory used by the Mapping (plus derived class data).
*        This will be used to allocate memory if a value of NULL is given for
*        the "mem" parameter. This value is also stored in the Mapping
*        structure, so a valid value must be supplied even if not required for
*        allocating memory.
*     init
*        A logical flag indicating if the Mapping's virtual function table is
*        to be initialised. If this value is non-zero, the virtual function
*        table will be initialised by this function.
*     vtab
*        Pointer to the start of the virtual function table to be associated
*        with the new Mapping.
*     name
*        Pointer to a constant null-terminated character string which contains
*        the name of the class to which the new object belongs (it is this
*        pointer value that will subsequently be returned by the astGetClass
*        method).
*     nin
*        The number of coordinate values per input point.
*     nout
*        The number of coordinate vales per output point.
*     tran_forward
*        A non-zero value indicates that the Mapping will be able to
*        transform coordinates in the forward direction. A zero value
*        indicates that it will not.
*     tran_inverse
*        A non-zero value indicates that the Mapping will be able to
*        transform coordinates in the inverse direction. A zero value
*        indicates that it will not.

*  Returned Value:
*     A pointer to the new Mapping.

*  Notes:
*     -  The Mappings produced by this function implement all the basic methods
*     defined by the Mapping class. However, their astTransform method does not
*     actually perform any coordinate transformation (although it performs all
*     necessary argument validation and creates an output PointSet if
*     necessary, leaving its coordinate values undefined).
*     -  This means that Mappings produced by this function are of limited use
*     on their own, but may easily be extended by a derived class simply by
*     over-riding the astTransform method to add the necessary coordinate
*     arithmetic.
*     -  A null pointer will be returned if this function is invoked with the
*     global error status set, or if it should fail for any reason.
*-
*/

/* Local Variables: */
   AstMapping *new;              /* Pointer to new Mapping */

/* Check the global status. */
   if ( !astOK ) return NULL;

/* If necessary, initialise the virtual function table. */
   if ( init ) astInitMappingVtab( vtab, name );

/* Initialise. */
   new = NULL;

/* Check the initialisation values for validity, reporting an error if
   necessary. */
   if ( nin < 0 ) {
      astError( AST__BADNI, "astInitMapping(%s): Bad number of input "
                "coordinates (%d).", name, nin );
      astError( AST__BADNI, "This number should be zero or more." );
   } else if ( nout < 0 ) {
      astError( AST__BADNO, "astInitMapping(%s): Bad number of output "
                "coordinates (%d).", name, nout );
      astError( AST__BADNI, "This number should be zero or more." );
   }

/* Initialise an Object structure (the parent class) as the first component
   within the Mapping structure, allocating memory if necessary. */
   new = (AstMapping *) astInitObject( mem, size, 0,
                                       (AstObjectVtab *) vtab, name );

   if ( astOK ) {

/* Initialise the Mapping data. */
/* ---------------------------- */
/* Store the numbers of input and output coordinates. */
      new->nin = nin;
      new->nout = nout;

/* Store the flags indicating which coordinate transformations are
   defined (constrain these values to 0 or 1). */
      new->tran_forward = ( tran_forward != 0 );
      new->tran_inverse = ( tran_inverse != 0 );

/* Initialise other attributes to their undefined values. */
      new->invert = CHAR_MAX;
      new->report = CHAR_MAX;
      new->issimple = 0;

/* If an error occurred, clean up by deleting the new object. */
      if ( !astOK ) new = astDelete( new );
   }

/* Return a pointer to the new object. */
   return new;
}

AstMapping *astLoadMapping_( void *mem, size_t size,
                             AstMappingVtab *vtab, const char *name,
                             AstChannel *channel ) {
/*
*+
*  Name:
*     astLoadMapping

*  Purpose:
*     Load a Mapping.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "mapping.h"
*     AstMapping *astLoadMapping( void *mem, size_t size,
*                                 AstMappingVtab *vtab, const char *name,
*                                 AstChannel *channel )

*  Class Membership:
*     Mapping loader.

*  Description:
*     This function is provided to load a new Mapping using data read
*     from a Channel. It first loads the data used by the parent class
*     (which allocates memory if necessary) and then initialises a
*     Mapping structure in this memory, using data read from the input
*     Channel.
*
*     If the "init" flag is set, it also initialises the contents of a
*     virtual function table for a Mapping at the start of the memory
*     passed via the "vtab" parameter.


*  Parameters:
*     mem
*        A pointer to the memory into which the Mapping is to be
*        loaded.  This must be of sufficient size to accommodate the
*        Mapping data (sizeof(Mapping)) plus any data used by derived
*        classes. If a value of NULL is given, this function will
*        allocate the memory itself using the "size" parameter to
*        determine its size.
*     size
*        The amount of memory used by the Mapping (plus derived class
*        data).  This will be used to allocate memory if a value of
*        NULL is given for the "mem" parameter. This value is also
*        stored in the Mapping structure, so a valid value must be
*        supplied even if not required for allocating memory.
*
*        If the "vtab" parameter is NULL, the "size" value is ignored
*        and sizeof(AstMapping) is used instead.
*     vtab
*        Pointer to the start of the virtual function table to be
*        associated with the new Mapping. If this is NULL, a pointer
*        to the (static) virtual function table for the Mapping class
*        is used instead.
*     name
*        Pointer to a constant null-terminated character string which
*        contains the name of the class to which the new object
*        belongs (it is this pointer value that will subsequently be
*        returned by the astGetClass method).
*
*        If the "vtab" parameter is NULL, the "name" value is ignored
*        and a pointer to the string "Mapping" is used instead.

*  Returned Value:
*     A pointer to the new Mapping.

*  Notes:
*     - A null pointer will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   AstMapping *new;              /* Pointer to the new Mapping */

/* Initialise. */
   new = NULL;

/* Check the global error status. */
   if ( !astOK ) return new;

/* If a NULL virtual function table has been supplied, then this is
   the first loader to be invoked for this Mapping. In this case the
   Mapping belongs to this class, so supply appropriate values to be
   passed to the parent class loader (and its parent, etc.). */
   if ( !vtab ) {
      size = sizeof( AstMapping );
      vtab = &class_vtab;
      name = "Mapping";

/* If required, initialise the virtual function table for this class. */
      if ( !class_init ) {
         astInitMappingVtab( vtab, name );
         class_init = 1;
      }
   }

/* Invoke the parent class loader to load data for all the ancestral
   classes of the current one, returning a pointer to the resulting
   partly-built Mapping. */
   new = astLoadObject( mem, size, (AstObjectVtab *) vtab, name,
                        channel );

   if ( astOK ) {

/* Read input data. */
/* ================ */
/* Request the input Channel to read all the input data appropriate to
   this class into the internal "values list". */
      astReadClassData( channel, "Mapping" );

/* Now read each individual data item from this list and use it to
   initialise the appropriate instance variable(s) for this class. */

/* In the case of attributes, we first read the "raw" input value,
   supplying the "unset" value as the default. If a "set" value is
   obtained, we then use the appropriate (private) Set... member
   function to validate and set the value properly. */

/* Nin. */
/* ---- */
      new->nin = astReadInt( channel, "nin", 0 );
      if ( new->nin < 0 ) new->nin = 0;

/* Nout. */
/* ----- */
      new->nout = astReadInt( channel, "nout", new->nin );
      if ( new->nout < 0 ) new->nout = 0;

/* Invert. */
/* ------- */
      new->invert = astReadInt( channel, "invert", CHAR_MAX );
      if ( TestInvert( new ) ) SetInvert( new, new->invert );

/* IsSimple. */
/* --------- */
      new->issimple = astReadInt( channel, "issimp", 0 );

/* TranForward. */
/* ------------ */
      new->tran_forward = ( astReadInt( channel, "fwd", 1 ) != 0 );

/* TranInverse. */
/* ------------ */
      new->tran_inverse = ( astReadInt( channel, "inv", 1 ) != 0 );

/* Report. */
/* ------- */
      new->report = astReadInt( channel, "report", CHAR_MAX );
      if ( TestReport( new ) ) SetReport( new, new->report );

/* If an error occurred, clean up by deleting the new Mapping. */
      if ( !astOK ) new = astDelete( new );
   }

/* Return the new Mapping pointer. */
   return new;
}

/* Virtual function interfaces. */
/* ============================ */
/* These provide the external interface to the virtual functions
   defined by this class. Each simply checks the global error status
   and then locates and executes the appropriate member function,
   using the function pointer stored in the object's virtual function
   table (this pointer is located using the astMEMBER macro defined in
   "object.h").

   Note that the member function may not be the one defined here, as
   it may have been over-ridden by a derived class. However, it should
   still have the same interface. */

void astDecompose_( AstMapping *this, AstMapping **map1, AstMapping **map2, 
                    int *series, int *invert1, int *invert2 ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,Decompose))( this, map1, map2, series, invert1, invert2 );
}
int astGetNin_( AstMapping *this ) {
   if ( !astOK ) return 0;
   return (**astMEMBER(this,Mapping,GetNin))( this );
}
int astGetNout_( AstMapping *this ) {
   if ( !astOK ) return 0;
   return (**astMEMBER(this,Mapping,GetNout))( this );
}
int astGetTranForward_( AstMapping *this ) {
   if ( !astOK ) return 0;
   return (**astMEMBER(this,Mapping,GetTranForward))( this );
}
int astGetTranInverse_( AstMapping *this ) {
   if ( !astOK ) return 0;
   return (**astMEMBER(this,Mapping,GetTranInverse))( this );
}
void astInvert_( AstMapping *this ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,Invert))( this );
}
void astMapBox_( AstMapping *this,
                 const double lbnd_in[], const double ubnd_in[], int forward,
                 int coord_out, double *lbnd_out, double *ubnd_out,
                 double xl[], double xu[] ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,MapBox))( this, lbnd_in, ubnd_in, forward,
                                       coord_out, lbnd_out, ubnd_out, xl, xu );
}
int astMapList_( AstMapping *this, int series, int invert, int *nmap,
                  AstMapping ***map_list, int **invert_list ) {
   if ( !astOK ) return 0;
   return (**astMEMBER(this,Mapping,MapList))( this, series, invert,
                                        nmap, map_list, invert_list );
}
int *astMapSplit_( AstMapping *this, int nin, int *in, AstMapping **map ){
   if( map ) *map = NULL;
   if ( !astOK ) return NULL;
   return (**astMEMBER(this,Mapping,MapSplit))( this, nin, in, map );
}
int astMapMerge_( AstMapping *this, int where, int series, int *nmap,
                  AstMapping ***map_list, int **invert_list ) {
   if ( !astOK ) return -1;
   return (**astMEMBER(this,Mapping,MapMerge))( this, where, series, nmap,
                                                map_list, invert_list );
}
void astReportPoints_( AstMapping *this, int forward,
                       AstPointSet *in_points, AstPointSet *out_points ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,ReportPoints))( this, forward,
                                             in_points, out_points );
}
#define MAKE_RESAMPLE_(X,Xtype) \
int astResample##X##_( AstMapping *this, int ndim_in, const int *lbnd_in, \
                       const int *ubnd_in, const Xtype *in, \
                       const Xtype *in_var, int interp, \
                       void (* finterp)(), const double *params, \
                       int flags, double tol, int maxpix, Xtype badval, \
                       int ndim_out, \
                       const int *lbnd_out, const int *ubnd_out, \
                       const int *lbnd, const int *ubnd, Xtype *out, \
                       Xtype *out_var ) { \
   if ( !astOK ) return 0; \
   return (**astMEMBER(this,Mapping,Resample##X))( this, ndim_in, lbnd_in, \
                                                   ubnd_in, in, in_var, \
                                                   interp, finterp, params, \
                                                   flags, tol, maxpix, \
                                                   badval, ndim_out, \
                                                   lbnd_out, ubnd_out, \
                                                   lbnd, ubnd, \
                                                   out, out_var ); \
}
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_RESAMPLE_(LD,long double)
#endif
MAKE_RESAMPLE_(D,double)
MAKE_RESAMPLE_(F,float)
MAKE_RESAMPLE_(L,long int)
MAKE_RESAMPLE_(UL,unsigned long int)
MAKE_RESAMPLE_(I,int)
MAKE_RESAMPLE_(UI,unsigned int)
MAKE_RESAMPLE_(S,short int)
MAKE_RESAMPLE_(US,unsigned short int)
MAKE_RESAMPLE_(B,signed char)
MAKE_RESAMPLE_(UB,unsigned char)
#undef MAKE_RESAMPLE_

#define MAKE_REBIN_(X,Xtype) \
void astRebin##X##_( AstMapping *this, double wlim, int ndim_in, const int *lbnd_in, \
                    const int *ubnd_in, const Xtype *in, \
                    const Xtype *in_var, int interp, \
                    const double *params, \
                    int flags, double tol, int maxpix, Xtype badval, \
                    int ndim_out, \
                    const int *lbnd_out, const int *ubnd_out, \
                    const int *lbnd, const int *ubnd, Xtype *out, \
                    Xtype *out_var ) { \
   if ( !astOK ) return; \
   (**astMEMBER(this,Mapping,Rebin##X))( this, wlim, ndim_in, lbnd_in, \
                                         ubnd_in, in, in_var, \
                                         interp, params, \
                                         flags, tol, maxpix, \
                                         badval, ndim_out, \
                                         lbnd_out, ubnd_out, \
                                         lbnd, ubnd, \
                                         out, out_var ); \
}
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_REBIN_(LD,long double)
#endif
MAKE_REBIN_(D,double)
MAKE_REBIN_(F,float)
MAKE_REBIN_(I,int)
#undef MAKE_REBIN_

#define MAKE_REBINSEQ_(X,Xtype) \
void astRebinSeq##X##_( AstMapping *this, double wlim, int ndim_in, const int *lbnd_in, \
                        const int *ubnd_in, const Xtype *in, \
                        const Xtype *in_var, int interp, \
                        const double *params, \
                        int flags, double tol, int maxpix, Xtype badval, \
                        int ndim_out, \
                        const int *lbnd_out, const int *ubnd_out, \
                        const int *lbnd, const int *ubnd, Xtype *out, \
                        Xtype *out_var, double *weights ) { \
   if ( !astOK ) return; \
   (**astMEMBER(this,Mapping,RebinSeq##X))( this, wlim, ndim_in, lbnd_in, \
                                         ubnd_in, in, in_var, \
                                         interp, params, \
                                         flags, tol, maxpix, \
                                         badval, ndim_out, \
                                         lbnd_out, ubnd_out, \
                                         lbnd, ubnd, \
                                         out, out_var, weights ); \
}
#if HAVE_LONG_DOUBLE     /* Not normally implemented */
MAKE_REBINSEQ_(LD,long double)
#endif
MAKE_REBINSEQ_(D,double)
MAKE_REBINSEQ_(F,float)
MAKE_REBINSEQ_(I,int)
#undef MAKE_REBINSEQ_

double astRate_( AstMapping *this, double *at, int ax1, int ax2 ){
   if ( !astOK ) return AST__BAD;

   if( ax1 < 0 || ax1 >= astGetNout( this ) ) {
      astError( AST__AXIIN, "astRate(%s): Invalid output index (%d) "
                "specified - should be in the range 1 to %d.",
                astGetClass( this ), ax1 + 1, astGetNout( this ) );
      
   } else if( ax2 < 0 || ax2 >= astGetNin( this ) ) {
      astError( AST__AXIIN, "astRate(%s): Invalid input index (%d) "
                "specified - should be in the range 1 to %d.",
                astGetClass( this ), ax2 + 1, astGetNin( this ) );
   }

   if( rate_disabled ) {
      return ( at[ ax2 ] != AST__BAD ) ? 1.0 : AST__BAD;
   } else {    
      return (**astMEMBER(this,Mapping,Rate))( this, at, ax1, ax2 );
   }   
}
AstMapping *astSimplify_( AstMapping *this ) {
   AstMapping *result;
   if ( !astOK ) return NULL;
   if( !astGetIsSimple( this ) ) {      /* Only simplify if not already done */
      result = (**astMEMBER(this,Mapping,Simplify))( this );
      if( result ) result->issimple = 1;/* Indicate simplification has been done */
   } else {
      result = astClone( this );
   }
   return result;
}
AstPointSet *astTransform_( AstMapping *this, AstPointSet *in,
                            int forward, AstPointSet *out ) {
   if ( !astOK ) return NULL;
   return (**astMEMBER(this,Mapping,Transform))( this, in, forward, out );
}
void astTran1_( AstMapping *this, int npoint, const double xin[],
                int forward, double xout[] ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,Tran1))( this, npoint, xin, forward, xout );
}
void astTran2_( AstMapping *this,
                int npoint, const double xin[], const double yin[],
                int forward, double xout[], double yout[] ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,Tran2))( this, npoint, xin, yin,
                                      forward, xout, yout );
}
void astTranGrid_( AstMapping *this, int ncoord_in, const int lbnd[], 
                   const int ubnd[], double tol, int maxpix, int forward, 
                   int ncoord_out, int outdim, double *out ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,TranGrid))( this, ncoord_in, lbnd, ubnd, tol, 
                                         maxpix, forward, ncoord_out, outdim, 
                                         out );
}
void astTranN_( AstMapping *this, int npoint,
                int ncoord_in, int indim, const double *in,
                int forward, int ncoord_out, int outdim, double *out ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,TranN))( this, npoint,
                                      ncoord_in, indim, in,
                                      forward, ncoord_out, outdim, out );
}
void astTranP_( AstMapping *this, int npoint,
                int ncoord_in, const double *ptr_in[],
                int forward, int ncoord_out, double *ptr_out[] ) {
   if ( !astOK ) return;
   (**astMEMBER(this,Mapping,TranP))( this, npoint,
                                      ncoord_in, ptr_in,
                                      forward, ncoord_out, ptr_out );
}
int astLinearApprox_( AstMapping *this, const double *lbnd, 
                       const double *ubnd, double tol, double *fit ){
   if ( !astOK ) return 0;
   return (**astMEMBER(this,Mapping,LinearApprox))( this, lbnd, ubnd, tol, fit );
}

/* Public Interface Function Prototypes. */
/* ------------------------------------- */
/* The following functions have public prototypes only (i.e. no
   protected prototypes), so we must provide local prototypes for use
   within this module. */
void DecomposeId_( AstMapping *, AstMapping **, AstMapping **, int *, int *, int * );
void MapBoxId_( AstMapping *, const double [], const double [], int, int, double *, double *, double [], double [] );
double astRateId_( AstMapping *, double *, int, int );
void astMapSplitId_( AstMapping *, int, int *, int *, AstMapping ** );

/* Special interface function implementations. */
/* ------------------------------------------- */
void astDecomposeId_( AstMapping *this, AstMapping **map1, 
                      AstMapping **map2, int *series, int *invert1, 
                      int *invert2  ) {
/*
*++
*  Name:
c     astDecompose
f     AST_DECOMPOSE

*  Purpose:
*     Decompose a Mapping into two component Mappings.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astDecompose( AstMapping *this, AstMapping **map1, 
c                        AstMapping **map2, int *series, int *invert1, 
c                        int *invert2  ) 
f     CALL AST_DECOMPOSE( THIS, MAP1, MAP2, SERIES, INVERT1, INVERT2, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function returns pointers to two Mappings which, when applied
f     This routine returns pointers to two Mappings which, when applied
*     either in series or parallel, are equivalent to the supplied Mapping.
*
*     Since the Frame class inherits from the Mapping class, Frames can
*     be considered as special types of Mappings and so this method can
*     be used to decompose either CmpMaps or CmpFrames.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping.
c     map1
f     MAP1 = INTEGER (Returned)
c        Address of a location to receive a pointer to first component
f        A pointer to first component
*        Mapping. 
c     map2
f     MAP2 = INTEGER (Returned)
c        Address of a location to receive a pointer to second component
f        A pointer to second component
*        Mapping. 
c     series
f     SERIES = LOGICAL (Returned)
c        Address of a location to receive a value indicating if the
c        component Mappings are applied in series or parallel. A non-zero
c        value means that the supplied Mapping is equivalent to applying map1 
c        followed by map2 in series. A zero value means that the supplied
c        Mapping is equivalent to applying map1 to the lower numbered axes
c        and map2 to the higher numbered axes, in parallel.
f        Indicates if the
f        component Mappings are applied in series or parallel. A .TRUE.
f        value means that the supplied Mapping is equivalent to applying MAP1 
f        followed by MAP2 in series. A zero value means that the supplied
f        Mapping is equivalent to applying MAP1 to the lower numbered axes
f        and MAP2 to the higher numbered axes, in parallel.
c     invert1
f     INVERT1 = INTEGER (Returned)
c        The value of the Invert attribute to be used with map1. 
f        The value of the Invert attribute to be used with MAP1. 
c     invert2
f     INVERT2 = INTEGER (Returned)
c        The value of the Invert attribute to be used with map2. 
f        The value of the Invert attribute to be used with MAP2. 

*  Applicability:
*     CmpMap
c        If the supplied Mapping is a CmpMap, then map1 and map2 will be
f        If the supplied Mapping is a CmpMap, then MAP1 and MAP2 will be
*        returned holding pointers to the component Mappings used to
*        create the CmpMap, either in series or parallel. Note, changing
*        the Invert attribute of either of the component Mappings using
*        the returned pointers will have no effect on the supplied CmpMap.
*        This is because the CmpMap remembers and uses the original settings 
*        of the Invert attributes (that is, the values of the Invert
*        attributes when the CmpMap was first created). These are the
c        Invert values which are returned in invert1 and invert2.
f        Invert values which are returned in INVERT1 and INVERT2.
*     TranMap
c        If the supplied Mapping is a TranMap, then map1 and map2 will be
f        If the supplied Mapping is a TranMap, then MAP1 and MAP2 will be
*        returned holding pointers to the forward and inverse Mappings 
*        represented by the TranMap (zero will be returned for
c        series).
f        SERIES).
*        Note, changing the Invert attribute of 
*        either of the component Mappings using the returned pointers will 
*        have no effect on the supplied TranMap. This is because the TranMap 
*        remembers and uses the original settings of the Invert attributes 
*        (that is, the values of the Invert attributes when the TranMap was 
*        first created). These are the
c        Invert values which are returned in invert1 and invert2.
f        Invert values which are returned in INVERT1 and INVERT2.
*     Mapping
c        For any class of Mapping other than a CmpMap, map1 will be
c        returned holding a clone of the supplied Mapping pointer, and map2
c        will be returned holding a NULL pointer. Invert1 will be returned
c        holding the current value of the Invert attribute for the supplied 
c        Mapping, and invert2 will be returned holding zero.
f        For any class of Mapping other than a CmpMap, MAP1 will be
f        returned holding a clone of the supplied Mapping pointer, and MAP2
f        will be returned holding AST__NULL. INVERT1 will be returned
f        holding the current value of the Invert attribute for the supplied 
f        Mapping, and INVERT2 will be returned holding zero.
*     CmpFrame
c        If the supplied Mapping is a CmpFrame, then map1 and map2 will be
f        If the supplied Mapping is a CmpFrame, then MAP1 and MAP2 will be
*        returned holding pointers to the component Frames used to
*        create the CmpFrame. The component Frames are considered to be in
*        applied in parallel.
*     Frame
c        For any class of Frame other than a CmpFrame, map1 will be 
c        returned holding a clone of the supplied Frame pointer, and map2
c        will be returned holding a NULL pointer.
f        For any class of Frame other than a CmpFrame, MAP1 will be 
f        returned holding a clone of the supplied Frame pointer, and MAP2
f        will be returned holding AST__NULL.

*  Notes:
*     - The returned Invert values should be used in preference to the 
*     current values of the Invert attribute in map1 and map2. This is
*     because the attributes may have changed value since the Mappings 
*     were combined.
*     - Any changes made to the component Mappings using the returned
*     pointers will be reflected in the supplied Mapping. 

*--

*  Implementation Notes:
*     This function implements the public interface for the
*     astDecompose method. It is identical to astDecompose_ except for
*     the following:
*
*     - ID values are returned via the "map1" and "map2" parameters
*     instead of true C pointers. This is required because this
*     conversion cannot be performed by the macro that invokes the
*     function.
*/

/* Check the global error status. */
   if ( !astOK ) return;

/* Invoke the normal astDecompose_ function to decompose the Mapping. */
      astDecompose_( this, map1, map2, series, invert1, invert2 );

/* If required, return ID values for the component Mappings. */
   if ( map1 ) *map1 = astMakeId( *map1 );
   if ( map2 ) *map2 = astMakeId( *map2 );

}

void astMapBoxId_( AstMapping *this,
                   const double lbnd_in[], const double ubnd_in[],
                   int forward, int coord_out,
                   double *lbnd_out, double *ubnd_out,
                   double xl[], double xu[] ) {
/*
*++
*  Name:
c     astMapBox
f     AST_MAPBOX

*  Purpose:
*     Find a bounding box for a Mapping.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astMapBox( AstMapping *this,
c                     const double lbnd_in[], const double ubnd_in[],
c                     int forward, int coord_out,
c                     double *lbnd_out, double *ubnd_out,
c                     double xl[], double xu[] );
f     CALL AST_MAPBOX( THIS, LBND_IN, UBND_IN, FORWARD, COORD_OUT,
f                      LBND_OUT, UBND_OUT, XL, XU, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function allows you to find the "bounding box" which just
c     encloses another box after it has been transformed by a Mapping
c     (using either its forward or inverse transformation). A typical
c     use might be to calculate the size of an image after being
c     transformed by a Mapping.
f     This routine allows you to find the "bounding box" which just
f     encloses another box after it has been transformed by a Mapping
f     (using either its forward or inverse transformation). A typical
f     use might be to calculate the size of an image after being
f     transformed by a Mapping.
*
c     The function works on one dimension at a time. When supplied
c     with the lower and upper bounds of a rectangular region (box) of
c     input coordinate space, it finds the lowest and highest values
c     taken by a nominated output coordinate within that
c     region. Optionally, it also returns the input coordinates where
c     these bounding values are attained. It should be used repeatedly
c     to obtain the extent of the bounding box in more than one
c     dimension.
f     The routine works on one dimension at a time. When supplied with
f     the lower and upper bounds of a rectangular region (box) of
f     input coordinate space, it finds the lowest and highest values
f     taken by a nominated output coordinate within that region. It
f     also returns the input coordinates where these bounding values
f     are attained. It should be used repeatedly to obtain the extent
f     of the bounding box in more than one dimension.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping.
c     lbnd_in
f     LBND_IN( * ) = DOUBLE PRECISION (Given)
c        Pointer to an array of double, with one element for each
c        Mapping input coordinate. This should contain the lower bound
c        of the input box in each input dimension.
f        An array with one element for each Mapping input
f        coordinate. This should contain the lower bound of the input
f        box in each input dimension.
c     ubnd_in
f     UBND_IN( * ) = DOUBLE PRECISION (Given)
c        Pointer to an array of double, with one element for each
c        Mapping input coordinate. This should contain the upper bound
c        of the input box in each input dimension.
f        An array with one element for each Mapping input
f        coordinate. This should contain the upper bound of the input
f        box in each input dimension.
*
*        Note that it is permissible for the upper bound to be less
*        than the corresponding lower bound, as the values will simply
*        be swapped before use.
c     forward
f     FORWARD = LOGICAL (Given)
c        If this value is non-zero, then the Mapping's forward
c        transformation will be used to transform the input
c        box. Otherwise, its inverse transformation will be used.
f        If this value is .TRUE., then the Mapping's forward
f        transformation will be used to transform the input
f        box. Otherwise, its inverse transformation will be used.
*
c        (If the inverse transformation is selected, then references
c        to "input" and "output" coordinates in this description
c        should be transposed. For example, the size of the "lbnd_in"
c        and "ubnd_in" arrays should match the number of output
c        coordinates, as given by the Mapping's Nout
c        attribute. Similarly, the "coord_out" parameter, below,
c        should nominate one of the Mapping's input coordinates.)
f        (If the inverse transformation is selected, then references
f        to "input" and "output" coordinates in this description
f        should be transposed. For example, the size of the LBND_IN
f        and UBND_IN arrays should match the number of output
f        coordinates, as given by the Mapping's Nout attribute.
f        Similarly, the COORD_OUT argument, below, should nominate one
f        of the Mapping's input coordinates.)
c     coord_out
f     COORD_OUT = INTEGER (Given)
*        The index of the output coordinate for which the lower and
*        upper bounds are required. This value should be at least one,
*        and no larger than the number of Mapping output coordinates.
c     lbnd_out
f     LBND_OUT = DOUBLE PRECISION (Returned)
c        Pointer to a double in which to return the lowest value taken
c        by the nominated output coordinate within the specified
c        region of input coordinate space.
f        The lowest value taken by the nominated output coordinate
f        within the specified region of input coordinate space.
c     ubnd_out
f     UBND_OUT = DOUBLE PRECISION (Returned)
c        Pointer to a double in which to return the highest value
c        taken by the nominated output coordinate within the specified
c        region of input coordinate space.
f        The highest value taken by the nominated output coordinate
f        within the specified region of input coordinate space.
c     xl
f     XL( * ) = DOUBLE PRECISION (Returned)
c        An optional pointer to an array of double, with one element
c        for each Mapping input coordinate. If given, this array will
c        be filled with the coordinates of an input point (although
c        not necessarily a unique one) for which the nominated output
c        coordinate attains the lower bound value returned in
c        "*lbnd_out".
c
c        If these coordinates are not required, a NULL pointer may be
c        supplied.
f        An array with one element for each Mapping input
f        coordinate. This will return the coordinates of an input
f        point (although not necessarily a unique one) for which the
f        nominated output coordinate attains the lower bound value
f        returned in LBND_OUT.
c     xu
f     XU( * ) = DOUBLE PRECISION (Returned)
c        An optional pointer to an array of double, with one element
c        for each Mapping input coordinate. If given, this array will
c        be filled with the coordinates of an input point (although
c        not necessarily a unique one) for which the nominated output
c        coordinate attains the upper bound value returned in
c        "*ubnd_out".
c
c        If these coordinates are not required, a NULL pointer may be
c        supplied.
f        An array with one element for each Mapping input
f        coordinate. This will return the coordinates of an input
f        point (although not necessarily a unique one) for which the
f        nominated output coordinate attains the upper bound value
f        returned in UBND_OUT.
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Notes:
*     - Any input points which are transformed by the Mapping to give
*     output coordinates containing the value AST__BAD are regarded as
*     invalid and are ignored. They will make no contribution to
*     determining the output bounds, even although the nominated
*     output coordinate might still have a valid value at such points.
c     - An error will occur if the required output bounds cannot be
c     found. Typically, this might happen if all the input points
c     which the function considers turn out to be invalid (see
c     above). The number of points considered before generating such
c     an error is quite large, so this is unlikely to occur by
c     accident unless valid points are restricted to a very small
c     subset of the input coordinate space.
f     - An error will occur if the required output bounds cannot be
f     found. Typically, this might happen if all the input points
f     which the routine considers turn out to be invalid (see
f     above). The number of points considered before generating such
f     an error is quite large, so this is unlikely to occur by
f     accident unless valid points are restricted to a very small
f     subset of the input coordinate space.
c     - The values returned via "lbnd_out", "ubnd_out", "xl" and "xu"
c     will be set to the value AST__BAD if this function should fail
c     for any reason. Their initial values on entry will not be
c     altered if the function is invoked with the AST error status
c     set.
f     - The values returned via LBND_OUT, UBND_OUT, XL and XU will be
f     set to the value AST__BAD if this routine should fail for any
f     reason. Their initial values on entry will not be altered if the
f     routine is invoked with STATUS set to an error value.
*--

*  Implementation Notes:
*     This function implements the public interface for the astMapBox
*     method. It is identical to astMapBox_ except that the nominated
*     output coordinate given in "coord_out" is decremented by one
*     before use.  This is to allow the public interface to use
*     one-based coordinate numbering (internally, zero-based
*     coordinate numbering is used).
*/

/* Check the global error status. */
   if ( !astOK ) return;

/* Invoke the protected version of this function with the "coord_out"
   value decremented. */
   astMapBox_( this, lbnd_in, ubnd_in, forward, coord_out - 1,
               lbnd_out, ubnd_out, xl, xu );
}

double astRateId_( AstMapping *this, double *at, int ax1, int ax2 ){
/*
*++
*  Name:
c     astRate
f     AST_RATE

*  Purpose:
*     Calculate the rate of change of a Mapping output.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     double astRate( AstMapping *this, double *at, int ax1, int ax2 )
f     RESULT = AST_RATE( THIS, AT, AX1, AX2, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function 
f     This routine 
*     evaluates the rate of change of a specified output of the supplied
*     Mapping with respect to a specified input, at a specified input
*     position. 
*
*     The result is estimated by interpolating the function using a
*     fourth order polynomial in the neighbourhood of the specified
*     position. The size of the neighbourhood used is chosen to minimise
*     the RMS residual per unit length between the interpolating
*     polynomial and the supplied Mapping function. This method produces
*     good accuracy but can involve evaluating the Mapping 100 or more
*     times.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping to be applied.
c     at
f     AT( * ) = DOUBLE PRECISION (Given)
c        The address of an 
f        An
*        array holding the axis values at the position at which the rate
*        of change is to be evaluated. The number of elements in this
*        array should equal the number of inputs to the Mapping.
c     ax1
f     AX1 = INTEGER (Given)
*        The index of the Mapping output for which the rate of change is to 
*        be found (output numbering starts at 1 for the first output).
c     ax2
f     AX2 = INTEGER (Given)
*        The index of the Mapping input which is to be varied in order to
*        find the rate of change (input numbering starts at 1 for the first 
*        input).
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Returned Value:
c     astRate()
f     AST_RATE = DOUBLE PRECISION
c        The rate of change of Mapping output "ax1" with respect to input 
c        "ax2", evaluated at "at", or AST__BAD if the value cannot be 
c        calculated.
f        The rate of change of Mapping output AX1 with respect to input 
f        AX2, evaluated at AT, or AST__BAD if the value cannot be 
f        calculated.

*  Notes:
*     - A value of AST__BAD will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*--

*  Implementation Notes:
*     This function implements the public interface for the astRate
*     method. It is identical to astRate_ except that the nominated
*     coordinates given in "ax1" and "ax2" are decremented by one
*     before use.  This is to allow the public interface to use
*     one-based coordinate numbering (internally, zero-based
*     coordinate numbering is used).
*/

/* Check the global error status. */
   if ( !astOK ) return AST__BAD;

/* Invoke the protected version of this function with the axis indices
   decremented. */
   return astRate_( this, at, ax1 - 1, ax2 - 1 );
}

void astMapSplitId_( AstMapping *this, int nin, int *in, int*out, 
                            AstMapping **map ){
/*
*++
*  Name:
c     astMapSplit
f     AST_MAPSPLIT

*  Purpose:
*     Split a Mapping up into parallel component Mappings.

*  Type:
*     Public virtual function.

*  Synopsis:
c     #include "mapping.h"
c     void astMapSplit( AstMapping *this, int nin, int *in, int *nout,
c                       AstMapping **map )
f     CALL AST_MAPSPLIT( THIS, NIN, IN, OUT, MAP, STATUS )

*  Class Membership:
*     Mapping method.

*  Description:
c     This function 
f     This routine
*     creates a new Mapping which connects specified inputs within a
*     supplied Mapping to the corresponding outputs of the supplied Mapping. 
*     This is only possible if the specified inputs correspond to some 
*     subset of the Mapping outputs. That is, there must exist a subset of 
*     the Mapping outputs for which each output depends only on the selected 
*     Mapping inputs, and not on any of the inputs which have not been 
*     selected. If this condition is not met by the supplied Mapping, then
c     a NULL 
f     an AST__NULL 
*     Mapping pointer is returned.

*  Parameters:
c     this
f     THIS = INTEGER (Given)
*        Pointer to the Mapping to be split.
c     nin
f     NIN = INTEGER (Given)
c        The number of inputs to pick from "this".
f        The number of inputs to pick from THIS.
c     in
f     IN( NIN ) = INTEGER (Given)
c        Pointer to an 
f        An
*        array holding the indices within the supplied Mapping of the inputs 
*        which are to be picked from the Mapping. 
c        This array should have "nin" elements. 
*        If "Nin" is the number of inputs of the supplied Mapping, then each 
*        element should have a value in the range 1 to Nin.
c     out
f     OUT( * ) = INTEGER (Returned)
c        Pointer to an 
f        An
*        array in which to return the indices of the outputs of the supplied 
*        Mapping which are fed by the picked inputs. A value of one is
*        used to refer to the first Mapping output. The supplied array should 
*        have a length at least equal to the number of outputs in the
*        supplied Mapping. The number of values stored in the array on
*        exit will equal the number of outputs in the returned Mapping.
*        The i'th element in the returned array holds the index within
*        the supplied Mapping which corresponds to the i'th output of 
*        the returned Mapping.
c     map
f     MAP = INTEGER (Returned)
c        Address of a location at which to return a pointer to the 
f        The
*        returned Mapping. This Mapping will have 
c        "nin" inputs (the number of outputs may be different to "nin"). NULL
f        NIN inputs (the number of outputs may be different to NIN). AST__NULL
*        is returned if the supplied Mapping has no subset of outputs which 
*        depend only on the selected inputs.

*  Notes:
*     - If this 
c     function 
f     routine
*     is invoked with the global error status set, or if it should fail for 
*     any reason, then 
c     a NULL value
f     AST__NULL
*     will be returned for 
c     the "map" pointer.
f     MAP.
*--

*  Implementation Notes:
*     - This function implements the astMapSplit method available via the 
*     public interface to the Mapping class and uses 1-based axis indices. 
*     The protected interface method is provided by the astMapSplit function
*     and uses zero-based axis indices. Also, an ID value is returned for
*     "map" rather than a pointer.
*/

/* Local Variables: */
   int *in_zero;           /* Pointer to array of zero-based input indices */
   int *result;            /* Pointer to array of zero-based output indices*/
   int i;                  /* Axis index */
   int nout;               /* No of outputs */

/* Initialise */
   *map = NULL;

/* Check the global error status. */
   if ( !astOK ) return;

/* Decrement the axis indices by 1. */
   in_zero = astMalloc( sizeof( int )*(size_t) nin );
   if( in_zero ) {
      for( i = 0; i < nin; i++ ) in_zero[ i ] = in[ i ] - 1;

/* Invoked the protected astMapSplit functon. */
      result = astMapSplit( this, nin, in_zero, map );

/* If succesful, copy the output axes to the supplied array. */
      if( result ) {
         nout = astGetNout( *map );
         for( i = 0; i < nout; i++ ) out[ i ] = result[ i ] + 1;

/* Free resurces. */
         result = astFree( result );
      }
      in_zero = astFree( in_zero );
   }

/* Free the returned Mapping if an error has occurred. */
   if( !astOK ) *map = astAnnul( *map );

/* Return an ID value for the Mapping. */
   *map = astMakeId( *map );
}


