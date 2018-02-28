#include <stdio.h>
#include <stdlib.h>

typedef struct char_freq
{
	int c_val;
	int freq;
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
	struct list_node * next;
	TreeNode * tree_ptr;
} ListNode;

TreeNode * read_Header(FILE * fptr_in);
ListNode * Merge_ListNode(ListNode * head);
ListNode * make_ListNode_entry(TreeNode * tn);
ListNode * insert_ListNode(ListNode * head, ListNode * wn);

void destroy_tree(TreeNode * node);
TreeNode * create_TreeNode(int char_val, int weight_val);
TreeNode * merge_Tree(TreeNode * tn1, TreeNode * tn2);

int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte);
int decoder(char * file_in, char * file_out);
//void Tree_print(TreeNode * tn, int level);
int lastchk(const char * x, const char * y);


