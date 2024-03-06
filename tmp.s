  # 定义全局main段
  .globl main

# =====程序开始===============
# main段标签，也是程序入口段
main:
  # 将fp压栈，fp属于“被调用者保存”的寄存器，需要恢复原值
  addi sp, sp, -8
  sd fp, 0(sp)
  # 将sp的值写入fp
  mv fp, sp
  # sp腾出StackSize大小的栈空间
  addi sp, sp, -16

# =====程序主体===============
  # 获取变量x的栈内地址为-16(fp)
  addi a0, fp, -16
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将3加载到a0中
  li a0, 3
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # 将a0的值，写入到a1中存放的地址
  sd a0, 0(a1)
  # 获取变量y的栈内地址为-8(fp)
  addi a0, fp, -8
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将5加载到a0中
  li a0, 5
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # 将a0的值，写入到a1中存放的地址
  sd a0, 0(a1)
# 返回语句
  # 获取变量y的栈内地址为-8(fp)
  addi a0, fp, -8
  # 读取a0中存放的地址，得到的值存入a0
  ld a0, 0(a0)
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 获取变量x的栈内地址为-16(fp)
  addi a0, fp, -16
  # 读取a0中存放的地址，得到的值存入a0
  ld a0, 0(a0)
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
  addi sp, sp, 8
  # 返回a0值给系统调用
  ret
