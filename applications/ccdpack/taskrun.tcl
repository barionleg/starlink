#
#  This script is not currently used by any of the CCDPACK code, but it
#  should work if it is necessary to run monoliths under the control of
#  tcl scripts.  See also tasksetup.tcl
# 

   proc taskrun {app arguments {message ""} {window ""}} {
#+
#  Name:
#     taskrun

#  Type of Module:
#     Tcl/Tk procedure.

#  Purpose:
#     Runs an application.

#  Description:
#     This routine runs an application of an ADAM task loaded using
#     the adamtask Tcl extensions. It will optionally display a message
#     while the task is running.  It will not return until the
#     application has completed.

#  Arguments:
#     app = string (read)
#        The name of the application to be run. This should be registered
#        in the routine CCDAppRegister.
#     arguments = string (read)
#        The arguments to be used with the application (as would be
#        supplied on the command line).
#     message = string (read)
#        If supplied as a non-empty string, this text will be displayed in
#        a window while the application executes.  In this case, no user
#        interaction will be allowed while the application is executing.
#     window = string (read)
#        If supplied as a non-empty string, and the message argument is
#        also present, this gives the pathname of a window over which
#        the message will be centred.

#  Global variables (of note):
#     CCDdir = string (read)
#        The CCDPACK binary directory.
#     TASK = array (read and write)
#        The element.
#
#          TASK($app,return)
#
#        Is only written to when the application exits, so can be used
#        to wait, but care needs to be taken with the timing since an
#        application may well complete quickly (i.e. before you get a
#        chance to tkwait). It is better to use the internal wait
#        method of this routine.
#
#        The elements.
#
#           TASK($app,error)
#           TASK($app,output)
#
#        may contain the actual textual output from the application
#        and any error messages.
#
#        The element
#
#           TASK($app,progress)
#
#        is used to indicate the fraction of the process that is completed.

#  Authors:
#     MBT: Mark Taylor (STARLINK)
#     {enter_new_authors_here}

#  History:
#     11-OCT-2000 (MBT):
#        Original version.
#     {enter_further_changes_here}

#-

#  Global variables.
      global CCDdir
      global TASK
      global MONOLITH
      global Bitmaps
#.

#  Check that the application is available.
      if { ! [CCDTaskStart $app] } {
         CCDIssueError "The application \"$app\" is not available in
this interface (probably programming error)."
         return
      }

#  Initialise task watch variables.
      set TASK($app,error) ""
      set TASK($app,output) ""
      set TASK($app,progress) 0

#  Display a wait widget window if required.
      set wait ""
      if { $message != "" } {
         set wait [ waiter $window.wait -text $message ]
      }

#  Run the task.
      set task $MONOLITH($TASK($app,monolith),taskname)
      $task obey $app "$arguments" \
         -endmsg   "global TASK; set TASK($app,return) completed" \
         -inform   "" \
         -paramreq "$task paramreply %R !!"

#  Wait for it to complete.
      tkwait variable TASK($app,return)

#  Remove the message window if required.
      if { [ winfo exists $wait ] } {
         destroy $wait
      }

}
# $Id$
