#include <stdio.h>
#include <stdlib.h>

int main(int Argc, char **Argv){
    if(Argc != 2){
        // 异常处理，提示参数数量不正确。
        fprintf(stderr,"%s: invalid number of arguments\n", Argv[0]);
        return 1;
    }

    char *P = Argv[1];
    // 声明一个全局main段，同时也是程序入口段
    printf("  .globl main\n");
    // main段标签
    printf("main:\n");

    printf("  li a0, %ld\n", strtol(P, &P, 10));
    // 将命令行参数Argv[1]转化成long型

    while (*P){
        if(*P == '+'){
            ++P; //跳过‘+’
            // 解析num
            // addi rd, rs1, imm
            printf("  addi a0, a0, %ld\n", strtol(P, &P, 10));
            continue;
        }

        if(*P == '-'){
            ++P; //跳过‘-’
            // 解析num
            // addi rd, rs1, imm(-imm)
            printf("  addi a0, a0, -%ld\n", strtol(P, &P, 10));
            continue;
        }

        // 如果存在未解析的字符，则报错
        fprintf(stderr, "unexpected character: '%c'\n", *P);
        return 1;
    }
    

    printf("  ret\n");
    
    return 0;
}