#ifndef runHishFlag
#include "runHist.h"
#endif




int make_directory(char * add_mkdir, char * label, char * scheme, char * version, int const m, int const n, int const SWITCH[], char * err_msg);


int open_fruncate(char * err_msg, char * add, FILE ** fp);

void matXwrite(int m, int n, double *data[], FILE * fp_write);


void matYwrite(int m, int n, double *data[], FILE * fp_write);


void write_vec_real(int m, double data[], FILE * fp_write);
void write_vec_int(int m, int data[], FILE * fp_write);
