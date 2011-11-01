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
## This source file is run through mksrc.pl to produce 
## a number of generated files:
##
##    pads-macros-gen.h       : generally useful macros
##    pads-read-macros-gen.h  : macros that help implement read  functions
##    pads-write-macros-gen.h : macros that help implement write functions
##    pads-acc-macros-gen.h   : macros that help implement accum functions
##    pads-misc-macros-gen.h  : macros that help implement misc  functions
## 
##    pads-read-gen.c         : generated read  functions
##    pads-write-gen.c        : generated write functions
##    pads-acc-gen.c          : generated accum functions
##    pads-misc-gen.c         : generated misc  functions
##    pads-gen.c              : the rest of the padsc library

##   Macros in this file need to end with a comment END_MACRO on its own line.
##
##
/* ********************* BEGIN_MACROS(pads-macros-gen.h) ********************** */
/*
 * Some generally useful macros
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

/* ********************************** END_HEADER ********************************** */

/* ================================================================================ */
/* MACROS USED BY READ FUNCTIONS */

#define PDCI_READFN_PD_INIT(pads, pd)
do {
  Pbase_pd_init_no_err(pd);
  PDCI_READFN_BEGINLOC(pads, (pd)->loc);
  PD_PGLX_INIT(pads,pd);
} while (0)
/* END_MACRO */



#define PDCI_FINISH_TIMESTAMP_READ(m_IN, format_IN, tzone_IN, errcode_IN, just_time_IN)
do {
  if (P_Test_Set(*(m_IN))) {
    time_t       tm;
    Pbyte       *tmp;
    char        *tmp_t;
    time_t       now;


    PDCI_STR_PRESERVE(s); /* this ensures s.str is null terminated */
    now = (just_time_IN) ? 0 : time(NiL);
    /* tm = tmdate(s->str, (char**)&tmp, NiL); */
    tmset(tzone_IN);
    tm = tmscan(s->str, (char**)&tmp, format_IN, &tmp_t, &now, 0L);
    if (!tmp_t || *tmp_t || !tmp || *tmp) {
      PDCI_READFN_BEGINLOC_MINUSK(pads, pd->loc, s->len);
      PDCI_READFN_ENDLOC_MINUS1(pads, pd->loc);
      PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, errcode_IN);
    }
    (*res_out) = tm;
    /* normally we do a tmset(pads->out_zone) before using fmttime, */
    /* but here for debugging purposes we output using tzone_IN */ 
    P_DBG4(pads->disc, "%s: converted string %s => %s (secs = %ld)",
	   whatfn, P_qfmt_str(s), fmttime("%K", (time_t)tm), (long)tm);
  };
  PDCI_ECHO_TOKEN_AS_PSTR(pads, whatfn,s);
  return P_OK;
 } while (0);
 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR)
/* END_MACRO */

#define PDCI_FINISH_DATE_READ(m_IN, format_IN, tzone_IN, errcode_IN, just_time_IN)
do {
  if (P_Test_Set(*(m_IN))) {
    time_t       tm;
    Pbyte       *tmp;
    char        *tmp_t;
    time_t       now;
    Tm_t        *struct_tm_ptr;

    PDCI_STR_PRESERVE(s); /* this ensures s.str is null terminated */
    now = (just_time_IN) ? 0 : time(NiL);
    /* tm = tmdate(s->str, (char**)&tmp, NiL); */
    tmset(tzone_IN);
    tm = tmscan(s->str, (char**)&tmp, format_IN, &tmp_t, &now, 0L);
    if (!tmp_t || *tmp_t || !tmp || *tmp) {
      PDCI_READFN_BEGINLOC_MINUSK(pads, pd->loc, s->len);
      PDCI_READFN_ENDLOC_MINUS1(pads, pd->loc);
      PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, errcode_IN);
    }
    /* replace time portion with midnight */
    struct_tm_ptr = tmmake(&tm);
    struct_tm_ptr->tm_hour  = 0;
    struct_tm_ptr->tm_min   = 0;
    struct_tm_ptr->tm_sec   = 0;
    tm = tmtime(struct_tm_ptr, 0);

    (*res_out) = tm;
    /* normally we do a tmset(pads->out_zone) before using fmttime, */
    /* but here for debugging purposes we output using tzone_IN */ 
    P_DBG4(pads->disc, "%s: converted string %s => %s (secs = %ld)",
	   whatfn, P_qfmt_str(s), fmttime("%K", (time_t)tm), (long)tm);
  }
  PDCI_ECHO_TOKEN_AS_PSTR(pads, whatfn,s);
  return P_OK;
 } while (0);
 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR)
/* END_MACRO */

#define PDCI_FINISH_TIME_READ(m_IN, format_IN, tzone_IN, errcode_IN, just_time_IN)
do {
  if (P_Test_Set(*(m_IN))) {
    time_t       tm;
    Pbyte       *tmp;
    char        *tmp_t;
    time_t       now;
    Tm_t        *struct_tm_ptr;

    PDCI_STR_PRESERVE(s); /* this ensures s.str is null terminated */
    now = (just_time_IN) ? 0 : time(NiL);
    /* tm = tmdate(s->str, (char**)&tmp, NiL); */
    tmset(tzone_IN);
    tm = tmscan(s->str, (char**)&tmp, format_IN, &tmp_t, &now, 0L);
    if (!tmp_t || *tmp_t || !tmp || *tmp) {
      PDCI_READFN_BEGINLOC_MINUSK(pads, pd->loc, s->len);
      PDCI_READFN_ENDLOC_MINUS1(pads, pd->loc);
      PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, errcode_IN);
    }
    /* replace date portion with 1-jan-1970 */
    struct_tm_ptr = tmmake(&tm);
    struct_tm_ptr->tm_mday  = 1;    /* 1-   */
    struct_tm_ptr->tm_mon   = 0;    /* jan- */
    struct_tm_ptr->tm_year  = 70;   /* 1970 */
    tm = tmtime(struct_tm_ptr, 0);

    (*res_out) = tm;
    /* normally we do a tmset(pads->out_zone) before using fmttime, */
    /* but here for debugging purposes we output using tzone_IN */ 
    P_DBG4(pads->disc, "%s: converted string %s => %s (secs = %ld)",
	   whatfn, P_qfmt_str(s), fmttime("%K", (time_t)tm), (long)tm);
  }
  PDCI_ECHO_TOKEN_AS_PSTR(pads, whatfn,s);
  return P_OK;
 } while (0);
 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR)
/* END_MACRO */

/*
 * Starting alloc size for strings, even if initial string is smaller;
 * saves on later alloc calls when Pstring field is re-used many
 * times with strings of different lengths.
 */ 

#define PDCI_STRING_HINT 8

/* END_MACRO */

/* Pstring_cstr_copy -- inline version.  Caller must provide fatal_alloc_err target */
#define PDCI_STR_CPY(sIN, bIN, wIN)
  do {
    size_t wdth_PDCI_STR_CPY = (wIN);
    if (!(sIN)->rbuf) {
      if (!((sIN)->rbuf = RMM_new_rbuf(pads->rmm_nz))) {
	goto fatal_alloc_err;
      }
    }
    if (RBuf_RESERVE_HINT((sIN)->rbuf, (sIN)->str, char, wdth_PDCI_STR_CPY+1, PDCI_STRING_HINT)) {
      goto fatal_alloc_err;
    }
    memcpy((sIN)->str, bIN, wdth_PDCI_STR_CPY);
    (sIN)->str[wdth_PDCI_STR_CPY] = 0;
    (sIN)->len = wdth_PDCI_STR_CPY;
    /* if ((sIN)->sharing) { P_WARN1(pads->disc, "XXX_REMOVE copy: string %p is no longer sharing", (void*)(sIN)); } */
    (sIN)->sharing = 0;
  } while (0)
/* END_MACRO */

/* copy and convert from ASCII to EBCDIC at same time.  Caller must provide fatal_alloc_err target */
#define PDCI_A2E_STR_CPY(sIN, bIN, wIN)
  do {
    int i;
    size_t wdth_PDCI_A2E_STR_CPY = (wIN);
    if (!(sIN)->rbuf) {
      if (!((sIN)->rbuf = RMM_new_rbuf(pads->rmm_nz))) {
	goto fatal_alloc_err;
      }
    }
    if (RBuf_RESERVE_HINT((sIN)->rbuf, (sIN)->str, char, wdth_PDCI_A2E_STR_CPY+1, PDCI_STRING_HINT)) {
      goto fatal_alloc_err;
    }
    for (i = 0; i < wdth_PDCI_A2E_STR_CPY; i++) {
      (sIN)->str[i] = P_mod_ae_tab[(int)((bIN)[i])];
    }
    (sIN)->str[wdth_PDCI_A2E_STR_CPY] = 0;
    (sIN)->len = wdth_PDCI_A2E_STR_CPY;
    /* if ((sIN)->sharing) { P_WARN1(pads->disc, "XXX_REMOVE copy: string %p is no longer sharing", (void*)(sIN)); } */
    (sIN)->sharing = 0;
  } while (0)
/* END_MACRO */

/* copy and convert from EBCDIC to ASCII at same time.  Caller must provide fatal_alloc_err target */
#define PDCI_E2A_STR_CPY(sIN, bIN, wIN)
  do {
    int i;
    size_t wdth_PDCI_E2A_STR_CPY = (wIN);
    if (!(sIN)->rbuf) {
      if (!((sIN)->rbuf = RMM_new_rbuf(pads->rmm_nz))) {
	goto fatal_alloc_err;
      }
    }
    if (RBuf_RESERVE_HINT((sIN)->rbuf, (sIN)->str, char, wdth_PDCI_E2A_STR_CPY+1, PDCI_STRING_HINT)) {
      goto fatal_alloc_err;
    }
    for (i = 0; i < wdth_PDCI_E2A_STR_CPY; i++) {
      (sIN)->str[i] = P_mod_ea_tab[(int)((bIN)[i])];
    }
    (sIN)->str[wdth_PDCI_E2A_STR_CPY] = 0;
    (sIN)->len = wdth_PDCI_E2A_STR_CPY;
    /* if ((sIN)->sharing) { P_WARN1(pads->disc, "XXX_REMOVE copy: string %p is no longer sharing", (void*)(sIN)); } */
    (sIN)->sharing = 0;
  } while (0)
/* END_MACRO */

/* Fill string s with n copies of c.  Caller must provide fatal_alloc_err target */
#define PDCI_STRFILL(sIN, cIN, nIN)
  do {
    if (!(sIN)->rbuf) {
      if (!((sIN)->rbuf = RMM_new_rbuf(pads->rmm_nz))) {
	goto fatal_alloc_err;
      }
    }
    if (RBuf_RESERVE_HINT((sIN)->rbuf, (sIN)->str, char, (nIN)+1, PDCI_STRING_HINT)) {
      goto fatal_alloc_err;
    }
    memset((sIN)->str, (cIN), (nIN));
    (sIN)->str[nIN] = 0;
    (sIN)->len = (nIN);
    /* if ((sIN)->sharing) { P_WARN1(pads->disc, "XXX_REMOVE fill: string %p is no longer sharing", (void*)(sIN)); } */
    (sIN)->sharing = 0;
  } while (0)
/* END_MACRO */

/* Pstring_preserve -- inline version.  Caller must provide fatal_alloc_err target */
#define PDCI_STR_PRESERVE(sIN)
  do {
    char *shared_str;
    /* P_WARN3(pads->disc, "XXX_REMOVE [%s:%d] preserve called on shared string %p", __FILE__, __LINE__, (void*)(sIN)); */
    /* if (!(sIN)->sharing) { P_WARN3(pads->disc, "XXX_REMOVE [%s:%d] ... but string %p was not shared",__FILE__, __LINE__, (void*)(sIN)); } */
    if ((sIN)->sharing) {
      shared_str = (sIN)->str;
      PDCI_STR_CPY((sIN), shared_str, (sIN)->len);
    }
  } while (0)
/* END_MACRO */

/* Set up str sharing */
#define PDCI_STR_SHARE(sIN, bIN, wIN)
  do {
    (sIN)->str = (char*)(bIN);
    (sIN)->len = (wIN);
    (sIN)->sharing = 1;
    /* P_WARN1(pads->disc, "XXX_REMOVE string %p is now sharing", (void*)(sIN)); */
  } while (0)
/* END_MACRO */

/* If P_Test_Set(*(mIN)), point to or copy (depending on pads->disc->copy_strings)
 * the string bIN of width wIN
 * Caller must provide fatal_alloc_err target
 */
#define PDCI_A_STR_SET(mIN, sIN, bIN, wIN)
  do {
    if (P_Test_Set(*(mIN))) {
      if (pads->disc->copy_strings) {
	PDCI_STR_CPY(sIN, bIN, wIN);
      } else {
	PDCI_STR_SHARE(sIN, bIN, wIN);
      }
    }
  } while (0)
/* END_MACRO */

/* If P_Test_Set(*(mIN)), copy string of width wIN.
 * Caller must provide fatal_alloc_err target
 */
#define PDCI_STR_CPY_ON_SET(mIN, sIN, bIN, wIN)
  do {
    if (P_Test_Set(*(mIN))) {
      PDCI_STR_CPY(sIN, bIN, wIN);
    }
  } while (0)
/* END_MACRO */

/* If P_Test_Set(*(mIN)), copy (always copy for EBCDIC) the
 * string bIN of width wIN and convert copy to ASCII.
 * Caller must provide fatal_alloc_err target.
 */
#define PDCI_E_STR_SET(mIN, sIN, bIN, wIN)
  do {
    if (P_Test_Set(*(mIN))) {
      PDCI_E2A_STR_CPY(sIN, bIN, wIN);
    }
  } while (0)
/* END_MACRO */

/* ----------------------------------------------------------- */
/* The following cases either have a hard or soft goal for max */
/* bytes (a soft goal means it is OK to produce more bytes if  */
/* they happen to be available in the current elt).            */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_PREAMBLE
  PDCI_stkElt_t    *tp              = &(pads->stack[pads->top]);
  Pio_elt_t        *elt             = tp->elt;
  Pio_elt_t        *keep_elt;
  Pio_elt_t        *next_elt;
  Pbyte            *begin;
  Pbyte            *end;
  size_t            bytes           = tp->remain;
  int               eor;
  int               eof
/* END_MACRO */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_MATCH
  PDCI_IO_NEED_BYTES_SETUP_PREAMBLE;
  int               soft_goal       = 0;
  size_t            goal_bytes      = pads->disc->match_max;
  if (!goal_bytes && !pads->disc->io_disc->rec_based) {
    soft_goal  = 1;
    goal_bytes = P_BUILTIN_MATCH_MAX;
  }
/* END_MACRO */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_SCAN
  PDCI_IO_NEED_BYTES_SETUP_PREAMBLE;
  int               soft_goal       = 0;
  size_t            goal_bytes      = pads->disc->scan_max;
  if (!goal_bytes && !pads->disc->io_disc->rec_based) {
    soft_goal  = 1;
    goal_bytes = P_BUILTIN_SCAN_MAX;
  }
/* END_MACRO */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_PANIC
  PDCI_IO_NEED_BYTES_SETUP_PREAMBLE;
  int               soft_goal       = 0;
  size_t            goal_bytes      = pads->disc->panic_max;
  if (!goal_bytes && !pads->disc->io_disc->rec_based) {
    soft_goal  = 1;
    goal_bytes = P_BUILTIN_PANIC_MAX;
  }
/* END_MACRO */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic)
  PDCI_IO_NEED_BYTES_SETUP_PREAMBLE;
  int               soft_goal       = 0;
  size_t            goal_bytes;
  if (panic) {
    goal_bytes = pads->disc->panic_max;
    if (!goal_bytes && !pads->disc->io_disc->rec_based) {
      soft_goal  = 1;
      goal_bytes = P_BUILTIN_PANIC_MAX;
    }
  } else {
    goal_bytes = pads->disc->scan_max;
    if (!goal_bytes && !pads->disc->io_disc->rec_based) {
      soft_goal  = 1;
      goal_bytes = P_BUILTIN_SCAN_MAX;
    }
  }
/* END_MACRO */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_NUMERIC
  PDCI_IO_NEED_BYTES_SETUP_PREAMBLE;
  int               soft_goal       = 0;
  size_t            goal_bytes      = pads->disc->numeric_max;
  if (!goal_bytes && !pads->disc->io_disc->rec_based) {
    soft_goal  = 1;
    goal_bytes = P_BUILTIN_NUMERIC_MAX;
  }
/* END_MACRO */

/* The IOerrorSTMT statement must be/include goto or return; it */
/* is assumed NOT to fall through.  Further, begin/end/etc are  */
/* NOT set properly if IOerrorSTMT is used.                     */
/* Invariant: at exit, begin <= end.*/
#define PDCI_IO_NEED_BYTES(IOerrorSTMT)
do {
  while (!(elt->eor|elt->eof) && (soft_goal || !goal_bytes || bytes < goal_bytes)) {
    /* try for more bytes */
    if (elt->next != pads->head) { /* the easy case */
      elt = elt->next;
      bytes += elt->len;
      continue;
    }
    /* elt->next is pads->head, getting more bytes requires use of read_fn */
    /* if soft goal and it has been hit, stop now */
    if (soft_goal && (bytes >= goal_bytes)) break;
    keep_elt = pads->stack[0].elt;
    if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
      IOerrorSTMT;
    }
    elt = elt->next;
    bytes += elt->len;
  }
  /* Why isn't this line before the while loop? 
     Can the while loop effect the contents of tp? 
     -- YHM
   */
  begin = tp->elt->end - tp->remain;
  /* either we hit eor or eof, or we found >= goal_bytes bytes */
  if (soft_goal && (bytes > goal_bytes)) {
    /* adjust goal upwards rather than not returning all the bytes we found */
    goal_bytes = bytes;
  }
  if (!goal_bytes || elt->eor) {
    /* Goal was eor and we got eor|eof, OR goal was set but we found eor first,
     * which is also acceptable as a goal. Either way, include all bytes found.
     * Since this is record-based case, set goal > end if !eor
     */
    end = elt->end;
    eor = elt->eor;
    eof = elt->eof;
  } else {
    /* had hard or soft goal_bytes (> 0) and eor not found.  eof may have been found */
    eor = 0;
    if (bytes > goal_bytes) {
      /* found more than enough (end will be prior to any eof byte) */
      end  = begin + goal_bytes;
      eof  = 0;
    } else if (bytes == goal_bytes) {
      /* found exactly enough (not sure if end is at eof) */
      end  = begin + goal_bytes;
      eof  = (end == elt->end) ? elt->eof : 0;
    } else {
      /* did not find enough (must have hit eof) */
      end  = elt->end;
      eof  = 1;
    }
  }
} while (0)
/* END_MACRO */

/* ----------------------------------------------------------- */
/* For the case where we specify a specific max bytes          */
/* greater than zero, which is a hard limit, or zero, which    */
/* uses eor as the hard limit (must be record-based to use 0). */ 

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_BYTES_SETUP_SPECIFIC
  PDCI_IO_NEED_BYTES_SETUP_PREAMBLE
/* END_MACRO */

/* The IOerrorSTMT statement must be/include goto or return; it */
/* is assumed NOT to fall through.  Further, begin/end/etc are  */
/* NOT set properly if IOerrorSTMT is used.                     */
#define PDCI_IO_NEED_BYTES_SPECIFIC(specific, IOerrorSTMT)
do {
  size_t goal_bytes = specific;
  if (goal_bytes == 0) {
    while (!(elt->eor|elt->eof)) {
      /* try for more bytes */
      if (elt->next != pads->head) { /* the easy case */
	elt = elt->next;
	bytes += elt->len;
	continue;
      }
      /* elt->next is pads->head, getting more bytes requires use of read_fn */
      keep_elt = pads->stack[0].elt;
      if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
	IOerrorSTMT;
      }
      elt = elt->next;
      bytes += elt->len;
    }
    begin = tp->elt->end - tp->remain;
    end  = elt->end;
    eor  = elt->eor;
    eof  = elt->eof;
  } else {
    /* goal_bytes > 0 */
    while (!(elt->eor|elt->eof) && (bytes < goal_bytes)) {
      /* try for more bytes */
      if (elt->next != pads->head) { /* the easy case */
	elt = elt->next;
	bytes += elt->len;
	continue;
      }
      /* elt->next is pads->head, getting more bytes requires use of read_fn */
      keep_elt = pads->stack[0].elt;
      if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
	IOerrorSTMT;
      }
      elt = elt->next;
      bytes += elt->len;
    }
    begin = tp->elt->end - tp->remain;
    if (bytes > goal_bytes) {
      /* found more than enough (end will be prior any eor/eof) */
      end = begin + goal_bytes;
      eor  = 0;
      eof  = 0;
    } else if (bytes == goal_bytes) {
      /* may have hit eor or eof */
      end = begin + goal_bytes;
      eor  = elt->eor;
      eof  = elt->eof;
    } else {
      /* def hit eor or eof.  produce as many bytes as available */
      end  = elt->end;
      eor  = elt->eor;
      eof  = elt->eof;
    }
  }
} while (0)
/* END_MACRO */

/* ---------------------------------------------------------------- */
/* For the case where we need exactly K bytes or there is an error. */

/* The following has to appear where variables can be declared */
#define PDCI_IO_NEED_K_BYTES_SETUP
  PDCI_stkElt_t    *tp              = &(pads->stack[pads->top]);
  Pio_elt_t        *elt             = tp->elt;
  Pio_elt_t        *keep_elt;
  Pio_elt_t        *next_elt;
  Pbyte            *begin;
  Pbyte            *end;
  size_t            bytes           = tp->remain;
  int               eor;
  int               eof
/* END_MACRO */

/* The IOerrorSTMT statement must be/include goto or return; it */
/* is assumed NOT to fall through.  Further, begin/end are NOT  */
/* set properly if IOerrorSTMT is used.                         */
#define PDCI_IO_NEED_K_BYTES(kIN, IOerrorSTMT)
do {
  while (!(elt->eor|elt->eof) && (bytes < (kIN))) {
    /* try for more bytes */
    if (elt->next != pads->head) { /* the easy case */
      elt = elt->next;
      bytes += elt->len;
      continue;
    }
    /* elt->next is pads->head, getting more bytes requires use of read_fn */
    keep_elt = pads->stack[0].elt;
    if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
      IOerrorSTMT;
    }
    elt = elt->next;
    bytes += elt->len;
  }
  /* either we hit eor or eof, or we found >= kIN bytes */
  begin = tp->elt->end - tp->remain;
  if (bytes > (kIN)) {
    end = begin + (kIN);
    eor = 0;
    eof = 0;
  } else {
    end = elt->end;
    eor = elt->eor;
    eof = elt->eof;
  }
} while (0)
/* END_MACRO */

/* ----------------------------------------------------------------- */
/* Move forward by K bytes; should be able to move forward K without */
/* reading new bytes or advancing past EOR/EOF.                      */
/* N.B. Assumes one of the NEED_BYTES macros has been used and that  */
/*      the variables introduced by the SETUP macro are in scope.    */

/* The IOerrorSTMT statement must be/include goto or return; it */
/* is assumed NOT to fall through.                              */

#define PDCI_IO_FORWARD(num_bytes, IOerrorSTMT)
do {
  size_t todo = num_bytes;
  while (todo > 0) {
    if (tp->remain == 0) {
      if (tp->elt->eor|tp->elt->eof) {
	P_FATAL(pads->disc, "Internal error, PDCI_IO_FORWARD hit EOR or EOF");
      }
      if (tp->elt->next == pads->head) {
	P_FATAL(pads->disc, "Internal error, PDCI_IO_FORWARD would need to read bytes from io stream");
      }
      tp->elt = tp->elt->next;
      tp->remain = tp->elt->len;
      continue;
    }
    if (todo <= tp->remain) {
      tp->remain -= todo;
      todo = 0;
      break;
    }
    /* current IO rec gets us partway */
    todo -= tp->remain;
    tp->remain = 0;
  }
  /* success */
  if (!tp->remain && !(tp->elt->eor|tp->elt->eof)) {
    /* at end of a non-EOR, non-EOF elt: advance now */
    if (tp->elt->next != pads->head) {
      tp->elt = tp->elt->next;
      tp->remain = tp->elt->len;
    } else {
      /* need to read some data -- use IO disc read_fn */
      keep_elt = pads->stack[0].elt;
      if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
	tp->elt = P_LAST_ELT(pads->head); /* IO disc may have added eof elt */
	tp->remain = 0;
	IOerrorSTMT;
      }
      tp->elt = next_elt;
      tp->remain = tp->elt->len;
    }
  }
} while (0)

/* ================================================================================ */
/* MACROS USED BY ACCUM FUNCTIONS */
 
/* Useful constants */

#define PDCI_HALFMIN_INT64   -4611686018427387904LL
#define PDCI_HALFMAX_INT64    4611686018427387903LL
#define PDCI_HALFMAX_UINT64   9223372036854775807ULL
#define PDCI_LARGE_NEG_DBL   -4611686018427387904.0
#define PDCI_LARGE_POS_DBL    4611686018427387903.0
/* END_MACRO */

/* Fold Points : when should the running int64 / uint64 sum be folded into the average? */

#define PDCI_FOLD_MIN_INT8    -9223372036854775680LL  /* P_MIN_INT64 - P_MIN_INT8  */
#define PDCI_FOLD_MAX_INT8     9223372036854775680LL  /* P_MAX_INT64 - P_MAX_INT8  */
#define PDCI_FOLD_MIN_INT16   -9223372036854743040LL  /* P_MIN_INT64 - P_MIN_INT16 */
#define PDCI_FOLD_MAX_INT16    9223372036854743040LL  /* P_MAX_INT64 - P_MAX_INT16 */
#define PDCI_FOLD_MIN_INT32   -9223372034707292160LL  /* P_MIN_INT64 - P_MIN_INT32 */
#define PDCI_FOLD_MAX_INT32    9223372034707292160LL  /* P_MAX_INT64 - P_MAX_INT32 */

#define PDCI_FOLD_MAX_UINT8   18446744073709551488ULL  /* P_MAX_UINT64 - P_MAX_UINT8  */
#define PDCI_FOLD_MAX_UINT16  18446744073709518848ULL  /* P_MAX_UINT64 - P_MAX_UINT16 */
#define PDCI_FOLD_MAX_UINT32  18446744069414584320ULL  /* P_MAX_UINT64 - P_MAX_UINT32 */
/* END_MACRO */

/* Macros that test whether folding should occur, given new val v and running sum s */

#define PDCI_FOLDTEST_INT8(v, s)  (((s) < PDCI_FOLD_MIN_INT8)  || ((s) > PDCI_FOLD_MAX_INT8))
#define PDCI_FOLDTEST_INT16(v, s) (((s) < PDCI_FOLD_MIN_INT16) || ((s) > PDCI_FOLD_MAX_INT16))
#define PDCI_FOLDTEST_INT32(v, s) (((s) < PDCI_FOLD_MIN_INT32) || ((s) > PDCI_FOLD_MAX_INT32))
#define PDCI_FOLDTEST_INT32(v, s) (((s) < PDCI_FOLD_MIN_INT32) || ((s) > PDCI_FOLD_MAX_INT32))
#define PDCI_FOLDTEST_INT64(v, s) ( (((s) < 0) && ((v) < PDCI_HALFMIN_INT64)) ||
				   (((v) < 0) && ((s) < PDCI_HALFMIN_INT64)) ||
				   (((s) > 0) && ((v) > PDCI_HALFMAX_INT64)) ||
				   (((v) > 0) && ((s) > PDCI_HALFMAX_INT64)) )
#define PDCI_FOLDTEST_UINT8(v, s)  ((s) > PDCI_FOLD_MAX_UINT8)
#define PDCI_FOLDTEST_UINT16(v, s) ((s) > PDCI_FOLD_MAX_UINT16)
#define PDCI_FOLDTEST_UINT32(v, s) ((s) > PDCI_FOLD_MAX_UINT32)
#define PDCI_FOLDTEST_UINT64(v, s) ( ((s) > PDCI_HALFMAX_UINT64) || ((v) > PDCI_HALFMAX_UINT64) )
/* END_MACRO */

/* ================================================================================ */
/* DOUBLY-LINKED LIST HELPER MACROS */

#define P_SOME_ELTS(head) ((head)->next != (head))
#define P_FIRST_ELT(head) ((head)->next)
#define P_LAST_ELT(head)  ((head)->prev)
/* END_MACRO */

#define P_REMOVE_ELT(elt)
  do {
    (elt)->prev->next = (elt)->next;
    (elt)->next->prev = (elt)->prev;
  } while (0)
/* END_MACRO */

#define P_APPEND_ELT(head, elt)
  do {
    (elt)->prev = (head)->prev;
    (elt)->next = (head);
    (elt)->prev->next = (elt);
    (elt)->next->prev = (elt);
  } while (0)
/* END_MACRO */

#define P_PREPEND_ELT(head, elt)
  do {
    (elt)->prev = (head);
    (elt)->next = (head)->next;
    (elt)->prev->next = (elt);
    (elt)->next->prev = (elt);
  } while (0)
/* END_MACRO */

/* ================================================================================ */
/* WRITE FUNCTION HELPER MACROS */

#define PDCI_WFMT_INT_WRITE(writelen, iostr, wfmt, width, t)
  do {
    writelen = sfprintf(iostr, wfmt, ((t < 0) ? width-1 : width), t);
  } while (0)
/* END_MACRO */

#define PDCI_WFMT_UINT_WRITE(writelen, iostr, wfmt, width, t)
  do {
    writelen = sfprintf(iostr, wfmt, width, t);
  } while (0)
/* END_MACRO */

#define PDCI_FMT_INT_WRITE(writelen, iostr, fmt, t)
  do {
    writelen = sfprintf(iostr, fmt, t);
  } while (0)
/* END_MACRO */

#define PDCI_FMT_UINT_WRITE(writelen, iostr, fmt, t)
  do {
    writelen = sfprintf(iostr, fmt, t);
  } while (0)
/* END_MACRO */

/* sfprintf workaround for signed int1 vals */
#define PDCI_WFMT_INT1_WRITE(writelen, iostr, wfmt, width, t)
  do {
    Pint32 t_subst = t;
    writelen = sfprintf(iostr, "%0.*I4d", ((t < 0) ? width-1 : width), t_subst);
  } while (0)
/* END_MACRO */

/* sfprintf workaround for signed int1 vals */
#define PDCI_FMT_INT1_WRITE(writelen, iostr, fmt, t)
  do {
    Pint32 t_subst = t;
    writelen = sfprintf(iostr, "%I4d", t_subst);
  } while (0)
/* END_MACRO */

#define PDCI_FMT_FLOAT_WRITE(writelen, iostr, fmt, t)
  do {
    writelen = sfprintf(iostr, fmt, t);
  } while (0)
/* END_MACRO */

#define PDCI_WFMT_FLOAT_WRITE(writelen, iostr, wfmt, width, t)
  do {
    writelen = sfprintf(iostr, wfmt, ((t < 0) ? width-1 : width), t);
  } while (0)
/* END_MACRO */

#define PDCI_BASELIT_XML_OUT(sfprintf_prefix, io, tag, def_tag, indent, outfmt, outval)
  do {
    if (!tag) { tag = def_tag; }
    indent = (indent > 128) ? 128 : indent;
    sfprintf_prefix sfprintf(io, "%.*s<%s><val>" outfmt "</val></%s>\n", indent, PDCI_spaces, tag, outval, tag);
  } while (0)
/* END_MACRO */

/* assumes variables io, tag, indent are in scope */
#define PDCI_BASELIT_XML_OUT2IO(def_tag, outfmt, outval)
  PDCI_BASELIT_XML_OUT(return, io, tag, def_tag, indent, outfmt, outval)
/* END_MACRO */

/* assumes variables writelen, buf, buf_full, buf_len, pads, tag, indent are in scope */
#define PDCI_BASELIT_XML_OUT2BUF(def_tag, outfmt, outval)
  do {
    PDCI_sfstr_seek2zero(pads->tmp3);
    PDCI_BASELIT_XML_OUT(writelen = , pads->tmp3, tag, def_tag, indent, outfmt, outval);
    if (writelen <= 0) {
      return -1;
    }
    if (writelen > buf_len) {
      (*buf_full) = 1;
      return -1;
    }
    memcpy(buf, PDCI_sfstr_use(pads->tmp3), writelen);
    return writelen;
  } while (0)
/* END_MACRO */

#define LOC_FMT "<loc><b><num>%lld</num><byte>%lld</byte></b><e><num>%lld</num><byte>%lld</byte></e></loc>"
#define PDCI_BASEVAL_XML_OUT(sfprintf_prefix, io, tag, def_tag, indent, pd, outfmt, outval)
  do {
    if (!tag) { tag = def_tag; }
    indent = (indent > 128) ? 128 : indent;
    if ((pd)->errCode == P_NO_ERR) { /* value, no errors */
      sfprintf_prefix sfprintf(io, "%.*s<%s><val>" outfmt "</val></%s>\n", 
			       indent, 
			       PDCI_spaces, 
			       tag, 
			       outval, 
			       tag);
    } else if ((pd)->errCode < 100) { /* no value, no location */
      sfprintf_prefix sfprintf(io, "%.*s<%s><pd>" PSTATE_NERR_ERRCODE_FMT "</pd></%s>\n",
			       indent, 
			       PDCI_spaces, 
			       tag, 
			       P_pstate2str((pd)->pstate), 
			       (pd)->nerr, 
			       P_errCode2str((pd)->errCode), 
			       tag);
    } else if ((pd)->errCode == P_USER_CONSTRAINT_VIOLATION) { /* value and location */
      sfprintf_prefix sfprintf(io, "%.*s<%s><val>" outfmt "</val>\n%.*s<pd>" PSTATE_NERR_ERRCODE_FMT "\n%.*s" LOC_FMT "</pd></%s>\n",
			       indent, 
			       PDCI_spaces, 
			       tag, 			       	
			       outval,
 			       indent, 
			       PDCI_spaces, 
			       P_pstate2str((pd)->pstate), 
			       (pd)->nerr, 
	                       P_errCode2str((pd)->errCode),
			       indent, 
			       PDCI_spaces, 
			       (long long)(pd)->loc.b.num, (long long)(pd)->loc.b.byte, 
			       (long long)(pd)->loc.e.num, (long long)(pd)->loc.e.byte, 
			       tag);
    } else { /* no value, location */
      sfprintf_prefix sfprintf(io, "%.*s<%s><pd>" PSTATE_NERR_ERRCODE_FMT LOC_FMT "</pd></%s>\n",
			       indent, 
			       PDCI_spaces, 
			       tag, 
			       P_pstate2str((pd)->pstate), 
			       (pd)->nerr, 
			       P_errCode2str((pd)->errCode),
			       (long long)(pd)->loc.b.num, (long long)(pd)->loc.b.byte, 
			       (long long)(pd)->loc.e.num, (long long)(pd)->loc.e.byte, 
			       tag);
    }
  } while (0)
/* END_MACRO */

/* assumes variables io, tag, indent, pd are in scope */
#define PDCI_BASEVAL_XML_OUT2IO(def_tag, outfmt, outval)
  PDCI_BASEVAL_XML_OUT(return, io, tag, def_tag, indent, pd, outfmt, outval)
/* END_MACRO */

/* assumes variables writelen, buf, buf_full, buf_len, pads, tag, indent, pd are in scope */
#define PDCI_BASEVAL_XML_OUT2BUF(def_tag, outfmt, outval)
  do {
    PDCI_sfstr_seek2zero(pads->tmp3);
    PDCI_BASEVAL_XML_OUT(writelen = , pads->tmp3, tag, def_tag, indent, pd, outfmt, outval);
    if (writelen <= 0) {
      return -1;
    }
    if (writelen > buf_len) {
      (*buf_full) = 1;
      return -1;
    }
    memcpy(buf, PDCI_sfstr_use(pads->tmp3), writelen);
    return writelen;
  } while (0)
/* END_MACRO */

/* ********************************* BEGIN_TRAILER ******************************** */
/* ********************************** END_MACROS ********************************** */

/* ****************** BEGIN_MACROS(pads-read-macros-gen.h) ******************** */
/*
 * Macros that help implement read functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-config.h"

/* ********************************** END_HEADER ********************************** */

#define PDCI_AE_INT_READ_FN_GEN(fn_pref, targ_type, bytes2num_fn, invalid_err, isspace_fn, isdigit_fn, a_or_e_int)

Perror_t
fn_pref ## _read(P_t *pads, const Pbase_m *m,
		 Pbase_pd *pd, targ_type *res_out)
{
  targ_type    tmp;   /* tmp num */
  Pbyte        ct;    /* char tmp */
  Pbyte       *p1;
  PDCI_IO_NEED_BYTES_SETUP_NUMERIC;

  PDCI_IODISC_3P_CHECKS( PDCI_MacroArg2String(fn_pref) "_read", m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  PDCI_IO_NEED_BYTES(goto fatal_nb_io_err);
  if (begin == end) {
    goto at_eor_or_eof_err;
  }
  p1 = begin;
  if (P_Test_Ignore(*m)) {
    /* move beyond anything that looks like an ascii number, return P_ERR if none such */
    if (isspace_fn(*p1) && !(pads->disc->flags & P_WSPACE_OK)) {
      return P_ERR;
    }
    while (isspace_fn(*p1)) { /* skip spaces, if any */
      p1++;
      if (p1 == end) return P_ERR; /* did not find digit */
    }
    if ('-' == (*p1) || '+' == (*p1)) { /* skip +/-, if any */
      p1++;
      if (p1 == end) return P_ERR; /* did not find a digit */
    }
    if (!isdigit_fn(*p1)) {
      return P_ERR; /* did not find a digit */
    }
    /* all set: skip digits, move IO cursor, and return P_OK */
    while (isdigit_fn(*p1)) {
      p1++;
      if (p1 == end && !(eor|eof)) {
	/* did not find end of digits within P_BUILTIN_NUMERIC_MAX bytes */
	return P_ERR;
      }
    }
    PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
    return P_OK;

  } else { /* !P_Test_Ignore(*m) */

    if (isspace_fn(*p1) && !(pads->disc->flags & P_WSPACE_OK)) {
      goto invalid_wspace;
    }
    ct = *end;    /* save */
    *end = 0;     /* null */
    if (P_Test_SemCheck(*m)) {
      tmp = bytes2num_fn(begin, &p1);
    } else {
      tmp = bytes2num_fn ## _norange(begin, &p1);
    }
    *end = ct;    /* restore */
    if (errno == EINVAL) {
      if (p1 != end) p1++; /* move to just beyond offending char */
      goto invalid;
    }
    if (errno == ERANGE) goto range_err;
    /* success */
    PDCI_ECHO_MACRO_TOKEN(pads, fn_pref,begin,p1);
    PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
    if (P_Test_Set(*m)) {
      (*res_out) = tmp;
    }
    return P_OK;
  }

 at_eor_or_eof_err:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", 0, eor ? P_AT_EOR : P_AT_EOF);

 invalid_wspace:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", "spaces not allowed in " a_or_e_int " field unless flag P_WSPACE_OK is set", invalid_err);

 invalid:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", 0, invalid_err);

 range_err:
  /* range error still consumes the number */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", 0, P_RANGE);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_pref) "_read", *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_pref) "_read", *m, "IO_forward error", P_FORWARD_ERR);
}
/* END_MACRO */

#define PDCI_AE_INT_FW_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, invalid_err, isspace_fn, a_or_e_int)

Perror_t
fn_name(P_t *pads, const Pbase_m *m,
	Pbase_pd *pd, targ_type *res_out, size_t width)
{
  targ_type    tmp;   /* tmp num */
  Pbyte        ct;    /* char tmp */
  Pbyte       *p1;
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_3P_CHECKS( PDCI_MacroArg2String(fn_name), m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  PDCI_READFN_WIDTH_CHECK( PDCI_MacroArg2String(fn_name), "", width);
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  if (end-begin != width)     goto width_not_avail;
  if (P_Test_NotIgnore(*m)) {
    if (isspace_fn(*begin) && !(pads->disc->flags & P_WSPACE_OK)) {
      goto invalid_wspace;
    }
    ct = *end;    /* save */
    *end = 0;     /* null */
    if (P_Test_SemCheck(*m)) {
      tmp = bytes2num_fn(begin, &p1);
    } else {
      tmp = bytes2num_fn ## _norange(begin, &p1);
    }
    *end = ct;    /* restore */
    if (errno == EINVAL) goto invalid;
    if (p1 < end && isspace_fn(*p1)) {
      if (!(pads->disc->flags & P_WSPACE_OK)) {
	goto invalid_wspace;
      }
      do { p1++; } while (p1 < end && isspace_fn(*p1));
    }
    if (p1 != end) {
      goto invalid;
    }
    if (errno == ERANGE) goto range_err;
    /* success */
    PDCI_ECHO_MACRO_TOKEN(pads, fn_name,begin,p1);
    PDCI_IO_FORWARD(width, goto fatal_forward_err);
    if (P_Test_Set(*m)) {
      (*res_out) = tmp;
    }
  } else {
    /* just move forward */
    PDCI_IO_FORWARD(width, goto fatal_forward_err);
  }
  return P_OK;

 width_not_avail:
  /* FW field: eat the space whether or not there is an error */
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
    PDCI_IO_FORWARD(end-begin, goto fatal_forward_err);
  }
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_WIDTH_NOT_AVAILABLE);

 invalid:
  /* FW field: eat the space whether or not there is an error */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, width-1);
  PDCI_IO_FORWARD(width, goto fatal_forward_err);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, invalid_err);

 invalid_wspace:
  /* FW field: eat the space whether or not there is an error */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, width-1);
  PDCI_IO_FORWARD(width, goto fatal_forward_err);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), "spaces not allowed in " a_or_e_int " field unless flag P_WSPACE_OK is set", invalid_err);

 range_err:
  /* FW field: eat the space whether or not there is an error */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, width-1);
  PDCI_IO_FORWARD(width, goto fatal_forward_err);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_RANGE);

  /* fatal_alloc_err:
     PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "Memory alloc err", P_ALLOC_ERR); */

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO_forward error", P_FORWARD_ERR);
}
/* END_MACRO */

#define PDCI_ASCII_LOWER_E   'e'
/* END_MACRO */

#define PDCI_EBCDIC_LOWER_E   XXX_TODO
/* END_MACRO */

#define PDCI_AE_FLOAT_READ_FN_GEN(fn_pref, targ_type, bytes2num_fn, invalid_err, isspacemap, isdigitmap, charmap, a_or_e_float)
/* Char-based floats have the form:     [+|-]DIGITS[.][DIGITS][(e|E)[+|-]DIGITS] */
Perror_t
fn_pref ## _read(P_t *pads, const Pbase_m *m,
		 Pbase_pd *pd, targ_type *res_out)
{
  targ_type tmp;
  Pbyte    ct;
  Pbyte   *p1;
  Pchar plus     = charmap[(int)'+'];
  Pchar minus    = charmap[(int)'-'];
  Pchar e_lower  = charmap[(int)'e'];
  Pchar e_upper  = charmap[(int)'E'];
  Pchar dot      = charmap[(int)'.'];
  PDCI_IO_NEED_BYTES_SETUP_NUMERIC;

  PDCI_IODISC_3P_CHECKS(PDCI_MacroArg2String(fn_pref) "_read", m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  PDCI_IO_NEED_BYTES(goto fatal_nb_io_err);
  if (begin == end) {
    goto at_eor_or_eof_err;
  }
  p1 = begin;
  ct = *end;    /* save */
  *end = 0;     /* null */
  if (P_Test_Ignore(*m)) {
    /* move beyond anything that looks like a float, return P_ERR if none such */
    if (isspacemap[(int)*p1] && !(pads->disc->flags & P_WSPACE_OK)) goto nocheck_err;
    while (isspacemap[(int)(*p1)]) { p1++; }            /* skip spaces, if any */
    if (plus == (*p1) || minus == (*p1)) { p1++; }   /* skip +/-, if any */
    if (!isdigitmap[(int)*p1]) goto nocheck_err;        /* did not find a digit */
    while (isdigitmap[(int)*p1]) { p1++; }              /* skip digits */
    if (dot == (*p1)) {    /* skip dot and trailing digits */
      p1++;
      while (isdigitmap[(int)*p1]) { p1++; }
    }
    /* skip e, optional sign, digits, if present */
    if ((e_lower == (*p1) || e_upper == (*p1))) {
      p1++;
      if (p1 == end) goto nocheck_err; /* did not find at least 1 digit after e */
      if (plus == (*p1) || minus == (*p1)) {  /* skip +/-, if any */
	p1++;
	if (p1 == end) goto nocheck_err; /* did not find at least 1 digit after e */
      }
      if (!isdigitmap[(int)*p1]) goto nocheck_err; /* did not find at least 1 digit after e */
      while (isdigitmap[(int)*p1]) { p1++; }  /* skip digits */
    }
    if (p1 == end && !(eor|eof)) goto nocheck_err; /* did not find end of float within P_BUILTIN_NUMERIC_MAX bytes */
    /* OK */
    *end = ct; /* restore */
    PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
    return P_OK;

  } else { /* !P_Test_Ignore(*m) */

    if (isspacemap[(int)(*p1)] && !(pads->disc->flags & P_WSPACE_OK)) {
      goto invalid_wspace;
    }
    if (P_Test_SemCheck(*m)) {
      tmp = bytes2num_fn(begin, &p1);
    } else {
      tmp = bytes2num_fn ## _norange(begin, &p1);
    }
    if (errno == EINVAL) {
      if (p1 != end) p1++; /* move to just beyond offending char */
      goto invalid;
    }
    if (errno == ERANGE) goto range_err;
    PDCI_ECHO_MACRO_TOKEN(pads, fn_pref,begin,p1);
    PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
    if (P_Test_Set(*m)) {
      (*res_out) = tmp;
    }
    *end = ct;    /* restore */
    return P_OK;
  }

 nocheck_err:
  *end = ct; /* restore */
  return P_ERR;

 at_eor_or_eof_err:
  /* don't need to restore, eor/eof error occurs before *end is set to 0 */
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", 0, eor ? P_AT_EOR : P_AT_EOF);

 invalid_wspace:
  *end = ct; /* restore */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", "spaces not allowed in " a_or_e_float " field unless flag P_WSPACE_OK is set", invalid_err);

 invalid:
  *end = ct; /* restore */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", 0, invalid_err);

 range_err:
  *end = ct; /* restore */
  /* range error still consumes the number */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_pref) "_read", 0, P_RANGE);

 fatal_nb_io_err:
  /* don't need to restore, need bytes error occurs before *end is set to 0 */
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_pref) "_read", *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  *end = ct;    /* restore */
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_pref) "_read", *m, "IO_forward error", P_FORWARD_ERR);
}
/* END_MACRO */

#define PDCI_B1_INT_READ_FN_GEN(fn_name, targ_type)

Perror_t
fn_name(P_t *pads, const Pbase_m *m,
	Pbase_pd *pd, targ_type *res_out)
{
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_3P_CHECKS( PDCI_MacroArg2String(fn_name), m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  PDCI_IO_NEED_K_BYTES(1, goto fatal_nb_io_err);
  if (end-begin != 1)     goto width_not_avail;
  if (P_Test_Set(*m)) {
    (*res_out) = *begin;
  }
  PDCI_ECHO_MACRO_TOKEN(pads, fn_name,begin,end);
  PDCI_IO_FORWARD(1, goto fatal_forward_err);
  return P_OK;

 width_not_avail:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_WIDTH_NOT_AVAILABLE);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO_forward error", P_FORWARD_ERR);
}
/* END_MACRO */

#define PDCI_B_INT_READ_FN_GEN(fn_name, targ_type, width, swapmem_op)

Perror_t
fn_name(P_t *pads, const Pbase_m *m,
	Pbase_pd *pd, targ_type *res_out)
{
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_3P_CHECKS( PDCI_MacroArg2String(fn_name), m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  if (end-begin != width)     goto width_not_avail;
  if (P_Test_Set(*m)) {
    if (pads->m_endian != pads->disc->d_endian) {
      swapmem(swapmem_op, begin, res_out, width);
    } else {
      swapmem(0, begin, res_out, width);
    }
  };
  PDCI_IO_FORWARD(width, goto fatal_forward_err);
  return P_OK;

 width_not_avail:
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_WIDTH_NOT_AVAILABLE);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO_forward error", P_FORWARD_ERR);
}
/* END_MACRO */

#define PDCI_EBCBCDSB_INT_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, invalid_err, width, bytes2num_args)

Perror_t
fn_name(P_t *pads, const Pbase_m *m, Pbase_pd *pd, targ_type *res_out, Puint32 num_digits_or_bytes)
{
  targ_type     tmp;   /* tmp num */
  Pbyte        *p1;    /* available for use in bytes2num_args */
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_3P_CHECKS( PDCI_MacroArg2String(fn_name), m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  if (end-begin != width)     goto width_not_avail;
  if (P_Test_NotIgnore(*m)) {
    if (P_Test_SemCheck(*m)) {
      tmp = bytes2num_fn bytes2num_args;
    } else {
      tmp = bytes2num_fn ## _norange bytes2num_args;
    }
    if (errno) goto invalid_range_dom;
    /* success */
    PDCI_IO_FORWARD(width, goto fatal_forward_err);
    if (P_Test_Set(*m)) {
      (*res_out) = tmp;
    }
  } else {
    /* just move forward */
    PDCI_IO_FORWARD(width, goto fatal_forward_err);
  }
  return P_OK;

 invalid_range_dom:
  /* FW field: eat the space whether or not there is an error */
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, width-1);
  PDCI_IO_FORWARD(width, goto fatal_forward_err);
  switch (errno) {
  case EINVAL:
    PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, invalid_err);
  case ERANGE:
    PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_RANGE);
  case EDOM:
    PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_BAD_PARAM);
  }

 width_not_avail:
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_WIDTH_NOT_AVAILABLE);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(PDCI_MacroArg2String(fn_name), *m, "IO_forward error", P_FORWARD_ERR);
}
/* END_MACRO */

#define PDCI_EBCBCDSB_FPOINT_READ_FN_GEN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)

Perror_t
fn_name(P_t *pads, const Pbase_m *m,
	Pbase_pd *pd, targ_type *res_out, Puint32 num_digits_or_bytes, Puint32 d_exp)
{
  targ_type       tmp;   /* tmp num */

  PDCI_IODISC_3P_CHECKS( PDCI_MacroArg2String(fn_name), m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  (pads->inestlev)++;
  if (P_ERR == internal_numerator_read_fn(pads, m, pd, &(tmp.num), num_digits_or_bytes)) {
    /* pd filled in already, IO cursor advanced if appropriate */
    (pads->inestlev)--;
    PDCI_READFN_RET_EXIST_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0);
  }
  (pads->inestlev)--;
  /* so far so good, IO cursor has been advanced, pd->errCode set to P_NO_ERR */
  if (d_exp > dexp_max) {
    PDCI_READFN_BEGINLOC_MINUSK(pads, pd->loc, width);
    PDCI_READFN_ENDLOC_MINUS1(pads, pd->loc);
    PDCI_READFN_RET_ERRCODE_WARN(PDCI_MacroArg2String(fn_name), 0, P_BAD_PARAM);
  }
  if (P_Test_Set(*m)) {
    tmp.denom = PDCI_10toThe[d_exp];
    (*res_out) = tmp;
  }
  return P_OK;
}
/* END_MACRO */

/* ********************************* BEGIN_TRAILER ******************************** */

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_A_INT > 0
#  define PDCI_A_INT_READ_FN(fn_pref, targ_type, bytes2num_fn) \
            PDCI_AE_INT_READ_FN_GEN(fn_pref, targ_type, bytes2num_fn, P_INVALID_A_NUM, PDCI_is_a_space, PDCI_is_a_digit, "a_int")
#else
#  define PDCI_A_INT_READ_FN(fn_pref, targ_type, bytes2num_fn)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_A_INT_FW > 0
#  define PDCI_A_INT_FW_READ_FN(fn_name, targ_type, bytes2num_fn) \
            PDCI_AE_INT_FW_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, P_INVALID_A_NUM, PDCI_is_a_space, "a_int")
#else
#  define PDCI_A_INT_FW_READ_FN(fn_name, targ_type, bytes2num_fn)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_E_INT > 0
#  define PDCI_E_INT_READ_FN(fn_pref, targ_type, bytes2num_fn) \
            PDCI_AE_INT_READ_FN_GEN(fn_pref, targ_type, bytes2num_fn, P_INVALID_E_NUM, PDCI_is_e_space, PDCI_is_e_digit, "e_int")
#else
#  define PDCI_E_INT_READ_FN(fn_pref, targ_type, bytes2num_fn)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_E_INT_FW > 0
#  define PDCI_E_INT_FW_READ_FN(fn_name, targ_type, bytes2num_fn) \
            PDCI_AE_INT_FW_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, P_INVALID_E_NUM, PDCI_is_e_space, "e_int")
#else
#  define PDCI_E_INT_FW_READ_FN(fn_name, targ_type, bytes2num_fn)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_B_INT > 0
#  define PDCI_B1_INT_READ_FN(fn_name, targ_type) \
            PDCI_B1_INT_READ_FN_GEN(fn_name, targ_type)
#  define PDCI_B_INT_READ_FN(fn_name, targ_type, width, swapmem_op) \
            PDCI_B_INT_READ_FN_GEN(fn_name, targ_type, width, swapmem_op)
#else
#  define PDCI_B1_INT_READ_FN(fn_name, targ_type)
#  define PDCI_B_INT_READ_FN(fn_name, targ_type, width, swapmem_op)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && (P_CONFIG_EBC_INT > 0 || P_CONFIG_EBC_FPOINT > 0)
#  define PDCI_EBC_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width) \
            PDCI_EBCBCDSB_INT_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, P_INVALID_EBC_NUM, width, (begin, num_digits_or_bytes, &p1))
#else
#  define PDCI_EBC_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && (P_CONFIG_BCD_INT > 0 || P_CONFIG_BCD_FPOINT > 0)
#  define PDCI_BCD_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width) \
            PDCI_EBCBCDSB_INT_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, P_INVALID_BCD_NUM, width, (begin, num_digits_or_bytes, &p1))
#else
#  define PDCI_BCD_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && (P_CONFIG_SBL_INT > 0 || P_CONFIG_SBL_FPOINT > 0)
#  define PDCI_SBL_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width) \
            PDCI_EBCBCDSB_INT_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, P_UNEXPECTED_ERR, width, (pads, begin, num_digits_or_bytes, &p1))
#else
#  define PDCI_SBL_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && (P_CONFIG_SBH_INT > 0 || P_CONFIG_SBH_FPOINT > 0)
#  define PDCI_SBH_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width) \
            PDCI_EBCBCDSB_INT_READ_FN_GEN(fn_name, targ_type, bytes2num_fn, P_UNEXPECTED_ERR, width, (pads, begin, num_digits_or_bytes, &p1))
#else
#  define PDCI_SBH_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_EBC_FPOINT > 0
#  define PDCI_EBC_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max) \
            PDCI_EBCBCDSB_FPOINT_READ_FN_GEN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#else
#  define PDCI_EBC_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_BCD_FPOINT > 0
#  define PDCI_BCD_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max) \
            PDCI_EBCBCDSB_FPOINT_READ_FN_GEN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#else
#  define PDCI_BCD_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_SBL_FPOINT > 0
#  define PDCI_SBL_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max) \
            PDCI_EBCBCDSB_FPOINT_READ_FN_GEN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#else
#  define PDCI_SBL_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_SBH_FPOINT > 0
#  define PDCI_SBH_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max) \
            PDCI_EBCBCDSB_FPOINT_READ_FN_GEN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#else
#  define PDCI_SBH_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_A_FLOAT > 0
#  define PDCI_A_FLOAT_READ_FN(fn_pref, targ_type, bytes2num_fn) \
            PDCI_AE_FLOAT_READ_FN_GEN(fn_pref, targ_type, bytes2num_fn, P_INVALID_A_NUM, PDCI_ascii_is_space, PDCI_ascii_is_digit, PDCI_identity_charmap, "a_float")
#else
#  define PDCI_A_FLOAT_READ_FN(fn_pref, targ_type, bytes2num_fn)
#endif

#if P_CONFIG_READ_FUNCTIONS > 0 && P_CONFIG_E_FLOAT > 0
#  define PDCI_E_FLOAT_READ_FN(fn_pref, targ_type, bytes2num_fn) \
            PDCI_AE_FLOAT_READ_FN_GEN(fn_pref, targ_type, bytes2num_fn, P_INVALID_E_NUM, PDCI_ebcdic_is_space, PDCI_ebcdic_is_digit, P_mod_ae_tab, "e_float")
#else
#  define PDCI_E_FLOAT_READ_FN(fn_pref, targ_type, bytes2num_fn)
#endif


/* ********************************** END_MACROS ********************************** */
/* ****************** BEGIN_MACROS(pads-write-macros-gen.h) ******************* */
/*
 * Macros that help implement write functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-config.h"

/* ********************************** END_HEADER ********************************** */

/* The following macros implement fmt2buf_final, fmt2buf, and fmt2io using a write2buf call */

#define PDCI_FMT_FN_GEN(fn_pref, afn_pref, rep_ty, lookup_ty)
ssize_t
fn_pref ## _fmt2buf_final(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
			  Pbase_m *m, Pbase_pd *pd, rep_ty *rep)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep);
}

ssize_t
fn_pref ## _fmt2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
		    Pbase_m *m, Pbase_pd *pd, rep_ty *rep)
{
  Pfmt_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    return P_invoke_fmt_fn(fn, pads, buf, buf_len, buf_full, requested_out, delims, m, pd, rep);
  }
  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep);
}

ssize_t
fn_pref ## _fmt2io(P_t *pads, Sfio_t *io, int *requested_out, const char *delims,
		   Pbase_m *m, Pbase_pd *pd, rep_ty *rep)
{
  Pfmt_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", io, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", m, pd, rep);

  PDCI_STANDARD_FMT2IO_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    ssize_t length;
    Pbyte *buf;
    int buf_full;
    size_t buf_len;
    PDCI_FMT2IO_USE_FMT2BUF_FN(PDCI_MacroArg2String(fn_pref) "_fmt2io", length=P_invoke_fmt_fn(fn, pads, buf, buf_len, &buf_full, requested_out, delims, m, pd, rep));
  }
  (*requested_out) = 1;
  return afn_pref ## _write2io(pads, io, pd, rep);
}
/* END_MACRO */

#define PDCI_FMT_FN_XTRA1_GEN(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty)
ssize_t
fn_pref ## _fmt2buf_final(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
			  Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep, x1);
}

ssize_t
fn_pref ## _fmt2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
		    Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1)
{
  Pfmt_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    return P_invoke_fmt_fn(fn, pads, buf, buf_len, buf_full, requested_out, delims, m, pd, rep, x1);
  }
  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep, x1);
}

ssize_t
fn_pref ## _fmt2io(P_t *pads, Sfio_t *io, int *requested_out, const char *delims,
		   Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1)
{
  Pfmt_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", io, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", m, pd, rep);

  PDCI_STANDARD_FMT2IO_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    ssize_t length;
    Pbyte *buf;
    int buf_full;
    size_t buf_len;
    PDCI_FMT2IO_USE_FMT2BUF_FN(PDCI_MacroArg2String(fn_pref) "_fmt2io", length=P_invoke_fmt_fn(fn, pads, buf, buf_len, &buf_full, requested_out, delims, m, pd, rep, x1));
  }
  (*requested_out) = 1;
  return afn_pref ## _write2io(pads, io, pd, rep, x1);
}
/* END_MACRO */

#define PDCI_FMT_FN_XTRA1_IGNORE_GEN(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty)
ssize_t
fn_pref ## _fmt2buf_final(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
			  Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep);
}

ssize_t
fn_pref ## _fmt2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
		    Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1)
{
  Pfmt_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    return P_invoke_fmt_fn(fn, pads, buf, buf_len, buf_full, requested_out, delims, m, pd, rep);
  }
  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep);
}

ssize_t
fn_pref ## _fmt2io(P_t *pads, Sfio_t *io, int *requested_out, const char *delims,
		   Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1)
{
  Pfmt_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", io, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", m, pd, rep);

  PDCI_STANDARD_FMT2IO_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    ssize_t length;
    Pbyte *buf;
    int buf_full;
    size_t buf_len;
    PDCI_FMT2IO_USE_FMT2BUF_FN(PDCI_MacroArg2String(fn_pref) "_fmt2io", length=P_invoke_fmt_fn(fn, pads, buf, buf_len, &buf_full, requested_out, delims, m, pd, rep));
  }
  (*requested_out) = 1;
  return afn_pref ## _write2io(pads, io, pd, rep);
}
/* END_MACRO */

#define PDCI_FMT_FN_XTRA3_GEN(fn_pref, afn_pref, rep_ty, xtra_ty1, xtra_ty2, xtra_ty3, lookup_ty)
ssize_t
fn_pref ## _fmt2buf_final(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
			  Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1, xtra_ty2 x2, xtra_ty3 x3)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf_final", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep, x1, x2, x3);
}

ssize_t
fn_pref ## _fmt2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, int *requested_out, const char *delims,
		    Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1, xtra_ty2 x2, xtra_ty3 x3)
{
  Pfmt_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", buf, buf_full, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2buf", m, pd, rep);

  PDCI_STANDARD_FMT2BUF_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    return P_invoke_fmt_fn(fn, pads, buf, buf_len, buf_full, requested_out, delims, m, pd, rep, x1, x2, x3);
  }
  (*requested_out) = 1;
  return afn_pref ## _write2buf(pads, buf, buf_len, buf_full, pd, rep, x1, x2, x3);
}

ssize_t
fn_pref ## _fmt2io(P_t *pads, Sfio_t *io, int *requested_out, const char *delims,
		   Pbase_m *m, Pbase_pd *pd, rep_ty *rep, xtra_ty1 x1, xtra_ty2 x2, xtra_ty3 x3)
{
  Pfmt_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", io, requested_out, delims);
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_fmt2io", m, pd, rep);

  PDCI_STANDARD_FMT2IO_INIT(*m, requested_out);

  if ((fn = PDCI_GET_FMT_FN(pads, lookup_ty))) {
    ssize_t length;
    Pbyte *buf;
    int buf_full;
    size_t buf_len;
    PDCI_FMT2IO_USE_FMT2BUF_FN(PDCI_MacroArg2String(fn_pref) "_fmt2io", length=P_invoke_fmt_fn(fn, pads, buf, buf_len, &buf_full, requested_out, delims, m, pd, rep, x1, x2, x3));
  }
  (*requested_out) = 1;
  return afn_pref ## _write2io(pads, io, pd, rep, x1, x2, x3);
}
/* END_MACRO */

#define PDCI_A_INT_FW_WRITE_FN_GEN(fn_pref, targ_type, wfmt, inv_type, inv_val, sfpr_macro_w)

ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, targ_type *val, size_t width)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (width > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, width)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, *val);
  if (writelen != width) {
    return -1;
  }
  memcpy(buf, PDCI_sfstr_use(pads->tmp1), writelen);
  return writelen;
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, size_t width)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, width)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, *val);
  if (writelen != width) {
    return -1;
  }
  return sfwrite(io, PDCI_sfstr_use(pads->tmp1), writelen);
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, targ_type *val, const char *tag, int indent, size_t width)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, width)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, *val);
  if (writelen != width) {
    return -1;
  }
  PDCI_BASEVAL_XML_OUT2BUF(inv_type, "%s", PDCI_sfstr_use(pads->tmp1));
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, const char *tag, int indent, size_t width)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, width)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, *val);
  if (writelen != width) {
    return -1;
  }
  PDCI_BASEVAL_XML_OUT2IO(inv_type, "%s", PDCI_sfstr_use(pads->tmp1));
}
/* END_MACRO */

#define PDCI_A_NUM_WRITE_FN_GEN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro)

ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, targ_type *val)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro(writelen, pads->tmp1, fmt, *val);
  if (writelen <= 0) {
    return -1;
  }
  if (writelen > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  memcpy(buf, PDCI_sfstr_use(pads->tmp1), writelen);
  return writelen;
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  sfpr_macro(writelen, io, fmt, *val);
  return writelen;
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, targ_type *val, const char *tag, int indent)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro(writelen, pads->tmp1, fmt, *val);
  if (writelen <= 0) {
    return -1;
  }
  PDCI_BASEVAL_XML_OUT2BUF(inv_type, "%s", PDCI_sfstr_use(pads->tmp1));
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, const char *tag, int indent)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro(writelen, pads->tmp1, fmt, *val);
  if (writelen <= 0) {
    return -1;
  }
  PDCI_BASEVAL_XML_OUT2IO(inv_type, "%s", PDCI_sfstr_use(pads->tmp1));
}
/* END_MACRO */

#define PDCI_E_INT_FW_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)

ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, targ_type *val, size_t width)
{
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, width)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _FW_buf (pads, buf, buf_len, buf_full, *val, width);
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, size_t width)
{
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, width)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _FW_io (pads, io, *val, width);
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, targ_type *val, const char *tag, int indent, size_t width)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  return a_fn_pref ## _write_xml_2buf(pads, buf, buf_len, buf_full, pd, val, tag, indent, width);
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, const char *tag, int indent, size_t width)
{
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2io", io, pd, val);
  return a_fn_pref ## _write_xml_2io(pads, io, pd, val, tag, indent, width);
}
/* END_MACRO */

#define PDCI_E_NUM_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)

ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, Pbase_pd *pd, targ_type *val)
{
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _buf (pads, buf, buf_len, buf_full, *val);
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val)
{
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _io (pads, io, *val);
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, Pbase_pd *pd, targ_type *val,
			   const char *tag, int indent)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  return a_fn_pref ## _write_xml_2buf(pads, buf, buf_len, buf_full, pd, val, tag, indent);
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, const char *tag, int indent)
{
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2io", io, pd, val);
  return a_fn_pref ## _write_xml_2io(pads, io, pd, val, tag, indent);
}
/* END_MACRO */

#define PDCI_B_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, Pbase_pd *pd, targ_type *val)
{
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _buf (pads, buf, buf_len, buf_full, *val);
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val)
{
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _io (pads, io, *val);
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, targ_type *val, const char *tag, int indent)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  return a_fn_pref ## _write_xml_2buf(pads, buf, buf_len, buf_full, pd, val, tag, indent);
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val,
			  const char *tag, int indent)
{
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2io", io, pd, val);
  return a_fn_pref ## _write_xml_2io(pads, io, pd, val, tag, indent);
}
/* END_MACRO */

#define PDCI_EBCBCDSB_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, targ_type *val, Puint32 num_digits_or_bytes)
{
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, num_digits_or_bytes)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _buf (pads, buf, buf_len, buf_full, *val, num_digits_or_bytes);
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val, Puint32 num_digits_or_bytes)
{
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, num_digits_or_bytes)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val) = inv_val;
    }
  }
  return num2pre ## _io (pads, io, *val, num_digits_or_bytes);
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, targ_type *val, const char *tag, int indent, Puint32 num_digits_or_bytes)
{
  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  return a_fn_pref ## _write_xml_2buf(pads, buf, buf_len, buf_full, pd, val, tag, indent);
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, targ_type *val,
			  const char *tag, int indent, Puint32 num_digits_or_bytes)
{
  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2io", io, pd, val);
  return a_fn_pref ## _write_xml_2io(pads, io, pd, val, tag, indent);
}
/* END_MACRO */

#define PDCI_EBCBCDSB_FPOINT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val)
ssize_t
fn_pref ## _write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, targ_type *val,
		      Puint32 num_digits_or_bytes, Puint32 d_exp)
{
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, num_digits_or_bytes, d_exp)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val).num = inv_val;
      (*val).denom = PDCI_10toThe[d_exp];
    }
  }
  if ((*val).denom != PDCI_10toThe[d_exp]) {
    if (pads->speclev == 0) {
      P_WARN2(pads->disc, PDCI_MacroArg2String(fn_pref) "_write2buf: val's denom (%llu) does not equal 10^d_exp (dex = %lu)",
		(*val).denom, d_exp);
    }
    return -1;
  }
  return num2pre ## _buf (pads, buf, buf_len, buf_full, (*val).num, num_digits_or_bytes);
}

ssize_t
fn_pref ## _write2io(P_t *pads, Sfio_t *io,
		     Pbase_pd *pd, targ_type *val, Puint32 num_digits_or_bytes, Puint32 d_exp)
{
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, num_digits_or_bytes, d_exp)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val).num = inv_val;
      (*val).denom = PDCI_10toThe[d_exp];
    }
  }
  if ((*val).denom != PDCI_10toThe[d_exp]) {
    if (pads->speclev == 0) {
      P_WARN2(pads->disc, PDCI_MacroArg2String(fn_pref) "_write2io: val's denom (%llu) does not equal 10^d_exp (dex = %lu)",
		(*val).denom, d_exp);
    }
    return -1;
  }
  return num2pre ## _io (pads, io, (*val).num, num_digits_or_bytes);
}

ssize_t
fn_pref ## _write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, targ_type *val,
			   const char *tag, int indent,
			   Puint32 num_digits_or_bytes, Puint32 d_exp)
{
  ssize_t       writelen;
  Pfloat64      d;
  Pinv_val_fn   fn;

  PDCI_DISC_4P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2buf", buf, buf_full, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, num_digits_or_bytes, d_exp)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val).num = inv_val;
      (*val).denom = PDCI_10toThe[d_exp];
    }
  }
  if ((*val).denom != PDCI_10toThe[d_exp]) {
    if (pads->speclev == 0) {
      P_WARN2(pads->disc, PDCI_MacroArg2String(fn_pref) "_write_xml_2buf: val's denom (%llu) does not equal 10^d_exp (dex = %lu)",
	      (*val).denom, d_exp);
    }
    return -1;
  }
  d = P_FPOINT2FLOAT64(*val);
  PDCI_BASEVAL_XML_OUT2BUF(inv_type, "%I8f", d);
}

ssize_t
fn_pref ## _write_xml_2io(P_t *pads, Sfio_t *io,
			  Pbase_pd *pd, targ_type *val, const char *tag, int indent, Puint32 num_digits_or_bytes, Puint32 d_exp)
{
  Pfloat64      d;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE( PDCI_MacroArg2String(fn_pref) "_write_xml_2io", io, pd, val);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    if ((!fn || P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)val, num_digits_or_bytes, d_exp)) &&
	(pd->errCode != P_USER_CONSTRAINT_VIOLATION)) {
      (*val).num = inv_val;
      (*val).denom = PDCI_10toThe[d_exp];
    }
  }
  if ((*val).denom != PDCI_10toThe[d_exp]) {
    if (pads->speclev == 0) {
      P_WARN2(pads->disc, PDCI_MacroArg2String(fn_pref) "_write_xml_2io: val's denom (%llu) does not equal 10^d_exp (dex = %lu)",
		(*val).denom, d_exp);
    }
    return -1;
  }
  d = P_FPOINT2FLOAT64(*val);
  PDCI_BASEVAL_XML_OUT2IO(inv_type, "%I8f", d);
}
/* END_MACRO */

/* ********************************* BEGIN_TRAILER ******************************** */

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_A_INT_FW > 0
#  define PDCI_A_INT_FW_WRITE_FN(fn_pref, targ_type, wfmt, inv_type, inv_val, sfpr_macro_w) \
            PDCI_A_INT_FW_WRITE_FN_GEN(fn_pref, targ_type, wfmt, inv_type, inv_val, sfpr_macro_w)
#else
#  define PDCI_A_INT_FW_WRITE_FN(fn_pref, targ_type, wfmt, inv_type, inv_val, sfpr_macro_w)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_A_INT > 0
#  define PDCI_A_INT_WRITE_FN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro) \
            PDCI_A_NUM_WRITE_FN_GEN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro)
#else
#  define PDCI_A_INT_WRITE_FN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_E_INT_FW > 0
#  define PDCI_E_INT_FW_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_E_INT_FW_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_E_INT_FW_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_E_INT > 0
#  define PDCI_E_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_E_NUM_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_E_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_B_INT > 0
#  define PDCI_B_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_B_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_B_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && (P_CONFIG_EBC_INT > 0 || P_CONFIG_EBC_FPOINT > 0)
#  define PDCI_EBC_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_EBCBCDSB_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_EBC_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && (P_CONFIG_BCD_INT > 0 || P_CONFIG_BCD_FPOINT > 0)
#  define PDCI_BCD_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_EBCBCDSB_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_BCD_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && (P_CONFIG_SBL_INT > 0 || P_CONFIG_SBL_FPOINT > 0)
#  define PDCI_SBL_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_EBCBCDSB_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_SBL_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && (P_CONFIG_SBH_INT > 0 || P_CONFIG_SBH_FPOINT > 0)
#  define PDCI_SBH_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_EBCBCDSB_INT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_SBH_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_EBC_FPOINT > 0
#  define PDCI_EBC_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val) \
            PDCI_EBCBCDSB_FPOINT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#else
#  define PDCI_EBC_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_BCD_FPOINT > 0
#  define PDCI_BCD_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val) \
            PDCI_EBCBCDSB_FPOINT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#else
#  define PDCI_BCD_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_SBL_FPOINT > 0
#  define PDCI_SBL_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val) \
            PDCI_EBCBCDSB_FPOINT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#else
#  define PDCI_SBL_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_SBH_FPOINT > 0
#  define PDCI_SBH_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val) \
            PDCI_EBCBCDSB_FPOINT_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#else
#  define PDCI_SBH_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_A_FLOAT > 0
#  define PDCI_A_FLOAT_WRITE_FN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro) \
            PDCI_A_NUM_WRITE_FN_GEN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro)
#else
#  define PDCI_A_FLOAT_WRITE_FN(fn_pref, targ_type, fmt, inv_type, inv_val, sfpr_macro)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0 && P_CONFIG_E_FLOAT > 0
#  define PDCI_E_FLOAT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref) \
            PDCI_E_NUM_WRITE_FN_GEN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#else
#  define PDCI_E_FLOAT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
#endif

#if P_CONFIG_WRITE_FUNCTIONS > 0
#  define PDCI_FMT_FN(fn_pref, afn_pref, rep_ty, lookup_ty) \
            PDCI_FMT_FN_GEN(fn_pref, afn_pref, rep_ty, lookup_ty)
#  define PDCI_FMT_FN_XTRA1(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty) \
            PDCI_FMT_FN_XTRA1_GEN(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty)
#  define PDCI_FMT_FN_XTRA1_IGNORE(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty) \
            PDCI_FMT_FN_XTRA1_IGNORE_GEN(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty)
#  define PDCI_FMT_FN_XTRA3(fn_pref, afn_pref, rep_ty, xtra_ty1, xtra_ty2, xtra_ty3, lookup_ty) \
            PDCI_FMT_FN_XTRA3_GEN(fn_pref, afn_pref, rep_ty, xtra_ty1, xtra_ty2, xtra_ty3, lookup_ty)
#else
#  define PDCI_FMT_FN(fn_pref, afn_pref, rep_ty, lookup_ty)
#  define PDCI_FMT_FN_XTRA1(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty)
#  define PDCI_FMT_FN_XTRA1_IGNORE(fn_pref, afn_pref, rep_ty, xtra_ty1, lookup_ty)
#  define PDCI_FMT_FN_XTRA3(fn_pref, afn_pref, rep_ty, xtra_ty1, xtra_ty2, xtra_ty3, lookup_ty)
#endif

/* ********************************** END_MACROS ********************************** */
/* ****************** BEGIN_MACROS(pads-acc-macros-gen.h) ********************* */
/*
 * Macros that help implement accum functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-config.h"

/* ********************************** END_HEADER ********************************** */
/* int_type:  pads int type
 * int_descr: string description of integer type.
 * num_bytes: number of bytes used to store this type.
 * fmt:       ???
 * mfmt:      ???
 * fold_test: ???
 */
#define PDCI_INT_ACCUM_GEN(int_type, int_descr, num_bytes, fmt, mfmt, fold_test)

/*
 * Order set comparison function: only used at the end to rehash
 * the (formerly unordered) set.  Since same val only occurs
 * once, ptr equivalence produces key equivalence.
 *   different keys: sort keys by cnt field, break tie with vals
 */
int
int_type ## _dt_elt_oset_cmp(Dt_t *dt, int_type ## _dt_key_t *a, int_type ## _dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a == b) { /* same key */
    return 0;
  }
  if (a->cnt == b->cnt) { /* same count, do val comparison */
    return (a->val < b->val) ? -1 : 1;
  }
  /* different counts */
  return (a->cnt > b->cnt) ? -1 : 1;
}

/*
 * Unordered set comparison function: all that matters is val equality
 * (0 => equal, 1 => not equal)
 */
int
int_type ## _dt_elt_set_cmp(Dt_t *dt, int_type ## _dt_key_t *a, int_type ## _dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a->val == b->val) {
    return 0;
  }
  return 1;
}

void*
int_type ## _dt_elt_make(Dt_t *dt, int_type ## _dt_elt_t *a, Dtdisc_t *disc)
{
  int_type ## _dt_elt_t *b;
  if ((b = oldof(0, int_type ## _dt_elt_t, 1, 0))) {
    b->key.val  = a->key.val;
    b->key.cnt  = a->key.cnt;
  }
  return b;
}

void
int_type ## _dt_elt_free(Dt_t *dt, int_type ## _dt_elt_t *a, Dtdisc_t *disc)
{
  free(a);
}

static Dtdisc_t int_type ## _acc_dt_set_disc = {
  DTOFFSET(int_type ## _dt_elt_t, key),     /* key     */
  num_bytes,                                /* size    */
  DTOFFSET(int_type ## _dt_elt_t, link),    /* link    */
  (Dtmake_f)int_type ## _dt_elt_make,       /* makef   */
  (Dtfree_f)int_type ## _dt_elt_free,       /* freef   */
  (Dtcompar_f)int_type ## _dt_elt_set_cmp,  /* comparf */
  NiL,                                      /* hashf   */
  NiL,                                      /* memoryf */
  NiL                                       /* eventf  */
};

static Dtdisc_t int_type ## _acc_dt_oset_disc = {
  DTOFFSET(int_type ## _dt_elt_t, key),     /* key     */
  num_bytes,                                /* size    */
  DTOFFSET(int_type ## _dt_elt_t, link),    /* link    */
  (Dtmake_f)int_type ## _dt_elt_make,       /* makef   */
  (Dtfree_f)int_type ## _dt_elt_free,       /* freef   */
  (Dtcompar_f)int_type ## _dt_elt_oset_cmp, /* comparf */
  NiL,                                      /* hashf   */
  NiL,                                      /* memoryf */
  NiL                                       /* eventf  */
};

Perror_t
int_type ## _acc_init(P_t *pads, int_type ## _acc *a)
{
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(int_type) "_acc_init", a);
  memset((void*)a, 0, sizeof(*a));
  if (!(a->dict = dtopen(&int_type ## _acc_dt_set_disc, Dtset))) {
    return P_ERR;
  }
  a->max2track  = pads->disc->acc_max2track;
  a->max2rep    = pads->disc->acc_max2rep;
  a->pcnt2rep   = pads->disc->acc_pcnt2rep;
  return P_OK;
}

Perror_t
int_type ## _acc_reset(P_t *pads, int_type ## _acc *a)
{
  Dt_t        *dict;

  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(int_type) "_acc_reset", a);
  if (!(dict = a->dict)) {
    return P_ERR;
  }
  memset((void*)a, 0, sizeof(*a));
  dtclear(dict);
  a->dict = dict;
  return P_OK;
}

Perror_t
int_type ## _acc_cleanup(P_t *pads, int_type ## _acc *a)
{
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(int_type) "_acc_cleanup", a);
  if (a->dict) {
    dtclose(a->dict);
    a->dict = 0;
  }
  return P_OK;
}

void
int_type ## _acc_fold_psum(int_type ## _acc *a) {
  Pfloat64 pavg, navg;
  Puint64 recent = a->good - a->fold;
  if (recent == 0) {
    return;
  }
  pavg = a->psum / (Pfloat64)recent;
  navg = ((a->avg * a->fold) + (pavg * recent))/(Pfloat64)a->good;
  /* could test for change between a->avg and navg */
  a->avg = navg;
  a->psum = 0;
  a->fold += recent;
}

Pfloat64
int_type ## _acc_avg(P_t *pads, int_type ## _acc *a) {
  int_type ## _acc_fold_psum(a);
  return a->avg;
}

int_type
int_type ## _acc_ravg(P_t *pads, int_type ## _acc *a) {
  int_type res;
  int_type ## _acc_fold_psum(a);
  if (a->avg >= 0) {
    res = (a->avg + 0.5); /* truncate( avg + 0.5) */ 
  } else {
    res = (a->avg - 0.5); /* truncate( avg - 0.5) */ 
  }
  return res;
}

Perror_t
int_type ## _acc_add(P_t *pads, int_type ## _acc *a, const Pbase_pd *pd, const int_type *val)
{
  int_type               v;
  int_type ## _dt_elt_t  insert_elt;
  int_type ## _dt_key_t  lookup_key;
  int_type ## _dt_elt_t  *tmp1;
  PDCI_DISC_3P_CHECKS( PDCI_MacroArg2String(int_type) "_acc_add", a, pd, val);
  v = (*val);
  if (!a->dict) {
    return P_ERR;
  }
  if (pd->errCode != P_NO_ERR) {
    (a->bad)++;
    return P_OK;
  }
  if (fold_test(v, a->psum)) {
    int_type ## _acc_fold_psum(a);
  }
  a->psum += v;
  (a->good)++;
  if (a->good == 1) {
    a->min = a->max = v;
  } else if (v < a->min) {
    a->min = v;
  } else if (v > a->max) {
    a->max = v;
  }
  if (v == 0 || dtsize(a->dict) < a->max2track) {
    insert_elt.key.val = v;
    insert_elt.key.cnt = 0;
    if (!(tmp1 = dtinsert(a->dict, &insert_elt))) {
      P_WARN(pads->disc, "** PADSC internal error: dtinsert failed (out of memory?) **");
      return P_ERR;
    }
    (tmp1->key.cnt)++;
    (a->tracked)++;
  } else {
    lookup_key.val = v;
    lookup_key.cnt = 0;
    if ((tmp1 = dtmatch(a->dict, &lookup_key))) {
      (tmp1->key.cnt)++;
      (a->tracked)++;
    }
  }
  return P_OK;
}

Perror_t
int_type ## _acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what, int nst,
			   int_type ## _acc *a)
{
  int                    i, sz, rp;
  Puint64                cnt_sum;
  Pfloat64               cnt_sum_pcnt;
  Pfloat64               bad_pcnt;
  Pfloat64               track_pcnt;
  Pfloat64               elt_pcnt;
  Void_t                *velt;
  int_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(int_type) "_acc_report2io called" );
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = int_descr;
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0);
  int_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (int_type ## _dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %10" fmt "\n",
	     a->good, what, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (int_type ## _dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %10" fmt "\n",
	     what, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &int_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "  Characterizing %s values:  min %" mfmt, what, a->min);
  sfprintf(outstr, " max %" mfmt, a->max);
  sfprintf(outstr, " avg %.3I8f\n", a->avg);
  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (int_type ## _dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    sfprintf(outstr, "        val: %10" fmt, elt->key.val);
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
  sfprintf(outstr,   "        SUMMING         count: %10llu  pcnt-of-good-vals: %8.3I8f\n",
	   cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &int_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
int_type ## _acc_report2xml_io(P_t *pads, Sfio_t *outstr, int nst, int_type ## _acc *a)
{
  int                    i, sz, rp;
  Pfloat64               track_pcnt;
  Void_t                *velt;
  int_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(int_type) "_acc_report2xml_io called" );
  int_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<%s><none/></%s>\n", PDCI_MacroArg2String(int_type), PDCI_MacroArg2String(int_type));
    return P_OK;
  }
  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<%s>\n", PDCI_MacroArg2String(int_type));
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%" mfmt, a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%" mfmt, a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &int_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<avg>%.3I8f</avg>\n", a->avg);
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%" mfmt, a->min);
  sfprintf(outstr, "</min>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%" mfmt, a->max);
  sfprintf(outstr, "</max>\n");
  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);
     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     };
  
     for (i = 0, velt = dtfirst(a->dict);
          velt && i < a->max2rep;
          velt = dtnext(a->dict, velt), i++) {
            elt = (int_type ## _dt_elt_t*)velt;
            PDCI_indent(outstr, nst);
            sfprintf(outstr, "<item>");
            sfprintf(outstr, "<val>%" fmt, elt->key.val);
            sfprintf(outstr, "</val>");
            sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
            sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</%s>\n", PDCI_MacroArg2String(int_type));

  dtnext(a->dict, 0); /* discard any iterator state */
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &int_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}


Perror_t
int_type ## _acc_report(P_t *pads, const char *prefix, const char *what, int nst,
			int_type ## _acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(int_type) "_acc_report", a);

  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = int_type ## _acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}
/* END_MACRO */

#define PDCI_INT_ACCUM_MAP_REPORT_GEN(int_type, int_descr, fmt, mfmt)
Perror_t
int_type ## _acc_map_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what,  int nst,
			       int_type ## _map_fn fn, int_type ## _acc *a)
{
  size_t                 pad;
  const char            *mapped_min;
  const char            *mapped_max;
  const char            *mapped_val;
  int                    i, sz, rp, tmp;
  Puint64                cnt_sum;
  Pfloat64               cnt_sum_pcnt;
  Pfloat64               bad_pcnt;
  Pfloat64               track_pcnt;
  Pfloat64               elt_pcnt;
  Void_t                *velt;
  int_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(int_type) "_acc_map_report2io called" );
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = int_descr;
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0);
  int_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (int_type ## _dt_elt_t*)dtfirst(a->dict);
    mapped_val = fn(elt->key.val);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %s (%5" fmt ")\n",
	     a->good, what, mapped_val, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (int_type ## _dt_elt_t*)dtfirst(a->dict);
    mapped_val = fn(elt->key.val);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %s (%5" fmt ")\n",
	     what, mapped_val, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &int_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  mapped_min = fn(a->min);
  mapped_max = fn(a->max);
  sfprintf(outstr, "  Characterizing %s values:  min %s (%5" mfmt, what, mapped_min, a->min);
  sfprintf(outstr, ")  max %s (%5" mfmt, mapped_max, a->max);
  sfprintf(outstr, ")\n");
  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  sz = tmp = 0;
  for (i = 0, velt = dtfirst(a->dict); velt && i < a->max2rep; velt = dtnext(a->dict, velt), i++) {
    elt = (int_type ## _dt_elt_t*)velt;
    sz = strlen(fn(elt->key.val));
    if (sz > tmp) {
      tmp = sz; 
    }
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    dtnext(a->dict, 0); /* discard any iterator state */
    elt = (int_type ## _dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    mapped_val = fn(elt->key.val);
    sfprintf(outstr, "        val: %s (%5" fmt, mapped_val, elt->key.val);
    sfprintf(outstr, ") ");
    pad = tmp-strlen(mapped_val);
    sfprintf(outstr, "%-.*s", pad, PDCI_spaces);
    sfprintf(outstr, "  count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  sfprintf(outstr,   "%-.*s", tmp,
	   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .");
  sfprintf(outstr,   " . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n        SUMMING");
  sfprintf(outstr,   "%-.*s", tmp, PDCI_spaces);
  sfprintf(outstr,   "         count: %10llu  pcnt-of-good-vals: %8.3I8f\n", cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &int_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
int_type ## _acc_map_report(P_t *pads, const char *prefix, const char *what, int nst,
			    int_type ## _map_fn fn, int_type ## _acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(int_type) "_acc_map_report", a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = int_type ## _acc_map_report2io(pads, tmpstr, prefix, what, nst, fn, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}

Perror_t
int_type ## _acc_map_report2xml_io(P_t *pads, Sfio_t *outstr, int nst,
			           int_type ## _map_fn fn, int_type ## _acc *a)
{
  const char            *mapped_min;
  const char            *mapped_max;
  const char            *mapped_val;
  int                    i, sz, rp, tmp;
  Pfloat64               track_pcnt;
  Void_t                *velt;
  int_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(int_type) "_acc_map_report2xml_io called" );
  int_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    return P_OK;
  }
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%" mfmt, a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%" mfmt, a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &int_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  mapped_min = fn(a->min);
  mapped_max = fn(a->max);

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%s</min>\n", mapped_min);
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%s</max>\n", mapped_max);

  if (sz != 0) {
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);
     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     }
     sz = tmp = 0;
     for (i = 0, velt = dtfirst(a->dict); velt && i < a->max2rep; velt = dtnext(a->dict, velt), i++) {
       elt = (int_type ## _dt_elt_t*)velt;
       sz = strlen(fn(elt->key.val));
       if (sz > tmp) {
	 tmp = sz; 
       }
     }
     dtnext(a->dict, 0); /* discard any iterator state */
    for (i = 0, velt = dtfirst(a->dict);
	 velt && i < a->max2rep;
	 velt = dtnext(a->dict, velt), i++) {
      dtnext(a->dict, 0); /* discard any iterator state */
      elt = (int_type ## _dt_elt_t*)velt;
      mapped_val = fn(elt->key.val);
      PDCI_indent(outstr, nst);
      sfprintf(outstr, "<item>");
      sfprintf(outstr, "<val>%s</val>", mapped_val);
      sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
      sfprintf(outstr, "</item>\n");
    }
    nst--;
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "</distribution>\n");
  }

  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &int_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

/* END_MACRO */

#define PDCI_FPOINT_ACCUM_GEN(fpoint_type, fpoint_descr)

typedef struct fpoint_type ## _dt_key_s {
  Pfloat64     val;
  Puint64      cnt;
} fpoint_type ## _dt_key_t;

typedef struct fpoint_type ## _dt_elt_s {
  fpoint_type ## _dt_key_t key;
  Dtlink_t link;
} fpoint_type ## _dt_elt_t;

/*
 * Order set comparison function: only used at the end to rehash
 * the (formerly unordered) set.  Since same val only occurs
 * once, ptr equivalence produces key equivalence.
 *   different keys: sort keys by cnt field, break tie with vals
 */
int
fpoint_type ## _dt_elt_oset_cmp(Dt_t *dt, fpoint_type ## _dt_key_t *a, fpoint_type ## _dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a == b) { /* same key */
    return 0;
  }
  if (a->cnt == b->cnt) { /* same count, do val comparison */
    return (a->val < b->val) ? -1 : 1;
  }
  /* different counts */
  return (a->cnt > b->cnt) ? -1 : 1;
}

/*
 * Unordered set comparison function: all that matters is val equality
 * (0 => equal, 1 => not equal)
 */
int
fpoint_type ## _dt_elt_set_cmp(Dt_t *dt, fpoint_type ## _dt_key_t *a, fpoint_type ## _dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a->val == b->val) {
    return 0;
  }
  return 1;
}

void*
fpoint_type ## _dt_elt_make(Dt_t *dt, fpoint_type ## _dt_elt_t *a, Dtdisc_t *disc)
{
  fpoint_type ## _dt_elt_t *b;
  if ((b = oldof(0, fpoint_type ## _dt_elt_t, 1, 0))) {
    b->key.val  = a->key.val;
    b->key.cnt  = a->key.cnt;
  }
  return b;
}

void
fpoint_type ## _dt_elt_free(Dt_t *dt, fpoint_type ## _dt_elt_t *a, Dtdisc_t *disc)
{
  free(a);
}

static Dtdisc_t fpoint_type ## _acc_dt_set_disc = {
  DTOFFSET(fpoint_type ## _dt_elt_t, key),     /* key     */
  sizeof(Pfloat64),                            /* size    */
  DTOFFSET(fpoint_type ## _dt_elt_t, link),    /* link    */
  (Dtmake_f)fpoint_type ## _dt_elt_make,       /* makef   */
  (Dtfree_f)fpoint_type ## _dt_elt_free,       /* freef   */
  (Dtcompar_f)fpoint_type ## _dt_elt_set_cmp,  /* comparf */
  NiL,                                         /* hashf   */
  NiL,                                         /* memoryf */
  NiL                                          /* eventf  */
};

static Dtdisc_t fpoint_type ## _acc_dt_oset_disc = {
  DTOFFSET(fpoint_type ## _dt_elt_t, key),     /* key     */
  sizeof(Pfloat64),                            /* size    */
  DTOFFSET(fpoint_type ## _dt_elt_t, link),    /* link    */
  (Dtmake_f)fpoint_type ## _dt_elt_make,       /* makef   */
  (Dtfree_f)fpoint_type ## _dt_elt_free,       /* freef   */
  (Dtcompar_f)fpoint_type ## _dt_elt_oset_cmp, /* comparf */
  NiL,                                         /* hashf   */
  NiL,                                         /* memoryf */
  NiL                                          /* eventf  */
};

Perror_t
fpoint_type ## _acc_init(P_t *pads, fpoint_type ## _acc *a)
{
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(fpoint_type) "_acc_init", a);
  memset((void*)a, 0, sizeof(*a));
  if (!(a->dict = dtopen(&fpoint_type ## _acc_dt_set_disc, Dtset))) {
    return P_ERR;
  }
  a->max2track  = pads->disc->acc_max2track;
  a->max2rep    = pads->disc->acc_max2rep;
  a->pcnt2rep   = pads->disc->acc_pcnt2rep;
  return P_OK;
}

Perror_t
fpoint_type ## _acc_reset(P_t *pads, fpoint_type ## _acc *a)
{
  Dt_t        *dict;

  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(fpoint_type) "_acc_reset", a);
  if (!(dict = a->dict)) {
    return P_ERR;
  }
  memset((void*)a, 0, sizeof(*a));
  dtclear(dict);
  a->dict = dict;
  return P_OK;
}

Perror_t
fpoint_type ## _acc_cleanup(P_t *pads, fpoint_type ## _acc *a)
{
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(fpoint_type) "_acc_cleanup", a);
  if (a->dict) {
    dtclose(a->dict);
    a->dict = 0;
  }
  return P_OK;
}

void
fpoint_type ## _acc_fold_psum(fpoint_type ## _acc *a) {
  Pfloat64 pavg, navg;
  Puint64 recent = a->good - a->fold;
  if (recent == 0) {
    return;
  }
  pavg = a->psum / (Pfloat64)recent;
  navg = ((a->avg * a->fold) + (pavg * recent))/(Pfloat64)a->good;
  /* could test for change between a->avg and navg */
  a->avg = navg;
  a->psum = 0;
  a->fold += recent;
}

Pfloat64
fpoint_type ## _acc_avg(P_t *pads, fpoint_type ## _acc *a) {
  fpoint_type ## _acc_fold_psum(a);
  return a->avg;
}

Perror_t
fpoint_type ## _acc_add(P_t *pads, fpoint_type ## _acc *a, const Pbase_pd *pd, const fpoint_type *val)
{
  Pfloat64                  v;
  fpoint_type ## _dt_elt_t  insert_elt;
  fpoint_type ## _dt_key_t  lookup_key;
  fpoint_type ## _dt_elt_t  *tmp1;
  PDCI_DISC_3P_CHECKS( PDCI_MacroArg2String(fpoint_type) "_acc_add", a, pd, val);
  v = P_FPOINT2FLOAT64(*val);
  if (!a->dict) {
    return P_ERR;
  }
  if (pd->errCode != P_NO_ERR) {
    (a->bad)++;
    return P_OK;
  }
  if ( (v > 0 && a->psum > PDCI_LARGE_POS_DBL) ||
       (v < 0 && a->psum < PDCI_LARGE_NEG_DBL) ) {
    fpoint_type ## _acc_fold_psum(a);
  }
  a->psum += v;
  (a->good)++;
  if (a->good == 1) {
    a->min = a->max = v;
  } else if (v < a->min) {
    a->min = v;
  } else if (v > a->max) {
    a->max = v;
  }
  if (v == 0 || dtsize(a->dict) < a->max2track) {
    insert_elt.key.val = v;
    insert_elt.key.cnt = 0;
    if (!(tmp1 = dtinsert(a->dict, &insert_elt))) {
      P_WARN(pads->disc, "** PADSC internal error: dtinsert failed (out of memory?) **");
      return P_ERR;
    }
    (tmp1->key.cnt)++;
    (a->tracked)++;
  } else {
    lookup_key.val = v;
    lookup_key.cnt = 0;
    if ((tmp1 = dtmatch(a->dict, &lookup_key))) {
      (tmp1->key.cnt)++;
      (a->tracked)++;
    }
  }
  return P_OK;
}

Perror_t
fpoint_type ## _acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what, int nst,
			      fpoint_type ## _acc *a)
{
  int                       i, sz, rp;
  Puint64                   cnt_sum;
  Pfloat64                  cnt_sum_pcnt;
  Pfloat64                  bad_pcnt;
  Pfloat64                  track_pcnt;
  Pfloat64                  elt_pcnt;
  Void_t                   *velt;
  fpoint_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(fpoint_type) "_acc_report2io called" );
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = fpoint_descr;
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0);
  fpoint_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (fpoint_type ## _dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %10.5I8f\n",
	     a->good, what, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (fpoint_type ## _dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %10.5I8f\n",
	     what, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &fpoint_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "  Characterizing %s values:  min %.5I8f", what, a->min);
  sfprintf(outstr, " max %.5I8f", a->max);
  sfprintf(outstr, " avg %.3I8f\n", a->avg);
  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (fpoint_type ## _dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    sfprintf(outstr, "        val: %10.5I8f", elt->key.val);
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
  sfprintf(outstr,   "        SUMMING         count: %10llu  pcnt-of-good-vals: %8.3I8f\n",
	   cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &fpoint_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
fpoint_type ## _acc_report2xml_io(P_t *pads, Sfio_t *outstr, int nst,fpoint_type ## _acc *a)
{
  int                       i, sz, rp;
  Pfloat64                  track_pcnt;
  Void_t                   *velt;
  fpoint_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(fpoint_type) "_acc_report2xml_io called" );
  fpoint_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<%s><none/></%s>\n", PDCI_MacroArg2String(fpoint_type), PDCI_MacroArg2String(fpoint_type));
    return P_OK;
  }
  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<%s>\n", PDCI_MacroArg2String(fpoint_type));
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%llu", a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%llu", a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &fpoint_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<avg>%.3I8f</avg>\n", a->avg);
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%.5I8f", a->min);
  sfprintf(outstr, "</min>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%.5I8f", a->max);
  sfprintf(outstr, "</max>\n");

  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);
     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     };

     for (i = 0, velt = dtfirst(a->dict);
	  velt && i < a->max2rep;
	  velt = dtnext(a->dict, velt), i++) {
            elt = (fpoint_type ## _dt_elt_t*)velt;
            PDCI_indent(outstr, nst);
            sfprintf(outstr, "<item>");
            sfprintf(outstr, "<val>%.5I8f", elt->key.val);
            sfprintf(outstr, "</val>");
            sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
            sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</%s>\n", PDCI_MacroArg2String(fpoint_type));

  dtnext(a->dict, 0); /* discard any iterator state */
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &fpoint_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}


Perror_t
fpoint_type ## _acc_report(P_t *pads, const char *prefix, const char *what, int nst,
			   fpoint_type ## _acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(fpoint_type) "_acc_report", a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = fpoint_type ## _acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}
/* END_MACRO */

#define PDCI_FLOAT_ACCUM_GEN(float_type, float_descr)

typedef struct float_type ## _dt_key_s {
  Pfloat64     val;
  Puint64      cnt;
} float_type ## _dt_key_t;

typedef struct float_type ## _dt_elt_s {
  float_type ## _dt_key_t key;
  Dtlink_t link;
} float_type ## _dt_elt_t;

/*
 * Order set comparison function: only used at the end to rehash
 * the (formerly unordered) set.  Since same val only occurs
 * once, ptr equivalence produces key equivalence.
 *   different keys: sort keys by cnt field, break tie with vals
 */
int
float_type ## _dt_elt_oset_cmp(Dt_t *dt, float_type ## _dt_key_t *a, float_type ## _dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a == b) { /* same key */
    return 0;
  }
  if (a->cnt == b->cnt) { /* same count, do val comparison */
    return (a->val < b->val) ? -1 : 1;
  }
  /* different counts */
  return (a->cnt > b->cnt) ? -1 : 1;
}

/*
 * Unordered set comparison function: all that matters is val equality
 * (0 => equal, 1 => not equal)
 */
int
float_type ## _dt_elt_set_cmp(Dt_t *dt, float_type ## _dt_key_t *a, float_type ## _dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a->val == b->val) {
    return 0;
  }
  return 1;
}

void*
float_type ## _dt_elt_make(Dt_t *dt, float_type ## _dt_elt_t *a, Dtdisc_t *disc)
{
  float_type ## _dt_elt_t *b;
  if ((b = oldof(0, float_type ## _dt_elt_t, 1, 0))) {
    b->key.val  = a->key.val;
    b->key.cnt  = a->key.cnt;
  }
  return b;
}

void
float_type ## _dt_elt_free(Dt_t *dt, float_type ## _dt_elt_t *a, Dtdisc_t *disc)
{
  free(a);
}

static Dtdisc_t float_type ## _acc_dt_set_disc = {
  DTOFFSET(float_type ## _dt_elt_t, key),     /* key     */
  sizeof(Pfloat64),                           /* size    */
  DTOFFSET(float_type ## _dt_elt_t, link),    /* link    */
  (Dtmake_f)float_type ## _dt_elt_make,       /* makef   */
  (Dtfree_f)float_type ## _dt_elt_free,       /* freef   */
  (Dtcompar_f)float_type ## _dt_elt_set_cmp,  /* comparf */
  NiL,                                        /* hashf   */
  NiL,                                        /* memoryf */
  NiL                                         /* eventf  */
};

static Dtdisc_t float_type ## _acc_dt_oset_disc = {
  DTOFFSET(float_type ## _dt_elt_t, key),     /* key     */
  sizeof(Pfloat64),                           /* size    */
  DTOFFSET(float_type ## _dt_elt_t, link),    /* link    */
  (Dtmake_f)float_type ## _dt_elt_make,       /* makef   */
  (Dtfree_f)float_type ## _dt_elt_free,       /* freef   */
  (Dtcompar_f)float_type ## _dt_elt_oset_cmp, /* comparf */
  NiL,                                        /* hashf   */
  NiL,                                        /* memoryf */
  NiL                                         /* eventf  */
};

Perror_t
float_type ## _acc_init(P_t *pads, float_type ## _acc *a)
{
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(float_type) "_acc_init", a);
  memset((void*)a, 0, sizeof(*a));
  if (!(a->dict = dtopen(&float_type ## _acc_dt_set_disc, Dtset))) {
    return P_ERR;
  }
  a->max2track  = pads->disc->acc_max2track;
  a->max2rep    = pads->disc->acc_max2rep;
  a->pcnt2rep   = pads->disc->acc_pcnt2rep;
  return P_OK;
}

Perror_t
float_type ## _acc_reset(P_t *pads, float_type ## _acc *a)
{
  Dt_t        *dict;

  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(float_type) "_acc_reset", a);
  if (!(dict = a->dict)) {
    return P_ERR;
  }
  memset((void*)a, 0, sizeof(*a));
  dtclear(dict);
  a->dict = dict;
  return P_OK;
}

Perror_t
float_type ## _acc_cleanup(P_t *pads, float_type ## _acc *a)
{
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(float_type) "_acc_cleanup", a);
  if (a->dict) {
    dtclose(a->dict);
    a->dict = 0;
  }
  return P_OK;
}

void
float_type ## _acc_fold_psum(float_type ## _acc *a) {
  Pfloat64 pavg, navg;
  Puint64 recent = a->good - a->fold;
  if (recent == 0) {
    return;
  }
  pavg = a->psum / (Pfloat64)recent;
  navg = ((a->avg * a->fold) + (pavg * recent))/(Pfloat64)a->good;
  /* could test for change between a->avg and navg */
  a->avg = navg;
  a->psum = 0;
  a->fold += recent;
}

Pfloat64
float_type ## _acc_avg(P_t *pads, float_type ## _acc *a) {
  float_type ## _acc_fold_psum(a);
  return a->avg;
}

Perror_t
float_type ## _acc_add(P_t *pads, float_type ## _acc *a, const Pbase_pd *pd, const float_type *val)
{
  Pfloat64                 v;
  float_type ## _dt_elt_t  insert_elt;
  float_type ## _dt_key_t  lookup_key;
  float_type ## _dt_elt_t  *tmp1;
  PDCI_DISC_3P_CHECKS( PDCI_MacroArg2String(float_type) "_acc_add", a, pd, val);
  v = (*val);
  if (!a->dict) {
    return P_ERR;
  }
  if (pd->errCode != P_NO_ERR) {
    (a->bad)++;
    return P_OK;
  }
  if ( (v > 0 && (v > PDCI_LARGE_POS_DBL || a->psum > PDCI_LARGE_POS_DBL)) ||
       (v < 0 && (v < PDCI_LARGE_NEG_DBL || a->psum < PDCI_LARGE_NEG_DBL)) ) {
    float_type ## _acc_fold_psum(a);
  }
  a->psum += v;
  (a->good)++;
  if (a->good == 1) {
    a->min = a->max = v;
  } else if (v < a->min) {
    a->min = v;
  } else if (v > a->max) {
    a->max = v;
  }
  if (v == 0 || dtsize(a->dict) < a->max2track) {
    insert_elt.key.val = v;
    insert_elt.key.cnt = 0;
    if (!(tmp1 = dtinsert(a->dict, &insert_elt))) {
      P_WARN(pads->disc, "** PADSC internal error: dtinsert failed (out of memory?) **");
      return P_ERR;
    }
    (tmp1->key.cnt)++;
    (a->tracked)++;
  } else {
    lookup_key.val = v;
    lookup_key.cnt = 0;
    if ((tmp1 = dtmatch(a->dict, &lookup_key))) {
      (tmp1->key.cnt)++;
      (a->tracked)++;
    }
  }
  return P_OK;
}

Perror_t
float_type ## _acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what, int nst,
			     float_type ## _acc *a)
{
  int                      i, sz, rp;
  Puint64                  cnt_sum;
  Pfloat64                 cnt_sum_pcnt;
  Pfloat64                 bad_pcnt;
  Pfloat64                 track_pcnt;
  Pfloat64                 elt_pcnt;
  Void_t                  *velt;
  float_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(float_type) "_acc_report2io called" );
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = float_descr;
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0);
  float_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (float_type ## _dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %I810.5f\n",
	     a->good, what, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %I88.3f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (float_type ## _dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %I810.5f\n",
	     what, elt->key.val);
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &float_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "  Characterizing %s values:  min %I8.5f", what, a->min);
  sfprintf(outstr, " max %I8.5f", a->max);
  sfprintf(outstr, " avg %I8.3f\n", a->avg);
  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %I8.3f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %I8.2f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (float_type ## _dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    sfprintf(outstr, "        val: %I810.5f", elt->key.val);
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %I88.3f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
  sfprintf(outstr,   "        SUMMING         count: %10llu  pcnt-of-good-vals: %I88.3f\n",
	   cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &float_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}


Perror_t
float_type ## _acc_report2xml_io(P_t *pads, Sfio_t *outstr, int nst, float_type ## _acc *a)
{
  int                      i, sz, rp;
  Pfloat64                 track_pcnt;
  Void_t                  *velt;
  float_type ## _dt_elt_t *elt;

  P_TRACE(pads->disc, PDCI_MacroArg2String(float_type) "_acc_report2xml_io called" );
  float_type ## _acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<%s><none/></%s>\n", PDCI_MacroArg2String(float_type), PDCI_MacroArg2String(float_type));
    return P_OK;
  }

  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<%s>\n", PDCI_MacroArg2String(float_type));
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%llu", a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%llu", a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &float_type ## _acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<avg>%.3I8f</avg>\n", a->avg);
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%.5I8f", a->min);
  sfprintf(outstr, "</min>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%.5I8f", a->max);
  sfprintf(outstr, "</max>\n");

  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);

     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     };

     for (i = 0, velt = dtfirst(a->dict);
          velt && i < a->max2rep;
          velt = dtnext(a->dict, velt), i++) {
       elt = (float_type ## _dt_elt_t*)velt;
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<item>");
       sfprintf(outstr, "<val>%I810.5f", elt->key.val);
       sfprintf(outstr, "</val>");
       sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
       sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</%s>\n", PDCI_MacroArg2String(float_type));

  dtnext(a->dict, 0); /* discard any iterator state */
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &float_type ## _acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
float_type ## _acc_report(P_t *pads, const char *prefix, const char *what, int nst,
			  float_type ## _acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS( PDCI_MacroArg2String(float_type) "_acc_report", a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = float_type ## _acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}
/* END_MACRO */

/* ********************************* BEGIN_TRAILER ******************************** */

#if P_CONFIG_ACCUM_FUNCTIONS > 0
#  define PDCI_INT_ACCUM(int_type, int_descr, num_bytes, fmt, mfmt, fold_test) \
            PDCI_INT_ACCUM_GEN(int_type, int_descr, num_bytes, fmt, mfmt, fold_test)
#  define PDCI_INT_ACCUM_MAP_REPORT(int_type, int_descr, fmt, mfmt) \
            PDCI_INT_ACCUM_MAP_REPORT_GEN(int_type, int_descr, fmt, mfmt)
#  define PDCI_FPOINT_ACCUM(fpoint_type, fpoint_descr) \
            PDCI_FPOINT_ACCUM_GEN(fpoint_type, fpoint_descr)
#  define PDCI_FLOAT_ACCUM(float_type, float_descr) \
            PDCI_FLOAT_ACCUM_GEN(float_type, float_descr)
#else
#  define PDCI_INT_ACCUM(int_type, int_descr, num_bytes, fmt, mfmt, fold_test)
#  define PDCI_INT_ACCUM_MAP_REPORT(int_type, int_descr, fmt, mfmt)
#  define PDCI_FPOINT_ACCUM(fpoint_type, fpoint_descr)
#  define PDCI_FLOAT_ACCUM(float_type, float_descr)
#endif

/* ********************************** END_MACROS ********************************** */

/* ****************** BEGIN_MACROS(pads-misc-macros-gen.h) ********************* */
/*
 * Macros that help implement accum functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-config.h"

/* ********************************** END_HEADER ********************************** */

#define PDCI_A2INT_GEN(fn_name, targ_type, int_min, int_max, Pstring2int_fn)
targ_type
fn_name(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  int  neg = 0, range_err = 0;
  targ_type res = 0;

  while (PDCI_is_a_space(*bytes)) {
    bytes++;
  }
  if (*bytes == '+') {
    bytes++;
  } else if (*bytes == '-') {
    bytes++;
    neg = 1;
  }
  if (!PDCI_is_a_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ascii_digit[*bytes]) != -1) {
    if (res < int_min ## _DIV10) {
      range_err = 1;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res < int_min + digit) {
      range_err = 1;
    }
    res -= digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  if (range_err) {
    errno = ERANGE;
    return neg ? int_min : int_max;
  }
  errno = 0;
  return neg ? res : - res;
}

targ_type
fn_name ## _max_bytes(const Pbyte *bytes, size_t max_bytes, Pbyte **ptr_out)
{
  int digit;
  int  neg = 0, range_err = 0;
  targ_type res = 0;

  while (max_bytes > 0 && PDCI_is_a_space(*bytes)) {
    bytes++;
    max_bytes--;
  }
  if (max_bytes == 0) {
    errno = EINVAL;
    return 0;
  }
  if (*bytes == '+') {
    bytes++;
    max_bytes--;
  } else if (*bytes == '-') {
    bytes++;
    max_bytes--;
    neg = 1;
  }
  if (max_bytes == 0) {
    errno = EINVAL;
    return 0;
  }
  if (!PDCI_is_a_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while (max_bytes > 0 && ((digit = PDCI_ascii_digit[*bytes]) != -1)) {
    if (res < int_min ## _DIV10) {
      range_err = 1;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res < int_min + digit) {
      range_err = 1;
    }
    res -= digit;
    bytes++;
    max_bytes--;
  }
  (*ptr_out) = (Pbyte*)bytes;
  if (range_err) {
    errno = ERANGE;
    return neg ? int_min : int_max;
  }
  errno = 0;
  return neg ? res : - res;
}

targ_type
Pstring2int_fn(const Pstring *s)
{
  Pbyte *bytes, *ptr;
  size_t max_bytes;
  targ_type res;

  if (!s) {
    errno = EINVAL;
    return 0;
  }
  ptr = bytes = (Pbyte*)s->str;
  max_bytes = s->len;
  res = fn_name ## _max_bytes(bytes, max_bytes, &ptr);
  if (errno) return res;
  while (ptr - bytes < max_bytes && PDCI_is_a_space(*ptr)) {
    ptr++;
  }
  if (ptr - bytes != max_bytes) {
    errno = EINVAL;
    return 0;
  }
  return res;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  int  neg = 0;
  targ_type res = 0;

  while (PDCI_is_a_space(*bytes)) {
    bytes++;
  }
  if (*bytes == '+') {
    bytes++;
  } else if (*bytes == '-') {
    bytes++;
    neg = 1;
  }
  if (!PDCI_is_a_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ascii_digit[*bytes]) != -1) {
    res = (res << 3) + (res << 1); /* res *= 10 */
    res -= digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  errno = 0;
  return neg ? res : - res;
}
/* END_MACRO */

#define PDCI_A2UINT_GEN(fn_name, targ_type, int_max, Pstring2int_fn)
targ_type
fn_name(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  int  range_err = 0;
  targ_type res = 0;

  while (PDCI_is_a_space(*bytes)) {
    bytes++;
  }
  if (*bytes == '+') {
    bytes++;
  } else if (*bytes == '-') {
    bytes++;
    range_err = 1;
  }
  if (!PDCI_is_a_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ascii_digit[*bytes]) != -1) {
    if (res > int_max ## _DIV10) {
      range_err = 1;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res > int_max - digit) {
      range_err = 1;
    }
    res += digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  if (range_err) {
    errno = ERANGE;
    return int_max;
  }
  errno = 0;
  return res;
}

targ_type
fn_name ## _max_bytes(const Pbyte *bytes, size_t max_bytes, Pbyte **ptr_out)
{
  int digit;
  int  range_err = 0;
  targ_type res = 0;

  while (max_bytes > 0 && PDCI_is_a_space(*bytes)) {
    bytes++;
    max_bytes--;
  }
  if (max_bytes == 0) {
    errno = EINVAL;
    return 0;
  }
  if (*bytes == '+') {
    bytes++;
    max_bytes--;
  } else if (*bytes == '-') {
    bytes++;
    max_bytes--;
    range_err = 1;
  }
  if (max_bytes == 0) {
    errno = EINVAL;
    return 0;
  }
  if (!PDCI_is_a_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while (max_bytes > 0 && ((digit = PDCI_ascii_digit[*bytes]) != -1)) {
    if (res > int_max ## _DIV10) {
      range_err = 1;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res > int_max - digit) {
      range_err = 1;
    }
    res += digit;
    bytes++;
    max_bytes--;
  }
  (*ptr_out) = (Pbyte*)bytes;
  if (range_err) {
    errno = ERANGE;
    return int_max;
  }
  errno = 0;
  return res;
}

targ_type
Pstring2int_fn(const Pstring *s)
{
  Pbyte *bytes, *ptr;
  size_t max_bytes;
  targ_type res;

  if (!s) {
    errno = EINVAL;
    return 0;
  }
  ptr = bytes = (Pbyte*)s->str;
  max_bytes = s->len;
  res = fn_name ## _max_bytes(bytes, max_bytes, &ptr);
  if (errno) return res;
  while (ptr - bytes < max_bytes && PDCI_is_a_space(*ptr)) {
    ptr++;
  }
  if (ptr - bytes != max_bytes) {
    errno = EINVAL;
    return 0;
  }
  return res;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  targ_type res = 0;

  while (PDCI_is_a_space(*bytes)) {
    bytes++;
  }
  if (*bytes == '+') {
    bytes++;
  }
  if (!PDCI_is_a_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ascii_digit[*bytes]) != -1) {
    res = (res << 3) + (res << 1); /* res *= 10 */
    res += digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  errno = 0;
  return res;
}
/* END_MACRO */

#define PDCI_NUM2A_GEN(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i)
{
  ssize_t writelen;

  errno = 0;
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro(writelen, pads->tmp1, fmt, i);
  if (writelen <= 0) {
    return -1;
  }
  if (writelen > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    return -1;
  }
  memcpy(outbuf, PDCI_sfstr_use(pads->tmp1), writelen);
  return writelen;
}

ssize_t
rev_fn_name ## _FW_buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i, size_t width)
{
  ssize_t writelen;

  errno = 0;
  if (width > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    return -1;
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, i);
  if (writelen != width) {
    return -1;
  }
  memcpy(outbuf, PDCI_sfstr_use(pads->tmp1), writelen);
  return writelen;
}

ssize_t
rev_fn_name ## _io(P_t *pads, Sfio_t *io, targ_type i)
{
  ssize_t writelen;

  errno = 0;
  sfpr_macro(writelen, io, fmt, i);
  return writelen;
}

ssize_t
rev_fn_name ## _FW_io(P_t *pads, Sfio_t *io, targ_type i, size_t width)
{
  ssize_t writelen;

  errno = 0;
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, i);
  if (writelen != width) {
    return -1;
  }
  return sfwrite(io, PDCI_sfstr_use(pads->tmp1), writelen);
}
/* END_MACRO */

#define PDCI_E2INT_GEN(fn_name, targ_type, int_min, int_max)
targ_type
fn_name(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  int  neg = 0, range_err = 0;
  targ_type res = 0;

  while (PDCI_is_e_space(*bytes)) {
    bytes++;
  }
  if (*bytes == P_EBCDIC_PLUS) {
    bytes++;
  } else if (*bytes == P_EBCDIC_MINUS) {
    bytes++;
    neg = 1;
  }
  if (!PDCI_is_e_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ebcdic_digit[*bytes]) != -1) {
    if (res < int_min ## _DIV10) {
      range_err = 1;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res < int_min + digit) {
      range_err = 1;
    }
    res -= digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  if (range_err) {
    errno = ERANGE;
    return neg ? int_min : int_max;
  }
  errno = 0;
  return neg ? res : - res;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  int  neg = 0;
  targ_type res = 0;

  while (PDCI_is_e_space(*bytes)) {
    bytes++;
  }
  if (*bytes == P_EBCDIC_PLUS) {
    bytes++;
  } else if (*bytes == P_EBCDIC_MINUS) {
    bytes++;
    neg = 1;
  }
  if (!PDCI_is_e_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ebcdic_digit[*bytes]) != -1) {
    res = (res << 3) + (res << 1); /* res *= 10 */
    res -= digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  errno = 0;
  return neg ? res : - res;
}
/* END_MACRO */

#define PDCI_E2UINT_GEN(fn_name, targ_type, int_max)
targ_type
fn_name(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  int range_err = 0;
  targ_type res = 0;

  while (PDCI_is_e_space(*bytes)) {
    bytes++;
  }
  if (*bytes == P_EBCDIC_PLUS) {
    bytes++;
  } else if (*bytes == P_EBCDIC_MINUS) {
    bytes++;
    range_err = 1;
  }
  if (!PDCI_is_e_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ebcdic_digit[*bytes]) != -1) {
    if (res > int_max ## _DIV10) {
      range_err = 1;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res > int_max - digit) {
      range_err = 1;
    }
    res += digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  if (range_err) {
    errno = ERANGE;
    return int_max;
  }
  errno = 0;
  return res;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Pbyte **ptr_out)
{
  int digit;
  targ_type res = 0;

  while (PDCI_is_e_space(*bytes)) {
    bytes++;
  }
  if (*bytes == P_EBCDIC_PLUS) {
    bytes++;
  }
  if (!PDCI_is_e_digit(*bytes)) {
    (*ptr_out) = (Pbyte*)bytes;
    errno = EINVAL;
    return 0;
  }
  while ((digit = PDCI_ebcdic_digit[*bytes]) != -1) {
    res = (res << 3) + (res << 1); /* res *= 10 */
    res += digit;
    bytes++;
  }
  (*ptr_out) = (Pbyte*)bytes;
  errno = 0;
  return res;
}
/* END_MACRO */

#define PDCI_NUM2E_GEN(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i)
{
  ssize_t  j, writelen;
  char    *buf;

  errno = 0;
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro(writelen, pads->tmp1, fmt, i);
  if (writelen <= 0) {
    return -1;
  }
  if (writelen > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    return -1;
  }
  buf = PDCI_sfstr_use(pads->tmp1);
  for (j = 0; j < writelen; j++) {
    outbuf[j] = P_mod_ae_tab[(int)(buf[j])];
  }
  return writelen;
}

ssize_t
rev_fn_name ## _FW_buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i, size_t width)
{
  ssize_t  j, writelen;
  char    *buf;

  errno = 0;
  if (width > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    return -1;
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, i);
  if (writelen != width) {
    return -1;
  }
  buf = PDCI_sfstr_use(pads->tmp1);
  for (j = 0; j < writelen; j++) {
    outbuf[j] = P_mod_ae_tab[(int)(buf[j])];
  }
  return writelen;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type i)
{
  ssize_t  j, writelen;
  char    *buf;

  errno = 0;
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro(writelen, pads->tmp1, fmt, i);
  if (-1 == writelen) return -1;
  buf = PDCI_sfstr_use(pads->tmp1);
  for (j = 0; j < writelen; j++) {
    buf[j] = P_mod_ae_tab[(int)(buf[j])];
  }
  return sfwrite(io, buf, writelen);
}

ssize_t
rev_fn_name ## _FW_io (P_t *pads, Sfio_t *io, targ_type i, size_t width)
{
  ssize_t  j, writelen;
  char    *buf;

  errno = 0;
  PDCI_sfstr_seek2zero(pads->tmp1);
  sfpr_macro_w(writelen, pads->tmp1, wfmt, width, i);
  if (writelen != width) {
    return -1;
  }
  buf = PDCI_sfstr_use(pads->tmp1);
  for (j = 0; j < writelen; j++) {
    buf[j] = P_mod_ae_tab[(int)(buf[j])];
  }
  return sfwrite(io, buf, writelen);
}
/* END_MACRO */

#define PDCI_INT1_2B_GEN(rev_fn_name, targ_type)
ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i)
{
  if (!outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  /* on-disk order always same as in-memory rep */
  outbuf[0] = *((Pbyte*)(&i));
  errno = 0;
  return 1;
}

ssize_t
rev_fn_name ## _io(P_t *pads, Sfio_t *io, targ_type i)
{
  Pbyte *ibytes = (Pbyte*)(&i);
  /* on-disk order always same as in-memory rep */
  errno = 0;
  return sfwrite(io, ibytes, 1);
}
/* END_MACRO */

#define PDCI_INT2B_GEN(rev_fn_name, targ_type, num_bytes)
ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i)
{
  Pbyte *ibytes = (Pbyte*)(&i);

  if (num_bytes > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (pads->m_endian == pads->disc->d_endian) {
    /* on-disk order same as in-memory rep */
    memcpy(outbuf, ibytes, num_bytes);
  } else {
    /* must reverse the order */
    int n = num_bytes;
    while (--n >= 0) {
      outbuf[n] = *ibytes++;
    }
  }
  errno = 0;
  return num_bytes;
}

ssize_t
rev_fn_name ## _io(P_t *pads, Sfio_t *io, targ_type i)
{
  Pbyte *ibytes = (Pbyte*)(&i);

  errno = 0;
  if (pads->m_endian == pads->disc->d_endian) {
    /* on-disk order same as in-memory rep */
    return sfwrite(io, ibytes, num_bytes);
  }
  {
    /* else must reverse the order */
    Pbyte  sb[num_bytes];
    int n = num_bytes;
    while (--n >= 0) {
      sb[n] = *ibytes++;
    }
    return sfwrite(io, sb, num_bytes);
  }
}
/* END_MACRO */

#define PDCI_EBC2INT_GEN(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max)
targ_type
fn_name(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  Pint32 n = num_digits;
  targ_type res = 0;
  int neg, digit;

  (*ptr_out) = (Pbyte*)bytes + n;
  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return int_min;
  }
  neg = ((bytes[n-1]&0xF0) == 0xD0); /* look at sign nibble; C,F >=0; D < 0 */
  while (--n >= 0) {
    if ((digit = (0xF & *bytes)) > 9) {
      errno = EINVAL;
      return 0;
    }
    if (res < int_min ## _DIV10) {
      goto range_err;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res < int_min + digit) {
      goto range_err;
    }
    res -= digit;
    bytes++;
  }
  errno = 0;
  return neg ? res : - res;
 range_err:
  errno = ERANGE;
  return neg ? int_min : int_max;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  Pint32 n = num_digits;
  targ_type res = 0;
  int neg, digit;

  (*ptr_out) = (Pbyte*)bytes + n;
  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return int_min;
  }
  neg = ((bytes[n-1]&0xF0) == 0xD0); /* look at sign nibble; C,F >=0; D < 0 */
  while (--n >= 0) {
    if ((digit = (0xF & *bytes)) > 9) {
      errno = EINVAL;
      return 0;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    res -= digit;
    bytes++;
  }
  errno = 0;
  return neg ? res : - res;
}

ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i_in, Puint32 num_digits)
{
  Pint32 n = num_digits;
  Pbyte  ebc[30];
  targ_type lim;
  higher_targ_type     i      = i_in;

  if (num_digits > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (n < act_nd_max) {
    lim = PDCI_10toThe[n];
    if (i >= lim || (-i) >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  if (i < 0) {
    i = -i;
    while (--n >= 0) {
      ebc[n] = 0xF0 | (i % 10);
      i /= 10;
    }
    ebc[num_digits-1] &= 0xDF; /* force sign nibble to negative */
  } else {
    while (--n >= 0) {
      ebc[n] = 0xF0 | (i % 10);
      i /= 10;
    }
  }
  errno = 0;
  memcpy(outbuf, ebc, num_digits);
  return num_digits;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type i_in, Puint32 num_digits)
{
  Pint32 n = num_digits;
  Pbyte  ebc[30];
  targ_type lim;
  higher_targ_type i     = i_in;

  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (n < act_nd_max) {
    lim = PDCI_10toThe[n];
    if (i >= lim || (-i) >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  if (i < 0) {
    i = -i;
    while (--n >= 0) {
      ebc[n] = 0xF0 | (i % 10);
      i /= 10;
    }
    ebc[num_digits-1] &= 0xDF; /* force sign nibble to negative */
  } else {
    while (--n >= 0) {
      ebc[n] = 0xF0 | (i % 10);
      i /= 10;
    }
  }
  errno = 0;
  return sfwrite(io, ebc, num_digits);
}
/* END_MACRO */

#define PDCI_EBC2UINT_GEN(fn_name, rev_fn_name, targ_type, int_max, nd_max)
targ_type
fn_name(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  Pint32 n = num_digits;
  targ_type res = 0;
  int digit;

  (*ptr_out) = (Pbyte*)bytes + n;
  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return int_max;
  }
  if ((bytes[n-1]&0xF0) == 0xD0) { /* look at sign nibble; C,F >=0; D < 0 */
    goto range_err;
  }
  while (--n >= 0) {
    if ((digit = (0xF & *bytes)) > 9) {
      errno = EINVAL;
      return 0;
    }
    if (res > int_max ## _DIV10) {
      goto range_err;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res > int_max - digit) {
      goto range_err;
    }
    res += digit;
    bytes++;
  }
  errno = 0;
  return res;
 range_err:
  errno = ERANGE;
  return int_max;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  Pint32 n = num_digits;
  targ_type res = 0;
  int digit;

  (*ptr_out) = (Pbyte*)bytes + n;
  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return int_max;
  }
  while (--n >= 0) {
    if ((digit = (0xF & *bytes)) > 9) {
      errno = EINVAL;
      return 0;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    res += digit;
    bytes++;
  }
  errno = 0;
  return res;
}

ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type u, Puint32 num_digits)
{
  Pint32 n = num_digits;
  Pbyte  ebc[30];
  targ_type lim;

  if (num_digits > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (n < nd_max) {
    lim = PDCI_10toThe[n];
    if (u >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  while (--n >= 0) {
    ebc[n] = 0xF0 | (u % 10);
    u /= 10;
  }
  errno = 0;
  memcpy(outbuf, ebc, num_digits);
  return num_digits;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type u, Puint32 num_digits)
{
  Pint32 n = num_digits;
  Pbyte  ebc[30];
  targ_type lim;

  if (n == 0 || n > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (n < nd_max) {
    lim = PDCI_10toThe[n];
    if (u >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  while (--n >= 0) {
    ebc[n] = 0xF0 | (u % 10);
    u /= 10;
  }
  errno = 0;
  return sfwrite(io, ebc, num_digits);
}
/* END_MACRO */

#define PDCI_BCD2INT_GEN(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max)
targ_type
fn_name(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  int  digit, two_digits;
  int  neg = 0;
  Pint32 num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  targ_type res = 0;

  (*ptr_out) = (Pbyte*)bytes + num_bytes;
  neg = ((num_digits % 2 == 1) && ((bytes[num_bytes-1]&0xF) == 0xD)); /* look at sign nibble; C,F >=0; D < 0 */
  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return int_min;
  }
  while (num_digits >= 2) {
    if (-1 == (two_digits = PDCI_bcd_hilo_digits[*bytes])) {
      (*ptr_out) = (Pbyte*)bytes;
      errno = EINVAL;
      return 0;
    }
    if (res < int_min ## _DIV100) {
      goto range_err;
    }
    res *= 100;
    if (res < int_min + two_digits) {
      goto range_err;
    }
    res -= two_digits;
    bytes++;
    num_digits -= 2;
  }
  if (num_digits) {
    if (-1 == (digit = PDCI_bcd_hi_digit[*bytes])) {
      errno = EINVAL;
      return 0;
    }
    if (res < int_min ## _DIV10) {
      goto range_err;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res < int_min + digit) {
      goto range_err;
    }
    res -= digit;
    bytes++;
  }
  errno = 0;
  return neg ? res : - res;
 range_err:
  errno = ERANGE;
  return neg ? int_min : int_max;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  int  digit, two_digits;
  int  neg = 0;
  Pint32 num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  targ_type res = 0;

  (*ptr_out) = (Pbyte*)bytes + num_bytes;
  neg = ((num_digits % 2 == 1) && ((bytes[num_bytes-1]&0xF) == 0xD)); /* look at sign nibble; C,F >=0; D < 0 */
  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return int_min;
  }
  while (num_digits >= 2) {
    if (-1 == (two_digits = PDCI_bcd_hilo_digits[*bytes])) {
      (*ptr_out) = (Pbyte*)bytes;
      errno = EINVAL;
      return 0;
    }
    res *= 100;
    res -= two_digits;
    bytes++;
    num_digits -= 2;
  }
  if (num_digits) {
    if (-1 == (digit = PDCI_bcd_hi_digit[*bytes])) {
      errno = EINVAL;
      return 0;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    res -= digit;
    bytes++;
  }
  errno = 0;
  return neg ? res : - res;
}

ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i_in, Puint32 num_digits)
{
  Pbyte  bcd[30];
  Pint32 num_bytes;
  int       x, n;
  int       odd_digits = (num_digits % 2 == 1);
  targ_type lim;
  higher_targ_type i     = i_in;

  num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  if (num_bytes > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (num_digits < act_nd_max) {
    lim = PDCI_10toThe[num_digits];
    if (i >= lim || (-i) >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  n = num_bytes - 1;
  if (i < 0) {
    if (!odd_digits) {  /* must use odd number of digits for negative number */
      errno = EDOM;
      return -1;
    }
    i = -i;
    bcd[n--] = ((i%10)<<4) | 0xD; /* force sign nibble to negative */
    i /= 10;
    while (n >= 0) {
      x = i % 100;
      i /= 100;
      bcd[n--] = (x%10) | ((x/10)<<4);
    }
  } else { /* i positive */
    if (odd_digits) {
      bcd[n--] = ((i%10)<<4);
      i /= 10;
    } else { /* for even number of digits, last byte is not used; set it to zero to be safe */
      bcd[n--] = 0;
    }
    while (n >= 0) {
      x = i % 100;
      i /= 100;
      bcd[n--] = (x%10) | ((x/10)<<4);
    }
  }
  errno = 0;
  memcpy(outbuf, bcd, num_bytes);
  return num_bytes;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type i_in, Puint32 num_digits)
{
  Pbyte  bcd[30];
  Pint32 num_bytes;
  int       x, n;
  int       odd_digits = (num_digits % 2 == 1);
  targ_type lim;
  higher_targ_type i     = i_in;

  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (num_digits < act_nd_max) {
    lim = PDCI_10toThe[num_digits];
    if (i >= lim || (-i) >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  n = num_bytes - 1;
  if (i < 0) {
    if (!odd_digits) {  /* must use odd number of digits for negative number */
      errno = EDOM;
      return -1;
    }
    i = -i;
    bcd[n--] = ((i%10)<<4) | 0xD; /* force sign nibble to negative */
    i /= 10;
    while (n >= 0) {
      x = i % 100;
      i /= 100;
      bcd[n--] = (x%10) | ((x/10)<<4);
    }
  } else { /* i positive */
    if (odd_digits) {
      bcd[n--] = ((i%10)<<4);
      i /= 10;
    } else { /* for even number of digits, last byte is not used; set it to zero to be safe */
      bcd[n--] = 0;
    }
    while (n >= 0) {
      x = i % 100;
      i /= 100;
      bcd[n--] = (x%10) | ((x/10)<<4);
    }
  }
  errno = 0;
  return sfwrite(io, bcd, num_bytes);
}
/* END_MACRO */

#define PDCI_BCD2UINT_GEN(fn_name, rev_fn_name, targ_type, int_max, nd_max)
targ_type
fn_name(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  int  digit, two_digits;
  Pint32 num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  targ_type res = 0;

  (*ptr_out) = (Pbyte*)bytes + num_bytes;
  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return int_max;
  }
  while (num_digits >= 2) {
    if (-1 == (two_digits = PDCI_bcd_hilo_digits[*bytes])) {
      (*ptr_out) = (Pbyte*)bytes;
      errno = EINVAL;
      return 0;
    }
    if (res > int_max ## _DIV100) {
      goto range_err;
    }
    res *= 100;
    if (res > int_max - two_digits) {
      goto range_err;
    }
    res += two_digits;
    bytes++;
    num_digits -= 2;
  }
  if (num_digits) {
    if (-1 == (digit = PDCI_bcd_hi_digit[*bytes])) {
      errno = EINVAL;
      return 0;
    }
    if (res > int_max ## _DIV10) {
      goto range_err;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    if (res > int_max - digit) {
      goto range_err;
    }
    res += digit;
    bytes++;
  }
  errno = 0;
  return res;
 range_err:
  errno = ERANGE;
  return int_max;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Puint32 num_digits, Pbyte **ptr_out)
{
  int  digit, two_digits;
  Pint32 num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  targ_type res = 0;

  (*ptr_out) = (Pbyte*)bytes + num_bytes;
  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return int_max;
  }
  while (num_digits >= 2) {
    if (-1 == (two_digits = PDCI_bcd_hilo_digits[*bytes])) {
      (*ptr_out) = (Pbyte*)bytes;
      errno = EINVAL;
      return 0;
    }
    res *= 100;
    res += two_digits;
    bytes++;
    num_digits -= 2;
  }
  if (num_digits) {
    if (-1 == (digit = PDCI_bcd_hi_digit[*bytes])) {
      errno = EINVAL;
      return 0;
    }
    res = (res << 3) + (res << 1); /* res *= 10 */
    res += digit;
    bytes++;
  }
  errno = 0;
  return res;
}

ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type u, Puint32 num_digits)
{
  Pbyte  bcd[30];
  Pint32 num_bytes;
  int       x, n;
  targ_type lim;

  num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  if (num_bytes > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (num_digits < nd_max) {
    lim = PDCI_10toThe[num_digits];
    if (u >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  n = num_bytes - 1;
  if (num_digits % 2 == 1) {
    bcd[n--] = ((u%10)<<4);
    u /= 10;
  } else { /* for even number of digits, last byte is not used; set it to zero to be safe */
    bcd[n--] = 0;
  }
  while (n >= 0) {
    x = u % 100;
    u /= 100;
    bcd[n--] = (x%10) | ((x/10)<<4);
  }
  errno = 0;
  memcpy(outbuf, bcd, num_bytes);
  return num_bytes;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type u, Puint32 num_digits)
{
  Pbyte  bcd[30];
  Pint32 num_bytes;
  int       x, n;
  targ_type lim;

  if (num_digits == 0 || num_digits > nd_max) {
    errno = EDOM;
    return -1;
  }
  if (num_digits < nd_max) {
    lim = PDCI_10toThe[num_digits];
    if (u >= lim) {
      errno = ERANGE;
      return -1;
    }
  }
  num_bytes = ((num_digits+2)/2); /* XXX_CHECK */
  n = num_bytes - 1;
  if (num_digits % 2 == 1) {
    bcd[n--] = ((u%10)<<4);
    u /= 10;
  } else { /* for even number of digits, last byte is not used; set it to zero to be safe */
    bcd[n--] = 0;
  }
  while (n >= 0) {
    x = u % 100;
    u /= 100;
    bcd[n--] = (x%10) | ((x/10)<<4);
  }
  errno = 0;
  return sfwrite(io, bcd, num_bytes);
}
/* END_MACRO */

#define PDCI_SB2INT_GEN(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max)
targ_type
fn_name(P_t *pads, const Pbyte *bytes, Puint32 num_bytes, Pbyte **ptr_out)
{
  Pint32 n = num_bytes;
  targ_type res = 0;
  Pbyte *resbytes = (Pbyte*)(&res);

  (*ptr_out) = (Pbyte*)bytes + num_bytes;
  if (n == 0 || n > nb_max) {
    errno = EDOM;
    return int_min;
  }
  if (pads->m_endian == sb_endian) {
    /* on-disk order same as in-memory rep */
    memcpy(resbytes , bytes, n);
  } else {
    /* must reverse the order */
    while (--n >= 0) {
      resbytes[n] = *bytes++;
    }
  }
  errno = 0;
  if (pads->m_endian == PbigEndian){
    /* High-order byte of value from bytes is aligned with high-order byte of res.
       Shift res so that value and res are aligned at on low-order byte.
       nb_max - n is number of bytes. Multiply x 8 = # bits. */
    res = res >> ((nb_max - num_bytes) * 8);
  }
  return res;
}

ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type i, Puint32 num_bytes)
{
  Pint32 n = num_bytes;
  Pbyte *ibytes = (Pbyte*)(&i);

  if (num_bytes > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (n == 0 || n > 8) {
    errno = EDOM;
    return -1;
  };
  if (i > P_MAX_FOR_NB[n] || i < P_MIN_FOR_NB[n]) {
    errno = ERANGE;
    return -1;
  }
  if (pads->m_endian == PbigEndian){
    /* Shift i so that the high-order byte of the value it contains is aligned with i's high-order byte.
       nb_max - n is number of bytes. Multiply x 8 = # bits. */
    i <<= ((nb_max - num_bytes) * 8);
  }
  if (pads->m_endian == sb_endian) {
    /* on-disk order same as in-memory rep. */
    memcpy(outbuf, ibytes, num_bytes);
  } else {
    /* must reverse the order */
    while (--n >= 0) {
      outbuf[n] = *ibytes++;
    }
  }
  errno = 0;
  return num_bytes;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type i, Puint32 num_bytes)
{
  Pint32 n = num_bytes;
  Pbyte *ibytes = (Pbyte*)(&i);

  if (n == 0 || n > 8) {
    errno = EDOM;
    return -1;
  };
  if (i > P_MAX_FOR_NB[n] || i < P_MIN_FOR_NB[n]) {
    errno = ERANGE;
    return -1;
  }
  if (pads->m_endian == PbigEndian){
    /* Shift i so that the high-order byte of the value it contains is aligned with i's high-order byte.
       nb_max - n is number of bytes. Multiply x 8 = # bits. */
    i <<= ((nb_max - num_bytes) * 8);
  }
  errno = 0;
  if (pads->m_endian == sb_endian) {
    /* on-disk order same as in-memory rep */
    return sfwrite(io, ibytes, n);
  }
  {
    /* else must reverse the order */
    Pbyte  sb[30];
    while (--n >= 0) {
      sb[n] = *ibytes++;
    }
    return sfwrite(io, sb, num_bytes);
  }
}
/* END_MACRO */

#define PDCI_SB2UINT_GEN(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max)
targ_type
fn_name(P_t *pads, const Pbyte *bytes, Puint32 num_bytes, Pbyte **ptr_out)
{
  Pint32 n = num_bytes;
  targ_type res = 0;
  Pbyte *resbytes = (Pbyte*)(&res);

  (*ptr_out) = (Pbyte*)bytes + n;
  if (n == 0 || n > nb_max) {
    errno = EDOM;
    return int_max;
  }
  if (pads->m_endian == sb_endian) {
    /* on-disk order same as in-memory rep */
    memcpy(resbytes, bytes, n);
  } else {
    /* must reverse the order */
    while (--n >= 0) {
      resbytes[n] = *bytes++;
    }
  }
  errno = 0;
  if (pads->m_endian == PbigEndian){
    /* High-order byte of value from bytes is aligned with high-order byte of res.
       Shift res so that value and res are aligned at on low-order byte.
       nb_max - n is number of bytes. Multiply x 8 = # bits. */
    res = res >> ((nb_max - num_bytes) * 8);
  }
  return res;
}

ssize_t
rev_fn_name ## _buf (P_t *pads, Pbyte *outbuf, size_t outbuf_len, int *outbuf_full, targ_type u, Puint32 num_bytes)
{
  Pint32 n = num_bytes;
  Pbyte *ubytes = (Pbyte*)(&u);

  if (num_bytes > outbuf_len) {
    if (outbuf_full) { (*outbuf_full) = 1; }
    errno = EDOM;
    return -1;
  }
  if (n == 0 || n > 8) {
    errno = EDOM;
    return -1;
  };
  if (u > P_UMAX_FOR_NB[n]) {
    errno = ERANGE;
    return -1;
  }
  if (pads->m_endian == PbigEndian){
    /* Shift u so that the high-order byte of the value it contains is aligned with u's high-order byte.
       nb_max - n is number of bytes. Multiply x 8 = # bits. */
    u <<= ((nb_max - num_bytes) * 8);
  }
  if (pads->m_endian == sb_endian) {
    /* on-disk order same as in-memory rep */
    memcpy(outbuf, ubytes, num_bytes);
  } else {
    /* must reverse the order */
    while (--n >= 0) {
      outbuf[n] = *ubytes++;
    }
  }
  errno = 0;
  return num_bytes;
}

ssize_t
rev_fn_name ## _io (P_t *pads, Sfio_t *io, targ_type u, Puint32 num_bytes)
{
  Pint32 n = num_bytes;
  Pbyte *ubytes = (Pbyte*)(&u);

  if (n == 0 || n > 8) {
    errno = EDOM;
    return -1;
  };
  if (u > P_UMAX_FOR_NB[n]) {
    errno = ERANGE;
    return -1;
  }
  if (pads->m_endian == PbigEndian){
    /* Shift u so that the high-order byte of the value it contains is aligned with u's high-order byte.
       nb_max - n is number of bytes. Multiply x 8 = # bits. */
    u <<= ((nb_max - num_bytes) * 8);
  }
  errno = 0;
  if (pads->m_endian == sb_endian) {
    /* on-disk order same as in-memory rep */
    return sfwrite(io, ubytes, n);
  }
  {
    /* else must reverse the order */
    Pbyte sb[30];
    while (--n >= 0) {
      sb[n] = *ubytes++;
    }
    return sfwrite(io, sb, num_bytes);
  }
}
/* END_MACRO */

#define PDCI_A2FLOAT_GEN(fn_name, targ_type, float_min, float_max, Pstring2float_fn)
targ_type
fn_name(const Pbyte *bytes, Pbyte **ptr_out)
{
  Pbyte* ptr;
  _ast_fltmax_t d;

  /* skip spaces so that ptr == bytes means failed conversion */
  while (PDCI_is_a_space(*bytes)) {
    bytes++;
  }
  errno = 0;
  d = strtold((const char *)bytes, (char**)&ptr);
  if (ptr >= bytes) {
    (*ptr_out) = ptr;
  } else {
    (*ptr_out) = (Pbyte*)bytes;
  }
  if (ptr == bytes) {
    errno = EINVAL;
    return 0;
  }
  if ((ptr == bytes + 1) && (bytes[0] == '.')){
    errno = EINVAL;
    return 0;
  }
  if ((errno == ERANGE && d < 0) || (d < float_min)) {
    errno = ERANGE;
    return float_min;
  }
  if ((errno == ERANGE) || (d > float_max)) {
    errno = ERANGE;
    return float_max;
  }
  return (targ_type)d;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Pbyte **ptr_out)
{
  Pbyte* ptr;
  _ast_fltmax_t d;

  /* skip spaces so that ptr == bytes means failed conversion */
  while (PDCI_is_a_space(*bytes)) {
    bytes++;
  }
  errno = 0;
  d = strtold((const char *)bytes, (char**)&ptr);
  if (ptr >= bytes) {
    (*ptr_out) = ptr;
  } else {
    (*ptr_out) = (Pbyte*)bytes;
  }
  if (ptr == bytes) {
    errno = EINVAL;
    return 0;
  }
  if ((ptr == bytes + 1) && (bytes[0] == '.')){
    errno = EINVAL;
    return 0;
  }
  /* ignore all range errors, return potentially bogus wrapped/cast value */
  errno = 0; 
  return (targ_type)d;
}

targ_type
fn_name ## _max_bytes(const Pbyte *bytes, size_t max_bytes, Pbyte **ptr_out)
{

  Pbyte* ptr;
  _ast_fltmax_t d;

  /* skip spaces so that ptr == bytes means failed conversion */
  while (max_bytes > 0 && PDCI_is_a_space(*bytes)) {
    bytes++;
    max_bytes--;
  }
  if (max_bytes == 0) {
    errno = EINVAL;
    return 0;
  }
  errno = 0;
  d = strntold((const char *)bytes, max_bytes, (char**)&ptr);
  if (ptr >= bytes) {
    (*ptr_out) = ptr;
  } else {
    (*ptr_out) = (Pbyte*)bytes;
  }
  if (ptr == bytes) {
    errno = EINVAL;
    return 0;
  }
  if ((ptr == bytes + 1) && (bytes[0] == '.')){
    errno = EINVAL;
    return 0;
  }
  if ((errno == ERANGE && d < 0) || (d < float_min)) {
    errno = ERANGE;
    return float_min;
  }
  if ((errno == ERANGE) || (d > float_max)) {
    errno = ERANGE;
    return float_max;
  }
  return (targ_type)d;
}

targ_type
Pstring2float_fn(const Pstring *s)
{
  Pbyte *bytes, *ptr;
  size_t max_bytes;
  targ_type res;

  if (!s) {
    errno = EINVAL;
    return 0;
  }
  ptr = bytes = (Pbyte*)s->str;
  max_bytes = s->len;
  res = fn_name ## _max_bytes(bytes, max_bytes, &ptr);
  if (errno) {
    return res;
  }
  while (ptr - bytes < max_bytes && PDCI_is_a_space(*ptr)) {
    ptr++;
  }
  if (ptr - bytes != max_bytes) {
    errno = EINVAL;
    return 0;
  }
  return res;
}
/* END_MACRO */

#define PDCI_E2FLOAT_GEN(fn_name, targ_type, float_min, float_max)
targ_type
fn_name(const Pbyte *bytes, Pbyte **ptr_out)
{
  Sfio_t        *tmpstr;
  char          *ascii_bytes, *ascii_ptr;
  _ast_fltmax_t  d;

  if (!(tmpstr = sfstropen())) { 
    P_FATAL(&Pdefault_disc, "sfstropen failed in " PDCI_MacroArg2String(fn_name));
  }

  while (PDCI_is_e_space(*bytes)) {
    bytes++;
  }
  /* XXX_TODO should either add enough chars here to support e notation plus INF/INFINITY and (NAN) or disallow those forms for ASCII above */
  while ((*bytes) == P_EBCDIC_PLUS || (*bytes) == P_EBCDIC_MINUS || (*bytes) == P_EBCDIC_DOT || PDCI_is_e_digit(*bytes)) {
    sfputc(tmpstr, P_mod_ea_tab[(int)(*bytes)]);
    bytes++;
  }
  ascii_bytes = PDCI_sfstr_use(tmpstr);
  errno = 0;
  d = strtold((const char *)ascii_bytes, &ascii_ptr);
  if (ascii_ptr >= ascii_bytes) {
    (*ptr_out) = (Pbyte*)bytes + (ascii_ptr - ascii_bytes);
  } else {
    (*ptr_out) = (Pbyte*)bytes;
  }
  sfstrclose(tmpstr);
  if (ascii_ptr == ascii_bytes) {
    errno = EINVAL;
    return 0;
  }
  if ((ascii_ptr == ascii_bytes + 1) && (ascii_bytes[0] == '.')){
    errno = EINVAL;
    return 0;
  }
  if ((errno == ERANGE && d < 0) || (d < float_min)) {
    errno = ERANGE;
    return float_min;
  }
  if ((errno == ERANGE) || (d > float_max)) {
    errno = ERANGE;
    return float_max;
  }
  return (targ_type)d;
}

targ_type
fn_name ## _norange(const Pbyte *bytes, Pbyte **ptr_out)
{
  Sfio_t        *tmpstr;
  char          *ascii_bytes, *ascii_ptr;
  _ast_fltmax_t  d;

  if (!(tmpstr = sfstropen())) { 
    P_FATAL(&Pdefault_disc, "sfstropen failed in " PDCI_MacroArg2String(fn_name));
  }

  while (PDCI_is_e_space(*bytes)) {
    bytes++;
  }
  /* XXX_TODO should either add enough chars here to support e notation plus INF/INFINITY and (NAN) or disallow those forms for ASCII above */
  while ((*bytes) == P_EBCDIC_PLUS || (*bytes) == P_EBCDIC_MINUS || (*bytes) == P_EBCDIC_DOT || PDCI_is_e_digit(*bytes)) {
    sfputc(tmpstr, P_mod_ea_tab[(int)(*bytes)]);
    bytes++;
  }
  ascii_bytes = PDCI_sfstr_use(tmpstr);
  errno = 0;
  d = strtold((const char *)ascii_bytes, &ascii_ptr);
  if (ascii_ptr >= ascii_bytes) {
    (*ptr_out) = (Pbyte*)bytes + (ascii_ptr - ascii_bytes);
  } else {
    (*ptr_out) = (Pbyte*)bytes;
  }
  sfstrclose(tmpstr);
  if (ascii_ptr == ascii_bytes) {
    errno = EINVAL;
    return 0;
  }
  if ((ascii_ptr == ascii_bytes + 1) && (ascii_bytes[0] == '.')){
    errno = EINVAL;
    return 0;
  }
  /* ignore all range errors, return potentially bogus wrapped/cast value */
  errno = 0; 
  return (targ_type)d;
}

targ_type
fn_name ## _max_bytes(const Pbyte *bytes, size_t max_bytes, Pbyte **ptr_out)
{
  Sfio_t        *tmpstr;
  char          *ascii_bytes, *ascii_ptr;
  _ast_fltmax_t  d;

  if (!(tmpstr = sfstropen())) { 
    P_FATAL(&Pdefault_disc, "sfstropen failed in " PDCI_MacroArg2String(fn_name));
  }

  while (max_bytes > 0 && PDCI_is_e_space(*bytes)) {
    bytes++;
    max_bytes--;
  }
  if (max_bytes == 0) {
    errno = EINVAL;
    return 0;
  }
  /* XXX_TODO should either add enough chars here to support e notation plus INF/INFINITY and (NAN) or disallow those forms for ASCII above */
  while ((*bytes) == P_EBCDIC_PLUS || (*bytes) == P_EBCDIC_MINUS || (*bytes) == P_EBCDIC_DOT || PDCI_is_e_digit(*bytes)) {
    sfputc(tmpstr, P_mod_ea_tab[(int)(*bytes)]);
    bytes++;
  }
  ascii_bytes = PDCI_sfstr_use(tmpstr);
  errno = 0;
  d = strntold((const char *)ascii_bytes, max_bytes, &ascii_ptr);
  if (ascii_ptr >= ascii_bytes) {
    (*ptr_out) = (Pbyte*)bytes + (ascii_ptr - ascii_bytes);
  } else {
    (*ptr_out) = (Pbyte*)bytes;
  }
  sfstrclose(tmpstr);
  if (ascii_ptr == ascii_bytes) {
    errno = EINVAL;
    return 0;
  }
  if ((ascii_ptr == ascii_bytes + 1) && (ascii_bytes[0] == '.')){
    errno = EINVAL;
    return 0;
  }
  if ((errno == ERANGE && d < 0) || (d < float_min)) {
    errno = ERANGE;
    return float_min;
  }
  if ((errno == ERANGE) || (d > float_max)) {
    errno = ERANGE;
    return float_max;
  }
  return (targ_type)d;
}
/* END_MACRO */

/* ********************************* BEGIN_TRAILER ******************************** */

/* We need conversion routines for helpers such as Pstring2int32 */
#if 1 /* P_CONFIG_A_INT_FW > 0 || P_CONFIG_A_INT > 0 */
#  define PDCI_A2INT(fn_name, targ_type, int_min, int_max, Pstring2int_fn) \
            PDCI_A2INT_GEN(fn_name, targ_type, int_min, int_max, Pstring2int_fn)
#  define PDCI_A2UINT(fn_name, targ_type, int_max, Pstring2int_fn) \
            PDCI_A2UINT_GEN(fn_name, targ_type, int_max, Pstring2int_fn)
#  define PDCI_INT2A(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) \
            PDCI_NUM2A_GEN(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#else
#  define PDCI_A2INT(fn_name, targ_type, int_min, int_max, Pstring2int_fn)
#  define PDCI_A2UINT(fn_name, targ_type, int_max, Pstring2int_fn)
#  define PDCI_INT2A(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#endif

#if P_CONFIG_E_INT_FW > 0 || P_CONFIG_E_INT > 0
#  define PDCI_E2INT(fn_name, targ_type, int_min, int_max) \
            PDCI_E2INT_GEN(fn_name, targ_type, int_min, int_max)
#  define PDCI_E2UINT(fn_name, targ_type, int_max) \
            PDCI_E2UINT_GEN(fn_name, targ_type, int_max)
#  define PDCI_INT2E(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) \
            PDCI_NUM2E_GEN(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#else
#  define PDCI_E2INT(fn_name, targ_type, int_min, int_max)
#  define PDCI_E2UINT(fn_name, targ_type, int_max)
#  define PDCI_INT2E(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#endif

#if P_CONFIG_B_INT > 0
#  define PDCI_INT1_2B(rev_fn_name, targ_type) \
            PDCI_INT1_2B_GEN(rev_fn_name, targ_type)
#  define PDCI_INT2B(rev_fn_name, targ_type, num_bytes) \
            PDCI_INT2B_GEN(rev_fn_name, targ_type, num_bytes)
#else
#  define PDCI_INT1_2B(rev_fn_name, targ_type)
#  define PDCI_INT2B(rev_fn_name, targ_type, num_bytes)
#endif

#if P_CONFIG_EBC_INT > 0 || P_CONFIG_EBC_FPOINT > 0
#  define PDCI_EBC2INT(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max) \
            PDCI_EBC2INT_GEN(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max)
#  define PDCI_EBC2UINT(fn_name, rev_fn_name, targ_type, int_max, nd_max) \
            PDCI_EBC2UINT_GEN(fn_name, rev_fn_name, targ_type, int_max, nd_max)
#else
#  define PDCI_EBC2INT(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max)
#  define PDCI_EBC2UINT(fn_name, rev_fn_name, targ_type, int_max, nd_max)
#endif

#if P_CONFIG_BCD_INT > 0 || P_CONFIG_BCD_FPOINT > 0
#  define PDCI_BCD2INT(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max) \
            PDCI_BCD2INT_GEN(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max)
#  define PDCI_BCD2UINT(fn_name, rev_fn_name, targ_type, int_max, nd_max) \
            PDCI_BCD2UINT_GEN(fn_name, rev_fn_name, targ_type, int_max, nd_max)
#else
#  define PDCI_BCD2INT(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max)
#  define PDCI_BCD2UINT(fn_name, rev_fn_name, targ_type, int_max, nd_max)
#endif

#if P_CONFIG_SBL_INT > 0 || P_CONFIG_SBL_FPOINT > 0
#  define PDCI_SBL2INT(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max) \
            PDCI_SB2INT_GEN(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max)
#  define PDCI_SBL2UINT(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max) \
            PDCI_SB2UINT_GEN(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max)
#else
#  define PDCI_SBL2INT(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max)
#  define PDCI_SBL2UINT(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max)
#endif

#if P_CONFIG_SBH_INT > 0 || P_CONFIG_SBH_FPOINT > 0
#  define PDCI_SBH2INT(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max) \
            PDCI_SB2INT_GEN(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max)
#  define PDCI_SBH2UINT(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max) \
            PDCI_SB2UINT_GEN(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max)
#else
#  define PDCI_SBH2INT(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max)
#  define PDCI_SBH2UINT(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max)
#endif

#if P_CONFIG_A_FLOAT > 0
#  define PDCI_A2FLOAT(fn_name, targ_type, float_min, float_max, Pstring2float_fn) \
            PDCI_A2FLOAT_GEN(fn_name, targ_type, float_min, float_max, Pstring2float_fn)
#  define PDCI_FLOAT2A(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) \
            PDCI_NUM2A_GEN(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#else
#  define PDCI_A2FLOAT(fn_name, targ_type, float_min, float_max, Pstring2float_fn)
#  define PDCI_FLOAT2A(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#endif

#if P_CONFIG_E_FLOAT > 0
#  define PDCI_E2FLOAT(fn_name, targ_type, float_min, float_max) \
            PDCI_E2FLOAT_GEN(fn_name, targ_type, float_min, float_max)
#  define PDCI_FLOAT2E(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) \
            PDCI_NUM2E_GEN(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#else
#  define PDCI_E2FLOAT(fn_name, targ_type, float_min, float_max)
#  define PDCI_FLOAT2E(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w)
#endif

/* ********************************** END_MACROS ********************************** */

/* ********************** BEGIN_MACGEN(pads-read-gen.c) *********************** */
/*
 * Generated read functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-internal.h"
#gen_include "pads-macros-gen.h"

/* ********************************** END_HEADER ********************************** */
#gen_include "pads-read-macros-gen.h"

/* ================================================================================ */
/* VARIABLE-WIDTH ASCII INTEGER READ FUNCTIONS */

/*
 * PDCI_A_INT_READ_FN(fn_pref, targ_type, bytes2num_fn)
 */

PDCI_A_INT_READ_FN(Pa_int8,   Pint8,   PDCI_a2int8  )
PDCI_A_INT_READ_FN(Pa_int16,  Pint16,  PDCI_a2int16 )
PDCI_A_INT_READ_FN(Pa_int32,  Pint32,  PDCI_a2int32 )
PDCI_A_INT_READ_FN(Pa_int64,  Pint64,  PDCI_a2int64 )
PDCI_A_INT_READ_FN(Pa_uint8,  Puint8,  PDCI_a2uint8 )
PDCI_A_INT_READ_FN(Pa_uint16, Puint16, PDCI_a2uint16)
PDCI_A_INT_READ_FN(Pa_uint32, Puint32, PDCI_a2uint32)
PDCI_A_INT_READ_FN(Pa_uint64, Puint64, PDCI_a2uint64)

/* ================================================================================ */
/* FIXED-WIDTH ASCII INTEGER READ FUNCTIONS */

/*
 * PDCI_A_INT_FW_READ_FN(fn_name, targ_type, bytes2num_fn)
 */

PDCI_A_INT_FW_READ_FN(Pa_int8_FW_read,   Pint8,   PDCI_a2int8  )
PDCI_A_INT_FW_READ_FN(Pa_int16_FW_read,  Pint16,  PDCI_a2int16 )
PDCI_A_INT_FW_READ_FN(Pa_int32_FW_read,  Pint32,  PDCI_a2int32 )
PDCI_A_INT_FW_READ_FN(Pa_int64_FW_read,  Pint64,  PDCI_a2int64 )
PDCI_A_INT_FW_READ_FN(Pa_uint8_FW_read,  Puint8,  PDCI_a2uint8 )
PDCI_A_INT_FW_READ_FN(Pa_uint16_FW_read, Puint16, PDCI_a2uint16)
PDCI_A_INT_FW_READ_FN(Pa_uint32_FW_read, Puint32, PDCI_a2uint32)
PDCI_A_INT_FW_READ_FN(Pa_uint64_FW_read, Puint64, PDCI_a2uint64)

/* ================================================================================ */
/* BINARY INTEGER READ FUNCTIONS */

/*
 * PDCI_B1_INT_READ_FN(fn_name, targ_type)
 *   read 1 byte
 */

PDCI_B1_INT_READ_FN(Pb_int8_read,   Pint8  )
PDCI_B1_INT_READ_FN(Pb_uint8_read,  Puint8 )

/*
 * PDCI_B_INT_READ_FN(fn_name, targ_type, width, swapmem_op)
 *   read width bytes
 *
 * swapmem ops:
 *    0 -> straight copy
 *    1 -> reverse each byte in each string of 2 bytes
 *    3 -> reverse each byte in each string of 4 bytes
 *    4 -> swap upper/lower 4 bytes in each 8 byte value
 *    7 -> reverse each byte in each string of 8 bytes
 */

PDCI_B_INT_READ_FN(Pb_int16_read,  Pint16,  2, 1)
PDCI_B_INT_READ_FN(Pb_uint16_read, Puint16, 2, 1)
PDCI_B_INT_READ_FN(Pb_int32_read,  Pint32,  4, 3)
PDCI_B_INT_READ_FN(Pb_uint32_read, Puint32, 4, 3)
PDCI_B_INT_READ_FN(Pb_int64_read,  Pint64,  8, 7)
PDCI_B_INT_READ_FN(Pb_uint64_read, Puint64, 8, 7)

/* ================================================================================ */
/* VARIABLE-WIDTH EBCDIC CHAR ENCODING INTEGER READ FUNCTIONS */

/*
 * PDCI_E_INT_READ_FN(fn_pref, targ_type, bytes2num_fn)
 */

PDCI_E_INT_READ_FN(Pe_int8,   Pint8,   PDCI_e2int8  )
PDCI_E_INT_READ_FN(Pe_int16,  Pint16,  PDCI_e2int16 )
PDCI_E_INT_READ_FN(Pe_int32,  Pint32,  PDCI_e2int32 )
PDCI_E_INT_READ_FN(Pe_int64,  Pint64,  PDCI_e2int64 )
PDCI_E_INT_READ_FN(Pe_uint8,  Puint8,  PDCI_e2uint8 )
PDCI_E_INT_READ_FN(Pe_uint16, Puint16, PDCI_e2uint16)
PDCI_E_INT_READ_FN(Pe_uint32, Puint32, PDCI_e2uint32)
PDCI_E_INT_READ_FN(Pe_uint64, Puint64, PDCI_e2uint64)

/* ================================================================================ */
/* FIXED-WIDTH EBCDIC CHAR ENCODING INTEGER READ FUNCTIONS */

/*
 * PDCI_E_INT_FW_READ_FN(fn_name, targ_type, bytes2num_fn)
 */

PDCI_E_INT_FW_READ_FN(Pe_int8_FW_read,   Pint8,   PDCI_e2int8  )
PDCI_E_INT_FW_READ_FN(Pe_int16_FW_read,  Pint16,  PDCI_e2int16 )
PDCI_E_INT_FW_READ_FN(Pe_int32_FW_read,  Pint32,  PDCI_e2int32 )
PDCI_E_INT_FW_READ_FN(Pe_int64_FW_read,  Pint64,  PDCI_e2int64 )
PDCI_E_INT_FW_READ_FN(Pe_uint8_FW_read,  Puint8,  PDCI_e2uint8 )
PDCI_E_INT_FW_READ_FN(Pe_uint16_FW_read, Puint16, PDCI_e2uint16)
PDCI_E_INT_FW_READ_FN(Pe_uint32_FW_read, Puint32, PDCI_e2uint32)
PDCI_E_INT_FW_READ_FN(Pe_uint64_FW_read, Puint64, PDCI_e2uint64)

/* ================================================================================ */
/* EBC, BCD, SBL, SBH NUMERIC ENCODING INTEGER READ FUNCTIONS */

/*
 * PDCI_EBC_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
 */

PDCI_EBC_INT_READ_FN(Pebc_int8_read,   Pint8,   PDCI_ebc2int8,   num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_int16_read,  Pint16,  PDCI_ebc2int16,  num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_int32_read,  Pint32,  PDCI_ebc2int32,  num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_int64_read,  Pint64,  PDCI_ebc2int64,  num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_uint8_read,  Puint8,  PDCI_ebc2uint8,  num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_uint16_read, Puint16, PDCI_ebc2uint16, num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_uint32_read, Puint32, PDCI_ebc2uint32, num_digits_or_bytes)
PDCI_EBC_INT_READ_FN(Pebc_uint64_read, Puint64, PDCI_ebc2uint64, num_digits_or_bytes)

/*
 * PDCI_BCD_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
 */

PDCI_BCD_INT_READ_FN(Pbcd_int8_read,   Pint8,   PDCI_bcd2int8,   ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_int16_read,  Pint16,  PDCI_bcd2int16,  ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_int32_read,  Pint32,  PDCI_bcd2int32,  ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_int64_read,  Pint64,  PDCI_bcd2int64,  ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_uint8_read,  Puint8,  PDCI_bcd2uint8,  ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_uint16_read, Puint16, PDCI_bcd2uint16, ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_uint32_read, Puint32, PDCI_bcd2uint32, ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */
PDCI_BCD_INT_READ_FN(Pbcd_uint64_read, Puint64, PDCI_bcd2uint64, ((num_digits_or_bytes+2)/2)) /* XXX_CHECK */

/*
 * PDCI_SBL_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
 */

PDCI_SBL_INT_READ_FN(Psbl_int8_read,   Pint8,   PDCI_sbl2int8,   num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_int16_read,  Pint16,  PDCI_sbl2int16,  num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_int32_read,  Pint32,  PDCI_sbl2int32,  num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_int64_read,  Pint64,  PDCI_sbl2int64,  num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_uint8_read,  Puint8,  PDCI_sbl2uint8,  num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_uint16_read, Puint16, PDCI_sbl2uint16, num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_uint32_read, Puint32, PDCI_sbl2uint32, num_digits_or_bytes)
PDCI_SBL_INT_READ_FN(Psbl_uint64_read, Puint64, PDCI_sbl2uint64, num_digits_or_bytes)

/*
 * PDCI_SBH_INT_READ_FN(fn_name, targ_type, bytes2num_fn, width)
 */

PDCI_SBH_INT_READ_FN(Psbh_int8_read,   Pint8,   PDCI_sbh2int8,   num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_int16_read,  Pint16,  PDCI_sbh2int16,  num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_int32_read,  Pint32,  PDCI_sbh2int32,  num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_int64_read,  Pint64,  PDCI_sbh2int64,  num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_uint8_read,  Puint8,  PDCI_sbh2uint8,  num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_uint16_read, Puint16, PDCI_sbh2uint16, num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_uint32_read, Puint32, PDCI_sbh2uint32, num_digits_or_bytes)
PDCI_SBH_INT_READ_FN(Psbh_uint64_read, Puint64, PDCI_sbh2uint64, num_digits_or_bytes)

/* ================================================================================ */
/* EBC, BCD, SBL, SBH NUMERIC ENCODING FIXED POINT READ FUNCTIONS */

/*
 * PDCI_EBC_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
 */

PDCI_EBC_FPOINT_READ_FN(Pebc_fpoint8_read,   Pfpoint8,   Pebc_int8_read,   num_digits_or_bytes,  2)
PDCI_EBC_FPOINT_READ_FN(Pebc_fpoint16_read,  Pfpoint16,  Pebc_int16_read,  num_digits_or_bytes,  4)
PDCI_EBC_FPOINT_READ_FN(Pebc_fpoint32_read,  Pfpoint32,  Pebc_int32_read,  num_digits_or_bytes,  9)
PDCI_EBC_FPOINT_READ_FN(Pebc_fpoint64_read,  Pfpoint64,  Pebc_int64_read,  num_digits_or_bytes, 19)
PDCI_EBC_FPOINT_READ_FN(Pebc_ufpoint8_read,  Pufpoint8,  Pebc_uint8_read,  num_digits_or_bytes,  2)
PDCI_EBC_FPOINT_READ_FN(Pebc_ufpoint16_read, Pufpoint16, Pebc_uint16_read, num_digits_or_bytes,  4)
PDCI_EBC_FPOINT_READ_FN(Pebc_ufpoint32_read, Pufpoint32, Pebc_uint32_read, num_digits_or_bytes,  9)
PDCI_EBC_FPOINT_READ_FN(Pebc_ufpoint64_read, Pufpoint64, Pebc_uint64_read, num_digits_or_bytes, 19)

/*
 * PDCI_BCD_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
 */

PDCI_BCD_FPOINT_READ_FN(Pbcd_fpoint8_read,   Pfpoint8,   Pbcd_int8_read,   ((num_digits_or_bytes+2)/2),  2) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_fpoint16_read,  Pfpoint16,  Pbcd_int16_read,  ((num_digits_or_bytes+2)/2),  4) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_fpoint32_read,  Pfpoint32,  Pbcd_int32_read,  ((num_digits_or_bytes+2)/2),  9) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_fpoint64_read,  Pfpoint64,  Pbcd_int64_read,  ((num_digits_or_bytes+2)/2), 19) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_ufpoint8_read,  Pufpoint8,  Pbcd_uint8_read,  ((num_digits_or_bytes+2)/2),  2) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_ufpoint16_read, Pufpoint16, Pbcd_uint16_read, ((num_digits_or_bytes+2)/2),  4) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_ufpoint32_read, Pufpoint32, Pbcd_uint32_read, ((num_digits_or_bytes+2)/2),  9) /* XXX_CHECK */
PDCI_BCD_FPOINT_READ_FN(Pbcd_ufpoint64_read, Pufpoint64, Pbcd_uint64_read, ((num_digits_or_bytes+2)/2), 19) /* XXX_CHECK */

/*
 * PDCI_SBL_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
 */

PDCI_SBL_FPOINT_READ_FN(Psbl_fpoint8_read,   Pfpoint8,   Psbl_int8_read,   num_digits_or_bytes,  2)
PDCI_SBL_FPOINT_READ_FN(Psbl_fpoint16_read,  Pfpoint16,  Psbl_int16_read,  num_digits_or_bytes,  4)
PDCI_SBL_FPOINT_READ_FN(Psbl_fpoint32_read,  Pfpoint32,  Psbl_int32_read,  num_digits_or_bytes,  9)
PDCI_SBL_FPOINT_READ_FN(Psbl_fpoint64_read,  Pfpoint64,  Psbl_int64_read,  num_digits_or_bytes, 19)
PDCI_SBL_FPOINT_READ_FN(Psbl_ufpoint8_read,  Pufpoint8,  Psbl_uint8_read,  num_digits_or_bytes,  2)
PDCI_SBL_FPOINT_READ_FN(Psbl_ufpoint16_read, Pufpoint16, Psbl_uint16_read, num_digits_or_bytes,  4)
PDCI_SBL_FPOINT_READ_FN(Psbl_ufpoint32_read, Pufpoint32, Psbl_uint32_read, num_digits_or_bytes,  9)
PDCI_SBL_FPOINT_READ_FN(Psbl_ufpoint64_read, Pufpoint64, Psbl_uint64_read, num_digits_or_bytes, 19)

/*
 * PDCI_SBH_FPOINT_READ_FN(fn_name, targ_type, internal_numerator_read_fn, width, dexp_max)
 */

PDCI_SBH_FPOINT_READ_FN(Psbh_fpoint8_read,   Pfpoint8,   Psbh_int8_read,   num_digits_or_bytes,  2)
PDCI_SBH_FPOINT_READ_FN(Psbh_fpoint16_read,  Pfpoint16,  Psbh_int16_read,  num_digits_or_bytes,  4)
PDCI_SBH_FPOINT_READ_FN(Psbh_fpoint32_read,  Pfpoint32,  Psbh_int32_read,  num_digits_or_bytes,  9)
PDCI_SBH_FPOINT_READ_FN(Psbh_fpoint64_read,  Pfpoint64,  Psbh_int64_read,  num_digits_or_bytes, 19)
PDCI_SBH_FPOINT_READ_FN(Psbh_ufpoint8_read,  Pufpoint8,  Psbh_uint8_read,  num_digits_or_bytes,  2)
PDCI_SBH_FPOINT_READ_FN(Psbh_ufpoint16_read, Pufpoint16, Psbh_uint16_read, num_digits_or_bytes,  4)
PDCI_SBH_FPOINT_READ_FN(Psbh_ufpoint32_read, Pufpoint32, Psbh_uint32_read, num_digits_or_bytes,  9)
PDCI_SBH_FPOINT_READ_FN(Psbh_ufpoint64_read, Pufpoint64, Psbh_uint64_read, num_digits_or_bytes, 19)

/* ================================================================================ */
/* VARIABLE-WIDTH ASCII FLOAT READ FUNCTIONS */

/*
 * PDCI_A_FLOAT_READ_FN(fn_pref, targ_type, bytes2num_fn)
 */

PDCI_A_FLOAT_READ_FN(Pa_float32,  Pfloat32,  PDCI_a2float32 )
PDCI_A_FLOAT_READ_FN(Pa_float64,  Pfloat64,  PDCI_a2float64 )

/* ================================================================================ */
/* VARIABLE-WIDTH EBCDIC FLOAT READ FUNCTIONS */

/*
 * PDCI_E_FLOAT_READ_FN(fn_pref, targ_type, bytes2num_fn)
 */

PDCI_E_FLOAT_READ_FN(Pe_float32,  Pfloat32,  PDCI_e2float32 )
PDCI_E_FLOAT_READ_FN(Pe_float64,  Pfloat64,  PDCI_e2float64 )

/* ********************************* BEGIN_TRAILER ******************************** */

/*
 * XXX dummy going away eventually
 */
Perror_t
Pdummy_read(P_t *pads, const Pbase_m *m, Pint32 dummy_val, Pbase_pd *pd, Pint32 *res_out)
{
  PDCI_DISC_3P_CHECKS("Pdummy_read", m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  (*res_out) = dummy_val;
  return P_OK;
}

/* ********************************** END_MACGEN ********************************** */
/* ********************* BEGIN_MACGEN(pads-write-gen.c) *********************** */
/*
 * Generated write functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-internal.h"
#gen_include "pads-macros-gen.h"

/* ********************************** END_HEADER ********************************** */
#gen_include "pads-write-macros-gen.h"

/* ================================================================================ */
/* WRITE FUNCTIONS */

/*
 * PDCI_A_INT_FW_WRITE_FN(fn_pref, targ_type, wfmt, inv_type, inv_val, sfpr_macro_w)
 */

PDCI_A_INT_FW_WRITE_FN(Pa_int8_FW,   Pint8,   "%0.*I1d", "Pint8_FW",   P_INT8_DEF_INV_VAL,   PDCI_WFMT_INT1_WRITE)
PDCI_A_INT_FW_WRITE_FN(Pa_int16_FW,  Pint16,  "%0.*I2d", "Pint16_FW",  P_INT16_DEF_INV_VAL,  PDCI_WFMT_INT_WRITE)
PDCI_A_INT_FW_WRITE_FN(Pa_int32_FW,  Pint32,  "%0.*I4d", "Pint32_FW",  P_INT32_DEF_INV_VAL,  PDCI_WFMT_INT_WRITE)
PDCI_A_INT_FW_WRITE_FN(Pa_int64_FW,  Pint64,  "%0.*I8d", "Pint64_FW",  P_INT64_DEF_INV_VAL,  PDCI_WFMT_INT_WRITE)

PDCI_A_INT_FW_WRITE_FN(Pa_uint8_FW,  Puint8,  "%0.*I1u", "Puint8_FW",  P_UINT8_DEF_INV_VAL,  PDCI_WFMT_UINT_WRITE)
PDCI_A_INT_FW_WRITE_FN(Pa_uint16_FW, Puint16, "%0.*I2u", "Puint16_FW", P_UINT16_DEF_INV_VAL, PDCI_WFMT_UINT_WRITE)
PDCI_A_INT_FW_WRITE_FN(Pa_uint32_FW, Puint32, "%0.*I4u", "Puint32_FW", P_UINT32_DEF_INV_VAL, PDCI_WFMT_UINT_WRITE)
PDCI_A_INT_FW_WRITE_FN(Pa_uint64_FW, Puint64, "%0.*I8u", "Puint64_FW", P_UINT64_DEF_INV_VAL, PDCI_WFMT_UINT_WRITE)

/*
 * PDCI_A_INT_WRITE_FN(fn_pref, targ_type, fmt, inv_type, inv_val)
 */

PDCI_A_INT_WRITE_FN(Pa_int8,   Pint8,   "%I1d",   "Pint8",   P_INT8_DEF_INV_VAL,   PDCI_FMT_INT1_WRITE)
PDCI_A_INT_WRITE_FN(Pa_int16,  Pint16,  "%I2d",   "Pint16",  P_INT16_DEF_INV_VAL,  PDCI_FMT_INT_WRITE)
PDCI_A_INT_WRITE_FN(Pa_int32,  Pint32,  "%I4d",   "Pint32",  P_INT32_DEF_INV_VAL,  PDCI_FMT_INT_WRITE)
PDCI_A_INT_WRITE_FN(Pa_int64,  Pint64,  "%I8d",   "Pint64",  P_INT64_DEF_INV_VAL,  PDCI_FMT_INT_WRITE)

PDCI_A_INT_WRITE_FN(Pa_uint8,  Puint8,  "%I1u",   "Puint8",  P_UINT8_DEF_INV_VAL,  PDCI_FMT_UINT_WRITE)
PDCI_A_INT_WRITE_FN(Pa_uint16, Puint16, "%I2u",   "Puint16", P_UINT16_DEF_INV_VAL, PDCI_FMT_UINT_WRITE)
PDCI_A_INT_WRITE_FN(Pa_uint32, Puint32, "%I4u",   "Puint32", P_UINT32_DEF_INV_VAL, PDCI_FMT_UINT_WRITE)
PDCI_A_INT_WRITE_FN(Pa_uint64, Puint64, "%I8u",   "Puint64", P_UINT64_DEF_INV_VAL, PDCI_FMT_UINT_WRITE)

/*
 * PDCI_E_INT_FW_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_E_INT_FW_WRITE_FN(Pe_int8_FW,   Pint8,   PDCI_int8_2e,   "Pint8_FW",   P_INT8_DEF_INV_VAL,   Pa_int8_FW  )
PDCI_E_INT_FW_WRITE_FN(Pe_int16_FW,  Pint16,  PDCI_int16_2e,  "Pint16_FW",  P_INT16_DEF_INV_VAL,  Pa_int16_FW )
PDCI_E_INT_FW_WRITE_FN(Pe_int32_FW,  Pint32,  PDCI_int32_2e,  "Pint32_FW",  P_INT32_DEF_INV_VAL,  Pa_int32_FW )
PDCI_E_INT_FW_WRITE_FN(Pe_int64_FW,  Pint64,  PDCI_int64_2e,  "Pint64_FW",  P_INT64_DEF_INV_VAL,  Pa_int64_FW )
PDCI_E_INT_FW_WRITE_FN(Pe_uint8_FW,  Puint8,  PDCI_uint8_2e,  "Puint8_FW",  P_UINT8_DEF_INV_VAL,  Pa_uint8_FW )
PDCI_E_INT_FW_WRITE_FN(Pe_uint16_FW, Puint16, PDCI_uint16_2e, "Puint16_FW", P_UINT16_DEF_INV_VAL, Pa_uint16_FW)
PDCI_E_INT_FW_WRITE_FN(Pe_uint32_FW, Puint32, PDCI_uint32_2e, "Puint32_FW", P_UINT32_DEF_INV_VAL, Pa_uint32_FW)
PDCI_E_INT_FW_WRITE_FN(Pe_uint64_FW, Puint64, PDCI_uint64_2e, "Puint64_FW", P_UINT64_DEF_INV_VAL, Pa_uint64_FW)

/*
 * PDCI_E_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_E_INT_WRITE_FN(Pe_int8,   Pint8,   PDCI_int8_2e,   "Pint8",   P_INT8_DEF_INV_VAL,   Pa_int8  )
PDCI_E_INT_WRITE_FN(Pe_int16,  Pint16,  PDCI_int16_2e,  "Pint16",  P_INT16_DEF_INV_VAL,  Pa_int16 )
PDCI_E_INT_WRITE_FN(Pe_int32,  Pint32,  PDCI_int32_2e,  "Pint32",  P_INT32_DEF_INV_VAL,  Pa_int32 )
PDCI_E_INT_WRITE_FN(Pe_int64,  Pint64,  PDCI_int64_2e,  "Pint64",  P_INT64_DEF_INV_VAL,  Pa_int64 )
PDCI_E_INT_WRITE_FN(Pe_uint8,  Puint8,  PDCI_uint8_2e,  "Puint8",  P_UINT8_DEF_INV_VAL,  Pa_uint8 )
PDCI_E_INT_WRITE_FN(Pe_uint16, Puint16, PDCI_uint16_2e, "Puint16", P_UINT16_DEF_INV_VAL, Pa_uint16)
PDCI_E_INT_WRITE_FN(Pe_uint32, Puint32, PDCI_uint32_2e, "Puint32", P_UINT32_DEF_INV_VAL, Pa_uint32)
PDCI_E_INT_WRITE_FN(Pe_uint64, Puint64, PDCI_uint64_2e, "Puint64", P_UINT64_DEF_INV_VAL, Pa_uint64)

/*
 * PDCI_EBC_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_EBC_INT_WRITE_FN(Pebc_int8,   Pint8,   PDCI_int8_2ebc,   "Pint8",   P_INT8_DEF_INV_VAL,   Pa_int8  )  
PDCI_EBC_INT_WRITE_FN(Pebc_int16,  Pint16,  PDCI_int16_2ebc,  "Pint16",  P_INT16_DEF_INV_VAL,  Pa_int16 ) 
PDCI_EBC_INT_WRITE_FN(Pebc_int32,  Pint32,  PDCI_int32_2ebc,  "Pint32",  P_INT32_DEF_INV_VAL,  Pa_int32 ) 
PDCI_EBC_INT_WRITE_FN(Pebc_int64,  Pint64,  PDCI_int64_2ebc,  "Pint64",  P_INT64_DEF_INV_VAL,  Pa_int64 ) 
PDCI_EBC_INT_WRITE_FN(Pebc_uint8,  Puint8,  PDCI_uint8_2ebc,  "Puint8",  P_UINT8_DEF_INV_VAL,  Pa_uint8 ) 
PDCI_EBC_INT_WRITE_FN(Pebc_uint16, Puint16, PDCI_uint16_2ebc, "Puint16", P_UINT16_DEF_INV_VAL, Pa_uint16)
PDCI_EBC_INT_WRITE_FN(Pebc_uint32, Puint32, PDCI_uint32_2ebc, "Puint32", P_UINT32_DEF_INV_VAL, Pa_uint32)
PDCI_EBC_INT_WRITE_FN(Pebc_uint64, Puint64, PDCI_uint64_2ebc, "Puint64", P_UINT64_DEF_INV_VAL, Pa_uint64)

/*
 * PDCI_B_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_B_INT_WRITE_FN(Pb_int8,   Pint8,   PDCI_int8_2b,   "Pint8",   P_INT8_DEF_INV_VAL,   Pa_int8  )  
PDCI_B_INT_WRITE_FN(Pb_int16,  Pint16,  PDCI_int16_2b,  "Pint16",  P_INT16_DEF_INV_VAL,  Pa_int16 ) 
PDCI_B_INT_WRITE_FN(Pb_int32,  Pint32,  PDCI_int32_2b,  "Pint32",  P_INT32_DEF_INV_VAL,  Pa_int32 ) 
PDCI_B_INT_WRITE_FN(Pb_int64,  Pint64,  PDCI_int64_2b,  "Pint64",  P_INT64_DEF_INV_VAL,  Pa_int64 ) 
PDCI_B_INT_WRITE_FN(Pb_uint8,  Puint8,  PDCI_uint8_2b,  "Puint8",  P_UINT8_DEF_INV_VAL,  Pa_uint8 ) 
PDCI_B_INT_WRITE_FN(Pb_uint16, Puint16, PDCI_uint16_2b, "Puint16", P_UINT16_DEF_INV_VAL, Pa_uint16)
PDCI_B_INT_WRITE_FN(Pb_uint32, Puint32, PDCI_uint32_2b, "Puint32", P_UINT32_DEF_INV_VAL, Pa_uint32)
PDCI_B_INT_WRITE_FN(Pb_uint64, Puint64, PDCI_uint64_2b, "Puint64", P_UINT64_DEF_INV_VAL, Pa_uint64)

/*
 * PDCI_BCD_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_BCD_INT_WRITE_FN(Pbcd_int8,   Pint8,   PDCI_int8_2bcd,   "Pint8",   P_INT8_DEF_INV_VAL,   Pa_int8  )  
PDCI_BCD_INT_WRITE_FN(Pbcd_int16,  Pint16,  PDCI_int16_2bcd,  "Pint16",  P_INT16_DEF_INV_VAL,  Pa_int16 ) 
PDCI_BCD_INT_WRITE_FN(Pbcd_int32,  Pint32,  PDCI_int32_2bcd,  "Pint32",  P_INT32_DEF_INV_VAL,  Pa_int32 ) 
PDCI_BCD_INT_WRITE_FN(Pbcd_int64,  Pint64,  PDCI_int64_2bcd,  "Pint64",  P_INT64_DEF_INV_VAL,  Pa_int64 ) 
PDCI_BCD_INT_WRITE_FN(Pbcd_uint8,  Puint8,  PDCI_uint8_2bcd,  "Puint8",  P_UINT8_DEF_INV_VAL,  Pa_uint8 ) 
PDCI_BCD_INT_WRITE_FN(Pbcd_uint16, Puint16, PDCI_uint16_2bcd, "Puint16", P_UINT16_DEF_INV_VAL, Pa_uint16)
PDCI_BCD_INT_WRITE_FN(Pbcd_uint32, Puint32, PDCI_uint32_2bcd, "Puint32", P_UINT32_DEF_INV_VAL, Pa_uint32)
PDCI_BCD_INT_WRITE_FN(Pbcd_uint64, Puint64, PDCI_uint64_2bcd, "Puint64", P_UINT64_DEF_INV_VAL, Pa_uint64)

/*
 * PDCI_SBL_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_SBL_INT_WRITE_FN(Psbl_int8,   Pint8,   PDCI_int8_2sbl,   "Pint8",   P_INT8_DEF_INV_VAL,   Pa_int8  )  
PDCI_SBL_INT_WRITE_FN(Psbl_int16,  Pint16,  PDCI_int16_2sbl,  "Pint16",  P_INT16_DEF_INV_VAL,  Pa_int16 ) 
PDCI_SBL_INT_WRITE_FN(Psbl_int32,  Pint32,  PDCI_int32_2sbl,  "Pint32",  P_INT32_DEF_INV_VAL,  Pa_int32 ) 
PDCI_SBL_INT_WRITE_FN(Psbl_int64,  Pint64,  PDCI_int64_2sbl,  "Pint64",  P_INT64_DEF_INV_VAL,  Pa_int64 ) 
PDCI_SBL_INT_WRITE_FN(Psbl_uint8,  Puint8,  PDCI_uint8_2sbl,  "Puint8",  P_UINT8_DEF_INV_VAL,  Pa_uint8 ) 
PDCI_SBL_INT_WRITE_FN(Psbl_uint16, Puint16, PDCI_uint16_2sbl, "Puint16", P_UINT16_DEF_INV_VAL, Pa_uint16)
PDCI_SBL_INT_WRITE_FN(Psbl_uint32, Puint32, PDCI_uint32_2sbl, "Puint32", P_UINT32_DEF_INV_VAL, Pa_uint32)
PDCI_SBL_INT_WRITE_FN(Psbl_uint64, Puint64, PDCI_uint64_2sbl, "Puint64", P_UINT64_DEF_INV_VAL, Pa_uint64)

/*
 * PDCI_SBH_INT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_SBH_INT_WRITE_FN(Psbh_int8,   Pint8,   PDCI_int8_2sbh,   "Pint8",   P_INT8_DEF_INV_VAL,   Pa_int8  )  
PDCI_SBH_INT_WRITE_FN(Psbh_int16,  Pint16,  PDCI_int16_2sbh,  "Pint16",  P_INT16_DEF_INV_VAL,  Pa_int16 ) 
PDCI_SBH_INT_WRITE_FN(Psbh_int32,  Pint32,  PDCI_int32_2sbh,  "Pint32",  P_INT32_DEF_INV_VAL,  Pa_int32 ) 
PDCI_SBH_INT_WRITE_FN(Psbh_int64,  Pint64,  PDCI_int64_2sbh,  "Pint64",  P_INT64_DEF_INV_VAL,  Pa_int64 ) 
PDCI_SBH_INT_WRITE_FN(Psbh_uint8,  Puint8,  PDCI_uint8_2sbh,  "Puint8",  P_UINT8_DEF_INV_VAL,  Pa_uint8 ) 
PDCI_SBH_INT_WRITE_FN(Psbh_uint16, Puint16, PDCI_uint16_2sbh, "Puint16", P_UINT16_DEF_INV_VAL, Pa_uint16)
PDCI_SBH_INT_WRITE_FN(Psbh_uint32, Puint32, PDCI_uint32_2sbh, "Puint32", P_UINT32_DEF_INV_VAL, Pa_uint32)
PDCI_SBH_INT_WRITE_FN(Psbh_uint64, Puint64, PDCI_uint64_2sbh, "Puint64", P_UINT64_DEF_INV_VAL, Pa_uint64)

/*
 * PDCI_EBC_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
 */

PDCI_EBC_FPOINT_WRITE_FN(Pebc_fpoint8,   Pfpoint8,   PDCI_int8_2ebc,   "Pfpoint8",   P_INT8_DEF_INV_VAL)  
PDCI_EBC_FPOINT_WRITE_FN(Pebc_fpoint16,  Pfpoint16,  PDCI_int16_2ebc,  "Pfpoint16",  P_INT16_DEF_INV_VAL) 
PDCI_EBC_FPOINT_WRITE_FN(Pebc_fpoint32,  Pfpoint32,  PDCI_int32_2ebc,  "Pfpoint32",  P_INT32_DEF_INV_VAL) 
PDCI_EBC_FPOINT_WRITE_FN(Pebc_fpoint64,  Pfpoint64,  PDCI_int64_2ebc,  "Pfpoint64",  P_INT64_DEF_INV_VAL) 
PDCI_EBC_FPOINT_WRITE_FN(Pebc_ufpoint8,  Pufpoint8,  PDCI_uint8_2ebc,  "Pufpoint8",  P_UINT8_DEF_INV_VAL) 
PDCI_EBC_FPOINT_WRITE_FN(Pebc_ufpoint16, Pufpoint16, PDCI_uint16_2ebc, "Pufpoint16", P_UINT16_DEF_INV_VAL)
PDCI_EBC_FPOINT_WRITE_FN(Pebc_ufpoint32, Pufpoint32, PDCI_uint32_2ebc, "Pufpoint32", P_UINT32_DEF_INV_VAL)
PDCI_EBC_FPOINT_WRITE_FN(Pebc_ufpoint64, Pufpoint64, PDCI_uint64_2ebc, "Pufpoint64", P_UINT64_DEF_INV_VAL)

/*
 * PDCI_BCD_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
 */

PDCI_BCD_FPOINT_WRITE_FN(Pbcd_fpoint8,   Pfpoint8,   PDCI_int8_2bcd,   "Pfpoint8",   P_INT8_DEF_INV_VAL)  
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_fpoint16,  Pfpoint16,  PDCI_int16_2bcd,  "Pfpoint16",  P_INT16_DEF_INV_VAL) 
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_fpoint32,  Pfpoint32,  PDCI_int32_2bcd,  "Pfpoint32",  P_INT32_DEF_INV_VAL) 
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_fpoint64,  Pfpoint64,  PDCI_int64_2bcd,  "Pfpoint64",  P_INT64_DEF_INV_VAL) 
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_ufpoint8,  Pufpoint8,  PDCI_uint8_2bcd,  "Pufpoint8",  P_UINT8_DEF_INV_VAL) 
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_ufpoint16, Pufpoint16, PDCI_uint16_2bcd, "Pufpoint16", P_UINT16_DEF_INV_VAL)
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_ufpoint32, Pufpoint32, PDCI_uint32_2bcd, "Pufpoint32", P_UINT32_DEF_INV_VAL)
PDCI_BCD_FPOINT_WRITE_FN(Pbcd_ufpoint64, Pufpoint64, PDCI_uint64_2bcd, "Pufpoint64", P_UINT64_DEF_INV_VAL)

/*
 * PDCI_SBL_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
 */

PDCI_SBL_FPOINT_WRITE_FN(Psbl_fpoint8,   Pfpoint8,   PDCI_int8_2sbl,   "Pfpoint8",   P_INT8_DEF_INV_VAL)  
PDCI_SBL_FPOINT_WRITE_FN(Psbl_fpoint16,  Pfpoint16,  PDCI_int16_2sbl,  "Pfpoint16",  P_INT16_DEF_INV_VAL) 
PDCI_SBL_FPOINT_WRITE_FN(Psbl_fpoint32,  Pfpoint32,  PDCI_int32_2sbl,  "Pfpoint32",  P_INT32_DEF_INV_VAL) 
PDCI_SBL_FPOINT_WRITE_FN(Psbl_fpoint64,  Pfpoint64,  PDCI_int64_2sbl,  "Pfpoint64",  P_INT64_DEF_INV_VAL) 
PDCI_SBL_FPOINT_WRITE_FN(Psbl_ufpoint8,  Pufpoint8,  PDCI_uint8_2sbl,  "Pufpoint8",  P_UINT8_DEF_INV_VAL) 
PDCI_SBL_FPOINT_WRITE_FN(Psbl_ufpoint16, Pufpoint16, PDCI_uint16_2sbl, "Pufpoint16", P_UINT16_DEF_INV_VAL)
PDCI_SBL_FPOINT_WRITE_FN(Psbl_ufpoint32, Pufpoint32, PDCI_uint32_2sbl, "Pufpoint32", P_UINT32_DEF_INV_VAL)
PDCI_SBL_FPOINT_WRITE_FN(Psbl_ufpoint64, Pufpoint64, PDCI_uint64_2sbl, "Pufpoint64", P_UINT64_DEF_INV_VAL)

/*
 * PDCI_SBH_FPOINT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val)
 */

PDCI_SBH_FPOINT_WRITE_FN(Psbh_fpoint8,   Pfpoint8,   PDCI_int8_2sbh,   "Pfpoint8",   P_INT8_DEF_INV_VAL)  
PDCI_SBH_FPOINT_WRITE_FN(Psbh_fpoint16,  Pfpoint16,  PDCI_int16_2sbh,  "Pfpoint16",  P_INT16_DEF_INV_VAL) 
PDCI_SBH_FPOINT_WRITE_FN(Psbh_fpoint32,  Pfpoint32,  PDCI_int32_2sbh,  "Pfpoint32",  P_INT32_DEF_INV_VAL) 
PDCI_SBH_FPOINT_WRITE_FN(Psbh_fpoint64,  Pfpoint64,  PDCI_int64_2sbh,  "Pfpoint64",  P_INT64_DEF_INV_VAL) 
PDCI_SBH_FPOINT_WRITE_FN(Psbh_ufpoint8,  Pufpoint8,  PDCI_uint8_2sbh,  "Pufpoint8",  P_UINT8_DEF_INV_VAL) 
PDCI_SBH_FPOINT_WRITE_FN(Psbh_ufpoint16, Pufpoint16, PDCI_uint16_2sbh, "Pufpoint16", P_UINT16_DEF_INV_VAL)
PDCI_SBH_FPOINT_WRITE_FN(Psbh_ufpoint32, Pufpoint32, PDCI_uint32_2sbh, "Pufpoint32", P_UINT32_DEF_INV_VAL)
PDCI_SBH_FPOINT_WRITE_FN(Psbh_ufpoint64, Pufpoint64, PDCI_uint64_2sbh, "Pufpoint64", P_UINT64_DEF_INV_VAL)

/*
 * PDCI_A_FLOAT_WRITE_FN(fn_pref, targ_type, fmt, inv_type, inv_val)
 */

PDCI_A_FLOAT_WRITE_FN(Pa_float32,  Pfloat32,  "%I4f",   "Pfloat32",  P_FLOAT32_DEF_INV_VAL,  PDCI_FMT_FLOAT_WRITE)
PDCI_A_FLOAT_WRITE_FN(Pa_float64,  Pfloat64,  "%I8f",   "Pfloat64",  P_FLOAT64_DEF_INV_VAL,  PDCI_FMT_FLOAT_WRITE)

/*
 * PDCI_E_FLOAT_WRITE_FN(fn_pref, targ_type, num2pre, inv_type, inv_val, a_fn_pref)
 */

PDCI_E_FLOAT_WRITE_FN(Pe_float32,  Pfloat32,  PDCI_float32_2e,  "Pfloat32",  P_FLOAT32_DEF_INV_VAL,  Pa_float32 )
PDCI_E_FLOAT_WRITE_FN(Pe_float64,  Pfloat64,  PDCI_float64_2e,  "Pfloat64",  P_FLOAT64_DEF_INV_VAL,  Pa_float64 )

/*
 * fmt2buf/fmt2io functions 
 */

#if P_CONFIG_A_CHAR_STRING > 0
PDCI_FMT_FN      (Pa_char,        Pa_char,        Pchar,                    "Pa_char")
PDCI_FMT_FN_XTRA1(Pa_string,      Pa_string,      Pstring,  Pchar,          "Pstring")
PDCI_FMT_FN_XTRA1(Pa_string_FW,   Pa_string_FW,   Pstring,  size_t,         "Pstring_FW")
PDCI_FMT_FN_XTRA1(Pa_string_ME,   Pa_string_ME,   Pstring,  const char *,   "Pstring_ME")
PDCI_FMT_FN_XTRA1(Pa_string_CME,  Pa_string_CME,  Pstring,  Pregexp_t *,    "Pstring_CME")
PDCI_FMT_FN_XTRA1(Pa_string_SE,   Pa_string_SE,   Pstring,  const char *,   "Pstring_SE")
PDCI_FMT_FN_XTRA1(Pa_string_CSE,  Pa_string_CSE,  Pstring,  Pregexp_t *,    "Pstring_CSE")
#endif

#if P_CONFIG_E_CHAR_STRING > 0
PDCI_FMT_FN      (Pe_char,        Pa_char,        Pchar,                    "Pe_char")
PDCI_FMT_FN_XTRA1(Pe_string,      Pa_string,      Pstring,  Pchar,          "Pe_string")
PDCI_FMT_FN_XTRA1(Pe_string_FW,   Pa_string_FW,   Pstring,  size_t,         "Pe_string_FW")
PDCI_FMT_FN_XTRA1(Pe_string_ME,   Pa_string_ME,   Pstring,  const char *,   "Pe_string_ME")
PDCI_FMT_FN_XTRA1(Pe_string_CME,  Pa_string_CME,  Pstring,  Pregexp_t *,    "Pe_string_CME")
PDCI_FMT_FN_XTRA1(Pe_string_SE,   Pa_string_SE,   Pstring,  const char *,   "Pe_string_SE")
PDCI_FMT_FN_XTRA1(Pe_string_CSE,  Pa_string_CSE,  Pstring,  Pregexp_t *,    "Pe_string_CSE")
#endif

#if P_CONFIG_A_CHAR_STRING > 0 || P_CONFIG_E_CHAR_STRING > 0
PDCI_FMT_FN      (Pchar,        Pa_char,        Pchar,                      "Pchar")
PDCI_FMT_FN_XTRA1(Pstring,      Pa_string,      Pstring,  Pchar,            "Pstring")
PDCI_FMT_FN_XTRA1(Pstring_FW,   Pa_string_FW,   Pstring,  size_t,           "Pstring_FW")
PDCI_FMT_FN_XTRA1(Pstring_ME,   Pa_string_ME,   Pstring,  const char *,     "Pstring_ME")
PDCI_FMT_FN_XTRA1(Pstring_CME,  Pa_string_CME,  Pstring,  Pregexp_t *,      "Pstring_CME")
PDCI_FMT_FN_XTRA1(Pstring_SE,   Pa_string_SE,   Pstring,  const char *,     "Pstring_SE")
PDCI_FMT_FN_XTRA1(Pstring_CSE,  Pa_string_CSE,  Pstring,  Pregexp_t *,      "Pstring_CSE")
#endif

#if P_CONFIG_A_CHAR_STRING > 0

PDCI_FMT_FN_XTRA3(Pa_timestamp_explicit,      Pa_timestamp_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pa_timestamp_explicit")
PDCI_FMT_FN_XTRA3(Pa_timestamp_explicit_FW,   Pa_timestamp_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pa_timestamp_explicit_FW")
PDCI_FMT_FN_XTRA3(Pa_timestamp_explicit_ME,   Pa_timestamp_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pa_timestamp_explicit_ME")
PDCI_FMT_FN_XTRA3(Pa_timestamp_explicit_CME,  Pa_timestamp_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pa_timestamp_explicit_CME")
PDCI_FMT_FN_XTRA3(Pa_timestamp_explicit_SE,   Pa_timestamp_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pa_timestamp_explicit_SE")
PDCI_FMT_FN_XTRA3(Pa_timestamp_explicit_CSE,  Pa_timestamp_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pa_timestamp_explicit_CSE")

PDCI_FMT_FN_XTRA3(Pa_date_explicit,      Pa_date_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pa_date_explicit")
PDCI_FMT_FN_XTRA3(Pa_date_explicit_FW,   Pa_date_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pa_date_explicit_FW")
PDCI_FMT_FN_XTRA3(Pa_date_explicit_ME,   Pa_date_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pa_date_explicit_ME")
PDCI_FMT_FN_XTRA3(Pa_date_explicit_CME,  Pa_date_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pa_date_explicit_CME")
PDCI_FMT_FN_XTRA3(Pa_date_explicit_SE,   Pa_date_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pa_date_explicit_SE")
PDCI_FMT_FN_XTRA3(Pa_date_explicit_CSE,  Pa_date_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pa_date_explicit_CSE")

PDCI_FMT_FN_XTRA3(Pa_time_explicit,      Pa_time_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pa_time_explicit")
PDCI_FMT_FN_XTRA3(Pa_time_explicit_FW,   Pa_time_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pa_time_explicit_FW")
PDCI_FMT_FN_XTRA3(Pa_time_explicit_ME,   Pa_time_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pa_time_explicit_ME")
PDCI_FMT_FN_XTRA3(Pa_time_explicit_CME,  Pa_time_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pa_time_explicit_CME")
PDCI_FMT_FN_XTRA3(Pa_time_explicit_SE,   Pa_time_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pa_time_explicit_SE")
PDCI_FMT_FN_XTRA3(Pa_time_explicit_CSE,  Pa_time_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pa_time_explicit_CSE")

PDCI_FMT_FN_XTRA1(Pa_timestamp,      Pa_timestamp,      Puint32,  Pchar,              "Pa_timestamp")
PDCI_FMT_FN_XTRA1(Pa_timestamp_FW,   Pa_timestamp_FW,   Puint32,  size_t,             "Pa_timestamp_FW")
PDCI_FMT_FN_XTRA1(Pa_timestamp_ME,   Pa_timestamp_ME,   Puint32,  const char *,       "Pa_timestamp_ME")
PDCI_FMT_FN_XTRA1(Pa_timestamp_CME,  Pa_timestamp_CME,  Puint32,  Pregexp_t *,        "Pa_timestamp_CME")
PDCI_FMT_FN_XTRA1(Pa_timestamp_SE,   Pa_timestamp_SE,   Puint32,  const char *,       "Pa_timestamp_SE")
PDCI_FMT_FN_XTRA1(Pa_timestamp_CSE,  Pa_timestamp_CSE,  Puint32,  Pregexp_t *,        "Pa_timestamp_CSE")

PDCI_FMT_FN_XTRA1(Pa_date,      Pa_date,      Puint32,  Pchar,              "Pa_date")
PDCI_FMT_FN_XTRA1(Pa_date_FW,   Pa_date_FW,   Puint32,  size_t,             "Pa_date_FW")
PDCI_FMT_FN_XTRA1(Pa_date_ME,   Pa_date_ME,   Puint32,  const char *,       "Pa_date_ME")
PDCI_FMT_FN_XTRA1(Pa_date_CME,  Pa_date_CME,  Puint32,  Pregexp_t *,        "Pa_date_CME")
PDCI_FMT_FN_XTRA1(Pa_date_SE,   Pa_date_SE,   Puint32,  const char *,       "Pa_date_SE")
PDCI_FMT_FN_XTRA1(Pa_date_CSE,  Pa_date_CSE,  Puint32,  Pregexp_t *,        "Pa_date_CSE")

PDCI_FMT_FN_XTRA1(Pa_time,      Pa_time,      Puint32,  Pchar,              "Pa_time")
PDCI_FMT_FN_XTRA1(Pa_time_FW,   Pa_time_FW,   Puint32,  size_t,             "Pa_time_FW")
PDCI_FMT_FN_XTRA1(Pa_time_ME,   Pa_time_ME,   Puint32,  const char *,       "Pa_time_ME")
PDCI_FMT_FN_XTRA1(Pa_time_CME,  Pa_time_CME,  Puint32,  Pregexp_t *,        "Pa_time_CME")
PDCI_FMT_FN_XTRA1(Pa_time_SE,   Pa_time_SE,   Puint32,  const char *,       "Pa_time_SE")
PDCI_FMT_FN_XTRA1(Pa_time_CSE,  Pa_time_CSE,  Puint32,  Pregexp_t *,        "Pa_time_CSE")
#endif

#if P_CONFIG_E_CHAR_STRING > 0

PDCI_FMT_FN_XTRA3(Pe_timestamp_explicit,      Pa_timestamp_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pe_timestamp_explicit")
PDCI_FMT_FN_XTRA3(Pe_timestamp_explicit_FW,   Pa_timestamp_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pe_timestamp_explicit_FW")
PDCI_FMT_FN_XTRA3(Pe_timestamp_explicit_ME,   Pa_timestamp_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pe_timestamp_explicit_ME")
PDCI_FMT_FN_XTRA3(Pe_timestamp_explicit_CME,  Pa_timestamp_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pe_timestamp_explicit_CME")
PDCI_FMT_FN_XTRA3(Pe_timestamp_explicit_SE,   Pa_timestamp_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pe_timestamp_explicit_SE")
PDCI_FMT_FN_XTRA3(Pe_timestamp_explicit_CSE,  Pa_timestamp_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pe_timestamp_explicit_CSE")

PDCI_FMT_FN_XTRA3(Pe_date_explicit,      Pa_date_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pe_date_explicit")
PDCI_FMT_FN_XTRA3(Pe_date_explicit_FW,   Pa_date_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pe_date_explicit_FW")
PDCI_FMT_FN_XTRA3(Pe_date_explicit_ME,   Pa_date_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pe_date_explicit_ME")
PDCI_FMT_FN_XTRA3(Pe_date_explicit_CME,  Pa_date_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pe_date_explicit_CME")
PDCI_FMT_FN_XTRA3(Pe_date_explicit_SE,   Pa_date_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pe_date_explicit_SE")
PDCI_FMT_FN_XTRA3(Pe_date_explicit_CSE,  Pa_date_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pe_date_explicit_CSE")

PDCI_FMT_FN_XTRA3(Pe_time_explicit,      Pa_time_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pe_time_explicit")
PDCI_FMT_FN_XTRA3(Pe_time_explicit_FW,   Pa_time_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pe_time_explicit_FW")
PDCI_FMT_FN_XTRA3(Pe_time_explicit_ME,   Pa_time_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pe_time_explicit_ME")
PDCI_FMT_FN_XTRA3(Pe_time_explicit_CME,  Pa_time_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pe_time_explicit_CME")
PDCI_FMT_FN_XTRA3(Pe_time_explicit_SE,   Pa_time_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pe_time_explicit_SE")
PDCI_FMT_FN_XTRA3(Pe_time_explicit_CSE,  Pa_time_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pe_time_explicit_CSE")

PDCI_FMT_FN_XTRA1(Pe_timestamp,      Pa_timestamp,      Puint32,  Pchar,              "Pe_timestamp")
PDCI_FMT_FN_XTRA1(Pe_timestamp_FW,   Pa_timestamp_FW,   Puint32,  size_t,             "Pe_timestamp_FW")
PDCI_FMT_FN_XTRA1(Pe_timestamp_ME,   Pa_timestamp_ME,   Puint32,  const char *,       "Pe_timestamp_ME")
PDCI_FMT_FN_XTRA1(Pe_timestamp_CME,  Pa_timestamp_CME,  Puint32,  Pregexp_t *,        "Pe_timestamp_CME")
PDCI_FMT_FN_XTRA1(Pe_timestamp_SE,   Pa_timestamp_SE,   Puint32,  const char *,       "Pe_timestamp_SE")
PDCI_FMT_FN_XTRA1(Pe_timestamp_CSE,  Pa_timestamp_CSE,  Puint32,  Pregexp_t *,        "Pe_timestamp_CSE")

PDCI_FMT_FN_XTRA1(Pe_date,      Pa_date,      Puint32,  Pchar,              "Pe_date")
PDCI_FMT_FN_XTRA1(Pe_date_FW,   Pa_date_FW,   Puint32,  size_t,             "Pe_date_FW")
PDCI_FMT_FN_XTRA1(Pe_date_ME,   Pa_date_ME,   Puint32,  const char *,       "Pe_date_ME")
PDCI_FMT_FN_XTRA1(Pe_date_CME,  Pa_date_CME,  Puint32,  Pregexp_t *,        "Pe_date_CME")
PDCI_FMT_FN_XTRA1(Pe_date_SE,   Pa_date_SE,   Puint32,  const char *,       "Pe_date_SE")
PDCI_FMT_FN_XTRA1(Pe_date_CSE,  Pa_date_CSE,  Puint32,  Pregexp_t *,        "Pe_date_CSE")

PDCI_FMT_FN_XTRA1(Pe_time,      Pa_time,      Puint32,  Pchar,              "Pe_time")
PDCI_FMT_FN_XTRA1(Pe_time_FW,   Pa_time_FW,   Puint32,  size_t,             "Pe_time_FW")
PDCI_FMT_FN_XTRA1(Pe_time_ME,   Pa_time_ME,   Puint32,  const char *,       "Pe_time_ME")
PDCI_FMT_FN_XTRA1(Pe_time_CME,  Pa_time_CME,  Puint32,  Pregexp_t *,        "Pe_time_CME")
PDCI_FMT_FN_XTRA1(Pe_time_SE,   Pa_time_SE,   Puint32,  const char *,       "Pe_time_SE")
PDCI_FMT_FN_XTRA1(Pe_time_CSE,  Pa_time_CSE,  Puint32,  Pregexp_t *,        "Pe_time_CSE")
#endif

#if P_CONFIG_A_CHAR_STRING > 0 || P_CONFIG_E_CHAR_STRING > 0

PDCI_FMT_FN_XTRA3(Ptimestamp_explicit,      Pa_timestamp_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Ptimestamp_explicit")
PDCI_FMT_FN_XTRA3(Ptimestamp_explicit_FW,   Pa_timestamp_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Ptimestamp_explicit_FW")
PDCI_FMT_FN_XTRA3(Ptimestamp_explicit_ME,   Pa_timestamp_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Ptimestamp_explicit_ME")
PDCI_FMT_FN_XTRA3(Ptimestamp_explicit_CME,  Pa_timestamp_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Ptimestamp_explicit_CME")
PDCI_FMT_FN_XTRA3(Ptimestamp_explicit_SE,   Pa_timestamp_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Ptimestamp_explicit_SE")
PDCI_FMT_FN_XTRA3(Ptimestamp_explicit_CSE,  Pa_timestamp_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Ptimestamp_explicit_CSE")

PDCI_FMT_FN_XTRA3(Pdate_explicit,      Pa_date_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Pdate_explicit")
PDCI_FMT_FN_XTRA3(Pdate_explicit_FW,   Pa_date_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Pdate_explicit_FW")
PDCI_FMT_FN_XTRA3(Pdate_explicit_ME,   Pa_date_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pdate_explicit_ME")
PDCI_FMT_FN_XTRA3(Pdate_explicit_CME,  Pa_date_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pdate_explicit_CME")
PDCI_FMT_FN_XTRA3(Pdate_explicit_SE,   Pa_date_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Pdate_explicit_SE")
PDCI_FMT_FN_XTRA3(Pdate_explicit_CSE,  Pa_date_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Pdate_explicit_CSE")

PDCI_FMT_FN_XTRA3(Ptime_explicit,      Pa_time_explicit,      Puint32,  Pchar,         const char *,  Tm_zone_t *, "Ptime_explicit")
PDCI_FMT_FN_XTRA3(Ptime_explicit_FW,   Pa_time_explicit_FW,   Puint32,  size_t,        const char *,  Tm_zone_t *, "Ptime_explicit_FW")
PDCI_FMT_FN_XTRA3(Ptime_explicit_ME,   Pa_time_explicit_ME,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Ptime_explicit_ME")
PDCI_FMT_FN_XTRA3(Ptime_explicit_CME,  Pa_time_explicit_CME,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Ptime_explicit_CME")
PDCI_FMT_FN_XTRA3(Ptime_explicit_SE,   Pa_time_explicit_SE,   Puint32,  const char *,  const char *,  Tm_zone_t *, "Ptime_explicit_SE")
PDCI_FMT_FN_XTRA3(Ptime_explicit_CSE,  Pa_time_explicit_CSE,  Puint32,  Pregexp_t *,   const char *,  Tm_zone_t *, "Ptime_explicit_CSE")

PDCI_FMT_FN_XTRA1(Ptimestamp,      Pa_timestamp,      Puint32,  Pchar,                "Ptimestamp")
PDCI_FMT_FN_XTRA1(Ptimestamp_FW,   Pa_timestamp_FW,   Puint32,  size_t,               "Ptimestamp_FW")
PDCI_FMT_FN_XTRA1(Ptimestamp_ME,   Pa_timestamp_ME,   Puint32,  const char *,         "Ptimestamp_ME")
PDCI_FMT_FN_XTRA1(Ptimestamp_CME,  Pa_timestamp_CME,  Puint32,  Pregexp_t *,          "Ptimestamp_CME")
PDCI_FMT_FN_XTRA1(Ptimestamp_SE,   Pa_timestamp_SE,   Puint32,  const char *,         "Ptimestamp_SE")
PDCI_FMT_FN_XTRA1(Ptimestamp_CSE,  Pa_timestamp_CSE,  Puint32,  Pregexp_t *,          "Ptimestamp_CSE")

PDCI_FMT_FN_XTRA1(Pdate,      Pa_date,      Puint32,  Pchar,                "Pdate")
PDCI_FMT_FN_XTRA1(Pdate_FW,   Pa_date_FW,   Puint32,  size_t,               "Pdate_FW")
PDCI_FMT_FN_XTRA1(Pdate_ME,   Pa_date_ME,   Puint32,  const char *,         "Pdate_ME")
PDCI_FMT_FN_XTRA1(Pdate_CME,  Pa_date_CME,  Puint32,  Pregexp_t *,          "Pdate_CME")
PDCI_FMT_FN_XTRA1(Pdate_SE,   Pa_date_SE,   Puint32,  const char *,         "Pdate_SE")
PDCI_FMT_FN_XTRA1(Pdate_CSE,  Pa_date_CSE,  Puint32,  Pregexp_t *,          "Pdate_CSE")

PDCI_FMT_FN_XTRA1(Ptime,      Pa_time,      Puint32,  Pchar,                "Ptime")
PDCI_FMT_FN_XTRA1(Ptime_FW,   Pa_time_FW,   Puint32,  size_t,               "Ptime_FW")
PDCI_FMT_FN_XTRA1(Ptime_ME,   Pa_time_ME,   Puint32,  const char *,         "Ptime_ME")
PDCI_FMT_FN_XTRA1(Ptime_CME,  Pa_time_CME,  Puint32,  Pregexp_t *,          "Ptime_CME")
PDCI_FMT_FN_XTRA1(Ptime_SE,   Pa_time_SE,   Puint32,  const char *,         "Ptime_SE")
PDCI_FMT_FN_XTRA1(Ptime_CSE,  Pa_time_CSE,  Puint32,  Pregexp_t *,          "Ptime_CSE")
#endif

#if P_CONFIG_A_INT > 0
PDCI_FMT_FN(Pa_ip,   Pa_ip,      Puint32,         "Pa_ip"  )
#endif

#if P_CONFIG_E_INT > 0
PDCI_FMT_FN(Pe_ip,   Pa_ip,      Puint32,         "Pe_ip"  )
#endif

#if P_CONFIG_A_INT > 0 || P_CONFIG_E_INT > 0
PDCI_FMT_FN(Pip,     Pa_ip,      Puint32,         "Pip"    )
#endif

#if P_CONFIG_A_INT > 0
PDCI_FMT_FN(Pa_int8,   Pa_int8,   Pint8  ,                 "Pa_int8"  )
PDCI_FMT_FN(Pa_int16,  Pa_int16,  Pint16 ,                 "Pa_int16" )
PDCI_FMT_FN(Pa_int32,  Pa_int32,  Pint32 ,                 "Pa_int32" )
PDCI_FMT_FN(Pa_int64,  Pa_int64,  Pint64 ,                 "Pa_int64" )
PDCI_FMT_FN(Pa_uint8,  Pa_uint8,  Puint8 ,                 "Pa_uint8" )
PDCI_FMT_FN(Pa_uint16, Pa_uint16, Puint16,                 "Pa_uint16")
PDCI_FMT_FN(Pa_uint32, Pa_uint32, Puint32,                 "Pa_uint32")
PDCI_FMT_FN(Pa_uint64, Pa_uint64, Puint64,                 "Pa_uint64")
#endif

#if P_CONFIG_A_INT_FW > 0
PDCI_FMT_FN_XTRA1(Pa_int8_FW,   Pa_int8_FW,   Pint8,   size_t,         "Pa_int8_FW"  )
PDCI_FMT_FN_XTRA1(Pa_int16_FW,  Pa_int16_FW,  Pint16,  size_t,         "Pa_int16_FW" )
PDCI_FMT_FN_XTRA1(Pa_int32_FW,  Pa_int32_FW,  Pint32,  size_t,         "Pa_int32_FW" )
PDCI_FMT_FN_XTRA1(Pa_int64_FW,  Pa_int64_FW,  Pint64,  size_t,         "Pa_int64_FW" )
PDCI_FMT_FN_XTRA1(Pa_uint8_FW,  Pa_uint8_FW,  Puint8,  size_t,         "Pa_uint8_FW" )
PDCI_FMT_FN_XTRA1(Pa_uint16_FW, Pa_uint16_FW, Puint16, size_t,         "Pa_uint16_FW")
PDCI_FMT_FN_XTRA1(Pa_uint32_FW, Pa_uint32_FW, Puint32, size_t,         "Pa_uint32_FW")
PDCI_FMT_FN_XTRA1(Pa_uint64_FW, Pa_uint64_FW, Puint64, size_t,         "Pa_uint64_FW")
#endif

#if P_CONFIG_E_INT > 0
PDCI_FMT_FN(Pe_int8,   Pa_int8,   Pint8  ,                 "Pe_int8"  )
PDCI_FMT_FN(Pe_int16,  Pa_int16,  Pint16 ,                 "Pe_int16" )
PDCI_FMT_FN(Pe_int32,  Pa_int32,  Pint32 ,                 "Pe_int32" )
PDCI_FMT_FN(Pe_int64,  Pa_int64,  Pint64 ,                 "Pe_int64" )
PDCI_FMT_FN(Pe_uint8,  Pa_uint8,  Puint8 ,                 "Pe_uint8" )
PDCI_FMT_FN(Pe_uint16, Pa_uint16, Puint16,                 "Pe_uint16")
PDCI_FMT_FN(Pe_uint32, Pa_uint32, Puint32,                 "Pe_uint32")
PDCI_FMT_FN(Pe_uint64, Pa_uint64, Puint64,                 "Pe_uint64")
#endif

#if P_CONFIG_E_INT_FW > 0
PDCI_FMT_FN_XTRA1(Pe_int8_FW,   Pa_int8_FW,   Pint8,   size_t,         "Pe_int8_FW"  )
PDCI_FMT_FN_XTRA1(Pe_int16_FW,  Pa_int16_FW,  Pint16,  size_t,         "Pe_int16_FW" )
PDCI_FMT_FN_XTRA1(Pe_int32_FW,  Pa_int32_FW,  Pint32,  size_t,         "Pe_int32_FW" )
PDCI_FMT_FN_XTRA1(Pe_int64_FW,  Pa_int64_FW,  Pint64,  size_t,         "Pe_int64_FW" )
PDCI_FMT_FN_XTRA1(Pe_uint8_FW,  Pa_uint8_FW,  Puint8,  size_t,         "Pe_uint8_FW" )
PDCI_FMT_FN_XTRA1(Pe_uint16_FW, Pa_uint16_FW, Puint16, size_t,         "Pe_uint16_FW")
PDCI_FMT_FN_XTRA1(Pe_uint32_FW, Pa_uint32_FW, Puint32, size_t,         "Pe_uint32_FW")
PDCI_FMT_FN_XTRA1(Pe_uint64_FW, Pa_uint64_FW, Puint64, size_t,         "Pe_uint64_FW")
#endif

#if P_CONFIG_A_INT > 0 || P_CONFIG_E_INT > 0
PDCI_FMT_FN(Pint8,   Pa_int8,   Pint8  ,                 "Pint8"  )
PDCI_FMT_FN(Pint16,  Pa_int16,  Pint16 ,                 "Pint16" )
PDCI_FMT_FN(Pint32,  Pa_int32,  Pint32 ,                 "Pint32" )
PDCI_FMT_FN(Pint64,  Pa_int64,  Pint64 ,                 "Pint64" )
PDCI_FMT_FN(Puint8,  Pa_uint8,  Puint8 ,                 "Puint8" )
PDCI_FMT_FN(Puint16, Pa_uint16, Puint16,                 "Puint16")
PDCI_FMT_FN(Puint32, Pa_uint32, Puint32,                 "Puint32")
PDCI_FMT_FN(Puint64, Pa_uint64, Puint64,                 "Puint64")
#endif

#if P_CONFIG_A_INT_FW > 0 || P_CONFIG_E_FW_INT > 0
PDCI_FMT_FN_XTRA1(Pint8_FW,   Pa_int8_FW,   Pint8,   size_t,         "Pint8_FW"  )
PDCI_FMT_FN_XTRA1(Pint16_FW,  Pa_int16_FW,  Pint16,  size_t,         "Pint16_FW" )
PDCI_FMT_FN_XTRA1(Pint32_FW,  Pa_int32_FW,  Pint32,  size_t,         "Pint32_FW" )
PDCI_FMT_FN_XTRA1(Pint64_FW,  Pa_int64_FW,  Pint64,  size_t,         "Pint64_FW" )
PDCI_FMT_FN_XTRA1(Puint8_FW,  Pa_uint8_FW,  Puint8,  size_t,         "Puint8_FW" )
PDCI_FMT_FN_XTRA1(Puint16_FW, Pa_uint16_FW, Puint16, size_t,         "Puint16_FW")
PDCI_FMT_FN_XTRA1(Puint32_FW, Pa_uint32_FW, Puint32, size_t,         "Puint32_FW")
PDCI_FMT_FN_XTRA1(Puint64_FW, Pa_uint64_FW, Puint64, size_t,         "Puint64_FW")
#endif

#if P_CONFIG_B_INT > 0
PDCI_FMT_FN(Pb_int8,   Pa_int8,   Pint8  ,                 "Pb_int8"  )
PDCI_FMT_FN(Pb_int16,  Pa_int16,  Pint16 ,                 "Pb_int16" )
PDCI_FMT_FN(Pb_int32,  Pa_int32,  Pint32 ,                 "Pb_int32" )
PDCI_FMT_FN(Pb_int64,  Pa_int64,  Pint64 ,                 "Pb_int64" )
PDCI_FMT_FN(Pb_uint8,  Pa_uint8,  Puint8 ,                 "Pb_uint8" )
PDCI_FMT_FN(Pb_uint16, Pa_uint16, Puint16,                 "Pb_uint16")
PDCI_FMT_FN(Pb_uint32, Pa_uint32, Puint32,                 "Pb_uint32")
PDCI_FMT_FN(Pb_uint64, Pa_uint64, Puint64,                 "Pb_uint64")
#endif

#if P_CONFIG_EBC_INT > 0
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_int8,   Pa_int8,   Pint8,   Puint32,                 "Pebc_int8"  )
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_int16,  Pa_int16,  Pint16,  Puint32,                 "Pebc_int16" )
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_int32,  Pa_int32,  Pint32,  Puint32,                 "Pebc_int32" )
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_int64,  Pa_int64,  Pint64,  Puint32,                 "Pebc_int64" )
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_uint8,  Pa_uint8,  Puint8,  Puint32,                 "Pebc_uint8" )
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_uint16, Pa_uint16, Puint16, Puint32,                 "Pebc_uint16")
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_uint32, Pa_uint32, Puint32, Puint32,                 "Pebc_uint32")
PDCI_FMT_FN_XTRA1_IGNORE(Pebc_uint64, Pa_uint64, Puint64, Puint32,                 "Pebc_uint64")
#endif

#if P_CONFIG_BCD_INT > 0
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_int8,   Pa_int8,   Pint8,   Puint32,                 "Pbcd_int8"  )
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_int16,  Pa_int16,  Pint16,  Puint32,                 "Pbcd_int16" )
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_int32,  Pa_int32,  Pint32,  Puint32,                 "Pbcd_int32" )
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_int64,  Pa_int64,  Pint64,  Puint32,                 "Pbcd_int64" )
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_uint8,  Pa_uint8,  Puint8,  Puint32,                 "Pbcd_uint8" )
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_uint16, Pa_uint16, Puint16, Puint32,                 "Pbcd_uint16")
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_uint32, Pa_uint32, Puint32, Puint32,                 "Pbcd_uint32")
PDCI_FMT_FN_XTRA1_IGNORE(Pbcd_uint64, Pa_uint64, Puint64, Puint32,                 "Pbcd_uint64")
#endif

#if P_CONFIG_SBL_INT > 0
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_int8,   Pa_int8,   Pint8,   Puint32,                 "Psbl_int8"  )
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_int16,  Pa_int16,  Pint16,  Puint32,                 "Psbl_int16" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_int32,  Pa_int32,  Pint32,  Puint32,                 "Psbl_int32" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_int64,  Pa_int64,  Pint64,  Puint32,                 "Psbl_int64" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_uint8,  Pa_uint8,  Puint8,  Puint32,                 "Psbl_uint8" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_uint16, Pa_uint16, Puint16, Puint32,                 "Psbl_uint16")
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_uint32, Pa_uint32, Puint32, Puint32,                 "Psbl_uint32")
PDCI_FMT_FN_XTRA1_IGNORE(Psbl_uint64, Pa_uint64, Puint64, Puint32,                 "Psbl_uint64")
#endif

#if P_CONFIG_SBH_INT > 0
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_int8,   Pa_int8,   Pint8,   Puint32,                 "Psbh_int8"  )
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_int16,  Pa_int16,  Pint16,  Puint32,                 "Psbh_int16" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_int32,  Pa_int32,  Pint32,  Puint32,                 "Psbh_int32" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_int64,  Pa_int64,  Pint64,  Puint32,                 "Psbh_int64" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_uint8,  Pa_uint8,  Puint8,  Puint32,                 "Psbh_uint8" )
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_uint16, Pa_uint16, Puint16, Puint32,                 "Psbh_uint16")
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_uint32, Pa_uint32, Puint32, Puint32,                 "Psbh_uint32")
PDCI_FMT_FN_XTRA1_IGNORE(Psbh_uint64, Pa_uint64, Puint64, Puint32,                 "Psbh_uint64")
#endif

#if P_CONFIG_A_FLOAT > 0
PDCI_FMT_FN(Pa_float32,  Pa_float32,  Pfloat32,                                  "Pa_float32")
PDCI_FMT_FN(Pa_float64,  Pa_float64,  Pfloat64,                                  "Pe_float64")
#endif

#if P_CONFIG_E_FLOAT > 0
PDCI_FMT_FN(Pe_float32,  Pa_float32,  Pfloat32,                                  "Pe_float32")
PDCI_FMT_FN(Pe_float64,  Pa_float64,  Pfloat64,                                  "Pe_float64")
#endif

#if P_CONFIG_A_FLOAT > 0 || P_CONFIG_E_FLOAT > 0
PDCI_FMT_FN(Pfloat32,  Pa_float32,  Pfloat32,                                  "Pfloat32")
PDCI_FMT_FN(Pfloat64,  Pa_float64,  Pfloat64,                                  "Pfloat64")
#endif

/* ********************************* BEGIN_TRAILER ******************************** */
/* ********************************** END_MACGEN ********************************** */
/* ********************** BEGIN_MACGEN(pads-acc-gen.c) ************************ */
/*
 * Generated accumulator functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-internal.h"
#gen_include "pads-macros-gen.h"

/* ********************************** END_HEADER ********************************** */
#gen_include "pads-acc-macros-gen.h"

/* PDCI_INT_ACCUM_GEN(int_type, int_descr, num_bytes, fmt, mfmt, fold_test) */
/* Always generate uint8, int32 and uint32 accumulator types */

PDCI_INT_ACCUM_GEN(Puint8,  "uint8",  1, "I1u",   "llu", PDCI_FOLDTEST_UINT8)
PDCI_INT_ACCUM_GEN(Pint32,  "int32",  4, "I4d",  "lld", PDCI_FOLDTEST_INT32)
PDCI_INT_ACCUM_GEN(Puint32, "uint32", 4, "I4u",  "llu", PDCI_FOLDTEST_UINT32)

/* PDCI_INT_ACCUM(int_type, int_descr, num_bytes, fmt, mfmt, fold_test) */

PDCI_INT_ACCUM(Pint8,   "int8",   1, "I1d",   "lld", PDCI_FOLDTEST_INT8)
PDCI_INT_ACCUM(Pint16,  "int16",  2, "I2d",   "lld", PDCI_FOLDTEST_INT16)
PDCI_INT_ACCUM(Puint16, "uint16", 2, "I2u",   "llu", PDCI_FOLDTEST_UINT16)
PDCI_INT_ACCUM(Pint64,  "int64",  8, "lld", "lld", PDCI_FOLDTEST_INT64)
PDCI_INT_ACCUM(Puint64, "uint64", 8, "llu", "llu", PDCI_FOLDTEST_UINT64)

/* PDCI_INT_ACCUM_MAP_REPORT_GEN(int_type, int_descr, fmt, mfmt) */
/* Always generate this report function */  
PDCI_INT_ACCUM_MAP_REPORT_GEN(Pint32, "int32", "I4d", "lld")

/* PDCI_FPOINT_ACCUM(fpoint_type, fpoint_descr) */

PDCI_FPOINT_ACCUM(Pfpoint8,   "fpoint8")
PDCI_FPOINT_ACCUM(Pufpoint8,  "ufpoint8")
PDCI_FPOINT_ACCUM(Pfpoint16,  "fpoint16")
PDCI_FPOINT_ACCUM(Pufpoint16, "ufpoint16")
PDCI_FPOINT_ACCUM(Pfpoint32,  "fpoint32")
PDCI_FPOINT_ACCUM(Pufpoint32, "ufpoint32")
PDCI_FPOINT_ACCUM(Pfpoint64,  "fpoint64")
PDCI_FPOINT_ACCUM(Pufpoint64, "ufpoint64")

/* PDCI_FLOAT_ACCUM(float_type, float_descr) */
PDCI_FLOAT_ACCUM(Pfloat32,  "float32")
PDCI_FLOAT_ACCUM(Pfloat64,  "float64")

/* ********************************* BEGIN_TRAILER ******************************** */

#if P_CONFIG_ACCUM_FUNCTIONS > 0

typedef struct PDCI_string_dt_key_s {
  Puint64      cnt;
  size_t       len;
  char        *str;
} PDCI_string_dt_key_t;

typedef struct PDCI_string_dt_elt_s {
  PDCI_string_dt_key_t  key;
  Dtlink_t              link;
  char                  buf[1];
} PDCI_string_dt_elt_t;

unsigned int
PDCI_string_dt_elt_hash(Dt_t *dt, Void_t *key, Dtdisc_t *disc)
{
  PDCI_string_dt_key_t *k = (PDCI_string_dt_key_t*)key;
  int len = k->len;
  NoP(dt);
  NoP(disc);
  if (len == 0) return 0;
  return dtstrhash(0, k->str, len);
}

/*
 * Order set comparison function: only used at the end to rehash
 * the (formerly unordered) set.  Since same string only occurs
 * once, ptr equivalence produces key equivalence.
 *   different keys: sort keys by cnt field, break tie with string vals
 */
int
PDCI_string_dt_elt_oset_cmp(Dt_t *dt, PDCI_string_dt_key_t *a, PDCI_string_dt_key_t *b, Dtdisc_t *disc)
{
  size_t min_len;
  int res;
  NoP(dt);
  NoP(disc);
  if (a == b) { /* same key */
    return 0;
  }
  if (a->cnt == b->cnt) { /* same count, so do lexicographic comparison */
    if (a->len == 0) return -1;
    if (b->len == 0) return 1;
    min_len = (a->len < b->len) ? a->len : b->len;
    if ((res = strncmp(a->str, b->str, min_len))) {
      return res;
    }
    return (a->len < b->len) ? -1 : 1;
  }
  /* different counts */
  return (a->cnt > b->cnt) ? -1 : 1;
}

/*
 * Unordered set comparison function: all that matters is string equality
 * (0 => equal, 1 => not equal)
 */
int
PDCI_string_dt_elt_set_cmp(Dt_t *dt, PDCI_string_dt_key_t *a, PDCI_string_dt_key_t *b, Dtdisc_t *disc)
{
  NoP(dt);
  NoP(disc);
  if (a->len != b->len) return 1;
  if (a->len == 0) return 0;
  return strncmp(a->str, b->str, a->len);
}

void*
PDCI_string_dt_elt_make(Dt_t *dt, PDCI_string_dt_elt_t *a, Dtdisc_t *disc)
{
  PDCI_string_dt_elt_t *b;
  NoP(dt);
  NoP(disc);
  if ((b = oldof(0, PDCI_string_dt_elt_t, 1, a->key.len))) {
    memcpy(b->buf, a->key.str, a->key.len);
    b->key.cnt = a->key.cnt;
    b->key.len = a->key.len;
    b->key.str = b->buf;
  }
  return b;
}

void
PDCI_string_dt_elt_free(Dt_t *dt, PDCI_string_dt_elt_t *a, Dtdisc_t *disc)
{
  free(a);
}

static Dtdisc_t PDCI_string_acc_dt_set_disc = {
  DTOFFSET(PDCI_string_dt_elt_t, key),     /* key     */
  0,				           /* size    */
  DTOFFSET(PDCI_string_dt_elt_t, link),    /* link    */
  (Dtmake_f)PDCI_string_dt_elt_make,       /* makef   */
  (Dtfree_f)PDCI_string_dt_elt_free,       /* freef */
  (Dtcompar_f)PDCI_string_dt_elt_set_cmp,  /* comparf */
  (Dthash_f)PDCI_string_dt_elt_hash,       /* hashf   */
  NiL,				           /* memoryf */
  NiL				           /* eventf  */
};

static Dtdisc_t PDCI_string_acc_dt_oset_disc = {
  DTOFFSET(PDCI_string_dt_elt_t, key),     /* key     */
  0,				           /* size    */
  DTOFFSET(PDCI_string_dt_elt_t, link),    /* link    */
  (Dtmake_f)PDCI_string_dt_elt_make,       /* makef   */
  (Dtfree_f)PDCI_string_dt_elt_free,       /* freef */
  (Dtcompar_f)PDCI_string_dt_elt_oset_cmp, /* comparf */
  (Dthash_f)PDCI_string_dt_elt_hash,       /* hashf   */
  NiL,				           /* memoryf */
  NiL				           /* eventf  */
};

Perror_t
Pstring_acc_init(P_t *pads, Pstring_acc *a)
{
  PDCI_DISC_1P_CHECKS("Pstring_acc_init", a);
  if (!(a->dict = dtopen(&PDCI_string_acc_dt_set_disc, Dtset))) {
    return P_ERR;
  }
  a->max2track  = pads->disc->acc_max2track;
  a->max2rep    = pads->disc->acc_max2rep;
  a->pcnt2rep   = pads->disc->acc_pcnt2rep;
  a->tracked    = 0;
  return Puint32_acc_init(pads, &(a->len_accum));
}

Perror_t
Pstring_acc_reset(P_t *pads, Pstring_acc *a)
{
  PDCI_DISC_1P_CHECKS("Pstring_acc_reset", a);
  if (!a->dict) {
    return P_ERR;
  }
  dtclear(a->dict);
  a->tracked = 0;
  return Puint32_acc_reset(pads, &(a->len_accum));
}

Perror_t
Pstring_acc_cleanup(P_t *pads, Pstring_acc *a)
{
  PDCI_DISC_1P_CHECKS("Pstring_acc_cleanup", a);
  if (a->dict) {
    dtclose(a->dict);
    a->dict = 0;
  }
  return Puint32_acc_cleanup(pads, &(a->len_accum));
}

Perror_t
Pstring_acc_add(P_t *pads, Pstring_acc *a, const Pbase_pd *pd, const Pstring *val)
{
  Puint32 val_len;
  PDCI_string_dt_elt_t  insert_elt;
  PDCI_string_dt_key_t  lookup_key;
  PDCI_string_dt_elt_t  *tmp1;
  PDCI_DISC_3P_CHECKS("Pstring_acc_add", a, pd, val);
  if (!a->dict) {
    return P_ERR;
  }
  val_len = val->len;
  if (P_ERR == Puint32_acc_add(pads, &(a->len_accum), pd, &val_len)) {
    return P_ERR;
  }
  if (pd->errCode != P_NO_ERR) {
    return P_OK;
  }
  if (val->len == 0 || dtsize(a->dict) < a->max2track) {
#if 0
    if (val->len == 0) 
      P_WARN(pads->disc, "XXX_REMOVE val->len 0");
    else
      P_WARN3(pads->disc, "XXX_REMOVE val->len %lu string[%.*s]", (unsigned long)val->len, val->len, val->str);
#endif
    insert_elt.key.str = val->str;
    insert_elt.key.len = val->len;
    insert_elt.key.cnt = 0;
    if (!(tmp1 = dtinsert(a->dict, &insert_elt))) {
      P_WARN(pads->disc, "** PADSC internal error: dtinsert failed (out of memory?) **");
      return P_ERR;
    }
    (tmp1->key.cnt)++;
    (a->tracked)++;
  } else {
#if 0
    if (val->len == 0) 
      P_WARN(pads->disc, "XXX_REMOVE val->len 0 (max2track hit)");
    else
      P_WARN3(pads->disc, "XXX_REMOVE val->len %lu string[%.*s] (max2track hit)", (unsigned long)val->len, val->len, val->str);
#endif
    lookup_key.str = val->str;
    lookup_key.len = val->len;
    lookup_key.cnt = 0;
    if ((tmp1 = dtmatch(a->dict, &lookup_key))) {
      (tmp1->key.cnt)++;
      (a->tracked)++;
    }
  }
  return P_OK;
}

Perror_t
Pstring_acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what, int nst,
		      Pstring_acc *a)
{
  size_t                 pad;
  int                    i, sz, len_sz, rp;
  Puint64             cnt_sum;
  Pfloat64               cnt_sum_pcnt;
  Pfloat64               track_pcnt;
  Pfloat64               elt_pcnt;
  Void_t                 *velt;
  PDCI_string_dt_elt_t   *elt;

  P_TRACE(pads->disc, "Pstring_acc_report2io called");
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = "string";
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0);
  sz = dtsize(a->dict);
  len_sz = dtsize(a->len_accum.dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (len_sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No string values.)\n");
    return P_OK;
  }
  if (sz == 1 && a->len_accum.bad == 0) {
    elt = (PDCI_string_dt_elt_t*)dtfirst(a->dict);
    /*    sfprintf(outstr, "%llu string values, 100 pcnt good, 100 pcnt identical (length %8lu): %-.*s\n",
	     a->len_accum.good,
	     (unsigned long)elt->key.len, elt->key.len+2, P_qfmt_cstr_n(elt->key.str, elt->key.len)); */
         sfprintf(outstr, "%llu string values, 100 pcnt good, 100 pcnt identical (length %8lu): %s\n",
	     a->len_accum.good, (unsigned long)elt->key.len, P_qfmt_cstr_n(elt->key.str, elt->key.len)); 
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (P_ERR == Puint32_acc_report2io(pads, outstr, "String lengths", "string length", -1, &(a->len_accum))) {
    return P_ERR;
  }
  if (a->len_accum.good == 0) {
    sfprintf(outstr, "(No good string values.)\n");
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (PDCI_string_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good string values, 100 pcnt identical (length %lu): %-.*s\n",
	     (unsigned long)elt->key.len, elt->key.len+2, P_qfmt_cstr_n(elt->key.str, elt->key.len));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  /* rehash tree to get keys ordered by count */
  dtdisc(a->dict, &PDCI_string_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "\n  Characterizing string values:\n");
  sfprintf(outstr, "    => distribution of top %d strings out of %d distinct strings:\n", rp, sz);
  if (sz == a->max2track && a->len_accum.good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->len_accum.good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (PDCI_string_dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->len_accum.good);
    sfprintf(outstr, "        val: ");
    /*    sfprintf(outstr, "%-.*s", elt->key.len+2, P_qfmt_cstr_n(elt->key.str, elt->key.len)); */
    sfprintf(outstr, "%s", P_qfmt_cstr_n(elt->key.str, elt->key.len));
    sfprintf(outstr, "");
    pad = a->len_accum.max - elt->key.len;
    sfprintf(outstr, "%-.*s", pad, PDCI_spaces);
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->len_accum.good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr, ". . . . . . . .");
  pad = a->len_accum.max;
  sfprintf(outstr, "%-.*s", pad,
	   " . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .");
  sfprintf(outstr, " . . . . . . . . . . . . . . . . . . . . . . .\n");

  sfprintf(outstr, "        SUMMING");
  sfprintf(outstr, "%-.*s", pad, PDCI_spaces);
  sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict, &PDCI_string_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
Pstring_acc_report2xml_io(P_t *pads, Sfio_t *outstr, int nst, Pstring_acc *a)
{
  int                    i, sz, len_sz, rp;
  Pfloat64               track_pcnt;
  Void_t                 *velt;
  PDCI_string_dt_elt_t   *elt;

  P_TRACE(pads->disc, "Pstring_acc_report2xml_io called");
  sz = dtsize(a->dict);
  len_sz = dtsize(a->len_accum.dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (len_sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<Pstring><none/></Pstring>\n");
    return P_OK;
  }
  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<Pstring>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<length>\n");
  if (P_ERR == Puint32_acc_report2xml_io(pads, outstr, nst+1, &(a->len_accum))) {
    return P_ERR;
  }
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "</length>\n");

  /* Apparently, string accumulators don't have good and bad fields; all are good? 
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%llu", a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%llu", a->bad);
  sfprintf(outstr, "</bad>\n"); */

  /* rehash tree to get keys ordered by count */
  dtdisc(a->dict, &PDCI_string_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);

     if (sz == a->max2track && a->len_accum.good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->len_accum.good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     }
     for (i = 0, velt = dtfirst(a->dict);
          velt && i < a->max2rep;
          velt = dtnext(a->dict, velt), i++) {
            elt = (PDCI_string_dt_elt_t*)velt;

            PDCI_indent(outstr, nst);
            sfprintf(outstr, "<item>");
            sfprintf(outstr, "<val>%s", P_qfmt_cstr_n(elt->key.str, elt->key.len));
            sfprintf(outstr, "</val>");
            sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
            sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</Pstring>\n");

  dtnext(a->dict, 0); /* discard any iterator state */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict, &PDCI_string_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
Pstring_acc_report(P_t *pads, const char *prefix, const char *what, int nst, Pstring_acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS("Pstring_acc_report", a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = Pstring_acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}

Perror_t Pip_acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix,
			   const char *what, int nst, Puint32_acc *a)
{
  int                i, sz, rp;
  Puint64            cnt_sum;
  Pfloat64           bad_pcnt;
  Pfloat64           cnt_sum_pcnt;
  Pfloat64           track_pcnt;
  Pfloat64           elt_pcnt;
  Void_t            *velt;
  Puint32_dt_elt_t  *elt;
  const char        *xtra;

  P_TRACE(pads->disc, "Pip_acc_report2io called");
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = "Pip";
  }
  xtra = "\
    ** N.B. IPs are reported using the Puint32 internal rep, and are also pretty-printed using the default Pip output format";
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, xtra);
  Puint32_acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (Puint32_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %10lu (%s)\n",
	     a->good, what, elt->key.val,
	     fmtip4(elt->key.val, -1));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (Puint32_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %10lu (%s)\n",
	     what, elt->key.val,
	     fmtip4(elt->key.val, -1));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &Puint32_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "  Characterizing %s values:  min %llu (%s)", what, a->min, fmtip4(a->min, -1));
  sfprintf(outstr, " max %llu (%s)", a->max, fmtip4(a->max, -1));
  {
    Puint32 rounded_down = (Puint32)a->avg;
    sfprintf(outstr, " avg rounded down to nearest Puint32: %lu (%s)\n", rounded_down, fmtip4(rounded_down, -1));
  }

  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (Puint32_dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    sfprintf(outstr, "        val: %10lu (%s)", elt->key.val, fmtip4(elt->key.val, -1));
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
  sfprintf(outstr,   "        SUMMING         count: %10llu  pcnt-of-good-vals: %8.3I8f\n",
	   cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &Puint32_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}


Perror_t Pip_acc_report2xml_io(P_t *pads, Sfio_t *outstr, int nst, Puint32_acc *a)
{
  int                i, sz, rp;
  Pfloat64           track_pcnt;
  Void_t            *velt;
  Puint32_dt_elt_t  *elt;

  P_TRACE(pads->disc, "Pip_acc_report2xml_io called");
  Puint32_acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<Pip><none/></Pip>\n");
    return P_OK;
  }

  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<Pip>\n");

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%llu", a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%llu", a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &Puint32_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%s", fmtip4(a->min, -1));
  sfprintf(outstr, "</min>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%s", fmtip4(a->max, -1));
  sfprintf(outstr, "</max>\n");

  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);

     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     }
     for (i = 0, velt = dtfirst(a->dict);
	  velt && i < a->max2rep;
	  velt = dtnext(a->dict, velt), i++) {
            elt = (Puint32_dt_elt_t*)velt;
            PDCI_indent(outstr, nst);
            sfprintf(outstr, "<item>");
            sfprintf(outstr, "<val>%s", fmtip4(elt->key.val,-1));
            sfprintf(outstr, "</val>");
            sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
            sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</Pip>\n");

  dtnext(a->dict, 0); /* discard any iterator state */
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &Puint32_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t Pip_acc_report(P_t *pads, const char *prefix, const char *what,
			int nst, Puint32_acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS("Pip_acc_report", a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = Pip_acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}


Perror_t
PDCI_date_time_acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix,
			     const char *what, int nst, Puint32_acc *a,
			     const char *whatfn, const char *def_what,
			     const char *format, const char *format_descr,
			     Tm_zone_t *tzone, const char *tzone_descr)
{
  int                i, sz, rp;
  Puint64            cnt_sum;
  Pfloat64           bad_pcnt;
  Pfloat64           cnt_sum_pcnt;
  Pfloat64           track_pcnt;
  Pfloat64           elt_pcnt;
  Void_t            *velt;
  Puint32_dt_elt_t  *elt;
  const char        *xtra;

  P_TRACE2(pads->disc, "PDCI_date_time_acc_report2io called, whatfn = %s, out format = %s",
	   whatfn, format);
  tmset(tzone);
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = def_what;
  }
  if (strcmp(def_what, "time") == 0) {
    xtra = "\
    ** N.B. times are reported in # of seconds, and also as formatted strings (using the default time output format)";
  } else if (strcmp(def_what, "date") == 0) {
    xtra = "\
    ** N.B. dates are reported in # of seconds since midnight 1-jan-1970, and also as formatted strings (using the default date output format)";
  } else {
    xtra = "\
    ** N.B. timestamps are reported in # of seconds since midnight 1-jan-1970, and also as formatted strings (using the default timestamp output format)";
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, xtra);
  Puint32_acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (Puint32_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %10lu (%s)\n",
	     a->good, what, elt->key.val,
	     fmttime(format, (time_t)elt->key.val));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (Puint32_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %10lu (%s)\n",
	     what, elt->key.val,
	     fmttime(format, (time_t)elt->key.val));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &Puint32_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "  Characterizing %s values:  min %llu (%s)", what, a->min, fmttime(format, (time_t)a->min));
  sfprintf(outstr, " max %llu (%s)", a->max, fmttime(format, (time_t)a->max));
  {
    Puint32 rounded_down = (Puint32)a->avg;
    sfprintf(outstr, " avg rounded down to nearest Puint32: %lu (%s)\n", rounded_down, fmttime(format, (time_t)rounded_down));
  }
  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (Puint32_dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    sfprintf(outstr, "        val: %10lu (%s)", elt->key.val, fmttime(format, (time_t)elt->key.val));
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
  sfprintf(outstr,   "        SUMMING         count: %10llu  pcnt-of-good-vals: %8.3I8f\n",
	   cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &Puint32_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}


Perror_t
PDCI_date_time_acc_report2xml_io(P_t *pads, Sfio_t *outstr, const char *src_type,
			         int nst, Puint32_acc *a, const char *whatfn, 
				 const char *format, Tm_zone_t *tzone)
{
  int                i, sz, rp;
  Pfloat64           track_pcnt;
  Void_t            *velt;
  Puint32_dt_elt_t  *elt;

  P_TRACE2(pads->disc, "PDCI_date_time_acc_report2xml_io called, whatfn = %s, out format = %s",
	   whatfn, format);
  tmset(tzone);
  Puint32_acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<%s><none/></%s>\n", src_type, src_type);
    return P_OK;
  }

  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<%s>\n", src_type);

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%llu", a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%llu", a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &Puint32_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  {
    Puint32 rounded_down = (Puint32) a->avg;
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<avg>%s</avg>\n", fmttime(format, (time_t)rounded_down));
  }
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%s", fmttime(format, (time_t)a->min));
  sfprintf(outstr, "</min>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%s", fmttime(format, (time_t)a->max));
  sfprintf(outstr, "</max>\n");

  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);

     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     }
     for (i = 0, velt = dtfirst(a->dict);
	  velt && i < a->max2rep;
	  velt = dtnext(a->dict, velt), i++) {
            elt = (Puint32_dt_elt_t*)velt;
	    PDCI_indent(outstr, nst);
	    sfprintf(outstr, "<item>");
	    sfprintf(outstr, "<val>%s", fmttime(format, (time_t)elt->key.val));
	    sfprintf(outstr, "</val>");
	    sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
	    sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</%s>\n", src_type);

  dtnext(a->dict, 0); /* discard any iterator state */
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &Puint32_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
PDCI_date_time_acc_report(P_t *pads, const char *prefix, const char *what, int nst, Puint32_acc *a,
			  const char *whatfn, const char *def_what,
			  const char *format, const char *format_descr,
			  Tm_zone_t *tzone, const char *tzone_descr)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_1P_CHECKS(whatfn, a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = PDCI_date_time_acc_report2io(pads, tmpstr, prefix, what, nst, a,
				     whatfn, def_what,
				     format, format_descr,
				     tzone, tzone_descr);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}


Perror_t
Pchar_acc_init(P_t *pads, Pchar_acc *a)
{
  return Puint8_acc_init(pads, a);
}

Perror_t
Pchar_acc_reset(P_t *pads, Pchar_acc *a)
{
  return Puint8_acc_reset(pads, a);
}

Perror_t
Pchar_acc_cleanup(P_t *pads, Pchar_acc *a)
{
  return Puint8_acc_cleanup(pads, a);
}

Perror_t
Pchar_acc_add(P_t *pads, Pchar_acc *a, const Pbase_pd *pd, const Puint8 *val)
{
  return Puint8_acc_add(pads, a, pd, val);
}

Perror_t
Pchar_acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what, int nst,
		       Pchar_acc *a)
{
  int                   i, sz, rp;
  Puint64            cnt_sum;
  Pfloat64              cnt_sum_pcnt;
  Pfloat64              bad_pcnt;
  Pfloat64              track_pcnt;
  Pfloat64              elt_pcnt;
  Void_t                *velt;
  Puint8_dt_elt_t    *elt;

  P_TRACE(pads->disc, "Pchar_acc_report2io called");
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = "char";
  }
  PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0);
  Puint8_acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    sfprintf(outstr, "(No %s values.)\n", what);
    return P_OK;
  }
  if (sz == 1 && a->bad == 0) {
    elt = (Puint8_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "%llu %s values, 100 pcnt good, 100 pcnt identical: %s (as ASCII)\n",
	     a->good, what, P_qfmt_char(elt->key.val));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  if (a->good == 0) {
    bad_pcnt = (a->bad == 0) ? 0.0 : 100.0;
  } else {
    bad_pcnt = 100.0 * (a->bad / (Pfloat64)(a->good + a->bad));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   a->good, a->bad, bad_pcnt);
  if (a->good == 0) {
    sfprintf(outstr, "(No good %s values.)\n", what);
    return P_OK;
  }
  /* check for 100% identical values */
  if (sz == 1) {
    elt = (Puint8_dt_elt_t*)dtfirst(a->dict);
    sfprintf(outstr, "For good %s values, 100 pcnt identical: %s (as ASCII)\n",
	     what, P_qfmt_char(elt->key.val));
    dtnext(a->dict, 0); /* discard any iterator state */
    return P_OK;
  }
  dtdisc(a->dict,   &Puint8_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
  sfprintf(outstr, "  Characterizing %s values:  min %s", what, P_qfmt_char(a->min));
  sfprintf(outstr, " max %s", P_qfmt_char(a->max));
  sfprintf(outstr, " (based on ASCII encoding)\n");

  sfprintf(outstr, "    => distribution of top %d values out of %d distinct values:\n", rp, sz);
  if (sz == a->max2track && a->good > a->tracked) {
    track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
    sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all values *) \n", track_pcnt);
  }
  for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
       velt && i < a->max2rep;
       velt = dtnext(a->dict, velt), i++) {
    if (cnt_sum_pcnt >= a->pcnt2rep) {
      sfprintf(outstr, " [... %d of top %d values not reported due to %.2I8f pcnt limit on reported values ...]\n",
	       rp-i, rp, a->pcnt2rep);
      break;
    }
    elt = (Puint8_dt_elt_t*)velt;
    elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
    sfprintf(outstr, "        val: %6s", P_qfmt_char(elt->key.val));
    sfprintf(outstr, " count: %10llu  pcnt-of-good-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
    cnt_sum += elt->key.cnt;
    cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
  }
  dtnext(a->dict, 0); /* discard any iterator state */
  sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
  sfprintf(outstr,   "        SUMMING     count: %10llu  pcnt-of-good-vals: %8.3I8f\n",
	   cnt_sum, cnt_sum_pcnt);
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &Puint8_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}


Perror_t
Pchar_acc_report2xml_io(P_t *pads, Sfio_t *outstr, int nst, Pchar_acc *a)
{
  int                   i, sz, rp;
  Pfloat64              track_pcnt;
  Void_t                *velt;
  Puint8_dt_elt_t    *elt;

  P_TRACE(pads->disc, "Pchar_acc_report2xml_io called");
  Puint8_acc_fold_psum(a);
  sz = dtsize(a->dict);
  rp = (sz < a->max2rep) ? sz : a->max2rep;
  if (sz == 0) { /* no values accumulated */
    PDCI_indent(outstr, nst);
    sfprintf(outstr, "<Pchar><none/></Pchar>\n");
    return P_OK;
  }

  PDCI_indent(outstr, nst++);
  sfprintf(outstr, "<Pchar>\n");

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<good>%llu", a->good);
  sfprintf(outstr, "</good>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<bad>%llu", a->bad);
  sfprintf(outstr, "</bad>\n");

  dtdisc(a->dict,   &Puint8_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
  dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */

  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<min>%s", P_qfmt_char(a->min));
  sfprintf(outstr, "</min>\n");
  PDCI_indent(outstr, nst);
  sfprintf(outstr, "<max>%s", P_qfmt_char(a->max));
  sfprintf(outstr, "</max>\n");

  if (sz!=0){
     PDCI_indent(outstr, nst++);
     sfprintf(outstr, "<distribution>\n");
     PDCI_indent(outstr, nst);
     sfprintf(outstr, "<top>%d</top>", rp);
     sfprintf(outstr, "<distinct>%d</distinct>\n", sz);

     if (sz == a->max2track && a->good > a->tracked) {
       track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
       PDCI_indent(outstr, nst);
       sfprintf(outstr, "<tracked>%.3I8f</tracked>\n", track_pcnt);
     }
     for (i = 0, velt = dtfirst(a->dict);
          velt && i < a->max2rep;
          velt = dtnext(a->dict, velt), i++) {
           elt = (Puint8_dt_elt_t*)velt;
            PDCI_indent(outstr, nst);
            sfprintf(outstr, "<item>");
            sfprintf(outstr, "<val>%s", P_qfmt_char(elt->key.val));
            sfprintf(outstr, "</val>");
            sfprintf(outstr, "<count>%llu</count>", elt->key.cnt);
            sfprintf(outstr, "</item>\n");
     }
     PDCI_indent(outstr, --nst);
     sfprintf(outstr, "</distribution>\n");
  }
  PDCI_indent(outstr, --nst);
  sfprintf(outstr, "</Pchar>\n");

  dtnext(a->dict, 0); /* discard any iterator state */
  /* revert to unordered set in case more inserts will occur after this report */
  dtmethod(a->dict, Dtset); /* change to unordered set */
  dtdisc(a->dict,   &Puint8_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  return P_OK;
}

Perror_t
Pchar_acc_report(P_t *pads, const char *prefix, const char *what, int nst,
		 Pchar_acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS("Pchar_acc_report", a);
  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = Pchar_acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}

#endif /* P_CONFIG_ACCUM_FUNCTIONS */

Perror_t
P_nerr_acc_report2io(P_t *pads, Sfio_t *outstr, const char *prefix, const char *what, int nst,
		     Puint32_acc *a)
{
  int                i, sz, rp;
  Puint64            ngood, nbad;
  Puint64            cnt_sum;
  Pfloat64           bad_pcnt;
  Pfloat64           cnt_sum_pcnt;
  Pfloat64           track_pcnt;
  Pfloat64           elt_pcnt;
  Void_t            *velt;
  Puint32_dt_elt_t  *elt;
  Puint32_dt_key_t   lookup_key;

  P_TRACE(pads->disc, "P_nerr_acc_report2io called");
  if (!prefix || *prefix == 0) {
    prefix = "<top>";
  }
  if (!what) {
    what = "nerr";
  }
  /* XXX_REMOVE PDCI_nst_prefix_what(outstr, &nst, prefix, what, 0); */
#ifndef NDEBUG
  if (a->bad) {
    P_WARN(pads->disc, "** UNEXPECTED: P_nerr_acc_report called with bad values (all nerr are valid).  Ignoring bad.");
  }
#endif
  if (a->good == 0) {
    /* sfprintf(outstr, "good vals: %10u    bad vals: %10u    pcnt-bad: %8.3f\n", 0, 0, 0.0); XXX do not report anything */
    return P_OK;
  }
  Puint32_acc_fold_psum(a);
  lookup_key.val = 0;
  lookup_key.cnt = 0;
  if (!(elt = dtmatch(a->dict, &lookup_key))) {
    /* there were no good values */
    ngood = 0;
    nbad = a->good;
    bad_pcnt = 100.0; 
  } else {
    ngood = elt->key.cnt;
    nbad = a->good - ngood;
    bad_pcnt = 100.0 * (nbad / (Pfloat64)(a->good));
  }
  sfprintf(outstr, "good vals: %10llu    bad vals: %10llu    pcnt-bad: %8.3I8f\n",
	   ngood, nbad, bad_pcnt);
  if (nbad) {
    sz = dtsize(a->dict);
    rp = (sz < a->max2rep) ? sz : a->max2rep;
    dtdisc(a->dict,   &Puint32_acc_dt_oset_disc, DT_SAMEHASH); /* change cmp function */
    dtmethod(a->dict, Dtoset); /* change to ordered set -- establishes an ordering */
    sfprintf(outstr, "  Characterizing number of errors PER READ CALL (nerr-per-read) :");
    sfprintf(outstr, " min %lld", a->min);
    sfprintf(outstr, " max %lld", a->max);
    sfprintf(outstr, " avg %.3I8f\n", a->avg);
    sfprintf(outstr, "    => distribution of top %d nerr-per-read values out of %d distinct nerr-per-read values:\n", rp, sz);
    if (sz == a->max2track && a->good > a->tracked) {
      track_pcnt = 100.0 * (a->tracked/(Pfloat64)a->good);
      sfprintf(outstr, "        (* hit tracking limit, tracked %.3I8f pcnt of all nerr-per-read values *) \n", track_pcnt);
    }
    for (i = 0, cnt_sum = 0, cnt_sum_pcnt = 0, velt = dtfirst(a->dict);
	 velt && i < a->max2rep;
	 velt = dtnext(a->dict, velt), i++) {
      if (cnt_sum_pcnt >= a->pcnt2rep) {
	sfprintf(outstr, " [... %d of top %d nerr-per-read values not reported due to %.2I8f pcnt limit on reported values ...]\n",
		 rp-i, rp, a->pcnt2rep);
	break;
      }
      elt = (Puint32_dt_elt_t*)velt;
      elt_pcnt = 100.0 * (elt->key.cnt/(Pfloat64)a->good);
      sfprintf(outstr, "        val: %10ld", elt->key.val);
      sfprintf(outstr, " count: %10llu pcnt-of-total-vals: %8.3I8f\n", elt->key.cnt, elt_pcnt);
      cnt_sum += elt->key.cnt;
      cnt_sum_pcnt = 100.0 * (cnt_sum/(Pfloat64)a->good);
    }
    dtnext(a->dict, 0); /* discard any iterator state */
    sfprintf(outstr,   ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .\n");
    sfprintf(outstr,   "        SUMMING         count: %10llu pcnt-of-total-vals: %8.3I8f\n",
	     cnt_sum, cnt_sum_pcnt);
    /* revert to unordered set in case more inserts will occur after this report */
    dtmethod(a->dict, Dtset); /* change to unordered set */
    dtdisc(a->dict,   &Puint32_acc_dt_set_disc, DT_SAMEHASH); /* change cmp function */
  }
  return P_OK;
}

Perror_t
P_nerr_acc_report(P_t *pads, const char *prefix, const char *what, int nst,
		    Puint32_acc *a)
{
  Sfio_t *tmpstr;
  Perror_t res;
  PDCI_DISC_1P_CHECKS("P_nerr_acc_report", a);

  if (!pads->disc->error_fn) {
    return P_OK;
  }
  if (!(tmpstr = sfstropen ())) { 
    return P_ERR;
  }
  res = P_nerr_acc_report2io(pads, tmpstr, prefix, what, nst, a);
  if (res == P_OK) {
    pads->disc->error_fn(NiL, 0, "%s", PDCI_sfstr_use(tmpstr));
  }
  sfstrclose (tmpstr);
  return res;
}

/* ACCUM IMPL HELPERS */

static const char *PDCI_hdr_strings[] = {
  "*****************************************************************************************************\n",
  "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",
  "=====================================================================================================\n",
  "-----------------------------------------------------------------------------------------------------\n",
  ".....................................................................................................\n",
  "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n",
  "+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +\n",
  "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n",
  "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
};

void 
PDCI_indent(Sfio_t *outstr, int nst)
{
  int i;
  for (i=0; i<nst; i++){
    sfprintf(outstr, " ");
  };
};

void
PDCI_nst_prefix_what(Sfio_t *outstr, int *nst, const char *prefix, const char *what, const char *xtra)
{
  if (prefix) {
    if ((*nst) >= 0) {
      int idx = (*nst) % 9;
      sfprintf(outstr, "\n%s", PDCI_hdr_strings[idx]);
      sfprintf(outstr, "%s : %s\n", prefix, what);
      if (xtra) {
	sfprintf(outstr, "%s\n", xtra);
      }
      sfprintf(outstr, "%s", PDCI_hdr_strings[idx]);
      (*nst)++;
    } else {
      sfprintf(outstr, "%s: ", prefix);
    }
  }
}

/* ********************************** END_MACGEN ********************************** */
/* ********************** BEGIN_MACGEN(pads-misc-gen.c) *********************** */
/*
 * Generated misc functions
 * 
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-internal.h"
#gen_include "pads-macros-gen.h"

/* ================================================================================ */
/* USEFUL CONVERSION CONSTANTS */

#define P_MIN_INT8_DIV10                       -12
#define P_MIN_INT8_DIV100                       -1

#define P_MAX_UINT8_DIV10                      25U
#define P_MAX_UINT8_DIV100                      2U

#define P_MIN_INT16_DIV10                    -3276
#define P_MIN_INT16_DIV100                    -327

#define P_MAX_UINT16_DIV10                   6553U
#define P_MAX_UINT16_DIV100                   655U

#define P_MIN_INT32_DIV10              -214748364L
#define P_MIN_INT32_DIV100              -21474836L

#define P_MAX_UINT32_DIV10             429496729UL
#define P_MAX_UINT32_DIV100             42949672UL

#define P_MIN_INT64_DIV10    -922337203685477580LL
#define P_MIN_INT64_DIV100    -92233720368547758LL

#define P_MAX_UINT64_DIV10  1844674407370955161ULL
#define P_MAX_UINT64_DIV100  184467440737095516ULL

static Pint64 P_MIN_FOR_NB[] = {
  0,
  P_MIN_INT8,
  P_MIN_INT16,
  P_MIN_INT24,
  P_MIN_INT32,
  P_MIN_INT40,
  P_MIN_INT48,
  P_MIN_INT56,
  P_MIN_INT64
};

static Pint64 P_MAX_FOR_NB[] = {
  0,
  P_MAX_INT8,
  P_MAX_INT16,
  P_MAX_INT24,
  P_MAX_INT32,
  P_MAX_INT40,
  P_MAX_INT48,
  P_MAX_INT56,
  P_MAX_INT64
};

static Puint64 P_UMAX_FOR_NB[] = {
  0,
  P_MAX_UINT8,
  P_MAX_UINT16,
  P_MAX_UINT24,
  P_MAX_UINT32,
  P_MAX_UINT40,
  P_MAX_UINT48,
  P_MAX_UINT56,
  P_MAX_UINT64
};

/* ********************************** END_HEADER ********************************** */
#gen_include "pads-misc-macros-gen.h"


/* PDCI_A2INT(fn_name, targ_type, int_min, int_max, Pstring2int_fn) */
PDCI_A2INT(PDCI_a2int8,  Pint8,  P_MIN_INT8,  P_MAX_INT8,  Pstring2int8)
PDCI_A2INT(PDCI_a2int16, Pint16, P_MIN_INT16, P_MAX_INT16, Pstring2int16)
PDCI_A2INT(PDCI_a2int32, Pint32, P_MIN_INT32, P_MAX_INT32, Pstring2int32)
PDCI_A2INT(PDCI_a2int64, Pint64, P_MIN_INT64, P_MAX_INT64, Pstring2int64)

/* PDCI_A2UINT(fn_name, targ_type, int_max, Pstring2int_fn) */
PDCI_A2UINT(PDCI_a2uint8,  Puint8,  P_MAX_UINT8,  Pstring2uint8)
PDCI_A2UINT(PDCI_a2uint16, Puint16, P_MAX_UINT16, Pstring2uint16)
PDCI_A2UINT(PDCI_a2uint32, Puint32, P_MAX_UINT32, Pstring2uint32)
PDCI_A2UINT(PDCI_a2uint64, Puint64, P_MAX_UINT64, Pstring2uint64)

/* PDCI_INT2A(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) */
PDCI_INT2A(PDCI_int8_2a,   Pint8,   "%I1d", "%0.*I1d", PDCI_FMT_INT1_WRITE, PDCI_WFMT_INT1_WRITE)
PDCI_INT2A(PDCI_int16_2a,  Pint16,  "%I2d", "%0.*I2d", PDCI_FMT_INT_WRITE,  PDCI_WFMT_INT_WRITE)
PDCI_INT2A(PDCI_int32_2a,  Pint32,  "%I4d", "%0.*I4d", PDCI_FMT_INT_WRITE,  PDCI_WFMT_INT_WRITE)
PDCI_INT2A(PDCI_int64_2a,  Pint64,  "%I8d", "%0.*I8d", PDCI_FMT_INT_WRITE,  PDCI_WFMT_INT_WRITE)

PDCI_INT2A(PDCI_uint8_2a,  Puint8,  "%I1u", "%0.*I1u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)
PDCI_INT2A(PDCI_uint16_2a, Puint16, "%I2u", "%0.*I2u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)
PDCI_INT2A(PDCI_uint32_2a, Puint32, "%I4u", "%0.*I4u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)
PDCI_INT2A(PDCI_uint64_2a, Puint64, "%I8u", "%0.*I8u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)

/* PDCI_E2INT(fn_name, targ_type, int_min, int_max) */
PDCI_E2INT(PDCI_e2int8,  Pint8,  P_MIN_INT8,  P_MAX_INT8)
PDCI_E2INT(PDCI_e2int16, Pint16, P_MIN_INT16, P_MAX_INT16)
PDCI_E2INT(PDCI_e2int32, Pint32, P_MIN_INT32, P_MAX_INT32)
PDCI_E2INT(PDCI_e2int64, Pint64, P_MIN_INT64, P_MAX_INT64)

/* PDCI_E2UINT(fn_name, targ_type, int_max) */
PDCI_E2UINT(PDCI_e2uint8,  Puint8,  P_MAX_UINT8)
PDCI_E2UINT(PDCI_e2uint16, Puint16, P_MAX_UINT16)
PDCI_E2UINT(PDCI_e2uint32, Puint32, P_MAX_UINT32)
PDCI_E2UINT(PDCI_e2uint64, Puint64, P_MAX_UINT64)

/* PDCI_INT2E(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) */
PDCI_INT2E(PDCI_int8_2e,   Pint8,   "%I1d", "%0.*I1d", PDCI_FMT_INT1_WRITE, PDCI_WFMT_INT1_WRITE)
PDCI_INT2E(PDCI_int16_2e,  Pint16,  "%I2d", "%0.*I2d", PDCI_FMT_INT_WRITE,  PDCI_WFMT_INT_WRITE)
PDCI_INT2E(PDCI_int32_2e,  Pint32,  "%I4d", "%0.*I4d", PDCI_FMT_INT_WRITE,  PDCI_WFMT_INT_WRITE)
PDCI_INT2E(PDCI_int64_2e,  Pint64,  "%I8d", "%0.*I8d", PDCI_FMT_INT_WRITE,  PDCI_WFMT_INT_WRITE)

PDCI_INT2E(PDCI_uint8_2e,  Puint8,  "%I1u", "%0.*I1u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)
PDCI_INT2E(PDCI_uint16_2e, Puint16, "%I2u", "%0.*I2u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)
PDCI_INT2E(PDCI_uint32_2e, Puint32, "%I4u", "%0.*I4u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)
PDCI_INT2E(PDCI_uint64_2e, Puint64, "%I8u", "%0.*I8u", PDCI_FMT_UINT_WRITE, PDCI_WFMT_UINT_WRITE)

/* PDCI_INT2B(rev_fn_name, targ_type, num_bytes) */
PDCI_INT1_2B(PDCI_int8_2b,   Pint8)
PDCI_INT2B  (PDCI_int16_2b,  Pint16,  2)
PDCI_INT2B  (PDCI_int32_2b,  Pint32,  4)
PDCI_INT2B  (PDCI_int64_2b,  Pint64,  8)
PDCI_INT1_2B(PDCI_uint8_2b,  Puint8)
PDCI_INT2B  (PDCI_uint16_2b, Puint16, 2)
PDCI_INT2B  (PDCI_uint32_2b, Puint32, 4)
PDCI_INT2B  (PDCI_uint64_2b, Puint64, 8)

/* PDCI_EBC2INT(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max) */
PDCI_EBC2INT(PDCI_ebc2int8,  PDCI_int8_2ebc,  Pint8,  Pint16,  P_MIN_INT8,  P_MAX_INT8,   3, 3)
PDCI_EBC2INT(PDCI_ebc2int16, PDCI_int16_2ebc, Pint16, Pint32, P_MIN_INT16, P_MAX_INT16,  5, 5)
PDCI_EBC2INT(PDCI_ebc2int32, PDCI_int32_2ebc, Pint32, Pint64, P_MIN_INT32, P_MAX_INT32, 10, 10)
PDCI_EBC2INT(PDCI_ebc2int64, PDCI_int64_2ebc, Pint64, Pint64, P_MIN_INT64, P_MAX_INT64, 19, 19)

/* PDCI_EBC2UINT(fn_name, rev_fn_name, targ_type, int_max, nd_max) */
PDCI_EBC2UINT(PDCI_ebc2uint8,  PDCI_uint8_2ebc,  Puint8,  P_MAX_UINT8,   3)
PDCI_EBC2UINT(PDCI_ebc2uint16, PDCI_uint16_2ebc, Puint16, P_MAX_UINT16,  5)
PDCI_EBC2UINT(PDCI_ebc2uint32, PDCI_uint32_2ebc, Puint32, P_MAX_UINT32, 10)
PDCI_EBC2UINT(PDCI_ebc2uint64, PDCI_uint64_2ebc, Puint64, P_MAX_UINT64, 20)

/* PDCI_BCD2INT(fn_name, rev_fn_name, targ_type, higher_targ_type, int_min, int_max, nd_max, act_nd_max) */
PDCI_BCD2INT(PDCI_bcd2int8,  PDCI_int8_2bcd,  Pint8,  Pint16, P_MIN_INT8,  P_MAX_INT8,   3, 3)
PDCI_BCD2INT(PDCI_bcd2int16, PDCI_int16_2bcd, Pint16, Pint32, P_MIN_INT16, P_MAX_INT16,  5, 5)
PDCI_BCD2INT(PDCI_bcd2int32, PDCI_int32_2bcd, Pint32, Pint64, P_MIN_INT32, P_MAX_INT32, 11, 10)
PDCI_BCD2INT(PDCI_bcd2int64, PDCI_int64_2bcd, Pint64, Pint64, P_MIN_INT64, P_MAX_INT64, 19, 19)

/* PDCI_BCD2UINT(fn_name, rev_fn_name, targ_type, int_max, nd_max) */
PDCI_BCD2UINT(PDCI_bcd2uint8,  PDCI_uint8_2bcd,  Puint8,  P_MAX_UINT8,   3)
PDCI_BCD2UINT(PDCI_bcd2uint16, PDCI_uint16_2bcd, Puint16, P_MAX_UINT16,  5)
PDCI_BCD2UINT(PDCI_bcd2uint32, PDCI_uint32_2bcd, Puint32, P_MAX_UINT32, 10)
PDCI_BCD2UINT(PDCI_bcd2uint64, PDCI_uint64_2bcd, Puint64, P_MAX_UINT64, 20)

/* PDCI_SBL2INT(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max) */
PDCI_SBL2INT(PDCI_sbl2int8,  PDCI_int8_2sbl,  Pint8,  PlittleEndian, P_MIN_INT8,  P_MAX_INT8,  1)
PDCI_SBL2INT(PDCI_sbl2int16, PDCI_int16_2sbl, Pint16, PlittleEndian, P_MIN_INT16, P_MAX_INT16, 2)
PDCI_SBL2INT(PDCI_sbl2int32, PDCI_int32_2sbl, Pint32, PlittleEndian, P_MIN_INT32, P_MAX_INT32, 4)
PDCI_SBL2INT(PDCI_sbl2int64, PDCI_int64_2sbl, Pint64, PlittleEndian, P_MIN_INT64, P_MAX_INT64, 8)

/* PDCI_SBL2UINT(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max) */
PDCI_SBL2UINT(PDCI_sbl2uint8,  PDCI_uint8_2sbl,  Puint8,  PlittleEndian, P_MAX_UINT8,  1)
PDCI_SBL2UINT(PDCI_sbl2uint16, PDCI_uint16_2sbl, Puint16, PlittleEndian, P_MAX_UINT16, 2)
PDCI_SBL2UINT(PDCI_sbl2uint32, PDCI_uint32_2sbl, Puint32, PlittleEndian, P_MAX_UINT32, 4)
PDCI_SBL2UINT(PDCI_sbl2uint64, PDCI_uint64_2sbl, Puint64, PlittleEndian, P_MAX_UINT64, 8)

/* PDCI_SBH2INT(fn_name, rev_fn_name, targ_type, sb_endian, int_min, int_max, nb_max) */
PDCI_SBH2INT(PDCI_sbh2int8,  PDCI_int8_2sbh,  Pint8,  PbigEndian, P_MIN_INT8,  P_MAX_INT8,  1)
PDCI_SBH2INT(PDCI_sbh2int16, PDCI_int16_2sbh, Pint16, PbigEndian, P_MIN_INT16, P_MAX_INT16, 2)
PDCI_SBH2INT(PDCI_sbh2int32, PDCI_int32_2sbh, Pint32, PbigEndian, P_MIN_INT32, P_MAX_INT32, 4)
PDCI_SBH2INT(PDCI_sbh2int64, PDCI_int64_2sbh, Pint64, PbigEndian, P_MIN_INT64, P_MAX_INT64, 8)

/* PDCI_SBH2UINT(fn_name, rev_fn_name, targ_type, sb_endian, int_max, nb_max) */
PDCI_SBH2UINT(PDCI_sbh2uint8,  PDCI_uint8_2sbh,  Puint8,  PbigEndian, P_MAX_UINT8,  1)
PDCI_SBH2UINT(PDCI_sbh2uint16, PDCI_uint16_2sbh, Puint16, PbigEndian, P_MAX_UINT16, 2)
PDCI_SBH2UINT(PDCI_sbh2uint32, PDCI_uint32_2sbh, Puint32, PbigEndian, P_MAX_UINT32, 4)
PDCI_SBH2UINT(PDCI_sbh2uint64, PDCI_uint64_2sbh, Puint64, PbigEndian, P_MAX_UINT64, 8)

/* PDCI_FLOAT2A(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) */
PDCI_FLOAT2A(PDCI_float32_2a,  Pfloat32,  "%I4f", "%0.*I4f", PDCI_FMT_FLOAT_WRITE,  PDCI_WFMT_FLOAT_WRITE)
PDCI_FLOAT2A(PDCI_float64_2a,  Pfloat64,  "%I8f", "%0.*I8f", PDCI_FMT_FLOAT_WRITE,  PDCI_WFMT_FLOAT_WRITE)

/* PDCI_A2FLOAT(fn_name, targ_type, float_min, float_max, Pstring2float_fn) */
PDCI_A2FLOAT(PDCI_a2float32, Pfloat32, P_MIN_FLOAT32, P_MAX_FLOAT32, Pstring2float32)
PDCI_A2FLOAT(PDCI_a2float64, Pfloat64, P_MIN_FLOAT64, P_MAX_FLOAT64, Pstring2float64)

/* PDCI_FLOAT2E(rev_fn_name, targ_type, fmt, wfmt, sfpr_macro, sfpr_macro_w) */
PDCI_FLOAT2E(PDCI_float32_2e,  Pfloat32,  "%I4f", "%0.*I4f", PDCI_FMT_FLOAT_WRITE,  PDCI_WFMT_FLOAT_WRITE)
PDCI_FLOAT2E(PDCI_float64_2e,  Pfloat64,  "%I8f", "%0.*I8f", PDCI_FMT_FLOAT_WRITE,  PDCI_WFMT_FLOAT_WRITE)

/* PDCI_E2FLOAT(fn_name, targ_type, float_min, float_max) */
PDCI_E2FLOAT(PDCI_e2float32, Pfloat32, P_MIN_FLOAT32, P_MAX_FLOAT32)
PDCI_E2FLOAT(PDCI_e2float64, Pfloat64, P_MIN_FLOAT64, P_MAX_FLOAT64)

/* ********************************* BEGIN_TRAILER ******************************** */
/* ********************************** END_MACGEN ********************************** */

/* DEFGEN(pads-gen.c) */
/*
 * library routines for library that goes with padsc
 *
 * Kathleen Fisher, Robert Gruber
 * AT&T Labs Research
 */

#gen_include "pads-internal.h"
#gen_include "pads-macros-gen.h"

static const char id[] = "\n@(#)$Id: pads.c,v 1.214 2007/12/16 07:42:17 kfisher Exp $\0\n";

static const char lib[] = "padsc";

/* ================================================================================ */ 
/* IMPL CONSTANTS */

#define PDCI_initStkElts      8

/* ================================================================================
 * ASCII CHAR TABLES
 */

/* Identity map */
int PDCI_identity_charmap[256] = {
  /* 0x0? */ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  /* 0x1? */ 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  /* 0x2? */ 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  /* 0x3? */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  /* 0x4? */ 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  /* 0x5? */ 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
  /* 0x6? */ 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
  /* 0x7? */ 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
  /* 0x8? */ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
  /* 0x9? */ 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
  /* 0xA? */ 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  /* 0xB? */ 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
  /* 0xC? */ 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
  /* 0xD? */ 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
  /* 0xE? */ 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
  /* 0xF? */ 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

/* ASCII digits are 0x3[0-9] */
int PDCI_ascii_digit[256] = {
  /* 0x0? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x1? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x2? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x3? */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
  /* 0x4? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x5? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x6? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x7? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x8? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x9? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xA? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xB? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xC? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xD? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xE? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xF? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

/* ASCII digits are 0x3[0-9] */
int PDCI_ascii_is_digit[256] = {
  /* 0x0? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x1? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x2? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x3? */  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  /* 0x4? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x5? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x6? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x7? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x8? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x9? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xA? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xB? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xC? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xD? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xE? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xF? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

/* ASCII spaces : 0x09:HT, 0x0A:LF, 0x0B:VT, 0x0C:FF, 0x0D:CR, 0x20:SP */
int PDCI_ascii_is_space[256] = {
  /* 0x0? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,
  /* 0x1? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x2? */  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x3? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x4? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x5? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x6? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x7? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x8? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x9? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xA? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xB? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xC? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xD? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xE? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xF? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

/* ================================================================================
 * EBCDIC CHAR TABLES : tables for EBCDIC char conversion
 *   -- from Andrew Hume (ng_ebcdic.c)
 *
 * ================================================================================ */

/* not-sign 0xac -> circumflex 0x5e */
/* non-spacing macron 0xaf -> tilde 0x7e */
Pbyte P_ea_tab[256] =
{
  /* 0x0? */ 0x00, 0x01, 0x02, 0x03, '?',  0x09, '?',  0x7f, '?',  '?',  '?',  0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  /* 0x1? */ 0x10, 0x11, 0x12, 0x13, '?',  '?',  0x08,'?',   0x18, 0x09, '?',  '?',  0x1c, 0x1d, 0x1e, 0x1f,
  /* 0x2? */ '?',  '?',  '?',  '?',  '?',  0x0a, 0x17, 0x1b, '?',  '?',  '?',  '?',  '?',  0x05, 0x06, 0x07,
  /* 0x3? */ '?',  '?',  0x16, '?',  '?',  '?',  '?',  0x04, '?',  '?',  '?',  '?',  0x14, 0x15, '?',  0x1a,
  /* 0x4? */ 0x20, '?',  '?',  '?',  '?',  '?',  '?', '?',   '?',  '?',  0x5b, 0x2e, 0x3c, 0x28, 0x2b, 0x21,
  /* 0x5? */ 0x26, '?',  '?',  '?',  '?',  '?',  '?', '?',   '?',  '?',  0x5d, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
  /* 0x6? */ 0x2d, 0x2f, '?',  '?',  '?',  '?',  '?', '?',   '?',  '?',  0x7c, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
  /* 0x7? */ '?',  '?',  '?',  '?',  '?',  '?',  '?', '?',   '?',  0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
  /* 0x8? */ '?',  0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, '?',  '?',  '?',  '?',  '?', '?',
  /* 0x9? */ '?',  0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, '?',  '?',  '?',  '?',  '?', '?',
  /* 0xA? */ '?',  0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, '?',  '?',  '?',  '?',  '?', '?',
  /* 0xB? */ '?',  '?',  '?',  '?',  '?',  '?',  '?', '?',   '?',  '?',  '?',  '?',  '?',  '?',  '?', '?',
  /* 0xC? */ 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, '?',  '?',  '?',  '?',  '?', '?',
  /* 0xD? */ 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, '?',  '?',  '?',  '?',  '?', '?',
  /* 0xE? */ 0x5c, '?',  0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, '?',  '?',  '?',  '?',  '?', '?',
  /* 0xF? */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, '?',  '?',  '?',  '?',  '?', '?',
};

Pbyte P_ae_tab[256] =
{
  /* 0x0? */ 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f, 0x16, 0x19, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
  /* 0x1? */ 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26, 0x18, '?',  0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f, 
  /* 0x2? */ 0x40, 0x4f, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d, 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61, 
  /* 0x3? */ 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f, 
  /* 0x4? */ 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 
  /* 0x5? */ 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0x4a, 0xe0, 0x5a, 0x5f, 0x6d, 
  /* 0x6? */ 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 
  /* 0x7? */ 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xc0, 0x6a, 0xd0, 0xa1, 0x07, 
  /* 0x8? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0x9? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0xA? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0xB? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0xC? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0xD? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0xE? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
  /* 0xF? */ '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',  
};

/* Modified versions of the above tables.
 * replaced '?' with 0xff
 *    '?' is a valid character value ['?' ASCII char, EBCDIC SUB (substitute) character]
 * would rather use 0xff which is unspecified
 */

/* Note that both 0x05 and 0x19 map to ASCII 0x09.
 * This results in one more ea mapping than ae mapping
 */
Pbyte P_mod_ea_tab[256] =
{
  /* 0x0? */ 0x00, 0x01, 0x02, 0x03, 0xff, 0x09, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  /* 0x1? */ 0x10, 0x11, 0x12, 0x13, 0xff, 0xff, 0x08, 0xff, 0x18, 0x09, 0xff, 0xff, 0x1c, 0x1d, 0x1e, 0x1f,
  /* 0x2? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0x0a, 0x17, 0x1b, 0xff, 0xff, 0xff, 0xff, 0xff, 0x05, 0x06, 0x07,
  /* 0x3? */ 0xff, 0xff, 0x16, 0xff, 0xff, 0xff, 0xff, 0x04, 0xff, 0xff, 0xff, 0xff, 0x14, 0x15, 0xff, 0x1a,
  /* 0x4? */ 0x20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5b, 0x2e, 0x3c, 0x28, 0x2b, 0x21,
  /* 0x5? */ 0x26, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5d, 0x24, 0x2a, 0x29, 0x3b, 0x5e,
  /* 0x6? */ 0x2d, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7c, 0x2c, 0x25, 0x5f, 0x3e, 0x3f,
  /* 0x7? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
  /* 0x8? */ 0xff, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0x9? */ 0xff, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0xA? */ 0xff, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0xB? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0xC? */ 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0xD? */ 0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0xE? */ 0x5c, 0xff, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  /* 0xF? */ 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

Pbyte P_mod_ae_tab[256] =
{
  /* 0x0? */ 0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f, 0x16, 0x19, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 
  /* 0x1? */ 0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26, 0x18, 0xff, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f, 
  /* 0x2? */ 0x40, 0x4f, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d, 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61, 
  /* 0x3? */ 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f, 
  /* 0x4? */ 0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 
  /* 0x5? */ 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0x4a, 0xe0, 0x5a, 0x5f, 0x6d, 
  /* 0x6? */ 0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 
  /* 0x7? */ 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xc0, 0x6a, 0xd0, 0xa1, 0x07, 
  /* 0x8? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0x9? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0xA? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0xB? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0xC? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0xD? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0xE? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  /* 0xF? */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

/* EBCDIC digits are 0xC[0-9], 0xD[0-9], 0XF[0-9] */
/* aka 192-201, 208-217, 240-249 */
int PDCI_ebcdic_digit[256] = {
  /* 0x0? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x1? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x2? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x3? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x4? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x5? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x6? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x7? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x8? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0x9? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xA? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xB? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xC? */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
  /* 0xD? */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
  /* 0xE? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xF? */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
};

/* EBCDIC digits are 0xC[0-9], 0xD[0-9], 0XF[0-9] */
int PDCI_ebcdic_is_digit[256] = {
  /* 0x0? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x1? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x2? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x3? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x4? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x5? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x6? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x7? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x8? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x9? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xA? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xB? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xC? */  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  /* 0xD? */  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
  /* 0xE? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xF? */  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
};

/* EBCDIC spaces : 0x05:HT, 0x0B:VT, 0x0C:FF, 0x0D:CR, 0x15:NL, 0x40:SP */
int PDCI_ebcdic_is_space[256] = {
  /* 0x0? */  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,
  /* 0x1? */  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x2? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x3? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x4? */  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x5? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x6? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x7? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x8? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x9? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xA? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xB? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xC? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xD? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xE? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xF? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

/* ================================================================================
 * BCD TABLES : tables for BCD conversion
 *     -- from Andrew Hume (ng_bcd.c)
 *
 * ================================================================================ */

int PDCI_bcd_hilo_digits[256] = {
  /* 0x0? */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
  /* 0x1? */ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, -1, -1, -1, -1, -1, -1,
  /* 0x2? */ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, -1, -1, -1, -1, -1, -1,
  /* 0x3? */ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, -1, -1, -1, -1, -1, -1,
  /* 0x4? */ 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, -1, -1, -1, -1, -1, -1,
  /* 0x5? */ 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, -1, -1, -1, -1, -1, -1,
  /* 0x6? */ 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, -1, -1, -1, -1, -1, -1,
  /* 0x7? */ 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, -1, -1, -1, -1, -1, -1,
  /* 0x8? */ 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, -1, -1, -1, -1, -1, -1,
  /* 0x9? */ 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, -1, -1, -1, -1, -1, -1,
  /* 0xA? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xB? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xC? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xD? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xE? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xF? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

#if 0 
/* HUME version */
int PDCI_bcd_hilo_digits[256] = {
  /* 0x0? */  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
  /* 0x1? */ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 10, 10, 10, 10, 10, 10,
  /* 0x2? */ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 20, 20, 20, 20, 20, 20,
  /* 0x3? */ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 30, 30, 30, 30, 30, 30,
  /* 0x4? */ 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 40, 40, 40, 40, 40, 40,
  /* 0x5? */ 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 50, 50, 50, 50, 50, 50,
  /* 0x6? */ 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 60, 60, 60, 60, 60,
  /* 0x7? */ 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 70, 70, 70, 70, 70, 70,
  /* 0x8? */ 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 80, 80, 80, 80, 80, 80,
  /* 0x9? */ 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 90, 90, 90, 90, 90, 90,
  /* 0xA? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xB? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xC? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xD? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xE? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xF? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
#endif

int PDCI_bcd_hi_digit[256] = {
  /* 0x0? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x1? */  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* 0x2? */  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  /* 0x3? */  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
  /* 0x4? */  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
  /* 0x5? */  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
  /* 0x6? */  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
  /* 0x7? */  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
  /* 0x8? */  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
  /* 0x9? */  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
  /* 0xA? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xB? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xC? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xD? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xE? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xF? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

#if 0
/* HUME version */
int PDCI_bcd_hi_digit[256] = {
  /* 0x0? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0x1? */  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  /* 0x2? */  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  /* 0x3? */  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
  /* 0x4? */  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
  /* 0x5? */  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
  /* 0x6? */  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
  /* 0x7? */  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
  /* 0x8? */  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
  /* 0x9? */  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
  /* 0xA? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xB? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xC? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xD? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xE? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  /* 0xF? */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
#endif

#if 0
/* XXX the only valid 2nd nible is  C, D, or F, so an alternate
 * XXX form of the above would be: */
int PDCI_bcd_hi_digit[256] = {
  /* 0x0? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0, -1,  0,
  /* 0x1? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  1, -1,  1,
  /* 0x2? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  2,  2, -1,  2,
  /* 0x3? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3,  3, -1,  3,
  /* 0x4? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  4,  4, -1,  4,
  /* 0x5? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5,  5, -1,  5,
  /* 0x6? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  6,  6, -1,  6,
  /* 0x7? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  7,  7, -1,  7,
  /* 0x8? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  8,  8, -1,  8,
  /* 0x9? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  9,  9, -1,  9,
  /* 0xA? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xB? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xC? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xD? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xE? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  /* 0xF? */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
#endif

/* ================================================================================
 * MISC TABLES
 * ================================================================================ */

Puint64 PDCI_10toThe[] = {
  /* 10^0  = */                          1ULL,
  /* 10^1  = */                         10ULL,
  /* 10^2  = */                        100ULL,
  /* 10^3  = */                       1000ULL,
  /* 10^4  = */                      10000ULL,
  /* 10^5  = */                     100000ULL,
  /* 10^6  = */                    1000000ULL,
  /* 10^7  = */                   10000000ULL,
  /* 10^8  = */                  100000000ULL,
  /* 10^9  = */                 1000000000ULL,
  /* 10^10 = */                10000000000ULL,
  /* 10^11 = */               100000000000ULL,
  /* 10^12 = */              1000000000000ULL,
  /* 10^13 = */             10000000000000ULL,
  /* 10^14 = */            100000000000000ULL,
  /* 10^15 = */           1000000000000000ULL,
  /* 10^16 = */          10000000000000000ULL,
  /* 10^17 = */         100000000000000000ULL,
  /* 10^18 = */        1000000000000000000ULL,
  /* 10^19 = */       10000000000000000000ULL
};

/* ================================================================================
 * MISC STRING CONSTANTS
 * ================================================================================ */

/* used for indent, max length 128 */ 
const char *PDCI_spaces = "                                                                                                                                                                                                                                                                 ";

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * EXTERNAL FUNCTIONS (see pads.h)
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

/* ================================================================================ */ 
/* EXTERNAL ERROR REPORTING FUNCTIONS */

int
P_error(const char *libnm, int level, ...)
{
  va_list ap;
  va_start(ap, level);
  errorv(libnm, (libnm ? level|ERROR_LIBRARY : level), ap);
  va_end(ap);
  return 0;
}

/* ================================================================================ */ 
/* EXTERNAL INVOKE HELPERS */

Perror_t
P_invoke_inv_val_fn(Pinv_val_fn fn, P_t *pads, void *pd_void, void *val_void, ...)
{
  Perror_t  res;
  va_list   type_args;

  va_start(type_args, val_void);
  res = fn(pads, pd_void, val_void, type_args);
  va_end(type_args);
  return res;
}

ssize_t
P_invoke_fmt_fn(Pfmt_fn fn, P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		int *requested_out, const char *delims,
		void *m, void *pd, void *rep, ...)
{
  ssize_t   res;
  va_list   type_args;

  va_start(type_args, rep);
  res = fn(pads, buf, buf_len, buf_full, requested_out, delims, m, pd, rep, type_args);
  va_end(type_args);
  return res;
}

/* ================================================================================ */
/* EXTERNAL LIBRARY TOP-LEVEL OPEN/CLOSE FUNCTIONS */

/* The default disc */
Pdisc_t Pdefault_disc = {
  P_VERSION,
  (Pflags_t)P_NULL_CTL_FLAG,
  Pcharset_ASCII,
  0, /* string read functions do not copy strings */
  0, /* match_max:   0 means end-of-record / soft limit for non-record-based IO disciplines */
  0, /* numeric_max: 0 means end-of-record / soft limit for non-record-based IO disciplines */
  0, /* scan_max:    0 means end-of-record / soft limit for non-record-based IO disciplines */
  0, /* panic_max:   0 means end-of-record / soft limit for non-record-based IO disciplines */
  P_fopen,
  P_error,
  PerrorRep_Max,
  PlittleEndian,
  1000,   /* default max2track */
  10,     /* default max2rep   */
  100,    /* default pcnt2rep  */
  "UTC",  /* in_time_zone      */
  "UTC",  /* out_time_zone     */
  {
    /* default input formats */
    "%m%d%y+%H%M%S%|%m%d%y+%H:%M:%S%|%m%d%Y+%H%M%S%|%m%d%Y+%H:%M:%S%|%&",    /* timestamp */
    "%m%d%y%|%m%d%Y%|%&",      /* date */
    "%H%M%S%|%H:%M:%S%|%&"     /* time */
  },

  {
    /* default output formats */
    "%K",            /* timestamp_explicit */
    "%K",            /* timestamp */
    "%Y-%m-%d",      /* date_explicit */
    "%Y-%m-%d",      /* date */
    "%H:%M:%S",      /* time_explicit */
    "%H:%M:%S"       /* time */
  },

  0,    /* by default, no inv_val_fn map */
  0,    /* by default, no fmt_fn map */
  0     /* a default IO discipline is installed on P_open */
};


Tm_zone_t*
PDCI_cstr2timezone(P_t *pads, const char *zone_str, const char *whatfn)
{
  size_t   expected_len, actual_len;
  char    *end;
  Tm_zone_t *res;

  PDCI_NULLPARAM_CHECK_RET_0("P_cstr2timezone", zone_str);
  expected_len = strlen(zone_str);
  res = tmzone(zone_str, &end, 0, 0);
  if (!res) {
    P_WARN2(pads ? pads->disc : &Pdefault_disc, "%s: failed to parse time zone '%s'", whatfn, zone_str);
    return NULL;
  }
  actual_len = end - zone_str;
  if (actual_len != expected_len) {
    P_WARN5(pads ? pads->disc : &Pdefault_disc, "%s: ignoring extra characters at end of time zone string: %.*s>>%.*s<<",
	    whatfn, actual_len, zone_str, expected_len - actual_len, zone_str + actual_len);
  }
  return res;
}

Tm_zone_t*
P_cstr2timezone(const char *zone_str)
{
  return PDCI_cstr2timezone(NULL, zone_str, "P_cstr2timezone");
}

Perror_t
PDCI_set_in_time_zone(P_t *pads, const char *new_in_time_zone, const char *whatfn)
{
  if (!(pads->in_zone = PDCI_cstr2timezone(pads, new_in_time_zone, whatfn))) {
    return P_ERR;
  }
  pads->disc->in_time_zone = new_in_time_zone;
  return P_OK;
}

Perror_t
PDCI_set_out_time_zone(P_t *pads, const char *new_out_time_zone, const char *whatfn)
{
  if (!(pads->out_zone = PDCI_cstr2timezone(pads, new_out_time_zone, whatfn))) {
    return P_ERR;
  }
  pads->disc->out_time_zone = new_out_time_zone;
  return P_OK;
}

Perror_t
P_set_in_time_zone(P_t *pads, const char *new_in_time_zone)
{
  return PDCI_set_in_time_zone(pads, new_in_time_zone, "P_set_in_time_zone");
}

Perror_t
P_set_out_time_zone(P_t *pads, const char *new_out_time_zone)
{
  return PDCI_set_out_time_zone(pads, new_out_time_zone, "P_set_out_time_zone");
}

Perror_t
PDCI_libopen(P_t **pads_out, Pdisc_t *disc, Pio_disc_t *io_disc, int iodisc_required, const char *whatfn)
{
  Vmalloc_t    *vm;
  P_t          *pads;
  Pint32        testint = 2;

  P_TRACE2(&Pdefault_disc, "PDCI_libopen called, iodisc_required = %d whatfn = %s",
	   iodisc_required, whatfn);
  if (!pads_out) {
    P_WARN1(&Pdefault_disc, "%s: param pads_out must not be NULL", whatfn);
    return P_ERR;
  }
  if (!(vm = vmopen(Vmdcheap, Vmbest, 0))) {
    goto fatal_alloc_err;
  }
  if (!disc) { /* copy the default discipline */
    if (!(disc = vmnewof(vm, 0, Pdisc_t, 1, 0))) {
      disc = &Pdefault_disc;
      goto fatal_alloc_err;
    }
    (*disc) = Pdefault_disc;
  }
  if (io_disc) {
    disc->io_disc = io_disc;
  } else if (iodisc_required && !disc->io_disc) {
    P_WARN1(disc, "%s: Installing default IO discipline : newline-terminated records", whatfn);
    if (!(disc->io_disc = P_ctrec_noseek_make('\n', 0))) {
      P_FATAL1(disc, "%s: Unexpected failure to install default IO discipline", whatfn);
    }
  }
  if (!(pads = vmnewof(vm, 0, P_t, 1, 0))) {
    goto fatal_alloc_err;
  }
  /* allocate a 1 MB + 1 byte buffer to use with sfio */
  if (!(pads->sfbuf = vmoldof(vm, 0, Pbyte, 1024 * 1024, 1))) {
    goto fatal_alloc_err;
  }
  pads->outbuf_len = 1024 * 64;
  pads->outbuf_res = 1024 * 32;
  if (!(pads->outbuf = vmoldof(vm, 0, Pbyte, pads->outbuf_len, 1))) {
    goto fatal_alloc_err;
  }
  pads->inestlev = 0;
  if (!(pads->tmp1 = sfstropen())) {
    goto fatal_alloc_err;
  }
  if (!(pads->tmp2 = sfstropen())) {
    goto fatal_alloc_err;
  }
  if (!(pads->tmp3 = sfstropen())) {
    goto fatal_alloc_err;
  }
  if (!(pads->tmp4 = sfstropen())) {
    goto fatal_alloc_err;
  }
  if (!(pads->rmm_z = RMM_open(RMM_zero_disc_ptr))) {
    goto fatal_alloc_err;
  }
  if (!(pads->rmm_nz = RMM_open(RMM_nozero_disc_ptr))) {
    goto fatal_alloc_err;
  }
  pads->m_endian = (((char*)(&testint))[0]) ? PlittleEndian : PbigEndian;
  pads->id          = lib;
  pads->vm          = vm;
  pads->disc        = disc;
  if (!(pads->head = vmnewof(vm, 0, Pio_elt_t, 1, 0))) {
    goto fatal_alloc_err;
  }
  pads->head->next = pads->head;
  pads->head->prev = pads->head;

  pads->salloc = PDCI_initStkElts;
  if (!(pads->stack = vmnewof(vm, 0, PDCI_stkElt_t, pads->salloc, 0))) {
    goto fatal_alloc_err;
  }
  Pstring_init(pads, &pads->stmp1);
  Pstring_init(pads, &pads->stmp2);
  /* These fields are 0/NiL due to zero-based alloc of pads:
   *   path, io_state, top, buf, balloc, bchars, speclev
   */
  (*pads_out) = pads;
  PDCI_INIT_ECHO_TOKENS(pads);
  P_lib_init();
  if (!(pads->disc->in_time_zone)) {
    P_WARN1(pads->disc, "%s: pads->disc->in_time_zone is null, replacing with 'UTC'", whatfn);
    pads->disc->in_time_zone = "UTC";
  }
  if (P_ERR == PDCI_set_in_time_zone(pads, pads->disc->in_time_zone, whatfn)) {
    P_WARN1(pads->disc, "%s: pads->disc->in_time_zone invalid, replacing with 'UTC'", whatfn);
    pads->disc->in_time_zone = "UTC";
    PDCI_set_in_time_zone(pads, pads->disc->in_time_zone, whatfn);
  }
  if (!(pads->disc->out_time_zone)) {
    P_WARN1(pads->disc, "%s: pads->disc->out_time_zone is null, replacing with 'UTC'", whatfn);
    pads->disc->out_time_zone = "UTC";
  }
  if (P_ERR == PDCI_set_out_time_zone(pads, pads->disc->out_time_zone, whatfn)) {
    P_WARN1(pads->disc, "%s: pads->disc->out_time_zone invalid, replacing with 'UTC'", whatfn);
    pads->disc->out_time_zone = "UTC";
    PDCI_set_out_time_zone(pads, pads->disc->out_time_zone, whatfn);
  }
  return P_OK;

 fatal_alloc_err:
  P_FATAL(disc, "out of space error during PDCI_libopen");
#if 0
  /* P_FATAL halts program, so the following is not needed */
  if (pads) {
    if (pads->rmm_z) {
      RMM_close(pads->rmm_z);
    }
    if (pads->rmm_nz) {
      RMM_close(pads->rmm_nz);
    }
    if (pads->tmp1) {
      sfstrclose(pads->tmp1);
    }
    if (pads->tmp2) {
      sfstrclose(pads->tmp2);
    }
    if (pads->tmp3) {
      sfstrclose(pads->tmp3);
    }
    if (pads->tmp4) {
      sfstrclose(pads->tmp4);
    }
  }
  if (vm) {
    vmclose(vm);
  }
#endif
  return P_ERR;
}

Perror_t
P_open(P_t **pads_out, Pdisc_t *disc, Pio_disc_t *io_disc)
{
  return PDCI_libopen(pads_out, disc, io_disc, 1, "P_open");
}

Perror_t
P_libopen(P_t **pads_out, Pdisc_t *disc, Pio_disc_t *io_disc, int iodisc_required)
{
  return PDCI_libopen(pads_out, disc, io_disc, iodisc_required, "P_libopen");
}

Perror_t
P_close_keep_io_disc(P_t *pads, int keep_io_disc)
{

  PDCI_DISC_0P_CHECKS("P_close");
  Pstring_cleanup(pads, &pads->stmp1);
  Pstring_cleanup(pads, &pads->stmp2);
  if (pads->disc->io_disc && !keep_io_disc) {
    pads->disc->io_disc->unmake_fn(pads->disc->io_disc);
  }
  pads->disc->io_disc = 0;
  if (pads->rmm_z) {
    RMM_close(pads->rmm_z);
  }
  if (pads->rmm_nz) {
    RMM_close(pads->rmm_nz);
  }
  if (pads->tmp1) {
    sfstrclose(pads->tmp1);
  }
  if (pads->tmp2) {
    sfstrclose(pads->tmp2);
  }
  if (pads->vm) {
    vmclose(pads->vm); /* frees everything alloc'd using vm */
  }
#if CACHE_REGEX
#warning caching REGEX
  regcache(0, 0, 0);
#endif

  RBUF_DBG_REPORT("At end of P_close");
  IODISC_DBG_REPORT("At end of P_close");
  return P_OK;
}

Perror_t
P_close(P_t *pads)
{
  PDCI_CLOSE_ECHO_TOKENS(pads);
  return P_close_keep_io_disc(pads, 0);
}

/* ================================================================================ */
/* EXTERNAL DISCIPLINE GET/SET FUNCTIONS */

Pdisc_t *
P_get_disc(P_t *pads)
{
  return (pads ? pads->disc : 0);
}

Perror_t
P_set_disc(P_t *pads, Pdisc_t *new_disc, int xfer_io)
{
  PDCI_DISC_1P_CHECKS("P_set_disc", new_disc);
  if (xfer_io) {
    if (new_disc->io_disc) {
      P_WARN(pads->disc, "P_set_disc: Cannot transfer IO discipline when new_disc->io_disc is non-NULL");
      return P_ERR;
    }
    new_disc->io_disc = pads->disc->io_disc;
    pads->disc->io_disc = 0;
  }
  pads->disc = new_disc;
  return P_OK;
}

Perror_t
P_set_io_disc_keep_old(P_t* pads, Pio_disc_t* new_io_disc, int keep_old_io_disc)
{
  PDCI_stkElt_t    *bot       = &(pads->stack[0]);
  Pio_elt_t     *io_elt    = bot->elt;
  size_t           io_remain  = bot->remain;

  PDCI_DISC_1P_CHECKS("P_set_disc", new_io_disc);
  if (pads->top != 0) {
    P_WARN(pads->disc, "P_set_io_disc: cannot change IO discipline "
	     "in the middle of a speculative read function (e.g., union, ...)");
    return P_ERR;
  }
  if (pads->io && pads->disc->io_disc) {
    /* do a clean sfclose */
    if (P_ERR == pads->disc->io_disc->sfclose_fn(pads, pads->disc->io_disc, io_elt, io_remain)) {
      /* XXX perhaps it was not open?? */
    }
  }
  if (pads->disc->io_disc && !keep_old_io_disc) {
    /* unmake the previous discipline */
    if (P_ERR == pads->disc->io_disc->unmake_fn(pads->disc->io_disc)) {
      /* XXX report an error ??? */
    }
  }
  pads->disc->io_disc = new_io_disc;
  if (pads->io) {
    if (P_ERR == pads->disc->io_disc->sfopen_fn(pads, new_io_disc, pads->io, pads->head)) {
      /* XXX report an error ??? */
    }
  }
  return P_OK;
}

Perror_t
P_set_io_disc(P_t* pads, Pio_disc_t* new_io_disc)
{
  return P_set_io_disc_keep_old(pads, new_io_disc, 0);
}

/* ================================================================================ */
/* EXTERNAL RMM ACCESSORS */

RMM_t *
P_rmm_zero(P_t *pads)
{
  return (pads ? pads->rmm_z : 0);
}

RMM_t *
P_rmm_nozero(P_t *pads)
{
  return (pads ? pads->rmm_nz : 0);
}

/* ================================================================================ */
/* EXTERNAL inv_val FUNCTIONS */

/* Type Pinv_val_fn_map_t: */
struct Pinv_val_fn_map_s {
  Dt_t *dt;
};

typedef struct PDCI_inv_val_fn_elt_s {
  Dtlink_t        link;
  const char     *key;
  Pinv_val_fn     val;
} PDCI_inv_val_fn_elt_t;

void*
PDCI_inv_val_fn_elt_make(Dt_t *dt, PDCI_inv_val_fn_elt_t *a, Dtdisc_t *disc)
{
  PDCI_inv_val_fn_elt_t *b;
  if ((b = oldof(0, PDCI_inv_val_fn_elt_t, 1, 0))) {
    b->key  = a->key;
    b->val  = a->val;
  }
  return b;
}

void
PDCI_inv_val_fn_elt_free(Dt_t *dt, PDCI_inv_val_fn_elt_t *a, Dtdisc_t *disc)
{
  free(a);
}

static Dtdisc_t PDCI_inv_val_fn_map_disc = {
  DTOFFSET(PDCI_inv_val_fn_elt_t, key),     /* key     */
  -1,                                       /* size    */
  DTOFFSET(PDCI_inv_val_fn_elt_t, link),    /* link    */
  (Dtmake_f)PDCI_inv_val_fn_elt_make,       /* makef   */
  (Dtfree_f)PDCI_inv_val_fn_elt_free,       /* freef   */
  NiL,                                      /* comparf */
  NiL,                                      /* hashf   */
  NiL,                                      /* memoryf */
  NiL                                       /* eventf  */
};

Pinv_val_fn_map_t*
Pinv_val_fn_map_create(P_t *pads)
{
  Pinv_val_fn_map_t *map; 

  PDCI_DISC_0P_CHECKS_RET_0("Pinv_val_fn_map_create");
  if (!pads->vm) {
    P_WARN(pads->disc, "Pinv_val_fn_map_create: pads handle not initialized properly");
    return 0;
  }
  if (!(map = vmnewof(pads->vm, 0, Pinv_val_fn_map_t, 1, 0))) {
    goto alloc_err;
  }
  if (!(map->dt = dtopen(&PDCI_inv_val_fn_map_disc, Dtset))) {
    vmfree(pads->vm, map);
    goto alloc_err;
  }
  return map;

 alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, "Pinv_val_fn_map_create", "Memory alloc error");
  return 0;
}

Perror_t
Pinv_val_fn_map_destroy(P_t *pads, Pinv_val_fn_map_t *map)
{
  PDCI_DISC_1P_CHECKS("Pinv_val_fn_map_destroy", map);
  if (map->dt) {
    dtclose(map->dt);
    map->dt = 0;
  }
  if (pads->vm) {
    vmfree(pads->vm, map);
  }
  return P_OK;
}

Perror_t
Pinv_val_fn_map_clear(P_t *pads, Pinv_val_fn_map_t *map)
{
  PDCI_DISC_1P_CHECKS("Pinv_val_fn_map_clear", map);
  if (map->dt) {
    dtclear(map->dt);
    return P_OK;
  }
  return P_ERR;
}

Pinv_val_fn
P_get_inv_val_fn(P_t* pads, Pinv_val_fn_map_t *map, const char *type_name)
{
  PDCI_inv_val_fn_elt_t *tmp;

  PDCI_DISC_2P_CHECKS_RET_0("P_get_inv_val_fn", map, type_name);
#ifndef NDEBUG
  if (!map->dt) {
    P_WARN(pads->disc, "P_get_inv_val_fn: map not initialized properly");
    return 0;
  }
#endif
  if ((tmp = dtmatch(map->dt, type_name))) {
    return tmp->val;
  }
  return 0;
}
 
Pinv_val_fn
P_set_inv_val_fn(P_t* pads, Pinv_val_fn_map_t *map, const char *type_name, Pinv_val_fn fn)
{
  Pinv_val_fn             res = 0;
  PDCI_inv_val_fn_elt_t  *tmp;
  PDCI_inv_val_fn_elt_t   insert_elt;

  PDCI_DISC_2P_CHECKS_RET_0("P_set_inv_val_fn", map, type_name);
#ifndef NDEBUG
  if (!map->dt) {
    P_WARN(pads->disc, "P_set_inv_val_fn: map not initialized properly");
    return 0;
  }
#endif
  if ((tmp = dtmatch(map->dt, type_name))) {
    res = tmp->val;
    tmp->val = fn;
    return res;
  }
  if (fn) {
    insert_elt.key = type_name;
    insert_elt.val = fn;
    if (!(tmp = dtinsert(map->dt, &insert_elt))) {
      P_WARN(pads->disc, "** PADSC internal error: dtinsert failed (out of memory?) **");
    }
  }
  return 0;
}

/* ================================================================================ */
/* EXTERNAL fmt FUNCTIONS */

/* Type Pfmt_fn_map_t: */
struct Pfmt_fn_map_s {
  Dt_t *dt;
};

typedef struct PDCI_fmt_fn_elt_s {
  Dtlink_t        link;
  const char     *key;
  Pfmt_fn         val;
} PDCI_fmt_fn_elt_t;

void*
PDCI_fmt_fn_elt_make(Dt_t *dt, PDCI_fmt_fn_elt_t *a, Dtdisc_t *disc)
{
  PDCI_fmt_fn_elt_t *b;
  if ((b = oldof(0, PDCI_fmt_fn_elt_t, 1, 0))) {
    b->key  = a->key;
    b->val  = a->val;
  }
  return b;
}

void
PDCI_fmt_fn_elt_free(Dt_t *dt, PDCI_fmt_fn_elt_t *a, Dtdisc_t *disc)
{
  free(a);
}

static Dtdisc_t PDCI_fmt_fn_map_disc = {
  DTOFFSET(PDCI_fmt_fn_elt_t, key),         /* key     */
  -1,                                       /* size    */
  DTOFFSET(PDCI_fmt_fn_elt_t, link),        /* link    */
  (Dtmake_f)PDCI_fmt_fn_elt_make,           /* makef   */
  (Dtfree_f)PDCI_fmt_fn_elt_free,           /* freef   */
  NiL,                                      /* comparf */
  NiL,                                      /* hashf   */
  NiL,                                      /* memoryf */
  NiL                                       /* eventf  */
};

Pfmt_fn_map_t*
Pfmt_fn_map_create(P_t *pads)
{
  Pfmt_fn_map_t *map; 

  PDCI_DISC_0P_CHECKS_RET_0("Pfmt_fn_map_create");
  if (!pads->vm) {
    P_WARN(pads->disc, "Pfmt_fn_map_create: pads handle not initialized properly");
    return 0;
  }
  if (!(map = vmnewof(pads->vm, 0, Pfmt_fn_map_t, 1, 0))) {
    goto alloc_err;
  }
  if (!(map->dt = dtopen(&PDCI_fmt_fn_map_disc, Dtset))) {
    vmfree(pads->vm, map);
    goto alloc_err;
  }
  return map;

 alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, "Pfmt_fn_map_create", "Memory alloc error");
  return 0;
}

Perror_t
Pfmt_fn_map_destroy(P_t *pads, Pfmt_fn_map_t *map)
{
  PDCI_DISC_1P_CHECKS("Pfmt_fn_map_destroy", map);
  if (map->dt) {
    dtclose(map->dt);
    map->dt = 0;
  }
  if (pads->vm) {
    vmfree(pads->vm, map);
  }
  return P_OK;
}

Perror_t
Pfmt_fn_map_clear(P_t *pads, Pfmt_fn_map_t *map)
{
  PDCI_DISC_1P_CHECKS("Pfmt_fn_map_clear", map);
  if (map->dt) {
    dtclear(map->dt);
    return P_OK;
  }
  return P_ERR;
}

Pfmt_fn
P_get_fmt_fn(P_t* pads, Pfmt_fn_map_t *map, const char *type_name)
{
  PDCI_fmt_fn_elt_t *tmp;

  PDCI_DISC_2P_CHECKS_RET_0("P_get_fmt_fn", map, type_name);
#ifndef NDEBUG
  if (!map->dt) {
    P_WARN(pads->disc, "P_get_fmt_fn: map not initialized properly");
    return 0;
  }
#endif
  if ((tmp = dtmatch(map->dt, type_name))) {
    return tmp->val;
  }
  return 0;
}
 
Pfmt_fn
P_set_fmt_fn(P_t* pads, Pfmt_fn_map_t *map, const char *type_name, Pfmt_fn fn)
{
  Pfmt_fn             res = 0;
  PDCI_fmt_fn_elt_t  *tmp;
  PDCI_fmt_fn_elt_t   insert_elt;

  PDCI_DISC_2P_CHECKS_RET_0("P_set_fmt_fn", map, type_name);
#ifndef NDEBUG
  if (!map->dt) {
    P_WARN(pads->disc, "P_set_fmt_fn: map not initialized properly");
    return 0;
  }
#endif
  if ((tmp = dtmatch(map->dt, type_name))) {
    res = tmp->val;
    tmp->val = fn;
    return res;
  }
  if (fn) {
    insert_elt.key = type_name;
    insert_elt.val = fn;
    if (!(tmp = dtinsert(map->dt, &insert_elt))) {
      P_WARN(pads->disc, "** PADSC internal error: dtinsert failed (out of memory?) **");
    }
  }
  return 0;
}

/* ================================================================================ */
/* EXTERNAL IO FUNCTIONS */

const char *
P_io_read_unit(P_t *pads)
{
  PDCI_IODISC_INIT_CHECKS_RET_0("P_io_read_unit");
  return pads->disc->io_disc->read_unit_fn(pads, pads->disc->io_disc);
}

Perror_t
P_io_set(P_t *pads, Sfio_t *io)
{
  PDCI_IODISC_1P_CHECKS("P_io_set", io);
  if (pads->io) {
    if (pads->path && strcmp(pads->path, "/dev/stdin")) {
      P_WARN(pads->disc, "IO_set called with previous installed io due to fopen; closing");
    }
    P_io_close(pads);
    /* path and io are no longer set */
  }
  return PDCI_io_install_io(pads, io);
}

Perror_t
P_io_fopen(P_t *pads, const char *path)
{
  Sfio_t           *io; 
  Pfopen_fn         fopen_fn;

  PDCI_IODISC_1P_CHECKS("P_io_fopen", path);
  if (pads->io) {
    if (pads->path && strcmp(pads->path, "/dev/stdin")) {
      P_WARN(pads->disc, "IO_fopen called while previous file still open; closing");
    }
    P_io_close(pads);
    /* path and io are no longer set */
  }
  if (!(pads->path = vmnewof(pads->vm, 0, char, strlen(path) + 1, 0))) {
    P_FATAL(pads->disc, "out of space [string to record file path]");
    return P_ERR;
  }
  strcpy(pads->path, path);
  fopen_fn = (pads->disc->fopen_fn) ? pads->disc->fopen_fn : P_fopen;
  if (!(io = fopen_fn(path, "r"))) {
    if (errno) {
      P_SYSERR1(pads->disc, "Failed to open file \"%s\"", path);
    } else {
      P_WARN1(pads->disc, "Failed to open file \"%s\"", path);
    }
    vmfree(pads->vm, pads->path);
    pads->path = 0;
    return P_ERR;
  }
  return PDCI_io_install_io(pads, io);
}

Perror_t
P_io_close(P_t *pads)
{
  PDCI_stkElt_t    *bot;
  Pio_elt_t     *io_elt;
  size_t           io_remain;

  PDCI_DISC_0P_CHECKS("P_io_close");
  bot        = &(pads->stack[0]);
  io_elt     = bot->elt;
  io_remain  = bot->remain;

  if (!pads->io) {
    return P_ERR;
  }
  /* close IO discpline */
  if (pads->disc->io_disc) {
    pads->disc->io_disc->sfclose_fn(pads, pads->disc->io_disc, io_elt, io_remain);
  }
  if (pads->path && strcmp(pads->path, "/dev/stdin")) {
    sfclose(pads->io);
  }
  if (pads->vm && pads->path) {
    vmfree(pads->vm, pads->path);
  }
  pads->io = 0;
  pads->path = 0;
  return P_OK;
}

Perror_t
P_io_next_rec(P_t *pads, size_t *skipped_bytes_out) {
  PDCI_stkElt_t    *tp;
  Pio_elt_t        *keep_elt;
  Pio_elt_t        *next_elt;
  int               prev_eor;

  PDCI_IODISC_1P_CHECKS("P_io_next_rec", skipped_bytes_out);
  tp                    = &(pads->stack[pads->top]);
  (*skipped_bytes_out)  = 0;
  if (pads->disc->io_disc->rec_based == 0) {
    P_WARN(pads->disc, "P_io_next_rec called when pads->disc->io_disc does not support records");
    return P_ERR;
  }
  while (1) {
    prev_eor = tp->elt->eor;
    (*skipped_bytes_out) += tp->remain;
    tp->remain = 0;
    if (tp->elt->eof) {
      return P_ERR;
    }
    /* advance IO cursor */
    if (tp->elt->next != pads->head) {
      tp->elt = tp->elt->next;
    } else {
      /* use IO disc read_fn */
      keep_elt = pads->stack[0].elt;
      if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
	tp->elt = P_LAST_ELT(pads->head); /* IO disc may have added eof elt */
	tp->remain = 0;
	return P_ERR;
      }
#ifndef NDEBUG
      if (next_elt == pads->head) { /* should not happen */
	P_FATAL(pads->disc, "Internal error, P_io_next_rec observed incorrect read_fn behavior");
	return P_ERR;
      }
#endif
      tp->elt = next_elt;
    }
    tp->remain = tp->elt->len;
    if (prev_eor) { /* we just advanced past an EOR */
      break;
    }
    /* just advanced past a partial read -- continue while loop */
  }
  return P_OK;
}

Perror_t
P_io_skip_bytes(P_t *pads, size_t width, size_t *skipped_bytes_out) {
  size_t            skipped;
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_1P_CHECKS("P_io_skip_bytes", skipped_bytes_out);
  if (width == 0) return P_OK;
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  skipped = end - begin;
  PDCI_IO_FORWARD(skipped, goto fatal_forward_err);
  (*skipped_bytes_out) = skipped;
  return (skipped == width) ? P_OK : P_ERR;

 fatal_nb_io_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_IO_ERR, "P_io_skip_bytes", "IO error (nb)");
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, "P_io_skip_bytes", "IO_forward error");
  return P_ERR;
}

int
P_io_at_eor(P_t *pads) {
  PDCI_stkElt_t    *tp;

  PDCI_DISC_0P_CHECKS_RET_0("P_io_at_eor");
  tp        = &(pads->stack[pads->top]);
  return (tp->remain == 0 && tp->elt && tp->elt->eor) ? 1 : 0;
}

int
P_io_at_eof(P_t *pads) {
  PDCI_stkElt_t    *tp;

  PDCI_DISC_0P_CHECKS_RET_0("P_io_at_eof");
  tp        = &(pads->stack[pads->top]);
  return (tp->remain == 0 && tp->elt && tp->elt->eof) ? 1 : 0;
}

int
P_io_at_eor_OR_EOF(P_t *pads) {
  PDCI_stkElt_t    *tp;

  PDCI_DISC_0P_CHECKS_RET_0("P_io_at_eor_or_eof");
  tp        = &(pads->stack[pads->top]);
  return (tp->remain == 0 && tp->elt && (tp->elt->eor || tp->elt->eof)) ? 1 : 0;
}

Perror_t
P_io_getPos(P_t *pads, Ppos_t *pos, int offset)
{
  Perror_t res = P_OK;
  PDCI_DISC_1P_CHECKS("P_io_getPos", pos);
  if (offset == 0) {
    PDCI_ALWAYS_GETPOS_CHECKED(pads, *pos, res);
  } else if (offset > 0) {
    PDCI_ALWAYS_GETPOS_PLUS_CHECKED(pads, *pos, offset, res);
  } else {
    int noffset = -1 * offset;
    PDCI_ALWAYS_GETPOS_MINUS_CHECKED(pads, *pos, noffset, res);
  }
  return res;
}

Perror_t
P_io_getLocB(P_t *pads, Ploc_t *loc, int offset)
{
  Perror_t res = P_OK;
  PDCI_DISC_1P_CHECKS("P_io_getLocB", loc);
  if (offset == 0) {
    PDCI_ALWAYS_GETPOS_CHECKED(pads, loc->b, res);
  } else if (offset > 0) {
    PDCI_ALWAYS_GETPOS_PLUS_CHECKED(pads, loc->b, offset, res);
  } else {
    int noffset = -1 * offset;
    PDCI_ALWAYS_GETPOS_MINUS_CHECKED(pads, loc->b, noffset, res);
  }
  return res;
}

Perror_t
P_io_getLocE(P_t *pads, Ploc_t *loc, int offset)
{
  Perror_t res = P_OK;
  PDCI_DISC_1P_CHECKS("P_io_getLocE", loc);
  if (offset == 0) {
    PDCI_ALWAYS_GETPOS_CHECKED(pads, loc->e, res);
  } else if (offset > 0) {
    PDCI_ALWAYS_GETPOS_PLUS_CHECKED(pads, loc->e, offset, res);
  } else {
    int noffset = -1 * offset;
    PDCI_ALWAYS_GETPOS_MINUS_CHECKED(pads, loc->e, noffset, res);
  }
  return res;
}

Perror_t
P_io_getLoc(P_t *pads, Ploc_t *loc, int offset)
{
  Perror_t res = P_OK;
  PDCI_DISC_1P_CHECKS("P_io_getLoc", loc);
  if (offset == 0) {
    PDCI_ALWAYS_GETPOS_CHECKED(pads, loc->b, res);
  } else if (offset > 0) {
    PDCI_ALWAYS_GETPOS_PLUS_CHECKED(pads, loc->b, offset, res);
  } else {
    int noffset = -1 * offset;
    PDCI_ALWAYS_GETPOS_MINUS_CHECKED(pads, loc->b, noffset, res);
  }
  PDCI_ALWAYS_ENDLOC_SPAN0(pads, *loc);
  return res;
}

#if P_CONFIG_WRITE_FUNCTIONS > 0
Pbyte*
P_io_write_start(P_t *pads, Sfio_t *io, size_t *buf_len, int *set_buf)
{
  PDCI_DISC_3P_CHECKS_RET_0("P_io_write_start", io, buf_len, set_buf);
  return PDCI_io_write_start(pads, io, buf_len, set_buf, "P_io_write_start");
}

ssize_t
P_io_write_commit(P_t *pads, Sfio_t *io, Pbyte *buf, int set_buf, size_t num_bytes)
{
  PDCI_DISC_2P_CHECKS_RET_SSIZE("P_io_write_commit", io, buf);
  return PDCI_io_write_commit(pads, io, buf, set_buf, num_bytes, "P_io_write_commit");
}

void
P_io_write_abort (P_t *pads, Sfio_t *io, Pbyte *buf, int set_buf)
{
  PDCI_DISC_2P_CHECKS_RET_VOID("P_io_write_abort", io, buf);
  PDCI_io_write_abort(pads, io, buf, set_buf, "P_io_write_abort");
}

ssize_t
P_io_rec_write2io(P_t *pads, Sfio_t *io, Pbyte *buf, size_t rec_data_len)
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rec_write2io");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_write2io", io);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_write2io", buf);
  return PDCI_io_rec_write2io(pads, io, buf, rec_data_len, "P_io_rec_write2io");
}

ssize_t
P_io_rec_open_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full)
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rec_open_write2buf");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_open_write2buf", buf);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_open_write2buf", buf_full);
  return PDCI_io_rec_open_write2buf(pads, buf, buf_len, buf_full, "P_io_rec_open_write2buf");
}

ssize_t
P_io_rec_close_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			 Pbyte *rec_start, size_t num_bytes) 
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rec_close_write2buf");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_close_write2buf", buf);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_close_write2buf", buf_full);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_close_write2buf", rec_start);
  return PDCI_io_rec_close_write2buf(pads, buf, buf_len, buf_full, rec_start, num_bytes, "P_io_rec_close_write2buf");
}

ssize_t
P_io_rblk_write2io(P_t *pads, Sfio_t *io, Pbyte *buf, size_t blk_data_len, Puint32 num_recs)
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rblk_write2io");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_write2io", io);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_write2io", buf);
  return PDCI_io_rblk_write2io(pads, io, buf, blk_data_len, num_recs, "P_io_rblk_write2io");
}

ssize_t
P_io_rblk_open_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full)
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rblk_open_write2buf");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_open_write2buf", buf);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_open_write2buf", buf_full);
  return PDCI_io_rblk_open_write2buf(pads, buf, buf_len, buf_full, "P_io_rblk_open_write2buf");
}

ssize_t
P_io_rblk_close_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			    Pbyte *blk_start, size_t num_bytes, Puint32 num_recs)
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rblk_close_write2buf");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_close_write2buf", buf);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_close_write2buf", buf_full);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rblk_close_write2buf", blk_start);
  return PDCI_io_rblk_close_write2buf(pads, buf, buf_len, buf_full, blk_start, num_bytes, num_recs, "P_io_rblk_close_write2buf");
}

ssize_t
P_io_rec_fmt2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full)
{
  PDCI_IODISC_INIT_CHECKS_RET_SSIZE("P_io_rec_fmt2buf");
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_fmt2buf", buf);
  PDCI_NULLPARAM_CHECK_RET_SSIZE("P_io_rec_fmt2buf", buf_full);
  return PDCI_io_rec_fmt2buf(pads, buf, buf_len, buf_full, Pcharset_ASCII, "P_io_rec_fmt2buf");
}

#endif

/* ================================================================================ */
/* EXTERNAL IO CHECKPOINT API */

Perror_t
P_io_checkpoint(P_t *pads, int speculative)
{
  PDCI_DISC_INIT_CHECKS("P_io_checkpoint");
  if (++(pads->top) >= pads->salloc) {
    PDCI_stkElt_t *stack_next;
    size_t salloc_next = 2 * pads->salloc;
    error(0, "XXX_REMOVE Growing from %d to %d checkpoint stack slots", pads->salloc, salloc_next);
    if (!(stack_next = vmnewof(pads->vm, pads->stack, PDCI_stkElt_t, salloc_next, 0))) {
      P_FATAL(pads->disc, "out of space [input cursor stack]");
      return P_ERR;
    }
    pads->stack  = stack_next;
    pads->salloc = salloc_next;
  }
  pads->stack[pads->top].elt     = pads->stack[pads->top - 1].elt;
  pads->stack[pads->top].remain  = pads->stack[pads->top - 1].remain;
#ifdef USE_GALAX
  pads->stack[pads->top].id_gen  = pads->stack[pads->top - 1].id_gen;
#endif
  pads->stack[pads->top].spec    = speculative;
  if (speculative) {
    (pads->speclev)++;
  }
  PDCI_ECHO_CHKPOINT(pads, "CheckPoint");
  return P_OK;
}

Perror_t
P_io_restore(P_t *pads)
{
  PDCI_DISC_INIT_CHECKS("P_io_restore");
  if (pads->top <= 0) {
    P_WARN(pads->disc, "Internal error: P_io_restore called when stack top <= 0");
    return P_ERR;
  }
  if (pads->stack[pads->top].spec) {
    (pads->speclev)--;
  }
  /* this discards all changes since the latest checkpoint */ 
  (pads->top)--;
  PDCI_ECHO_CHKPOINT(pads, "Rollback");
  return P_OK;
}

Perror_t
P_io_commit(P_t *pads)
{
  PDCI_DISC_INIT_CHECKS("P_io_commit");
#ifndef NDEBUG
  if (pads->top <= 0) {
    P_WARN(pads->disc, "Internal error: P_io_commit called when stack top <= 0");
    return P_ERR;
  }
#endif
  if (pads->stack[pads->top].spec) {
    (pads->speclev)--;
  }
  /* propagate changes to elt/remain up to next level */
  pads->stack[pads->top - 1].elt    = pads->stack[pads->top].elt;
  pads->stack[pads->top - 1].remain = pads->stack[pads->top].remain;
#ifdef USE_GALAX
  pads->stack[pads->top - 1].id_gen  = pads->stack[pads->top].id_gen;
#endif
  (pads->top)--;
  PDCI_ECHO_CHKPOINT(pads, "Commit");
  return P_OK;
}

Perror_t
P_io_commit_pos(P_t *pads, Ppos_t pos)
{
  Pio_elt_t *elt;
  PDCI_DISC_INIT_CHECKS("P_io_commit_pos");
#ifndef NDEBUG
  if (pads->top <= 0) {
    P_WARN(pads->disc, "Internal error: P_io_commit_pos called when stack top <= 0");
    return P_ERR;
  }
#endif
  if (pads->stack[pads->top].spec) {
    (pads->speclev)--;
  }
  /* see if we can find pos */
#ifdef USE_GALAX
  pads->stack[pads->top - 1].id_gen  = pads->stack[pads->top].id_gen;
#endif
  (pads->top)--;
  if (P_ERR == PDCI_io_getElt(pads, pos.num, &elt)) {
    P_FATAL(pads->disc, "P_io_commit_pos called with invalid pos (elt not found)");
  }
  pads->stack[pads->top].elt    = elt;
  pads->stack[pads->top].remain = elt->len - (pos.offset - elt->offset); /* if pos.offset == elt->offset, all bytes remain */
  return P_OK;
}

unsigned int
P_spec_level(P_t *pads)
{
  PDCI_DISC_INIT_CHECKS("P_spec_level");
  return pads->speclev;
}

unsigned int
P_is_current_spec(P_t *pads)
{
  PDCI_DISC_INIT_CHECKS("P_is_current_spec");

  if (pads->top <= 0)
    return 0;

  return pads->stack[pads->top].spec;
}
/* ================================================================================ */
/* Pstring helper functions */

Perror_t
Pstring_init(P_t *pads, Pstring *s)
{
#ifndef NDEBUG
  if (!s) {
    return P_ERR;
  }
#endif
  memset((void*)s, 0, sizeof(*s));
  return P_OK;
}

Perror_t
Pstring_cleanup(P_t *pads, Pstring *s)
{
  PDCI_DISC_1P_CHECKS("Pstring_cleanup", s);
  /* if (s->sharing) { P_WARN1(pads->disc, "XXX_REMOVE cleanup: string %p is no longer sharing", (void*)s); } */
  if (s->rbuf && (0 != RMM_free_rbuf(s->rbuf))) {
    PDCI_report_err(pads, P_LEV_FATAL, 0, P_ALLOC_ERR, "Pstring_cleanup", "Couldn\'t free growable buffer");
  }
  memset((void*)s, 0, sizeof(*s));
  return P_OK;
}

Perror_t
Pstring_share(P_t *pads, Pstring *targ, const Pstring *src)
{
  PDCI_DISC_2P_CHECKS("Pstring_share", src, targ);
  PDCI_STR_SHARE(targ, src->str, src->len);
  return P_OK;
}

Perror_t
Pstring_cstr_share(P_t *pads, Pstring *targ, const char *src, size_t len)
{
  PDCI_DISC_2P_CHECKS("Pstring_cstr_share", src, targ);
  PDCI_STR_SHARE(targ, src, len);
  return P_OK;
}

Perror_t
Pstring_copy(P_t *pads, Pstring *targ, const Pstring *src)
{
  PDCI_DISC_2P_CHECKS("Pstring_copy", src, targ);
  PDCI_STR_CPY(targ, src->str, src->len);
  return P_OK;

 fatal_alloc_err:
  P_FATAL(pads->disc, "Pstring_copy: out of space");
  return P_ERR;
}

Perror_t
Pstring_cstr_copy(P_t *pads, Pstring *targ, const char *src, size_t len)
{
  PDCI_DISC_2P_CHECKS("Pstring_cstr_copy", src, targ);
  PDCI_STR_CPY(targ, src, len);
  return P_OK;

 fatal_alloc_err:
  P_FATAL(pads->disc, "Pstring_cstr_copy: out of space");
  return P_ERR;
}

Perror_t
Pstring_preserve(P_t *pads, Pstring *s)
{
  PDCI_DISC_1P_CHECKS("Pstring_preserve", s);
  PDCI_STR_PRESERVE(s);
  return P_OK;

 fatal_alloc_err:
  P_FATAL(pads->disc, "Pstring_preserve: out of space");
  return P_ERR;
}

Perror_t
Pstring_pd_init(P_t *pads, Pbase_pd *pd)
{
  PDCI_DISC_1P_CHECKS("Pstring_pd_init", pd);
  memset((void*)pd, 0, sizeof(*pd));
  return P_OK;
}

Perror_t
Pstring_pd_cleanup(P_t *pads, Pbase_pd *pd)
{
  PDCI_DISC_1P_CHECKS("Pstring_pd_cleanup", pd);
  return P_OK;
}

Perror_t
Pstring_pd_copy(P_t *pads, Pbase_pd *targ, const Pbase_pd *src)
{
  PDCI_DISC_2P_CHECKS("Pstring_pd_copy", src, targ);
  (*targ) = (*src);
  return P_OK;
}

/* ================================================================================ */
/* EXTERNAL MISC ROUTINES */

/* helpers for enumeration types */
#define _F1 "|NoSet|NoPrint"
#define _F2 "|NoBaseCheck"
#define _F3 "|NoUserCheck"
#define _F4 "|NoWhereCheck"
#define _F5 "|NoForallCheck"

const char *
Pbase_m2str(P_t *pads, Pbase_m m)
{
  const char *s;
  switch (m) {
  case 0:    s =  "|"                ; break;
  case 1:    s =  _F1                ; break;
  case 2:    s =      _F2            ; break;
  case 3:    s =  _F1 _F2            ; break;
  case 4:    s =          _F3        ; break;
  case 5:    s =  _F1     _F3        ; break;
  case 6:    s =      _F2 _F3        ; break;
  case 7:    s =  _F1 _F2 _F3        ; break;
  case 8:    s =              _F4    ; break;
  case 9:    s =  _F1         _F4    ; break;
  case 10:   s =      _F2     _F4    ; break;
  case 11:   s =  _F1 _F2     _F4    ; break;
  case 12:   s =          _F3 _F4    ; break;
  case 13:   s =  _F1     _F3 _F4    ; break;
  case 14:   s =      _F2 _F3 _F4    ; break;
  case 15:   s =  _F1 _F2 _F3 _F4    ; break;
  case 16:   s =                  _F5; break;
  case 17:   s =  _F1             _F5; break;
  case 18:   s =      _F2         _F5; break;
  case 19:   s =  _F1 _F2         _F5; break;
  case 20:   s =          _F3     _F5; break;
  case 21:   s =  _F1     _F3     _F5; break;
  case 22:   s =      _F2 _F3     _F5; break;
  case 23:   s =  _F1 _F2 _F3     _F5; break;
  case 24:   s =              _F4 _F5; break;
  case 25:   s =  _F1         _F4 _F5; break;
  case 26:   s =      _F2     _F4 _F5; break;
  case 27:   s =  _F1 _F2     _F4 _F5; break;
  case 28:   s =          _F3 _F4 _F5; break;
  case 29:   s =  _F1     _F3 _F4 _F5; break;
  case 30:   s =      _F2 _F3 _F4 _F5; break;
  case 31:   s =  _F1 _F2 _F3 _F4 _F5; break;
  default:   s = "|*Invalid Pbase_m value*"; break;
  }
  return s+1;
}

const char *
PerrorRep2str(PerrorRep e)
{
  switch (e)
    {
    case PerrorRep_Max:
      return "PerrorRep_Max";
    case PerrorRep_Med:
      return "PerrorRep_Med";
    case PerrorRep_Min:
      return "PerrorRep_Min";
    case PerrorRep_None:
      return "PerrorRep_None";
    default:
      break;
    }
  return "*Invalid PerrorRep value*";
}

const char *
Pendian2str(Pendian_t e)
{
  switch (e)
    {
    case PbigEndian:
      return "PbigEndian";
    case PlittleEndian:
      return "PlittleEndian";
    default:
      break;
    }
  return "*Invalid Pendian value*";
}

const char *
Pcharset2str(Pcharset e)
{
  switch (e)
    {
    case Pcharset_ASCII:
      return "Pcharset_ASCII";
    case Pcharset_EBCDIC:
      return "Pcharset_EBCDIC";
    default:
      break;
    }
  return "*Invalid Pcharset value*";
}

char*
P_fmt_char(char c) {
  return fmtquote(&c, NiL, NiL, 1, 0);
}

char*
P_qfmt_char(char c) {
  return fmtquote(&c, "\'", "\'", 1, 1);
}

char*
P_fmt_str(const Pstring *s) {
  return fmtquote(s->str, NiL, NiL, s->len, 0);
}

char*
P_qfmt_str(const Pstring *s) {
  return fmtquote(s->str, "\"", "\"", s->len, 1);
}

char*
P_fmt_cstr_n(const char *s, size_t len) {
  return fmtquote(s, NiL, NiL, len, 0);
}

char*
P_fmt_cstr(const char *s) {
  return fmtquote(s, NiL, NiL, strlen(s), 0);
}

char*
P_qfmt_cstr_n(const char *s, size_t len) {
  return fmtquote(s, "\"", "\"", len, 1);
}

char*
P_qfmt_cstr(const char *s) {
  return fmtquote(s, "\"", "\"", strlen(s), 1);
}

/*
 * Note: swapmem ops documented with binary read functions
 * Here we use in-place swap, which is safe with gsf's swapmem
 */

Perror_t
P_swap_bytes(Pbyte *bytes, size_t num_bytes)
{
#ifndef NDEBUG
  if (!bytes) {
    P_WARN(&Pdefault_disc, "P_swap_bytes: param bytes must not be NULL");
    return P_ERR;
  }
#endif

  switch (num_bytes) {
  case 2:
    swapmem(1, bytes, bytes, num_bytes);
    return P_OK;
  case 4:
    swapmem(3, bytes, bytes, num_bytes);
    return P_OK;
  case 8:
    swapmem(7, bytes, bytes, num_bytes);
    return P_OK;
  }
  P_WARN1(&Pdefault_disc, "P_swap_bytes: invalid num_bytes (%d), use 2, 4, or 8", num_bytes);
  return P_ERR;
}

Sfio_t*
P_fopen(const char *string, const char *mode)
{
  Sfio_t* io;
  if (strcmp(string, "/dev/stdin")  == 0) {
    if (strcmp(mode, "r")) {
      /* not simple read mode, let sfopen decide if mode can be applied to sfstdin */
      io = sfopen(sfstdin,  NiL, mode);
      goto install_pzip;
    }
    io = sfstdin;
    goto install_pzip;
  }
  if (strcmp(string, "/dev/stdout") == 0) {
    if (strcmp(mode, "a")) {
      /* not simple append mode, let sfopen decide if mode can be applied to sfstdout */
      return sfopen(sfstdout,  NiL, mode);
    }
    return sfstdout;
  }
  if (strcmp(string, "/dev/stderr") == 0) {
    if (strcmp(mode, "a")) {
      /* not simple append mode, let sfopen decide if mode can be applied to sfstderr */
      return sfopen(sfstderr,  NiL, mode);
    }
    return sfstderr;
  }
  if (strchr(mode, 'r')) {
    io = sfopen(NiL, string, mode);
    goto install_pzip;
  }
  /* not a read mode */
  return sfopen(NiL, string, mode);

 install_pzip:
#ifdef USE_PZIP
  {
    struct stat	st;
    static Pzdisc_t pzdisc;
    if (!io) return 0;
    if (sfsize(io) || (!fstat(sffileno(io), &st) && S_ISFIFO(st.st_mode))) {
      pzdisc.errorf = errorf; /* from <error.h> */
      if (sfdcpzip(io, string, 0, &pzdisc) < 0) {
	P_SYSERR1(&Pdefault_disc, "P_fopen for file \"%s\": decompression error", string);
	sfclose(io);
	errno = 0; /* already reported system error */
	return 0;
      }
    }
  }
  return io;
#else /* decompression libraries not included, do not attempt to inflate */
  return io;
#endif
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * INTERNAL FUNCTIONS (see pads-internal.h)
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

/* ================================================================================ */ 
/* INTERNAL ERROR REPORTING FUNCTIONS */

const char *P_pstate2str(Pflags_t pstate)
{
  if (pstate & P_Panic) return "Panic";
  if (pstate) return "*UnknownPStateFlags*";
  return "Normal";
}

const char *P_errCode2str(PerrCode_t code)
{
  switch (code) {
  case P_NOT_PARSED: return "P_NOT_PARSED";
  case P_NO_ERR: return "P_NO_ERR";
  case P_SKIPPED: return "P_SKIPPED";
  case P_UNEXPECTED_ERR: return "P_UNEXPECTED_ERR";
  case P_BAD_PARAM: return "P_BAD_PARAM";
  case P_SYS_ERR: return "P_SYS_ERR";
  case P_IO_ERR: return "P_IO_ERR";
  case P_CHKPOINT_ERR: return "P_CHKPOINT_ERR";
  case P_COMMIT_ERR: return "P_COMMIT_ERR";
  case P_RESTORE_ERR: return "P_RESTORE_ERR";
  case P_ALLOC_ERR: return "P_ALLOC_ERR";
  case P_FORWARD_ERR: return "P_FORWARD_ERR";
  case P_PANIC_SKIPPED: return "P_PANIC_SKIPPED";
  case P_USER_CONSTRAINT_VIOLATION: return "P_USER_CONSTRAINT_VIOLATION";
  case P_MISSING_LITERAL: return "P_MISSING_LITERAL";
  case P_ARRAY_ELEM_ERR: return "P_ARRAY_ELEM_ERR";
  case P_ARRAY_SEP_ERR: return "P_ARRAY_SEP_ERR";
  case P_ARRAY_TERM_ERR: return "P_ARRAY_TERM_ERR";
  case P_ARRAY_SIZE_ERR: return "P_ARRAY_SIZE_ERR";
  case P_ARRAY_SEP_TERM_SAME_ERR: return "P_ARRAY_SEP_TERM_SAME_ERR";
  case P_ARRAY_USER_CONSTRAINT_ERR: return "P_ARRAY_USER_CONSTRAINT_ERR";
  case P_ARRAY_MIN_BIGGER_THAN_MAX_ERR: return "P_ARRAY_MIN_BIGGER_THAN_MAX_ERR";
  case P_ARRAY_MIN_NEGATIVE: return "P_ARRAY_MIN_NEGATIVE";
  case P_ARRAY_MAX_NEGATIVE: return "P_ARRAY_MAX_NEGATIVE";
  case P_ARRAY_EXTRA_BEFORE_SEP: return "P_ARRAY_EXTRA_BEFORE_SEP";
  case P_ARRAY_EXTRA_BEFORE_TERM: return "P_ARRAY_EXTRA_BEFORE_TERM";
  case P_STRUCT_FIELD_ERR: return "P_STRUCT_FIELD_ERR";
  case P_STRUCT_EXTRA_BEFORE_SEP: return "P_STRUCT_EXTRA_BEFORE_SEP";
  case P_UNION_MATCH_ERR: return "P_UNION_MATCH_ERR";
  case P_ENUM_MATCH_ERR: return "P_ENUM_MATCH_ERR";
  case P_TYPEDEF_CONSTRAINT_ERR: return "P_TYPEDEF_CONSTRAINT_ERR";
  case P_AT_EOF: return "P_AT_EOF";
  case P_AT_EOR: return "P_AT_EOR";
  case P_EXTRA_BEFORE_EOR: return "P_EXTRA_BEFORE_EOR";
  case P_EOF_BEFORE_EOR: return "P_EOF_BEFORE_EOR";
  case P_COUNT_MAX_LIMIT: return "P_COUNT_MAX_LIMIT";
  case P_RANGE: return "P_RANGE";
  case P_INVALID_A_NUM: return "P_INVALID_A_NUM";
  case P_INVALID_E_NUM: return "P_INVALID_E_NUM";
  case P_INVALID_EBC_NUM: return "P_INVALID_EBC_NUM";
  case P_INVALID_BCD_NUM: return "P_INVALID_BCD_NUM";
  case P_INVALID_CHARSET: return "P_INVALID_CHARSET";
  case P_INVALID_WIDTH: return "P_INVALID_WIDTH";
  case P_CHAR_LIT_NOT_FOUND: return "P_CHAR_LIT_NOT_FOUND";
  case P_STR_LIT_NOT_FOUND: return "P_STR_LIT_NOT_FOUND";
  case P_REGEXP_NOT_FOUND: return "P_REGEXP_NOT_FOUND";
  case P_INVALID_REGEXP: return "P_INVALID_REGEXP";
  case P_WIDTH_NOT_AVAILABLE: return "P_WIDTH_NOT_AVAILABLE";
  case P_INVALID_TIMESTAMP: return "P_INVALID_TIMESTAMP";
  case P_INVALID_DATE: return "P_INVALID_DATE";
  case P_INVALID_TIME: return "P_INVALID_TIME";
  case P_INVALID_IP: return "P_INVALID_IP";
  case P_INVALID_IP_RANGE: return "P_INVALID_IP_RANGE";
  default: break;
  }
  return "*UNKNOWN_ERRCODE*";
}

Perror_t
PDCI_report_err(P_t *pads, int level, Ploc_t *loc,
		PerrCode_t errCode, const char *whatfn, const char *format, ...)
{
  Perror_fn   pdc_errorf;
  char       *severity = "Error";
  char       *msg      = "** unknown error code **";
  char       *infn, *tmpstr1, *tmpstr2, *tmpstr3;
  const char *unit;
  size_t      tmplen1, tmplen2, tmplen3;
  int         nullspan = 0;

  P_TRACE(pads->disc, "PDCI_report_err called");
#ifndef TRACE
  if ((pads->speclev || pads->disc->e_rep == PerrorRep_None) && P_GET_LEV(level) != P_LEV_FATAL) return P_OK;
#else
  if ((pads->disc->e_rep == PerrorRep_None) && P_GET_LEV(level) != P_LEV_FATAL) return P_OK;
  if (pads->speclev) {
    error(0, "*** SPECULATIVE MSG, NORMALLY IGNORED:");
  }
#endif
  if (!whatfn) {
    infn = "";
  } else {
    PDCI_sfstr_seek2zero(pads->tmp2);
    sfprintf(pads->tmp2, "[in %s]", whatfn);
    infn = PDCI_sfstr_use(pads->tmp2);
  }
  pdc_errorf = pads->disc->error_fn;
  if (P_GET_LEV(level) == P_LEV_FATAL) {
    severity = "FATAL error";
    if (!pdc_errorf) { /* need an error function anyway for fatal case */
      pdc_errorf = P_error;
    }
  } else if (!pdc_errorf) {
    return P_OK;
  }
  if (errCode == P_NO_ERR) {
    severity = "Note";
  }
  /* Any backwards span is treated as a null span */
  if (loc && ((loc->e.num < loc->b.num) || (loc->b.num == loc->e.num && loc->e.byte < loc->b.byte))) {
    nullspan = 1;
  }
  if (!(unit = P_io_read_unit(pads))) {
    unit = "";
  }
  PDCI_sfstr_seek2zero(pads->tmp1);
  if (pads->disc->e_rep == PerrorRep_Min) {
    if (loc) {
      pdc_errorf(NiL, level, "%s %s: %s %d byte %d: errCode %d",
		 severity, infn, unit, loc->b.num, loc->b.byte, errCode);
    } else {
      pdc_errorf(NiL, level, "%s %s: errCode %d", severity, infn, errCode);
    }
    return P_OK;
  }
  if (format && strlen(format)) {
    va_list ap;
    if (loc) {
      sfprintf(pads->tmp1, "%s %s: %s %d byte %d : ", severity, infn, unit, loc->b.num, loc->b.byte);
    } else {
      sfprintf(pads->tmp1, "%s %s: ", severity, infn);
    }
    va_start(ap, format);
    sfvprintf(pads->tmp1, format, ap);
    va_end(ap);
  } else {
    switch (errCode) {
    case P_NO_ERR:
      msg = "";
      break;
    case P_UNEXPECTED_ERR:
      msg = "XXX Unexpected error (should not happen)";
      break;
    case P_BAD_PARAM:
      msg = "Invalid argument value used in padsc library call";
      break;
    case P_SYS_ERR:
      msg = "System error";
      break;
    case P_CHKPOINT_ERR:
      msg = "Checkpoint error (misuse of padsc IO checkpoint facility)";
      break;
    case P_COMMIT_ERR:
      msg = "Commit error (misuse of padsc IO checkpoint facility)";
      break;
    case P_RESTORE_ERR:
      msg = "Restore error (misuse of padsc IO checkpoint facility)";
      break;
    case P_ALLOC_ERR:
      msg = "Memory alloc failure (out of space)";
      break;
    case P_PANIC_SKIPPED:
      msg = "Data element parsing skipped: in panic mode due to earlier error(s)";
      break;
    case P_USER_CONSTRAINT_VIOLATION:
      msg = "User constraint violation";
      break;
    case P_MISSING_LITERAL:
      msg = "Missing literal";
      break;
    case P_ARRAY_ELEM_ERR:
      msg = "Array element error";
      break;
    case P_ARRAY_SEP_ERR:
      msg = "Array seperator error";
      break;
    case P_ARRAY_TERM_ERR:
      msg = "Array terminator error";
      break;
    case P_ARRAY_SIZE_ERR:
      msg = "Array size error";
      break;
    case P_ARRAY_SEP_TERM_SAME_ERR:
      msg = "Array terminator/separator value error";
      break;
    case P_ARRAY_USER_CONSTRAINT_ERR:
      msg = "Array user constraint violation";
      break;
    case P_ARRAY_MIN_BIGGER_THAN_MAX_ERR:
      msg = "Array min bigger than array max";
      break;
    case P_ARRAY_MIN_NEGATIVE:
      msg = "Negative number used for array min";
      break;
    case P_ARRAY_MAX_NEGATIVE:
      msg = "Negative number used for array max";
      break;
    case P_ARRAY_EXTRA_BEFORE_SEP:
      msg = "Unexpected extra data before array element separator";
      break;
    case P_ARRAY_EXTRA_BEFORE_TERM:
      msg = "Unexpected extra data before array element terminator";
      break;
    case P_STRUCT_EXTRA_BEFORE_SEP:
      msg = "Unexpected extra data before field separator in struct";
      break;
    case P_STRUCT_FIELD_ERR:
      msg = "Structure field error";
      break;
    case P_UNION_MATCH_ERR:
      msg = "Union match failure";
      break;
    case P_ENUM_MATCH_ERR:
      msg = "Enum match failure";
      break;
    case P_TYPEDEF_CONSTRAINT_ERR:
      msg = "Typedef constraint error";
      break;
    case P_AT_EOF:
      msg = "Unexpected end of file (field too short?)";
      break;
    case P_AT_EOR:
      msg = "Unexpected end of record (field too short?)";
      break;
    case P_EXTRA_BEFORE_EOR:
      msg = "Unexpected extra data before EOR";
      break;
    case P_EOF_BEFORE_EOR:
      msg = "EOF encountered prior to expected EOR";
      break;
    case P_COUNT_MAX_LIMIT:
      msg = "cont_max limit hit before count routine found other terminating condition";
      break;
    case P_RANGE:
      msg = "Number out of range error";
      break;
    case P_INVALID_A_NUM:
      msg = "Invalid ASCII character encoding of a number";
      break;
    case P_INVALID_E_NUM:
      msg = "Invalid EBCDIC character encoding of a number";
      break;
    case P_INVALID_EBC_NUM:
      msg = "Invalid EBCDIC numeric encoding";
      break;
    case P_INVALID_BCD_NUM:
      msg = "Invalid BCD numeric encoding";
      break;
    case P_INVALID_CHARSET:
      msg = "Invalid Pcharset value";
      break;
    case P_INVALID_WIDTH:
      msg = "Invalid fixed width arg: does not match width of Pstring arg";
      break;
    case P_CHAR_LIT_NOT_FOUND:
      msg = "Expected character literal not found";
      break;
    case P_STR_LIT_NOT_FOUND:
      msg = "Expected string literal not found";
      break;
    case P_REGEXP_NOT_FOUND:
      msg = "Match for regular expression not found";
      break;
    case P_INVALID_REGEXP:
      msg = "Invalid regular expression";
      break;
    case P_WIDTH_NOT_AVAILABLE:
      msg = "Specified width not available (EOR/EOF encountered)";
      break;
    case P_INVALID_TIMESTAMP:
      msg = "Invalid timestamp";
      break;
    case P_INVALID_DATE:
      msg = "Invalid date";
      break;
    case P_INVALID_TIME:
      msg = "Invalid time";
      break;
    case P_INVALID_IP:
      msg = "Invalid IP address";
      break;
    case P_INVALID_IP_RANGE:
      msg = "Invalid IP address, numbers must be between 0 and 255";
      break;
    default:
      sfprintf(pads->tmp1, "** unknown error code: %d **", errCode);
      msg = "";
      break;
    }
    if (loc) {
      if (loc->b.num < loc->e.num) {
	sfprintf(pads->tmp1, "%s %s: from %s %d byte %d to %s %d byte %d: %s ",
		 severity, infn,
		 unit, loc->b.num, loc->b.byte, 
		 unit, loc->e.num, loc->e.byte,
		 msg);
      } else if (nullspan) {
	sfprintf(pads->tmp1, "%s %s: at %s %d just before byte %d: %s",
		 severity, infn,
		 unit, loc->b.num, loc->b.byte,
		 msg);
      } else if (loc->b.byte == loc->e.byte) {
	sfprintf(pads->tmp1, "%s %s: at %s %d at byte %d : %s ",
		 severity, infn,
		 unit, loc->b.num, loc->b.byte,
		 msg);
      } else {
	sfprintf(pads->tmp1, "%s %s: at %s %d from byte %d to byte %d: %s ",
		 severity, infn,
		 unit, loc->b.num, loc->b.byte, loc->e.byte,
		 msg);
      }
    } else {
      sfprintf(pads->tmp1, "%s %s: %s ", severity, infn, msg);
    }
  }
  if (loc && (pads->disc->e_rep == PerrorRep_Max)) {
    Pio_elt_t *elt1, *elt2;
    if (loc->b.num < loc->e.num) {
      if (P_OK == PDCI_io_getElt(pads, loc->b.num, &elt1)) {
	sfprintf(pads->tmp1, "\n[%s %d]", unit, loc->b.num);
	if (elt1->len == 0) {
	  sfprintf(pads->tmp1, "(**EMPTY**)>>>");
	} else {
	  tmplen1 = loc->b.byte - 1;
	  tmplen2 = elt1->len - tmplen1;
	  tmpstr1 = P_fmt_cstr_n((char*)elt1->begin,           tmplen1);
	  tmpstr2 = P_fmt_cstr_n((char*)elt1->begin + tmplen1, tmplen2);
	  sfprintf(pads->tmp1, "%s>>>%s", tmpstr1, tmpstr2);
	}
      }
      if (P_OK == PDCI_io_getElt(pads, loc->e.num, &elt2)) {
	if (!elt1) {
	  sfprintf(pads->tmp1, "\n[%s %d]: ... >>>(char pos %d) ...",
		   unit, loc->b.num, loc->b.byte);
	}
	sfprintf(pads->tmp1, "\n[%s %d]", unit, loc->e.num);
	if (elt2->len == 0) {
	  sfprintf(pads->tmp1, "(**EMPTY**)<<<");
	} else {
	  tmplen1 = loc->e.byte;
	  tmplen2 = elt2->len - tmplen1;
	  tmpstr1 = P_fmt_cstr_n((char*)elt2->begin,           tmplen1);
	  tmpstr2 = P_fmt_cstr_n((char*)elt2->begin + tmplen1, tmplen2);
	  sfprintf(pads->tmp1, "%s<<<%s", tmpstr1, tmpstr2);
	}
      }
    } else { /* same elt */
      if (P_OK == PDCI_io_getElt(pads, loc->b.num, &elt1)) {
	sfprintf(pads->tmp1, "\n[%s %d]", unit, loc->b.num);
	if (elt1->len == 0) {
	  sfprintf(pads->tmp1, ">>>(**EMPTY**)<<<");
	} else if (nullspan) {
	  tmplen1 = loc->b.byte - 1;
	  tmplen2 = elt1->len - tmplen1;
	  tmpstr1 = P_fmt_cstr_n((char*)elt1->begin,           tmplen1);
	  tmpstr2 = P_fmt_cstr_n((char*)elt1->begin + tmplen1, tmplen2);
	  sfprintf(pads->tmp1, "%s>>><<<%s", tmpstr1, tmpstr2);
	} else {
	  tmplen1 = loc->b.byte - 1;
	  tmplen3 = elt1->len - loc->e.byte;
	  tmplen2 = elt1->len - tmplen1 - tmplen3;
	  tmpstr1 = P_fmt_cstr_n((char*)elt1->begin,                     tmplen1);
	  tmpstr2 = P_fmt_cstr_n((char*)elt1->begin + tmplen1,           tmplen2);
	  tmpstr3 = P_fmt_cstr_n((char*)elt1->begin + tmplen1 + tmplen2, tmplen3);
	  sfprintf(pads->tmp1, "%s>>>%s<<<%s", tmpstr1, tmpstr2, tmpstr3);
	}
      }
    }
  }
  pdc_errorf(NiL, level, "%s", PDCI_sfstr_use(pads->tmp1));
  return P_OK;
}

/* ================================================================================ */
/* INTERNAL IO FUNCTIONS */

Perror_t
PDCI_io_install_io(P_t *pads, Sfio_t *io)
{
  PDCI_stkElt_t    *tp        = &(pads->stack[0]);
  Pio_elt_t     *next_elt;
  Void_t           *buf;

  /* XXX_TODO handle case where pads->io is already set, io_discipline already open, etc */
  pads->io = io;
  /* tell sfio to use pads->sfbuf but only let it know about sizeof(sfbuf)-1 space */
  buf = sfsetbuf(pads->io, (Void_t*)1, 0);
  if (!buf) {
    sfsetbuf(pads->io, pads->sfbuf, 1024 * 1024);
  } else if (buf == (Void_t*)pads->sfbuf) {
    /* P_WARN(pads->disc, "XXX_REMOVE pads->sfbuf has already been installed so not installing it again"); */
  } else {
    /* P_WARN(pads->disc, "XXX_REMOVE An unknown buffer has already been installed so not installing pads->sfbuf\n"
                 "  (could be due to use of sfungetc)"); */
  }
  /* AT PRESENT we only support switching io at a very simply boundary:
   *    1. no checkpoint established
   *    2. not performing a speculative read (redundant based on 1)
   *    3. no nested internal calls in progress
   *    ...
   */
  if (P_SOME_ELTS(pads->head)) {
    P_FATAL(pads->disc, "Internal error: new io is being installed when pads->head list is non-empty\n"
	      "Should not happen if IO discipline close is working properly");
  }
  if (pads->top != 0) {
    P_FATAL(pads->disc, "Switching io during IO checkpoint not supported yet");
  }
  if (pads->speclev != 0) {
    P_FATAL(pads->disc, "Switching io during speculative read not supported yet");
  }
  if (pads->inestlev != 0) {
    P_FATAL(pads->disc, "Switching io during internal call nesting not supported yet");
  }

  /* open IO discipline */
  if (P_ERR == pads->disc->io_disc->sfopen_fn(pads, pads->disc->io_disc, pads->io, pads->head)) {
    return P_ERR;
  }
  /* perform first read */
  if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, 0, &next_elt)) {
    tp->elt = P_LAST_ELT(pads->head); /* IO disc may have added eof elt */
    tp->remain = 0;
    return P_ERR;
  }
  tp->elt = P_FIRST_ELT(pads->head);
  if (tp->elt == pads->head || tp->elt != next_elt) {
    P_FATAL(pads->disc, "Internal error : IO read function failure in PDCI_io_install_io");
  }
  tp->remain = tp->elt->len;
  return P_OK;
}

/* ================================================================================ */
/* PURELY INTERNAL IO FUNCTIONS */

#if 0
XXX_REMOVE
Perror_t
PDCI_io_need_K_bytes(P_t *pads, size_t K,
		     Pbyte **b_out, Pbyte **e_out,
		     int *bor_out, int *eor_out, int *eof_out)
{
  PDCI_stkElt_t    *tp       = &(pads->stack[pads->top]);
  Pio_elt_t     *elt      = tp->elt;
  Pio_elt_t     *keep_elt;
  Pio_elt_t     *next_elt;
  size_t            bytes;

  P_TRACE(pads->disc, "PDCI_io_need_K_bytes called");

  bytes      = tp->remain;
  (*bor_out) = (elt->bor && (bytes == elt->len));

  while (!(elt->eor|elt->eof) && (bytes < K)) {
    /* try for more bytes */
    if (elt->next != pads->head) { /* the easy case */
      elt = elt->next;
      bytes += elt->len;
      continue;
    }
    /* elt->next is pads->head, getting more bytes requires use of read_fn */
    keep_elt = pads->stack[0].elt;
    if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
      goto any_err;
    }
#ifndef NDEBUG
    if (elt->next != next_elt || next_elt == pads->head) { /* should not happen */
      P_FATAL(pads->disc, "Internal error, PDCI_io_need_K_bytes observed incorrect read_fn behavior");
      goto any_err;
    }
#endif
    elt = elt->next;
    bytes += elt->len;
  }
  /* either we hit eor or eof, or we found >= K bytes */
  (*b_out) = (tp->elt->end - tp->remain);
  if (bytes > K) {
    (*e_out)       = (*b_out) + K;
    (*eor_out)     = 0;
    (*eof_out)     = 0;
  } else {
    (*e_out)       = elt->end;
    (*eor_out)     = elt->eor;
    (*eof_out)     = elt->eof;
  }
  return P_OK;

 any_err:
   /* Error eof case.  Since K bytes not found, may as well always return P_ERR even if bytes > 0 */
  (*b_out)     = (tp->elt->end - tp->remain);
  (*e_out)     = elt->end;
  (*eof_out)   = 1;
  (*eor_out)   = 0;
  return P_ERR;
}
#endif

#if 0
Perror_t
PDCI_io_need_some_bytes(P_t *pads, PDCI_goal_t goal, size_t specific,
			Pbyte **b_out, Pbyte **e_out, Pbyte **g_out,
			int *bor_out, int *eor_out, int *eof_out)
{
  PDCI_stkElt_t    *tp       = &(pads->stack[pads->top]);
  Pio_elt_t     *elt      = tp->elt;
  Pio_elt_t     *keep_elt;
  Pio_elt_t     *next_elt;
  size_t            bytes, goal_bytes;
  int               soft_goal;

  P_TRACE(pads->disc, "PDCI_io_need_some_bytes called");

  goal_bytes = 0; /* goal is end-of-record */
  soft_goal  = 0; /* set to 1 only if we are forced to use a BUILTIN goal */
  switch (goal)
    {
    case PDCI_goal_match:
      goal_bytes = pads->disc->match_max;
      if (!goal_bytes && !pads->disc->io_disc->rec_based) {
	goal_bytes = P_BUILTIN_MATCH_MAX;
	soft_goal  = 1;
      }
      break;
    case PDCI_goal_scan:
      goal_bytes = pads->disc->scan_max;
      if (!goal_bytes && !pads->disc->io_disc->rec_based) {
	goal_bytes = P_BUILTIN_SCAN_MAX;
	soft_goal  = 1;
      }
      break;
    case PDCI_goal_panic:
      goal_bytes = pads->disc->panic_max;
      if (!goal_bytes && !pads->disc->io_disc->rec_based) {
	goal_bytes = P_BUILTIN_PANIC_MAX;
	soft_goal  = 1;
      }
      break;
    case PDCI_goal_numeric:
      goal_bytes = pads->disc->numeric_max;
      if (!goal_bytes && !pads->disc->io_disc->rec_based) {
	goal_bytes = P_BUILTIN_NUMERIC_MAX;
	soft_goal  = 1;
      }
      break;
    case PDCI_goal_specific:
      if (specific <= 0) return P_ERR;
      goal_bytes = specific;
      break;
    }

  bytes      = tp->remain;
  (*bor_out) = (elt->bor && (bytes == elt->len));

  while (!(elt->eor|elt->eof) && (soft_goal || !goal_bytes || bytes < goal_bytes)) {
    /* try for more bytes */
    if (elt->next != pads->head) { /* the easy case */
      elt = elt->next;
      bytes += elt->len;
      continue;
    }
    /* elt->next is pads->head, getting more bytes requires use of read_fn */
    /* if soft goal and it has been hit, stop now */
    if (soft_goal && (bytes >= goal_bytes)) break;
    keep_elt = pads->stack[0].elt;
    if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
      goto any_err;
    }
#ifndef NDEBUG
    if (elt->next != next_elt || next_elt == pads->head) { /* should not happen */
      P_FATAL(pads->disc, "Internal error, PDCI_io_need_some_bytes observed incorrect read_fn behavior");
      goto any_err;
    }
#endif
    elt = elt->next;
    bytes += elt->len;
  }
  /* either we hit eor or eof, or we found >= goal_bytes bytes */
  if (soft_goal && (bytes > goal_bytes)) {
    /* adjust goal upwards rather than not returning all the bytes we found */
    goal_bytes = bytes;
  }
  (*b_out) = (tp->elt->end - tp->remain);
  if (!goal_bytes || elt->eor) {
    /* Goal was eor and we got eor|eof, OR goal was set but we found eor first,
     * which is also acceptable as a goal. Either way, include all bytes found.
     * Since this is record-based case, set (*g_out) > (*e_out) if !eor
     */
    (*e_out)       = elt->end;
    if (elt->eor) {
      (*g_out)     = elt->end;
    } else if (goal_bytes) {
      (*g_out)     = (*b_out) + goal_bytes; /* Note (*g_out) > (*e_out) */
    } else {
      (*g_out)     = elt->end + PDCI_GOAL_HUGE; /* Note that (*g_out) > (*e_out) */
    }
    (*eor_out)     = elt->eor;
    (*eof_out)     = elt->eof;
  } else {
    /* had hard or soft goal_bytes (> 0) and eor not found.  eof may have been found */
    (*eor_out)     = 0;
    if (bytes > goal_bytes) {
      /* found more than enough (end will be prior to any eof byte) */
      (*e_out)     = (*b_out) + goal_bytes;
      (*g_out)     = (*e_out);
      (*eof_out)   = 0;
    } else if (bytes == goal_bytes) {
      /* found exactly enough (not sure if end is at eof) */
      (*e_out)     = (*b_out) + goal_bytes;
      (*g_out)     = (*e_out);
      (*eof_out)   = ((*e_out) == elt->end) ? elt->eof : 0;
    } else {
      /* did not find enough (must have hit eof) */
      (*e_out)     = elt->end;
      (*g_out)     = (*b_out) + goal_bytes; /* Note (*g_out) > (*e_out) */
      (*eof_out)   = 1;
    }
  }
  return P_OK;

 any_err:
   /* Error eof case.  Since goal not found, may as well return P_ERR even if bytes > 0 */
  (*b_out)         = (tp->elt->end - tp->remain);
  (*e_out)         = elt->end;
  if (goal_bytes) {
    (*g_out)       = (*b_out) + goal_bytes; /* Note (*g_out) > (*e_out) */
  } else {
    (*g_out)       = elt->end + PDCI_GOAL_HUGE; /* Note (*g_out) > (*e_out) */
  }
  (*eof_out)       = 1;
  (*eor_out)       = 0;
  return P_ERR;
}
#endif

Perror_t
PDCI_io_need_rec_bytes(P_t *pads, int skip_rec,
		       Pbyte **b_out, Pbyte **e_out,
		       int *eor_out, int *eof_out, size_t *skipped_bytes_out)
{
  PDCI_stkElt_t    *tp          = &(pads->stack[pads->top]);
  Pio_elt_t     *elt;
  Pio_elt_t     *next_elt;
  Pio_elt_t     *keep_elt;

#ifndef NDEBUG
  if (!pads->disc->io_disc->rec_based) {
    P_FATAL(pads->disc, "Internal error, PDCI_io_need_rec_bytes called on non-rec-based IO discipline");
  }
#endif
  (*skipped_bytes_out) = 0;
  if (skip_rec) {
    /* assumes PDCI_io_need_rec_bytes already called once and there is an elt with eor == 1 and eof == 0*/
    while (!tp->elt->eor) {
#ifndef NDEBUG
      if (tp->elt->eof || tp->elt->next == pads->head) {
	P_FATAL(pads->disc, "Internal error, PDCI_io_need_rec_bytes called in bad start state");
      }
#endif
      (*skipped_bytes_out) += tp->remain;
      tp->elt = tp->elt->next;
      tp->remain = tp->elt->len;
    }
    /* found eor elt */
#ifndef NDEBUG
    if (tp->elt->eof) {
      P_FATAL(pads->disc, "Internal error, PDCI_io_need_rec_bytes called in bad start state");
    }
#endif
    (*skipped_bytes_out) += tp->remain;
    tp->remain = 0; /* advance past rec */
    /* move top to following elt */
    /* (also moves bot when top==bot [pads->top == 0]) */
    if (tp->elt->next != pads->head) {
      tp->elt = tp->elt->next;
    } else {
      /* need to read another elt using IO discipline */
      keep_elt = pads->stack[0].elt;
      if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
	/* read problem, return zero length, !eor, eof */
	tp->elt      = P_LAST_ELT(pads->head); /* IO disc may have added eof elt */
	tp->remain   = 0;
	(*b_out)     = tp->elt->end;
	(*e_out)     = (*b_out);
	(*eor_out)   = 0;
	(*eof_out)   = 1;
	return P_ERR;
      }
#ifndef NDEBUG
      if (next_elt == pads->head) { /* should not happen */
	P_FATAL(pads->disc, "Internal error, P_io_need_rec_bytes observed incorrect read_fn behavior");
      }
#endif
      tp->elt = next_elt;
    }
    tp->remain = tp->elt->len;
    /* record has been skipped, tp->elt is start loc for requested bytes */
  } /* else do not skip record, tp->elt is still start lock for requested bytes */

  /* find elt with eor or eof marker, starting with tp->elt */
  elt = tp->elt;
  while (!(elt->eor|elt->eof)) {
    if (elt->next == pads->head) {
      /* need to read another elt using IO discipline */
      keep_elt = pads->stack[0].elt;
      if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
	/* read problem, return zero length, !eor, eof */
	(*b_out)     = elt->end;
	(*e_out)     = (*b_out);
	(*eor_out)   = 0;
	(*eof_out)   = 1;
	return P_ERR;
      }
#ifndef NDEBUG
      if (elt->next != next_elt || next_elt == pads->head) { /* should not happen */
	P_FATAL(pads->disc, "Internal error, PDCI_io_need_rec_bytes observed incorrect read_fn behavior");
      }
#endif
    }
    elt = elt->next; /* keep looking for eor|eof */
  }
  /* found eor|eof elt */
  (*b_out)     = (tp->elt->end - tp->remain);
  (*e_out)     = elt->end;
  (*eor_out)   = elt->eor;
  (*eof_out)   = elt->eof;
  return P_OK;
}

Perror_t
PDCI_io_forward(P_t *pads, size_t num_bytes)
{
  PDCI_stkElt_t    *tp        = &(pads->stack[pads->top]);
  size_t todo                 = num_bytes;
  Pio_elt_t     *keep_elt;
  Pio_elt_t     *next_elt;

  P_TRACE(pads->disc, "PDCI_io_forward called");
  /* should be able to move forward without reading new bytes or advancing past EOR/EOF */
  while (todo > 0) {
    if (tp->remain == 0) {
      if (tp->elt->eor|tp->elt->eof) {
	P_FATAL(pads->disc, "Internal error, PDCI_io_forward hit EOR OR EOF");
      }
      if (tp->elt->next == pads->head) {
	P_FATAL(pads->disc, "Internal error, PDCI_io_forward would need to read bytes from io stream");
	return P_ERR;
      }
      tp->elt = tp->elt->next;
      tp->remain = tp->elt->len;
      continue;
    }
    if (todo <= tp->remain) {
      tp->remain -= todo;
      todo = 0;
      break;
    }
    /* current IO rec gets us partway */
    todo -= tp->remain;
    tp->remain = 0;
  }
  /* success */
  if (tp->remain || (tp->elt->eor|tp->elt->eof)) {
    return P_OK;
  }
  /* at end of a non-EOR, non-EOF elt: advance now */
  if (tp->elt->next != pads->head) {
    tp->elt = tp->elt->next;
    tp->remain = tp->elt->len;
    return P_OK;
  }
  /* need to read some data -- use IO disc read_fn */
  keep_elt = pads->stack[0].elt;
  if (P_ERR == pads->disc->io_disc->read_fn(pads, pads->disc->io_disc, keep_elt, &next_elt)) {
    tp->elt = P_LAST_ELT(pads->head); /* IO disc may have added eof elt */
    tp->remain = 0;
    return P_ERR;
  }
#ifndef NDEBUG
  if (next_elt == pads->head) { /* should not happen */
    P_FATAL(pads->disc, "Internal error, PDCI_io_forward observed incorrect read_fn behavior");
    return P_ERR;
  }
#endif
  tp->elt = next_elt;
  tp->remain = tp->elt->len;
  return P_OK;
}

Perror_t
PDCI_io_getElt(P_t *pads, size_t num, Pio_elt_t **elt_out) {
  Pio_elt_t *elt;

  P_TRACE(pads->disc, "PDCI_io_getElt called");
  PDCI_NULLPARAM_CHECK("PDCI_io_getElt", elt_out);
  for (elt = P_FIRST_ELT(pads->head); elt != pads->head; elt = elt->next) {
    if (elt->num == num) {
      (*elt_out) = elt;
      return P_OK;
    }
  }
  return P_ERR;
}

#if P_CONFIG_WRITE_FUNCTIONS > 0
Pbyte*
PDCI_io_write_start(P_t *pads, Sfio_t *io, size_t *buf_len, int *set_buf, const char *whatfn)
{
  Pbyte  *buf;
  ssize_t    n, nm;

  P_TRACE1(pads->disc, "PDCI_io_write_start called for size_t: %ld", ((long)*buf_len) );
  if (!sfsetbuf(io, (Void_t *)1, 0))  {
    P_TRACE1(pads->disc, "PDCI_io_write_start sets buf with size: %ld", ((long)pads->outbuf_len) );
    sfsetbuf(io, pads->outbuf, pads->outbuf_len);
    (*set_buf) = 1;
  } else {
    (*set_buf) = 0;
  }
  n = (*buf_len);
  nm = -1 * n;
  if (!(buf = (Pbyte*)sfreserve(io, nm, SF_LOCKR))) {
    PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_IO_ERR, whatfn, "sfreserve failed");
    if (*set_buf) {
      sfsetbuf(io, (Void_t*)0, 0); /* undo sfsetbuf */
    }
    return 0;
  }
  nm = sfvalue(io);
  P_TRACE1(pads->disc, "PDCI_io_write_start sfreserve returns: %ld", ((long)nm) );
  if (nm < (*buf_len)) {
    error(0, "tried to reserve %ld bytes, got %ld bytes", (long) *buf_len, (long) nm);
    PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_IO_ERR, whatfn, "sfreserve returned insufficient bytes");
    sfwrite(io, (Void_t*)buf, 0); /* release sfreserve */
    if (*set_buf) {
      sfsetbuf(io, (Void_t*)0, 0); /* undo sfsetbuf */
    }
    return 0;
  }
  if (nm > (*buf_len)) {
    (*buf_len) = nm;
  }
  return buf;
}

ssize_t
PDCI_io_write_commit(P_t *pads, Sfio_t *io, Pbyte *buf, int set_buf, size_t num_bytes, const char *whatfn)
{
  ssize_t n;

  P_TRACE(pads->disc, "PDCI_io_write_commit called");
  n = sfwrite(io, (Void_t*)buf, num_bytes);
  if (set_buf) {
    sfsetbuf(io, (Void_t*)0, 0); /* undo sfsetbuf */
  }
  if (n != num_bytes) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;
}

void
PDCI_io_write_abort(P_t *pads, Sfio_t *io, Pbyte *buf, int set_buf, const char *whatfn)
{
  P_TRACE(pads->disc, "PDCI_io_write_abort called");
  sfwrite(io, (Void_t*)buf, 0); /* release sfreserve */
  if (set_buf) {
    sfsetbuf(io, (Void_t*)0, 0); /* undo sfsetbuf */
  }
}

ssize_t
PDCI_io_rec_write2io(P_t *pads, Sfio_t *io, Pbyte *buf, size_t rec_data_len, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;
  Pbyte      *iobuf, *iobuf_cursor;
  size_t         num_bytes, iobuf_len;
  int            set_buf = 0;
  ssize_t        tlen;

  P_TRACE(pads->disc, "PDCI_io_rec_write2io called");
  if (!iodisc->rec_based) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support records to use this function", whatfn);
    return -1;
  }
  num_bytes = rec_data_len + iodisc->rec_obytes;
  iobuf_len = num_bytes + iodisc->rec_cbytes + 1;
  iobuf = PDCI_io_write_start(pads, io, &iobuf_len, &set_buf, whatfn);
  if (!iobuf) {
    /* write_start reported the error */
    /* don't have to abort because write_start failed */
    return -1;
  }
  iobuf_cursor = iobuf + iodisc->rec_obytes;
  memcpy(iobuf_cursor, buf, rec_data_len);
  iobuf_cursor += rec_data_len;
  if (-1 == (tlen = iodisc->rec_close_buf_fn(pads, iodisc, iobuf_cursor, iobuf, num_bytes))) {
    P_WARN1(pads->disc, "%s: internal error, failed to write record", whatfn);
    PDCI_io_write_abort(pads, io, iobuf, set_buf, whatfn);
    return -1;
  }
  iobuf_len = num_bytes + tlen;
  return PDCI_io_write_commit(pads, io, iobuf, iobuf_len, set_buf, whatfn);
}

ssize_t
PDCI_io_rec_open_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rec_open_write2buf called");
  if (!iodisc->rec_based) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support records to use this function", whatfn);
    return -1;
  }
  if (buf_len < iodisc->rec_obytes) {
    (*buf_full) = 1;
    return -1;
  }
  return iodisc->rec_obytes;
}

ssize_t
PDCI_io_rec_close_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			    Pbyte *rec_start, size_t num_bytes, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rec_close_write2buf called");
  if (!iodisc->rec_based) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support records to use this function", whatfn);
    return -1;
  }
  if (buf_len < iodisc->rec_cbytes) {
    (*buf_full) = 1;
    return -1;
  }
  return iodisc->rec_close_buf_fn(pads, iodisc, buf, rec_start, num_bytes);
}

ssize_t
PDCI_io_rec_open_write2io(P_t *pads, Sfio_t *io, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rec_open_write2io called");
  if (!iodisc->rec_based) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support records to use this function", whatfn);
    return -1;
  }
  return iodisc->rec_obytes;
}

ssize_t
PDCI_io_rec_close_write2io(P_t *pads, Sfio_t *io, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rec_close_write2io called");
  if (!iodisc->rec_based) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support records to use this function", whatfn);
    return -1;
  }

  return iodisc->rec_close_io_fn(pads, iodisc, io);
}

ssize_t
PDCI_io_rblk_write2io(P_t *pads, Sfio_t *io, Pbyte *buf, size_t blk_data_len, Puint32 num_recs, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;
  Pbyte      *iobuf, *iobuf_cursor;
  size_t         num_bytes, iobuf_len;
  int            set_buf = 0;
  ssize_t        tlen;

  P_TRACE(pads->disc, "PDCI_io_rblk_write2io called");
  if (!iodisc->has_rblks) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support record blocks to use this function", whatfn);
    return -1;
  }
  num_bytes = blk_data_len + iodisc->blk_obytes;
  iobuf_len = num_bytes + iodisc->blk_cbytes + 1;
  iobuf = PDCI_io_write_start(pads, io, &iobuf_len, &set_buf, whatfn);
  if (!iobuf) {
    /* write_start reported the error */
    /* don't have to abort because write_start failed */
    return -1;
  }
  iobuf_cursor = iobuf + iodisc->blk_obytes;
  memcpy(iobuf_cursor, buf, blk_data_len);
  iobuf_cursor += blk_data_len;
  if (-1 == (tlen = iodisc->blk_close_fn(pads, iodisc, iobuf_cursor, iobuf, num_bytes, num_recs))) {
    P_WARN1(pads->disc, "%s: internal error, failed to write block of records", whatfn);
    PDCI_io_write_abort(pads, io, iobuf, set_buf, whatfn);
    return -1;
  }
  iobuf_len = num_bytes + tlen;
  return PDCI_io_write_commit(pads, io, iobuf, iobuf_len, set_buf, whatfn);
}

ssize_t
PDCI_io_rblk_open_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rblk_open_write2buf called");
  if (!iodisc->has_rblks) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support record blocks to use this function", whatfn);
    return -1;
  }
  if (buf_len < iodisc->blk_obytes) {
    (*buf_full) = 1;
    return -1;
  }
  return iodisc->blk_obytes;
}

ssize_t
PDCI_io_rblk_close_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			     Pbyte *blk_start, size_t num_bytes, Puint32 num_recs, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rblk_close_write2buf called");
  if (!iodisc->has_rblks) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support record blocks to use this function", whatfn);
    return -1;
  }
  if (buf_len < iodisc->blk_cbytes) {
    (*buf_full) = 1;
    return -1;
  }
  return iodisc->blk_close_fn(pads, iodisc, buf, blk_start, num_bytes, num_recs);
}

ssize_t
PDCI_io_rec_fmt2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		    Pcharset char_set, const char *whatfn)
{
  Pio_disc_t *iodisc = pads->disc->io_disc;

  P_TRACE(pads->disc, "PDCI_io_rec_fmt2buf called");
  if (!iodisc->rec_based) {
    P_WARN1(pads->disc, "%s: pads->disc->io_disc must support records to use this function", whatfn);
    return -1;
  }
  if (buf_len < 1) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      *buf = P_ASCII_NEWLINE;
      break;
    case Pcharset_EBCDIC:
      *buf = P_EBCDIC_NEWLINE;
      break;
    default:
      goto invalid_charset;
    }
  return 1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;
}

#endif /* P_CONFIG_WRITE_FUNCTIONS */

/* ================================================================================ */
/* CHARSET INTERNAL SCAN FUNCTIONS */

#if P_CONFIG_READ_FUNCTIONS > 0

Perror_t
PDCI_char_lit_scan1(P_t *pads, Pchar f, int eat_f, int panic,
		    size_t *offset_out, Pcharset char_set, const char *whatfn)
{
  Pbyte       *p1;
  PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic);

  PDCI_IODISC_1P_CHECKS(whatfn, offset_out);
  P_TRACE5(pads->disc, "PDCI_char_lit_scan1 args: f %s eat_f %d panic %d, char_set = %s, whatfn = %s",
	     P_qfmt_char(f), eat_f, panic, Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      f = P_mod_ae_tab[(int)f]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  (*offset_out) = 0;
  PDCI_IO_NEED_BYTES(return P_ERR);
  p1 = begin;
  while (1) {
    if (p1 == end) return P_ERR;
    /* p1 < end */
    if (f == (*p1)) {
      (*offset_out) = (p1-begin);
      if (eat_f) {
	p1++; /* advance beyond char found */
	PDCI_ECHO_TOKEN(pads, "Char_lit",begin,p1);
      }
      PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
      return P_OK;
    }
    p1++;
  }

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;
}

Perror_t
PDCI_char_lit_scan2(P_t *pads, Pchar f, Pchar s, int eat_f, int eat_s, int panic,
		    int *f_found_out, size_t *offset_out, Pcharset char_set, const char *whatfn)
{
  Pbyte       *p1;
  PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic);

  PDCI_IODISC_2P_CHECKS(whatfn, f_found_out, offset_out);
  P_TRACE7(pads->disc, "PDCI_char_lit_scan2 args: f %s s %s eat_f %d eat_s %d, panic %d, char_set = %s, whatfn = %s",
	     P_qfmt_char(f), P_qfmt_char(s), eat_f, eat_s, panic, Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      f = P_mod_ae_tab[(int)f]; /* convert to EBCDIC char */
      s = P_mod_ae_tab[(int)s]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  (*offset_out) = 0;
  PDCI_IO_NEED_BYTES(return P_ERR);
  p1 = begin;
  while (1) {
    if (p1 == end) return P_ERR;
    /* p1 < end */
    if (f == (*p1)) {
      (*f_found_out) = 1;
      (*offset_out) = (p1-begin);
      if (eat_f) {
	p1++; /* advance beyond char found */
	PDCI_ECHO_TOKEN(pads, "Char_lit",begin,p1);
      }
      PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
      return P_OK;
    }
    if (s == (*p1)) {
      (*f_found_out) = 0;
      (*offset_out) = (p1-begin);
      if (eat_s) {
	p1++; /* advance beyond char found */
	PDCI_ECHO_TOKEN(pads, "Char_lit",begin,p1);
      }
      PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
      return P_OK;
    }
    p1++;
  }

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;
}

Perror_t
PDCI_str_lit_scan1(P_t *pads, const Pstring *f,
		   int eat_f, int panic,
		   size_t *offset_out, Pcharset char_set,
		   const char *whatfn) 
{
  Pstring      *tmp_f = (Pstring*)f;
  size_t        width;
  Pbyte        *p1;
  PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic);

  PDCI_IODISC_2P_CHECKS(whatfn, f, offset_out);

  P_TRACE5(pads->disc, "PDCI_str_lit_scan args: f = %s eat_f = %d panic %d, char_set = %s, whatfn = %s",
	     P_qfmt_str(f), eat_f, panic, Pcharset2str(char_set), whatfn);
  (*offset_out) = 0;
  width = f->len;
#ifndef NDEBUG
  if (width == 0) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: empty find string specified", whatfn);
    }
    return P_ERR;
  }
#endif
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_f = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_f, f->str, width);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_BYTES(return P_ERR);
  p1 = begin;
  while (1) {
    if (p1 + width > end) return P_ERR;
    /* p1 + width <= end */
    if (strncmp((char*)p1, tmp_f->str, width) == 0) {
      (*offset_out) = (p1-begin);
      if (eat_f) {
	p1 += width; /* advance beyond f */
	PDCI_ECHO_TOKEN(pads, "Str_lit",begin,p1);
      }
      PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
      return P_OK;
    }
    p1++;
  }

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return P_ERR;
}

Perror_t
PDCI_str_lit_scan2(P_t *pads, const Pstring *f, const Pstring *s,
		   int eat_f, int eat_s, int panic,
		   int *f_found_out, size_t *offset_out, Pcharset char_set,
		   const char *whatfn) 
{
  Pstring      *tmp_f = (Pstring*)f;
  Pstring      *tmp_s = (Pstring*)s;
  size_t        fwidth, swidth;
  Pbyte        *p1;
  PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic);

  PDCI_IODISC_4P_CHECKS(whatfn, f, s, f_found_out, offset_out);

  P_TRACE7(pads->disc, "PDCI_str_lit_scan args: f = %s s = %s eat_f = %d eat_s = %d, panic %d, char_set = %s, whatfn = %s",
	     P_qfmt_str(f), P_qfmt_str(s), eat_f, eat_s, panic, Pcharset2str(char_set), whatfn);
  (*offset_out) = 0;
  fwidth = f->len;
  swidth = s->len;
#ifndef NDEBUG
  if (fwidth == 0) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: empty f string specified", whatfn);
    }
    return P_ERR;
  }
  if (swidth == 0) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: empty s string specified", whatfn);
    }
    return P_ERR;
  }
#endif
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_f = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_f, f->str, fwidth);
      tmp_s = &pads->stmp2;
      PDCI_A2E_STR_CPY(tmp_s, s->str, swidth);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_BYTES(return P_ERR);
  p1 = begin;
  while (1) {
    if (p1 + fwidth > end) return P_ERR;
    /* p1 + fwidth <= end */
    if (strncmp((char*)p1, tmp_f->str, fwidth) == 0) {
      (*f_found_out) = 1;
      (*offset_out) = (p1-begin);
      if (eat_f) {
	p1 += fwidth; /* advance beyond f */
	PDCI_ECHO_TOKEN(pads, "Str_lit",begin,p1);
      }
      PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
      return P_OK;
    }
    if ((p1 + swidth <= end) &&
	strncmp((char*)p1, tmp_s->str, swidth) == 0) {
      (*f_found_out) = 0;
      (*offset_out) = (p1-begin);
      if (eat_s) {
	p1 += swidth; /* advance beyond s */
	PDCI_ECHO_TOKEN(pads, "Str_lit",begin,p1);
      }
      PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
      return P_OK;
    }
    p1++;
  }

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return P_ERR;
}

Perror_t
PDCI_re_scan1(P_t *pads, Pregexp_t *f,
	      int eat_f, int panic,
	      size_t *offset_out, Pcharset char_set,
	      const char *whatfn) 
{
  regflags_t    e_flags;
  Pbyte        *p1;
  int           bor;
  PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic);
  bor = elt->bor && (bytes == elt->len);

  PDCI_IODISC_2P_CHECKS(whatfn, f, offset_out);

  P_TRACE4(pads->disc, "PDCI_re_scan args: eat_f = %d panic %d, char_set = %s, whatfn = %s",
	     eat_f, panic, Pcharset2str(char_set), whatfn);
  (*offset_out) = 0;
  if (!f->valid) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: invalid regular expression, f->valid is zero", whatfn);
    }
    return P_ERR;
  }
  PDCI_IO_NEED_BYTES(return P_ERR);
  if (end-begin == 0 && !eor) {
    /* must be at eof, do not want to match anything (not even /$/) */
    return P_ERR;
  }
  e_flags = 0; /* do not pin left when we do a scan */
  if (!bor) {
    e_flags |= REG_NOTBOL;
  }
  if (!eor) {
    e_flags |= REG_NOTEOL;
  }
  if (PDCI_regexp_match(pads, f, begin, end, e_flags, char_set)) return P_ERR;
  /* found */
  (*offset_out) = f->match[0].rm_so;
  if (eat_f) {
    p1 = begin + f->match[0].rm_eo; /* if rm_eo is 1 then last char in match is at begin */
    PDCI_ECHO_TOKEN(pads, "Re_lit",begin,p1);
  } else {
    p1 = begin + f->match[0].rm_so; /* if rm_so is zero then match occurred at begin */
  }
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  return P_OK;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;
}

Perror_t
PDCI_re_scan2(P_t *pads, Pregexp_t *f, Pregexp_t *s,
	      int eat_f, int eat_s, int panic,
	      int *f_found_out, size_t *offset_out, Pcharset char_set,
	      const char *whatfn) 
{
  regflags_t    e_flags;
  Pbyte        *p1;
  int           bor;
  PDCI_IO_NEED_BYTES_SETUP_SCAN_OR_PANIC(panic);
  bor = elt->bor && (bytes == elt->len);

  PDCI_IODISC_4P_CHECKS(whatfn, f, s, f_found_out, offset_out);

  P_TRACE5(pads->disc, "PDCI_re_scan args: eat_f = %d, eat_s = %d, panic %d, char_set = %s, whatfn = %s",
	     eat_f, eat_s, panic, Pcharset2str(char_set), whatfn);
  (*offset_out) = 0;
  if (!f->valid) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: invalid regular expression, f->valid is zero", whatfn);
    }
    return P_ERR;
  }
  if (!s->valid) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: invalid regular expression, s->valid is zero", whatfn);
    }
    return P_ERR;
  }
  PDCI_IO_NEED_BYTES(return P_ERR);
  if (end-begin == 0 && !eor) {
    /* must be at eof, do not want to match anything (not even /$/) */
    return P_ERR;
  }
  e_flags = 0; /* do not pin left when we do a scan */
  if (!bor) {
    e_flags |= REG_NOTBOL;
  }
  if (!eor) {
    e_flags |= REG_NOTEOL;
  }
  if (PDCI_regexp_match(pads, f, begin, end, e_flags, char_set)) {
    /* no f match */
    if (PDCI_regexp_match(pads, s, begin, end, e_flags, char_set)) {
      /* neither f or s match */
      return P_ERR;
    }
    /* s match */
    (*f_found_out) = 0;
    (*offset_out) = s->match[0].rm_so;
    if (eat_s) {
      p1 = begin + s->match[0].rm_eo; /* if rm_eo is 1 then last char in match is at begin */
      PDCI_ECHO_TOKEN(pads, "Re_lit",begin,p1);
    } else {
      p1 = begin + s->match[0].rm_so; /* if rm_so is zero then match occurred at begin */
    }
  } else {
    /* f match */
    if (f->match[0].rm_so &&
	!PDCI_regexp_match(pads, s, begin, end, e_flags, char_set) &&
	s->match[0].rm_so < f->match[0].rm_so) {
      /* s match that begins earlier than f match */
      (*f_found_out) = 0;
      (*offset_out) = s->match[0].rm_so;
      if (eat_s) {
	p1 = begin + s->match[0].rm_eo; /* if rm_eo is 1 then last char in match is at begin */
        PDCI_ECHO_TOKEN(pads, "Re_lit",begin,p1);
      } else {
	p1 = begin + s->match[0].rm_so; /* if rm_so is zero then match occurred at begin */
      }
    } else {
      /* f match is the best or only match */
      (*f_found_out) = 1;
      (*offset_out) = f->match[0].rm_so;
      if (eat_f) {
	p1 = begin + f->match[0].rm_eo; /* if rm_eo is 1 then last char in match is at begin */
        PDCI_ECHO_TOKEN(pads, "Re_lit",begin,p1);
      } else {
	p1 = begin + f->match[0].rm_so; /* if rm_so is zero then match occurred at begin */
      }
    }
  }
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  return P_OK;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;
}

Perror_t
PDCI_cstr_lit_scan1(P_t *pads, const char *f,
		    int eat_f, int panic,
		    size_t *offset_out, Pcharset char_set,
		    const char *whatfn)
{
  Pstring fS;

  PDCI_IODISC_2P_CHECKS(whatfn, f, offset_out);
  P_STRING_INIT_CSTR(fS, f);
  return PDCI_str_lit_scan1(pads, &fS, eat_f, panic, offset_out, char_set, whatfn);
}

Perror_t
PDCI_cstr_lit_scan2(P_t *pads, const char *f, const char *s,
		    int eat_f, int eat_s, int panic,
		    int *f_found_out, size_t *offset_out, Pcharset char_set,
		    const char *whatfn)
{
  Pstring fS, sS;

  PDCI_IODISC_4P_CHECKS(whatfn, f, s, f_found_out, offset_out);
  P_STRING_INIT_CSTR(fS, f);
  P_STRING_INIT_CSTR(sS, s);
  return PDCI_str_lit_scan2(pads, &fS, &sS, eat_f, eat_s, panic, f_found_out, offset_out, char_set, whatfn);
}

#endif /* P_CONFIG_READ_FUNCTIONS */

/* ================================================================================ */
/* CHARSET INTERNAL MATCH FUNCTIONS */

#if P_CONFIG_READ_FUNCTIONS > 0

Perror_t
PDCI_char_lit_match(P_t *pads, Pchar f, int eat_f,
		    Pcharset char_set, const char *whatfn)
{
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_0P_CHECKS(whatfn);
  P_TRACE4(pads->disc, "PDCI_char_lit_match args: f %s eat_f %d char_set = %s, whatfn = %s",
	     P_qfmt_char(f), eat_f, Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      f = P_mod_ae_tab[(int)f]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_K_BYTES(1, goto fatal_nb_io_err);
  if (end-begin != 1) return P_ERR;
  if (f == (*begin)) {
    if (eat_f) {
      PDCI_IO_FORWARD(1, goto fatal_forward_err);
    }
    return P_OK;
  }
  /* not found */
  return P_ERR;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return P_ERR;

 fatal_nb_io_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_IO_ERR, whatfn, "IO error (nb)");
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;
}

Perror_t
PDCI_str_lit_match(P_t *pads, const Pstring *f, int eat_f,
		   Pcharset char_set, const char *whatfn) 
{
  Pstring      *tmp_f = (Pstring*)f;
  size_t        width;
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_1P_CHECKS(whatfn, f);

  P_TRACE4(pads->disc, "PDCI_str_lit_match args: f = %s eat_f = %d char_set = %s, whatfn = %s",
	     P_qfmt_str(f), eat_f, Pcharset2str(char_set), whatfn);
  width = f->len;
#ifndef NDEBUG
  if (width == 0) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: empty find string specified", whatfn);
    }
    return P_ERR;
  }
#endif
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_f = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_f, f->str, width);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  if (end-begin != width) return P_ERR;
  if (strncmp((char*)begin, tmp_f->str, width) == 0) {
    if (eat_f) {
      PDCI_IO_FORWARD(width, goto fatal_forward_err);
    }
    return P_OK;
  }
  /* not found */ 
  return P_ERR;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return P_ERR;

 fatal_nb_io_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_IO_ERR, whatfn, "IO error (nb)");
  return P_ERR;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return P_ERR;
}

Perror_t
PDCI_re_match(P_t *pads, Pregexp_t *f, int eat_f,
	      Pcharset char_set, const char *whatfn)
{
  regflags_t    e_flags;
  int           bor;
  PDCI_IO_NEED_BYTES_SETUP_MATCH;
  bor = elt->bor && (bytes == elt->len);

  PDCI_IODISC_1P_CHECKS(whatfn, f);

  P_TRACE3(pads->disc, "PDCI_re_match args: eat_f = %d char_set = %s, whatfn = %s",
	   eat_f, Pcharset2str(char_set), whatfn);
  if (!f->valid) {
    if (pads->speclev == 0) {
      P_WARN1(pads->disc, "%s: invalid regular expression, f->valid is zero", whatfn);
    }
    return P_ERR;
  }
  PDCI_IO_NEED_BYTES(return P_ERR);
  if (end-begin == 0 && !eor) {
    /* must be at eof, do not want to match anything (not even /$/) */
    return P_ERR;
  }
  e_flags = REG_LEFT; /* pin left when we do an inclusive match */
  if (!bor) {
    e_flags |= REG_NOTBOL;
  }
  if (!eor) {
    e_flags |= REG_NOTEOL;
  }
  if (PDCI_regexp_match(pads, f, begin, end, e_flags, char_set)) return P_ERR;
  /* found */
  if (eat_f) {
    size_t width = f->match[0].rm_eo; /* if rm_eo is 1 then last char in match is at begin */
    PDCI_IO_FORWARD(width, goto fatal_forward_err);
  }
  return P_OK;

 fatal_forward_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_FORWARD_ERR, whatfn, "IO_forward error");
  return P_ERR;
}

Perror_t
PDCI_cstr_re_match(P_t *pads, const char *f, int eat_f,
		   Pcharset char_set, const char *whatfn)
{
  Perror_t    res;
  P_REGEXP_DECL_NULL(compiled_exp);

  PDCI_IODISC_1P_CHECKS(whatfn, f);
  if (P_ERR == PDCI_regexp_compile_cstr(pads, f, &compiled_exp, "Pcstr_re_match arg", whatfn)) {
    goto bad_exp;
  }
  res = PDCI_re_match(pads, &compiled_exp, eat_f, char_set, whatfn);
  PDCI_regexp_cleanup(pads, &compiled_exp, whatfn);
  return res;

 bad_exp:
  /* regexp_compile already issued a warning */
  return P_ERR;
}

Perror_t
PDCI_cstr_lit_match(P_t *pads, const char *f, int eat_f,
		    Pcharset char_set, const char *whatfn)
{
  Pstring fS;

  PDCI_IODISC_1P_CHECKS(whatfn, f);
  P_STRING_INIT_CSTR(fS, f);
  return PDCI_str_lit_match(pads, &fS, eat_f, char_set, whatfn);
}

#endif /* P_CONFIG_READ_FUNCTIONS */

/* ================================================================================ */
/* CHARSET INTERNAL READ ROUTINES */

#if P_CONFIG_READ_FUNCTIONS > 0

Perror_t
PDCI_char_lit_read(P_t *pads, const Pbase_m *m, Pchar c,
		   Pbase_pd *pd, Pchar *c_out, Pcharset char_set,
		   const char *whatfn)
{
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, c_out);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE3(pads->disc, "PDCI_char_lit_read called, arg: %s, char_set %s, whatfn = %s",
	     P_qfmt_char(c), Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      c = P_mod_ae_tab[(int)c]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_K_BYTES(1, goto fatal_nb_io_err);
  if (end-begin != 1) goto at_eor_or_eof_err;
  if (P_Test_NotSynCheck(*m) || (c == (*begin))) {
    PDCI_IO_FORWARD(1, goto fatal_forward_err);
    (*c_out) = c;
    return P_OK;  /* IO cursor is one beyond c */
  }
  goto not_found;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 at_eor_or_eof_err:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, eor ? P_AT_EOR : P_AT_EOF);

 not_found:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_CHAR_LIT_NOT_FOUND);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

Perror_t
PDCI_str_lit_read(P_t *pads, const Pbase_m *m, const Pstring *s,
		  Pbase_pd *pd, Pstring *s_out, Pcharset char_set, const char *whatfn)
{
  Pstring      *es = (Pstring*)s;
  size_t        width;
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_4P_CHECKS(whatfn, m, pd, s, s_out);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE3(pads->disc, "PDCI_str_lit_read called, arg: %s, char_set %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);
  width = s->len;
  PDCI_READFN_WIDTH_CHECK(whatfn, "string literal", width);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      es = &pads->stmp1;
      PDCI_A2E_STR_CPY(es, s->str, width);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  if (end-begin != width)     goto width_not_avail;
  if (P_Test_NotSynCheck(*m) || (strncmp((char*)begin, es->str, width) == 0)) {
    error(0, "Copying literal character.");
    switch (char_set)
      {
      case Pcharset_ASCII:
	PDCI_A_STR_SET(m, s_out, begin, width);
	break;
      case Pcharset_EBCDIC:
	PDCI_STR_CPY_ON_SET(m, s_out, s->str, width);
	break;
      default:
	goto invalid_charset;
      }
    PDCI_IO_FORWARD(width, goto fatal_forward_err);
    return P_OK;    /* found it */
  }
  goto not_found;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 width_not_avail:
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_WIDTH_NOT_AVAILABLE);

 not_found:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, width-1);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_STR_LIT_NOT_FOUND);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);

 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR);
}

Perror_t
PDCI_cstr_lit_read(P_t *pads, const Pbase_m *m, const char *s, 
		   Pbase_pd *pd, Pstring *s_out, Pcharset char_set, const char *whatfn)
{
  Pstring  p_s;

  PDCI_IODISC_4P_CHECKS(whatfn, m, pd, s, s_out);
  P_STRING_INIT_CSTR(p_s, s);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  return PDCI_str_lit_read(pads, m, &p_s, pd, s_out, char_set, whatfn);
}

Perror_t
PDCI_countX_read(P_t *pads, const Pbase_m *m,
		 Pbase_pd *pd, Pint32 *res_out, Pcharset char_set, const char *whatfn, Puint8 x, int eor_required, size_t count_max)
{
  Pint32       count = 0;
  Pbyte       *p1;
  PDCI_IO_NEED_BYTES_SETUP_SPECIFIC;

  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE5(pads->disc, "PDCI_countX_read called, args: x = %s eor_required = %d, count_max = %lld, char_set %s, whatfn = %s",
	     P_qfmt_char(x), eor_required, (long long)count_max, Pcharset2str(char_set), whatfn);
  (*res_out) = 0;
  if (!pads->disc->io_disc->rec_based && !count_max) {
    goto bad_param;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      x = P_mod_ae_tab[(int)x]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_BYTES_SPECIFIC(count_max, goto fatal_nb_io_err);
  p1 = begin;
  while (!(eor|eof)) { /* end > begin, otherwise eor|eof would be true. */
    if (p1 == end) goto hit_limit; 
    if (x == (*p1)) {
      count++;
    }
    p1++;
  }
  if (!eor && eor_required) { /* EOF encountered first, error */
    if (begin == end){ /* Must be at eof. */
      PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
    } else {
      PDCI_READFN_BEGINLOC(pads, pd->loc);
      PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
    }
    PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_EOF_BEFORE_EOR);
  }
  (*res_out) = count;
  return P_OK;

 bad_param:
  if (pads->speclev == 0) {
    P_WARN1(pads->disc, "%s: countX_read must have scan_max > 0 with a non-record-based IO discipline", whatfn);
  }
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_BAD_PARAM);

 hit_limit:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_COUNT_MAX_LIMIT);

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);
}

Perror_t
PDCI_countXtoY_read(P_t *pads, const Pbase_m *m,
		    Pbase_pd *pd, Pint32 *res_out, Pcharset char_set, const char *whatfn, Puint8 x, Puint8 y, size_t count_max)
{
  Pint32       count = 0;
  Pbyte       *p1;
  PDCI_IO_NEED_BYTES_SETUP_SPECIFIC;

  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE5(pads->disc, "PDCI_countXtoY_read called, args: x = %s y = %s, count_max %lld, char_set %s, whatfn = %s",
	     P_qfmt_char(x), P_qfmt_char(y), (long long)count_max, Pcharset2str(char_set), whatfn);
  (*res_out) = 0;
  if (!pads->disc->io_disc->rec_based && !count_max) {
    goto bad_param;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      x = P_mod_ae_tab[(int)x]; /* convert to EBCDIC char */
      y = P_mod_ae_tab[(int)y]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_BYTES_SPECIFIC(count_max, goto fatal_nb_io_err);
  p1 = begin;
  /* Looks like this is an abbreviation for "if (!(eor|oef)) while
     (1){...}", as eor and eof cannot be modified within the loop. 
     -- YHM
  */
  while (!(eor|eof)) {
    if (p1 == end) goto hit_limit;
    if (y == (*p1)) { /* success */
      (*res_out) = count;
      return P_OK;
    }
    if (x == (*p1)) {
      count++;
    }
    p1++;
  }
  goto not_found; /* hit eor or eof, y not found */

 bad_param:
  if (pads->speclev == 0) {
    P_WARN1(pads->disc, "%s: countXtoY_read must have scan_max > 0 with a non-record-based IO discipline", whatfn);
  }
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_BAD_PARAM);

 hit_limit:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_COUNT_MAX_LIMIT);

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 not_found:
  if (begin == p1){ 
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_CHAR_LIT_NOT_FOUND);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);
}

Perror_t
PDCI_timestamp_FW_read(P_t *pads,
		       const Pbase_m *m,
		       Pbase_pd *pd,
		       Puint32 *res_out,
		       Pcharset char_set,
		       const char *whatfn,
		       const char *format_descr,
		       const char *tzone_descr,
		       PerrCode_t errCode,
		       int just_time,
		       const char *format,
		       Tm_zone_t *tzone,
		       size_t width)
{
  Pstring     *s;
  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_timestamp_FW_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_FW_read(pads, m, pd, s, char_set, whatfn, width)) {
    return P_ERR;
  }
  PDCI_FINISH_TIMESTAMP_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_timestamp_read(P_t *pads,
		    const Pbase_m *m,
		    Pbase_pd *pd,
		    Puint32 *res_out,
		    Pcharset char_set,
		    const char *whatfn,
		    const char *format_descr,
		    const char *tzone_descr,
		    PerrCode_t errCode,
		    int just_time,
		    const char *format,
		    Tm_zone_t *tzone,
		    Pchar stopChar)
{
  Pstring     *s;
  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE5(pads->disc, "PDCI_timestamp_read called, args: stopChar %s char_set %s, whatfn = %s, format = %s, tzone = %s",
	   P_qfmt_char(stopChar), Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_read(pads, m, pd, s, char_set, whatfn, stopChar)) {
    return P_ERR;
  }
  PDCI_FINISH_TIMESTAMP_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_timestamp_ME_read(P_t *pads,
		       const Pbase_m *m,
		       Pbase_pd *pd,
		       Puint32 *res_out,
		       Pcharset char_set,
		       const char *whatfn,
		       const char *format_descr,
		       const char *tzone_descr,
		       PerrCode_t errCode,
		       int just_time,
		       const char *format,
		       Tm_zone_t *tzone,
		       const char *matchRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, matchRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_timestamp_ME_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_ME_read(pads, m, pd, s, char_set, whatfn, matchRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIMESTAMP_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_timestamp_CME_read(P_t *pads,
			const Pbase_m *m,
			Pbase_pd *pd,
			Puint32 *res_out,
			Pcharset char_set,
			const char *whatfn,
			const char *format_descr,
			const char *tzone_descr,
			PerrCode_t errCode,
			int just_time,
			const char *format,
			Tm_zone_t *tzone,
			Pregexp_t *matchRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, matchRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_timestamp_CME_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_CME_read(pads, m, pd, s, char_set, whatfn, matchRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIMESTAMP_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_timestamp_SE_read(P_t *pads,
		       const Pbase_m *m,
		       Pbase_pd *pd,
		       Puint32 *res_out,
		       Pcharset char_set,
		       const char *whatfn,
		       const char *format_descr,
		       const char *tzone_descr,
		       PerrCode_t errCode,
		       int just_time,
		       const char *format,
		       Tm_zone_t *tzone,
		       const char *stopRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, stopRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_timestamp_SE_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_SE_read(pads, m, pd, s, char_set, whatfn, stopRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIMESTAMP_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_timestamp_CSE_read(P_t *pads,
			const Pbase_m *m,
			Pbase_pd *pd,
			Puint32 *res_out,
			Pcharset char_set,
			const char *whatfn,
			const char *format_descr,
			const char *tzone_descr,
			PerrCode_t errCode,
			int just_time,
			const char *format,
			Tm_zone_t *tzone,
			Pregexp_t *stopRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, stopRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_timestamp_CSE_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_CSE_read(pads, m, pd, s, char_set, whatfn, stopRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIMESTAMP_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_date_FW_read(P_t *pads,
		  const Pbase_m *m,
		  Pbase_pd *pd,
		  Puint32 *res_out,
		  Pcharset char_set,
		  const char *whatfn,
		  const char *format_descr,
		  const char *tzone_descr,
		  PerrCode_t errCode,
		  int just_time,
		  const char *format,
		  Tm_zone_t *tzone,
		  size_t width)
{
  Pstring     *s;
  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_date_FW_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_FW_read(pads, m, pd, s, char_set, whatfn, width)) {
    return P_ERR;
  }
  PDCI_FINISH_DATE_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_date_read(P_t *pads,
	       const Pbase_m *m,
	       Pbase_pd *pd,
	       Puint32 *res_out,
	       Pcharset char_set,
	       const char *whatfn,
	       const char *format_descr,
	       const char *tzone_descr,
	       PerrCode_t errCode,
	       int just_time,
	       const char *format,
	       Tm_zone_t *tzone,
	       Pchar stopChar)
{
  Pstring     *s;
  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE5(pads->disc, "PDCI_date_read called, args: stopChar %s char_set %s, whatfn = %s, format = %s, tzone = %s",
	   P_qfmt_char(stopChar), Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_read(pads, m, pd, s, char_set, whatfn, stopChar)) {
    return P_ERR;
  }
  PDCI_FINISH_DATE_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_date_ME_read(P_t *pads,
		  const Pbase_m *m,
		  Pbase_pd *pd,
		  Puint32 *res_out,
		  Pcharset char_set,
		  const char *whatfn,
		  const char *format_descr,
		  const char *tzone_descr,
		  PerrCode_t errCode,
		  int just_time,
		  const char *format,
		  Tm_zone_t *tzone,
		  const char *matchRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, matchRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_date_ME_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_ME_read(pads, m, pd, s, char_set, whatfn, matchRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_DATE_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_date_CME_read(P_t *pads,
		   const Pbase_m *m,
		   Pbase_pd *pd,
		   Puint32 *res_out,
		   Pcharset char_set,
		   const char *whatfn,
		   const char *format_descr,
		   const char *tzone_descr,
		   PerrCode_t errCode,
		   int just_time,
		   const char *format,
		   Tm_zone_t *tzone,
		   Pregexp_t *matchRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, matchRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_date_CME_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_CME_read(pads, m, pd, s, char_set, whatfn, matchRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_DATE_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_date_SE_read(P_t *pads,
		  const Pbase_m *m,
		  Pbase_pd *pd,
		  Puint32 *res_out,
		  Pcharset char_set,
		  const char *whatfn,
		  const char *format_descr,
		  const char *tzone_descr,
		  PerrCode_t errCode,
		  int just_time,
		  const char *format,
		  Tm_zone_t *tzone,
		  const char *stopRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, stopRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_date_SE_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_SE_read(pads, m, pd, s, char_set, whatfn, stopRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_DATE_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_date_CSE_read(P_t *pads,
		   const Pbase_m *m,
		   Pbase_pd *pd,
		   Puint32 *res_out,
		   Pcharset char_set,
		   const char *whatfn,
		   const char *format_descr,
		   const char *tzone_descr,
		   PerrCode_t errCode,
		   int just_time,
		   const char *format,
		   Tm_zone_t *tzone,
		   Pregexp_t *stopRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, stopRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_date_CSE_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_CSE_read(pads, m, pd, s, char_set, whatfn, stopRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_DATE_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_time_FW_read(P_t *pads,
		  const Pbase_m *m,
		  Pbase_pd *pd,
		  Puint32 *res_out,
		  Pcharset char_set,
		  const char *whatfn,
		  const char *format_descr,
		  const char *tzone_descr,
		  PerrCode_t errCode,
		  int just_time,
		  const char *format,
		  Tm_zone_t *tzone,
		  size_t width)
{
  Pstring     *s;
  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_time_FW_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_FW_read(pads, m, pd, s, char_set, whatfn, width)) {
    return P_ERR;
  }
  PDCI_FINISH_TIME_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_time_read(P_t *pads,
	       const Pbase_m *m,
	       Pbase_pd *pd,
	       Puint32 *res_out,
	       Pcharset char_set,
	       const char *whatfn,
	       const char *format_descr,
	       const char *tzone_descr,
	       PerrCode_t errCode,
	       int just_time,
	       const char *format,
	       Tm_zone_t *tzone,
	       Pchar stopChar)
{
  Pstring     *s;
  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE5(pads->disc, "PDCI_time_read called, args: stopChar %s char_set %s, whatfn = %s, format = %s, tzone = %s",
	   P_qfmt_char(stopChar), Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_read(pads, m, pd, s, char_set, whatfn, stopChar)) {
    return P_ERR;
  }
  PDCI_FINISH_TIME_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_time_ME_read(P_t *pads,
		  const Pbase_m *m,
		  Pbase_pd *pd,
		  Puint32 *res_out,
		  Pcharset char_set,
		  const char *whatfn,
		  const char *format_descr,
		  const char *tzone_descr,
		  PerrCode_t errCode,
		  int just_time,
		  const char *format,
		  Tm_zone_t *tzone,
		  const char *matchRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, matchRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_time_ME_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_ME_read(pads, m, pd, s, char_set, whatfn, matchRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIME_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_time_CME_read(P_t *pads,
		   const Pbase_m *m,
		   Pbase_pd *pd,
		   Puint32 *res_out,
		   Pcharset char_set,
		   const char *whatfn,
		   const char *format_descr,
		   const char *tzone_descr,
		   PerrCode_t errCode,
		   int just_time,
		   const char *format,
		   Tm_zone_t *tzone,
		   Pregexp_t *matchRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, matchRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_time_CME_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_CME_read(pads, m, pd, s, char_set, whatfn, matchRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIME_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_time_SE_read(P_t *pads,
		  const Pbase_m *m,
		  Pbase_pd *pd,
		  Puint32 *res_out,
		  Pcharset char_set,
		  const char *whatfn,
		  const char *format_descr,
		  const char *tzone_descr,
		  PerrCode_t errCode,
		  int just_time,
		  const char *format,
		  Tm_zone_t *tzone,
		  const char *stopRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, stopRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_time_SE_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_SE_read(pads, m, pd, s, char_set, whatfn, stopRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIME_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_time_CSE_read(P_t *pads,
		   const Pbase_m *m,
		   Pbase_pd *pd,
		   Puint32 *res_out,
		   Pcharset char_set,
		   const char *whatfn,
		   const char *format_descr,
		   const char *tzone_descr,
		   PerrCode_t errCode,
		   int just_time,
		   const char *format,
		   Tm_zone_t *tzone,
		   Pregexp_t *stopRegexp)
{
  Pstring     *s;
  PDCI_IODISC_4P_CHECKS(whatfn, m, stopRegexp, pd, res_out);
  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  s = &pads->stmp1;
  P_TRACE4(pads->disc, "PDCI_time_CSE_read called, args: char_set %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);
  /* Following call does a Pbase_pd_init_no_err(pd) */
  if (P_ERR == PDCI_string_CSE_read(pads, m, pd, s, char_set, whatfn, stopRegexp)) {
    return P_ERR;
  }
  PDCI_FINISH_TIME_READ(m, format, tzone, errCode, just_time);
}

Perror_t
PDCI_ip_read(P_t *pads, const Pbase_m *m,
	     Pbase_pd *pd, Puint32 *res_out, Pcharset char_set, const char *whatfn)
{
  Puint32      addr = 0;
  Pbyte       *p1;
  Pbyte       *part_start;
  Pbyte       *range_err_start = 0;
  Pbyte       *range_err_end   = 0;
  int          zero = '0', nine = '9', dot = '.';
  int          digits;
  int          byte = 0;
  int          parts = 0;
  PDCI_IO_NEED_BYTES_SETUP_MATCH;

  PDCI_IODISC_3P_CHECKS(whatfn, m, pd, res_out);
  P_TRACE2(pads->disc, "PDCI_ip_read called, args: char_set %s, whatfn = %s",
	   Pcharset2str(char_set), whatfn);
  PDCI_READFN_PD_INIT(pads, pd);

  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      zero = P_mod_ae_tab[zero]; /* convert to EBCDIC char */
      nine = P_mod_ae_tab[nine]; /* convert to EBCDIC char */
      dot  = P_mod_ae_tab[dot];  /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }

  PDCI_IO_NEED_BYTES(goto fatal_nb_io_err);
  if (begin == end) {
    goto at_eor_or_eof_err;
  }
  p1 = begin;
  if (P_Test_Ignore(*m)) {
    /* move beyond anything that looks like an ip address, return P_ERR if none such */
    if (!((*p1) >= zero && (*p1) <= nine)) {
      return P_ERR; /* did not find an ip address */
    }
    /* skip up to four parts with up to 3 digits each, move IO cursor, return P_OK */
    while (parts < 4) {
      parts++;
      digits = 0;
      while (((*p1) >= zero && (*p1) <= nine) && digits < 3) {
	p1++;
	digits++;
	if (p1 == end && !(eor|eof)) {
	  /* did not find end of ip address within goal_bytes (normally P_BUILTIN_MATCH_MAX) */
	  return P_ERR;
	}
      }
      if ((*p1) != dot) break;
      // found a dot after some digits, skip it
      p1++;
      if (!((*p1) >= zero && (*p1) <= nine)) break;
      // found another part, continue loop
    }
    // successfully skipped over up to four parts and optional trailing dot
    PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
    return P_OK;

  } else { /* !P_Test_Ignore(*m) */

    if (!((*p1) >= zero && (*p1) <= nine)) {
      p1++;  /* include first char as the error loc */
      goto invalid; /* did not find an ip address */
    }
    /* parse up to four parts with up to 3 digits each, move IO cursor, return P_OK */
    while (parts < 4) {
      part_start = p1;
      if (parts > 0) {
	// byte is folded in after each loop iter completes
	// including just after while loop
	addr = addr << 8;
	addr += byte;
      }
      parts++;
      byte = 0;
      digits = 0;
      while (((*p1) >= zero && (*p1) <= nine) && digits < 3) {
	byte *= 10;
	byte += (*p1) - zero;
	p1++;
	digits++;
	if (p1 == end && !(eor|eof)) {
	  /* did not find end of ip address within goal_bytes (normally P_BUILTIN_MATCH_MAX) */
	  goto invalid;
	}
      }
      if (byte > 255 && !range_err_start) {
	range_err_start = part_start;
	range_err_end   = p1;
	byte = 255;
      }
      if ((*p1) != dot) break;
      // found a dot after some digits, skip it
      p1++;
      if (!((*p1) >= zero && (*p1) <= nine)) break;
      // found another part, continue loop
    }
    // Successfully skipped over four parts and optional trailing dot.
    // (OR didn't find a dot or found a char outside 0..9!)
    if (parts != 4) {
      // IP addresses have four parts
      goto invalid;
    } 
    // The final byte has not been added to addr yet.
    addr = addr << 8;
    addr += byte;
    PDCI_ECHO_TOKEN(pads,"IP", begin,p1);
    PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
    if (range_err_start && P_Test_SemCheck(*m)) goto invalid_range;
    if (P_Test_Set(*m)) {
      (*res_out) = addr;
    }
    return P_OK;
  }

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 at_eor_or_eof_err:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, eor ? P_AT_EOR : P_AT_EOF);

 invalid:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_IP);

 invalid_range:
  PDCI_READFN_BEGINLOC_MINUSK(pads, pd->loc, p1 - range_err_start);
  PDCI_READFN_ENDLOC_MINUSK(pads, pd->loc, (p1 - range_err_end) + 1);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_IP_RANGE);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

Perror_t
PDCI_char_read(P_t *pads, const Pbase_m *m,
	       Pbase_pd *pd, Pchar *c_out, Pcharset char_set,
	       const char *whatfn)
{
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_2P_CHECKS(whatfn, m, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE2(pads->disc, "PDCI_char_read called, char_set = %s, whatfn = %s",
	     Pcharset2str(char_set), whatfn);
  PDCI_IO_NEED_K_BYTES(1, goto fatal_nb_io_err);
  if (end-begin != 1)     goto width_not_avail;
  if (c_out && P_Test_Set(*m)) {
    switch (char_set)
      {
      case Pcharset_ASCII:
	(*c_out) = *begin;
	break;
      case Pcharset_EBCDIC:
	(*c_out) = P_mod_ea_tab[(int)(*begin)];
	break;
      default:
	goto invalid_charset;
      }
  }
  PDCI_ECHO_TOKEN(pads, "Pchar", begin, begin+1);
  PDCI_IO_FORWARD(1, goto fatal_forward_err);
  return P_OK;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 width_not_avail:
  PDCI_READFN_BEGINLOC(pads, pd->loc);
  PDCI_READFN_ENDLOC_MINUS1(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_WIDTH_NOT_AVAILABLE);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

Perror_t
PDCI_string_FW_read(P_t *pads, const Pbase_m *m,
		    Pbase_pd *pd, Pstring *s_out, Pcharset char_set,
		    const char *whatfn, size_t width)
{
  PDCI_IO_NEED_K_BYTES_SETUP;

  PDCI_IODISC_2P_CHECKS(whatfn, m, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE2(pads->disc, "PDCI_string_FW_read called, char_set = %s, whatfn = %s",
	     Pcharset2str(char_set), whatfn);
  PDCI_READFN_WIDTH_CHECK_ZERO_OK(whatfn, "string", width);

  /* ensure there are width chars available */
  PDCI_IO_NEED_K_BYTES(width, goto fatal_nb_io_err);
  if (width == 0) {
    s_out->str = (char*)begin;
    s_out->len = 0;
    return P_OK;
  }
  if (end-begin != width) goto width_not_avail;
  switch (char_set)
    {
    case Pcharset_ASCII:
      PDCI_A_STR_SET(m, s_out, begin, width);
      break;
    case Pcharset_EBCDIC:
      PDCI_E_STR_SET(m, s_out, begin, width);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_ECHO_TOKEN(pads, whatfn, begin, begin+width);
  PDCI_IO_FORWARD(width, goto fatal_forward_err);
  return P_OK;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 width_not_avail:
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_WIDTH_NOT_AVAILABLE);

 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

Perror_t
PDCI_string_read(P_t *pads, const Pbase_m *m,
		 Pbase_pd *pd, Pstring *s_out, Pcharset char_set,
		 const char *whatfn, Pchar stopChar)
{
  Pbyte        *p1;
  PDCI_IO_NEED_BYTES_SETUP_SCAN;

  PDCI_IODISC_2P_CHECKS(whatfn, m, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE2(pads->disc, "PDCI_string_read called, char_set = %s, whatfn = %s",
	     Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      stopChar = P_mod_ae_tab[(int)stopChar]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  PDCI_IO_NEED_BYTES(goto fatal_nb_io_err);
  p1 = begin;
  while (1) {
    if (p1 == end) {
      /* a NULL stopChar is allowed to match at eor or eof */
      if (!stopChar && (eor|eof)) break; /* found */
      goto not_found;
    }
    if (stopChar == (*p1)) break; /* found */
    p1++;
  }
  /* termination condition found */
  switch (char_set)
    {
    case Pcharset_ASCII:
      PDCI_A_STR_SET(m, s_out, begin, p1-begin);
      break;
    case Pcharset_EBCDIC:
      PDCI_E_STR_SET(m, s_out, begin, p1-begin);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_ECHO_TOKEN(pads, whatfn, begin, p1);
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  return P_OK;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 not_found:
  if (begin == p1){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {
    /* p1 must be > begin */
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, p1-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_CHAR_LIT_NOT_FOUND);

 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

Perror_t
PDCI_string_ME_read(P_t *pads, const Pbase_m *m,
		    Pbase_pd *pd, Pstring *s_out, Pcharset char_set,
		    const char *whatfn, const char *matchRegexp)
{
  Perror_t    res;
  P_REGEXP_DECL_NULL(compiled_exp);

  PDCI_IODISC_3P_CHECKS(whatfn, m, matchRegexp, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  if (P_ERR == PDCI_regexp_compile_cstr(pads, matchRegexp, &compiled_exp, "Pstring_ME arg", whatfn)) {
    goto bad_exp;
  };
  PDCI_ECHO_TOKEN_PREFIX(pads,whatfn);
  PDCI_ECHO_TOKEN_PREFIX(pads,matchRegexp);
  res = PDCI_string_CME_read(pads, m, pd, s_out, char_set, whatfn, &compiled_exp);
  PDCI_regexp_cleanup(pads, &compiled_exp, whatfn);
  return res;

 bad_exp:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  /* regexp_compile already issued a warning */
  PDCI_READFN_RET_ERRCODE_NOWARN(P_INVALID_REGEXP);
}

Perror_t
PDCI_string_CME_read(P_t *pads, const Pbase_m *m,
		     Pbase_pd *pd, Pstring *s_out, Pcharset char_set,
		     const char *whatfn, Pregexp_t *matchRegexp)
{
  regflags_t   e_flags;
  Pbyte       *p1;
  int           bor;
  PDCI_IO_NEED_BYTES_SETUP_MATCH;
  bor = elt->bor && (bytes == elt->len);

  PDCI_IODISC_3P_CHECKS(whatfn, m, matchRegexp, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE2(pads->disc, "PDCI_string_CME_read called, char_set = %s, whatfn = %s",
	     Pcharset2str(char_set), whatfn);
  PDCI_IO_NEED_BYTES(goto fatal_nb_io_err);
  if (end-begin == 0 && !eor) {
    /* must be at eof, do not want to match anything (not even /$/) */
    goto not_found;
  }
  e_flags = REG_LEFT; /* pin left when we do an inclusive match */
  if (!bor) {
    e_flags |= REG_NOTBOL;
  }
  if (!eor) {
    e_flags |= REG_NOTEOL;
  }
  if (PDCI_regexp_match(pads, matchRegexp, begin, end, e_flags, char_set)) goto not_found;
  /* found, set p1 to just beyond last char to include */
  /*   p1 = begin + (exp->match[0].rm_eo - exp->match[0].rm_so); */
  p1 = begin + matchRegexp->match[0].rm_eo; /* rm_so must be zero due to pin-left */
  switch (char_set) 
    {
    case Pcharset_ASCII:
      PDCI_A_STR_SET(m, s_out, begin, p1-begin);
      break;
    case Pcharset_EBCDIC:
      PDCI_E_STR_SET(m, s_out, begin, p1-begin);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_ECHO_TOKEN(pads,whatfn, begin,p1);
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  return P_OK;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 not_found:
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {    
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_REGEXP_NOT_FOUND);
  
 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

Perror_t
PDCI_string_SE_read(P_t *pads, const Pbase_m *m,
		    Pbase_pd *pd, Pstring *s_out, Pcharset char_set,
		    const char *whatfn, const char *stopRegexp)
{
  P_REGEXP_DECL_NULL(compiled_exp);
  Perror_t    res;

  PDCI_IODISC_3P_CHECKS(whatfn, m, stopRegexp, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  if (P_ERR == PDCI_regexp_compile_cstr(pads, stopRegexp, &compiled_exp, "Pstring_SE arg", whatfn)) {
    goto bad_exp;
  }
  PDCI_ECHO_TOKEN_PREFIX(pads,whatfn);
  PDCI_ECHO_TOKEN_PREFIX(pads,stopRegexp);
  res = PDCI_string_CSE_read(pads, m, pd, s_out, char_set, whatfn, &compiled_exp);
  PDCI_regexp_cleanup(pads, &compiled_exp, whatfn);
  return res;

 bad_exp:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  /* regexp_compile already issued a warning */
  PDCI_READFN_RET_ERRCODE_NOWARN(P_INVALID_REGEXP);
}

Perror_t
PDCI_string_CSE_read(P_t *pads, const Pbase_m *m,
		     Pbase_pd *pd, Pstring *s_out, Pcharset char_set,
		     const char *whatfn, Pregexp_t *stopRegexp)
{
  regflags_t   e_flags;
  Pbyte       *p1;
  int           bor;
  PDCI_IO_NEED_BYTES_SETUP_SCAN;
  bor = elt->bor && (bytes == elt->len);

  PDCI_IODISC_3P_CHECKS(whatfn, m, stopRegexp, pd);
  PDCI_READFN_PD_INIT(pads, pd);
  P_TRACE2(pads->disc, "PDCI_string_CSE_read called, char_set = %s, whatfn = %s",
	     Pcharset2str(char_set), whatfn);
  PDCI_IO_NEED_BYTES(goto fatal_nb_io_err);
  if (end-begin == 0 && !eor) {
    /* must be at eof, do not want to match anything (not even /$/) */
    goto not_found;
  }
  if (end < begin){
    P_WARN(pads->disc, "PDCI_string_CSE_read: end < begin");
    goto fatal_nb_io_err;
  }
  e_flags = 0; /* do not pin left when we do a terminating match */
  if (!bor) {
    e_flags |= REG_NOTBOL;
  }
  if (!eor) {
    e_flags |= REG_NOTEOL;
  }
  if (PDCI_regexp_match(pads, stopRegexp, begin, end, e_flags, char_set)) goto not_found;
  /* found, set p1 to just beyond last char to include == first char in match*/
  p1 = begin + stopRegexp->match[0].rm_so; /* if rm_so is zero then match occurred at begin */
  switch (char_set) 
    {
    case Pcharset_ASCII:
      PDCI_A_STR_SET(m, s_out, begin, p1-begin);
      break;
    case Pcharset_EBCDIC:
      PDCI_E_STR_SET(m, s_out, begin, p1-begin);
      break;
    default:
      goto invalid_charset;
    }
  PDCI_ECHO_TOKEN(pads, whatfn, begin, p1);
  PDCI_IO_FORWARD(p1-begin, goto fatal_forward_err);
  return P_OK;

 invalid_charset:
  PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_INVALID_CHARSET);

 not_found:
  if (begin == end){ /* Must be at eor or eof. */
    PDCI_READFN_GETLOC_SPAN0(pads, pd->loc);
  } else {    
    PDCI_READFN_BEGINLOC(pads, pd->loc);
    PDCI_READFN_ENDLOC_PLUSK(pads, pd->loc, end-begin-1);
  }
  PDCI_READFN_RET_ERRCODE_WARN(whatfn, 0, P_REGEXP_NOT_FOUND);

 fatal_alloc_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "Memory alloc error", P_ALLOC_ERR);

 fatal_nb_io_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO error (nb)", P_IO_ERR);

 fatal_forward_err:
  PDCI_READFN_RET_ERRCODE_FATAL(whatfn, *m, "IO_forward error", P_FORWARD_ERR);
}

#endif /* P_CONFIG_READ_FUNCTIONS */

/* ================================================================================ */
/* CHARSET INTERNAL WRITE ROUTINES */

#if P_CONFIG_WRITE_FUNCTIONS > 0

ssize_t
PDCI_char_lit_write2io(P_t *pads, Sfio_t *io, Pchar c,
		       Pcharset char_set, const char *whatfn)
{
  PDCI_DISC_1P_CHECKS_RET_SSIZE(whatfn, io);
  P_TRACE3(pads->disc, "PDCI_char_lit_write2io args: c %s, char_set = %s, whatfn = %s",
	     P_qfmt_char(c), Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      c = P_mod_ae_tab[(int)c]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  if (c != sfputc(io, c)) {
    P_WARN1(pads->disc, "%s: low-level sfputc failure", whatfn);
    return -1;
  }
  return 1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;
}

ssize_t
PDCI_char_lit_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, Pchar c,
			Pcharset char_set, const char *whatfn)
{
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, buf, buf_full);
  P_TRACE3(pads->disc, "PDCI_char_lit_write2buf args: c %s, char_set = %s, whatfn = %s",
	     P_qfmt_char(c), Pcharset2str(char_set), whatfn);
  if (1 > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      c = P_mod_ae_tab[(int)c]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  *buf = c;
  return 1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;
}

ssize_t
PDCI_char_lit_write_xml_2io(P_t *pads, Sfio_t *io, Pchar c, const char *tag, int indent, const char *whatfn)
{
  PDCI_DISC_1P_CHECKS_RET_SSIZE(whatfn, io);
  P_TRACE2(pads->disc, "PDCI_char_lit_write_xml_2io args: c %s, whatfn = %s", P_qfmt_char(c), whatfn);
  PDCI_BASELIT_XML_OUT2IO("Pchar_lit", "%s", P_fmt_char(c));
}

ssize_t
PDCI_char_lit_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, Pchar c,
			     const char *tag, int indent, const char *whatfn)
{
  ssize_t writelen;
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, buf, buf_full);
  P_TRACE2(pads->disc, "PDCI_char_lit_write_xml_2buf args: c %s, whatfn = %s", P_qfmt_char(c), whatfn);
  PDCI_BASELIT_XML_OUT2BUF("Pchar_lit", "%s", P_fmt_char(c));
}

ssize_t
PDCI_str_lit_write2io(P_t *pads, Sfio_t *io, const Pstring *s,
		      Pcharset char_set, const char *whatfn)
{
  ssize_t         n;
  Pstring     *tmp_s = (Pstring*)s;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE3(pads->disc, "PDCI_str_lit_write2io args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s->str, s->len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_str_lit_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, const Pstring *s,
		       Pcharset char_set, const char *whatfn)
{
  Pstring     *tmp_s = (Pstring*)s;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE3(pads->disc, "PDCI_str_lit_write2buf args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);
  if (tmp_s->len > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s->str, s->len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
  return tmp_s->len;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_str_lit_write_xml_2io(P_t *pads, Sfio_t *io, const Pstring *s, const char *tag, int indent, const char *whatfn)
{
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE2(pads->disc, "PDCI_str_lit_write_xml_2io args: s %s, whatfn = %s", P_qfmt_str(s), whatfn);
  PDCI_BASELIT_XML_OUT2IO("Pstr_lit", "%s", P_fmt_str(s));
}

ssize_t
PDCI_str_lit_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, const Pstring *s,
			    const char *tag, int indent, const char *whatfn)
{
  ssize_t writelen;
  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE2(pads->disc, "PDCI_str_lit_write_xml_2buf args: s %s, whatfn = %s", P_qfmt_str(s), whatfn);
  PDCI_BASELIT_XML_OUT2BUF("Pstr_lit", "%s", P_fmt_str(s));
}

ssize_t
PDCI_cstr_lit_write2io(P_t *pads, Sfio_t *io, const char *s,
		       Pcharset char_set, const char *whatfn)
{
  ssize_t         n;
  Pstring      stack_s;
  Pstring     *tmp_s = &stack_s;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE3(pads->disc, "PDCI_cstr_lit_write2io args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_cstr(s), Pcharset2str(char_set), whatfn);
  stack_s.str = (char*)s;
  stack_s.len = strlen(s);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s, stack_s.len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_cstr_lit_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, const char *s,
			Pcharset char_set, const char *whatfn)
{
  Pstring      stack_s;
  Pstring     *tmp_s = &stack_s;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE3(pads->disc, "PDCI_cstr_lit_write2buf args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_cstr(s), Pcharset2str(char_set), whatfn);
  stack_s.str = (char*)s;
  stack_s.len = strlen(s);
  if (stack_s.len > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s, stack_s.len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
  return tmp_s->len;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_cstr_lit_write_xml_2io(P_t *pads, Sfio_t *io, const char *s, const char *tag, int indent, const char *whatfn)
{
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE2(pads->disc, "PDCI_cstr_lit_write_xml_2io args: s %s, whatfn = %s", P_qfmt_cstr(s), whatfn);
  PDCI_BASELIT_XML_OUT2IO("Pcstr_lit", "%s", P_fmt_cstr(s));
}

ssize_t
PDCI_cstr_lit_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full, const char *s,
			     const char *tag, int indent, const char *whatfn)
{
  ssize_t writelen;
  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE2(pads->disc, "PDCI_cstr_lit_write_xml_2buf args: s %s, whatfn = %s", P_qfmt_cstr(s), whatfn);
  PDCI_BASELIT_XML_OUT2BUF("Pcstr_lit", "%s", P_fmt_cstr(s));
}

ssize_t
PDCI_char_write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, Pbyte *val,
		   Pcharset char_set, const char *whatfn)
{
  Pchar         c;
  Pinv_val_fn   fn;

  PDCI_DISC_1P_CHECKS_RET_SSIZE(whatfn, io);
  P_TRACE3(pads->disc, "PDCI_char_write2io args: c %s, char_set = %s, whatfn = %s",
	     P_qfmt_char(*val), Pcharset2str(char_set), whatfn);
  if (pd->errCode == P_NO_ERR) {
    c = *val;
  } else {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pchar");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)&c))) {
      c = (pd->errCode == P_USER_CONSTRAINT_VIOLATION) ? *val : P_CHAR_DEF_INV_VAL;
    }
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      c = P_mod_ae_tab[(int)c]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  if (c != sfputc(io, c)) {
    P_WARN1(pads->disc, "%s: low-level sfputc failure", whatfn);
    return -1;
  }
  return 1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;
}

ssize_t
PDCI_char_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		    Pbase_pd *pd, Pbyte *val,
		    Pcharset char_set, const char *whatfn)
{
  Pchar         c;
  Pinv_val_fn   fn;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, buf, buf_full);
  P_TRACE3(pads->disc, "PDCI_char_write2buf args: c %s, char_set = %s, whatfn = %s",
	     P_qfmt_char(*val), Pcharset2str(char_set), whatfn);
  if (1 > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  if (pd->errCode == P_NO_ERR) {
    c = *val;
  } else {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pchar");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)&c))) {
      c = (pd->errCode == P_USER_CONSTRAINT_VIOLATION) ? *val : P_CHAR_DEF_INV_VAL;
    }
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      c = P_mod_ae_tab[(int)c]; /* convert to EBCDIC char */
      break;
    default:
      goto invalid_charset;
    }
  *buf = c;
  return 1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;
}

ssize_t
PDCI_char_write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, Pbyte *val,
			const char *tag, int indent, const char *whatfn)
{
  Pchar         c;
  Pinv_val_fn   fn;

  PDCI_DISC_1P_CHECKS_RET_SSIZE(whatfn, io);
  P_TRACE2(pads->disc, "PDCI_char_write_xml_2io args: c %s, whatfn = %s", P_qfmt_char(*val), whatfn);
  if (pd->errCode == P_NO_ERR) {
    c = *val;
  } else {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pchar");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)&c))) {
      c = (pd->errCode == P_USER_CONSTRAINT_VIOLATION) ? *val : P_CHAR_DEF_INV_VAL;
    }
  }
  PDCI_BASEVAL_XML_OUT2IO("Pchar", "%s", P_fmt_char(c));
}

ssize_t
PDCI_char_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			 Pbase_pd *pd, Pbyte *val,
			 const char *tag, int indent, const char *whatfn)
{
  ssize_t       writelen;
  Pchar         c;
  Pinv_val_fn   fn;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, buf, buf_full);
  P_TRACE2(pads->disc, "PDCI_char_write_xml_2buf args: c %s, whatfn = %s", P_qfmt_char(*val), whatfn);
  if (pd->errCode == P_NO_ERR) {
    c = *val;
  } else {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pchar");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)&c))) {
      c = (pd->errCode == P_USER_CONSTRAINT_VIOLATION) ? *val : P_CHAR_DEF_INV_VAL;
    }
  }
  PDCI_BASEVAL_XML_OUT2BUF("Pchar", "%s", P_fmt_char(c));
}

ssize_t
PDCI_string_FW_write2io(P_t *pads, Sfio_t *io,
			Pbase_pd *pd, Pstring *s,
			Pcharset char_set, const char *whatfn, size_t width)
{
  ssize_t       n;
  Pstring      *tmp_s = (Pstring*)s;
  Pinv_val_fn   fn;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE3(pads->disc, "PDCI_string_FW_write2io args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pstring_FW");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)s, width))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	PDCI_STRFILL(s, P_CHAR_DEF_INV_VAL, width);
      }
    }
  }
  if (s->len != width) {
    goto invalid_width;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s->str, s->len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;

 invalid_width:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_WIDTH, whatfn, 0);
  return -1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_string_FW_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			 Pbase_pd *pd, Pstring *s,
			 Pcharset char_set, const char *whatfn, size_t width)
{
  Pstring      *tmp_s = (Pstring*)s;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE3(pads->disc, "PDCI_string_FW_write2buf args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);

  if (width > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pstring_FW");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)s, width))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	PDCI_STRFILL(s, P_CHAR_DEF_INV_VAL, width);
      }
    }
  }
  if (s->len != width) {
    goto invalid_width;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s->str, s->len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
  return tmp_s->len;

 invalid_width:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_WIDTH, whatfn, 0);
  return -1;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_string_FW_write_xml_2io(P_t *pads, Sfio_t *io,
			     Pbase_pd *pd, Pstring *s,
			     const char *tag, int indent, const char *whatfn, size_t width)
{
  Pinv_val_fn   fn;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE2(pads->disc, "PDCI_string_FW_write_xml_2io args: s %s, whatfn = %s", P_qfmt_str(s), whatfn);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pstring_FW");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)s, width))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	PDCI_STRFILL(s, P_CHAR_DEF_INV_VAL, width);
      }
    }
  }
  if (s->len != width) {
    goto invalid_width;
  }
  PDCI_BASEVAL_XML_OUT2IO("Pstring_FW", "%s", P_fmt_str(s));

 invalid_width:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_WIDTH, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_string_FW_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			      Pbase_pd *pd, Pstring *s,
			      const char *tag, int indent, const char *whatfn, size_t width)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE2(pads->disc, "PDCI_string_FW_write_xml_2buf args: s %s, whatfn = %s", P_qfmt_str(s), whatfn);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, "Pstring_FW");
    if (!fn || (P_ERR == P_invoke_inv_val_fn(fn, pads, (void*)pd, (void*)s, width))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	PDCI_STRFILL(s, P_CHAR_DEF_INV_VAL, width);
      }
    }
  }
  if (s->len != width) {
    goto invalid_width;
  }
  PDCI_BASEVAL_XML_OUT2BUF("Pstring_FW", "%s", P_fmt_str(s));

 invalid_width:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_WIDTH, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_string_write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, Pstring *s,
		     Pcharset char_set, const char *inv_type, const char *whatfn, ...)
{
  ssize_t       n;
  Pstring      *tmp_s = (Pstring*)s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE3(pads->disc, "PDCI_string_write2io args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)s, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	s->len = 0;
      }
    }
    va_end(type_args);
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s->str, s->len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_string_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, Pstring *s,
		      Pcharset char_set, const char *inv_type, const char *whatfn, ...)
{
  Pstring      *tmp_s = (Pstring*)s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE3(pads->disc, "PDCI_string_write2buf args: s %s, char_set = %s, whatfn = %s",
	     P_qfmt_str(s), Pcharset2str(char_set), whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)s, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	s->len = 0;
      }
    }
    va_end(type_args);
  }
  if (tmp_s->len > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s->str, s->len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
  return tmp_s->len;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_string_write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, Pstring *s,
			  const char *tag, int indent, const char *inv_type, const char *whatfn, ...)
{
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, s);
  P_TRACE2(pads->disc, "PDCI_string_write_xml_2io args: s %s, whatfn = %s", P_qfmt_str(s), whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)s, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	s->len = 0;
      }
    }
    va_end(type_args);
  }
  PDCI_BASEVAL_XML_OUT2IO(inv_type, "%s", P_fmt_str(s));
}

ssize_t
PDCI_string_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, Pstring *s,
			   const char *tag, int indent, const char *inv_type, const char *whatfn, ...)
{
  ssize_t       writelen;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, s);
  P_TRACE2(pads->disc, "PDCI_string_write_xml_2buf args: s %s, whatfn = %s", P_qfmt_str(s), whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)s, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	s->len = 0;
      }
    }
    va_end(type_args);
  }
  PDCI_BASEVAL_XML_OUT2BUF(inv_type, "%s", P_fmt_str(s));
}

ssize_t
PDCI_date_time_FW_write2io(P_t *pads,
			   Sfio_t *io,
			   Pbase_pd *pd,
			   Puint32 *d,
			   Pcharset char_set,
			   const char *inv_type,
			   const char *whatfn,
			   const char *format_descr,
			   const char *tzone_descr,
			   const char *format,
			   Tm_zone_t *tzone,
			   size_t width,
			   ...)
{
  ssize_t       n;
  Pstring       s;
  Pstring      *tmp_s = &s;
  Pbyte         space;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, d);
  P_TRACE4(pads->disc, "PDCI_date_time_FW_write2io args: char_set = %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);

  tmset(tzone);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, width);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmttime(format, (time_t)(*d));
  s.len = strlen(s.str);
#if 0
  /* XXX_TODO: output format can differ from input format, so we cannot check if */
  /* output width is equal to input width, unless (1) the input and output formats are identical */
  /* and (2) the input format has no alternations.  At the moment, however, we are */
  /* not passed the input format, so we cannot do any check at all */
  if (tmp_s->len > width) { /* format does not fit in width, give up */
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_WIDTH, whatfn, 0);
    return -1;
  }
#endif
  switch (char_set)
    {
    case Pcharset_ASCII:
      space = P_ASCII_SPACE;
      break;
    case Pcharset_EBCDIC:
      space = P_EBCDIC_SPACE;
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s.str, s.len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
#if 0
  /* if format does not fill the FW, add spaces. */
  /* does not make sense to do this since the out format may have nothing to do with the FW */
  while (n < width) {
    if (-1 == sfputc(io, space)) {
      P_WARN1(pads->disc, "%s: low-level sfputc failure", whatfn);
      if (n > 0) {
	/* XXX_TODO try to back up ??? */
      }
      return -1;
    }
    n++;
  }
#endif
  return n;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_date_time_FW_write2buf(P_t *pads,
			    Pbyte *buf,
			    size_t buf_len,
			    int *buf_full,
			    Pbase_pd *pd,
			    Puint32 *d,
			    Pcharset char_set,
			    const char *inv_type,
			    const char *whatfn,
			    const char *format_descr,
			    const char *tzone_descr,
			    const char *format,
			    Tm_zone_t *tzone,
			    size_t width,
			    ...)
{
  Pstring       s;
  Pstring      *tmp_s = &s;
  Pbyte         space;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, d);
  P_TRACE4(pads->disc, "PDCI_date_time_FW_write2buf args: char_set = %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);

  tmset(tzone);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, width);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmttime(format, (time_t)(*d));
  s.len = strlen(s.str);
#if 0
  /* XXX_TODO: output format can differ from input format, so we cannot check if */
  /* output width is equal to input width, unless (1) the input and output formats are identical */
  /* and (2) the input format has no alternations.  At the moment, however, we are */
  /* not passed the input format, so we cannot do any check at all */
  if (tmp_s->len > width) { /* format does not fit in width, give up */
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_WIDTH, whatfn, 0);
    return -1;
  }
#endif
  if (tmp_s->len > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      space = P_ASCII_SPACE;
      break;
    case Pcharset_EBCDIC:
      space = P_EBCDIC_SPACE;
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s.str, s.len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
#if 0
  {
    Pbyte *buf2, *buf_end;
    /* if format does not fill the FW, add spaces. */
    /* does not make sense to do this since the out format may have nothing to do with the FW */
    buf2 = buf + tmp_s->len;
    buf_end = buf + width;
    while (buf2 < buf_end) {
      *buf2++ = space;
    }
  }
#endif
  return tmp_s->len;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_date_time_write2io(P_t *pads,
			Sfio_t *io,
			Pbase_pd *pd,
			Puint32 *d,
			Pcharset char_set,
			const char *inv_type,
			const char *whatfn,
			const char *format_descr,
			const char *tzone_descr,
			const char *format,
			Tm_zone_t *tzone,
			...)
{
  ssize_t       n;
  Pstring       s;
  Pstring      *tmp_s = &s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, d);
  P_TRACE4(pads->disc, "PDCI_date_time_write2io args: char_set = %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);

  tmset(tzone);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, tzone);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmttime(format, (time_t)(*d));
  s.len = strlen(s.str);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s.str, s.len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_date_time_write2buf(P_t *pads,
			 Pbyte *buf,
			 size_t buf_len,
			 int *buf_full,
			 Pbase_pd *pd,
			 Puint32 *d,
			 Pcharset char_set,
			 const char *inv_type,
			 const char *whatfn,
			 const char *format_descr,
			 const char *tzone_descr,
			 const char *format,
			 Tm_zone_t *tzone,
			 ...)
{
  Pstring       s;
  Pstring      *tmp_s = &s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, d);
  P_TRACE4(pads->disc, "PDCI_date_time_write2buf args: char_set = %s, whatfn = %s, format = %s, tzone = %s",
	   Pcharset2str(char_set), whatfn, format, tzone_descr);

  tmset(tzone);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, tzone);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmttime(format, (time_t)(*d));
  s.len = strlen(s.str);
  if (tmp_s->len > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s.str, s.len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
  return tmp_s->len;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_date_time_write_xml_2io(P_t *pads,
			     Sfio_t *io,
			     Pbase_pd *pd,
			     Puint32 *d,
			     const char *tag,
			     int indent,
			     const char *inv_type,
			     const char *whatfn,
			     const char *format_descr,
			     const char *tzone_descr,
			     const char *format,
			     Tm_zone_t *tzone,
			     ...)
{
  Pstring       s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, d);
  P_TRACE3(pads->disc, "PDCI_date_time_write_xml_2io args: whatfn = %s, format = %s, tzone = %s",
	   whatfn, format, tzone_descr);

  tmset(tzone);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, tzone);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmttime(format, (time_t)(*d));
  s.len = strlen(s.str);
  PDCI_BASEVAL_XML_OUT2IO(inv_type, "%s", P_fmt_str(&s));
}

ssize_t
PDCI_date_time_write_xml_2buf(P_t *pads,
			      Pbyte *buf,
			      size_t buf_len,
			      int *buf_full,
			      Pbase_pd *pd,
			      Puint32 *d,
			      const char *tag,
			      int indent,
			      const char *inv_type,
			      const char *whatfn,
			      const char *format_descr,
			      const char *tzone_descr,
			      const char *format,
			      Tm_zone_t *tzone,
			      ...)
{
  ssize_t       writelen;
  Pstring       s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_ARG_OR_DISC_ELT_CHECK(format, format_descr, whatfn);
  PDCI_ARG_OR_DISC_ELT_CHECK(tzone, tzone_descr, whatfn);
  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, d);
  P_TRACE3(pads->disc, "PDCI_date_time_write_xml_2buf args: whatfn = %s, format = %s, tzone = %s",
	   whatfn, format, tzone_descr);

  tmset(tzone);

  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, tzone);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmttime(format, (time_t)(*d));
  s.len = strlen(s.str);
  PDCI_BASEVAL_XML_OUT2BUF(inv_type, "%s", P_fmt_str(&s));
}

ssize_t
PDCI_ip_write2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, Puint32 *d,
		 Pcharset char_set, const char *inv_type, const char *whatfn, ...)
{
  ssize_t       n;
  Pstring       s;
  Pstring      *tmp_s = &s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, d);
  P_TRACE2(pads->disc, "PDCI_ip_write2io args: char_set = %s, whatfn = %s",
	   Pcharset2str(char_set), whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmtip4((*d), -1);
  s.len = strlen(s.str);
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s.str, s.len);
      break;
    default:
      goto invalid_charset;
    }
  n = sfwrite(io, (Void_t*)tmp_s->str, tmp_s->len);
  if (n != tmp_s->len) {
    P_WARN1(pads->disc, "%s: low-level sfwrite failure", whatfn);
    if (n > 0) {
      /* XXX_TODO try to back up ??? */
    }
    return -1;
  }
  return n;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_ip_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		  Pbase_pd *pd, Puint32 *d,
		  Pcharset char_set, const char *inv_type, const char *whatfn, ...)
{
  Pstring       s;
  Pstring      *tmp_s = &s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, d);
  P_TRACE2(pads->disc, "PDCI_ip_write2buf args: char_set = %s, whatfn = %s",
	     Pcharset2str(char_set), whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmtip4((*d), -1);
  s.len = strlen(s.str);
  if (tmp_s->len > buf_len) {
    (*buf_full) = 1;
    return -1;
  }
  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      tmp_s = &pads->stmp1;
      PDCI_A2E_STR_CPY(tmp_s, s.str, s.len);
      break;
    default:
      goto invalid_charset;
    }
  memcpy(buf, tmp_s->str, tmp_s->len);
  return tmp_s->len;

 invalid_charset:
  PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_CHARSET, whatfn, 0);
  return -1;

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, whatfn, "Memory alloc error");
  return -1;
}

ssize_t
PDCI_ip_write_xml_2io(P_t *pads, Sfio_t *io, Pbase_pd *pd, Puint32 *d,
		      const char *tag, int indent, const char *inv_type, const char *whatfn, ...)
{
  Pstring       s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_2P_CHECKS_RET_SSIZE(whatfn, io, d);
  P_TRACE1(pads->disc, "PDCI_ip_write_xml_2io args: whatfn = %s", whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmtip4((*d), -1);
  s.len = strlen(s.str);
  PDCI_BASEVAL_XML_OUT2IO(inv_type, "%s", P_fmt_str(&s));
}

ssize_t
PDCI_ip_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		       Pbase_pd *pd, Puint32 *d,
		       const char *tag, int indent, const char *inv_type, const char *whatfn, ...)
{
  ssize_t       writelen;
  Pstring       s;
  Pinv_val_fn   fn;
  va_list       type_args;

  PDCI_DISC_3P_CHECKS_RET_SSIZE(whatfn, buf, buf_full, d);
  P_TRACE1(pads->disc, "PDCI_ip_write_xml_2buf args: whatfn = %s", whatfn);
  if (pd->errCode != P_NO_ERR) {
    fn = PDCI_GET_INV_VAL_FN(pads, inv_type);
    va_start(type_args, whatfn);
    if (!fn || (P_ERR == fn(pads, (void*)pd, (void*)d, type_args))) {
      if (pd->errCode != P_USER_CONSTRAINT_VIOLATION) {
	(*d) = 0;
      }
    }
    va_end(type_args);
  }
  s.str = fmtip4((*d), -1);
  s.len = strlen(s.str);
  PDCI_BASEVAL_XML_OUT2BUF(inv_type, "%s", P_fmt_str(&s));
}

/* MISC WRITE FUNCTIONS */

ssize_t
PDCI_countX_write2io(P_t *pads, Sfio_t *io,
		     Pbase_pd *pd, Pint32  *val, Pcharset char_set, const char *whatfn,
		     Puint8 x, int eor_required, size_t count_max)
{
  return 0;
}

ssize_t
PDCI_countX_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
		      Pbase_pd *pd, Pint32  *val, Pcharset char_set, const char *whatfn,
		      Puint8 x, int eor_required, size_t count_max)
{
  return 0;
}

ssize_t
PDCI_countXtoY_write2io(P_t *pads, Sfio_t *io,
			Pbase_pd *pd, Pint32  *val, Pcharset char_set, const char *whatfn,
			Puint8 x, Puint8 y, size_t count_max)
{
  return 0;
}

ssize_t
PDCI_countXtoY_write2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			 Pbase_pd *pd, Pint32  *val, Pcharset char_set, const char *whatfn,
			 Puint8 x, Puint8 y, size_t count_max)
{
  return 0;

}

ssize_t
PDCI_countX_write_xml_2io(P_t *pads, Sfio_t *io,
			  Pbase_pd *pd, Pint32  *val, const char *tag, int indent,
			  const char *whatfn,
			  Puint8 x, int eor_required, size_t count_max)
{
  return 0;
}

ssize_t
PDCI_countX_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			   Pbase_pd *pd, Pint32  *val, const char *tag, int indent,
			   const char *whatfn,
			   Puint8 x, int eor_required, size_t count_max)
{
  return 0;
}

ssize_t
PDCI_countXtoY_write_xml_2io(P_t *pads, Sfio_t *io,
			     Pbase_pd *pd, Pint32  *val, const char *tag, int indent,
			     const char *whatfn,
			     Puint8 x, Puint8 y, size_t count_max)
{
  return 0;
}

ssize_t
PDCI_countXtoY_write_xml_2buf(P_t *pads, Pbyte *buf, size_t buf_len, int *buf_full,
			      Pbase_pd *pd, Pint32  *val, const char *tag, int indent,
			      const char *whatfn,
			      Puint8 x, Puint8 y, size_t count_max)
{
  return 0;

}
#endif /* P_CONFIG_WRITE_FUNCTIONS */

/* ================================================================================ */
/* INTERNAL MISC ROUTINES */

#if defined(__GNUC__) && (__GNUC__ >= 4) || defined(__CYGWIN__)
/*
 * sfstruse and sfstrseek both use '?' conditional exprs that can
 * return NULL.  The newest gcc does not like such exprs appearing in
 * argument positions that expect const void*, even if casting is
 * used.  As a workaround, we provide the following wrappers.
 */ 
char* PDCI_sfstr_use(Sfio_t* io) {
  return sfstruse(io);
}

char* PDCI_sfstr_seek2zero(Sfio_t* f) {
  return sfstrseek(f, 0, SEEK_SET);
}
#endif

Perror_t
PDCI_regexp_compile_cstr(P_t *pads, const char *regexp_str, Pregexp_t *regexp,
			 const char *err_prefix, const char *whatfn)
{
  regflags_t    c_flags;
  size_t        len;
  char          delim;
  const char   *end, *rdelim, *regexp_end;
  int           cret;

  PDCI_DISC_2P_CHECKS(whatfn, regexp_str, regexp);
#if ! CACHE_REGEX
#warning not caching REGEX
  if (regexp->valid) { /* cleanup before installing a new compiled regexp */
    regfree(&(regexp->preg));
    regexp->valid = 0;
  }
#endif
  len = strlen(regexp_str);
  regexp_end = regexp_str + len - 1;
  if (len < 3) {
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_REGEXP, whatfn,
		    "%s regular expression %s: expr of length %d cannot be a valid regexp",
		    err_prefix, P_qfmt_cstr_n(regexp_str, len), (int)len);
    goto any_err;
  }
  delim = regexp_str[0];
  if (delim == regexp_str[1]) {
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_REGEXP, whatfn,
		    "%s regular expression %s: invalid (empty pattern)",
		    err_prefix, P_qfmt_cstr_n(regexp_str, len));
    goto any_err;
  }
  for (rdelim = regexp_end; *rdelim != delim; rdelim--);
  if (rdelim == regexp_str) {
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_REGEXP, whatfn,
		    "%s regular expression %s: beginning delimiter %s has no ending %s",
		    err_prefix, P_qfmt_cstr_n(regexp_str, len), P_qfmt_char(delim), P_qfmt_char(delim));
    goto any_err;
  }
  /* initalize c_flags */
  cret = 0;
  c_flags = (REG_AUGMENTED|REG_EXTENDED|REG_DELIMITED|REG_MUSTDELIM|REG_LENIENT|REG_ESCAPE|REG_SPAN|REG_MULTIREF);
  for (end = regexp_end; end > rdelim; end--) {
    if (*end == 'i') {
      /*
       * Do case-insensitive pattern matching.
       */
      c_flags |= REG_ICASE;
      continue;
    }
#if 0
    /* need to work out the right semantics for this */
    if (*end == 'm') {
      /* 
       * Treat a record (or data region for discipline norec) containing
       * newlines as a set of records for the purposes of
       * matching "^" and "$".  That is, use newlines (as well as record boundaries)
       * as the boundaries delimited start ("^") or ("$") points.
       * Further, allow matching anywhere in the record scope (for record-based
       * discipline) or up to the first newline (for non-record-based discipline).
       * [XXX scope for norec case not implemented!]
       */
      c_flags |= REG_NEWLINE;
      continue;
    }
#endif
#if 0
    /* on by default since newlines not the same thing as records */
    if (*end == 's') {
      /*
       * Treat string as single line.  That is, change "." to
       * match any character whatsoever, even a newline, which
       * normally it would not match.
       */
      c_flags |= REG_SPAN;
      continue;
    }
#endif
    if (*end == 'x') {
      /*
       * Extend your pattern's legibility by permitting whitespace
       * and comments.
       *
       * Tells the regular expression parser to ignore whitespace that
       * is neither backslashed nor within a character class You can
       * use this to break up your regular expression into (slightly)
       * more readable parts.  The "#" character is also treated as a
       * metacharacter introducing a comment.  This also means that if
       * you want real whitespace or "#" characters in the pattern
       * (outside a character class, where they are unaffected by
       * "/x"), you'll either have to escape them or encode them using
       * octal or hex escapes.  Be careful not to include the pattern
       * delimiter in the comment -- there is no way of knowing you
       * did not intend to close the pattern early. 
       */
      c_flags |= REG_COMMENT;
      continue;
    }
    if (*end == 'f') {
      /*
       * First match found will do. 
       */
      c_flags |= REG_FIRST;
      continue;
    }
    if (*end == '?') {
      /*
       * Minimal match.
       */
      c_flags |= REG_MINIMAL;
      continue;
    }
    if (*end == 'l') {
      /*
       * No operators (treat entire regexp as a literal).
       */
      c_flags |= REG_LITERAL;
      continue;
    }
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_REGEXP, whatfn,
		    "%s regular expression %s: bad pattern modifier char: %s",
		    err_prefix, P_qfmt_cstr_n(regexp_str, len), P_qfmt_char(*end));
    cret = 1;
  }
  if (cret) goto any_err;
#if CACHE_REGEX
#warning caching REGEX
  regexp->preg = *regcache(regexp_str, c_flags, &cret);
#else
#warning not caching REGEX
  cret = regcomp(&(regexp->preg), regexp_str, c_flags);
#endif
  if (cret) {
    PDCI_report_err(pads, P_WARN_FLAGS, 0, P_INVALID_REGEXP, whatfn,
		    "%s regular expression %s: invalid",
		    err_prefix, P_qfmt_cstr_n(regexp_str, len));
    goto any_err;
  }
  regexp->valid = 1;
  return P_OK;

 any_err:
  regexp->valid = 0;
  return P_ERR;
}

/* XXX_FIX change this when gsf provides a regncomp */
Perror_t
PDCI_regexp_compile(P_t *pads, const Pstring *regexp_str, Pregexp_t *regexp,
		    const char *err_prefix, const char *whatfn)
{
  PDCI_DISC_2P_CHECKS(whatfn, regexp_str, regexp);
  PDCI_sfstr_seek2zero((pads)->tmp2);
  sfprintf((pads)->tmp2, "%.*s", regexp_str->len, regexp_str->str);
  return PDCI_regexp_compile_cstr(pads, PDCI_sfstr_use(pads->tmp2), regexp, err_prefix, whatfn);
}

Perror_t
PDCI_regexp_cleanup(P_t *pads, Pregexp_t *regexp, const char *whatfn)
{
  PDCI_DISC_1P_CHECKS(whatfn, regexp);
#if ! CACHE_REGEX
#warning not caching REGEX
  if (regexp->valid) {
    regfree(&(regexp->preg));
    regexp->valid = 0;
  }
#endif
  return P_OK;
}

int
PDCI_regexp_match(P_t *pads, Pregexp_t *regexp, Pbyte *begin, Pbyte *end,
		  regflags_t e_flags, Pcharset char_set)
{
  const char  *tmp_match_str = (const char*)begin;
  Pstring     *tmp;

  switch (char_set)
    {
    case Pcharset_ASCII:
      break;
    case Pcharset_EBCDIC:
      /* alloc the ASCII-converted chars in a temporary space */
      tmp = &pads->stmp1;
      PDCI_E2A_STR_CPY(tmp, begin, end-begin);
      tmp_match_str = tmp->str;
      break;
    default:
      /* should not get here, calling function should already have vetted char_set */
      return 0;
    }

  /* execute the compiled re against match_str.str */
#ifdef DEBUG_REGEX
  return regnexec(&(regexp->preg), tmp_match_str, end-begin, regexp->preg.re_nsub+1, regexp->match, e_flags);
#else
  return regnexec(&(regexp->preg), tmp_match_str, end-begin, 1, regexp->match, e_flags);
#endif

 fatal_alloc_err:
  PDCI_report_err(pads, P_FATAL_FLAGS, 0, P_ALLOC_ERR, "PDCI_regexp_match", "Memory alloc error");
  return 0;
}

Pbyte*
PDCI_findfirst(const Pbyte *begin, const Pbyte *end, Pbyte b)
{
  begin--;
  while (++begin < end) {
    if (*begin == b) return (Pbyte*)begin;
  }
  return 0;
}

Pbyte*
PDCI_findlast(const Pbyte *begin, const Pbyte *end, Pbyte b)
{
  while (--end >= begin) {
    if (*end == b) return (Pbyte*)end;
  }
  return 0;
}

int
PDCI_Plongest_chkErr(Puint32 nerr, int *consume)
{
  if (nerr) {
    *consume = 0;
    return 1;
  }
  return 0;
}

/* ================================================================================ */
/* INTERNAL REC.MASK INIT ROUTINES */

static Void_t* amask_make(Dt_t *dt, AMask_t *a, Dtdisc_t *disc)
{
  AMask_t *b;
  if ((b = oldof(0, AMask_t, 1, 0))) {
    b->typeName  = a->typeName;
    b->maskPtr  = a->maskPtr;
  }
  return b;
}

static void amask_free(Dt_t *dt, AMask_t *am, Dtdisc_t *disc)
{
  free(am);
}

static int amask_compare(Dt_t* dt, Void_t* key1, Void_t* key2, Dtdisc_t* disc){
  return strcmp(key1,key2);
}

static Dtdisc_t mask_map_set_disc = {
  DTOFFSET(AMask_t,typeName),   /* key      */
  -1,                           /* size     */
  DTOFFSET(AMask_t, link),      /* link     */
  (Dtmake_f)amask_make,         /* makef    */
  (Dtfree_f)amask_free,         /* freef    */
  (Dtcompar_f)amask_compare     /* comparef */
};

Perror_t PDCI_alloc_mask_map(P_t* pads,int* did_alloc)
{  
  if (pads->mask_map != NULL)
    { // Map already allocated.
      *did_alloc = 0;
      return P_OK;
    } 
 else
   {  
     pads->mask_map = dtopen(&mask_map_set_disc, Dtset);
     if (pads->mask_map == NULL)
       {
	 *did_alloc = 0;
	 return P_ERR;
       }
     *did_alloc = 1;
     return P_OK;
   }
}

Perror_t PDCI_dealloc_mask_map(P_t* pads){
  if (dtclose(pads->mask_map) == 0)
    return P_OK;

  return P_ERR;
}

void* PDCI_lookup_dyn_mask(P_t* pads, const char* s){
  AMask_t* am;

  if ((am = dtmatch(pads->mask_map,s)))
     { return am->maskPtr; }

  return NiL;
}

void *PDCI_insert_dyn_mask(P_t* pads, const char* tyname, void* maskPtr){
  AMask_t am; // Use stack-allocated AMask_t as discipline allocates new AMask_t-s on insert.
  
  am.typeName = tyname;
  am.maskPtr = maskPtr;
  
  return dtinsert(pads->mask_map, &am);
}
 
/* Generic version of recursive mask init function. Called from
 * generated m_rec_init functions.  
 *
 * N.B.: Calling this function otherwise is at the caller's own risk,
 * due to the precaurious use of void pointers. The real type of the
 * mask passed through the "mask" parameter and the real type of the
 * mask expected by parameter m_r_init **must** be the same.
 *
 * The approach used in this function (void pointers and function
 * pointers) is an alternative to the PCGEN_... approach.
 */
Perror_t PDCI_mask_rec_init(P_t* pads,void** mask, Pbase_m baseMask, 
			    mask_rec_init_f m_r_init, const char* tyname, ssize_t mask_size)
{
  void* tmp_m;
  // Lookup type name in table. 
  tmp_m = PDCI_lookup_dyn_mask(pads,tyname);

  if (tmp_m)
    {
      *mask = tmp_m;
      return P_OK;
    }

  // If not found, allocate and initialize new mask.
  tmp_m = malloc(mask_size);
  if (!tmp_m)
    {
      P_WARN(pads->disc, "** PADS runtime internal error: malloc of mask failed ***");
      return P_ERR;
    }
  *mask = tmp_m;
    
  // Add new mask to table. 
  // (Critical that this step come before next, or we'll cause infinite loop.)
  PDCI_insert_dyn_mask(pads,tyname,tmp_m);
      
  // Call mask init on new mask.
  return m_r_init(pads, tmp_m, baseMask);
}

/* ================================================================================ */
