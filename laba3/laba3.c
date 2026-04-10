#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

typedef struct Node {
	char type;
	char val[32];
	char op;
	struct Node *left;
	struct Node *right;
} Node;

char s[256];
int pos = 0;

Node* new_num(const char *v) {
	Node *t = (Node*)malloc(sizeof(Node));
	t->type = 'n';
	strcpy(t->val, v);
	t->op = 0;
	t->left = NULL;
	t->right = NULL;
	return t;
}

Node* new_op(char op, Node *l, Node *r) {
	Node *t = (Node*)malloc(sizeof(Node));
	t->type = 'o';
	t->val[0] = '\0';
	t->op = op;
	t->left = l;
	t->right = r;
	return t;
}

void skip_sp() {
	while (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n') pos++;
}

Node* copy_tree(Node *t) {
	if (!t) return NULL;
	if (t->type == 'n') return new_num(t->val);
	return new_op(t->op, copy_tree(t->left), copy_tree(t->right));
}

int same(Node *a, Node *b) {
	if (!a || !b) return 0;
	if (a->type != b->type) return 0;

	if (a->type == 'n') {
		return strcmp(a->val, b->val) == 0;
	}

	if (a->op != b->op) return 0;
	return same(a->left, b->left) && same(a->right, b->right);
}

Node* parse_expr();

Node* parse_atom() {
	skip_sp();

	if (s[pos] == '(') {
		pos++;
		Node *t = parse_expr();
		skip_sp();
		if (s[pos] == ')') pos++;
		return t;
	}

	if (isalnum((unsigned char)s[pos])) {
		char buf[32];
		int i = 0;
		while (isalnum((unsigned char)s[pos])) {
			buf[i++] = s[pos++];
		}
		buf[i] = '\0';
		return new_num(buf);
	}

	return NULL;
}

Node* parse_mul() {
	Node *left = parse_atom();
	skip_sp();

	while (s[pos] == '*') {
		pos++;
		Node *right = parse_atom();
		left = new_op('*', left, right);
		skip_sp();
	}

	return left;
}

Node* parse_expr() {
	Node *left = parse_mul();
	skip_sp();

	while (s[pos] == '+') {
		pos++;
		Node *right = parse_mul();
		left = new_op('+', left, right);
		skip_sp();
	}

	return left;
}

void print_expr(Node *t) {
	if (!t) return;

	if (t->type == 'n') {
		printf("%s", t->val);
		return;
	}

	printf("(");
	print_expr(t->left);
	printf(" %c ", t->op);
	print_expr(t->right);
	printf(")");
}

void print_tree(Node *t, int level) {
	if (!t) return;

	for (int i = 0; i < level; i++) printf("   ");

	if (t->type == 'n') printf("%s\n", t->val);
	else printf("%c\n", t->op);

	print_tree(t->left, level + 1);
	print_tree(t->right, level + 1);
}

void split_mul(Node *t, Node **a, Node **b) {
	if (t && t->type == 'o' && t->op == '*') {
		*a = t->left;
		*b = t->right;
	} else {
		*a = t;
		*b = NULL;
	}
}

Node* factor_sum(Node *t, int *changed) {
	if (!t) return NULL;

	if (t->type == 'o') {
		t->left = factor_sum(t->left, changed);
		t->right = factor_sum(t->right, changed);
	}

	if (t->type == 'o' && t->op == '+') {
		Node *l1, *l2, *r1, *r2;
		split_mul(t->left, &l1, &l2);
		split_mul(t->right, &r1, &r2);

		if (l2 && r2) {
				if (same(l1, r1)) {
					*changed = 1;
					return new_op('*', copy_tree(l1), new_op('+', copy_tree(l2), copy_tree(r2)));
				}
				if (same(l1, r2)) {
					*changed = 1;
					return new_op('*', copy_tree(l1), new_op('+', copy_tree(l2), copy_tree(r1)));
				}
				if (same(l2, r1)) {
					*changed = 1;
					return new_op('*', copy_tree(l2), new_op('+', copy_tree(l1), copy_tree(r2)));
				}
				if (same(l2, r2)) {
					*changed = 1;
					return new_op('*', copy_tree(l2), new_op('+', copy_tree(l1), copy_tree(r1)));
				}
		}
	}

	return t;
}

Node* full_factor(Node *t) {
	int changed;
	do {
		changed = 0;
		t = factor_sum(t, &changed);
	} while (changed);
	return t;
}

void free_tree(Node *t) {
	if (!t) return;
	free_tree(t->left);
	free_tree(t->right);
	free(t);
}

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");
	printf("Введите выражение:\n");
	fgets(s, sizeof(s), stdin);

	pos = 0;
	Node *root = parse_expr();

	printf("\nИсходное выражение:\n");
	print_expr(root);
	printf("\n");

	printf("\nИсходное дерево:\n");
	print_tree(root, 0);

	root = full_factor(root);

	printf("\nПосле преобразования:\n");
	print_expr(root);
	printf("\n");

	printf("\nДерево после преобразования:\n");
	print_tree(root, 0);

	free_tree(root);
	return 0;
}