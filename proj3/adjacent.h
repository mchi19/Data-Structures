#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list
{
	int node;
	int x;
	int y;
	struct list * next;
} Node;

Node * insert_node(Node * head, int n_val, int x_val, int y_val);
Node * make_node(int n_val, int x_val, int y_val);
void destroy_list(Node * tn);

