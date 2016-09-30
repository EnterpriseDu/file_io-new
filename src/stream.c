#include "file_i.h"


#ifndef realArrayFlag
#include "realArray.h"
#endif

#ifndef TextFlag
#include "Text.h"
#endif


int in_ch_set(char ch, char *set, int n_set)
{
  int i;
  for(i = 0; i < n_set; ++i)
    if(ch == set[i])
      return 1;
  return 0;
}



/* Example 1: input : '  123   '
 *            output: '123 '
 *
 * Example 2: input : '123EOF'
 *            output: '123EOF'
 *
 * Example 3: input : '   \n'
 *            output: '\n'
 *
 * Example 4: input : '   EOF'
 *            output: 'EOF'
 */
int buff_read(FILE * fp, char *buff, char *space, int n_space, char comment)
{
  int count = 0, flag = 0, flag_comment = 0;
  int j;
  long pos;
  char ch;

  flag = 0;
  flag_comment = 0;
  while((ch = getc(fp)) != EOF)
  {
    /* The following codes won't work until we come to a comment sign.
     * Before that, we just push charaters into the buffer as usual.
     * Once we meet a comment sign, the following codes would hijack
     * the whole procedure. At that time, the propose of this function
     * is to seek '\n' or 'EOF'. Then, we put it into the buffer, and
     * leave the function.
     */
    if(flag_comment)
      if((ch == '\n') || (ch == EOF))
      {
	buff[count++] = ch;
	break;
      }
      else
	continue;
    if(ch == comment)
    {
      flag_comment = 1;
      continue;
    }


    //if we have already begun reading "meaningful" charactors
    if(flag)
    {
      //now an 'space' one, finish reading
      if(in_ch_set(ch, space, n_space))
      {
	buff[count++] = ch;
	break;
      }
    }
    //if we haven't caught a "meaningful" charactor
    else
    {
      //still a 'space' one
      if(in_ch_set(ch, space, n_space))
	//sencible for '\n'
	if(ch == '\n')
	{
	  buff[count++] = ch;
	  break;
	}
	else
	  continue;
      //now we firstly catch a "meaningful" charactor, set the flag to be non-zero
      else
	flag = 1;
    }

    /* If we come to these codes, we are in a situation that
     * the buffer is full of "meaningful" character except for
     * the last position.
     * But we still have at least a 'space' as well as '\0'
     * to put in the buffer.
     * So we have to give up this reading.
     */
    if(count == L_BUFF)
      return 0;


    buff[count++] = ch;
  }

  /* If the above reading finished with a normal character,
   * just skip the following codes.
   * If the above reading finished with 'EOF',
   * there are two possible cases:
   *   1. we finish reading a "meaningful" string then immediate 'EOF'
   *   2. we omit the 'space' characters all the way until come to 'EOF'
   *      which means now the buffer is still empty
   * Either way, we should put this EOF to the end of the buffer.
   */
  if(ch == EOF)
    buff[count++] = EOF;

  /* If we come to here, the buffer would be non-empty. Either 
   * a "meaningful" string followed by a 'space'/'EOF' of a single
   * '\n'/'EOF' would be in the buffer.
   * 'count' would point to the next position in the buffer.
   *
   * Add '\0' at the end of the buffer
   */
  buff[count] = '\0';
  return count;
}



