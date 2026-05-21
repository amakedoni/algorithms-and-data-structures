#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define MAX_EDGES 1000

typedef struct {
	int from;
	int to;
} Edge;

int find_vertex_index(int vertices[], int vertex_count, int value) {
	for (int i = 0; i < vertex_count; i++) {
		if (vertices[i] == value) {
			return i;
		}
	}
	return -1;
}

void add_vertex_if_needed(int vertices[], int *vertex_count, int value) {
	if (find_vertex_index(vertices, *vertex_count, value) == -1) {
		vertices[*vertex_count] = value;
		(*vertex_count)++;
	}
}

void print_edge_list(Edge edges[], int edge_count) {
	printf("\nGraph edge list:\n");
	for (int i = 0; i < edge_count; i++) {
		printf("%d -> %d\n", edges[i].from, edges[i].to);
	}
}

int kahn_topological_sort(Edge edges[], int edge_count, int vertices[], int vertex_count, int result[]) {
	int in_degree[MAX_VERTICES] = {0};
	int queue[MAX_VERTICES];
	int queue_start = 0;
	int queue_end = 0;
	int result_count = 0;

	for (int i = 0; i < edge_count; i++) {
		int to_index = find_vertex_index(vertices, vertex_count, edges[i].to);
		if (to_index != -1) {
			in_degree[to_index]++;
		}
	}

	for (int i = 0; i < vertex_count; i++) {
		if (in_degree[i] == 0) {
			queue[queue_end++] = i;
		}
	}

	while (queue_start < queue_end) {
		int current_vertex_index = queue[queue_start++];
		int current_vertex = vertices[current_vertex_index];

		result[result_count++] = current_vertex;

		for (int i = 0; i < edge_count; i++) {
			if (edges[i].from == current_vertex) {
				int neighbor_index = find_vertex_index(vertices, vertex_count, edges[i].to);
				if (neighbor_index != -1) {
					in_degree[neighbor_index]--;

					if (in_degree[neighbor_index] == 0) {
							queue[queue_end++] = neighbor_index;
					}
				}
			}
		}
	}

	if (result_count != vertex_count) {
		return 0;
	}

	return 1;
}

int main() {
	int edge_count;
	Edge edges[MAX_EDGES];
	int vertices[MAX_VERTICES];
	int vertex_count = 0;
	int result[MAX_VERTICES];

	printf("Topological sort (Kahn's algorithm)\n");
	printf("Graph representation: edge list\n");
	printf("Enter number of edges: ");
	scanf("%d", &edge_count);

	if (edge_count <= 0 || edge_count > MAX_EDGES) {
		printf("Error: invalid number of edges.\n");
		return 1;
	}

	printf("Enter edges in format: from to\n");
	for (int i = 0; i < edge_count; i++) {
		printf("Edge %d: ", i + 1);
		scanf("%d %d", &edges[i].from, &edges[i].to);

		add_vertex_if_needed(vertices, &vertex_count, edges[i].from);
		add_vertex_if_needed(vertices, &vertex_count, edges[i].to);
	}

	print_edge_list(edges, edge_count);

	printf("\nGraph vertices:\n");
	for (int i = 0; i < vertex_count; i++) {
		printf("%d ", vertices[i]);
	}
	printf("\n");

	if (kahn_topological_sort(edges, edge_count, vertices, vertex_count, result)) {
		printf("\nTopological order:\n");
		for (int i = 0; i < vertex_count; i++) {
			printf("%d ", result[i]);
		}
		printf("\n");
	}
	else {
		printf("\nTopological sorting is impossible: the graph contains a cycle.\n");
	}

	return 0;
}