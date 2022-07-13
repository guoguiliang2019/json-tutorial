#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include <stdlib.h>  /* NULL, strtod() */

/**
 * @brief 辅助宏定义
 * 
 */
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

/*被解析的json数据*/
typedef struct {
    const char* json;
}lept_context;

/**
 * @brief 跳过解析数据头部的空白字符
 * 
 * @param c 被解析的数据
 */
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/**
 * @brief 解析字面量类型的json数据
 * 
 * @param c 
 * @param v 
 * @param literal 
 * @param type 
 * @return int 
 */
static int lept_parse_literal(lept_context* c, lept_value* v, const char* literal, lept_type type) {
    size_t i;
    EXPECT(c, literal[0]);  /*判断首字符*/
    /*判断c和literal中剩下的数据是否相等*/
    for (i = 0; literal[i + 1]; i++) 
        if (c->json[i] != literal[i + 1])
            return LEPT_PARSE_INVALID_VALUE;
    /*修改相关数据的状态*/
    c->json += i;
    v->type = type;
    return LEPT_PARSE_OK;
}

/**
 * @brief 解析数值类型的json数据
 * 这里的数值检查使用了模块化的思维，按照数值的语法，将数值检查分成负号、整数、小数、指数四部分
 * @param c 
 * @param v 
 * @return int 
 */
static int lept_parse_number(lept_context* c, lept_value* v) {
    /*检查数值是否合法*/
    const char* p = c->json;
    /*开始时，若指针指向负号，向后移动指针*/
    if (*p == '-') p++;
    /*整数部分，若整数首字符是0，则转至小数部分；否则逐一验证整数部分的数字*/
    if (*p == '0') p++;
    else {
        if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    /*小数部分*/
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    /*指数部分*/
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    /*解析数值*/
    errno = 0;
    v->n = strtod(c->json, NULL);
    /*检查数值是否过大*/
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
        return LEPT_PARSE_NUMBER_TOO_BIG;
    /*更新相关的状态*/
    v->type = LEPT_NUMBER;
    c->json = p;
    return LEPT_PARSE_OK;
}

/**
 * @brief 解析器的核心部分
 * 
 * @param c 
 * @param v 
 * @return int 
 */
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

/**
 * @brief json解析API
 * 
 * @param v 
 * @param json 
 * @return int 
 */
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

/**
 * @brief 获取json值的类型
 * 
 * @param v 
 * @return lept_type 
 */
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

/**
 * @brief 获取json数值
 * 
 * @param v 
 * @return double 
 */
double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
