#define SIZE_OF_MAGIC 6
#define SIZE_OF_FEILD 8
#define PADDING_MULTIPLE 4
#define STACK_SIZE 0x1000

#define CPIO_MAGIC_NUMBER_STRING "070701"
#define END_IDENTIFIER "TRAILER!!!"
#define NOW_DIRECTORY "."
#define PREVIOUS_DIRECTORY ".."

static char *cpio_base;

void init_cpio(char *address);
void list_files();
void print_file(char *filename);
char *get_file_base(char *address, unsigned int *fileSize, unsigned int *nameSize);
char *find_file(char *filename, unsigned int *fileSize, unsigned int *nameSize);
void execute_file(char *filename);