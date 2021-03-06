package scanner

/**
 * @Author qingyingliu
 * @Date 2021/3/6 17:03
 */
class Token(val tokenType: TokenType, val literal: String, val lineNumber: Int) {
    override fun toString(): String {
        return "$lineNumber:   <$tokenType, \"$literal\">"
    }
}