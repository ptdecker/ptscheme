/* lispchar.h */

#ifndef FILE_LISPCHAR_SEEN
#define FILE_LISPCHAR_SEEN

extern object *make_character(char value);
extern bool is_character(object *obj);

#endif // !FILE_LISPCHAR_SEEN