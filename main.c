#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

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


static char *CurrentInput;

// 输出错误信息
// static文件内可以访问的函数
static void error(char *Fmt, ...){
    // 定义一个va_list变量
    va_list VA;
    // VA获取Fmt后面的所有参数
    va_start(VA, Fmt);
    // vfprintf可以输出va_list类型的参数
    vfprintf(stderr, Fmt, VA);
    // 在结尾加上一个换行符
    fprintf(stderr, "\n");
    // 清除VA
    va_end(VA);
    // end program
    exit(1);
}

// 输出错误出现的位置
static void verrorAt(char *Loc, char *Fmt, va_list VA){
    // 输出源信息
    fprintf(stderr, "%s\n", CurrentInput);

    // 输出错误信息
    // 计算出错误位置，Loc是出错位置的指针
    // CurrentInput是当前输入位置的首地址
    int Pos = Loc - CurrentInput;
    // 将字符串补齐为Pos位，因为算空字符串，所以填充Pos个空格。
    fprintf(stderr, "%*s", Pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, Fmt, VA);
    fprintf(stderr, "\n");
    va_end(VA);
}

// 字符解析出错，并退出程序
static void errorAt(char *Loc, char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    verrorAt(Loc, Fmt, VA);
    exit(1);
}

// Tok解析出错，并退出程序
static void errorTok(Token *Tok, char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    verrorAt(Tok->Loc, Fmt, VA);
    exit(1);
}

// 判断Tok是否等于指定值
static bool equal(Token *Tok, char *Str){
    // 比较字符串LHS（左部），RHS（右部）的前N位，S2的长度应大于等于N.
    // 比较按照字典序，LHS<RHS回负值，LHS=RHS返回0，LHS>RHS返回正值
    // 同时确保，此处的Op位数=N
    // 只有当 Tok->Loc 指向的字符串的前 Tok->Len 个字符和 Str 相同,
    // 并且 Str 的长度和 Tok->Len 相等时，函数才返回 true，表示两个字符串相等。
    // 否则，函数返回 false，表示两个字符串不相等。
    return memcmp(Tok->Loc, Str, Tok->Len) == 0 && Str[Tok->Len] == '\0';
}

// 跳过指定的Str
static Token *skip(Token *Tok, char *Str){
    if(!equal(Tok, Str))
        error("expect '%s'", Str);
    return Tok->Next;
}

// 返回TK_NUM的值
static int getNumber(Token *Tok){
    if(Tok->Kind != TK_NUM)
        errorTok(Tok, "expect a number");
    return Tok->Val;
}

// 生成新的Token
static Token *newToken(TokenKind Kind, char *Start, char *End){
    // 分配1个Token的内存空间
    Token *Tok = calloc(1, sizeof(Token));
    Tok->Kind = Kind;
    Tok->Loc = Start;
    Tok->Len = End - Start;
    return Tok;
}

// 终结符解析
static Token *tokenize(){
    char *P = CurrentInput;
    Token Head = {};
    Token *Cur = &Head;
    
    while(*P){
        // 跳过所有空白符，如：空格、回车
        if(isspace(*P)){
            ++P;
            continue;
        }

        // 解析数字
        if(isdigit(*P)){
            // 初始化，类似于C++的构造函数
            // 我们不使用Head来存储信息，仅用来表示链表入口，这样每次都是存储在Cur->Next
            // 否则下述操作将使第一个Token的地址不在Head中。
            Cur->Next = newToken(TK_NUM, P, P);
            Cur = Cur->Next;
            const char *OldPtr = P;
            Cur->Val = strtoul(P, &P, 10);
            Cur->Len = P - OldPtr;
            continue;
        }

        // 解析操作符
        if(*P == '+' || *P == '-'){
            // 操作符长度都为1
            Cur->Next = newToken(TK_PUNCT, P, P+1);
            Cur = Cur->Next;
            ++P;
            continue;
        }

        // 无法识别字符
        errorAt(P, "invalid token");
    }

    // 解析结束，增加终结符EOF
    Cur->Next = newToken(TK_EOF, P, P);
    // Head无内容，所以直接返回Next
    return Head.Next;
}



int main(int Argc, char **Argv){
    if(Argc != 2){
        // 异常处理，提示参数数量不正确。
        error("%s: invalid number of arguments", Argv[0]);
    }

    // 解析Argv[1]
    CurrentInput = Argv[1];
    Token *Tok = tokenize();

    printf("  .globl main\n");
    printf("main:\n");

    printf("  li a0, %d\n", getNumber(Tok));
    Tok = Tok->Next;

    // 解析（op num）
    while(Tok->Kind != TK_EOF){
        if(equal(Tok, "+")){
            Tok = Tok->Next;
            printf("  addi a0, a0, %d\n", getNumber(Tok));
            Tok = Tok->Next;
            continue;
        }

        // 若不是+，则判断-。没用subi指令但addi支持有符号数
        // 所以对num取反
        Tok = skip(Tok, "-");
        printf("  addi a0, a0, -%d\n", getNumber(Tok));
        Tok = Tok->Next;
    }

    printf("  ret\n");
    
    return 0;
}