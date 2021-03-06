package scanner

/**
 * 可以以不同的方式(根据具体实现)给Lex提供源程序的一行文本
 * @Author qingyingliu
 * @Date 2021/3/6 19:51
 */
interface LineProvider {
    fun nextLine(): String?
}