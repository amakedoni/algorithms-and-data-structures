#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_LEN 11

typedef struct {
	int key;
	int data;
} TableItem;

void print_table(const TableItem *t, int n) {
	if (t == NULL || n <= 0) {
		printf("Таблица пустая.\n");
		return;
	}

	for (int i = 0; i < n; i++) {
		printf("%2d: key = %4d  data = %4d\n", i + 1, t[i].key, t[i].data);
	}
}

TableItem *copy_table(const TableItem *t, int n) {
	if (t == NULL || n <= 0) {
		return NULL;
	}

	TableItem *new_table = malloc(n * sizeof(TableItem));
	if (new_table == NULL) {
		fprintf(stderr, "Не удалось выделить память под копию таблицы.\n");
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		new_table[i] = t[i];
	}

	return new_table;
}

void swap_items(TableItem *a, TableItem *b) {
	TableItem tmp = *a;
	*a = *b;
	*b = tmp;
}

void bubble_sort(TableItem *t, int n) {
	if (t == NULL || n <= 0) {
		return;
	}

	for (int i = 0; i < n - 1; i++) {
		int swapped = 0;
		for (int j = 0; j < n - 1 - i; j++) {
			if (t[j].key > t[j + 1].key) {
				swap_items(&t[j], &t[j + 1]);
				swapped = 1;
			}
		}
		if (!swapped) {
			break;
		}
	}
}

void reverse_table(TableItem *t, int n) {
	if (t == NULL || n <= 0) {
		return;
	}

	int left = 0;
	int right = n - 1;

	while (left < right) {
		swap_items(&t[left], &t[right]);
		left++;
		right--;
	}
}

void shuffle_table(TableItem *t, int n) {
	if (t == NULL || n <= 0) {
		return;
	}

	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		swap_items(&t[i], &t[j]);
	}
}

int bin_search(const TableItem *t, int n, int key) {
	if (t == NULL || n <= 0) {
		return -1;
	}

	int left = 0;
	int right = n - 1;

	while (left <= right) {
		int mid = left + (right - left) / 2;

		if (key == t[mid].key) {
			return mid;
		}
		else if (key < t[mid].key) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}

	return -1;
}

void demo_case(const char *title, TableItem *t, int n) {
	printf("\n%s\n", title);
	printf("Исходная таблица:\n");
	print_table(t, n);

	clock_t start = clock();
	bubble_sort(t, n);
	clock_t end = clock();

	double seconds = (double)(end - start) / CLOCKS_PER_SEC;

	printf("\nПосле сортировки:\n");
	print_table(t, n);
	printf("Время сортировки: %.6f секунд\n", seconds);
}

void fill_sorted_table(TableItem *t, int n) {
	for (int i = 0; i < n; i++) {
			t[i].key = i + 1;
			t[i].data = (i + 1) * 10;
	}
}

int main(void) {
	srand((unsigned)time(NULL));

	int n = MIN_LEN;
	TableItem base[MIN_LEN];

	fill_sorted_table(base, n);

	TableItem *sorted_case = copy_table(base, n);
	TableItem *reverse_case = copy_table(base, n);
	TableItem *random_case = copy_table(base, n);

	if (sorted_case == NULL || reverse_case == NULL || random_case == NULL) {
			free(sorted_case);
			free(reverse_case);
			free(random_case);
			return 1;
	}

	reverse_table(reverse_case, n);
	shuffle_table(random_case, n);

	demo_case("СЛУЧАЙ 1: таблица уже упорядочена", sorted_case, n);
	demo_case("СЛУЧАЙ 2: таблица в обратном порядке", reverse_case, n);
	demo_case("СЛУЧАЙ 3: таблица не упорядочена", random_case, n);

	printf("\nДвоичный поиск в отсортированной таблице (случай 3 после сортировки).\n");
	printf("Введите ключ для поиска (-1 для выхода):\n");

	while (1) {
			int key;
			printf("key = ");
			if (scanf("%d", &key) != 1) {
					fprintf(stderr, "Ошибка ввода.\n");
					break;
			}

			if (key == -1) {
					break;
			}

			int pos = bin_search(random_case, n, key);
			if (pos >= 0) {
					printf("Найден элемент: номер = %d, key = %d, data = %d\n", pos + 1, random_case[pos].key, random_case[pos].data);
			} else {
					printf("Элемент с ключом %d не найден.\n", key);
			}
	}

	free(sorted_case);
	free(reverse_case);
	free(random_case);

	return 0;
}