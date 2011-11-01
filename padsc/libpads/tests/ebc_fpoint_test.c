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
 *  ebc_fpoint_test: tests
 *       Pebc_fpoint8_read
 *       Pebc_ufpoint8_read
 *       Pebc_fpoint16_read
 *       Pebc_ufpoint16_read
 *       Pebc_fpoint32_read
 *       Pebc_ufpoint32_read
 *       Pebc_fpoint64_read
 *       Pebc_ufpoint64_read
 */

#include "pads-internal.h" /* for testing - normally do not include internal */

P_NOGEN

#define NEXT_REC do {\
  if (strncmp(argv1, "norec", 5) == 0) { \
    if (P_ERR == Pe_char_lit_scan1(pads, '\n', 1, 0, &bytes_skipped)) { \
      error(2, "Could not find EOR (newline), ending program"); \
      goto done; \
    } \
  } else { \
    if (P_ERR == P_io_next_rec(pads, &bytes_skipped)) { \
      error(2, "Could not find EOR (newline), ending program"); \
      goto done; \
    } \
  } \
  ultmp = bytes_skipped; \
  error(0, "next_rec returned bytes_skipped = %ld", ultmp); \
} while (0)

int main(int argc, char** argv) {
  P_t*          pads;
  Pio_disc_t*  io_disc;
  Pfpoint8     f8;
  Pfpoint16    f16;
  Pfpoint32    f32;
  Pfpoint64    f64;
  Pufpoint8    uf8;
  Pufpoint16   uf16;
  Pufpoint32   uf32;
  Pufpoint64   uf64;
  Pdisc_t      my_disc = Pdefault_disc;
  Pbase_m      m       = P_CheckAndSet;
  Pbase_pd     pd;
  size_t          bytes_skipped;
  unsigned long   ultmp;
  const char     *argv1;

  my_disc.flags |= (Pflags_t)P_WSPACE_OK;

  if ((argc != 1) && (argc != 2)) {
    goto usage;
  }

  if (argc == 1) {
    argv1 = "fwrec_noseek";
  } else {
    argv1 = argv[1];
  }
  if (strcmp(argv1, "fwrec") == 0) {
    io_disc = P_fwrec_make(0, 25, 1);
  } else if (strcmp(argv1, "ctrec") == 0) {
    io_disc = P_ctrec_make(P_EBCDIC_NEWLINE, 0);
  } else if (strcmp(argv1, "norec") == 0) {
    io_disc = P_norec_make(0);
  } else if (strcmp(argv1, "fwrec_noseek") == 0) {
    io_disc = P_fwrec_noseek_make(0, 25, 1);
  } else if (strcmp(argv1, "ctrec_noseek") == 0) {
    io_disc = P_ctrec_noseek_make(P_EBCDIC_NEWLINE, 0);
  } else if (strcmp(argv1, "norec_noseek") == 0) {
    io_disc = P_norec_noseek_make(0);
  } else {
    goto usage;
  }
  if (!io_disc) {
    error(ERROR_FATAL, "\nFailed to install IO discipline %s", argv1);
  } else {
    error(0, "\nInstalled IO discipline %s", argv1);
  }

  if (P_ERR == P_libopen(&pads, &my_disc, io_disc, 1)) {
    error(2, "*** P_libopen failed ***");
    return -1;
  }
  if (P_ERR == P_io_fopen(pads, "../../data/ex_data.ebc_test")) {
    error(2, "*** P_io_fopen failed ***");
    return -1;
  }

  if (P_OK == Pebc_fpoint8_read(pads, &m, &pd, &f8, 3, 4) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint8_read(pads, &m, &pd, &f8, 3, 0)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f8.num, (unsigned long)f8.denom);
    if (f8.num != 0) {
      error(0, "XXX failure: num should be %ld XXX", (long)0);
      return -1;
    }
    if (f8.denom != 1) {
      error(0, "XXX failure: denom should be 1 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint8_read(pads, &m, &pd, &f8, 0, 1) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint8_read(pads, &m, &pd, &f8, 3, 1)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f8.num, (unsigned long)f8.denom);
    if (f8.num != P_MIN_INT8) {
      error(0, "XXX failure: num should be %ld XXX", (long)P_MIN_INT8);
      return -1;
    }
    if (f8.denom != 10) {
      error(0, "XXX failure: denom should be 10 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint8_read(pads, &m, &pd, &f8, 3, 2) || pd.errCode != P_RANGE) {
    error(0, "XXX failure: did not get expected RANGE error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint8_read(pads, &m, &pd, &f8, 3, 2)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f8.num, (unsigned long)f8.denom);
    if (f8.num != P_MAX_INT8) {
      error(0, "XXX failure: num should be %ld XXX", (long)P_MAX_INT8);
      return -1;
    }
    if (f8.denom != 100) {
      error(0, "XXX failure: denom should be 100 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_ufpoint8_read(pads, &m, &pd, &uf8, 3, 2) || pd.errCode != P_RANGE) {
    error(0, "XXX failure: did not get expected RANGE error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_ufpoint8_read(pads, &m, &pd, &uf8, 3, 2)) {
    error(0, "Read ebc ufpoint: num %lu denom %lu", (unsigned long)uf8.num, (unsigned long)uf8.denom);
    if (uf8.num != P_MAX_UINT8) {
      error(0, "XXX failure: num should be %lu XXX", (unsigned long)P_MAX_UINT8);
      return -1;
    }
    if (uf8.denom != 100) {
      error(0, "XXX failure: denom should be 100 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint16_read(pads, &m, &pd, &f16, 5, 5) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint16_read(pads, &m, &pd, &f16, 5, 4)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f16.num, (unsigned long)f16.denom);
    if (f16.num != P_MIN_INT16) {
      error(0, "XXX failure: num should be %ld XXX", (long)P_MIN_INT16);
      return -1;
    }
    if (f16.denom != 10000) {
      error(0, "XXX failure: denom should be 10,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint16_read(pads, &m, &pd, &f16, 10, 2) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint16_read(pads, &m, &pd, &f16, 5, 4)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f16.num, (unsigned long)f16.denom);
    if (f16.num != P_MAX_INT16) {
      error(0, "XXX failure: num should be %ld XXX", (long)P_MAX_INT16);
      return -1;
    }
    if (f16.denom != 10000) {
      error(0, "XXX failure: denom should be 10,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_ufpoint16_read(pads, &m, &pd, &uf16, 5, 4) || pd.errCode != P_RANGE) {
    error(0, "XXX failure: did not get expected RANGE error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_ufpoint16_read(pads, &m, &pd, &uf16, 5, 4)) {
    error(0, "Read ebc ufpoint: num %lu denom %lu", (unsigned long)uf16.num, (unsigned long)uf16.denom);
    if (uf16.num != P_MAX_UINT16) {
      error(0, "XXX failure: num should be %lu XXX", (unsigned long)P_MAX_UINT16);
      return -1;
    }
    if (uf16.denom != 10000) {
      error(0, "XXX failure: denom should be 10,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint32_read(pads, &m, &pd, &f32, 11, 9) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint32_read(pads, &m, &pd, &f32, 10, 9)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f32.num, (unsigned long)f32.denom);
    if (f32.num != P_MIN_INT32) {
      error(0, "XXX failure: num should be %ld XXX", (long)P_MIN_INT32);
      return -1;
    }
    if (f32.denom != 1000000000UL) {
      error(0, "XXX failure: denom should be 1,000,000,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint32_read(pads, &m, &pd, &f32, 10, 29) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint32_read(pads, &m, &pd, &f32, 10, 9)) {
    error(0, "Read ebc fpoint: num %ld denom %lu", (long)f32.num, (unsigned long)f32.denom);
    if (f32.num != P_MAX_INT32) {
      error(0, "XXX failure: num should be %ld XXX", (long)P_MAX_INT32);
      return -1;
    }
    if (f32.denom != 1000000000UL) {
      error(0, "XXX failure: denom should be 1,000,000,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_ufpoint32_read(pads, &m, &pd, &uf32, 11, 9) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_ufpoint32_read(pads, &m, &pd, &uf32, 10, 9)) {
    error(0, "Read ebc ufpoint: num %lu denom %lu", (unsigned long)uf32.num, (unsigned long)uf32.denom);
    if (uf32.num != P_MAX_UINT32) {
      error(0, "XXX failure: num should be %lu XXX", (unsigned long)P_MAX_UINT32);
      return -1;
    }
    if (uf32.denom != 1000000000UL) {
      error(0, "XXX failure: denom should be 1,000,000,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint64_read(pads, &m, &pd, &f64, 19, 19) || pd.errCode != P_RANGE) {
    error(0, "XXX failure: did not get expected RANGE error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint64_read(pads, &m, &pd, &f64, 19, 19)) {
    error(0, "Read ebc fpoint: num %lld denom %llu", (long long)f64.num, (unsigned long long)f64.denom);
    if (f64.num != P_MIN_INT64) {
      error(0, "XXX failure: num should be %lld XXX", (long long)P_MIN_INT64);
      return -1;
    }
    if (f64.denom != 10000000000000000000ULL) {
      error(0, "XXX failure: denom should be 10,000,000,000,000,000,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_fpoint64_read(pads, &m, &pd, &f64, 0, 19) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_fpoint64_read(pads, &m, &pd, &f64, 19, 19)) {
    error(0, "Read ebc fpoint: num %lld denom %llu", (long long)f64.num, (unsigned long long)f64.denom);
    if (f64.num != P_MAX_INT64) {
      error(0, "XXX failure: num should be %lld XXX", (long long)P_MAX_INT64);
      return -1;
    }
    if (f64.denom != 10000000000000000000ULL) {
      error(0, "XXX failure: denom should be 10,000,000,000,000,000,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

  if (P_OK == Pebc_ufpoint64_read(pads, &m, &pd, &uf64, 21, 19) || pd.errCode != P_BAD_PARAM) {
    error(0, "XXX failure: did not get expected BAD_PARAM error XXX");
    return -1;
  }
  NEXT_REC;

  if (P_OK == Pebc_ufpoint64_read(pads, &m, &pd, &uf64, 20, 19)) {
    error(0, "Read ebc ufpoint: num %llu denom %llu", (unsigned long long)uf64.num, (unsigned long long)uf64.denom);
    if (uf64.num != P_MAX_UINT64) {
      error(0, "XXX failure: should be %llu XXX", (unsigned long long)P_MAX_UINT64);
      return -1;
    }
    if (uf64.denom != 10000000000000000000ULL) {
      error(0, "XXX failure: denom should be 10,000,000,000,000,000,000 XXX");
      return -1;
    } 
  } else { return -1; } 
  NEXT_REC;

 done:
  if (P_ERR == P_io_close(pads)) {
    error(2, "*** P_io_close failed ***");
    return -1;
  }

  if (P_ERR == P_close(pads)) {
    error(2, "*** P_close failed ***");
    return -1;
  }

  return 0;

 usage:
  error(2, "\nUsage: %s [ <io-disc-name> ]\n\n\twhere <io-disc-name> is one of: fwrec, ctrec, norec, fwrec_noseek, ctrec_noseek, norec_noseek\n", argv[0]);
  return -1;
}
