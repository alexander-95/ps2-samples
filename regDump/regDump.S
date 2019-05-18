	.file	1 "dump.c"
	.section .mdebug.eabi64
	.previous
	.sdata
	.align	3

$LC0:	.ascii	"+-----+----------------------------------------+\n"
	.text
	.align	3
$LC1:
	.ascii	"| $%02d | 0x %08x %08x %08x %08x |\n\000"
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
	li	$2,0xbaadf00ddeadbeef	# load 64 bit value into register
	sd	$2,0($sp)	# push the register onto the stack
	li	$2,0xbadc0ffeecafebad	# load another 64 bit value into register
	sd	$2,8($sp)	# push the register onto the stack
	move	$2,$0		# zero the register
	lq	$17,0($sp)	# load the full 128 bit value into the register
	sq	$0,0($sp)	# zero the part of the stack that was used

	la	$4,$LC0		# print table border
	jal	printf

	sq	$17,0($sp)	# dump register onto stack

	la	$4,$LC1
	li	$5,17
	ld	$6,4($sp)
	ld	$7,0($sp)
        ld      $8,12($sp)
        ld      $9,8($sp)
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
