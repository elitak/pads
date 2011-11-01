/***********************************************************************
*                                                                      *
*              This software is part of the pads package               *
*           Copyright (c) 2005-2007 AT&T Knowledge Ventures            *
*                      and is licensed under the                       *
*                        Common Public License                         *
*                      by AT&T Knowledge Ventures                      *
*                                                                      *
*                A copy of the License is available at                 *
*                    www.padsproj.org/License.html                     *
*                                                                      *
*  This program contains certain software code or other information    *
*  ("AT&T Software") proprietary to AT&T Corp. ("AT&T").  The AT&T     *
*  Software is provided to you "AS IS". YOU ASSUME TOTAL RESPONSIBILITY*
*  AND RISK FOR USE OF THE AT&T SOFTWARE. AT&T DOES NOT MAKE, AND      *
*  EXPRESSLY DISCLAIMS, ANY EXPRESS OR IMPLIED WARRANTIES OF ANY KIND  *
*  WHATSOEVER, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF*
*  MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, WARRANTIES OF  *
*  TITLE OR NON-INFRINGEMENT.  (c) AT&T Corp.  All rights              *
*  reserved.  AT&T is a registered trademark of AT&T Corp.             *
*                                                                      *
*                   Network Services Research Center                   *
*                          AT&T Labs Research                          *
*                           Florham Park NJ                            *
*                                                                      *
*                 Robert Gruber <bob.gruber@gmail.com>                 *
*              Kathleen Fisher <kfisher@research.att.com>              *
*            Yitzhak Mandelbaum <yitzhakm@cs.princeton.edu>            *
*                                                                      *
***********************************************************************/
/*
 * CONFIG file for pads
 *   
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#ifndef __PADS_CONFIG_H__
#define __PADS_CONFIG_H__

/* Normal users of the pads library should not modify this file.
 *
 * Power users worried about the footprint of the library can 
 * assign 0 to some of the following defines to obtain a smaller library.
 * After modifying this file, the library can be rebuilt using:
 *
 * cd $PADS_HOME/padsc/libpads
 * gmake clean
 * gmake install
 */

/* The following control whether ANY read, write, or accum functions are included */

#define P_CONFIG_READ_FUNCTIONS   1 /* need read functions */
#define P_CONFIG_WRITE_FUNCTIONS  1 /* need write functions */
#define P_CONFIG_ACCUM_FUNCTIONS  1 /* need accum functions */

/* The following control whether functions for specific external reps
 * are included.
 * 
 * IMPORTANT: If you disable an ASCII or EBCDIC type, then the
 * corresponding default type (which uses pads->disc->char_set to
 * choose the actual type) will not be available.  For example, if you
 * set P_CONFIG_A_INT to 0, then this will not only cause ASCII
 * integer read functions (such as Pa_int8_read) to be omitted, it
 * also causes the default integer function (such as Pint8_read) to
 * be ommitted
 */

#define P_CONFIG_A_CHAR_STRING    1 /* need all ASCII character and string-based functions   */
#define P_CONFIG_E_CHAR_STRING    1 /* need all EBCDIC character and string-based functions  */
 
#define P_CONFIG_A_INT            1 /* need ASCII-based variable-width integers */
#define P_CONFIG_A_INT_FW         1 /* need ASCII-based fixed-width integers */

#define P_CONFIG_A_FLOAT          1 /* need ASCII-based variable-width floats */
#define P_CONFIG_A_FLOAT_FW       1 /* need ASCII-based fixed-width floats */

#define P_CONFIG_E_INT            1 /* need EBCDIC-based variable-width integers */
#define P_CONFIG_E_INT_FW         1 /* need EBCDIC-based fixed-width integers */

#define P_CONFIG_E_FLOAT          1 /* need EBCDIC-based variable-width floats */
#define P_CONFIG_E_FLOAT_FW       1 /* need EBCDIC-based fixed-width floats */

#define P_CONFIG_B_INT            1 /* need raw binary integers */ 

#define P_CONFIG_EBC_INT          1 /* need EBC encoding integers */ 
#define P_CONFIG_BCD_INT          1 /* need BCD encoding integers */ 
#define P_CONFIG_SBL_INT          1 /* need SBL encoding integers */ 
#define P_CONFIG_SBH_INT          1 /* need SBH encoding integers */ 

#define P_CONFIG_EBC_FPOINT       1 /* need EBC encoding fixed-points */ 
#define P_CONFIG_BCD_FPOINT       1 /* need BCD encoding fixed-points */ 
#define P_CONFIG_SBL_FPOINT       1 /* need SBL encoding fixed-points */ 
#define P_CONFIG_SBH_FPOINT       1 /* need SBH encoding fixed-points */ 

#endif   /*   ! __PADS_CONFIG_H__  */
