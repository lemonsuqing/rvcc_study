// 使用POSIX.1标准
// 使用了strndup函数
#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// 共用头文件，定义了多个文件间共同使用的函数和数据
//

//
// 终结符分析，词法分析
//

// 设置每个人终结符的种类
typedef enum{
    TK_IDENT,   // 标记符，可以为变量名、函数名等
    TK_PUNCT,   // 操作符，如：+ - * /
    TK_KEYWORD, // 关键字
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
bool consume(Token **Rest, Token *Tok, char *Str);
// 词法分析
Token *tokenize(char *Input);

//
// 生成AST（抽象语法树），语法解析
//

typedef struct Type Type;
typedef struct Node Node;

// 本地变量
typedef struct Obj Obj;
struct Obj {
  Obj *Next;  // 指向下一对象
  char *Name; // 变量名
  Type *Ty;   // 变量类型
  int Offset; // fp的偏移量
};

// 函数
typedef struct Function Function;
struct Function {
  Function *Next; // 下一函数
  char *Name;     // 函数名
  Obj *Params;    // 形参

  Node *Body;    // 函数体
  Obj *Locals;   // 本地变量
  int StackSize; // 栈大小
};

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
    ND_ASSIGN,      // 赋值
    ND_ADDR,        // 取地址 &
    ND_DEREF,       // 解引用 *
    ND_RETURN,      // 返回
    ND_IF,          // "if" 条件判断
    ND_FOR,         // "for" 或 "while"，循环
    ND_BLOCK,       // { ... }, 代码块
    ND_FUNCALL,     // 函数调用
    ND_EXPR_STMT,   // 表达式语句
    ND_VAR,         // 变量
    ND_NUM,         // 数字
} NodeKind;

// AST中二叉树节点
struct Node {
    NodeKind Kind;  //节点种类
    Node *Next;     // 指向下一个节点
    Token *Tok;     // 节点对应的终结符
    Type *Ty;       // 节点中数据的类型
  
    Node *LHS;      // 左部，left-hand side
    Node *RHS;      // 右部，right-hand side

    // "if" 语句 或者 "for"语句
    Node *Cond; // 条件内的表达式
    Node *Then; // 符合条件后的语句
    Node *Els;  // 不符合条件后的语句
    Node *Init; // 初始化语句
    Node *Inc;  // 递增语句

    // 代码块
    Node *Body;

    // 函数调用
    char *FuncName; // 函数名
    Node *Args;     // 函数参数

    Obj *Var;       // 存储ND_VAR种类的变量
    int Val;        // 存储ND_NUM种类的值
};

// 语法解析入口函数
Function *parse(Token *Tok);

//
// 类型系统
//

// 类型种类
typedef enum{
  TY_INT,   // int整型
  TY_PTR,   // 指针
  TY_FUNC,  // 函数
  TY_ARRAY, // 数组
} TypeKind;

struct Type {
  TypeKind Kind; // 种类
  int Size;      // 大小

  // 指针
  Type *Base;    // 指向的类型

  // 类型对应名称，如：变量名、函数名
  Token *Name;

  // 数组
  int ArrayLen; // 数组长度, 元素总个数
  
  // 函数类型
  Type *ReturnTy; // 函数返回的类型
  Type *Params;   // 形参
  Type *Next;     // 下一类型
};

// 声明一个全局变量，定义在type.c中。
extern Type *TyInt;

// 判断是否为整型
bool isInteger(Type *TY);
// 复制类型
Type *copyType(Type *Ty);
// 构建一个指针类型，并指向基类
Type *pointerTo(Type *Base);
// 为节点内的所有节点添加类型
void addType(Node *Nd);
// 数组类型
Type *arrayOf(Type *Base, int Size);
// 函数类型
Type *funcType(Type *ReturnTy);

//
// 语义分析与代码生成
//

// 代码生成入口函数
void codegen(Function *Prog);
