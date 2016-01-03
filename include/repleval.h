/* repleval.h */

#ifndef FILE_REPLEVAL_SEEN
#define FILE_REPLEVAL_SEEN

/* REPL - Read */

extern bool is_tagged_list(object *expression, object *tag);
extern object *eval(object *exp, object *env);

bool is_last_exp(object *seq);
object *first_exp(object *seq);
object *make_begin(object *exp);

#endif // FILE_REPLEVAL_SEEN
