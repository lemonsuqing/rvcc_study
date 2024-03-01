#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

//
// 共用头文件，定义了多个文件间共同使用的函数和数据
//

//
// 终结符分析，词法分析
//

// 设置每个人终结符的种类
typedef enum{
    TK_PUNCT,   // 操作符，如：+ - * /
    TK_NUM,     // 数字
    TK_EOF,     // 文件终止符
} TokenKind;

// 终结符结构体 用'Token'代替'struct Token'
typedef struct Token Token;
struct Token{
    TokenKind Kind; // 种类
    Token *Next;    // 指向下一个终结符
    int Val;        // 值
    char *Loc;      // 在‘解析的字符串’内的位置
    int Len;        // 长度
};

// 去除了static用以在多个文件间访问
// 报错函数
void error(char *Fmt, ...);
void errorAt(char *Loc, char *Fmt, ...);
void errorTok(Token *Tok, char *Fmt, ...);
// 判断Token与Str的关系
bool equal(Token *Tok, char *Str);
Token *skip(Token *Tok, char *Str);
// 词法分析
Token *tokenize(char *Input);

//
// 生成AST（抽象语法树），语法解析
//

// AST的节点种类
typedef enum{
    ND_ADD,         // +
    ND_SUB,         // -
    ND_MUL,         // *
    ND_DIV,         // /
    ND_NEG,         // 负号 - 不是减号
    ND_EQ,          // ==
    ND_NE,          // !=
    ND_LT,          // <
    ND_LE,          // <=
    ND_EXPR_STMT,   // 表达式语句
    ND_NUM,         // 数字
} NodeKind;

// AST中二叉树节点
typedef struct Node Node;
struct Node {
    NodeKind Kind;  //节点种类
    Node *Next;     // 指向下一个节点
    Node *LHS;      // 左部，left-hand side
    Node *RHS;      // 右部，right-hand side
    int Val;        // 存储ND_NUM种类的值
};

// 语法解析入口函数
Node *parse(Token *Tok);

//
// 语义分析与代码生成
//

// 代码生成入口函数
void codegen(Node *Nd);