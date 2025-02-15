#include "cpio.h"
#include "uart.h"
#include "helper.h"
#include "exec.h"
#include "malloc.h"

void init_cpio(char *address)
{
    cpio_base = address;
}

char check_magic_number(char *address)
{
    int size = string_length(CPIO_MAGIC_NUMBER_STRING);
    for (int i = 0; i < size; i++)
    {
        if (address[i] != CPIO_MAGIC_NUMBER_STRING[i])
        {
            return 0;
        }
    }
    return 1;
}

unsigned int get_number_from_ascii(char *address)
{
    unsigned int filesize = 0;
    for (int i = 0; i < SIZE_OF_FEILD; i++)
    {
        char value;
        if (address[i] >= '0' && address[i] <= '9')
        {
            value = address[i] - '0';
        }
        else if (address[i] >= 'A' && address[i] <= 'F')
        {
            value = address[i] - 'A' + 10;
        }
        else // should not happen but for safety
        {
            value = address[i];
        }
        filesize |= value << ((7 - i) * 4);
    }

    return filesize;
}

unsigned int get_remaining_padding(unsigned int size, unsigned int paddingSize)
{
    unsigned int remainder = size % paddingSize;
    if (remainder == 0)
    {
        return 0;
    }

    return paddingSize - remainder;
}

void list_files()
{
    char *address = cpio_base;
    unsigned int fileSize = 0;
    unsigned int nameSize = 0;

    while (address = get_file_base(address, &fileSize, &nameSize))
    {
        uart_puts(address);
        uart_newline();

        address += nameSize;
        address += fileSize;
    }
}

void print_file(char *filename)
{
    if (string_compare(NOW_DIRECTORY, filename) || string_compare(PREVIOUS_DIRECTORY, filename))
    {
        uart_puts("Could not print directory!\n");

        return;
    }

    char *address = 0;
    unsigned int fileSize = 0;
    unsigned int nameSize = 0;

    address = find_file(filename, &fileSize, &nameSize);
    if (address == 0)
    {
        uart_puts("Cannot find file\n");
        uart_newline();

        return;
    }

    uart_puts("Filename: ");
    uart_puts(address);
    uart_newline();
    address += nameSize;

    uart_puts(address);
    uart_newline();
    address += fileSize;
}

char *find_file(char *filename, unsigned int *fileSize, unsigned int *nameSize)
{
    if (string_compare(NOW_DIRECTORY, filename) || string_compare(PREVIOUS_DIRECTORY, filename))
    {
        uart_puts("Could not print directory!\n");
        return 0;
    }

    char *address = cpio_base;
    unsigned int tmpNameSize = 0;
    unsigned int tmpFileSize = 0;

    while (address = get_file_base(address, &tmpFileSize, &tmpNameSize))
    {
        if (string_compare(filename, address))
        {
            *nameSize = tmpNameSize;
            *fileSize = tmpFileSize;
            return address;
        }

        address += tmpNameSize;
        address += tmpFileSize;
    }

    return 0;
}

char *get_file_base(char *address, unsigned int *fileSize, unsigned int *nameSize)
{
    if (!check_magic_number(address))
    {
        return 0;
    }

    address += SIZE_OF_MAGIC + SIZE_OF_FEILD * 6; // skip not used parts

    *fileSize = get_number_from_ascii(address);

    // padding NULL
    unsigned int remainingPadding = get_remaining_padding(*fileSize, PADDING_MULTIPLE);
    *fileSize += remainingPadding;

    address += SIZE_OF_FEILD * 5; // skip not used parts

    *nameSize = get_number_from_ascii(address);

    // padding NULL
    remainingPadding = get_remaining_padding((*nameSize + SIZE_OF_MAGIC), PADDING_MULTIPLE);
    *nameSize += remainingPadding;

    address += SIZE_OF_FEILD * 2; // skip not used parts

    if (string_compare(END_IDENTIFIER, address))
    {
        return 0;
    }

    return address;
}

void execute_file(char *filename)
{
    unsigned int fileSize = 0;
    unsigned int nameSize = 0;

    char *address = find_file(filename, &fileSize, &nameSize);

    if (address == 0)
    {
        uart_puts("Cannot find file!\n");
        uart_newline();
        return;
    }

    char *filePointer = address + nameSize;
    char *stack = malloc(STACK_SIZE);

    if (stack == 0)
    {
        uart_puts("Cannot execute file!\n");
        uart_newline();
        return;
    }

    exec_user_prog(filePointer, stack);
}