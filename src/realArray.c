#include <stdio.h>
#include <stdlib.h>

#ifndef realArrayFlag
#include "realArray.h"
#endif


void init_realArray(realArray * array)
{
  array->n_box = 0;
  array->box_size = BoxSize;

  /* the capacity of the tail node
   * it's at least 1
   * since once a new box is required
   * there is must a entry to put in
   */
  array->tail_capacity = array->box_size;

  array->head = NULL;
  array->current = NULL;
  array->tail = NULL;
}

int insert_realArray(realArray * array)
{
  if(array->tail)
  {
    array->tail->next = (realBox *)malloc(sizeof(realBox));
    if(!array->tail->next)
      return array->n_box;

    array->tail->next->cargos = NULL;
    array->tail->next->cargos = (double *)malloc(array->box_size*sizeof(double));
    if(!array->tail->next->cargos)
    {
      free(array->tail->next);
      array->tail->next = NULL;
      return array->n_box;
    }

    array->tail = array->tail->next;
  }
  else
  {
    array->tail = (realBox *)malloc(sizeof(realBox));
    if(!array->tail)
      return array->n_box;

    array->tail->cargos = NULL;
    array->tail->cargos = (double *)malloc(array->box_size*sizeof(double));
    if(!array->tail->cargos)
    {
      free(array->tail);
      array->tail = NULL;
      return array->n_box;
    }

    array->head = array->tail;
  }

  ++(array->n_box);
  array->tail_capacity = 0;
  array->tail->next = NULL;

  return 0;
}

/*
 * p = 0, 1, 2,..., lenght-1
 */
int locate_realArray(int p, realArray * array)
{
  int j, count = 0;
  realBox * point;

  point = array->head;
  for(j = 0; j < p; ++j)
    if(point->next)
    {
      point = point->next;
      ++count;
    }
    else
      return count;//the link has only %d(count) compunonts while trying to reach [p]

  array->current = point;

  return 0;
}

/*
 * Find a certain entry in the linked array
 * idx starts from 0
 *
 */
int find_cargo_realArray(double * dest, int idx, realArray * array)
{
  int location;

  //the index of the box where the desired entry is
  location = (int)(idx/array->box_size);

  if(location >= array->n_box)
    return 1;
  idx = idx-location*array->box_size;
  if(location > array->n_box-2)//location == array->n_box-1
    if(idx >= array->tail_capacity)
      return 2;

  locate_realArray(location, array);
  *dest = array->current->cargos[idx];

  return 0;
}


void display_realArray(realArray * array)
{
  int i, box = 0;

  array ->current = array->head;
  
  while(array->current)
  {
    if(array->current->next)
    {
      printf("Box %d\n", box++);
      for(i = 0; i < array->box_size; ++i)
	printf("  %g\n", array->current->cargos[i]);
    }
    else
    {
      printf("Box %d\n", box++);
      for(i = 0; i < array->tail_capacity; ++i)
	printf("  %g\n", array->current->cargos[i]);
    }
    array->current = array->current->next;
  }

}



void delete_realArray(realArray * array)
{
  if(!array->n_box)
    return;

  while(array->n_box > 1)
  {
    /*
     * array[lenght-2] is the node immediately ahead of the tail
     * at the last step(length == 2), current == head
     */
    locate_realArray(array->n_box-2, array);

    if(array->tail->cargos)
      free(array->tail->cargos);
    free(array->tail);

    array->tail = array->current;
    --(array->n_box);

    array->tail->next = NULL;
  }

  array->tail = NULL;
  if(array->head->cargos)
    free(array->head->cargos);

  array->head = NULL;
  locate_realArray(0, array);

  array->n_box = 0;
  array->tail_capacity = array->box_size;
}


int check_realArray(realArray * array)
{
  int count = 0;
  
  array ->current = array->head;
  
  while(array->current)
  {    
    ++count;
    array->current = array->current->next;
  }

  
  return array->n_box - count;
}
