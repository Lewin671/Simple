package scanner

/**
 * @Author qingyingliu
 * @Date 2021/3/6 19:53
 */
enum class State {
    Start,
    InNum,
    InIdentifier,
    InAssign,
    InLess,
    InMore,
    InStr,
    InComment1,
    InComment2,
    Done
}