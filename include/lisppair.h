/* lisppair.h */

#ifndef FILE_LISPPAIR_SEEN
#define FILE_LISPPAIR_SEEN

/* Empty List   */

extern object *empty_list();
extern bool is_empty(object *obj);

/* Pair */

extern object *cons(object *car, object *cdr);
extern char is_pair(object *obj);

extern object *car(object *pair);
extern void set_car(object *obj, object* value);

extern object *cdr(object *pair);
extern void set_cdr(object *obj, object* value);

/* Syntactic Sugar for the car and cdr nested variants */
// Uncomment ones as needed

#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
//#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
//#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
//#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
//#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
//#define caaaar(obj) car(car(car(car(obj))))
//#define caaadr(obj) car(car(car(cdr(obj))))
//#define caadar(obj) car(car(cdr(car(obj))))
//#define caaddr(obj) car(car(cdr(cdr(obj))))
//#define cadaar(obj) car(cdr(car(car(obj))))
//#define cadadr(obj) car(cdr(car(cdr(obj))))
//#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
//#define cdaaar(obj) cdr(car(car(car(obj))))
//#define cdaadr(obj) cdr(car(car(cdr(obj))))
//#define cdadar(obj) cdr(car(cdr(car(obj))))
//#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
//#define cddaar(obj) cdr(cdr(car(car(obj))))
//#define cddadr(obj) cdr(cdr(car(cdr(obj))))
//#define cdddar(obj) cdr(cdr(cdr(car(obj))))
//#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))

#endif // !FILE_LISPPAIR_SEEN
