#!/usr/bin/env python

'''
*+
*  Name:
*     pol2cat

*  Purpose:
*     Create a catalogue of Q,U and I values from a set of POL-2 data
*     files, and optionally creates a vector plot.

*  Language:
*     python (2.7 or 3.*)

*  Description:
*     This script runs SMURF:CALCQU on the POL-2 data files specified by
*     parameter IN, to create a set of images holding the mean Q and U
*     values in each bolometer. Each pair of Q and U images contains data
*     for a single sub-array from a short period of time over which the
*     position of each bolometer on the sky does not change significantly.
*
*     For each sub-array, images holding the mean bolometer Q and U are
*     found. These mean Q and U images are then subtracted from the
*     individual Q and U images.
*
*     All the Q images are then mosaiced together into a single Q image,
*     using the total intensity map specified by parameter IREF to define
*     the pixel grid. All the U images are mosaiced together in the same
*     way.
*
*     These Q, U and I images are then converted into a set of polarisation
*     vectors using POLPACK:POLVEC, and are stored in the FITS file
*     specified by parameter CAT.
*
*     Optionally, a map of polarised intensity can be produced. See
*     parameter PI.
*
*     Optionally, a vector plot can then be produced from the catalogue.
*     See parameter PLOT. However, it is usually much more versatile and
*     convenient to examine the final catalogue using TOPCAT, or the
*     polarimetry toolbox in GAIA.

*  Usage:
*     pol2cat in cat iref pi [plot] [snr] [maxlen] [domain] [pixsize]
*             [config] [device] [retain] [msg_filter] [ilevel] [glevel]
*             [logfile]

*  Parameters:
*     IN = NDF (Read)
*        A group of POL-2 time series NDFs.
*     CAT = LITERAL (Read)
*        The output FITS vector catalogue.
*     CONFIG = LITERAL (Read)
*        The configuration to use when cleaning the raw data. This should
*        be specified in the same way as the CONFIG parameter for
*        SMURF:MAKEMAP. ["^$STARLINK_DIR/share/smurf/dimmconfig.lis"]
*     DEVICE = LITERAL (Read)
*        The name of the graphics device on which to produce the vector
*        plot.  [current graphics device]
*     DOMAIN = LITERAL (Read)
*        The domain for alignment:
*
*        - "FPLANE": Alignment occurs in focal plane coordinates.
*
*        - "SKY": Alignment occurs in celestial sky coordinates.
*
*        ["SKY"]
*     GLEVEL = LITERAL (Read)
*        Controls the level of information to write to a text log file.
*        Allowed values are as for "ILEVEL". The log file to create is
*        specified via parameter "LOGFILE. In adition, the glevel value
*        can be changed by assigning a new integer value (one of
*        starutil.NONE, starutil.CRITICAL, starutil.PROGRESS,
*        starutil.ATASK or starutil.DEBUG) to the module variable
*        starutil.glevel. ["ATASK"]
*     ILEVEL = LITERAL (Read)
*        Controls the level of information displayed on the screen by the
*        script. It can take any of the following values (note, these values
*        are purposefully different to the SUN/104 values to avoid confusion
*        in their effects):
*
*        - "NONE": No screen output is created
*
*        - "CRITICAL": Only critical messages are displayed such as warnings.
*
*        - "PROGRESS": Extra messages indicating script progress are also
*        displayed.
*
*        - "ATASK": Extra messages are also displayed describing each atask
*        invocation. Lines starting with ">>>" indicate the command name
*        and parameter values, and subsequent lines hold the screen output
*        generated by the command.
*
*        - "DEBUG": Extra messages are also displayed containing unspecified
*        debugging information. In addition scatter plots showing how each Q
*        and U image compares to the mean Q and U image are displayed at this
*        ILEVEL.
*
*        In adition, the glevel value can be changed by assigning a new
*        integer value (one of starutil.NONE, starutil.CRITICAL,
*        starutil.PROGRESS, starutil.ATASK or starutil.DEBUG) to the module
*        variable starutil.glevel. ["PROGRESS"]
*     IREF = NDF (Read)
*        An optional total intensity map covering the same area. If
*        supplied, this will be used to determine percentage polarisation,
*        to determine the pixel size and projection for the Q and U images,
*        and will also be used a background for the plotted vectors. It
*        will also determine the extent of the resulting catalogue and
*        vector plot. If a null value (!) is supplied, then an image of
*        polarised intensity calculated from the supplied POL-2 data
*        will be used instead. This will result in percentage polarisation
*        values being 100%. The extent and WCS of the vector plot and
*        catalogue will then be determined by the supplied POL-2 data files.
*     LOGFILE = LITERAL (Read)
*        The name of the log file to create if GLEVEL is not NONE. The
*        default is "<command>.log", where <command> is the name of the
*        executing script (minus any trailing ".py" suffix), and will be
*        created in the current directory. Any file with the same name is
*        over-written. The script can change the logfile if necessary by
*        assign the new log file path to the module variable
*        "starutil.logfile". Any old log file will be closed befopre the
*        new one is opened. []
*     MAXLEN = _REAL (Read)
*        The maximum length for the plotted vectors, in terms of the
*        quantity specified by parameter PLOT. Only vectors below this
*        length are plotted. If a null (!) value is supplied,  no maximum
*        length is imposed. [!]
*     MSG_FILTER = LITERAL (Read)
*        Controls the default level of information reported by Starlink
*        atasks invoked within the executing script. This default can be
*        over-ridden by including a value for the msg_filter parameter
*        within the command string passed to the "invoke" function. The
*        accepted values are the list defined in SUN/104 ("None", "Quiet",
*        "Normal", "Verbose", etc). ["Normal"]
*     PI = NDF (Read)
*        The output NDF in which to return the polarised intensity map.
*        No polarised intensity map will be created if null (!) is supplied.
*        If a value is supplied for parameter IREF, then PI defaults to
*        null. Otherwise, the user is prompted for a value if none was
*        supplied on the command line. []
*     PIXSIZE = _REAL (Read)
*        The pixel size to use when forming the combined Q and U images,
*        in arc-seconds. If null (!) is supplied, the pixel size of the
*        image given by IREF will be used. If no image is given for IREF,
*        then the default pixel size used by CALCQU will be used (i.e.
*        pixel size equals bolometer spacing). [!]
*     PLOT = LITERAL (Read)
*        The quantity that gives the lengths of the plotted vectors:
*
*         - "P": Percentage polarisation.
*
*         - "PI": Polarised intensity.
*
*        If a null(!) value is supplied, no vectors will be plotted. Only
*        the vectors that pass the tests specified by parameter SNR and
*        MAXLEN are plotted. If a value is supplied for parameter IREF, then
*        only vectors that fall within the IREF image will be displayed.
*        The plot is displayed on the device specified by parameter DEVICE. [!]
*     RETAIN = _LOGICAL (Read)
*        Should the temporary directory containing the intermediate files
*        created by this script be retained? If not, it will be deleted
*        before the script exits. If retained, a message will be
*        displayed at the end specifying the path to the directory. [FALSE]
*     SNR = _REAL (Read)
*        The minimum ratio of the polarised intensity to the error on
*        polarised intensity for vectors to be plotted. [3.0]

*  Copyright:
*     Copyright (C) 2012 Science & Technology Facilities Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either Version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
*     02110-1301, USA.

*  Authors:
*     DSB: David S. Berry (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     15-OCT-2012 (DSB):
*        Original version
*     16-NOV-2012 (DSB):
*        Use a temporary ADAM directory for each invocation.

*-
'''


