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
 * Template: pglx_linear_query.h 
 * Incrementally load PADS source using linear-streaming arrays, run Galax query,
 * and serialize result in XML.
 */

#include "pads.h"
#include "pglx.h"
#include <time.h>

#ifndef PDCI_MacroArg2String
#define PDCI_MacroArg2String(s) #s
#endif

#ifndef READ_MASK
#define READ_MASK P_CheckAndSet
#endif

#ifndef COPY_STRINGS
#  define COPY_STRINGS 1
#endif 

#ifndef EXTRA_READ_ARGS
#  define EXTRA_READ_ARGS
#endif

#ifndef EXTRA_HDR_READ_ARGS
#  define EXTRA_HDR_READ_ARGS
#endif

#ifndef DEF_INPUT_FILE
#  define DEF_INPUT_FILE "/dev/stdin"
#endif

#ifndef DEF_OUTPUT_FILE_1
#  define DEF_OUTPUT_FILE_1 ""
#endif

#ifndef DEF_OUTPUT_FILE_2
#  define DEF_OUTPUT_FILE_2 ""
#endif

#ifndef MAX_RECS
#  define MAX_RECS 0
#endif

#define exit_on_error(_Expr, _Msg) {err = _Expr; if (err != 0) {error(2, "%s: %s\n", _Msg, galax_error_string); exit(-1); }}

#define MAX_NODES 100000
#define MAX_ELTS 100000

#ifndef EXTRA_ARGS
#  define EXTRA_ARGS
#endif

