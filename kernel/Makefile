SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
DTB = "../shares/bcm2710-rpi-3-b-plus.dtb"
CPIO = "../shares/initramfs.cpio"

LIBRARY_PATH = ../library
INCLUDES = -I ./include \
	$(LIBRARY_INCLUDE)

LIBRARY_INCLUDE = -I $(LIBRARY_PATH)/include
LIBRARY_SRCS = $(LIBRARY_PATH)/*.c
LIBRARY_OBJS = $(LIBRARY_SRCS:.c=.o)

all: clean kernel8.img user_prog.img

start.o: start.S
	aarch64-linux-gnu-gcc $(CFLAGS) -c start.S -o start.o

%.o: %.c
	aarch64-linux-gnu-gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

kernel8.img: start.o $(OBJS) initramfs.cpio
	aarch64-linux-gnu-ld start.o $(OBJS) $(LIBRARY_OBJS) -T link.ld -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img

initramfs.cpio:
	cd rootfs && find . | cpio -o -H newc > ../initramfs.cpio
	cp initramfs.cpio ${CPIO}
	rm initramfs.cpio

user_prog.o: user_prog.S
	aarch64-linux-gnu-gcc $(CFLAGS) -c user_prog.S -o user_prog.o

user_prog.img: user_prog.o $(OBJS)
	aarch64-linux-gnu-ld user_prog.o $(OBJS) $(LIBRARY_OBJS) -T link.ld -o user_prog.elf
	aarch64-linux-gnu-objcopy -O binary user_prog.elf user_prog.img
	cp user_prog.img rootfs/user_prog

clean:
	rm *.elf *.img *.o 2> /dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio -display none -initrd $(CPIO) -dtb $(DTB)

debug:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio -display none -initrd $(CPIO) -dtb $(DTB) -S -s