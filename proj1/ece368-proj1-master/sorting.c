#include <stdio.h>
#include "sorting.h"

//sequence function declarations
void Save_Seq1(char *Filename,int N);
void Save_Seq2(char *Filename, int N);

//Functions definitions
long *Load_File(char *Filename, int *Size)
{
	FILE *fptr;
	fptr = fopen(Filename, "r");
	if(fptr == NULL)
	{
		fprintf(stderr, "Failed to open '%s' for reading\n", Filename);
		return 0;
	}
	fscanf(fptr, "%d", Size);
	long *Array = NULL;
	Array = malloc(*Size*sizeof(long));
	int i;
	for(i = 0; i < *Size; i++)
	{
		fscanf(fptr, "%ld",&Array[i]);	
	}
	fclose(fptr);
	return Array;
}

int Save_File(char *Filename, long *Array, int Size)
{
	int i = 0;
	int count = 0;
	FILE *fptr;
	fptr = fopen(Filename, "w");
	if(fptr == NULL)
	{
		fprintf(stderr, "Failed to open '%s' for writing\n", Filename);
		return count;
	}
	fprintf(fptr, "%d\n", Size);
	for(i = 0; i < Size; i++)
	{
		fprintf(fptr, "%ld\n", Array[i]);	
		count++;
	}
	fclose(fptr);
	return count;
}

void Save_Seq1(char *Filename, int N)
{
	FILE *fptr;
	fptr = fopen(Filename, "w");
	if(fptr == NULL)
	{
		fprintf(stderr, "Failed to open '%s' for writing\n", Filename);
		return;
	}	
	int seq_arr[N];
	int x;
	for(x = 0; x < N; x++)
	{
		seq_arr[x] = 0;
	}
	seq_arr[0] = 1; //first number is always 1
	int i;
	int p = 0; //power for 2
	int q = 0; //power for 3
	int u2 = 0; //result for 2
	int u3 = 0; //result for 3
	int k = 0;	//shell size
	for(i = 1; k < N ;i++)
	{
		if(seq_arr[p] * 2 == seq_arr[i-1])
		{
			p += 1;
		}
		if(seq_arr[q] * 3 == seq_arr[i-1])
		{
			q += 1;
		}
		u2 = seq_arr[p] * 2;
		u3 = seq_arr[q] * 3;
		if(u2 < u3)
		{
			p += 1;
			seq_arr[i] = u2;
		}
		else
		{
			q += 1;
			seq_arr[i] = u3;
		}
		k = seq_arr[i];
	}
	int j;
	fprintf(fptr, "%d\n", N);
	for(j = 1; j < N; j++)
	{
		fprintf(fptr, "%d\n", seq_arr[j]);
	}
	fclose(fptr);
}

void Save_Seq2(char *Filename, int N)
{
	int seq_arr[N];
	int i;
	int count = 0;
	int m = N;
	FILE *fptr;
	fptr = fopen(Filename, "w");
	if(fptr == NULL)
	{
		fprintf(stderr, "Failed to open '%s' for writing\n", Filename);
		return;
	}
	while(m != 1)
	{
		m = m/1.3;
		count++;
	}
	seq_arr[0] = N/1.3;
	for(i = 1; i < count; i++)
	{
		seq_arr[i] = seq_arr[i-1] / 1.3;
		if((seq_arr[i] == 9) || (seq_arr[i] == 10))
		{
			seq_arr[i] = 11;
		}
	}
	int j;
	fprintf(fptr, "%d\n", i);
	for(j = 0; j < N; j++)
	{
		fprintf(fptr, "%d\n", seq_arr[j]);
	}
	fclose(fptr);
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
	int i;
	int p = 0;
	int q = 0;
	int u2 = 0;
	int u3 = 0;
	int k = 0;
	int gap = 0;
	long temp_r = 0;
	int x;
	int y;
	int z;
	int seq_arr[Size];
	seq_arr[0] = 1;
	for(i = 1; k < Size; i++)
	{
		if(seq_arr[p] * 2 == seq_arr[i - 1])
		{
			p += 1;
		}
		if(seq_arr[q] * 3 == seq_arr[i - 1])
		{
			q += 1;
		}
		u2 = seq_arr[p] * 2;
		u3 = seq_arr[q] * 3;
		if(u2 < u3)
		{
			p += 1;
			seq_arr[i] = u2;
		}
		else
		{
			q += 1;
			seq_arr[i] = u3;
		}
		k = seq_arr[i];
	}
	for(x = i; x >= 0; x--) //go through all of the gaps generated 
	{
		gap = seq_arr[x];
		for(y = gap; y < Size; y++)
		{
			temp_r = Array[y];
			*N_Move = *N_Move + 1;
			z = y;
			while((z >= gap) && (Array[z - gap] > temp_r)) //can't seem to enter the while loop
			{	
				*N_Comp = *N_Comp + 1;
				Array[z] = Array[z - gap];
				*N_Move = *N_Move + 1;
				z = z - gap;
			}
			Array[z] = temp_r;
			*N_Move = *N_Move + 1;
		}
	}
}

void Improved_Bubble_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
	int i;
	int j;
	int x;
	int gap = 0;
	int seq_arr[Size];
	int m = Size;
	int count = 0;
	long temp_r;
	while(m != 1)
	{
		m = m/1.3;
		count++;
	}
	seq_arr[0] = Size/1.3;
	for(i = 1; i < count; i++)
	{
		seq_arr[i] = seq_arr[i-1] / 1.3;
		if((seq_arr[i] == 9) || (seq_arr[i] == 10))
		{
			seq_arr[i] = 11;
		}
	}
	for(j = 0;j < count; j++)
	{
		x = 0;
		gap = seq_arr[j];
		while(gap <= Size)
		{
			*N_Comp = *N_Comp + 1;
			if(Array[x] > Array[gap])
			{
				temp_r = Array[x];
				*N_Move = *N_Move + 1;
				Array[x] = Array[gap];
				*N_Move = *N_Move + 1;
				Array[gap] = temp_r;
				*N_Move = *N_Move + 1;
			}
			x++;
			gap++;
		}
	}	
}

