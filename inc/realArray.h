#define realArrayFlag 1

#define BoxSize 1000

typedef struct realBox{
  double * cargos;
  struct realBox * next;
} realBox;

typedef struct{
  int n_box;
  int box_size;
  int tail_capacity;

  realBox * head, * tail, * current;
} realArray;


void init_realArray(realArray * array);


int insert_realArray(realArray * array);

/*
 * p = 0, 1, 2,..., lenght-1
 */
int locate_realArray(int p, realArray * array);

/*
 * Find a certain entry in the linked array
 * idx starts from 0
 *
 */
int find_cargo_realArray(double * dest, int idx, realArray * array);


void display_realArray(realArray * array);


void delete_realArray(realArray * array);


int check_realArray(realArray * array);
