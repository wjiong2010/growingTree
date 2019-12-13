
.text
.global _start


_start:
	b reset  						/* vector 0x00 : reset */
	ldr pc, _undefined_instruction 	/* vector 0x04 : und */
	ldr pc, _software_interrupt 	/* vector 0x08 : swi */
	bl halt							/* vector 0x0c : prefetch_abort */
	bl halt							/* vector 0x10 : data_abort */
	bl halt							/* vector 0x14 : reserved */
	ldr	pc, _irq					/* vector 0x18 : irq */
	bl halt							/* vector 0x1c : fiq */
	
_undefined_instruction:
	.word undefined_instruction

undefined_instruction:
	/* 执行到这里之前:
	 * 1. lr_und保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_und保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为11011, 进入到und模式
	 * 4. 跳到0x4的地方执行程序 
	 */

	/* sp_und未设置, 先设置它 */
	ldr sp, =0x34000000

	/* 在und异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr是异常处理完后的返回地址, 也要保存 */
	stmdb sp!, {r0-r12, lr}  
	
	/* 保存现场 */
	/* 处理und异常 */
	mrs r0, cpsr
	ldr r1, =und_string
	bl print_excepiton
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr的值恢复到cpsr里 */
	
und_string:
	.string "undefined_instruction exception"

.align 4

_software_interrupt:
	.word software_interrupt

software_interrupt:
	/* 执行到这里之前:
	 * 1. lr_svc保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_svc保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为10011, 进入到svc模式
	 * 4. 跳到0x8的地方执行程序 
	 */

	/* sp_und未设置, 先设置它 */
	ldr sp, =0x33f00000

	/* 在und异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr是异常处理完后的返回地址, 也要保存 */
	stmdb sp!, {r0-r12, lr}  

	/* 保存LR值， 用来提取SWI命令的参数 */
	mov r4, lr
	
	/* 保存现场 */
	/* 处理und异常 */
	mrs r0, cpsr
	ldr r1, =swi_string
	bl print_excepiton

	sub r4, r4, #4
	mov r0, r4
	bl print_swi_value
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr的值恢复到cpsr里 */
	
swi_string:
	.string "software_interrupt exception"

.align 4

_irq:
	.word irq

irq:
	/* 执行到这里之前:
	 * 1. lr_irq保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_irq保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为10010, 进入到irq模式
	 * 4. 跳到0x4的地方执行程序 
	 */

	/* sp_irq未设置, 先设置它 */
	ldr sp, =0x33d00000

	/* 保存现场 */
	/* 在irq异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr - 4是异常处理完后的返回地址, 也要保存 */
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	
	
	/* 处理irq异常 */
	bl irq_hdlr
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把irq_spsr的值恢复到cpsr里 */

.align 4

reset:
	/* 关闭看门狗 */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	/* 设置MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	/* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0X5, tFCLK:tHCLK:tPCLK = 1:4:8  */
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* 设置MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0) 
	 *  m = MDIV+8 = 92+8=100
	 *  p = PDIV+2 = 1+2 = 3
	 *  s = SDIV = 1
	 *  FCLK = 2*m*Fin/(p*2^s) = 2*100*12/(3*2^1)=400M
	 */
	ldr r0, =0x4C000004
	ldr r1, =(92<<12)|(1<<4)|(1<<0)
	str r1, [r0]

	/* 一旦设置PLL, 就会锁定lock time直到PLL输出稳定
	 * 然后CPU工作于新的频率FCLK
	 */

	/* 设置内存: sp 栈 */
	/* 分辨是nor/nand启动
	 * 写0到0地址, 再读出来
	 * 如果得到0, 表示0地址上的内容被修改了, 它对应ram, 这就是nand启动
	 * 否则就是nor启动
	 */
	mov r1, #0
	ldr r0, [r1] /* 读出原来的值备份 */
	str r1, [r1] /* 0->[0] */ 
	ldr r2, [r1] /* r2=[0] */
	cmp r1, r2   /* r1==r2? 如果相等表示是NAND启动 */
	ldr sp, =0x40000000+4096 /* 先假设是nor启动 */
	moveq sp, #4096  /* nand启动 */
	streq r0, [r1]   /* 恢复原来的值 */

	bl sdram_init
	//bl sdram_init2	 /* 用到有初始值的数组, 不是位置无关码 */

	/* 重定位text, rodata, data段整个程序 */
	bl copy2sdram

	/* 清除BSS段 */
	bl clean_bss

	/* 此时CPU工作在svc模式,低5bit(Mode bit = 0b10011)。
	 * 通过直接修改后4bits为0的方式改为usr模式(Mode bits = 0b10000) 
	 * usr模式不能直接操作硬件，只能靠中断的形式切换到其他模式
	 */
	mrs r0, cpsr
	bic r0, r0, #0xf
	bic r0, r0, #0x80 	/* 清除CPSR的bit7， 打开IRQ终端 */
	msr cpsr, r0

	/* 设置在usr 模式下的栈 */
	ldr sp, =0x33e00000

	ldr pc, =sdram
sdram:
	/*初始化串口*/
	bl uart0_init

	bl uart_print1
	/* 故意加入一条未定义指令 */
und_code:
	.word 0xdeadc0de  /* 未定义指令 */
	bl uart_print2

	/* 执行swi命令，触发swi中断 */
	swi 0x123

	//bl main  /* 使用BL命令相对跳转, 程序仍然在NOR/sram执行 */
	ldr pc, =main  /* 绝对跳转, 跳到SDRAM */

halt:
	b halt
	
