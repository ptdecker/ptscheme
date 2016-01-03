/* primitives.h */

#ifndef FILE_PRIMITIVES_SEEN
#define FILE_PRIMITIVES_SEEN

extern void populate_environment(object *env);
extern object *add_proc(object *arguments);
extern object *apply_proc(object *arguments);
extern object *eval_proc(object *arguments);
extern bool is_primitive_proc(object *obj);

#endif // FILE_PRIMITIVES_SEEN
