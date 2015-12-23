/* lisppair.h */

#ifndef FILE_LISPPAIR_SEEN
#define FILE_LISPPAIR_SEEN

/* Empty List   */

extern object *make_empty();
extern bool is_empty(object *obj);

/* Pair */

extern object *cons(object *car, object *cdr);
extern char is_pair(object *obj);

extern object *car(object *pair);
extern void set_car(object *obj, object* value);

extern object *cdr(object *pair);
extern void set_cdr(object *obj, object* value);

#endif // !FILE_LISPPAIR_SEEN