  .globl main
main:
  addi sp, sp, -8
  sd fp, 0(sp)
  mv fp, sp
  addi sp, sp, -0
.L.begin.1:
  li a0, 3
  j .L.return
  j .L.begin.1
.L.end.1:
  li a0, 5
  j .L.return
.L.return:
  mv sp, fp
  ld fp, 0(sp)
  addi sp, sp, 8
  ret