int main(int argc, char** argv) {
  P_t*          pads;
  Pdisc_t       my_disc = Pdefault_disc;
  Pio_disc_t  	*io_disc = 0;
  PADS_TY( )    rep;
  PADS_TY(_pd)  pd ;
  PADS_TY(_m)   m;
  PDCI_node_t   *linear_node;

  my_disc.out_formats.timestamp = "%Y-%m-%dT%H:%M:%S"; 
  my_disc.out_formats.timestamp_explicit = "%Y-%m-%dT%H:%M:%S"; 

#ifdef PADS_HDR_TY
  PADS_HDR_TY( )    hdr_rep;
  PADS_HDR_TY(_pd)  hdr_pd;
  PADS_HDR_TY(_m)   hdr_m;
#endif /* PADS_HDR_TY */
  Sfio_t       *io;
  char         *inName  = 0;
  char         *queryName = 0;

  galax_err err;
  item doc;
  itemlist docitems, monitems;
  processing_context pc;
  compiled_prolog cp;
  compiled_module cm;
  prepared_prolog pp;
  external_context exc;

  /* Galax configuration options */
  int monitor_flag = 0; 
  char *monitor_out;
  char *file_out;
  FILE *file_fp;
  int sbdo_flag = 0;
  int sbdo_kind = SBDO_AdHoc;; 

  clock_t start, finish, duration;

  monitor_out = malloc(strlen(argv[0]) + strlen(".out"));
  file_out = malloc(strlen(argv[0]) + strlen(".xml"));
  strcpy(monitor_out,argv[0]); strcat(monitor_out,".out");
  strcpy(file_out,argv[0]); strcat(file_out,".xml");

#ifdef PRE_LIT_LWS
  my_disc.pre_lit_lws = PRE_LIT_LWS;
#endif
#ifdef WSPACE_OK
  my_disc.flags |= (Pflags_t)P_WSPACE_OK;
#endif
#ifdef COPY_STRINGS
  my_disc.copy_strings = 1;
#endif

#ifdef TIMESTAMP_IN_FMT
  my_disc.in_formats.timestamp = TIMESTAMP_IN_FMT;
#endif
#ifdef DATE_IN_FMT
  my_disc.in_formats.date = DATE_IN_FMT;
#endif
#ifdef TIME_IN_FMT
  my_disc.in_formats.time = TIME_IN_FMT;
#endif

#ifdef TIMESTAMP_EXPLICIT_OUT_FMT
  my_disc.out_formats.timestamp_explicit = TIMESTAMP_EXPLICIT_OUT_FMT;
#endif
#ifdef TIMESTAMP_OUT_FMT
  my_disc.out_formats.timestamp = TIMESTAMP_OUT_FMT;
#endif
#ifdef DATE_EXPLICIT_OUT_FMT
  my_disc.out_formats.date_explicit = DATE_EXPLICIT_OUT_FMT;
#endif
#ifdef DATE_OUT_FMT
  my_disc.out_formats.date = DATE_OUT_FMT;
#endif
#ifdef TIME_EXPLICIT_OUT_FMT
  my_disc.out_formats.time_explicit = TIME_EXPLICIT_OUT_FMT;
#endif
#ifdef TIME_OUT_FMT
  my_disc.out_formats.time = TIME_OUT_FMT;
#endif

#ifdef IN_TIME_ZONE
  my_disc.in_time_zone = IN_TIME_ZONE;
  error(0, "Note: set my_disc.in_time_zone to \"%s\"\n", IN_TIME_ZONE);
#endif
#ifdef OUT_TIME_ZONE
  my_disc.out_time_zone = OUT_TIME_ZONE;
  error(0, "Note: set my_disc.out_time_zone to \"%s\"\n", OUT_TIME_ZONE);
#endif

  /* When linking with the Galax library, which contains a custom O'Caml runtime system, 
     it is necessary to call galax_init first, so the runtime is initialized and then 
     can delegate control back to the C program 
  */
  galax_init();

#ifdef IO_DISC_MK
  if (!(io_disc = IO_DISC_MK)) {
    error(ERROR_FATAL, "IO discipline make call [ " PDCI_MacroArg2String(IO_DISC_MK) " ] failed");
  }
#ifdef IO_DISC_DESCR
  else { 
    error(0, "Installed " IO_DISC_DESCR);
  }
#endif
#endif

  if (argc >= 2) {
    inName = argv[1];
  } else {
    inName = DEF_INPUT_FILE;
  }
  if (argc >= 3) {
    queryName = argv[2];
  } else {
    queryName = DEF_QUERY_FILE;
  }

  /* The remaining flags are for galax */
  {
    int i;
    for (i = 3; i < argc; i++) { 
      if (strcmp(argv[i], "-output-monitor") == 0) {
	monitor_flag = 1;
	i++;
	if (i < argc) {
	  monitor_out = argv[i];
	} else error(2, "Usage: -output-monitor <filename>");
      }
      else if (strcmp(argv[i], "-output-xml") == 0) {
	i++;
	if (i < argc) {
	  file_out = argv[i];
	} else error(2, "Usage: -output-xml <filename>");
      }
      else if  (strcmp(argv[i], "-sbdo") == 0) { 
	sbdo_flag = 1;
	i++;
	if (i < argc) {
	  if (strcmp(argv[i], "remove") == 0) sbdo_kind = SBDO_Remove;
	  else if (strcmp(argv[i], "preserve") == 0) sbdo_kind = SBDO_Preserve;
	  else if (strcmp(argv[i], "adhoc") == 0)	sbdo_kind = SBDO_AdHoc;
	  else if (strcmp(argv[i], "tidy") == 0) sbdo_kind = SBDO_Tidy;
	  else if (strcmp(argv[i], "duptidy") == 0) sbdo_kind = SBDO_DupTidy;
	  else if (strcmp(argv[i], "sloppy") == 0) sbdo_kind = SBDO_Sloppy;
	  else error(2, "Usage: -sbdo [remove, preserve, adhoc, tidy, duptidy, sloppy]");
	} else error(2, "Usage: -sbdo [remove, preserve, adhoc, tidy, duptidy, sloppy]");
      }
    }
  }
  /* Open files */	
  if (!(file_fp = fopen(file_out, "w"))) { error(2, "Cannot open %s\n", file_out); exit(-1); }

  /* Initialize Galax flags */
  exit_on_error(galax_default_processing_context(&pc), "galax_default_processing_context");
  exit_on_error(galax_set_serialization_kind(pc, Serialize_As_Well_Formed), "galax_set_serialization_kind");
  if (sbdo_flag)
    exit_on_error(galax_set_sbdo_kind(pc, sbdo_kind), "galax_set_sbdo_kind");
  if (monitor_flag) {
    exit_on_error(galax_set_monitor_mem(pc, 1), "galax_set_monitor_mem");
    exit_on_error(galax_set_monitor_time(pc, 1), "galax_set_monitor_time");
  }

  if (P_ERR == P_open(&pads, &my_disc, io_disc)) {
    error(2, "*** P_open failed ***");
    exit(-1);
  }
  io = P_fopen(inName,"r");
  if (!io) {
    error(2, "*** P_fopen failed ***");
    exit(-1);
  }
  if (P_ERR == P_io_set(pads, io)) {
    error(2, "*** P_io_set failed ***");
    exit(-1);
  }

  /* Initialize nodeid generator */
  PDCI_ID_RESET(pads,0);

  NodeMM_initMM(pads, MAX_NODES);  
  
  /* End initialize pads-galax */

  if (P_ERR == PADS_TY(_init)(pads, &rep)) {
    error(ERROR_FATAL, "*** representation initialization failed ***");
  }
  if (P_ERR == PADS_TY(_pd_init)(pads, &pd)) {
    error(ERROR_FATAL, "*** parse description initialization failed ***");
  }
  /* init mask -- must do this! */
  PADS_TY(_m_init)(pads, &m, READ_MASK);

#ifdef PADS_HDR_TY
  if (P_ERR == PADS_HDR_TY(_init)(pads, &hdr_rep)) {
    error(ERROR_FATAL, "*** header representation initialization failed ***");
  }
  if (P_ERR == PADS_HDR_TY(_pd_init)(pads, &hdr_pd)) {
    error(ERROR_FATAL, "*** header parse description initialization failed ***");
  }
  /* init mask -- must do this! */
  PADS_HDR_TY(_m_init)(pads, &hdr_m, P_CheckAndSet);
#endif /* PADS_HDR_TY */

#ifdef PADS_HDR_TY
  /*
   * Try to read header
   */
  if (!P_io_at_eof(pads)) {
    exit_on_error(galax_start_monitor_call(pc, "pads_header_read"), "galax_start_monitor_call");
    if (P_OK != PADS_HDR_TY(_read)(pads, &hdr_m, &hdr_pd, &hdr_rep EXTRA_HDR_READ_ARGS )) {
      error(2, "<note>header read returned error</note>");
    } else {
      exit_on_error(galax_end_monitor_call(pc), "galax_end_monitor_call");
      error(2, "<note>header read returned OK</note>");
    }
  }
#endif /* PADS_HDR_TY */

 {
      char *vars[0];
      itemlist vals[0]; 
      char *input = "";

      // Create the new linear node.
      exit_on_error(galax_start_monitor_call(pc, "pads_mk_top_node"), "galax_start_monitor_call");
      PDCI_MK_TOP_NODE_NORET (linear_node, &PADS_TY(_node_vtable), pads, "padsns:PSource", &m, &pd, &rep, "main");
      PADS_TY(_linearNode_init)(linear_node  EXTRA_ARGS);

      exit_on_error(galax_end_monitor_call(pc), "galax_end_monitor_call");

      exit_on_error(galax_load_standard_library(pc, &cp), "galax_load_standard_library");
      exit_on_error(padsDocument(pc, inName, PDCI_source, (nodeRep)linear_node, &doc), "padsDocument");
      docitems = itemlist_cons(doc, itemlist_empty()); 
      exit_on_error(galax_import_main_module(cp, ExternalContextItem, Buffer_Input, input, &cm), "galax_import_main_module");
      exit_on_error(galax_build_external_context(pc, docitems, itemlist_empty(), vars, vals, 0, &exc), "galax_build_external_context");
      exit_on_error(galax_eval_prolog(cm->compiled_prolog, exc, &pp), "galax_eval_prolog");
      exit_on_error(galax_eval_statement(pp, File_Input, queryName, &docitems), "galax_eval_statement"); 
      
      /*      if (is_empty(docitems)) error(2, "*** Result is empty") ;
	      else  */
      {
	char *result;
	exit_on_error(galax_serialize_to_string(pc,docitems,&result), "galax_serialize_to_string");
	fprintf(file_fp,"%s",result);
	fflush(file_fp);
	fclose(file_fp);
      }
 }
  /* 
   * The linear code doesn't use rep->elts, pd->elts, 
   * but it does use the length variables. Therefore,
   * we need to reset them to 0 before cleanup.
   */
  if (P_ERR == P_io_close(pads)) {
    error(ERROR_FATAL, "*** P_io_close failed ***");
  }
  rep.length = 0;
  if (P_ERR == PADS_TY(_cleanup)(pads, &rep)) {
    error(ERROR_FATAL, "** representation cleanup failed **");
  }
  pd.length = 0;
  if (P_ERR == PADS_TY(_pd_cleanup)(pads, &pd)) {
    error(ERROR_FATAL, "** parse descriptor cleanup failed **");
  }
  if (P_ERR == P_close(pads)) {
    error(ERROR_FATAL, "*** P_close failed ***");
  }
  sfclose(io);

  NodeMM_freeMM(pads);

  /*
  result_node = Puint32_node_new(linear_node,"result",NULL,NULL,NULL,"element","main");
  (result_node->vt->cachedNode_init)(result_node);
  result_node->child_cache[0]= PGLX_generic_kth_child_named((nodeRep)linear_node,0,argv[3]);
  result_node->child_cache[1]= result_node->child_cache[0];

  printf("Forming document ....\n");
  err = padsDocument("pd.xml", (nodeRep)result_node, &doc2);
  exit_on_error(err); 
  docitems = itemlist_cons(doc2, itemlist_empty()); 
  err = galax_serialize_to_stdout(docitems);
  exit_on_error(err);
  */

  /*
  printf("2nd Traversal.\n");

  // create a second cursor over the data:
  err = padsDocument(argv[1], (nodeRep)linear_node, &doc2);
  exit_on_error(err); 

  docitems = itemlist_cons(doc2, itemlist_empty()); 
  err = galax_serialize_to_stdout(docitems);
  exit_on_error(err);
  */

  //  printf("\nFinished Traversals.\n");

  return 0;
}
