package scanner

/**
 * @Author qingyingliu
 * @Date 2021/3/6 16:51
 */
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    CONSTANT_INT,
    CONSTANT_STRING,
    LOGICAL_OPERATOR,
    ARITHMETIC_OPERATOR,
    SPECIAL_TOKEN,
    UNEXPECTED_TOKEN,
    COMMENT,
    EOF
}