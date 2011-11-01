#include "sirius.h"
#define SEQ_MASK P_CheckAndSet
#define DEF_INPUT_FILE  "data/sirius"
#define ERR_FILE   "sirius_errors"
#define CLEAN_FILE "sirius_clean"

void cnvServiceTN(service_tn_t *stn){
  if ((some_service_tn_t == stn->tag) && (stn->val.some_service_tn_t == 0)) {
    stn->tag = none_service_tn_t;
  };
  return;
}
void cnvBillingTN(billing_tn_t *stn){
  if ((some_billing_tn_t == stn->tag) && (stn->val.some_billing_tn_t == 0)) {
    stn->tag = none_billing_tn_t;
  };
  return;
}
void cnvNLPServiceTN(nlp_service_tn_t *stn){
  if ((some_nlp_service_tn_t == stn->tag) && (stn->val.some_nlp_service_tn_t == 0)) {
    stn->tag = none_nlp_service_tn_t;
  };
  return;
}
void cnvNLPBillingTN(nlp_billing_tn_t *stn){
  if ((some_nlp_billing_tn_t == stn->tag) && (stn->val.some_nlp_billing_tn_t == 0)) {
    stn->tag = none_nlp_billing_tn_t;
  };
  return;
}
void cnvPhoneNumbers(entry_t *entry){
  cnvServiceTN(&entry->header.service_tn);
  cnvBillingTN(&entry->header.billing_tn);
  cnvNLPServiceTN(&entry->header.nlp_service_tn);
  cnvNLPBillingTN(&entry->header.nlp_billing_tn);
}

int main(int argc, char** argv) {
  P_t                  *p;
  Pio_disc_t           *io_disc;
  summary_header_t     header;
  summary_header_t_pd  header_pd;
  summary_header_t_m   header_m;
  entry_t              entry;
  entry_t_pd           pd;
  entry_t_m            mask;
  char                 *fname = DEF_INPUT_FILE;
  char                 *errname = ERR_FILE;
  char                 *cleanname  = CLEAN_FILE;
  Sfio_t*              errfile;
  Sfio_t*              cleanfile;
  Pbase_m              amask = SEQ_MASK;
  
  errfile = sfopen(0, errname, "w");
  cleanfile = sfopen(0, cleanname, "w");

  if (argc >= 2 ) {
    fname = argv[1];
  }
  error(0, "\nUsing input file %s", fname);

  if (argc >= 3 ) {
    amask = atoi(argv[2]);
  }

  io_disc = P_nlrec_noseek_make(0);
  if (!io_disc) {
    error(ERROR_FATAL, "\nFailed to install IO discipline nlrec_noseek");
  } else {
    error(0, "\nInstalled IO discipline nlrec_noseek");
  }

  if (P_ERR == P_open(&p, 0, io_disc)) {
    error(2, "*** P_open failed ***");
    return -1;
  }

  summary_header_t_init(p, &header);
  summary_header_t_pd_init(p, &header_pd);
  summary_header_t_m_init(p, &header_m, P_CheckAndSet);

  /* INIT entry -- must do this for all variable data types */
  entry_t_init(p, &entry);
  entry_t_pd_init(p, &pd);

  entry_t_m_init(p, &mask, P_CheckAndSet);
  mask.events.compoundLevel = P_Set;

  if (P_ERR == P_io_fopen(p, fname)) {
    error(2, "*** P_io_fopen failed ***");
    return -1;
  }

  /*
   * Try to read header
   */

  if (P_OK == summary_header_t_read(p, &header_m, &header_pd, &header)) {
    error(0, "reading header returned: OK");
    summary_header_t_write2io(p, cleanfile, &header_pd, &header);
  } else {
    error(2, "reading header returned: error");
  }

  /*
   * Try to read each line of data
   */
  while (!P_io_at_eof(p)) {
    entry_t_read(p, &mask, &pd, &entry);
    if (pd.nerr > 0) {
      entry_t_write2io(p, errfile, &pd, &entry);
    } else {
      cnvPhoneNumbers(&entry);      
      if (entry_t_verify(&entry)) {
        entry_t_write2io(p, cleanfile, &pd, &entry);
      } else {
	error(2, "Data transform failed.");
      }
    }
  }

  if (P_ERR == P_io_close(p)) {
    error(2, "*** P_io_close failed ***");
    return -1;
  }

  if (P_ERR == P_close(p)) {
    error(2, "*** P_close failed ***");
    return -1;
  }

  return 0;
}
