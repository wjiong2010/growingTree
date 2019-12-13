all: start.o init.o exception.o main.o led.o uart.o interrupt.o
	#arm-linux-ld -Ttext 0 -Tdata 0x30000000  start.o led.o uart.o init.o main.o -o sdram.elf
	arm-linux-ld -T sdram.lds $^ -o gtree.elf
	arm-linux-objcopy -O binary -S gtree.elf gtree.bin
	arm-linux-objdump -D gtree.elf > gtree.dis

%.o: %.c
	arm-linux-gcc -c -o $@ $<

%.o : %.S
	arm-linux-gcc -c -o $@ $<

clean:
	rm *.bin *.o *.elf *.dis
	
