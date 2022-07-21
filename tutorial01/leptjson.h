#ifndef LEPTJSON_H__
#define LEPTJSON_H__

<<<<<<< HEAD
/* value_type */
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

/* json_value struct*/
=======
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;

>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
typedef struct {
    lept_type type;
}lept_value;

<<<<<<< HEAD
/* 解析函数的返回状态类型*/
=======
>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR
};

<<<<<<< HEAD
/**
 * @brief 解析json字符串
 * 
 * @param v json对象的值，用于保存解析出来的json对象的值
 * @param json json字符串
 * @return int 返回至表示解析状态
 */
int lept_parse(lept_value* v, const char* json);

/**
 * @brief 获取json对象的值类型
 * 
 * @param v json对象的值
 * @return lept_type 
 */
=======
int lept_parse(lept_value* v, const char* json);

>>>>>>> e11b43d0ebea1ba876ffd72fcb3907a1da1317ec
lept_type lept_get_type(const lept_value* v);

#endif /* LEPTJSON_H__ */
