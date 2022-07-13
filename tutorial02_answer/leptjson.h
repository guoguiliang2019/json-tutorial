#ifndef LEPTJSON_H__
#define LEPTJSON_H__

/**
 * @brief json值的类型枚举
 * 
 */
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/**
 * @brief json值的结构定义
 * 
 */
typedef struct {
	double n;
    lept_type type;
}lept_value;

/**
 * @brief json解析函数的返回值枚举
 * 
 */
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_NUMBER_TOO_BIG
};

/*json解析函数*/
int lept_parse(lept_value* v, const char* json);

/*获取解析得到的值类型*/
lept_type lept_get_type(const lept_value* v);

/*获取解析得到的数值*/
double lept_get_number(const lept_value* v);

#endif /* LEPTJSON_H__ */
