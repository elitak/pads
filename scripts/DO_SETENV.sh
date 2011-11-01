########################################################################
#                                                                      #
#              This software is part of the pads package               #
#           Copyright (c) 2005-2007 AT&T Knowledge Ventures            #
#                      and is licensed under the                       #
#                        Common Public License                         #
#                      by AT&T Knowledge Ventures                      #
#                                                                      #
#                A copy of the License is available at                 #
#                    www.padsproj.org/License.html                     #
#                                                                      #
#  This program contains certain software code or other information    #
#  ("AT&T Software") proprietary to AT&T Corp. ("AT&T").  The AT&T     #
#  Software is provided to you "AS IS". YOU ASSUME TOTAL RESPONSIBILITY#
#  AND RISK FOR USE OF THE AT&T SOFTWARE. AT&T DOES NOT MAKE, AND      #
#  EXPRESSLY DISCLAIMS, ANY EXPRESS OR IMPLIED WARRANTIES OF ANY KIND  #
#  WHATSOEVER, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF#
#  MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, WARRANTIES OF  #
#  TITLE OR NON-INFRINGEMENT.  (c) AT&T Corp.  All rights              #
#  reserved.  AT&T is a registered trademark of AT&T Corp.             #
#                                                                      #
#                   Network Services Research Center                   #
#                          AT&T Labs Research                          #
#                           Florham Park NJ                            #
#                                                                      #
#              Kathleen Fisher <kfisher@research.att.com>              #
#                 Robert Gruber <bob.gruber@gmail.com>                 #
#                                                                      #
########################################################################
# DO_SETENV.sh is for use with sh/ksh/bash.
# From the same directory as DO_SETENV.sh, do:
#     . ./DO_SETENV.sh
#

_pads_do_program=DO_SETENV.sh
_pads_status=OK 

if [ "$PADS_HOME"x = x ]; then
  echo "##############################################################################"
  echo "# Set env var PADS_HOME and then use DO_SETENV.sh again."
  echo "##############################################################################"
  echo " "
  _pads_status=FAILED
fi

if [ $_pads_status = "OK" ]; then
  _pads_verbose=1
  _pads_use_nmake=0
  . $PADS_HOME/scripts/INT_DO_SETENV.sh
  _pads_use_nmake=
  _pads_verbose=
fi

_pads_status=
_pads_do_program=
