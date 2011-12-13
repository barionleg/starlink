#!/bin/csh
#+
#  Name:
#     OUTLINECLUMP

#  Purpose:
#     Draw an outline around a 2-dimensional clump identified by CUPID.

#  Language:
#     C-shell

#  Type of Module:
#     C-shell script

#  Description:
#     This procedure will outline a specified clump previously identified
#     by CUPID:FINDCLUMPS or CUPID:EXTRACTCLUMPS. The data must be
#     2-dimensional, and the image over which the outline is to be drawn
#     must have been displayed previously using KAPPA:DISPLAY.

#  Usage:
#     outlineclump ndf index [style]

#  ADAM Parameters:
#     NDF = NDF (Read)
#        The name of the NDF containing the clump information. This NDF
#        should have been created using the CUPID:FINDCLUMPS or
#        CUPID:EXTRACTCLUMPS command. The clump cut-out images contained in
#        the CUPID extension of this NDF will be used to define the outline
#        of the clump.
#     INDEX = _INTEGER (Read)
#        The integer index of the clump to be identified.
#     STYLE = LITERAL (Read)
#        A group of attribute settings describing the plotting style to
#        use for the outline.
#
#        A comma-separated list of strings should be given in which each
#        string is either an attribute setting, or the name of a text
#        file preceded by an up-arrow character "^".  Such text files
#        should contain further comma-separated lists which will be read
#        and interpreted in the same manner.  Attribute settings are
#        applied in the order in which they occur within the list, with
#        later settings overriding any earlier settings given for the
#        same attribute.
#
#        Each individual attribute setting should be of the form:
#
#           <name>=<value>
#
#        where <name> is the name of a plotting attribute, and <value>
#        is the value to assign to the attribute. Default values will be
#        used for any unspecified attributes.  All attributes will be
#        defaulted if a null value (!) is supplied.  See section
#        "Plotting Attributes" in SUN/95 for a description of the
#        available attributes.  Any unrecognised attributes are ignored
#        (no error is reported).
#
#        The appearance of the clump outline is controlled by the attributes
#        Colour(Curves), Width(Curves), etc (the synonym Contours may be
#        used in place of Curves). The contour appearance established in
#        this way may be modified using parameters PENS, PENROT and
#        DASHED.  [current value]

#  Examples:
#     outlineclump m51b 2 style="'colour=blue,width=4'"
#        This draws an outline of the second clump (as stored in
#        m51b.more.cupid.clumps(2).model) on the current graphics device,
#        using a blue line of four times the default thinkness.

#  Notes:
#     -  The script is simply a wrapper for the KAPPA command:
#
#     contour ndf="$ndf.more.cupid.clumps($index).model" labpos=\! mode=good clear=no

#  Copyright:
#     Copyright (C) 2007 Particle Physics & Astronomy Research Council.
#     All Rights Reserved.

#  Licence:
#     This program is free software; you can redistribute it and/or
#     modify it under the terms of the GNU General Public License as
#     published by the Free Software Foundation; either version 2 of
#     the License, or (at your option) any later version.
#
#     This program is distributed in the hope that it will be
#     useful, but WITHOUT ANY WARRANTY; without even the implied
#     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#     PURPOSE. See the GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
#     02110-1301, USA

#  Authors:
#     David S. Berry (DSB):
#     {enter_new_authors_here}

#  History:
#     31-JAN-2007 (DSB):
#        Original version (based on $KAPPA_DIR/fitsedit.csh).
#     {enter_further_changes_here}
#
#-

#
#  See whether or not an input NDF has been supplied.
#

set style = ""
set narg = $#argv

if ( $narg == 0 ) then
#
#   Set the logical that says whether or not a valid value has been
#   supplied.
#
   set ok = 0
   while ( $ok == 0 )
#
#   Ensure that the following invocation of parget will return a non-zero
#   status value if anything goes wrong.
#
      set adam_exit_set = $?ADAM_EXIT
      setenv ADAM_EXIT 1
#
#   Obtain the current DATA_ARRAY. Check that parget worked ok by testing
#   status. Also remove angle brackets introduced by parget (eg. replace
#   "$<KAPPA_DIR>/m31" by "$KAPPA_DIR/m31" ), and any NDF section specifier.
#
      set defndf = `$KAPPA_DIR/parget data_array GLOBAL`
      if ( $status || "$defndf" == "" ) then
         set prstring = "NDF - Name of the NDF > "
      else
         set defndf = `echo $defndf | sed -e 's/^\$<\(.*\)>\(.*\)/\$\1\2/' | sed -e 's/^\(.*\)(.*)/\1/'`
         set prstring = "NDF - Name of the NDF /@"$defndf"/ > "
         set ndf = $defndf
      endif
#
#   Clear ADAM_EXIT unless it was already set.
#
      if ( ! $adam_exit_set ) then
         unsetenv ADAM_EXIT
      endif
#
#   Assume that the value will be fine unless we discover otherwise
#   later.  Prompt for the value.  We must prevent the ? from being
#   treated as a single-character pattern match.  Various combinations
#   of quotes and backslashes do not seem to work.  Hence we use noglob.
#
      set ok = 1
      set noglob
      echo -n "$prstring"
      set ndf = $<