import starutil
from starutil import invoke
from starutil import NDG
from starutil import ParSys
from starutil import msg_out


try:

#  Declare the script parameters. Their positions in this list define
#  their expected position on the script command line. They can also be
#  specified by keyword on the command line. No validation of default
#  values or values supplied on the command line is performed until the
#  parameter value is first accessed within the script, at which time the
#  user is prompted for a value if necessary. The parameters "MSG_FILTER",
#  "ILEVEL", "GLEVEL" and "LOGFILE" are added automatically by the ParSys
#  constructor.
   params = []

   params.append(starutil.ParNDG("IN", "The input POL2 time series NDFs",
                                 starutil.get_task_par("DATA_ARRAY","GLOBAL")))

   params.append(starutil.Par0S("CAT", "The output FITS vector catalogue",
                                 "out.FIT"))

   params.append(starutil.ParNDG("IREF", "The reference total flux map", default=None,
                                 help="Enter a null (!) to use an artifical total flux map",
                                 minsize=0, maxsize=1 ))

   params.append(starutil.ParNDG("PI", "The output polarised intensity map",
                                 default=None, exists=False, minsize=0, maxsize=1 ))

   params.append(starutil.ParChoice("PLOT", ["P","PI"], "Quantity to define "
                                 "lengths of plotted vectors", None,
                                 noprompt=True))

   params.append(starutil.Par0F("SNR", "Polarised intensity SNR threshold for "
                                 "plotted vectors", 3.0, maxval=1000.0, minval=0.0,
                                 noprompt=True))

   params.append(starutil.Par0F("MAXLEN", "Maximum vector length to plot", None,
                                 noprompt=True))

   params.append(starutil.ParChoice("DOMAIN", ["SKY","FPLANE"], "Domain for alignment",
                                 "Sky", noprompt=True))

   params.append(starutil.Par0F("PIXSIZE", "Pixel size (arcsec)", None,
                                 maxval=1000, minval=0.01, noprompt=True))

   params.append(starutil.Par0S("CONFIG", "The cleaning config",
                                 "^$STARLINK_DIR/share/smurf/dimmconfig.lis",
                                 noprompt=True))

   params.append(starutil.Par0S("DEVICE", "Device for graphical output",
                                 starutil.get_task_par("GRAPHICS_DEVICE","GLOBAL"),
                                 noprompt=True))

   params.append(starutil.Par0L("RETAIN", "Retain temporary files?", False,
                                 noprompt=True))

