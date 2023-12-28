/*************************************/
/* Sonia Bologa                      */
/* UNM ID: 101317238                 */
/* Date: 05/05/2023                  */
/*                                   */
/* This program encodes a file using */
/* the huffman algorithm.            */
/*************************************/

#include <stdio.h>
#include <stdlib.h>
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

/**************************************************/
/* Inserts nodes in a list sorted by symbol value */
/* in -- struct Node pointer to a pointer to the  */
/*       head of the list                         */
/* out -- struct Node pointer to the head of the  */
/*        list sorted by symbol values            */
/**************************************************/
void sortSymbolValue(struct Node** head, struct Node* newNode)
{
  unsigned char symbol = newNode->symbol;
  struct Node** current = head;

  while(*current != NULL && (*current)->symbol < symbol)
  {
    current = &((*current)->next);
  }
  newNode->next = *current;
  *current = newNode;
}

/***************************************************/
/* Prints a character; If the character is not     */
/* printable, the function prints its ASCII value. */
/* in -- unsigned character to be printed          */
/* return -- void, does not return                    */
/***************************************************/
void printChar(unsigned char character)
{
  if(character > ' ' && character <= '~')
  { 
    printf("%c", character);
  } 
  else
  {
    printf("=%u", character);
  }  
}

/*************************************************/
/* Prints the array of bits that represnet a     */
/* single character's code.                      */
/* in -- int array holding bit values            */
/* return -- void, does not return anything         */
/*************************************************/ 
void printArray(int array[], int treeLevel)
{
  int i;
  for(i = 0; i < treeLevel; ++i)
  {
    printf("%d", array[i]);
  }
  printf("\n");
}

/*************************************************************/
/* Extracts codes from Huffman tree. Assigns zero to left    */
/* branches, and 1 to right branches. The function is then   */
/* called recursively until it reches a leaf. When it reaches*/
/* a leaf, it stores the code for the leaf's character in    */
/* an array, and inserts the leaf in a list sorted by        */
/* character value.                                          */
/* in -- struct node pointer to the root of the tree         */  
/*       code array that stores bit values as integers       */
/*       integer value representing the current depth        */
/*       struct pointer to a pointer to the head of a list   */
/* return -- void                                               */
/*************************************************************/      
void extractCodes(struct Node* root, int codeArray[], 
                  int treeLevel, struct Node** symbolListHead)
{
  int i;
  
  /* adapted from geeksforgeeks.org/huffman-coding-greedy-algo-3 */
  if(root->left)
  {
    codeArray[treeLevel] = 0;
    extractCodes(root->left, codeArray, treeLevel+1, symbolListHead);
  }
  if(root->right)
  {
    codeArray[treeLevel] = 1;
    extractCodes(root->right, codeArray, treeLevel+1, symbolListHead);
  }

  if(isLeaf(root))
  {
    root->codeLength = treeLevel;
    for(i = 0; i < treeLevel; ++i)
    {
      root->codeArray[i] = codeArray[i];
    }
    sortSymbolValue(symbolListHead, root);
  }
}

/********************************************************************/
/* Prints the table of characters, frequencies, and codes to        */
/* standard output.                                                 */
/* in -- struct node pointer to the head of the list                */
/*       unsigned char number of unique characters                  */
/*       unsigned long number of total characters                   */
/* return -- void                                                   */
/********************************************************************/
void printTable(struct Node* symbolListHead, 
                unsigned char uniqueChar, unsigned long totalChar)
{
  struct Node* current = symbolListHead;
  int i, j;

  for(i = 0; i < uniqueChar; i++)
  {
    printChar(current->symbol);
    printf("\t%lu\t", current->frequency);
    for(j = 0; j < current->codeLength; j++)
    {
      printf("%d", current->codeArray[j]);
    }
    printf("\n");
    current = current->next;
  }
  printf("Total chars = %lu\n", totalChar);
}

/***********************************************************/
/* Encodes a file using the Huffman algorithm. Reads bytes */
/* from a file, the looks up each character's code in the  */
/* list. It uses a buffer to write the encodes bytes to    */
/* an output file.                                         */
/* in -- input file                                        */
/* out -- output file                                      */
/***********************************************************/
void encodeFile(FILE* in, FILE* out)
{

  unsigned long frequencyAll[NUM_CHAR] = {0};
  unsigned char characters[NUM_CHAR];
  unsigned long frequency[NUM_CHAR];
  int codesArray[NUM_CHAR];
  unsigned char uniqueChar, charIn, buffer;
  unsigned long totalChar;

  int i, j, newIndex, bufferIndex;

  struct Node* head = NULL;
  struct Node* symbolListHead = NULL;
  struct Node* current;

  uniqueChar = 0;
  totalChar = 0;

  while(fread(&charIn, sizeof(unsigned char), 1, in) == 1)
  {
    if(frequencyAll[charIn] == 0)
    {
      uniqueChar++;
    }
    frequencyAll[charIn]++;
    totalChar++;
  }

  newIndex = 0;
  for(i = 0; i < NUM_CHAR; ++i)
  {
    if(frequencyAll[i] > 0)
    {
      characters[newIndex] = (char)i;
      frequency[newIndex] = frequencyAll[i];
      newIndex++;
    }
  }
  
  /* create linked list sorted by character frequencies */
  for(i = 0; i < newIndex; ++i)
  {
   struct Node* newNode = createNode(characters[i], frequency[i]);
   head = insertSorted(head, newNode);
  }

  /* build the Huffman tree */
  head = buildTree(head);

  /* print symbols, frequencies, and codes */
  printf("Symbol\tFreq\tCode\n");
  extractCodes(head, codesArray, 0, &symbolListHead);
  printTable(symbolListHead, uniqueChar, totalChar);

  /* write header to output file */
  fwrite(&uniqueChar, sizeof(unsigned char), 1, out);
  /* write symbols and frequencies to output file */
  current = symbolListHead;
  for(i = 0; i < uniqueChar; i++)
  {    
    fwrite(&(current->symbol), sizeof(unsigned char), 1, out);
    fwrite(&(current->frequency), sizeof(unsigned long), 1, out);
    current = current->next;
  }
  /* write total number of encoded symbols to output file */
  fwrite(&totalChar, sizeof(unsigned long), 1, out);

  buffer = 0;
  bufferIndex = 0;

  rewind(in);

  while(fread(&charIn, sizeof(unsigned char), 1, in) == 1)
  {
    current = symbolListHead;
    for(i = 0; i < uniqueChar; ++i)
    {
      if(current->symbol == charIn)
      {
	for(j = 0; j < current->codeLength; ++j)
	{
	  buffer = (buffer << 1) | current->codeArray[j];
	  bufferIndex++;

	  if(bufferIndex == 8)
	  {
	    fwrite(&buffer, sizeof(unsigned char), 1, out);
	    bufferIndex = 0;
	    buffer = 0;
	  }
	}
	break;
      }
      current = current->next;
    }
  }
  /* pads the last byte */
  if(bufferIndex > 0)
  {
    buffer <<= (8 - bufferIndex);
    fwrite(&buffer, sizeof(unsigned char), 1, out);
  }
  freeTree(head);
}

/*******************************************************/
/* Main function. Opens input and output files,        */
/* and checks whether the number of command            */
/* line arguments is correct. It calls the             */
/* encodeFile function, then closes the input          */
/* and output files                                    */
/* in -- integer argc number of command line arguments */
/*       character array containing strings of the     */
/*       command line arguments                        */
/* return - 0                                          */
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

  encodeFile(in, out);

  fclose(in);
  fclose(out);
  
  return 0;
}
