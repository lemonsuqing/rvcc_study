  # 定义全局main段
  .globl main

# =====程序开始===============
# main段标签，也是程序入口段
main:
  # 将ra寄存器压栈,保存ra的值
  addi sp, sp, -16
  sd ra, 8(sp)
  # 将fp压栈，fp属于“被调用者保存”的寄存器，需要恢复原值
  sd fp, 0(sp)
  # 将sp的值写入fp
  mv fp, sp
  # sp腾出StackSize大小的栈空间
  addi sp, sp, -0

# =====程序主体===============
# 返回语句

  # 调用函数ret5
  call ret5
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)

  # 调用函数ret3
  call ret3
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0+a1，结果写入a0
  add a0, a0, a1
  # 跳转到.L.return段
  j .L.return

# =====程序结束===============
# return段标签
.L.return:
  # 将fp的值写回sp
  mv sp, fp
  # 将最早fp保存的值弹栈，恢复fp和sp
  ld fp, 0(sp)
  # 将ra寄存器弹栈,恢复ra的值
  ld ra, 8(sp)
  addi sp, sp, 16
  # 返回a0值给系统调用
  ret
