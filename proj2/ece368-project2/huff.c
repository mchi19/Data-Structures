#include <stdio.h>
#include <stdlib.h>
#include "huff.h"
#define ASCISIZE 127

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("Error, please give input\n");	
		return 0;
	}
	int array_freq[ASCISIZE];
	char file_out[ASCISIZE];
	char * filename = argv[1];
	int u_count = character_frequency_counter(filename, array_freq);
	strcpy(file_out, filename); //copies content name to outputfile
	strcat(file_out, ".huff"); //adds the .huff extension
	Node * list = make_table(array_freq);
	if(list == NULL)
	{
		return 0;
	}
	ListNode * treelist = build_ListNode(list);
	while(treelist -> next != NULL)
	{
		ListNode * second = treelist -> next;
		ListNode * third = second -> next;
		TreeNode * tn1 = treelist -> tree_ptr;
		TreeNode * tn2 = second -> tree_ptr;
		free(treelist);
		free(second);
		treelist = third;
		TreeNode * fin = merge_Tree(tn1, tn2);
		ListNode * wn = make_ListNode_entry(fin);
		treelist = insert_ListNode(treelist, wn);
	}
	destroy_table(list);
	TreeNode * root = treelist -> tree_ptr;
	free(treelist);
	int numRow = Tree_leaf(root);
	int numCol = Tree_height(root);
	numCol++;
	int ** codebook = malloc(sizeof(int*) * numRow);
	int row;
	for(row = 0; row < numRow; row++)
	{
		codebook[row] = malloc(sizeof(int) * numCol);
		int col;
		for(col = 0; col < numCol; col++)
		{
			codebook[row][col] = -1;
		}
	}
	buildCodeBook(root, codebook);
	int map[ASCISIZE];
	int i;
	for(i = 0; i < ASCISIZE; i++)
	{
		map[i] = -1;
		int j;
		for(j = 0; j < numRow; j++)
		{
			if(codebook[j][0] == i)
			{
				map[i] = j;
			}
		}
	}
	Tree_header(root, u_count, file_out);
	compressor(filename, file_out, codebook, map);
	for(i = 0; i < numRow; i++)
	{
		free(codebook[i]);
	}
	free(codebook);
	destroy_tree(root);
	return 0;
}

//counts the frequncies of characters and returns the number of unique ones
int character_frequency_counter(char *filename, int *freq)
{
	FILE * fptr; 
	int i;
	int uniq_cnt = 0;
	fptr = fopen(filename, "r");
	if(fptr == NULL)
	{
		fprintf(stderr,"Failed to open '%s' for reading\n", filename);
		return uniq_cnt;
	}
	for(i = 0; i < ASCISIZE; i++)
	{
		freq[i] = 0;//sets the count of all characters to be 0
	}
	while((i = fgetc(fptr)) != EOF) //while i is not the end of the file
	{
		freq[i]++;	//adds 1 to the corresponding character count 
	}
	fclose(fptr);
	for(i = 0; i < ASCISIZE; i++)
	{
		if(freq[i] > 0)
		{
			uniq_cnt++; //counts number of unique characters used
		}
	}
	return uniq_cnt;
}
//creates an entry for a character and its frequencies
Node * make_entry(int val, int frequency)
{
	Node * ch;
	ch = malloc(sizeof(Node));
	ch -> c_val = val;
	ch -> freq = frequency;
	ch -> next = NULL;
	return ch;	
}

//inserts the character entries in order
Node * insert_table(Node * head, int value, int frequency)
{
	Node * curr;
	Node * temp;
	if((head == NULL) || (head->freq >= frequency))
	{
		curr = make_entry(value, frequency);
		curr -> next = head;
		return curr;
	}
	else
	{
		curr = head;
		while((curr->next != NULL) && (curr->freq < frequency))
		{
			curr = curr->next;
		}
		temp = make_entry(value, frequency);
		temp->next = curr->next;
		curr->next = temp;
	}
	return head;//might want to return curr originally return head
}

