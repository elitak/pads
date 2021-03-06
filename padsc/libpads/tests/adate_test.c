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
 *  adate_test: Test adate_read
 */


#include "pads-internal.h" /* for testing - normally do not include internal */

P_NOGEN

int main(int argc, char** argv) {
  P_t*            pads;
  Pio_disc_t*     io_disc;
  Pbase_m         m = P_CheckAndSet;
  Pbase_pd        pd;
  size_t          bytes_skipped;
  unsigned long   ultmp;
  Puint32         tm;
  Pchar           lit;

  error(0, "\nUsing PADSC IO discipline nlrec\n\n");
  io_disc = P_nlrec_make(0);

  if (P_ERR == P_libopen(&pads, 0, io_disc, 1)) {
    error(2, "*** P_libopen failed ***");
    exit(-1);
  }
  if (P_ERR == P_io_fopen(pads, "../../data/ex_data.adate_test")) {
    error(2, "*** P_io_fopen failed ***");
    exit(-1);
  }

  /*
   * XXX Process the data here XXX
   */
  while (1) {
    if (P_io_at_eof(pads)) {
      error(0, "Main program found eof");
      break;
    }
    /* try to read line with 1 date term by vbar, 1 date term by EOR */
    if (P_ERR == Pa_date_read(pads, &m, '|', &pd, &tm)) {
      if (pd.errCode != P_INVALID_DATE) {
	goto find_EOR;
      }
    } else {
      error(0, "Read date term by vbar: %s (secs = %lu)", fmttime("%K", (time_t)tm), (unsigned long)tm);
    }
    if (P_ERR == Pa_char_lit_read(pads, &m,  '|', &pd, &lit)) {
      PDCI_report_err (pads, 0, &pd.loc, pd.errCode, 0, 0);
      goto find_EOR;
    }
    if (P_ERR == Pa_date_read(pads, &m, 0, &pd, &tm)) {
      if (pd.errCode != P_INVALID_DATE) {
	goto find_EOR;
      }
    } else {
      error(0, "Read date term by NULL: %s (secs = %lu)", fmttime("%K", (time_t)tm), (unsigned long)tm);
    }
  find_EOR:
    if (P_ERR == P_io_next_rec(pads, &bytes_skipped)) {
      error(2, "Could not find EOR (newline), ending program");
      goto done;
    }
    ultmp = bytes_skipped;
    error(0, "bytes_skipped to find EOR/newline = %ld", ultmp);
  }

 done:

  if (P_ERR == P_io_close(pads)) {
    error(2, "*** P_io_close failed ***");
    exit(-1);
  }

  if (P_ERR == P_close(pads)) {
    error(2, "*** P_close failed ***");
    exit(-1);
  }

  return 0;
}
