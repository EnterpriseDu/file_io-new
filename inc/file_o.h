#ifndef runHishFlag
#include "runHist.h"
#endif




int make_directory(char * add_mkdir, char * label, char * scheme, char * version, int const m, int const n, double const CONFIG[]);


void matXwrite(int m, int n, double *data[], FILE * fp_write);


void matYwrite(int m, int n, double *data[], FILE * fp_write);


void write_vec_real(int m, double data[], FILE * fp_write);
void write_vec_int(int m, int data[], FILE * fp_write);