#  Initialise the parameters to hold any values supplied on the command
#  line.
   parsys = ParSys( params )

#  It's a good idea to get parameter values early if possible, in case
#  the user goes off for a coffee whilst the script is running and does not
#  see a later parameter propmpt or error...

#  Get the raw POL-2 data files. They should be supplied as the first item
#  on the command line, in the form of a Starlink "group expression" (i.e.
#  the same way they are supplied to other SMURF commands such as makemap).
#  Quote the string so that it can be used as command line argument when
#  running an atask from the shell.
   indata = parsys["IN"].value

#  Get the image to use for the total flux (I) map.
   iref = parsys["IREF"].value

#  If a value was supplied for IREF, indicate that parameter PI should not
#  be prompted for. The default value (None) will then be used if no value
#  has been supplied on the command line. Then get the PI value ot use.
   if iref: parsys["PI"].noprompt = True
   pimap = parsys["PI"].value

#  Get the output catalogue now to avoid a long wait before the user gets
#  prompted for it.
   outcat = parsys["CAT"].value

#  Get the alignment domain.
   domain = parsys["DOMAIN"].value

#  Get the pixel size to use. If no pixel size is supplied we use the pixel
#  size of the total intensity map if supplied, or of the Q and U maps
#  created by SMURF:CALCQU otherwise.
   pixsize = parsys["PIXSIZE"].value
   ref = None

#  Get the cleaning config, and quote it so that any shell metacharacters are
#  passed unchanged to the SMURF:CALCQU task.
   config = starutil.shell_quote( parsys["CONFIG"].value )

#  Get the graphics device for graphical output. Normalisation plots will
#  only be produced if at least one of ILEVEL and GLEVEL is DEBUG.
   device = parsys["DEVICE"].value
   if device != None:
      device = starutil.shell_quote( device )
      if starutil.ilevel >= starutil.DEBUG or starutil.glevel >= starutil.DEBUG:
         ndevice = device
      else:
         ndevice = "!"
   else:
      device = "!"
      ndevice = "!"

#  Get the quantity to use as the vector lengths (could be "None")
   plot = parsys["PLOT"].value

#  If any vectors are to be plotted, get the SNR limit for the plotted
#  vectors.
   if plot != None:
      snr = parsys["SNR"].value
      maxlen = parsys["MAXLEN"].value

#  See if temp files are to be retained.
   retain = parsys["RETAIN"].value

#  The following call to SMURF:CALCQU creates two HDS container files -
#  one holding a set of Q NDFs and the other holding a set of U NDFs. Create
#  these container files in the NDG temporary directory.
   qcont = NDG(1)
   qcont.comment = "qcont"
   ucont = NDG(1)
   ucont.comment = "ucont"

#  Create a set of Q images and a set of U images. These are put into the HDS
#  container files "q_TMP.sdf" and "u_TMP.sdf". Each image contains Q or U
#  values derived from a short section of raw data during which each bolometer
#  moves less than half a pixel.
   msg_out( "Calculating Q and U values for each bolometer...")
   invoke("$SMURF_DIR/calcqu in={0} config={1} outq={2} outu={3} fix".
          format(indata,config,qcont,ucont) )

