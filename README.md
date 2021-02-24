# Simple
Simple是一个简单的语言，这个项目是设计Simple语言，以及实现Simple编译器前端。

Simple语言主要的语言特性：

简单变量、简单常量、简单四则运算、简单布尔运算、顺序结构、选择结构、do-while循环和赋值语句等语言特性.



## 词法和语法规则

在Simple语言中，词法的规则如下：

简单变量 = (字母|下划线)( 字母|下划线|数字)*

简单常量 = int常量 | 字符串常量

Int常量 = 数字

字符串常量 = “(不为双引号字母)*”

字母 = [a-z] | [A-Z]

下划线 = _

数字 = [0-9]([0-9])*

简单四则运算 = + | - | * | /

简单布尔运算 = && | || | ! (C语言中的and、or、not)

比较运算: > | < | >= | <= | == | !=

单行注释: //（不为换行符的字符）*

保留字: if then else end do while int string bool


语法规则如下:


program -> statement;stmt_seq // 至少有一条语句

stmt_seq -> (statement;)* // // 一条或者多条语句，以逗号结尾

statement -> define-stmt | if-stmt | assign-stmt | while-stmt

define-stmt -> type id-list

if-stmt -> if bool-exp then stmt_seq end | if bool-exp then stmt_seq else stmt_seq end

assign-stmt -> id = arithmetic-exp | const_str

while-stmt -> do stmt_seq while bool-exp


## 词法分析

词法分析主要是使用lex工具实现。



## 语法分析

语法分析使用的是递归下降法，输入是词法分析的token序列，输出是一颗语法树。



## 语义分析

建立符号表，检查变量是否重复定义，是否未定义。

类型检查，检查操作是否合法。比如1 + "str"就不合法。



## 生成中间代码

将语法树以中间代码（四元式）输出。