//constructs the frequency table
Node * make_table(int * freq)
{
	Node * ch;
	int i = 0;
	int j;
	while(freq[i] == 0)
	{
		i++;//incriments until it finds first character used
	}
	ch = make_entry(i, freq[i]);
	i++;
	for(j = i; j < ASCISIZE; j++)
	{
		if(freq[j] > 0)
		{
			ch = insert_table(ch, j, freq[j]);
		}
	}
	return ch;
}

//destroies the table so no mem errors
void destroy_table(Node * head)
{
	if(head != NULL)
	{
		destroy_table(head->next);
	}
	free(head);
}

ListNode * make_ListNode_entry(TreeNode * tn)
{
	ListNode * wn; //weighted node
	wn = malloc(sizeof(ListNode));
	wn -> tree_ptr = tn;
	wn -> next = NULL;
	return wn;
}

ListNode * insert_ListNode(ListNode * head, ListNode * wn)
{
	int w1;
	int w2;
	if(head == NULL)
	{
		return wn;
	}
	w1 = (head -> tree_ptr) -> weight;
	w2 = (wn -> tree_ptr) -> weight;
	if(w1 > w2)
	{
		wn -> next = head;
		return wn;
	}
	head -> next = insert_ListNode(head -> next, wn);
	return head;
}

ListNode * build_ListNode(Node * freqList)
{
	ListNode * head = NULL;
	while(freqList != NULL)
	{
		TreeNode * tn = create_TreeNode(freqList -> c_val, freqList -> freq);
		ListNode * wn = make_ListNode_entry(tn);
		head = insert_ListNode(head, wn);
		freqList = freqList -> next;
	}
	return head;
}

void destroy_tree(TreeNode * node)
{
	if(node == NULL)
	{
		return;
	}
	destroy_tree(node -> left);
	destroy_tree(node -> right);
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

//Finding the height of a tree
int Tree_heightHelper(TreeNode * tn, int height)
{
	int l_h; //left height
	int r_h; //right height
	if(tn == 0)
	{
		return height;
	}
	l_h = Tree_heightHelper(tn -> left, height + 1);
	r_h = Tree_heightHelper(tn -> right, height + 1);
	if(l_h > r_h)
	{
		return l_h;
	}
	if(l_h < r_h)
	{
		return r_h;
	}
	return l_h;
}

int Tree_height(TreeNode * tn)
{
	return Tree_heightHelper(tn, 0);
}

//gets to a leaf
void Tree_leafHelper(TreeNode * tn, int * num)
{
	if(tn == 0)
	{
		return;
	}
	TreeNode * lc = tn -> left; //left child
	TreeNode * rc = tn -> right; //right child
	if((lc == NULL) && (rc == NULL))
	{
		(*num)++;
		return;
	}
	Tree_leafHelper(lc, num);
	Tree_leafHelper(rc, num);
}

int Tree_leaf(TreeNode * tn)
{
	int num = 0;
	Tree_leafHelper(tn, &num);
	return num;
}

void buildCodeBookHelper(TreeNode * tn, int **codebook, int * row, int col)
{
	TreeNode * lc;
	TreeNode * rc;
	int numRow;
	int i;
	if(tn == NULL)
	{
		return;
	}
	lc = tn -> left; //left child
	rc = tn -> right; //right child
	if((lc == NULL) && (rc == NULL));
	{
		codebook[*row][0] = tn -> character;
		(*row)++;
		return;
	}
	if(lc != NULL)
	{
		numRow = Tree_leaf(lc);
		for(i = *row; i < (*row) + numRow; i++)
		{
			codebook[i][col] = 0;
		}
		buildCodeBookHelper(lc, codebook, row, col + 1);
	}
	if(rc != NULL)
	{
		numRow = Tree_leaf(rc); 
		for(i = *row; i < (*row) + numRow; i++)
		{
			codebook[i][col] = 1;
		}
		buildCodeBookHelper(rc, codebook, row, col + 1);
	}
}

void buildCodeBook(TreeNode * root, int ** codebook)
{
	int row = 0;
	buildCodeBookHelper(root, codebook, &row, 1);
}

int writeBit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char* curr_bit)
{
	unsigned char temp;
	int value = 0;
	int ret;
	if((*whichbit) == 0)
	{
		*curr_bit = 0;
	}
	temp = bit << (7 - (*whichbit));
	*curr_bit |= temp;
	if((*whichbit) == 7)
	{
		ret = fwrite(curr_bit, sizeof(unsigned char), 1, fptr);
		if(ret == 1)
		{
			value = 1;
		}
		else
		{
			value = -1;
		}
	}
	*whichbit = ((*whichbit) + 1) % 8;
	return value;
}

