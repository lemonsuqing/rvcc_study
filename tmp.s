
  # 定义全局main段
  .globl main
# =====main段开始===============
# main段标签
main:
  # 将ra寄存器压栈,保存ra的值
  addi sp, sp, -16
  sd ra, 8(sp)
  # 将fp压栈，fp属于“被调用者保存”的寄存器，需要恢复原值
  sd fp, 0(sp)
  # 将sp的值写入fp
  mv fp, sp
  # sp腾出StackSize大小的栈空间
  addi sp, sp, -32
# =====main段主体===============
  # 获取变量x的栈内地址为-24(fp)
  addi a0, fp, -24
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
  # 将8加载到a0中
  li a0, 8
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将1加载到a0中
  li a0, 1
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0×a1，结果写入a0
  mul a0, a0, a1
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 获取变量x的栈内地址为-24(fp)
  addi a0, fp, -24
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0+a1，结果写入a0
  add a0, a0, a1
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将4加载到a0中
  li a0, 4
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # 将a0的值，写入到a1中存放的地址
  sd a0, 0(a1)
  # 将8加载到a0中
  li a0, 8
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将2加载到a0中
  li a0, 2
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0×a1，结果写入a0
  mul a0, a0, a1
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 获取变量x的栈内地址为-24(fp)
  addi a0, fp, -24
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0+a1，结果写入a0
  add a0, a0, a1
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
  # 将8加载到a0中
  li a0, 8
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 将2加载到a0中
  li a0, 2
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0×a1，结果写入a0
  mul a0, a0, a1
  # 压栈，将a0的值存入栈顶
  addi sp, sp, -8
  sd a0, 0(sp)
  # 获取变量x的栈内地址为-24(fp)
  addi a0, fp, -24
  # 弹栈，将栈顶的值存入a1
  ld a1, 0(sp)
  addi sp, sp, 8
  # a0+a1，结果写入a0
  add a0, a0, a1
  # 读取a0中存放的地址，得到的值存入a0
  ld a0, 0(a0)
  # 跳转到.L.return.main段
  j .L.return.main
# =====main段结束===============
# return段标签
.L.return.main:
  # 将fp的值写回sp
  mv sp, fp
  # 将最早fp保存的值弹栈，恢复fp和sp
  ld fp, 0(sp)
  # 将ra寄存器弹栈,恢复ra的值
  ld ra, 8(sp)
  addi sp, sp, 16
  # 返回a0值给系统调用
  ret
