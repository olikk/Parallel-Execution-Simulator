#ifndef EXEC_H
#define EXEC_H

struct ast;
struct exec_env;

/* creates the execution engine */
struct exec_env* create_vm();

/* removes the ExecEnviron */
void free_env(struct exec_env* e);

/* executes an AST */
void exec_ast(struct exec_env* e, ast* a);

#endif