#
#   Write some help information, but continue in the loop.
#
      if ( "$ndf" == '?' ) then
         sh -c "echo '  ' 1>&2"
         sh -c "echo '   NDF  = NDF (Read)' 1>&2"
         sh -c "echo '      The name of the NDF conting the clump information.' 1>&2"
         sh -c "echo ' ' 1>&2"
         set ok = 0
#
#   Abort when requested.
#
      else if ( "$ndf" == \!\! || "$ndf" == \! ) then
         exit
#
#   Reprompt when no value is given.
#
      else if ( "$defndf" == "" && "$ndf" == "" ) then
         sh -c "echo 'No NDF specified.  Enter "\!\!" to abort.' 1>&2"
         set ok = 0

      else
#
#   Accept the default.
#
         if ( "$ndf" == "" && "$defndf" != "" ) then
            set ndf = $defndf
         endif
         unset noglob
#
#   Remove any shell meta-characters
#
         eval set ndf = "$ndf"
#
#   Check that the supplied NDF exists.  If there is a specific
#   extension, test that the file exists.
#
         if ( $ndf:e != "" ) then
            if (! -e $ndf ) then
               sh -c "echo 'Data file "$ndf" does not exist.' 1>&2"
               set ok = 0
            endif
         else
#
#   The filename does not have a file extension.  Thus it is either an
#   NDF or a foreign format defined by the NDF_FORMATS_IN environment
#   variable.  First test for an NDF.
#
            set file = $ndf".sdf"
            if ( ! -e $file ) then
#
#   The file might be in a foreign format.  Obtain the number and a list
#   of the valid file extensions in search-order from NDF_FORMATS_IN.
#   The first value gives the number of formats, so if this is 0,
#   NDF_FORMATS_IN is undefined.
#
               set formats = `printenv | grep NDF_FORMATS_IN | awk -f $KAPPA_DIR/nfi.awk`
               if ( $formats == "" ) then
                  sh -c "echo 'NDF "$ndf" does not exist.' 1>&2"
                  set ok = 0
               else if ( $formats[1] == 0 ) then
                  sh -c "echo 'NDF "$ndf" does not exist.' 1>&2"
                  set ok = 0
               else
                  set noformats = $formats[1]
                  shift formats
#
#   Test for the existence of each format in the list, until a match
#   is found.  Set the flag to indicate failure to find a file, until one
#   is found.
#
                  set ok = 0
                  set iform = 1
                  while ( $iform <= $noformats )
                     set file = $ndf$formats[$iform]
                     if ( ! -e $file ) then
                        @ iform = $iform + 1

                     else
                        set ok = 1
                        @ iform = $noformats + 1
                     endif
                  end
#
#   Report the case where no foreign file could be found.
#
                  if ( $ok == 0 ) then
                     sh -c "echo 'Data file "$ndf" does not exist.' 1>&2"
                  endif
               endif
            endif
         endif
      endif
      unset noglob
   end
#
#  Get the clump index
#
   echo -n "INDEX - The index of the clump to be outlined > "
   set index = $<
#
#  If one argument was supplied, assume it is the NDF name. Store it and
#  prompt for the clump index.
#
else if ( $narg == 1 ) then
   set ndf = $1
   echo -n "INDEX - The index of the clump to be outlined > "
   set index = $<
   if ( "$index" == \!\! || "$index" == \! ) then
      exit
   endif
#
#  If both arguments were supplied, store them.
#
else if( $narg == 2 ) then
   set ndf = $1
   set index = $2
#
#  If a plotting style was also supplied, store it.
#
else if( $narg == 3 ) then
   set ndf = $1
   set index = $2
   set style = "$3"
endif
#
#  Remove the parameter name from the start of the parameter value (if
#  present).
#
set ndf = `echo $ndf | sed -e 's/^ndf=//'`
set index = `echo $index | sed -e 's/^index=//'`
set style = `echo $style | sed -e 's/^style=//'`
#
#   See if the NDF has a CUPID extension.
#
$KAPPA_DIR/ndftrace $ndf quiet
set gotext = 0
if ( `$KAPPA_DIR/parget nextn ndftrace` > 0 ) then
   foreach ext (`$KAPPA_DIR/parget extname ndftrace`)
      if ( $ext == "CUPID" ) set gotext = 1
   end
endif
#
#  Report an error if the NDF has no CUPID extension.
#
if ( $gotext != 1 ) then
   echo "outlineclump: '$ndf' has no CUPID extension!"
   echo "Please supply an NDF that has been created by the CUPID:FINDCLUMPS command."
else
   if( "$style" == "" ) then
      $KAPPA_DIR/contour ndf="$ndf.more.cupid.clumps\($index\).model" labpos=\! mode=good clear=no
   else
      $KAPPA_DIR/contour ndf="$ndf.more.cupid.clumps\($index\).model" \
                         labpos=\! mode=good clear=no style="$style"
   endif
endif
exit
