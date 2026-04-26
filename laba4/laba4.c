#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define RED 1
#define BLACK 0

typedef struct Node {
	char key[7];
	double value;
	int color;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
} Node;

Node *root = NULL;
Node *nil = NULL;


int is_valid_key(const char *key) {
	int len = strlen(key);
	if (len < 1 || len > 6) {
		return 0;
	}

	for (int i = 0; i < len; i++) {
		if (!((key[i] >= 'A' && key[i] <= 'Z') || (key[i] >= 'a' && key[i] <= 'z'))) {
			return 0;
		}
	}
	return 1;
}

Node* create_node(const char *key, double value) {
	Node *new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) {
		printf("Ошибка памяти\n");
		exit(1);
	}

	strcpy(new_node->key, key);
	new_node->value = value;
	new_node->color = RED;
	new_node->left = nil;
	new_node->right = nil;
	new_node->parent = nil;

	return new_node;
}

void init_tree() {
	nil = (Node*)malloc(sizeof(Node));
	if (nil == NULL) {
		printf("Ошибка памяти\n");
		exit(1);
	}

	strcpy(nil->key, "");
	nil->value = 0.0;
	nil->color = BLACK;
	nil->left = nil;
	nil->right = nil;
	nil->parent = nil;

	root = nil;
}

Node* find_node(const char *key) {
	Node *now = root;

	while (now != nil) {
		int cmp = strcmp(key, now->key);
		if (cmp == 0) {
			return now;
		}
		else if (cmp < 0) {
			now = now->left;
		}
		else {
			now = now->right;
		}
	}

	return nil;
}

Node* tree_min(Node *now) {
	while (now->left != nil) {
		now = now->left;
	}
	return now;
}

void replace_node(Node *old_node, Node *new_node) {
	if (old_node->parent == nil) {
		root = new_node;
	} else if (old_node == old_node->parent->left) {
		old_node->parent->left = new_node;
	} else {
		old_node->parent->right = new_node;
	}

	new_node->parent = old_node->parent;
}


