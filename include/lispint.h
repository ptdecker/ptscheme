/* lispint.h */

#ifndef FILE_LISPINT_SEEN
#define FILE_LISPINT_SEEN

extern object *make_fixnum(long value);
extern bool is_fixnum(object *obj);

#endif // !FILE_LISPINT_SEEN