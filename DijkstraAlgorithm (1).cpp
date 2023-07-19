#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <iostream>
#define ARRAY_SIZE 8


struct Road
{
	int *road;
	int len;
};

int **Inicialize(void)
{
	int tmp[ARRAY_SIZE][ARRAY_SIZE] = {
		{-1, 5, -1, -1, -1, -1, -1, 10},
		{5, -1, 45, -1, -1, -1, -1, 25},
		{-1, 45, -1, 53, -1, 50, -1, -1},
		{-1, -1, 53, -1, 26, -1, -1, -1},
		{-1, -1, -1, 26, -1, 61, -1, -1},
		{-1, -1, 50, -1, 61, -1, 73, -1},
		{-1, -1, -1, -1, -1, 73, -1, 42},
		{10, 25, -1, -1, -1, -1, 42, -1}
	};

	int **tmp_matrix = new int *[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		tmp_matrix[i] = new int[ARRAY_SIZE];
		for (int k = 0; k < ARRAY_SIZE; k++) tmp_matrix[i][k] = tmp[i][k];
	}
	return tmp_matrix;
}

void Dijkstra(int **matrix, int **new_matrix, int v)
{
	bool a[ARRAY_SIZE];
	int b[ARRAY_SIZE];
	int p[ARRAY_SIZE];

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		a[i] = false;
		b[i] = MAXINT;
		p[i] = -1;
	}

	b[v] = 0;

	for (int counter = 0; counter < ARRAY_SIZE; counter++)
	{
		int min = MAXINT, tmp_index = -1;
		for (int i = 0; i < ARRAY_SIZE; i++)
			if ((b[i] <= min) && (a[i] == false))
			{
				min = b[i];
				tmp_index = i;
			}
		a[tmp_index] = true;

		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			if ((matrix[tmp_index][i] != -1) && (a[i] != true))
				if (matrix[tmp_index][i] + b[tmp_index] < b[i])
				{
					b[i] = matrix[tmp_index][i] + b[tmp_index];
					p[i] = tmp_index;
				}
		}

	}
	printf("--------------------------------\n");
	for (int i = 0; i < ARRAY_SIZE; i++) printf("Город: %3i| Направление: %3i| Предыдущий: %3i|\n", i + 1, b[i], p[i] + 1);
	printf("--------------------------------\n\n");

	for (int i = 0; i < ARRAY_SIZE; i++)
		if (i != v)
		{
			new_matrix[v][i] = b[i];
			new_matrix[i][v] = b[i];
		}
}

int WayLength(int **matrix, Road *way)
{
	int sum = 0;

	for (int j = 0; j < ARRAY_SIZE - 1; j++)
	{
		int a = matrix[way->road[j]][way->road[j + 1]];
		if (a < 0)
		{
			sum = MAXINT;
			break;
		}
		else sum += a;
	}

	return sum;
}

Road *ShortestWay(int **matrix, int index)
{
	Road *res = new Road;
	res->road = new int[ARRAY_SIZE];
	res->len = 0;

	bool a[ARRAY_SIZE] = { false };

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		int min = MAXINT, tmp_index = -1;
		a[index] = true;
		for (int j = 0; j < ARRAY_SIZE; j++)
			if ((min > matrix[index][j]) && (matrix[index][j] >= 0) && (a[j] != true))
			{
				min = matrix[index][j];
				tmp_index = j;
			}
		res->road[i] = index;
		index = tmp_index;
	}

	return res;
}

void PrintTour(int **matrix, int **new_matrix, Road *way)
{
	for (int j = 0; j < ARRAY_SIZE - 1; j++)
		if (matrix[way->road[j]][way->road[j + 1]] > 0) printf("%i(%2i)->", way->road[j] + 1, matrix[way->road[j]][way->road[j + 1]]);
		else printf("%i(--)->", way->road[j] + 1);

	if (way->len != MAXINT) printf("%i | %i\n", way->road[ARRAY_SIZE - 1] + 1, way->len);
	else printf("%i | Такого пути не может быть\n", way->road[ARRAY_SIZE - 1] + 1);
}

Road *MinimumTour(int **matrix, int **new_matrix)
{
	Road *res = new Road;
	res->len = MAXINT;
	res->road = NULL;

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Road *way = ShortestWay(new_matrix, i);
		way->len = WayLength(matrix, way);

		PrintTour(matrix, new_matrix, way);

		if (res->len > way->len)
		{
			res->len = way->len;
			res->road = way->road;
		}
	}
	return res;
}

void PrintMatrix(int **matrix, int len)
{
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < len; k++)
			if (matrix[i][k] == -1) printf("  _  ");
			else printf("%3i  ", matrix[i][k]);
		printf("\n\n");
	}
}

int main(void)
{
	setlocale(LC_ALL, "Russian");
	int **matrix = Inicialize();
	int **new_matrix = Inicialize();

	for (int i = 0; i < ARRAY_SIZE; i++) Dijkstra(matrix, new_matrix, i);
	PrintMatrix(matrix, ARRAY_SIZE);
	printf("----------------------------------\n");
	PrintMatrix(new_matrix, ARRAY_SIZE);

	Road *res = MinimumTour(matrix, new_matrix);
	printf("\n\nСамый короткий путь: ");
	PrintTour(matrix, new_matrix, res);
	printf("\n\n");

	system("pause");
	free(res);
	free(matrix);
	free(new_matrix);
	return 0;
}