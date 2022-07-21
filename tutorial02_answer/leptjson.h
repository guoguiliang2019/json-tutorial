#ifndef LEPTJSON_H__
#define LEPTJSON_H__

<<<<<<< HEAD
/**
 * @brief json值的类型枚举
 * 
 */
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/**
 * @brief json值的结构定义
 * 
 */
=======
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
typedef struct {
	double n;
    lept_type type;
}lept_value;

<<<<<<< HEAD
/**
 * @brief json解析函数的返回值枚举
 * 
 */
=======
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_NUMBER_TOO_BIG
};

<<<<<<< HEAD
/*json解析函数*/
int lept_parse(lept_value* v, const char* json);

/*获取解析得到的值类型*/
lept_type lept_get_type(const lept_value* v);

/*获取解析得到的数值*/
=======
int lept_parse(lept_value* v, const char* json);

lept_type lept_get_type(const lept_value* v);

>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
double lept_get_number(const lept_value* v);

#endif /* LEPTJSON_H__ */
