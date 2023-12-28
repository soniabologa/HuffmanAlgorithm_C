/*************************************/
/* Sonia Bologa                      */   
/* UNM ID: 101317238                 */
/* Date: 05/05/2023                  */
/*                                   */
/* This file defines functions used  */
/* both to decode and encode using   */
/* the Huffman algorithm.            */
/*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

/***************************************************************/
/* Allocate a new node with given data.                        */
/* in -- a character representing the symbol, an unsigned long */
/*       representing the frequency at which the character     */
/*       occurs in the input file                              */
/* out -- a struct Node                                        */
/***************************************************************/
struct Node* createNode(unsigned char symbol, unsigned long frequency)
{
  struct Node* node = malloc(sizeof(struct Node));
  node->symbol = symbol;
  node->frequency = frequency;
  node->left = NULL;
  node->right = NULL;
  node->next = NULL;

  return node;
}

/**********************************************************/
/* Checks if a Node is a Leaf.                            */
/* in -- pointer to a Node                                */
/* out -- integer value representing either true or false */
/**********************************************************/
int isLeaf(struct Node* node)
{
  if(node->left == NULL && node->right == NULL)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**********************************************************/
/* Insert data at appropriate place in a sorted list,     */
/* return new list head.                                  */
/* in -- a pointer to the head of the list, and a pointer */
/*       to the newNode that will be added the the list   */
/* out -- the head of the list, with the new node added   */
/*        in the appropriate place                        */
/**********************************************************/
struct Node* insertSorted(struct Node* head, struct Node* newNode)
{
  unsigned long frequency = newNode->frequency;
  unsigned char symbol = newNode->symbol;
  struct Node* current;

  if(head == NULL || head->frequency > frequency
     || (head->frequency == frequency && head->symbol > symbol))
  {
    newNode->next = head;
    return newNode;
  }

  current = head;
  while(current->next != NULL
	&& (current->next->frequency < frequency ||
	    (current->next->frequency == frequency 
	    && current->next->symbol <= symbol)))
  {
    current = current->next;
  }

  newNode->next = current->next;
  current->next = newNode;

  return head;
}

/************************************************************/
/* Builds a Huffman tree.                                   */
/* in -- a pointer to a node that is the head of a priority */
/*       queue                                              */
/* out -- top node of the Huffman tree                      */
/************************************************************/
struct Node* buildTree(struct Node* head)
{
  
  while(head && head->next)
  {
    struct Node* left;
    struct Node* right;
    struct Node* top;
    
    left = head;
    head = head->next;
    left->next = NULL;
    right = head;
    head = head->next;
    right->next = NULL;

    top = createNode(left->symbol, left->frequency + right->frequency);
    top->left = left;
    top->right = right;
    head = insertSorted(head, top);
  }
  return head;
}

/***********************************/
/* Frees memory used by a tree.    */
/* in -- pointer to a struct node. */
/* out -- void                     */
/***********************************/
void freeTree(struct Node* root)
{
  if(root == NULL)
  {
    return;
  }
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}



