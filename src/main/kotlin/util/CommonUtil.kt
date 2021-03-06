package util

/**
 * @Author qingyingliu
 * @Date 2021/3/6 19:59
 */

fun isDigit(c: Char) = c in '0'..'9'
fun isLetter(c: Char) = (c in 'a'..'z') || (c in 'A'..'Z')
fun isWhiteSpace(c: Char) = Character.isWhitespace(c)