#  Remove spikes from the Q and U images. The cleaned NDFs are written to
#  temporary NDFs specified by two new NDG objects "qff" and "uff", which
#  inherit their size from the existing groups "qcont" and "ucont".
   msg_out( "Removing spikes from bolometer Q and U values...")
   qff = NDG(qcont)
   qff.comment = "qff"
   uff = NDG(ucont)
   uff.comment = "uff"
   invoke( "$KAPPA_DIR/ffclean in={0} out={1} box=3 clip=\[2,2,2\]"
           .format(qcont,qff) )
   invoke( "$KAPPA_DIR/ffclean in={0} out={1} box=3 clip=\[2,2,2\]"
           .format(ucont,uff) )

#  The next stuff we do independently for each subarray.
   qmos = {}
   umos = {}
   for a in ('S4A','S4B','S4C','S4D','S8A','S8B','S8C','S8D'):

#  Get an NDG object that contains the cleaned Q maps for the current
#  subarray.
      qarray = qcont.filter(a)

#  If any data was found for the current subarray...
      if qarray != None:

#  There seems to be a tendency for each bolometer to have its own fixed
#  bias in Q and U. We now try to remove these biases by removing the Q and
#  U values that are common to each image (as opposed to astronomical Q/U
#  variations, which are  fixed on the sky and so will vary from image to
#  image as the focal plane is moved on the sky). First we find the mean Q
#  value in each bolometer by averaging the Q images, aligned in PIXEL (i.e.
#  bolomter) coords. The mean Q value per bolometer is put in qcom.sdf.
         msg_out( "Removing background Q level from {0} bolometers...".format(a))
         qcom = NDG(1)
         qcom.comment = "qcom"
         invoke("$KAPPA_DIR/wcsframe {0} PIXEL".format(qarray))
         invoke("$KAPPA_DIR/wcsmosaic {0} ref=\! out={1} accept".
                format(qarray,qcom))

#  We simply assume that the fixed bolometer Q bias is linearly related to
#  the mean Q value per bolometer. Astronomical sources will affect this
#  mean to a small extent, but its probably the best we can do for now. To
#  find the gradient and offset of the linear relationship for each Q image,
#  we use kappa:normalize to do a least squares fit between each Q image and
#  the mean Q signal.
         qnm = NDG(qarray)
         qnm.comment = "qnm"
         invoke( "$KAPPA_DIR/normalize in1={0} in2={1} out={2} device={3}".
                 format(qcom,qarray,qnm,ndevice), buffer=True )

#  Now substract the normalized mean Q signal from each Q image.
         qsub = NDG(qarray)
         qsub.comment = "qsub"
         invoke( "$KAPPA_DIR/sub in1={0} in2={1} out={2}".
                 format(qarray,qnm,qsub) )

#  And then remove smaller spikes that have now become visible in the
#  Q images as a result of subtracting off the bolometer biases.
         qffb = NDG(qarray)
         qffb.comment = "qffb"
         invoke( "$KAPPA_DIR/ffclean in={0} out={1} box=3 clip=\[2,2,2\]".
                 format(qsub,qffb) )

#  Now do the same for the U images.
         msg_out( "Removing background U level from {0} bolometers...".format(a))
         uarray = ucont.filter(a)
         ucom = NDG(1)
         invoke("$KAPPA_DIR/wcsframe {0} PIXEL".format(uarray))
         invoke("$KAPPA_DIR/wcsmosaic {0} ref=\! out={1} accept".
                format(uarray,ucom))
         unm = NDG(uarray)
         unm.comment = "unm"
         invoke( "$KAPPA_DIR/normalize in1={0} in2={1} out={2} device={3}".
                 format(ucom,uarray,unm,ndevice), buffer=True )
         usub = NDG(uarray)
         usub.comment = "usub"
         invoke( "$KAPPA_DIR/sub in1={0} in2={1} out={2}".
                 format(uarray,unm,usub) )
         uffb = NDG(uarray)
         uffb.comment = "uffb"
         invoke( "$KAPPA_DIR/ffclean in={0} out={1} box=3 clip=\[2,2,2\]".
                 format(usub,uffb) )

#  Ensure all images have the required current WCS Frame (as indicated by
#  the DOMAIN parameter).
         invoke( "$KAPPA_DIR/wcsframe ndf={0} frame={1}".format(qffb,domain) )
         invoke( "$KAPPA_DIR/wcsframe ndf={0} frame={1}".format(uffb,domain) )

