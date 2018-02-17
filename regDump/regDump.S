	.file	1 "dump.c"
	.section .mdebug.eabi64
	.previous
	.sdata
	.align	3

$LC0:	.ascii	"+-----+--------------------+\n"
	.text
	.align	3
$LC1:
	.ascii	"| $%02d | 0x%08x%08x |\n\000"
	.text
	.align	2
	.globl	main
	.ent	main

main:
	.frame	$fp,48,$31		# vars= 16, regs= 4/0, args= 0, extra= 0
	.mask	0xc0000000,-16	# .mask may not be correct
	.fmask	0x00000000,0
	subu	$sp,$sp,48
	sd	$31,32($sp)
	sd	$fp,16($sp)
	move	$fp,$sp

	# load a 128 bit value into a register from 2 64 bit values
	li	$2,0xbaadf00d	# load 64 bit value into register
	sd	$2,0($sp)	# push the register onto the stack
	li	$2,0xdeadbeef	# load another 64 bit value into register
	sd	$2,8($sp)	# push the register onto the stack
	move	$2,$0		# zero the register
	lq	$17,0($sp)	# load the full 128 bit value into the register
	sq	$0,0($sp)	# zero the part of the stack that was used

	la	$4,$LC0		# print table border
	jal	printf
	
	sq	$0,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,0
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$1,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,1
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$2,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,2
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$3,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,3
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$4,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,4
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$5,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,5
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$6,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,6
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$7,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,7
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$8,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,8
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$9,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,9
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$10,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,10
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$11,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,11
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$12,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,12
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$13,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,13
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$14,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,14
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$15,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,15
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$16,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,16
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$17,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,17
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$18,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,18
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$19,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,19
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$20,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,20
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$21,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,21
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$22,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,22
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$23,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,23
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$24,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,24
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$25,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,25
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$26,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,26
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$27,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,27
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$28,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,28
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$29,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,29
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$30,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,30
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf

	sq	$31,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,31
	ld	$6,0($sp)
	ld	$7,8($sp)
	jal	printf

	la	$4,$LC0		# print table border
	jal	printf
	
	move	$2,$0
	move	$sp,$fp
	ld	$31,32($sp)
	ld	$fp,16($sp)
	addu	$sp,$sp,48
	j	$31
	.end	main
$Lfe1:
	.size	main,$Lfe1-main
	.ident	"GCC: (GNU) 3.2.3"
