#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "operand.h"
#include "stoa.h"

struct operand_t {
	char *var1, *var2;
	opType type;
	unsigned short data[2];
	short num;		/* this could be the register number or literal value */
};

operand newOp( const char *opString ) {
	operand op = malloc( sizeof( *op ) );
	op->var1 = malloc( 31 );
	op->var2 = malloc( 31 );

	if ( strlen( opString ) == 2 && opString[0] == 'r'
	     && isdigit( opString[1] ) ) {
		op->type = OpRegister;
		op->num = atoi( &opString[1] );
		/* printf("%d\n", op->num); */
	} else if ( sscanf( opString, "#%d", ( int * )&op->num ) == 1 ) {
		op->type = OpLiteral;
		/* printf("%d\n", (int)op->num); */
	} else
	    if ( sscanf
		 ( opString, "%30[a-zA-Z0-9][%30[a-zA-Z0-9]][r%d]", op->var1,
		   op->var2, ( int * )&op->num ) == 3 ) {
		op->type = Op2D;
		/* printf("Op2d: %s %s %d\n", op->var1, op->var2, (int)op->num); */
	} else
	    if ( sscanf
		 ( opString, "%30[a-zA-Z0-9][*%30[a-zA-Z0-9]]", op->var1,
		   op->var2 ) == 2 ) {
		op->type = OpRelative;
		/* printf("%s %s\n", op->var1, op->var2); */
	} else if ( sscanf( opString, "%30[a-zA-Z0-9]", op->var1 ) == 1 ) {
		op->type = OpDirect;
		/* printf("%s\n", op->var1); */
	}

	if ( op->type == OpError ) {
		fprintf( stderr, "Invalid operand %s\n", opString );
		deleteOp( op );
		return NULL;
	}

	return op;
}

void deleteOp( operand op ) {
	free( op->var1 );
	free( op->var2 );
	free( op );
}

opType getOpType( operand op ) {
	return op->type;
}

int getOpSize( operand op ) {
	switch ( op->type ) {
	case OpRegister:
		return 0;
	case OpDirect:
	case OpLiteral:
		return 1;
	case OpRelative:
	case Op2D:
		return 2;
	default:
		return -1;
	}
	return -1;
}

/* 
 * The stf needs to know the offset of the operand in the code segment so that
 * it can log it in the extern table if the variable is extern.
 *
 * The resolver has to know the code offset of the containing instruction so
 * that it can calculate relative indexes.
 *
 * @param instOffset The offset of the containing instruction in the code
 * segment
 * @param offset The offset of the operand relative to it's containing
 * instruction.
 * @returns 0 on success, non-zero on failure
 */
int resolveOp( operand op, symTabFacade stf, unsigned short instOffset,
	       unsigned short opOffset ) {
	int labelRet;

	/* resolve dst operand */
	if ( op->type == OpRelative || op->type == Op2D ) {
		labelRet =
		    stfGetLabelAddress( stf, op->var2, instOffset + opOffset );
		if ( labelRet < 0 ) {
			fprintf( stderr, "Undefined label: %s\n", op->var2 );
			return -1;
		}
		op->data[1] = ( unsigned short )labelRet;
		if ( op->type == OpRelative ) {
			if ( op->data[1] == 0 ) {
				fprintf( stderr,
					 "Error: extern label %s can't be used as a relative index\n",
					 op->var2 );
				return -1;
			}
			op->data[1] =
			    ( unsigned short )( ( short )op->data[1] -
						instOffset );
		}
	}

	/* resolve first src operand */
	if ( op->type == OpRelative || op->type == Op2D
	     || op->type == OpDirect ) {
		labelRet =
		    stfGetLabelAddress( stf, op->var1, instOffset + opOffset );
		if ( labelRet < 0 ) {
			fprintf( stderr, "Undefined label: %s\n", op->var1 );
			return -1;
		}
		op->data[0] = ( unsigned short )labelRet;
	}

	return 0;
}

int getOpRegNum( operand op ) {
	return op->num;
}

/* prints offset operand relocatability 
 * buffer must be at least 16 bytes 
 * @param offset The offset of the operand in the code segment.
 */
int printOp( operand op, char *buf, unsigned short offset ) {
	int len = 0;

	if ( op->type == OpRelative || op->type == Op2D
	     || op->type == OpDirect ) {
		/* print var1 */
		/* print out the offset */
		len += ustoa( offset, 12, buf, 5, 4 );
		buf[len++] = ' ';
		len += ustoa( op->data[0], 12, &buf[len], 9, 8 );
		buf[len++] = ' ';
		buf[len++] = op->data[0] == 0 ? 'E' : 'R';
		buf[len++] = '\n';

		if ( op->type == OpRelative || op->type == Op2D ) {
			/* print var2 */
			/* print out the offset */
			len += ustoa( offset + 1, 12, &buf[len], 5, 4 );
			buf[len++] = ' ';
			len +=
			    sstoa( ( short )op->data[1], 12, &buf[len], 9, 8 );
			buf[len++] = ' ';
			/* an address of 0 indicates an external variable */
			buf[len++] =
			    op->type == OpRelative ? 'A' : ( op->data[1] ==
							     0 ? 'E' : 'R' );
			buf[len++] = '\n';
		}
	} else if ( op->type == OpLiteral ) {
		/* literal is mutually exclusive with the above types */
		/* print out the offset */
		len += ustoa( offset, 12, &buf[len], 5, 4 );
		buf[len++] = ' ';
		len += sstoa( op->num, 12, &buf[len], 9, 8 );
		buf[len++] = ' ';
		buf[len++] = 'A';
		buf[len++] = '\n';
	}

	buf[len] = '\0';

	return len;
}