void char_to_bit(FILE * fptr_out, int ch, unsigned char * whichbit, unsigned char * curr_bit)
{
	unsigned char mask = 0x40;
	while(mask > 0)
	{
		writeBit(fptr_out, (ch & mask) == mask, whichbit, curr_bit);
		mask >>= 1;
	}
}

void Tree_headerHelper(TreeNode * tn, FILE * fptr_out, unsigned char * whichbit, unsigned char * curr_bit)
{
	TreeNode * lc;
	TreeNode * rc;
	if(tn == NULL)
	{
		return;
	}
	lc = tn -> left;
	rc = tn -> right;
	if((lc == NULL) && (rc == NULL))
	{
		writeBit(fptr_out, 1, whichbit, curr_bit);
		char_to_bit(fptr_out, tn -> character, whichbit, curr_bit);
		return;
	}
	Tree_headerHelper(lc, fptr_out, whichbit, curr_bit);
	Tree_headerHelper(rc, fptr_out, whichbit, curr_bit);
	writeBit(fptr_out, 0, whichbit, curr_bit);
}

void Tree_header(TreeNode * tn, unsigned int numChar, char * filename)
{
	FILE * fptr_out;
	unsigned char whichbit = 0;
	unsigned char curr_bit = 0;
	fptr_out = fopen(filename, "w");
	if(fptr_out == NULL)
	{
		return;
	}
	Tree_headerHelper(tn, fptr_out, &whichbit, &curr_bit);
	writeBit(fptr_out, 0, &whichbit, &curr_bit);
	padZero(fptr_out, &whichbit, &curr_bit);
	fwrite(&numChar, sizeof(unsigned int), 1, fptr_out);
	unsigned char newline = '\n';
	fwrite(&newline, sizeof(unsigned char), 1, fptr_out);
	fclose(fptr_out);
}

int compressor(char * file_in, char * file_out, int **codebook, int * map)
{
	unsigned char whichbit = 0;
	unsigned char curr_bit = 0;
	int onechar;
	int i;
	int j;
	FILE * fptr_in = fopen(file_in, "r");
	if(file_in == NULL)
	{
		printf("Error, not able to read file\n");
		//fclose(fptr_in);
		return 0;
	}
	FILE * fptr_out = fopen(file_out, "ab");
	if(fptr_out == NULL)
	{
		printf("ERROR, not able to append file\n");
		fclose(fptr_out);
		return 0;
	}	
	while(!feof(fptr_in))
	{
		onechar = fgetc(fptr_in);
		if(onechar != EOF)
		{
			i = map[onechar];
			j = 1;
			while(codebook[i][j] != -1)
			{
				writeBit(fptr_out, (codebook[i][j] == 1), &whichbit, &curr_bit);
				j++;
			}
		}
	}
	padZero(fptr_out, &whichbit, &curr_bit);
	fclose(fptr_in);
	fclose(fptr_out);
	return 0;
}

int padZero(FILE * fptr, unsigned char * whichbit, unsigned char * curr_bit)
{
	int x= 0;
	while((*whichbit) != 0)
	{
		x = writeBit(fptr, 0 , whichbit, curr_bit);
		if(x == -1)
		{
			return -1;
		}
	}
	return x;
}


