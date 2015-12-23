/* lispbool.h */

#ifndef FILE_LISPBOOL_SEEN
#define FILE_LISPBOOL_SEEN

// Boolean native type

extern object *make_boolean(char value);
extern bool is_boolean(object *obj);
extern bool is_false(object *obj);
extern bool is_true(object *obj);

#endif // !FILE_LISPBOOL_SEEN
