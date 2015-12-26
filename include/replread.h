/* replread.h */

#ifndef FILE_REPLREAD_SEEN
#define FILE_REPLREAD_SEEN

/* REPL - Read */


extern bool is_delimiter(int c);
extern char is_initial(int c);
extern int peek(FILE *in);
extern void eat_whitespace(FILE *in);
extern void flush_input(FILE *in);
extern char make_esc_seq(const char c);
extern object *read_character(FILE *in);
extern object *read_pair(FILE *in);
extern object *read(FILE *in);

#endif // FILE_REPLREAD_SEEN