int real_read
(realArray * array, char * end, FILE * fp, char * buffer, char * number, char comment,
 char barrier, char * space, int n_space, char * digit_real, int n_digit_real, char * err_msg)
{
  if(in_ch_set(comment, space, n_space))
  {
    sprintf(err_msg, "comment<space\n");
    return -1;
  }
  /*
  if(!in_ch_set(end, space, n_space));
  {
    sprintf(err_msg, "end!<space\n");
    return -2;
  }
  init_realArray(array);
  */


  char end_mark;
  int length, mem;

  int sign;
  int count = 1;

  count = 1;
  while(1)
  {
    length = buff_read(fp, buffer, space, n_space, comment);

    if(!length)
    {
      sprintf(err_msg, "Buffer overflow!\n");
      return count;
    }

    /* In the critical situation, the buffer is just full.
     * Then we have buffer[L_BUFF] == '\0'.
     * At that time, length == L_BUFF.
     */
    if(length > L_BUFF)
    {
      sprintf(err_msg, "'length' too large!\n");
      return count;
    }

    /* buffer[length-1] is the end mark, either a 'space' or 'EOF'
     */
    end_mark = buffer[length-1];
    if(!in_ch_set(end_mark, space, n_space) && (end_mark != EOF))
    {
      sprintf(err_msg, "Wrong ending mark! %x\n", end_mark);
      return count;
    }
    buffer[length-1] = '\0';



    //just a ending mark in the buffer
    if(length == 1)
      if((end_mark == barrier) || (end_mark == EOF))
	break;
      else
	continue;


    //convert the string into a real number
    strcpy(number, buffer);
    sign = is_real(number, digit_real, n_digit_real);
    if(!sign)
    {
      sprintf(err_msg, "nan\n");
      return count;
    }

    //add the newly read number into our array
    if(!(array->tail_capacity - array->box_size))//the last box is full
    {
      mem = insert_realArray(array);
      if(mem)
      {
	sprintf(err_msg, "Fail to add a new box, current box %d\n", mem);
	return count;
      }
      array->tail->cargos[(array->tail_capacity)++] = sign*str2real(number);
      ++count;
    }
    else
    {
      array->tail->cargos[(array->tail_capacity)++] = sign*str2real(number);
      ++count;
    }

    //check the ending mark
    if((end_mark == barrier) || (end_mark == EOF))
      break;
  }

  *end = end_mark;
  return 0;
}




int string_read
(Text * text, char * end, FILE * fp, char * buffer, char comment,
 char barrier, char * space, int n_space, char * err_msg)
{
  if(in_ch_set(comment, space, n_space))
  {
    sprintf(err_msg, "comment<space");
    return -1;
  }
  /*
  if(!in_ch_set(end, space, n_space));
  {
    sprintf(err_msg, "end!<space\n");
    return -2;
  }
  init_realArray(array);
  */


  char end_mark;
  int length, mem;

  int sign;
  int count = 1;

  count = 1;
  while(1)
  {
    length = buff_read(fp, buffer, space, n_space, comment);

    if(!length)
    {
      sprintf(err_msg, "Buffer overflow!");
      return count;
    }

    /* In the critical situation, the buffer is just full.
     * Then we have buffer[L_BUFF] == '\0'.
     * At that time, length == L_BUFF.
     */
    if(length > L_BUFF)
    {
      sprintf(err_msg, "'length' too large!");
      return count;
    }

    /* buffer[length-1] is the end mark, either a 'space' or 'EOF'
     */
    end_mark = buffer[length-1];
    if(!in_ch_set(end_mark, space, n_space) && (end_mark != EOF))
    {
      sprintf(err_msg, "Wrong ending mark! %x.", end_mark);
      return count;
    }
    buffer[length-1] = '\0';



    //just a ending mark in the buffer
    if(length == 1)
      if((end_mark == barrier) || (end_mark == EOF))
	break;
      else
	continue;



    //add the newly read string into our text
    if(strlen(buffer) > (L_BUFF-1))
    {
      sprintf(err_msg, "Buffer too long(%d:%d).", (int)strlen(buffer), L_BUFF);
      return count;
    }
    mem = insert_Text(text, buffer);
    if(mem)
    {
      sprintf(err_msg, "Fail to add a new paragraph, current para %d.", mem);
      return count;
    }

    //check the ending mark
    if((end_mark == barrier) || (end_mark == EOF))
      break;
  }

  *end = end_mark;
  return 0;
}

