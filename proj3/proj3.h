#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef int bool;
#define true 1
#define false 0

double distance(int x1, int y1, int x2, int y2);
int min_dist(int num_vert, int dist_tracker[], bool sptSet[]);
void dijkstra(int num_vert, int **matrix, int startv, int endv);


