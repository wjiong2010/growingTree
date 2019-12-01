all: start.o init.o exception.o main.o led.o uart.o  
	#arm-linux-ld -Ttext 0 -Tdata 0x30000000  start.o led.o uart.o init.o main.o -o sdram.elf
	arm-linux-ld -T sdram.lds $^ -o exception.elf
	arm-linux-objcopy -O binary -S exception.elf exception.bin
	arm-linux-objdump -D exception.elf > exception.dis

%.o: %.c
	arm-linux-gcc -c -o $@ $<

%.o : %.S
	arm-linux-gcc -c -o $@ $<

clean:
	rm *.bin *.o *.elf *.dis
	
