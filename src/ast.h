typedef struct ast {

  enum {id_type, number_type, operation_type, assignment_type, while_type, for_type, range_type, statements_type, if_type, last_element} type;

  union {

    int number;
    char* id;

    struct {

      struct ast* left;
      struct ast* right;

      char* op;

    } operation;

    struct {

      char*name;
      struct ast* right;

    } assignment;

     struct {

      int count;
      struct ast** statements;

      } statements;

      struct {
        struct ast* cond;
        struct ast* statements;

      } while_stmt;

      struct {
        char* iterator;
        struct ast* range;
        struct ast* statements;

      } for_stmt;

      struct {
      
        char* node;
        int left;
        int right;

      } range;

      struct {            // for "if/else" statements

        struct ast* cond;
        struct ast* if_branch;
        struct ast* else_branch;

      } if_stmt;

  } u;

} ast;

void* check_alloc(size_t sz);
ast* ast_new_operation(char*, ast*, ast*);
ast* ast_new_assignment(char*, ast*);
ast* ast_new_statements(int, ast*);
ast* ast_new_while(ast*, ast*);
ast* ast_new_for(char*, ast*, ast*);
ast* ast_new_range(int, int);
ast* ast_new_if(ast*, ast*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*);
void ast_print(ast*, int);