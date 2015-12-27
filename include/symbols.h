/* symbols.h */

#ifndef FILE_SYMBOLS_SEEN
#define FILE_SYMBOLS_SEEN

extern object *make_symbol(char *value);
extern char is_symbol(object *obj);
extern object *quote_symbol();

#endif // FILE_SYMBOLS_SEEN
