#include "s3c2440_soc.h"

/* 
 * INTPND 用来清中断 
 * INTOFFSET 用来确定哪一个中断被触发
 */
/* 读里面的值用来分辨哪个中断产生了 
 * 清除对应位清中断
 * EINTPEND &= ~((0x1<<11) | (0x1<<19));
 */
/* SRCPND
*/

/* 初始化中断 */
void eint_init(void)
{
	/* 不使能FIQ, 置0就好 */
	INTMOD = 0;

	/* 使能EINT 0, 2, 11, 19 */
	INTMSK &= ~(0x01 | (0x01<<2) | (0x01<<5));
}

/* 初始化中断源 */
void eint_src_init(void)
{
	/* 设置引脚用于中断 */
	GPFCON &= ~(0x3 | (0x3<<4));
	GPFCON |= (0x2 | (0x2<<4));

	GPGCON &= ~((0x3<<6) | (0x3<<22));
	GPGCON |= ((0x2<<6) | (0x2<<22));

	/* S2, S3 */
	EXTINT0 &= ~(0x7 | (0x7<<8));
	EXTINT0 |= (0x7 | (0x7<<8));
	/* S4 */
	EXTINT1 &= ~(0x7<<12);
	EXTINT1 |= (0x7<<12);
	/* S5 */
	EXTINT2 &= ~(0x7<<12);
	EXTINT2 |= (0x7<<12);

	/* 设置EINTMASK 使能EINT */
	EINTMASK &= ~((0x1<<11) | (0x1<<19));

}


void irq_hdlr(void)
{
	int eint_pnd = EINTPEND;
	int src_pnd = SRCPND;
	int int_pnd = INTPND;
	
	puts("\r\n");	
	puts(" _irq");	
	puts(" EINTPEND=");	
	printHex(EINTPEND);
	puts(" INTOFFSET=");	
	printHex(INTOFFSET);
	puts(" INTPND=");	
	printHex(INTPND);
	puts("\r\n");	

	EINTPEND = eint_pnd;
	SRCPND = src_pnd;
	INTPND = int_pnd;
}



