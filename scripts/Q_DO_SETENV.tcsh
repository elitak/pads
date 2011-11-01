#!/bin/echo You should SOURCE rather than execute
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
#
# Q_DO_SETENV.tcsh is for use with csh/tcsh
# It is the 'quite' version of DO_SETENV.tcsh
# From the same directory as Q_DO_SETENV.tcsh, do:
#      source Q_DO_SETENV.tcsh
#

set _pads_status = OK
set _pads_do_prog = Q_DO_SETENV.tcsh

if (! $?PADS_HOME) then
  set _pads_status = FAILED
else
  if ("$PADS_HOME"x == x) then
    set _pads_status = FAILED
  endif
endif

if ($_pads_status == "FAILED") then
  echo "##############################################################################"
  echo "# Set env var PADS_HOME and then use $_pads_do_prog again."
  echo "##############################################################################"
  echo " "
endif

if ($_pads_status == "OK") then
  set _pads_verbose = 0
  set _pads_use_nmake = 0
  source $PADS_HOME/scripts/INT_DO_SETENV.tcsh
  unset _pads_use_nmake
  unset _pads_verbose
endif

unset _pads_do_prog
unset _pads_status
