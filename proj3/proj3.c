#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "proj3.h"

int main(int argc, char ** argv)
{
	if(argc < 3)
	{
		printf("Error, please provide input files\n");
		return 0;
	}
	FILE * fptr1; 
	FILE * fptr2;
	///////////ERROR CHECKING///////////
	fptr1 = fopen(argv[1], "r");
	if(fptr1 == NULL)
	{
		printf("Error reading input file\n");
		return 0;
	}
	fptr2 = fopen(argv[2], "r");
	if(fptr2 == NULL)
	{
		printf("Error reading input file\n");
		return 0;
	}
	/////////CREATE & INPUT VERTEX LOCATIONS///////
	int num_vert;
	int num_edges;
	fscanf(fptr1, "%d %d\n", &num_vert, &num_edges);
	int t_x; //temp x coordinate
	int t_y; //temp y coordinate
	int t_vert; //temp vertex
	int i;
	int ** vert_loc = (int **)malloc(sizeof(int*) * num_vert);
	for(i = 0; i < num_vert; i++)
	{
		vert_loc[i] = (int*)malloc(sizeof(int)*2);
		vert_loc[i][0] = 0;
		vert_loc[i][1] = 0;
	}
	for(i = 0; i < num_vert; i++)
	{
		fscanf(fptr1, "%d %d %d\n", &t_vert, &t_x, &t_y);
		vert_loc[i][0] = t_x;
		vert_loc[i][1] = t_y;
	}
	///////CREATE AND INSERT DISTANCES BETWEEN ADJACENT VERTICIES////
	int ** adj_dist = (int **)malloc(sizeof(int*) * num_vert);
	int j;
	for(i = 0; i < num_vert; i++)
	{
		adj_dist[i] = (int *)malloc(sizeof(int)* num_vert);
		//printf("%d\n",i); 
		for(j = 0; j < num_vert; j++)
		{
			adj_dist[i][j] = 0;
		}
	}
	int tv1; //temp vertex 1
	int tv2; //temp vertex 2
	double t_dist;
	for(i = 0; i < num_edges; i++)
	{
		fscanf(fptr1, "%d %d\n", &tv1, &tv2);
		int tempx1 = vert_loc[tv1][0];
		int tempy1 = vert_loc[tv1][1];
		int tempx2 = vert_loc[tv2][0];
		int tempy2 = vert_loc[tv2][1];
		t_dist = distance(tempx1, tempy1, tempx2, tempy2);
		adj_dist[tv1][tv2] = t_dist;
		adj_dist[tv2][tv1] = t_dist;
	}

	/////////BEGIN FINDING SHORTEST PATH BASED ON QUERIES////////
	int num_query;
	int s_v; //starting vertex
	int e_v; //ending vertex
	fscanf(fptr2, "%d\n", &num_query);
	for(i = 0; i < num_query; i++)
	{
		fscanf(fptr2, "%d %d\n", &s_v, &e_v);
		if((s_v > num_vert) || (e_v > num_vert))
		{
			printf("INF\n");
			printf("%d %d\n", s_v, e_v);
		}
		else
		{
			dijkstra(num_vert, adj_dist, s_v, e_v);
		}
	}
	/////////FREE EVERYTHING THAT ALLOCATED MEMORY///////////
	for(i = 0; i < num_vert; i++)
	{
		free(vert_loc[i]);
		free(adj_dist[i]);
	}
	free(vert_loc);
	free(adj_dist);
	fclose(fptr1);
	fclose(fptr2);
	return 0;
}

double distance(int x1, int y1, int x2, int y2)
{
	double t1; //temp variable
	double t2; //temp variable
	double fd; //final distance
	t1 = pow((x2-x1),2);
	t2 = pow((y2-y1),2);
	fd = floor(sqrt(t1+t2));
	return fd;
}

int min_dist(int num_vert, int dist_tracker[], bool sptSet[])
{
	int i;
	int min = INT_MAX;
	int min_index = -1;
	for(i = 0; i < num_vert; i++)
	{
		if(sptSet[i] == false && dist_tracker[i] <= min)
		{
			min = dist_tracker[i];
			min_index = i;
		}
	}
	return min_index;
}

void dijkstra(int num_vert, int **matrix, int startv, int endv)
{
	int i;
	int cv; //current vertex
	int count;
	int dist[num_vert];
	bool sptSet[num_vert];
	int prev_path[num_vert];
	prev_path[startv] = -1;
	for(i = 0; i < num_vert; i++)
	{
		dist[i] = INT_MAX; //initializing all distances to infinite
		sptSet[i] = false;
	}
	dist[startv] = 0; //distance to itself is 0
	
	//short path for all verticies
	for(count = 0; count < num_vert; count++)
	{
		int u = min_dist(num_vert, dist, sptSet);
		//set the vertex as visited
		sptSet[u] = true;

		//update the dist values for the right vertexes
		for(cv = 0; cv < num_vert; cv++)
		{
			//only update the vertecies if it meets the following requirements
			if(!sptSet[cv] && matrix[u][cv] && dist[u] != INT_MAX && dist[u] + matrix[u][cv] < dist[cv])
			{
				dist[cv] = dist[u] + matrix[u][cv];
				//update route taken
				prev_path[cv] = u;
			}
		}
	}
	/////////////////////////PRINT RESULTS/////////////////////
	printf("%d\n", dist[endv]);
	int j = endv;
	//int j;
	int array[num_vert];
	for(i = 0; i < num_vert; i++)
	{
		array[i] = 0;
	}
	int steps = 0;
	array[0] = endv;
	while(prev_path[j] != -1)
	{
		steps++;
		array[steps] = prev_path[j];
		j = prev_path[j];
	}
	for(i = 0; i <= steps; i++)
	{
		printf("%d ", array[steps - i]);
	}
	printf("\n");

}
