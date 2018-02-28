#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef struct char_freq
{
	int c_val; //ASCII value/binary value of a character
	int freq; //frequency of character
	struct char_freq * next;
} Node;

typedef struct tree_node
{
	int character;
	int weight;
	struct tree_node * left;
	struct tree_node * right;
} TreeNode;

typedef struct list_node 
{
	TreeNode * tree_ptr;
	struct list_node * next;
} ListNode;

//declarations for counting character frequencies
int character_frequency_counter(char *filename, int *freq);
//the following 3 functions make  organize the character frequencies
Node * make_table(int * freq);
Node * insert_table(Node * head, int value, int frequency);
Node * make_entry(int val, int frequency);
//destorys the table to avoid mem leaks
void destroy_table(Node * head);

ListNode * make_ListNode_entry(TreeNode * tn);
ListNode * insert_ListNode(ListNode * head, ListNode * wn);
ListNode * build_ListNode(Node * freqList);

void destroy_tree(TreeNode * node);
TreeNode * create_TreeNode(int char_val, int weight_val);
TreeNode * merge_Tree(TreeNode * tn1, TreeNode * tn2);

int Tree_heightHelper(TreeNode * tn, int height);
int Tree_height(TreeNode * tn);

void Tree_leafHelper(TreeNode * tn, int * num);
int Tree_leaf(TreeNode * tn);

void buildCodeBookHelper(TreeNode * tn, int ** codebook, int * row, int col);
void buildCodeBook(TreeNode * root, int ** codebook);

int writeBit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curr_bit);
void char_to_bit(FILE * fptr_out, int ch, unsigned char * whichbit, unsigned char * curr_bit);
void Tree_headerHelper(TreeNode * tn, FILE * fptr_out, unsigned char * whichbit, unsigned char * curr_bit);

void Tree_header(TreeNode * tn, unsigned int numChar, char * filename);
int compressor(char * file_in, char * file_out, int ** codebook, int * map);
int padZero(FILE * fptr, unsigned char * whichbit, unsigned char * curr_bit);
