#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("Error, please provide input\n");
		return 0;
	}
	char file_out[255];
	char * filename = argv[1];
	strcpy(file_out, filename);
	strcat(file_out, ".unhuff");
	decoder(filename, file_out);
	return 0;
}

TreeNode * read_Header(FILE * fptr_in)
{
	int done = 0;
	unsigned char whichbit = 0;
	unsigned char curr_bit = 0;
	unsigned char onebit = 0;
	ListNode * head = NULL;
	while(done == 0)
	{
		readBit(fptr_in, &onebit, &whichbit, &curr_bit);
		if(onebit == 1)
		{
			int bit_cnt;
			unsigned char value = 0;
			for(bit_cnt = 0; bit_cnt < 7; bit_cnt++)
			{
				value <<=1; //shift left by one
				readBit(fptr_in, &onebit, &whichbit, &curr_bit);
				value |= onebit;
			}
		TreeNode * tn = create_TreeNode(value, 0);
		ListNode * wn = make_ListNode_entry(tn);
		head = insert_ListNode(head, wn);
		}		
		else
		{
			if(head == NULL)
			{
				printf("Error");
			}
			if((head->next) == NULL)
			{
				done = 1;
			}
			else
			{
				head = Merge_ListNode(head);
			}
		}
	}
	TreeNode * root = head -> tree_ptr;
	free(head);
	return(root);
}

int decoder(char * file_in, char * file_out)
{
	FILE * fptr_in = fopen(file_in, "r");
	if(fptr_in == NULL)
	{
		printf("Error, unable to open file\n");
		return 0;
	}
	int temp_ck = lastchk(".huff", file_in);
	if(temp_ck == 0)
	{
		printf("Error, input file requires .huff extension\n");
		return 0;
	}
	TreeNode * root = read_Header(fptr_in);
	unsigned int u_count = 0;
	fread(&u_count, sizeof(unsigned int), 1, fptr_in);
	if(u_count < 0)
	{
		printf("There are less than 0 unique characters\n");
		return 0;
	}
	unsigned char newline;
	fread(&newline, sizeof(unsigned char), 1, fptr_in);
	if(newline != '\n')
	{
		printf("Error, newline\n");
		return 0;
	}
	unsigned char whichbit = 0;
	unsigned char curr_bit = 0;
	unsigned char onebit = 0;
	FILE * fptr_out = fopen(file_out, "w");
	while(u_count != 0)
	{
		TreeNode * tn = root;
		while((tn-> left) != NULL)
		{
			readBit(fptr_in, &onebit, &whichbit, &curr_bit);
			if(onebit == 0)
			{
				tn = tn -> left;
			}
			else
			{
				tn = tn -> right;
			}
		}
		fprintf(fptr_out, "%c", tn-> character);
		u_count--;
	}
	destroy_tree(root);
	fclose(fptr_in);
	fclose(fptr_out);
	return 0;
}

ListNode * make_ListNode_entry(TreeNode * tn)
{
	ListNode * wn;
	wn = malloc(sizeof(ListNode));
	wn -> next = NULL;
	wn -> tree_ptr = tn;
	return wn;
}

ListNode * insert_ListNode(ListNode * head, ListNode * wn)
{
	if(wn == NULL)
	{
		printf("Error\n");
		return NULL;
	}
	if((wn->next) != NULL)
	{
		printf("Error\n");
	}
	if(head == NULL)
	{
		return wn;
	}
	wn -> next = head;
	return wn;
}

void destroy_tree(TreeNode * node)
{
	if(node == NULL)
	{
		return;
	}
	destroy_tree(node->left);
	destroy_tree(node->right);
	free(node);
}

TreeNode * create_TreeNode(int char_val, int weight_val)
{
	TreeNode * node;
	node = malloc(sizeof(TreeNode));
	node -> character = char_val;
	node -> weight = weight_val;
	node -> left = NULL;
	node -> right = NULL;
	return node;
}

TreeNode * merge_Tree(TreeNode * tn1, TreeNode * tn2)
{
	TreeNode * tn;
	tn = malloc(sizeof(TreeNode));
	tn -> character = 0;
	tn -> left = tn1;
	tn -> right = tn2;
	tn -> weight = (tn1 -> weight) + (tn2 -> weight);
	return tn;
}

ListNode * Merge_ListNode(ListNode * head)
{
	ListNode * second = head -> next;
	ListNode * third = second -> next;
	TreeNode * tn1 = head -> tree_ptr;
	TreeNode * tn2 = second -> tree_ptr;
	free(head);
	free(second);
	head = third;
	TreeNode * fin;
	fin = merge_Tree(tn2, tn1);
	ListNode * wn = make_ListNode_entry(fin);
	head = insert_ListNode(head, wn);
	return head;
}

int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curr_bit)
{
	int ret = 1;
	if((*whichbit) == 0)
	{
		ret = fread(curr_bit, sizeof(unsigned char), 1, fptr);
	}
	if(ret != 1)
	{
		return -1;
	}
	unsigned char temp = (*curr_bit) >> ( 7 - (*whichbit));
	temp = temp & 0x01;
	*whichbit = ((*whichbit) + 1) % 8;
	*bit = temp;
	return 0;
}

int lastchk(const char * x, const char * y)
{
	int len1 = strlen(x);
	int len2 = strlen(y);
	if(len1 > len2)
	{
		return 0;
	}
	return strcmp(x, y + (len2 - len1)) == 0;
}




