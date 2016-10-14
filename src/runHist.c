#include <stdio.h>
#include <stdlib.h>

#ifndef runHistFlag
#include "runHist.h"
#endif


void init_runHist(runHist * runhist)
{
  runhist->length = 0;
  runhist->head = NULL;
  runhist->current = NULL;
  runhist->tail = NULL;
}

int insert_runHist(runHist * runhist)
{
  int j;

  if(runhist->tail)
  {
    runhist->tail->next = (runNode *)malloc(sizeof(runNode));
    if(!runhist->tail->next)
      return runhist->length;
    runhist->tail = runhist->tail->next;
  }
  else
  {
    runhist->tail = (runNode *)malloc(sizeof(runNode));
    if(!runhist->tail)
      return runhist->length;
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
  int j, count = 0;
  runNode * point;

  point = runhist->head;
  for(j = 0; j < p; ++j)
    if(point->next)
    {
      point = point->next;
      ++count;
    }
    else
      return count;//the link has only %d(count) compunonts while trying to reach [p]
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
