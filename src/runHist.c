#include <stdio.h>
#include <stdlib.h>

#ifndef runHistFlag
#include "runHist.h"
#endif



int insert_runHist(runHist * runhist)
{
  int j;

  if(runhist->tail)
  {
    runhist->tail->next = (runNode *)malloc(sizeof(runNode));
    if(!runhist->tail->next)
      return runhist->length+1;
    runhist->tail = runhist->tail->next;
  }
  else
  {
    runhist->tail = (runNode *)malloc(sizeof(runNode));
    if(!runhist->tail)
      return runhist->length+1;
    runhist->head = runhist->tail;
  }
  ++(runhist->length);

  runhist->tail->trouble0 = NULL;
  runhist->tail->trouble1 = NULL;
  runhist->tail->next = NULL;
  for(j = 0; j < 6; ++j)
    runhist->tail->RcstrState[j] = '#';
  for(j = 0; j < 12; ++j)
    runhist->tail->RcstrErr[j] = 0.0;
  runhist->tail->time[0] = 0.0;
  runhist->tail->time[1] = 0.0;

  return 0;
}

/*
 * p = 0, 1, 2,..., lenght-1
 */
int locate_runHist(int p, runHist * runhist)
{
  int j, count = 1;
  runNode * point;

  point = runhist->head;
  for(j = 0; j < p; ++j)
    if(point->next)
    {
      point = point->next;
      ++count;
    }
    else
      return count;//the link has only %d(count-1) compunonts while trying to reach [p]
  runhist->current = point;

  return 0;
}

void delete_runHist(runHist * runhist)
{
  if(!runhist->length)
    return;

  while(runhist->length > 1)
  {
    /*
     * runhist[lenght-2] is the node immediately ahead of the tail
     * at the last step(length == 2), current == head
     */
    locate_runHist(runhist->length-2, runhist);

    if(runhist->tail->trouble0)
      free(runhist->tail->trouble0);
    if(runhist->tail->trouble1)
      free(runhist->tail->trouble1);
    free(runhist->tail);

    runhist->tail = runhist->current;
    --(runhist->length);

    runhist->tail->next = NULL;
  }

  runhist->tail = NULL;
  if(runhist->head->trouble0)
    free(runhist->head->trouble0);
  if(runhist->head->trouble1)
    free(runhist->head->trouble1);
  free(runhist->head);

  runhist->head = NULL;
  locate_runHist(0, runhist);
}


void init_runHist(runHist * runhist)
{
  delete_runHist(runhist);

  runhist->length = 0;
  runhist->head = NULL;
  runhist->current = NULL;
  runhist->tail = NULL;
}


int check_runHist(runHist * runhist)
{
  int count = 0;
  
  runhist ->current = runhist->head;
  
  while(runhist->current)
  {    
    ++count;
    runhist->current = runhist->current->next;
  }

  
  return runhist->length - count;
}


double write_runHist(runHist * runhist, FILE * fp_write, int flag_all, int flag_time[4], int flag_extra[2], int adp, double scaling, char split)
{
  double sum_tau = 0.0, sum_cpu = 0.0;
  int k = 0;

  runhist->current = runhist->head;

  if(flag_all)
    fprintf(fp_write, "\n k  \t|\ttau\t|\tT\t|\tcurrent cpu time\t|\ttotal cpu time\n");
  while(runhist->current)
  {
    runhist->current->time[0] = runhist->current->time[0]/scaling;
    sum_tau += runhist->current->time[0];
    sum_cpu += runhist->current->time[1];
    if(!flag_all)
    {
      runhist->current = runhist->current->next;
      //printf("%d\n", ++k);
      continue;
    }

    fprintf(fp_write, "\n%04d\t%c", ++k, split);
    if(flag_time[0])
      fprintf(fp_write, "\t%3.12lf", runhist->current->time[0]);
    if(flag_time[2])
      fprintf(fp_write, "\t%3.12lf\t%c", sum_tau, split);
    if(flag_time[1])
      fprintf(fp_write, " \t%lf", runhist->current->time[1]);
    if(flag_time[3])
      fprintf(fp_write, " \t%lf\t%c", sum_cpu, split);
    if(flag_extra[0])
      fprintf(fp_write, " \t%d\t%c", runhist->current->extraINT, split);
    if(flag_extra[1])
      fprintf(fp_write, " \t%lf\t%c", runhist->current->extraREAL, split);

    if(adp)
    {
      fprintf(fp_write, "    %c  %6d  %g %c ", runhist->current->RcstrState[0], (int)runhist->current->RcstrErr[0], runhist->current->RcstrErr[1], split);
      fprintf(fp_write, "%c  %6d  %g %c ",     runhist->current->RcstrState[1], (int)runhist->current->RcstrErr[2], runhist->current->RcstrErr[3], split);
      fprintf(fp_write, "%c  %6d  %g %c ",     runhist->current->RcstrState[2], (int)runhist->current->RcstrErr[4], runhist->current->RcstrErr[5], split);
      fprintf(fp_write, "%c  %6d  %g\t",      runhist->current->RcstrState[3], (int)runhist->current->RcstrErr[6], runhist->current->RcstrErr[7]);
      fprintf(fp_write, "    %c  %6d  %g %c ", runhist->current->RcstrState[4], (int)runhist->current->RcstrErr[8], runhist->current->RcstrErr[9], split);
      fprintf(fp_write, "%c  %6d  %g",      runhist->current->RcstrState[5], (int)runhist->current->RcstrErr[10], runhist->current->RcstrErr[11]);
    }
    runhist->current = runhist->current->next;
  }

  return sum_cpu;
}
