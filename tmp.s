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
  # 将1加载到a0中
  li a0, 1
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将2加载到a0中
  li a0, 2
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将3加载到a0中
  li a0, 3
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将4加载到a0中
  li a0, 4
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将5加载到a0中
  li a0, 5
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将6加载到a0中
  li a0, 6
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将7加载到a0中
  li a0, 7
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将8加载到a0中
  li a0, 8
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将9加载到a0中
  li a0, 9
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 弹栈，将栈顶的值存入a5
  ld a5, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a4
  ld a4, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a3
  ld a3, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a2
  ld a2, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a0
  ld a0, 0(sp)
  addi sp, sp, 8
  # 调用add6函数
  call add6
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将10加载到a0中
  li a0, 10
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将11加载到a0中
  li a0, 11
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将12加载到a0中
  li a0, 12
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将13加载到a0中
  li a0, 13
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 弹栈，将栈顶的值存入a5
  ld a5, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a4
  ld a4, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a3
  ld a3, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a2
  ld a2, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a0
  ld a0, 0(sp)
  addi sp, sp, 8
  # 调用add6函数
  call add6
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将14加载到a0中
  li a0, 14
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将15加载到a0中
  li a0, 15
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将16加载到a0中
  li a0, 16
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 弹栈，将栈顶的值存入a5
  ld a5, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a4
  ld a4, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a3
  ld a3, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a2
  ld a2, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # 弹栈，将栈顶的值存入a0
  ld a0, 0(sp)
  addi sp, sp, 8
  # 调用add6函数
  call add6
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
