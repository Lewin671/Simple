package scanner

import java.io.FileInputStream
import java.util.*

/**
 * 词法分析的简单测试
 * @Author qingyingliu
 * @Date 2021/3/6 20:42
 */

fun main() {

    val charProvider = object : LineProvider {
        val inputStream = FileInputStream("src/main/resources/test1.spp")
        val scanner = Scanner(inputStream)

        override fun nextLine(): String? {
            return if (scanner.hasNextLine()) {
                scanner.nextLine() + "\n"
            } else {
                inputStream.close()
                null
            }
        }

    }

    val scanner = Lexer(charProvider)

    while (true) {
        val nextToken = scanner.nextToken()
        println(nextToken)
        if (nextToken.tokenType == TokenType.EOF) {
            break
        }
    }
}