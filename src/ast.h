typedef struct ast {

  enum {id_type, number_type, operation_type, assignment_type, while_type, for_type, range_type, statements_type, if_type, finish_type, assync_type} type;
  union {
    int number;
    char* id;

    struct {             // for "expression" statements
      struct ast* left;
      struct ast* right;
      char* op;
    } operation;

    struct {               // for "assignment" statements
      char*name;
      struct ast* right;
    } assignment;

     struct {             //for a block of statements
      int count;
      struct ast** statements;
      } statements;

      struct {
        struct ast* cond;
        struct ast* statements;
      } while_stmt;

      struct {              // for "for" statements
        char* iterator;
        struct ast* range;
        struct ast* statements;
      } for_stmt;

      struct {             // for a range in "for" statements
        char* node;
        struct ast* left;
        struct ast* right;
      } range;

      struct {            // for "if/else" statements
        struct ast* cond;
        struct ast* if_branch;
        struct ast* else_branch;
      } if_stmt;

      struct {            // for "if/else" statements
        char** clocks;
        struct ast* stmt;
      } finish_stmt;

      struct {            // for "if/else" statements
        char** clocks;
        struct ast* stmt;
      } assync_stmt;
  } u;
} ast;

typedef struct {
  char * name;
  struct clock * next;
} clock;

typedef struct {
  char* clocks;
  size_t iter;
  size_t cap;
} list_of_clock;

void* check_alloc(size_t sz);
ast* ast_new_operation(char*, ast*, ast*);
ast* ast_new_assignment(char*, ast*);
ast* ast_new_statements(ast*, ast*);
ast* ast_new_while(ast*, ast*);
ast* ast_new_for(char*, ast*, ast*);
ast* ast_new_range(ast*, ast*);
ast* ast_new_if(ast*, ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*);
ast* ast_new_parallel(char*, char**, ast*);
clock* new_clocks(clock*, clock*);
void ast_print(ast*, int);