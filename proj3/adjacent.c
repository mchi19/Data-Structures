#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "adjacent.h"

//Global Declarations
typedef struct list
{
	int node;
	int x;
	int y;
	struct list * next;
} Node;

Node * insert_node(Node * head, int n_Val, int x_val, int y_val);
Node * make_node(int n_val, int x_val, int y_val);
void destroy_list(Node * tn);


int main(int argc, char ** argv)
{	
	if(argc < 2)
	{
		printf("Error, please provide input file\n");
		return 0;
	}
	FILE * fptr;
	fptr = fopen(argv[1], "r");
	if(fptr == NULL)
	{
		printf("Error reading input file\n");
		return 0;
	}
	int num_vertex;
	int num_edges;
	fscanf(fptr, "%d %d\n", &num_vertex, &num_edges);
	int adj_matrix[num_vertex][num_vertex];
	int k;
	int l;
	for(k = 0; k < num_vertex; k++) //initialize entire adjacency matrix to 0
	{
		for(l = 0; l < num_vertex; l++)
		{
			adj_matrix[k][l] = 0;
		}
	}
	int i = 0;
	Node * list = NULL; //list of vertex numbers
	while(i < num_vertex)
	{
		int t_vnum; //temporary vertex number
		int t_x; //temp x coordinate
		int t_y; //temp y coordinate
		fscanf(fptr, "%d %d %d\n", &t_vnum, &t_x, &t_y); 	
		list = insert_node(list, t_vnum, t_x, t_y);
		i++;
	}
	int t_n1; //temp node 1
	int t_n2; //temp node 2
	int j = 0;
	while(j <= num_edges){
	fscanf(fptr, "%d %d\n", &t_n1, &t_n2);
	for(k = 0; k < num_vertex; k++)
	{
		if(k == t_n1)
		{
			for(l = 0; l < num_vertex; l++)
			{
				if(l == t_n2)
				{
					adj_matrix[k][l] = 1; //1 represents adjacency
					adj_matrix[l][k] = 1; 
				}
			}
		}
	}
	j++;
	}
	for(k = 0; k < num_vertex; k++)
	{
		printf("%d:", k);
		for(l = 0; l < num_vertex; l++)
		{
			if(adj_matrix[k][l] == 1)
			{
				printf(" %d", l);
			}
		}
		printf("\n");
	}
	destroy_list(list);
	return 0;
}

Node* insert_node(Node * head, int n_val, int x_val, int y_val)
{
	Node * curr;
	Node * temp;
	if(head == NULL)
	{
		curr = make_node(n_val, x_val, y_val);
		return(curr);
	}
	curr = head;
	while(curr->next != NULL)
	{
		curr = curr->next;
	}
	temp = make_node(n_val, x_val, y_val);
	curr->next = temp;
	return head;
}

Node * make_node(int n_val, int x_val, int y_val)
{
	Node * ln;
	ln = malloc(sizeof(Node));
	ln->node = n_val;
	ln->x = x_val;
	ln->y = y_val;
	ln->next = NULL;
	return ln;
}

void destroy_list(Node * tn)
{
	if(tn->next != NULL)
	{
		destroy_list(tn->next);
	}
	free(tn);
}
	
