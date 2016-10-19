#define runHistFlag 1

typedef struct runNode{
  int *trouble0, *trouble1;
  char RcstrState[6];
  double RcstrErr[12];
  double time[2];  /* time[0] is the current time step, i.e. tau
		    * time[1] is the CPU time */
  struct runNode * next;
} runNode;


typedef struct{
  runNode * head, * current, * tail;
  int length;
} runHist;

typedef runHist runList;


void init_runList(runHist * runhist);

void insert_runList(runHist * runhist);

int locate_runList(int p, runHist * runhist);

void delete_runList(runHist * runhist);

double write_runHist(runHist * runhist, FILE * fp, int adp, double scaling, int flag);
