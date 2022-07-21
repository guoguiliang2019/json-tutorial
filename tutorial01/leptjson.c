#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

<<<<<<< HEAD
/*判断字符的宏*/
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

/*json字符串的包装结构，这里把它叫作json内容*/
=======
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
typedef struct {
    const char* json;
}lept_context;

<<<<<<< HEAD
/*解析json字符串前后的空白字符*/
=======
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

<<<<<<< HEAD
/*解析json内容中的null值*/
=======
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

<<<<<<< HEAD
/*解析json内容中的true值*/
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

/*解析json内容中的false值*/
static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

/*json解析器的核心，用于解析json对象的值*/
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_null(c, v);
        case 't':  return lept_parse_true(c, v);
        case 'f':  return lept_parse_false(c, v);
=======
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_null(c, v);
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

<<<<<<< HEAD
/*解析json字符串的尾部空白字符*/
static int lept_parse_root(lept_context* c, int parse_state) {
    /*解析空白字符*/
    lept_parse_whitespace(c);
    /*判断空白字符后是否有其他字符*/
    if(parse_state == LEPT_PARSE_OK && *c->json != '\0')
        return LEPT_PARSE_ROOT_NOT_SINGULAR;
    else
        return parse_state;
}

/*解析器API，把json字符串解析成json值*/
int lept_parse(lept_value* v, const char* json) {
    int parse_state = 0;
=======
int lept_parse(lept_value* v, const char* json) {
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
<<<<<<< HEAD
    /*解析值前空白*/
    lept_parse_whitespace(&c);
    /*解析值*/
    parse_state = lept_parse_value(&c, v);
    /*解析值后空白*/
    return lept_parse_root(&c, parse_state);
}

/*获取json值的类型*/
=======
    lept_parse_whitespace(&c);
    return lept_parse_value(&c, v);
}

>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
