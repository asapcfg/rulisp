#include "police.h"
#include <stdio.h>
#include <string.h>
static int check_expression(AST *node);

static int check_list(AST *list)
{
	if (!list->children) {
		fprintf(stderr, "пустой вызов\n");
		return 0;
	}

	AST *first = list->children;

	if (first->type != ast_symbol) {
		fprintf(
			stderr,
			"первым элементом вызова должен быть символ\n"
		);
		return 0;
	}

	AST *child = first->next;
	while (child) {
		if (!check_expression(child))
			return 0;

		child = child->next;
	}

	if (strcmp(first->value, "фун") == 0) {
		AST *signature = first->next;
		if (!signature ||
				signature->type != ast_list) {
			fprintf(
				stderr,
				"у функции должно быть название функции\n"
			);
			return 0;
					);
		}
		if (!signature->children ||
				signature->children->type != ast_symbol) {
			fprintf(
				stderr,
				"неправильное название функции\n"
					);
			return 0;
		}
	}
	return 1;
}

static int check_expression(AST *node)
{
		switch (node->type) {
			case ast_string:
				return 1;
			case ast_symbol:
				return 1;
			case ast_list:
				return check_list(node);
			default:
				return 0;

		}

}
int policechk(AST *root)
{
	AST *expression = root->children;
	while (expression) {
		if (!check_expression(expression))
			return 0;
		expression = expression->next;
	}
	return 1;
}

