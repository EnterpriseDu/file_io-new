#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TextFlag
#include "Text.h"
#endif

#ifndef realArrayFlag
#include "realArray.h"
#endif

#define L_BUFF 400


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
int is_real(char *str, char *digit, int n_digit);


/* This function examines whether a string
 * represents an integer.
 * Transform the string represents a
 * negtive integer into a string represents
 * the opposite one and return its' sign.
 * It returns 0 if the string do not
 * represents an interger.
 */
int is_int(char * str, char *digit, int n_digit);


/* This function transforms a string
 * consisting '1', '2', ..., '0'
 * into the integer number it represents.
 */
int str2int(char * number);


/* This function transforms a string
 * consisting '1', '2', ..., '0',
 * 'e', '-', '+', and '.'
 * into the real number it represent
 */
double str2real(char * number);


int in_char_set(char ch, char *set, int n_set);

int buff_read(FILE * fp, char *buff, char *space, int n_space, char comment);


int real_read
(realArray * array, char * end, FILE * fp, char * buffer, char * number, char comment,
 char barrier, char * space, int n_space, char * digit_real, int n_digit_real, char * err_msg);


int string_read
(Text * text, char * end, FILE * fp, char * buffer, char comment,
 char barrier, char * space, int n_space, char * err_msg);
