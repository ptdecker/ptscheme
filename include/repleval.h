/* repleval.h */

#ifndef FILE_REPLEVAL_SEEN
#define FILE_REPLEVAL_SEEN

/* REPL - Read */

extern bool is_tagged_list(object *expression, object *tag);
extern object *eval(object *exp, object *env);

#endif // FILE_REPLEVAL_SEEN
