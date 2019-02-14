#define TextFlag 1

typedef struct Para{
  char * words;
  int len_words;
  struct Para * next;
} Para;

typedef struct{
  int n_para;

  Para * head;
  Para * tail;
  Para * current;
} Text;


void init_Text(Text * text);

int insert_Text(Text * text, char * msg);

/*
 * p = 0, 1, 2,..., lenght-1
 */
int locate_Text(int p, Text * text);

/* Drop a certain paragraph.
 * The index 'p' follows the same manner of the index for 'locate_Text'. */
int drop_Text(int p, Text * text);

void delete_Text(Text * text);

int copy_Text(Text * dest, Text * source, char * err_msg);

int check_Text(Text * text);
