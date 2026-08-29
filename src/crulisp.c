#include "crulisp.h"
#include <stdio.h>
#include <string.h>

static void write_string(FILE *file, const char *value)
{
	fputc('"', file);

	for (const char *p = value; *p; p++) {
		switch (*p) {
			case '\\':
				fputs("\\\\", file);
				break;
			case '"':
				fputs("\\\"", file);
				break;
			case '\n':
				fputs("\\n", file);
				break;
			case '\t':
				fputs("\\t", file);
				break;
			default:
				fputc(*p, file);
		}
	}
	fputc('"', file);
}

static void generate_expression(FILE *file, AST *node)
{
	if (node->type == AST_STRING) {
		write_string(file, node->value);
		return;
	}
	if (node->type == AST_SYMBOL) {
		fprintf(file, "%s", node->value);
		return;
	}
	if (node->type == AST_LIST) {
		AST *function = node->children;
		if (!function)
			return;

		// (вывод STRING)
		if (function->type == AST_SYMBOL &&
			strcmp(function->value, "вывод") == 0) {
			AST *argument = function->next;
			fprintf(file, "printf(\"%%s\\n\", ");
			generate_expression(file, argument);
			fprintf(file, ")");
			return;
		}

		if (function->type == AST_SYMBOL) {
			AST *argument = function->next;
			if (argument) {
				generate_expression(file, argument);
				argument = argument->next;
				while (argument) {
					fprintf(file, " %s ",
							function->value);
					generate_expression(file, argument);
					argument = argument->next;
				}
			}
		}
	}
}

int crulispgen(AST *root, const char *filename)
{
	FILE *file = fopen(filename, "w");

	if (!file) {
		perror(filename);
		return 0;
	}
	fprintf(file,
			"#include <stdio.h>\n\n"
			"int main(void)\n"
			"{\n"
	       );
	AST *expression = root->children;

	while (expression) {
		fprintf(file, "    ");
		generate_expression(file, expression);
		fprintf(file, ";\n");
		expression = expression->next;
	}
	fprintf(file,
			"    return 0;\n"
			"}\n"
	       );
	fclose(file);
	return 1;
}
