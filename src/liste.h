#include <stdlib.h>
typedef struct clock {
  char * id;
  struct clock * prec;
} clock;

void* check_alloc(size_t sz);
clock* push_clock(clock*, char*);
void print_clock(clock *c);