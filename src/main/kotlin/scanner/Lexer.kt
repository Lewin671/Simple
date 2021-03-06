package scanner

import util.isDigit
import util.isKeywords
import util.isLetter
import util.isWhiteSpace

/**
 * @Author qingyingliu
 * @Date 2021/3/6 17:01
 */
class Lexer(private val lineProvider: LineProvider) {
    private var lineNumber = 1
    private var i = 0
    private var currentLine = lineProvider.nextLine()
    private var goBackNow = false
    private var lastChar: Char? = null


    // 从charProvider中读取下一个字符
    private fun nextCharFromLine(): Char? {
        if (currentLine != null) {
            return if (i >= currentLine!!.length) {
                currentLine = lineProvider.nextLine()
                lineNumber++
                i = 0
                nextCharFromLine()
            } else {
                currentLine!![i++]
            }
        }

        return null
    }


    private fun nextChar(): Char? {
        return if (goBackNow) {
            goBackNow = false
            lastChar
        } else {
            lastChar = nextCharFromLine()
            lastChar
        }
    }

    private fun goBack() {
        goBackNow = true
    }

    fun nextToken(): Token {
        var state = State.Start
        var c: Char?
        val literal = StringBuilder()
        var currentToken: Token? = null


        while (true) {
            c = nextChar()
            if (c == null) return Token(TokenType.EOF, "eof", lineNumber)

            when (state) {
                State.Start -> {
                    when {
                        isDigit(c) -> {
                            state = State.InNum
                        }
                        isLetter(c) || c == '_' -> {
                            state = State.InIdentifier
                        }
                        c == '=' -> state = State.InAssign
                        c == '<' -> state = State.InLess
                        c == '>' -> state = State.InMore
                        c == '"' -> state = State.InStr
                        c == ',' || c == ';' -> {
                            currentToken = Token(TokenType.SPECIAL_TOKEN, c.toString(), lineNumber)
                            break
                        }
                        c == '+' || c == '-' || c == '*' -> {
                            currentToken = Token(TokenType.ARITHMETIC_OPERATOR, c.toString(), lineNumber)
                            break
                        }
                        c == '/' -> state = State.InComment1
                        isWhiteSpace(c) -> continue
                        else -> break
                    }
                }
                State.InNum -> {
                    if (!isDigit(c)) {
                        goBack()
                        currentToken = Token(TokenType.CONSTANT_INT, literal.toString(), lineNumber)
                        break
                    }
                }
                State.InIdentifier -> {
                    if (!(isLetter(c) || isDigit(c) || c == '_')) {
                        goBack()
                        currentToken = if (isKeywords(literal.toString())) {
                            Token(TokenType.KEYWORD, literal.toString(), lineNumber)
                        } else {
                            Token(TokenType.IDENTIFIER, literal.toString(), lineNumber)
                        }
                        break
                    }
                }
                State.InAssign, State.InLess, State.InMore -> {
                    currentToken = if (c == '=') {
                        Token(TokenType.LOGICAL_OPERATOR, literal.toString(), lineNumber)
                    } else {
                        goBack()
                        Token(TokenType.ARITHMETIC_OPERATOR, literal.toString(), lineNumber)
                    }
                    break
                }
                State.InStr -> {
                    if (c == '"') {
                        currentToken = Token(TokenType.CONSTANT_STRING, literal.toString(), lineNumber)
                        break
                    }
                }
                State.InComment1 -> {
                    if (c == '/') {
                        state = State.InComment2
                    } else {
                        goBack()
                        currentToken = Token(TokenType.ARITHMETIC_OPERATOR, literal.toString(), lineNumber)
                        break
                    }
                }
                State.InComment2 -> {
                    if (c.toString() == System.lineSeparator()) {
                        currentToken = Token(TokenType.COMMENT, literal.toString(), lineNumber)
                        break
                    }
                }
                else -> break
            }

            literal.append(c)

        }

        if (currentToken == null) {
            return if (state == State.InIdentifier) Token(TokenType.IDENTIFIER, literal.toString(), lineNumber)
            else Token(TokenType.UNEXPECTED_TOKEN, literal.toString(), lineNumber)
        }

        return currentToken
    }
}