#  The reference map defines the output pixel grid - the origin, pixel size,
#  sky projection, etc (but not the pixel bounds) - of the final Q, U and I
#  maps. If we do not yet have such a reference map, create one now.
         if not ref:
            msg_out( "Creating reference map...")

#  Assume initially that we should use the first Q map as the reference.
#  Since it was created by CALCQU, we know that this will contain the
#  POLANAL WCS Frame that POLPACK uses to define the polarimetric reference
#  direction.
            ref = NDG( qffb[0] )

#  If a total flux reference map was supplied, we want to use it to
#  define the output pixel grid, but we need to make sure it has a POLANAL
#  Frame that describes the polarimetric reference direction used by CALCQU.
#  To do this we resample the Q map into the coordinate system of the total
#  flux map, and then use the resampled Q map as the reference.
            if iref:
               tmp = NDG( 1 )
               invoke( "$KAPPA_DIR/wcsalign in={0} out={1} ref={2} method=near lbnd=!".format(ref,tmp,iref) )
               ref = tmp

#  If a specific pixel size was requested, squash or stretch the
#  reference map so that it has the requested pixel size.
            if pixsize:
               tmp = NDG(1)
               invoke( "$KAPPA_DIR/sqorst in={0} out={1} mode=pixelscale piscale=\'{2}\,{2}\'".
                       format(ref,tmp,pixscale) )
               ref = tmp

#  Combine all the Q images for the current sub-array together into a single
#  image. We use a bilinear pixel-pasting scheme here, but maybe SINCSINC
#  would be better?
         msg_out( "Combining all Q images for {0} into a single map...".format(a))
         qmos[a] = NDG(1)
         invoke( "$KAPPA_DIR/wcsmosaic method=bilin in={0} ref={1} out={2} genvar=yes accept".
                 format(qffb,ref,qmos[a]) )

#  Combine all the U images together into a single image.
         msg_out( "Combining all U images for {0} into a single map...".format(a))
         umos[a] = NDG(1)
         invoke( "$KAPPA_DIR/wcsmosaic method=bilin in={0} ref={1} out={2} genvar=yes accept".
                 format(uffb,ref,umos[a]) )

#  All sub-arrays are done. Now combine the Q images for all sub-arrays
#  together into a single image. First form a new NDG containing all the
#  individual sub-array Q maps, and another one holding the name of the
#  total Q map.
   msg_out( "Combining all Q and U images for all sub-arrays...")

   qmos_list = []
   for c in qmos.values():
      qmos_list.append(c)
   qmos_all = NDG( qmos_list )
   qtotal = NDG( 1 )
   invoke( "$KAPPA_DIR/wcsmosaic method=bilin in={0} ref={1} out={2} lbnd=!".format(qmos_all,ref,qtotal) )

#  Do the same for U.
   umos_list = []
   for c in umos.values():
      umos_list.append(c)
   umos_all = NDG( umos_list )
   utotal = NDG( 1 )
   invoke( "$KAPPA_DIR/wcsmosaic method=bilin in={0} ref={1} out={2} lbnd=!".format(umos_all,ref,utotal) )

#  If no total intensity map was supplied, generate an artificial I image that
#  is just equal to the polarised intensity image. This is needed because
#  polpack:polvec uses the I value to normalise the Q and U values prior to
#  calculating the polarised intensity and angle.
   if iref == None:
      iref = NDG(1)
      msg_out( "Generating an artificial total intensity image...")
      invoke( "$KAPPA_DIR/maths exp='sqrt(ia**2+ib**2)' ia={0} ib={1} out={2}".format(qtotal,utotal,iref))

#  If a total intensity map was supplied, and we are not using the
#  default pixel size (i.e. the pixel size in the total intensity map),
#  aligned the total intensity map with the reference image.
   elif pixsize:
      tmp = NDG( 1 )
      invoke( "$KAPPA_DIR/wcsalign in={0} out={1} ref={2} method=bilin lbnd=!".format(iref,tmp,ref) )
      iref = tmp

