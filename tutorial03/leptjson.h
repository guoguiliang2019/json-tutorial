#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#include <stddef.h> /* size_t */


typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/*json值的数据结构定义*/
typedef struct {
    union {
        struct { char* s; size_t len; }s;  /* string: null-terminated string, string length */
        double n;                          /* number */
    }u;
    lept_type type;
}lept_value;

/*解析函数的返回值*/
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_NUMBER_TOO_BIG,
    LEPT_PARSE_MISS_QUOTATION_MARK,
    LEPT_PARSE_INVALID_STRING_ESCAPE,
    LEPT_PARSE_INVALID_STRING_CHAR
};

/*初始化json值*/
#define lept_init(v) do { (v)->type = LEPT_NULL; } while(0)

int lept_parse(lept_value* v, const char* json);

/*释放json值空间*/
void lept_free(lept_value* v);

/*json值类型的读取函数*/
lept_type lept_get_type(const lept_value* v);

/*null值的设置函数*/
#define lept_set_null(v) lept_free(v)

/*bool值的读写函数*/
int lept_get_boolean(const lept_value* v);
void lept_set_boolean(lept_value* v, int b);

/*数值的读写函数*/
double lept_get_number(const lept_value* v);
void lept_set_number(lept_value* v, double n);

/*string值和string_length的读写函数*/
const char* lept_get_string(const lept_value* v);
size_t lept_get_string_length(const lept_value* v);
void lept_set_string(lept_value* v, const char* s, size_t len);

#endif /* LEPTJSON_H__ */
