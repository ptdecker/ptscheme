/* lisperr.h */

#ifndef FILE_LISPERR_SEEN
#define FILE_LISPERR_SEEN

extern object *make_error(long error_num, char *error_msg);
extern bool is_error(object *obj);

#endif // !FILE_LISPERR_SEEN