#  Ensure the Q U and I images all have the same bounds, equal to the
#  overlap region between them. To get the overlap region, use MATHS to
#  add them together. Then use ndfcopy to produce the sections from each,
#  which match the overlap area.
   tmp = NDG( 1 )
   invoke( "$KAPPA_DIR/maths exp='ia+ib+ic' ia={0} ib={1} ic={2} out={3}".format(qtotal,utotal,iref,tmp) )
   qtrim = NDG( 1 )
   invoke( "$KAPPA_DIR/ndfcopy {0} like={1} out={2}".format(qtotal,tmp,qtrim) )
   utrim = NDG( 1 )
   invoke( "$KAPPA_DIR/ndfcopy {0} like={1} out={2}".format(utotal,tmp,utrim) )
   itrim = NDG( 1 )
   invoke( "$KAPPA_DIR/ndfcopy {0} like={1} out={2}".format(iref,tmp,itrim) )

#  The polarisation vectors are calculated by the polpack:polvec command,
#  which requires the input Stokes vectors in the form of a 3D cube. Paste
#  the 2-dimensional Q, U and I images into a 3D cube.
   planes = NDG( [qtrim,utrim,itrim] )
   cube = NDG( 1 )
   invoke( "$KAPPA_DIR/paste in={0} shift=\[0,0,1\] out={1}".format(planes,cube))

#  Check that the cube has a POLANAL frame, as required by POLPACK.
   try:
      invoke( "$KAPPA_DIR/wcsframe {0} POLANAL".format(cube) )

#  If it does not, see if it has a "POLANAL-" Frame (kappa:paste can
#  cause this by appending "-" to the end of the domain name to account for
#  the extra added 3rd axis).
   except AtaskError:
      invoke( "$KAPPA_DIR/wcsframe {0} POLANAL-".format(cube) )

#  We only arrive here if the POLANAL- frame was found, so rename it to POLANAL
      invoke( "$KAPPA_DIR/wcsattrib {0} set domain POLANAL".format(cube) )

#  Re-instate the required current Frame
   invoke( "$KAPPA_DIR/wcsframe {0} {1}".format(cube,domain) )

#  POLPACK needs to know the order of I, Q and U in the 3D cube. Store
#  this information in the POLPACK enstension within "cube.sdf".
   invoke( "$POLPACK_DIR/polext {0} stokes=qui".format(cube) )

#  Create a FITS catalogue containing the polarisation vectors.
   command = "$POLPACK_DIR/polvec {0} cat={1}".format(cube,outcat)
   if pimap:
      command = "{0} ip={1}".format(command,pimap)
      msg_out( "Creating the output catalogue {0} and polarised intensity map {1}...".format(outcat,pimap) )
   else:
      msg_out( "Creating the output catalogue: {0}...".format(outcat) )
   msg = invoke( command )
   msg_out( "\n{0}\n".format(msg) )

#  If required, produce a vector plot.
   if plot != None:
      msg_out( "Plotting the '{0}' vectors ...".format(plot) )

#  Select vectors where the signal to noise ratio for the Polarised
#  Intentisy value is more than 3. The catalogue is stored in the NDG
#  temporary directory.
      exp = "pi>{0}*dpi".format(snr)
      if maxlen != None:
         exp += "&{0}<{1}".format(plot,maxlen)
      selcat = "{0}/selcat".format(NDG.tempdir)
      invoke( "$CURSA_DIR/catselect catin={0} catout={1} norejcat seltyp=e expr='{2}'".format(outcat,selcat,exp))

#  If a total intensity reference was supplied, display it as a background
#  image, and plot the vectors over the top.
      if iref:
         invoke( "$KAPPA_DIR/lutable mapping=linear coltab=grey device={0}".format(device), buffer=True)
         invoke( "$KAPPA_DIR/display {0}'(,,3)' mode=perc percentiles=\[1,99\] badcol=black device={1}".format(cube,device), buffer=True)
         invoke( "$POLPACK_DIR/polplot {0} clear=no axes=no colmag={1} key=yes style='colour=red' device={2}".format(selcat,plot,device), buffer=True)

#  Otherwise, just plot the vectors.
      else:
         invoke( "$POLPACK_DIR/polplot {0} colmag={1} key=yes style=def device={2}".format(selcat,plot,device), buffer=True)

#  Delete all temporary NDFs etc
   if retain:
      msg_out( "Retaining temporary files in {0}".format(NDG.tempdir))
   else:
      NDG.cleanup()

#  Delete the temporary ADAM directory.
   ParSys.cleanup()

#  If an StarUtilError of any kind occurred, display the message but hide the
#  python traceback. To see the trace back, uncomment "raise()" instead.
except starutil.StarUtilError as err:
#  raise()
   print( err )

