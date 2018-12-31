#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#ifndef L_STR_IO
#include "file_io.h"
#endif



int make_directory(char * add_mkdir, char * label, char * scheme, char * version, int const m, int const n, int const SWITCH[], char * err_msg)
{
  char c_switch[4];

  if(SWITCH[0])
    c_switch[0] = 'W';
  else
    c_switch[0] = 'S';
  if(SWITCH[1])
    c_switch[1] = 'D';
  else
    c_switch[1] = 'C';
  if(SWITCH[2])
    c_switch[2] = 'Y';
  else
    c_switch[2] = 'N';
  c_switch[3] = '\0';




  char directory[L_STR_IO<<1] = "";
  char sub_dire[L_STR_IO<<2] = "";
  char size[L_STR_IO];
  int len, stat_mkdir;

  strcat(directory, label);
  strcat(directory, "_\0");
  strcat(directory, scheme);
  strcat(directory, "_\0");
  strcat(directory, version);
  strcat(directory, "_\0");
  sprintf(size, "%d", m);
  strcat(directory, size);
  if(n > 1)
  {
    strcat(directory, "x\0");
    sprintf(size, "%d", n);
    strcat(directory, size);
  }
  strcat(directory, "_\0");
  strcat(directory, c_switch);
  strcat(directory, "/\0");
  DIR * dir_test = NULL;
  strcat(add_mkdir, directory);
  strcat(add_mkdir, "\0");

  dir_test = opendir(add_mkdir);
  if(dir_test != NULL)
    printf("\nOutput directory [%s]\n.Already exists.\n\n", add_mkdir);
  else
  {
    stat_mkdir = mkdir(add_mkdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(stat_mkdir)
    {
      printf("\nOutput directory [%s]\n construction failed.\n", add_mkdir);
      add_mkdir = "~/HWENO_2D_tmp/";
      printf("Output direcotry changed to [%s].\n", add_mkdir);
      stat_mkdir = mkdir(add_mkdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      if(!stat_mkdir)
      {
	sprintf(err_msg, "Temporary output direcotry construction failed.\n");
	return 9;
      }
    }
    else
      printf("\nOutput directory [%s].\nNewly constructed.\n\n", add_mkdir);
  }
  closedir(dir_test);
  strcpy(sub_dire, add_mkdir);
  strcat(sub_dire, "rho\0");
  //printf("%s\n", sub_dire);
  mkdir(sub_dire, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  strcpy(sub_dire, add_mkdir);
  strcat(sub_dire, "mesh\0");
  //printf("%s\n", sub_dire);
  mkdir(sub_dire, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  strcpy(sub_dire, add_mkdir);
  strcat(sub_dire, "debug\0");
  mkdir(sub_dire, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  //printf("%s\n", sub_dire);

  return 0;
}



int open_fruncate(char * err_msg, char * add, FILE ** fp)
{
  if(*fp)
  {
    sprintf(err_msg, "The file pointer is not empty!\n");
    return 99;
  }
  if((*fp = fopen(add, "w+")) == 0)
  {
    sprintf(err_msg, "Cannot open solution output file: %s!\n", add);
    return 99;
  }
  fprintf(*fp, "a\n");
  fclose(*fp);
  if(remove(add))
  {
    sprintf(err_msg, "Fail to remove %s!\n", add);
    return 99;
  }
  if((*fp = fopen(add, "w+")) == 0)
  {
    sprintf(err_msg, "Cannot open solution output file: %s!\n", add);
    return 99;
  }
  /*
  if(ftruncate(*fp, 0))
  {
    sprintf(err_msg, "Fail to truncate %s!\n", add);
    return 99;
  }
  */
  rewind(*fp);

  return 0;
}



void intXwrite(int m, int n, int *data[], FILE * fp_write)
{
  int j = 0, i = 0;


    for(i = 0; i < n; ++i)
    {
      for(j = 0; j < m; ++j)
      {
	fprintf(fp_write, "%d\t", data[i][j]);
      }
      fprintf(fp_write, "\n");
    }
}


void intYwrite(int m, int n, int *data[], FILE * fp_write)
{
  int j = 0, i = 0;

    for(i = 0; i < n; ++i)
    {
      for(j = 0; j < m; ++j)
      {
	fprintf(fp_write, "%d\t", data[j][i]);
      }
      fprintf(fp_write, "\n");
    }
}



void matXwrite(int m, int n, double *data[], FILE * fp_write)
{
  int j = 0, i = 0;


    for(i = 0; i < n; ++i)
    {
      for(j = 0; j < m; ++j)
      {
	fprintf(fp_write, "%.18lf\t", data[i][j]);
      }
      fprintf(fp_write, "\n");
    }
}


void matYwrite(int m, int n, double *data[], FILE * fp_write)
{
  int j = 0, i = 0;

    for(i = 0; i < n; ++i)
    {
      for(j = 0; j < m; ++j)
      {
	fprintf(fp_write, "%.18lf\t", data[j][i]);
      }
      fprintf(fp_write, "\n");
    }
}






void write_vec_real(int m, double data[], FILE * fp_write)
{
  int j = 0;

  for(j = 0; j < m; ++j)
    fprintf(fp_write, "%.10lf\t", data[j]);
  fprintf(fp_write, "\n");
}
void write_vec_int(int m, int data[], FILE * fp_write)
{
  int j = 0;

  for(j = 0; j < m; ++j)
    fprintf(fp_write, "%d\t", data[j]);
  fprintf(fp_write, "\n");
}
