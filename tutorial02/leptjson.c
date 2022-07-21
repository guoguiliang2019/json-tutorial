#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, strtod() */
<<<<<<< HEAD
#include <math.h>


#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
#define ISSPACE(ch)         ((ch) == ' ' || (ch) == '\t' || (ch) == '\n' || (ch) == '\r')
=======

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
<<<<<<< HEAD
    while (ISSPACE(*p))
=======
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
        p++;
    c->json = p;
}

<<<<<<< HEAD
/* 
=======
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}
<<<<<<< HEAD
*/

/**
 * @brief 
 * 
 * @param c JSON字符串的内容 
 * @param v json值
 * @param str 对比的字面值
 * @param json_type 字面值类型
 * @return int 
 */
static int lept_parse_literal(lept_context* c, lept_value* v, const char *str, lept_type json_type) {
    while(*str != '\0' && *str == *c->json) {
        str++;
        c->json++;
    }
    if(*str != '\0') {
        return LEPT_PARSE_INVALID_VALUE;
    }
    else {
        v->type = json_type;
        return LEPT_PARSE_OK;
    }
}

/* 验证是否是有效数字，0表示非法数字，1表示数字有非法结尾，2表示有效 */
static int isnumber(const lept_context* c)
{
    const char *q = c->json;
    if(*q == '-')
        ++q;
    /*非法数字*/
    if(!ISDIGIT(*q))
        return 0;
    /*非法结尾*/
    else if(*q == '0' && (*(q+1) != '.' && *(q+1) != 'e' && *(q+1) != 'E' && *(q+1) != '\0'))
        return 1;

    /*验证内容是有效的*/
    while(!ISSPACE(*q) && *q != '\0') {
        if(!ISDIGIT(*q))
        {
            /*有bug，可能重复解析到小数部分和指数部分*/
            if(*q == '.' && !ISDIGIT(*(q+1)))
            {
                return 0;
            }
            else if(*q == 'e' || *q == 'E')
            {
                if(*(q+1) == '+' || *(q+1) == '-')
                {
                    if(!ISDIGIT(*(q+2)))
                        return 0;
                }
                else if(!ISDIGIT(*(q+1)))
                {
                    return 0;
                }
            }
        }
        ++q;
    }
    return 2;
}
=======
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec

static int lept_parse_number(lept_context* c, lept_value* v) {
    char* end;
    /* \TODO validate number */
<<<<<<< HEAD
    if(isnumber(c) == 0)
        return LEPT_PARSE_INVALID_VALUE;
    if(isnumber(c) == 1)
        return LEPT_PARSE_ROOT_NOT_SINGULAR;

    errno = 0;
    v->n = strtod(c->json, &end);
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
        return LEPT_PARSE_NUMBER_TOO_BIG;
    if (c->json == end)
        return LEPT_PARSE_INVALID_VALUE;
    
=======
    v->n = strtod(c->json, &end);
    if (c->json == end)
        return LEPT_PARSE_INVALID_VALUE;
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
    c->json = end;
    v->type = LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
<<<<<<< HEAD
        case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
=======
        case 't':  return lept_parse_true(c, v);
        case 'f':  return lept_parse_false(c, v);
        case 'n':  return lept_parse_null(c, v);
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

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

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
