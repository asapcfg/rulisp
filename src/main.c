#include <stdio.h>
#include <string.h>

static void version(void)
{
	printf("РУЛисп, версия 0.1.0\n");
}

static void usage(const char *program)
{
	printf("Как использовать:\n");
	printf("  %s -v		Показать версию\n", program);
	printf("  %s -c <файл>	Скомпилировать файл РУЛисп\n", program);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}
	if (strcmp(argv[1], "-v") == 0) {
		version();
		return 0;
	}
	if (strcmp(argv[1], "-c") == 0) {
		if (argc < 3) {
			fprintf(stderr, "rlp: не указан файл\n");
			return 2;
		}
		printf("файл: %s\n", argv[2]);
		return 0;
	}
	fprintf(stderr, "rlp: что такое '%s'?\n", argv[1]);
	return 1;
}