void left_rotate(Node *x) {
	Node *y = x->right;

	x->right = y->left;
	if (y->left != nil) {
		y->left->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == nil) {
		root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

void right_rotate(Node *x) {
	Node *y = x->left;

	x->left = y->right;
	if (y->right != nil) {
		y->right->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == nil) {
		root = y;
	} 
	else if (x == x->parent->right) {
		x->parent->right = y;
	} 
	else {
		x->parent->left = y;
	}

	y->right = x;
	x->parent = y;
}


void fix_insert(Node *node) {
	while (node->parent->color == RED) {
		if (node->parent == node->parent->parent->left) {
			Node *uncle = node->parent->parent->right;

			if (uncle->color == RED) {
				node->parent->color = BLACK;
				uncle->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			} 
			else {
				if (node == node->parent->right) {
					node = node->parent;
					left_rotate(node);
				}

				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				right_rotate(node->parent->parent);
			}
			} 
			else {
				Node *uncle = node->parent->parent->left;

				if (uncle->color == RED) {
					node->parent->color = BLACK;
					uncle->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
					} 
					else {
						if (node == node->parent->left) {
							node = node->parent;
							right_rotate(node);
						}

						node->parent->color = BLACK;
						node->parent->parent->color = RED;
						left_rotate(node->parent->parent);
				}
		}
	}

	root->color = BLACK;
}

int insert_node(const char *key, double value) {
	if (!is_valid_key(key)) {
			return 0;
	}

	if (find_node(key) != nil) {
			return -1;
	}

	Node *new_node = create_node(key, value);
	Node *parent = nil;
	Node *now = root;

	while (now != nil) {
			parent = now;
			if (strcmp(new_node->key, now->key) < 0) {
					now = now->left;
			} else {
					now = now->right;
			}
	}

	new_node->parent = parent;

	if (parent == nil) {
			root = new_node;
	} else if (strcmp(new_node->key, parent->key) < 0) {
			parent->left = new_node;
	} else {
			parent->right = new_node;
	}

	if (new_node->parent == nil) {
			new_node->color = BLACK;
			return 1;
	}

	if (new_node->parent->parent == nil) {
			return 1;
	}

	fix_insert(new_node);
	return 1;
}


void fix_delete(Node *node) {
	while (node != root && node->color == BLACK) {
		if (node == node->parent->left) {
				Node *brother = node->parent->right;

				if (brother->color == RED) {
					brother->color = BLACK;
					node->parent->color = RED;
					left_rotate(node->parent);
					brother = node->parent->right;
				}

				if (brother->left->color == BLACK && brother->right->color == BLACK) {
					brother->color = RED;
					node = node->parent;
					}
					else {
						if (brother->right->color == BLACK) {
							brother->left->color = BLACK;
							brother->color = RED;
							right_rotate(brother);
							brother = node->parent->right;
						}

						brother->color = node->parent->color;
						node->parent->color = BLACK;
						brother->right->color = BLACK;
						left_rotate(node->parent);
						node = root;
					}
			} 
			else {
				Node *brother = node->parent->left;

				if (brother->color == RED) {
					brother->color = BLACK;
					node->parent->color = RED;
					right_rotate(node->parent);
					brother = node->parent->left;
				}

				if (brother->right->color == BLACK && brother->left->color == BLACK) {
					brother->color = RED;
					node = node->parent;
					} 
					else {
						if (brother->left->color == BLACK) {
							brother->right->color = BLACK;
							brother->color = RED;
							left_rotate(brother);
							brother = node->parent->left;
						}

					brother->color = node->parent->color;
					node->parent->color = BLACK;
					brother->left->color = BLACK;
					right_rotate(node->parent);
					node = root;
			}
		}
	}

	node->color = BLACK;
}

int delete_node(const char *key) {
	Node *del = find_node(key);
	if (del == nil) {
		return 0;
	}

	Node *y = del;
	Node *x;
	int old_color = y->color;

	if (del->left == nil) {
		x = del->right;
		replace_node(del, del->right);
	} else if (del->right == nil) {
		x = del->left;
		replace_node(del, del->left);
	} else {
		y = tree_min(del->right);
		old_color = y->color;
		x = y->right;

		if (y->parent == del) {
			x->parent = y;
			} 
			else {
				replace_node(y, y->right);
				y->right = del->right;
				y->right->parent = y;
		}

		replace_node(del, y);
		y->left = del->left;
		y->left->parent = y;
		y->color = del->color;
	}

	free(del);

	if (old_color == BLACK) {
			fix_delete(x);
	}

	return 1;
}


void print_tree_side(Node *node, int level, FILE *out) {
	if (node == nil) {
		return;
	}

	print_tree_side(node->right, level + 1, out);

	for (int i = 0; i < level; i++) {
		fprintf(out, "    ");
	}

	fprintf(out, "%s %.6lf (%c)\n", node->key, node->value, node->color == RED ? 'R' : 'B');

	print_tree_side(node->left, level + 1, out);
}

void print_tree(FILE *out) {
	if (root == nil) {
		fprintf(out, "Дерево пустое\n");
		return;
	}

	print_tree_side(root, 0, out);
}


void free_tree(Node *node) {
	if (node == nil) {
		return;
	}

	free_tree(node->left);
	free_tree(node->right);
	free(node);
}


void run_commands(const char *input_name, const char *output_name) {
	FILE *in = fopen(input_name, "r");
	FILE *out = fopen(output_name, "w");

	if (in == NULL) {
		printf("Не удалось открыть входной файл %s\n", input_name);
		return;
	}

	if (out == NULL) {
		printf("Не удалось открыть выходной файл %s\n", output_name);
		fclose(in);
		return;
	}

	char line[256];

	while (fgets(line, sizeof(line), in) != NULL) {
		char save_line[256];
		strcpy(save_line, line);

		size_t len = strlen(save_line);
		if (len > 0 && save_line[len - 1] == '\n') {
			save_line[len - 1] = '\0';
		}

		fprintf(out, "%s\n", save_line);

		int cmd;
		char key[64];
		double value;

		int count = sscanf(line, "%d %63s %lf", &cmd, key, &value);

		if (count < 1) {
			fprintf(out, "Ошибка: пустая или неверная команда\n\n");
			continue;
		}

		if (cmd == 1) {
			if (count != 3) {
				fprintf(out, "Ошибка: для добавления нужны ключ и значение\n\n");
				continue;
			}

				if (!is_valid_key(key)) {
					fprintf(out, "Ошибка: ключ должен содержать только латинские буквы и иметь длину 1..6\n\n");
					continue;
				}

				int res = insert_node(key, value);
				if (res == 1) {
						fprintf(out, "OK: узел добавлен\n\n");
				} 
				else if (res == -1) {
						fprintf(out, "Ошибка: такой ключ уже есть\n\n");
					} 
				else {
						fprintf(out, "Ошибка: неверный ключ\n\n");
				}

			} 
			else if (cmd == 2) {
				if (count < 2) {
					fprintf(out, "Ошибка: для удаления нужен ключ\n\n");
						continue;
				}

				if (!is_valid_key(key)) {
					fprintf(out, "Ошибка: неверный ключ\n\n");
					continue;
				}

				int res = delete_node(key);
				if (res) {
					fprintf(out, "OK: узел удален\n\n");
				} 
				else {
					fprintf(out, "Ошибка: ключ не найден\n\n");
				}

			} 
			else if (cmd == 3) {
				print_tree(out);
				fprintf(out, "\n");

			} 
			else if (cmd == 4) {
				if (count < 2) {
					fprintf(out, "Ошибка: для поиска нужен ключ\n\n");
					continue;
				}

				if (!is_valid_key(key)) {
					fprintf(out, "Ошибка: неверный ключ\n\n");
					continue;
				}

				Node *found = find_node(key);
				if (found != nil) {
					fprintf(out, "FOUND: %.6lf\n\n", found->value);
				} 
				else {
					fprintf(out, "NOT FOUND\n\n");
				}

			} 
			else {
				fprintf(out, "Ошибка: неизвестная операция\n\n");
			}
	}

	fclose(in);
	fclose(out);
}


#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
	setlocale(LC_ALL, "");

	setlocale(LC_NUMERIC, "C");

#ifdef _WIN32
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
#endif

	init_tree();

	printf("Красно-черное дерево\n");
	printf("Файл команд читается из input.txt\n");
	printf("Результат записывается в output.txt\n");

	run_commands("input.txt", "output.txt");

	free_tree(root);
	free(nil);

	printf("Готово. Проверь файл output.txt\n");
	return 0;
}