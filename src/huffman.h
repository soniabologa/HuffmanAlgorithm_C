/***************************************/
/* Sonia Bologa                        */
/* UNM ID: 101317238                   */
/* Date: 05/05/2023                    */ 
/*                                     */
/* This header file declares used both */
/* to encode and and decode using the  */
/* Huffman algorithm.                  */ 
/***************************************/

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>

/***************************************************************/
/* Allocate a new node with given data.                        */
/* in -- a character representing the symbol, an unsigned long */
/*       representing the frequency at which the character     */
/*       occurs in the input file                              */
/* out -- a struct Node                                        */
/***************************************************************/
struct Node* createNode(unsigned char symbol, unsigned long frequency);

/**********************************************************/
/* Insert data at appropriate place in a sorted list,     */
/* return new list head.                                  */
/* in -- a pointer to the head of the list, and a pointer */
/*       to the newNode that will be added the the list   */
/* out -- the head of the list, with the new node added   */
/*        in the appropriate place                        */
/**********************************************************/
struct Node* insertSorted(struct Node* head, struct Node* newNode);

/**************************************************************/
/* Builds a Huffman tree.                                     */
/* in -- a pointet to a node that is the head of a priority   */
/*       queue                                                */
/* out -- top node of the Huffman tree                        */
/***************************************************************/
struct Node* buildTree(struct Node* head);


/**********************************************************/
/* Checks if a Node is a Leaf.                            */
/* in -- pointer to a Node                                */
/* out -- integer value representing either true or false */
/**********************************************************/
int isLeaf(struct Node* node);


/***********************************/
/* Frees memory used by a tree.    */
/* in -- pointer to a struct node. */ 
/* out -- void                     */
/***********************************/
void freeTree(struct Node* root);

/**************************************************************/
/* Huffman encode a file.                                     */
/*     Also writes freq/code table to standard output         */
/* in -- File to encode.                                      */
/*       May be binary, so don't assume printable characters. */
/* out -- File where encoded data will be written.            */
/**************************************************************/
void encodeFile(FILE* in, FILE* out);

/***************************************************/
/* Decode a Huffman encoded file.                  */
/* in -- File to decode.                           */
/* out -- File where decoded data will be written. */
/***************************************************/
void decodeFile(FILE* in, FILE* out);

#endif
