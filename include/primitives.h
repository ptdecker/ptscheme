/* primitives.h */

#ifndef FILE_PRIMITIVES_SEEN
#define FILE_PRIMITIVES_SEEN

extern void register_primitives();
extern object *add_proc(object *arguments);
extern bool is_primitive_proc(object *obj);

#endif // FILE_PRIMITIVES_SEEN
