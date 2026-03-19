#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
	int data;
	struct node* left;
	struct node* right;
} node;

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

int add_node(int data, node** root) {
	if (*root == NULL) {
		node* new_node = init(data);
		if (new_node == NULL) {
			return 0;
		}
		*root = new_node;
		return 1;
	}
	if (data > (*root)->data) {
		return add_node(data, &((*root)->right));
	}
	else if (data == (*root)->data) {
		return 2; // 2 знаит что узел с таким значением уже есть
	}
	else {
		return add_node(data, &((*root)->left));
	}
}

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

int count_nodes(node* root) {
	if (root == NULL) {
		return 0;
	}
	return 1 + count_nodes(root->left) + count_nodes(root->right);
}

void clear_tree(node** root) {
	if (*root == NULL) {
		return;
	}
	clear_tree(&((*root)->left));
	clear_tree(&((*root)->right));
	free(*root);
	*root = NULL;
}

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

