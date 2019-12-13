#include "s3c2440_soc.h"
#include "led.h"

/* 
 * INTPND �������ж� 
 * INTOFFSET ����ȷ����һ���жϱ�����
 */
/* �������ֵ�����ֱ��ĸ��жϲ����� 
 * �����Ӧλ���ж�
 * EINTPEND &= ~((0x1<<11) | (0x1<<19));
 */
/* SRCPND
*/

/* ��ʼ���ж� */
void eint_init(void)
{
	/* ��ʹ��FIQ, ��0�ͺ� */
	INTMOD = 0;

	/* ʹ��EINT 0, 2, 11, 19 */
	INTMSK &= ~(0x01 | (0x01<<2) | (0x01<<5));
}

/* ��ʼ���ж�Դ */
void eint_src_init(void)
{
	/* �������������ж� */
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

	/* ����EINTMASK ʹ��EINT */
	EINTMASK &= ~((0x1<<11) | (0x1<<19));

}

static void key_irq_hdlr(volatile unsigned int bit)
{
	/*
		S2 : EINT0 -> GPF0 		����   GPF4	LED1 -> D10
		S3 : EINT2 -> GPF2 		���� GPF5		LED2 -> D11
		S4 : EINT11 -> GPG3 	����	 GPF6	LED4 -> D12
		S5 : EINT19 -> GPG11 	����		ALL
	*/
	int eint_pnd = EINTPEND;

	switch (bit)
	{
		case 0:
		{
			if (GPFDAT & (1<<0))
			{
				turn_off_led(LED_1);
			}
			else
			{
				turn_on_led(LED_1);
			}
			break;
		}
		case 2:
		{
			if (GPFDAT & (1<<2))
			{
				turn_off_led(LED_2);
			}
			else
			{
				turn_on_led(LED_2);
			}
			break;
		}
		case 5:
		{
			puts("\r\n");	
			puts(" GPGDAT=");	
			printHex(GPGDAT);
			puts("\r\n");

			if (eint_pnd & (1<<11))
			{
				if (GPGDAT & (1<<3))
				{
					/* Released */
					turn_off_led(LED_2);
				}
				else
				{
					/* Pressed */
					turn_on_led(LED_2);
				}
			}
			else
			if (eint_pnd & (1<<19))
			{
				if (GPGDAT & (1<<11))
				{
					/* Released */
					turn_off_led(LED_1);
					turn_off_led(LED_2);
					turn_off_led(LED_4);
				}
				else
				{
					/* Pressed */
					turn_on_led(LED_1);
					turn_on_led(LED_2);
					turn_on_led(LED_4);
				}
			}
			
			break;
		}
		default:
		{
		}
	}
	
	EINTPEND &= eint_pnd;
}

void irq_hdlr(void)
{
	int src_pnd = SRCPND;
	int int_pnd = INTPND;
	#if 0
	puts("\r\n");	
	puts(" _irq");	
	puts(" EINTPEND=");	
	printHex(EINTPEND);
	puts(" INTOFFSET=");	
	printHex(INTOFFSET);
	puts(" INTPND=");	
	printHex(INTPND);
	puts("\r\n");	
	#else
	key_irq_hdlr(INTOFFSET);
	#endif
	SRCPND &= src_pnd;
	INTPND &= int_pnd;
}



