#ifndef _BST_H_
#define _BST_H_

#include "fp_defs.h"

/* This BST only allows unique values */
typedef struct bstNode_t *bstNode;

/* There is no BST object, only BST nodes.  Thus, the way you create a 
 * new tree is to pass a pointer to a NULL node to the insert function, which
 * creates the root node.
 * 
 * @param rp Pointer to the root node. If your tree is empty, your root must 
 * be NULL.
 * @param data A pointer to any arbitrary data.
 * @param cmp A pointer to a comparison function with the same symantics as
 * strcmp.
 *
 * @returns 0 on successful insertion, non-zero on failure.
 * */
int bstInsert( bstNode *rp, void *data, cmpFunc cmp );

/* Delete the entire bst 
 *
 * @param n The root node of the tree to delete.
 * @param df A destructor function which will be called to deallocate the data
 * in each node.  Pass NULL if you don't want or need it.
 * */
void deleteBst( bstNode n, deleteFunc df );

void *bstGetData( bstNode n );

/* Find if certain data exists in the tree.  
 *
 * @param n The root of the tree.
 * @param data A pointer to the data object to find.
 * @param cmp A pointer to a comparison function with the same symantics as
 * strcmp.
 *
 * @returns Either the node containing the data, or the node prior to where the 
 * data would be inserted.
 */
bstNode bstFind( bstNode n, void *data, cmpFunc cmp );

/* Perform an action on the data of every node in the tree */
void bstInOrderWalk( bstNode n, actionFunc fp );
void bstInPostfixWalk( bstNode n, actionFunc fp );

#endif
