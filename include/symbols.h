/* symbols.h */

#ifndef FILE_SYMBOLS_SEEN
#define FILE_SYMBOLS_SEEN

extern object *make_symbol(char *value);
extern bool is_symbol(object *obj);

/* Built-in symbols */

extern object *quote_symbol();
extern object *define_symbol();
extern object *set_symbol();
extern object *ok_symbol();
extern object *if_symbol();
extern object *lambda_symbol();
extern object *begin_symbol();
extern object *cond_symbol();
extern object *else_symbol();
extern object *let_symbol();
extern object *and_symbol();
extern object *or_symbol();

#endif // FILE_SYMBOLS_SEEN
