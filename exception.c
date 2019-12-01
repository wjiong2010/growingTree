#include "uart.h"

void print_excepiton(unsigned int cpsr, char *str)
{
	puts("\r\n");	
	puts(str);
	puts(" cpsr=");	
	printHex(cpsr);
	puts("\r\n");	
}

void print_swi_value(unsigned int *swi_p)
{
	puts("\r\nswi_value=");	
	printHex(*swi_p & 0x00ffffff);
}


