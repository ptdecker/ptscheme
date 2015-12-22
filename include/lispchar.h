/* lispchar.h */

#ifndef FILE_LISPCHAR_SEEN
#define FILE_LISPCHAR_SEEN

object *make_character(char value);
bool is_character(object *obj);

#endif // !FILE_LISPCHAR_SEEN