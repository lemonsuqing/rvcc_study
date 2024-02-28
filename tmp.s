  .globl main
main:
  li a0, 4
  neg a0, a0
  neg a0, a0
  neg a0, a0
  neg a0, a0
  addi sp, sp, -8
  sd a0, 0(sp)
  li a0, 12
  neg a0, a0
  neg a0, a0
  neg a0, a0
  neg a0, a0
  neg a0, a0
  neg a0, a0
  ld a1, 0(sp)
  addi sp, sp, 8
  mul a0, a0, a1
  ret
