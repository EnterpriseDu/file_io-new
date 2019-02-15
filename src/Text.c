#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TextFlag
#include "Text.h"
#endif


int insert_Text(Text * text, char * msg)
{
  int j;
  int len_msg = strlen(msg);

  if(text->tail)
  {
    text->tail->next = (Para *)malloc(sizeof(Para));
    if(!text->tail->next)
      return text->n_para+1;

    text->tail->next->words = NULL;
    text->tail->next->words = (char *)malloc((len_msg+1)*sizeof(char));
    if(!text->tail->next->words)
    {
      free(text->tail->next);
      text->tail->next = NULL;
      return text->n_para+1;
    }
    for(j = 0; j < len_msg; ++j)
      text->tail->next->words[j] = msg[j];
    text->tail->next->words[len_msg] = '\0';

    text->tail = text->tail->next;
  }
  else
  {
    text->tail = (Para *)malloc(sizeof(Para));
    if(!text->tail)
      return text->n_para+1;

    text->tail->words = NULL;
    text->tail->words = (char *)malloc((len_msg+1)*sizeof(char));
    if(!text->tail->words)
    {
      free(text->tail);
      text->tail = NULL;
      return text->n_para+1;
    }
    for(j = 0; j < len_msg; ++j)
      text->tail->words[j] = msg[j];
    text->tail->words[len_msg] = '\0';

    text->head = text->tail;
  }
  ++(text->n_para);

  text->tail->len_words = len_msg;//words[len_words] = '\0'
  text->tail->next = NULL;

  return 0;
}

/*
 * p = 0, 1, 2,..., lenght-1
 */
int locate_Text(int p, Text * text)
{
  int j, count = 1;
  Para * point;

  point = text->head;
  for(j = 0; j < p; ++j)
    if(point->next)
    {
      point = point->next;
      ++count;
    }
    else
      return count;//the link has only %d(count-1) compunonts while trying to reach [p]
      /*
      sprintf(err_msg, "Failed to locate at position %d (count form 0) in 'source'. Stopped at trying to reach %d which is NULL.", p, count;
      */

  text->current = point;
  return 0;
}

int drop_Text(int p, Text * text)
{
  int state;
  Para * pt, * previous;

  if(text->n_para == 0)
    return 0;
  if(p >= text->n_para)
    return 1;

  if(p == 0)
  {
    previous = NULL;
    pt = text->head;
    text->head = pt->next;
    free(pt);
  }
  else
  {
    state = locate_Text(p-1, text);
    if(state)
      return 2;
    previous = text->current;
    state = locate_Text(p, text);
    if(state)
      return 3;
    previous->next = text->current->next;
    free(text->current);
    text->current = NULL;
  }

  /* The case where p==0 and n_para==1 is included. */
  if(p == text->n_para-1)
    text->tail = previous;
  --(text->n_para);

  return 0;
}


void delete_Text(Text * text)
{
  if(!text->n_para)
    return;

  while(text->n_para > 1)
  {
    /*
     * text[n_para-2] is the node immediately ahead of the tail
     * at the last step(length == 2), current == head
     */
    locate_Text(text->n_para-2, text);

    if(text->tail->words)
      free(text->tail->words);
    free(text->tail);

    text->tail = text->current;
    --(text->n_para);

    text->tail->next = NULL;
  }

  text->tail = NULL;
  if(text->head)
  {
    if(text->head->words)
      free(text->head->words);
    free(text->head);
  }
  text->head = NULL;
  locate_Text(0, text);

  text->n_para = 0;
}


int copy_Text(Text * dest, Text * source, char * err_msg)
{
  int state, count = 0;

  delete_Text(dest);
  source->current = source->head;
  while(source->current != NULL)
  {
    state = insert_Text(dest, source->current->words);
    if(state)
    {
      sprintf(err_msg, "Failed to insert the %d-th (count form 0) item in 'dest'.", state-1);
      return state;
    }
    source->current = source->current->next;
    ++count;
  }

  if(count != dest->n_para)
  {
    sprintf(err_msg, "The number of items actually inserted into 'dest' is not equal to the length of 'source' : %d-%d.", count, source->n_para);
    return count;
  }
  return 0;
}


void init_Text(Text * text)
{
  //delete_Text(text);

  text->n_para = 0;
  text->head = NULL;
  text->current = NULL;
  text->tail = NULL;
}


int check_Text(Text * text)
{
  int count = 0;
  
  text ->current = text->head;
  
  while(text->current)
  {    
    ++count;
    text->current = text->current->next;
  }

  
  return text->n_para - count;
}
