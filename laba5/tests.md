## Тест 1 — пример из лекции

Ввод:

```text
9
3 8
3 10
5 11
7 11
7 8
8 9
11 2
11 9
11 10
```

Вывод (один из возможных):

```text
Topological sort (Kahn's algorithm)
Graph representation: edge list
Enter number of edges: 9
Enter edges in format: from to
Edge 1: 3 8
Edge 2: 3 10
Edge 3: 5 11
Edge 4: 7 11
Edge 5: 7 8
Edge 6: 8 9
Edge 7: 11 2
Edge 8: 11 9
Edge 9: 11 10

Graph edge list:
3 -> 8
3 -> 10
5 -> 11
7 -> 11
7 -> 8
8 -> 9
11 -> 2
11 -> 9
11 -> 10

Graph vertices:
3 8 10 5 11 7 9 2

Topological order:
3 5 7 11 8 2 10 9
```

---

## Тест 2 — простой DAG («ромб»)

Ввод:

```text
4
1 2
1 3
2 4
3 4
```

Возможный вывод:

```text
Topological sort (Kahn's algorithm)
Graph representation: edge list
Enter number of edges: 4
Enter edges in format: from to
Edge 1: 1 2
Edge 2: 1 3
Edge 3: 2 4
Edge 4: 3 4

Graph edge list:
1 -> 2
1 -> 3
2 -> 4
3 -> 4

Graph vertices:
1 2 3 4

Topological order:
1 2 3 4
```

Другой корректный порядок:

```text
1 3 2 4
```

---

## Тест 3 — граф с циклом

Ввод:

```text
3
1 2
2 3
3 1
```

Вывод:

```text
Topological sort (Kahn's algorithm)
Graph representation: edge list
Enter number of edges: 3
Enter edges in format: from to
Edge 1: 1 2
Edge 2: 2 3
Edge 3: 3 1

Graph edge list:
1 -> 2
2 -> 3
3 -> 1

Graph vertices:
1 2 3

Topological sorting is impossible: the graph contains a cycle.
```

---

## Тест 4 — простая цепочка

Ввод:

```text
4
10 20
20 30
30 40
40 50
```

Вывод:

```text
Topological sort (Kahn's algorithm)
Graph representation: edge list
Enter number of edges: 4
Enter edges in format: from to
Edge 1: 10 20
Edge 2: 20 30
Edge 3: 30 40
Edge 4: 40 50

Graph edge list:
10 -> 20
20 -> 30
30 -> 40
40 -> 50

Graph vertices:
10 20 30 40 50

Topological order:
10 20 30 40 50
```