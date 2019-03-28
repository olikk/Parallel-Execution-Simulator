
typedef struct symbol{
    enum { counter, clock } type;
    char* id;
    struct symbol * next;
} symbol;

symbol** init_s();
symbol * symbol_add (symbol **, char *, int);
symbol * symbol_lookup (symbol *, char *);
int semantic_check(ast*, symbol** tab);