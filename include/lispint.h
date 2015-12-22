/* lispint.h */

#ifndef FILE_LISPINT_SEEN
#define FILE_LISPINT_SEEN

object *make_fixnum(long value);
bool is_fixnum(object *obj);

#endif // !FILE_LISPINT_SEEN