/*
*+
*  Name:
*     datLocked

*  Purpose:
*     See if an object is locked.

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     Library routine

*  Invocation:
*     locked = datLocked( const HDSLoc *locator, int *status );

*  Arguments:
*     locator = const HDSLoc * (Given)
*        A locator for the object to be checked.
*     status = int* (Given and Returned)
*        Pointer to global status.

*  Returned function value:
*     A value indicating the status of the supplied Object:
*
*     0: the supplied object is unlocked. This is the condition that must
*        be met for the current thread to be able to lock the supplied
*        object for read-write access using function datLock. This condition
*        can be achieved by releasing any existing locks using datUnlock.
*
*     1: the supplied object is locked for reading and writing by the current
*        thread. This is the condition that must be met for the current
*        thread to be able to use the supplied object in any HDS function
*        that might modify the object (except for the locking and unlocking
*        functions - see below). This condition can be achieved by calling
*        datLock.
*
*     2: the supplied object is locked for reading and writing by a different
*        thread. An error will be reported if the current thread attempts to
*        use the object in any other HDS function.
*
*     3: the supplied object is locked read-only by the current thread
*        (and maybe other threads as well). This is the condition that must
*        be met for the current thread to be able to use the supplied object
*        in any HDS function that cannot modify the object. An error will be
*        reported if the current thread attempts to use the object in any HDS
*        function that could modify the object. This condition can be achieved
*        by calling datLock.
*
*     4: the supplied object is not locked by the current thread, but is
*        locked read-only by one or more other threads. An error will be
*        reported if the current thread attempts to use the object in any
*        other HDS function.

*  Description:
*     This is temporary stub for a new HDS-V5 function that returns a value
*     that indicates if the object specified by the supplied locator has
*     been locked for use by one or more threads. It simply reports an error
*     if called.

*  Authors:
*     DSB: David S Berry (EAO)
*     {enter_new_authors_here}

*  History:
*     7-SEP-2017 (DSB):
*        Initial version
*     {enter_further_changes_here}

*  Copyright:
*     Copyright (C) 2017 East Asian Observatory.
*     All Rights Reserved.

*  Licence:
*     Redistribution and use in source and binary forms, with or
*     without modification, are permitted provided that the following
*     conditions are met:
*
*     - Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*
*     - Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials
*       provided with the distribution.
*
*     - Neither the name of the {organization} nor the names of its
*       contributors may be used to endorse or promote products
*       derived from this software without specific prior written
*       permission.
*
*     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
*     CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*     MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
*     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*     LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*     USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*     AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
*     THE POSSIBILITY OF SUCH DAMAGE.

*  Bugs:
*     {note_any_bugs_here}
*-
*/


#include "ems.h"
#include "sae_par.h"
#include "dat_err.h"
#include "hds_types.h"

int datLocked( const HDSLoc *locator, int *status ) {

/* Local Variables; */
   int result = 0;

/* Check inherited status. */
   if (*status != SAI__OK) return result;

/* Report an error to indicate this function is not available yet. */
   *status = DAT__FATAL;
   emsRep( " ", "datLocked: The datLocked function has not yet been "
           "implemented (programming error).", status );

/* Return the result. */
   return result;
}

