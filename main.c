#include "rvcc.h"

int main(int Argc, char **Argv){

    if(Argc != 2){

        // 异常处理，提示参数数量不正确。

        error("%s: invalid number of arguments", Argv[0]);
    }

    // 解析Argv[1]
    Token *Tok = tokenize(Argv[1]);

    // 解析终结符流
   Function *Prog = parse(Tok);

    codegen(Prog);

    return 0;
}
