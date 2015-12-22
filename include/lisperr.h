/* lisperr.h */

#ifndef FILE_LISPERR_SEEN
#define FILE_LISPERR_SEEN

object *make_error(long error_num, char *error_msg);
bool is_error(object *obj);

#endif // !FILE_LISPERR_SEEN