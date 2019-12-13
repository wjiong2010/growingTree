
#include "s3c2440_soc.h"
#include "uart.h"
#include "init.h"

char g_Char = 'A';
char g_Char3 = 'a';
const char g_Char2 = 'B';
int g_A = 0;
int g_B;

int main(void)
{
	/* 初始化中断控制器 */
	eint_init();
	/* 初始化中断源 */
	eint_src_init();
	led_init();

	puts("\n\rg_A = ");
	printHex(g_A);
	puts("\n\r");

	puts("john test\r\n");

	while (1)
	{
#if 0		
		puts("\n\rg_Char = ");
		printHex(g_Char);
		puts("\n\r");


		puts("\n\rg_Char3 = ");
		printHex(g_Char3);
		puts("\n\r");
#endif

		putchar(g_Char);
		g_Char++;

		putchar(g_Char3);
		g_Char3++;
		delay(1000000);
	}

	
	return 0;
}



