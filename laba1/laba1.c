#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Структура узла бинарного дерева
typedef struct node {
	int data;
	struct node* left;
	struct node* right;
} node;

// Инициализация нового узла с заданным значением
node* init(int data) {
	node* new_node = malloc(sizeof(node));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}

// Добавление узла в бинарное дерево
node* add_node(int data, node* root) {
	if (root == NULL) {
		node* new_node = init(data);
		if (new_node == NULL) {
			return NULL;
		}
		return new_node;
	}
	if (data > root->data) {
		root->right = add_node(data, root->right);
	}
	else if (data < root->data) {
		root->left = add_node(data, root->left);
	}
	return root;
}

// Вывод дерева в консоль (визуализация)
int draw_tree(node* root, int level) {
	if (root == NULL) {
		return 1;
	}
	printf("%*s", level * 4, "");
	printf("%d\n", root->data);
	draw_tree(root->left, level + 1);
	draw_tree(root->right, level + 1);
	return 1;
}

// Подсчёт количества узлов в дереве
int count_nodes(node* root) {
	if (root == NULL) {
		return 0;
	}
	return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// Очистка дерева (освобождение памяти)
void clear_tree(node* root) {
	if (root == NULL) {
		return;
	}
	clear_tree(root->left);
	clear_tree(root->right);
	free(root);
}

// Удаление узла из дерева
node* remove_node(node* root, int data) {
	if (root == NULL) {
		return NULL;
	}
	if (data < root->data) {
		root->left = remove_node(root->left, data);
	}
	else if (data > root->data) {
		root->right = remove_node(root->right, data);
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			free(root);
			return NULL;
		}
		else if (root->left != NULL && root->right == NULL) {
			node* temp = root->left;
			free(root);
			return temp;
		}
		else if (root->left == NULL && root->right != NULL) {
			node* temp = root->right;
			free(root);
			return temp;
		}
		else {
			node* min = root->right;
			while(min->left != NULL) {
				min = min->left;
			}
			root->data = min->data;
			root->right = remove_node(root->right, min->data);
		}
	}
	return root;
}

// Вспомогательная функция для проверки листьев на одном уровне
void check_leafs_on_same_level(node* root, int level, int* first_leaf_level, int* result) {
	if (root == NULL || *result == 0) {
		return;
	}
	if (root->left == NULL && root->right == NULL) {
		if (*first_leaf_level == -1) {
			*first_leaf_level = level;
		}
		else {
			if (level != *first_leaf_level) {
				*result = 0;
			}
		}
	}
	else {
		check_leafs_on_same_level(root->left, level + 1, first_leaf_level, result);
		check_leafs_on_same_level(root->right, level + 1, first_leaf_level, result);
	}
}

// Проверка, все ли листья находятся на одном уровне
int leafs_on_same_level(node* root) {
	if (root == NULL) {
		return 1;
	}
	int first_leaf_level = -1;
	int result = 1;
	check_leafs_on_same_level(root, 0, &first_leaf_level, &result);
	return result;
}

int main(void) {
	node* root = NULL;
	int operation = 0;
	int current_data = 0;

	while (1) {
		printf("Выберите операцию:\n");
		printf("1. Вывести дерево\n");
		printf("2. Добавить узел\n");
		printf("3. Удалить узел\n");
		printf("4. Подсчитать количество узлов\n");
		printf("5. Проверить, все ли листья на одном уровне (вариант 11)\n");
		printf("6. Выход из программы\n");

		if (scanf("%d", &operation) != 1) {
			printf("Неверный ввод\n");
			return 1;
		}

		if (operation == 1) {
			if (root == NULL) {
				printf("Дерево пусто\n");
			}
			else {
				draw_tree(root, 0);
			}

		}
		else if (operation == 2) {
			printf("Введите значение для нового узла:\n");
			if (scanf("%d", &current_data) != 1) {
				printf("Неверный ввод\n");
				continue;
			}

			node* new_root = add_node(current_data, root);
			if (new_root == NULL && root == NULL) {
				printf("Ошибка выделения памяти\n");
				}
				else {
					root = new_root;
				}

		} else if (operation == 3) {
			printf("Введите значение узла для удаления:\n");
			if (scanf("%d", &current_data) != 1) {
				printf("Неверный ввод\n");
				continue;
			}
			root = remove_node(root, current_data);

		}
		else if (operation == 4) {
			printf("Количество узлов: %d\n", count_nodes(root));

		}
		else if (operation == 5) {
			int ok = leafs_on_same_level(root);
			if (ok) {
				printf("Все листья дерева находятся на одном уровне\n");
			} else {
				printf("Листья дерева находятся на разных уровнях\n");
			}

			}
			else if (operation == 6) {
				clear_tree(root);
				root = NULL;
				break;

			}
			else {
				printf("Неверная операция\n");
			}
	}

	if (root != NULL) {
		clear_tree(root);
		root = NULL;
	}

	return 0;
}