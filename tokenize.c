#include "rvcc.h"

// 输入的字符串
static char *CurrentInput;


// 输出错误信息
// static文件内可以访问的函数
void error(char *Fmt, ...){
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
void errorAt(char *Loc, char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    verrorAt(Loc, Fmt, VA);
    exit(1);
}

// Tok解析出错，并退出程序
void errorTok(Token *Tok, char *Fmt, ...){
    va_list VA;
    va_start(VA, Fmt);
    verrorAt(Tok->Loc, Fmt, VA);
    exit(1);
}

// 判断Tok是否等于指定值
bool equal(Token *Tok, char *Str){
    // 比较字符串LHS（左部），RHS（右部）的前N位，S2的长度应大于等于N.
    // 比较按照字典序，LHS<RHS回负值，LHS=RHS返回0，LHS>RHS返回正值
    // 同时确保，此处的Op位数=N
    // 只有当 Tok->Loc 指向的字符串的前 Tok->Len 个字符和 Str 相同,
    // 并且 Str 的长度和 Tok->Len 相等时，函数才返回 true，表示两个字符串相等。
    // 否则，函数返回 false，表示两个字符串不相等。
    return memcmp(Tok->Loc, Str, Tok->Len) == 0 && Str[Tok->Len] == '\0';
}

// 跳过指定的Str
Token *skip(Token *Tok, char *Str){
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

// 判断Str是否以SubStr开头
static bool startsWith(char *Str, char *SubStr) {
  // 比较LHS和RHS的N个字符是否相等
  return strncmp(Str, SubStr, strlen(SubStr)) == 0;
}

// 判断标记符的首字母规则
// [a-zA-Z_]
static bool isIdent1(char C) {
  // a-z与A-Z在ASCII中不相连，所以需要分别判断
  return ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_';
}

// 判断标记符的非首字母的规则
// [a-zA-Z0-9_]
static bool isIdent2(char C) { return isIdent1(C) || ('0' <= C && C <= '9'); }

// 读取操作符
static int readPunct(char *Ptr) {
  // 判断2字节的操作符
  if (startsWith(Ptr, "==") || startsWith(Ptr, "!=") || startsWith(Ptr, "<=") ||
      startsWith(Ptr, ">="))
    return 2;

  // 判断1字节的操作符
  return ispunct(*Ptr) ? 1 : 0;
}

// 判断是否为关键字
static bool isKeyword(Token *Tok){
    // 关键字列表
    static char *Kw[]={"return", "if", "else", "for", "while"};

    // 遍历关键字列表匹配
    for(int I = 0; I < sizeof(Kw) / sizeof(*Kw); ++I){
        if(equal(Tok, Kw[I]))
            return true;
    }

    return false;
}

// 将名为“return”的终结符转为KEYWORD
static void convertKeywords(Token *Tok){
    for(Token *T = Tok; T->Kind != TK_EOF; T= T->Next){
        if(isKeyword(T))
            T->Kind = TK_KEYWORD;
    }
}

// 终结符解析
Token *tokenize(char *P){
    CurrentInput = P;
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

        // 解析标记符或关键字
        // [a-zA-Z_][a-zA-Z0-9_]*
        if (isIdent1(*P)) {
        char *Start = P;
        do {
            ++P;
        } while (isIdent2(*P));
        Cur->Next = newToken(TK_IDENT, Start, P);
        Cur = Cur->Next;
        continue;
        }

        // 解析操作符
        int PunctLen = readPunct(P);
        if(PunctLen){
            // 操作符长度都为1
            Cur->Next = newToken(TK_PUNCT, P, P+PunctLen);
            Cur = Cur->Next;
            P += PunctLen;
            continue;
        }
        // 无法识别字符
        errorAt(P, "invalid token");
    }

    // 解析结束，增加终结符EOF
    Cur->Next = newToken(TK_EOF, P, P);
    // 将所有关键字的终结符，都标记为KEYWORD
    convertKeywords(Head.Next);
    // Head无内容，所以直接返回Next
    return Head.Next;
}
