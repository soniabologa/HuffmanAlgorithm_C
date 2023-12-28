/****************************************/
/* Sonia Bologa                         */
/* UNM ID: 101317238                    */
/* Date: 05/05/2023                     */
/*                                      */
/* This program decodes a file that has */
/* been encoded using the huffman       */
/* algorithm.                           */
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include<limits.h>
#include "huffman.h"

#define NUM_CHAR 256
#define TRUE 1
#define FALSE 0

struct Node
{
  unsigned char symbol;
  unsigned long frequency;
  struct Node* left;
  struct Node* right;
  struct Node* next;
  int codeArray[NUM_CHAR];
  int codeLength;
}; 

/*******************************************************/
/* Decodes a file encoded with the Huffman algorithm.  */
/* Reads characters and their frequencies from a file, */
/* stores them in two different arrays, then creates a */
/* Huffman tree using the data in the arrays. It then  */
/* continues reading from the file, and moves through  */
/* the tree as indicated by individual bits. When it   */
/* reaches a leaf, it outputs the decoded character to */
/* the output file.                                    */
/* in -- file containing input                         */
/* out -- file that the output will be written into    */
/******************************************************/
void decodeFile(FILE* in, FILE* out)
{
  unsigned char characters[NUM_CHAR];
  unsigned long frequency[NUM_CHAR];
  
  unsigned char uniqueChar, buffer;
  unsigned long totalChar;
  struct Node* head;
  struct Node* newNode;
  struct Node* top;
  struct Node* current;

  int i, bufferIndex, byteCounter;

  head = NULL;
  
  fread(&uniqueChar, sizeof(unsigned char), 1, in);
  for(i = 0; i < uniqueChar; i++)
  {
    fread(&characters[i], sizeof(unsigned char), 1, in);
    fread(&frequency[i], sizeof(unsigned long), 1, in);
    newNode = createNode(characters[i], frequency[i]);
    head = insertSorted(head, newNode);
  }
  fread(&totalChar, sizeof(unsigned long), 1, in);

  top = buildTree(head);

  current = top;
  byteCounter = 0;
  bufferIndex = 0;

  /* adapted from https://github.com/adeesha-savinda/huffman-encode-decode */
  
  while(byteCounter < totalChar)
  {
    if (bufferIndex == 0)
    {
      fread(&buffer, sizeof(unsigned char), 1, in);
    }
    if(buffer & 128)
    {
      current = current->right;
    }
    else
    {
      current = current->left;
    }
    if(isLeaf(current))
    {
      fwrite(&(current->symbol), sizeof(unsigned char), 1, out);
      byteCounter++;
      current = top;
    }
    buffer = buffer << 1;
    if(++bufferIndex > 7)
    {
      bufferIndex = 0;
    }
  }
  freeTree(top);
}

/*******************************************************/
/* Main function which open input and output files,    */
/* checks whether the number of arguments is correct,  */
/* then calls the decodeFile function. When decoding   */
/* is finished, it closes the input and output files,  */
/* and free memory allocated to the Huffman tree       */
/* in -- int argc, number of arguments                 */ 
/*       char ** argv, pointer to a pointers to arrays */
/*	 of strings containing command line arguments  */
/* out -- returns 0 at the end of main                 */
/*******************************************************/
int main(int argc, char** argv)
{
  char* infile;
  char* outfile;
  FILE* in;
  FILE* out;

  if(argc != 3) 
  {
    printf("wrong number of args\n");
    return 1;
  }

  infile = argv[1];
  outfile = argv[2];

  in = fopen(infile, "rb");
  if(in == NULL)
  {
    printf("couldn't open %s for reading\n", infile);
    return 2;
  }

  out = fopen(outfile, "wb");
  if(out == NULL)
  {
    printf("couldn't open %s for writing\n", outfile);
    return 3;
  }

  decodeFile(in, out);

  fclose(in);
  fclose(out);

  return 0;
}
