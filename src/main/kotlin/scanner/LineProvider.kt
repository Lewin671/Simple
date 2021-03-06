package scanner

/**
 * You can provide chars to Scanner with CharProvider
 * You can implement it by many ways
 * @Author qingyingliu
 * @Date 2021/3/6 19:51
 */
interface CharProvider {
    fun nextLine():String?
}