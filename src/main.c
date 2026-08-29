#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "police.h"
#include "crulisp.h"

static void print_version(void)
{
    printf(
        "RULisp version 0.1.0\n"
    );
}

static char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (!file) {
        perror(filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    long size = ftell(file);

    rewind(file);

    if (size < 0) {
        fclose(file);
        return NULL;
    }

    char *buffer = malloc((size_t)size + 1);

    if (!buffer) {
        fclose(file);
        return NULL;
    }

    size_t read = fread(
        buffer,
        1,
        (size_t)size,
        file
    );

    buffer[read] = '\0';

    fclose(file);

    return buffer;
}

static int compile_file(const char *filename)
{
    char *source = read_file(filename);

    if (!source)
        return 1;

    Parser parser;

    parser_init(&parser, source);

    AST *ast = parser_parse(&parser);

    tokenfree(&parser.current);

    if (!ast) {
        free(source);
        return 1;
    }

    printf("проверка\n");

    if (!policechk(ast)) {
        fprintf(stderr, "ошибка (rlpol-err)\n");

        ast_free(ast);
        free(source);

        return 1;
    }

    if (!crulispgen(
        ast,
        "output.c"
    )) {
        ast_free(ast);
        free(source);

        return 1;
    }

    printf(
        "Crulisp: сгенерирован output.c\n"
    );

    ast_free(ast);
    free(source);

    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Как использовать:\n");
        printf("  rlp -v\n");
        printf("  rlp -i <file.rlp>\n");
        return 1;
    }

    if (strcmp(argv[1], "-v") == 0) {
        print_version();
        return 0;
    }

    if (strcmp(argv[1], "-i") == 0) {

        if (argc < 3) {
            fprintf(
                stderr,
                "rlp: не указан файл\n"
            );

            return 1;
        }

        return compile_file(argv[2]);
    }

    fprintf(
        stderr,
        "rlp: неизвестный аргумент '%s'\n",
        argv[1]
    );

    return 1;
}
