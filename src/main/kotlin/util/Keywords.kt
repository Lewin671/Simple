package util

/**
 * @Author qingyingliu
 * @Date 2021/3/6 16:57
 */

val keywords = arrayOf(
    // 选择语句
    "if",
    "else",
    "then",
    "end",

    // 循环语句
    "do",
    "while",

    // 变量类型
    "int",
    "string",
    "bool"
)

fun isKeywords(str: String): Boolean {
    return keywords.contains(str)
}