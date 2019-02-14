#include "file_i.h"



/* This function examines whether a string
 * represents a real number.
 * Transform the string represents a
 * negtive number into a string represents
 * the opposite one and return its' sign.
 * It returns 0 if the string do not
 * represents a real number.
 *
 * In the result given by this function, there will
 * be only one 'e' in the string, and the
 * only position for '-' lies behind 'e', and
 * there can be only one dot in the string
 * and the only position for it lies before 'e'.
 */
int is_real(char *str, char *digit, int n_digit)
{
  int i = 0, length = 0, j = 0;
  int sign = 1;
  int flag_dot = 0; //the number of dots in the string
                    //should be at most one
  int pos_dot = 0;
  int flag_e = 0;
  int pos_e = 0;


  length = strlen(str);

  for(j = 0; j < length; ++j)
  {
    //if( !((str[j] == 46)||(str[j] == 45)||(str[j] == 43)||(str[j] == 69)||(str[j] == 101)||((str[j] > 47) && (str[j] < 58))) )
    if(!in_char_set(str[j], digit, n_digit))
      return 0;

    if((str[j] == 69) || (str[j] == 101))
    {
      str[j] = 101;
      flag_e += 1;
      pos_e = j;
    }
  }

  // there could not be more than one 'e' in one number
  if(flag_e > 1)
    return 0;
  // the position of the 'e' counld be the beginning
  if((flag_e) && (pos_e == 0))
    return 0;
  // the position of the 'e' counld be the ending
  if((flag_e) && (pos_e == length-1))
    return 0;
  // a dot only could not be a number
  if((str[0] == 46) && (length == 1))
    return 0;
  // a '-' only could not be a number
  if(str[0] == 45)
  {
    if(length == 1)
      return 0;
    sign = -1;
  }
  // a '+' only could not be a number
  if(str[0] == 43)
    if(length == 1)
      return 0;

  // eliminate '-/+' from the string
  if((sign < 0) || (str[0] == 43))
  {
    for(i = 0; i < length; ++i)
      str[i] = str[i+1];
    length -= 1;
    pos_e -= 1;
    if(pos_e == 0)
      return 0;
  }

  if(flag_e)
  {
    for(i = 0; i < length; ++i)
    {
      // after eliminating '-/+', the only possible position for '-/+'
      // is behind 'e'
      if((str[i] == 45) || (str[i] == 43))
      {
        if((i-pos_e) != 1)
	  return 0;
	// even if '-/+' lies behind 'e', it could not be at the ending
	else if(i == length-1)
	  return 0;
      }
      // there could not be two dots in one number (1.23e2.3 is illegal, 1.23e2 is OK)
      else if((str[i] == 46) && (flag_dot > 0))
        return 0;
      else if(str[i] == 46)
      {
        flag_dot += 1;
        pos_dot = i;
      }
    }
    // a dot lies behind 'e' or immediately ahead of 'e' is illegal
    // either 1.e-3 or 1e2.3 is illegal
    if((flag_dot) && (pos_dot > (pos_e-2)))
      return 0;
  }
  else
  {
    for(i = 0; i < length; ++i)
    {
      // after eliminating '-/+', there should be no more '-/+'
      if((str[i] == 45) || (str[i] == 43))
        return 0;
      // if there are more than one dot
      else if((str[i] == 46) && (flag_dot > 0))
        return 0;
      else if(str[i] == 46)
        flag_dot += 1;
    }
  }

  return sign;
}


/* This function examines whether a string
 * represents an integer.
 * Transform the string represents a
 * negtive integer into a string represents
 * the opposite one and return its' sign.
 * It returns 0 if the string do not
 * represents an interger.
 */
int is_int(char * str, char *digit, int n_digit)
{
  int i = 0, length = 0, j = 0;
  int sign = 1;

  length = strlen(str);

  for(j = 0; j < length; ++j)
    //if( !((str[j] == 45)||(str[j] == 43)||((str[j] > 47) && (str[j] < 58))) )
    if(!in_char_set(str[j], digit, n_digit))
      return 0;


  // a '-' only could not be a number
  if(str[0] == 45)
  {
    if(length == 1)
      return 0;
    sign = -1;
  }
  // a '+' only could not be a number
  if(str[0] == 43)
    if(length == 1)
      return 0;

  // eliminate '-/+' from the string
  if((sign < 0) || (str[0] == 43))
  {
    for(i = 0; i < length; ++i)
      str[i] = str[i+1];
    length -= 1;
  }


  for(i = 0; i < length; ++i)
    // after eliminating '-/+', there should be no more '-/+'
    if((str[i] == 45) || (str[i] == 43))
      return 0;

  //eliminate zeros in the head
  while(str[0] == 48)
  {
    for(i = 0; i < length; ++i)
      str[i] = str[i+1];
    length -= 1;
  }

  return sign;
}



/* This function transforms a string
 * consisting '1', '2', ..., '0'
 * into the integer number it represents.
 */
int str2int(char * number)
{
  int result = 0;
  int i = 0, j = 0;
  int length = 0;

  length = strlen(number);


  for (i = 0; i < length; ++i)
    result += (int)(number[i] - 48)*pow(10, length-1 - i);


  return result;
}


/* This function transforms a string
 * consisting '1', '2', ..., '0',
 * 'e', '-', '+', and '.'
 * into the real number it represent
 */
double str2real(char * number)
{
  double result = 0.0, super_script = 0.0;
  int idx = 0, dot = -2;
  int i = 0, j = 0, power, k = 0;
  int length = 0;
  int pos_e = 0;
  char * after_e = number;
  int sign = 1;

  length = strlen(number);

  for(j = 0; j < length; ++j)
    if(number[j] == 101)
      pos_e = j;


  if(pos_e)
  {
    after_e = number + pos_e + 1;
    number[pos_e] = 0;
    result = str2real(number);
    if(after_e[0] == 45)
    {
      sign = -1;
      after_e += 1;
    }
    else if(after_e[0] == 43)
      after_e += 1;
    super_script = str2real(after_e);
    result = result * pow(10.0, sign * super_script);
  }
  else
  {
    while(number[idx] != 0)
    {
      if(number[idx] == 46)
      {
        dot = idx - 1;
	idx = 0;
	break;
      }
      ++idx;
    }

    if(dot == -2)
      dot = idx - 1;

    for (i = 0; i <= dot; ++i)
      result += (double)(number[i] - 48)*pow(10, dot - i);

    dot += 1;
    double addon = 0.0;
    for (i = 1; i < length - dot; ++i)
      result += (double)(number[dot+i] - 48)*pow(0.1, i);
  }
  
  return result;
}
