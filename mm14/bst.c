#include <stdlib.h>
#include <stdio.h>

#include "bst.h"

struct bstNode_t {
	struct bstNode_t *left, *right;
	void *data;
};

void *bstGetData( bstNode n ) {
	return n->data;
}

bstNode newBstNode( void *data ) {
	bstNode n;
	if ( ( n = malloc( sizeof( struct bstNode_t ) ) ) == NULL ) {
		perror( "Fatal error" );
		exit( EXIT_FAILURE );
	}

	n->data = data;
	n->left = NULL;
	n->right = NULL;

	return n;
}

void deleteBstNode( bstNode n, deleteFunc df ) {
	df( n->data );
	free( n );
}

void deleteBst( bstNode n, deleteFunc df ) {
	if ( n->left != NULL )
		deleteBst( n->left, df );
	if ( n->right != NULL )
		deleteBst( n->right, df );
	deleteBstNode( n, df );
}

/* @returns Either the node containing this value or the node in the BST prior
 * to where this value should be inserted. If the BST is empty returns NULL.
 */
/*@null@*/
bstNode bstFind( bstNode n, void *data, cmpFunc cmp ) {
	int cmpVal;

	if ( n == NULL )
		return NULL;

	while ( ( cmpVal = cmp( data, n->data ) ) != 0 ) {
		if ( cmpVal < 0 ) {
			if ( n->left != NULL )
				n = n->left;
			else
				break;
		} else {
			if ( n->right != NULL )
				n = n->right;
			else
				break;
		}
	}

	return n;
}

/*@null@*/
int bstInsert( bstNode * rp, void *data, cmpFunc cmp ) {
	bstNode n;
	int cmpVal;

	if ( *rp == NULL ) {
		*rp = newBstNode( data );
		return 0;
	}

	/* look up the value */
	n = bstFind( *rp, data, cmp );

	/* if it doesn't exist insert it */
	if ( ( cmpVal = cmp( data, n->data ) ) != 0 ) {
		bstNode nNew = newBstNode( data );
		if ( cmpVal < 0 )
			n->left = nNew;
		else
			n->right = nNew;

		return 0;
	}

	/* if the word already exists
	 */
	return -1;
}

void bstInOrderWalk( bstNode n, actionFunc fp ) {
	if ( n->left != NULL )
		bstInOrderWalk( n->left, fp );
	fp( n->data );
	if ( n->right != NULL )
		bstInOrderWalk( n->right, fp );
}

void bstInPostfixWalk( bstNode n, actionFunc fp ) {
	if ( n->left != NULL )
		bstInOrderWalk( n->left, fp );
	if ( n->right != NULL )
		bstInOrderWalk( n->right, fp );
	fp( n->data );
}
