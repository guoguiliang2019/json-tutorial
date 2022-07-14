#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include <stdlib.h>  /* NULL, malloc(), realloc(), free(), strtod() */
#include <string.h>  /* memcpy() */

#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')
#define PUTC(c, ch)         do { *(char*)lept_context_push(c, sizeof(char)) = (ch); } while(0)

typedef struct {
    const char* json;
    /*堆栈缓冲区和栈大小、栈顶位置*/
    char* stack;
    size_t size, top;
}lept_context;

/**
 * @brief  
 * 
 * @param c 
 * @param size 
 * @return void* 
 */
static void* lept_context_push(lept_context* c, size_t size) {
    void* ret;
    assert(size > 0);
    if (c->top + size >= c->size) {
        if (c->size == 0)
            c->size = LEPT_PARSE_STACK_INIT_SIZE;
        while (c->top + size >= c->size)
            c->size += c->size >> 1;  /* c->size * 1.5 */
        c->stack = (char*)realloc(c->stack, c->size);
    }
    ret = c->stack + c->top;
    c->top += size;
    return ret;
}

/**
 * @brief 出栈,设置栈顶位置，返回栈顶指针
 * 
 * @param c 
 * @param size 
 * @return void* 
 */
static void* lept_context_pop(lept_context* c, size_t size) {
    assert(c->top >= size);
    return c->stack + (c->top -= size);
}

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/**
 * @brief 解析文本量
 * 
 * @param c 
 * @param v 
 * @param literal 
 * @param type 
 * @return int 
 */
static int lept_parse_literal(lept_context* c, lept_value* v, const char* literal, lept_type type) {
    size_t i;
    EXPECT(c, literal[0]);
    for (i = 0; literal[i + 1]; i++)
        if (c->json[i] != literal[i + 1])
            return LEPT_PARSE_INVALID_VALUE;
    c->json += i;
    v->type = type;
    return LEPT_PARSE_OK;
}

/**
 * @brief 解析数值
 * 
 * @param c 
 * @param v 
 * @return int 
 */
static int lept_parse_number(lept_context* c, lept_value* v) {
    const char* p = c->json;
    if (*p == '-') p++;
    if (*p == '0') p++;
    else {
        if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    errno = 0;
    v->u.n = strtod(c->json, NULL);
    if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
        return LEPT_PARSE_NUMBER_TOO_BIG;
    v->type = LEPT_NUMBER;
    c->json = p;
    return LEPT_PARSE_OK;
}

/**
 * @brief 解析字符串
 * 
 * @param c 
 * @param v 
 * @return int 
 */
static int lept_parse_string(lept_context* c, lept_value* v) {
    size_t head = c->top, len;
    const char* p;
    EXPECT(c, '\"');
    p = c->json;
    for (;;) {
        char ch = *p++;
        switch (ch) {
            case '\"':
                len = c->top - head;
                lept_set_string(v, (const char*)lept_context_pop(c, len), len);
                c->json = p;
                return LEPT_PARSE_OK;
            case '\0':
                c->top = head;
                return LEPT_PARSE_MISS_QUOTATION_MARK;
            case '\\':
                /*其后方是一个转义序列*/
            {
                ch = *p++;
                switch(ch)
                {
                    case '\\':
                        PUTC(c, '\\');break;
                    case '\"':
                        PUTC(c, '\"');break;
                    case '/':
                        PUTC(c, '/');break;
                    case 'b':
                        PUTC(c, '\b');break;
                    case 'f':
                        PUTC(c, '\f');break;
                    case 'n':
                        PUTC(c, '\n');break;
                    case 'r':
                        PUTC(c, '\r');break;
                    case 't':
                        PUTC(c, '\t');break;
                    default:
                        c->top = head;
                        return LEPT_PARSE_INVALID_STRING_ESCAPE;
                }
                break;
            }
            default:
                if((unsigned char)ch < 0x20)
                {
                    c->top = head;
                    return LEPT_PARSE_INVALID_STRING_CHAR;
                }
                PUTC(c, ch);
        }
    }
}

/**
 * @brief 解析器的状态机
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
        case '"':  return lept_parse_string(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return lept_parse_number(c, v);
    }
}

/**
 * @brief 解析器API
 * 
 */
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    lept_init(v);
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    assert(c.top == 0);
    free(c.stack);
    return ret;
}

/**
 * @brief 释放json值空间
 * 
 * @param v 
 */
void lept_free(lept_value* v) {
    assert(v != NULL);
    if (v->type == LEPT_STRING)
        free(v->u.s.s);
    v->type = LEPT_NULL;
}

/**
 * @brief 获取json值类型
 * 
 * @param v 
 * @return lept_type 
 */
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

/**
 * @brief 获取bool类型的json值
 * 
 * @param v 
 * @return int 
 */
int lept_get_boolean(const lept_value* v) {
    /* \TODO */
    assert(v != NULL && (v->type == LEPT_FALSE || v->type == LEPT_TRUE));
    return v->type == LEPT_TRUE;
}

/**
 * @brief 设置bool类型的json值
 * 
 * @param v 
 * @param b 
 */
void lept_set_boolean(lept_value* v, int b) {
    /* \TODO */
    lept_free(v);
    if(b == 0)
        v->type = LEPT_FALSE;
    else
        v->type = LEPT_TRUE;
}

/**
 * @brief 获取number类型的json值
 * 
 * @param v 
 * @return double 
 */
double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->u.n;
}

/**
 * @brief 设置number类型的json值
 * 
 * @param v 
 * @param n 
 */
void lept_set_number(lept_value* v, double n) {
    /* \TODO */
    lept_free(v);
    v->u.n = n;
    v->type = LEPT_NUMBER;
}

/**
 * @brief 获得string类型的json值
 * 
 * @param v 
 * @return const char* 
 */
const char* lept_get_string(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_STRING);
    return v->u.s.s;
}

/**
 * @brief 获取string类型json值的长度
 * 
 * @param v 
 * @return size_t 
 */
size_t lept_get_string_length(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_STRING);
    return v->u.s.len;
}

/**
 * @brief 设置string类型的json值
 * 
 * @param v 
 * @param s 
 * @param len 
 */
void lept_set_string(lept_value* v, const char* s, size_t len) {
    assert(v != NULL && (s != NULL || len == 0));
    lept_free(v);
    v->u.s.s = (char*)malloc(len + 1);
    memcpy(v->u.s.s, s, len);
    v->u.s.s[len] = '\0';
    v->u.s.len = len;
    v->type = LEPT_STRING;
}
