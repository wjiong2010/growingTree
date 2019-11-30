#include "uart.h"

void print_excepiton(unsigned int cpsr, char *str)
{
	puts("und excepiton cpsr=");	
	printHex(cpsr);
	puts(str);
	puts("\r\n");	
}
