#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "operand.h"
#include "stoa.h"

typedef enum {
	mov = 0, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red,
	prn, jsr, rts, hlt, cmdNotFound = -1
} instructionType;

struct instruction_t {
	instructionType it;
	operand src, dst;
	unsigned short offset;
};

// todo :: maybe use it
struct cmdValidOpTypes {
	unsigned char src;
	unsigned char dst;
} cmdValidOpTypesTab[] = {
	/* mov */  {
		( unsigned char )( 0x1 << OpLiteral ) | ( 0x1 << OpDirect ) |
		    ( 0x1 << OpRelative ) | ( 0x1 << Op2D ) | ( 0x1 <<
								OpRegister ),
		    ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 << OpRelative )
		    | ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* cmp */  {
		( unsigned char )( 0x1 << OpLiteral ) | ( 0x1 << OpDirect ) |
		    ( 0x1 << OpRelative ) | ( 0x1 << Op2D ) | ( 0x1 <<
								OpRegister ),
		    ( unsigned char )( 0x1 << OpLiteral ) | ( 0x1 << OpDirect )
		    | ( 0x1 << OpRelative ) | ( 0x1 << Op2D ) | ( 0x1 <<
								  OpRegister )
	},
	    /* add */  {
		( unsigned char )( 0x1 << OpLiteral ) | ( 0x1 << OpDirect ) |
		    ( 0x1 << OpRelative ) | ( 0x1 << Op2D ) | ( 0x1 <<
								OpRegister ),
		    ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 << OpRelative )
		    | ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* sub */  {
		( unsigned char )( 0x1 << OpLiteral ) | ( 0x1 << OpDirect ) |
		    ( 0x1 << OpRelative ) | ( 0x1 << Op2D ) | ( 0x1 <<
								OpRegister ),
		    ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 << OpRelative )
		    | ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* not */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* clr */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* lea */  {
		( unsigned char )( 0x1 << OpDirect ) | ( 0x1 << OpRelative ) |
		    ( 0x1 << Op2D ),
		    ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 << OpRelative )
		    | ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* inc */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* dec */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* jmp */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* bne */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* red */  {
		0x0, ( unsigned char )( 0x1 << OpDirect ) | ( 0x1 <<
							      OpRelative ) |
		    ( 0x1 << Op2D ) | ( 0x1 << OpRegister )
	},
	    /* prn */  {
		0x0, ( unsigned char )( 0x1 << OpLiteral ) | ( 0x1 << OpDirect )
		    | ( 0x1 << OpRelative ) | ( 0x1 << Op2D ) | ( 0x1 <<
								  OpRegister )
	},
	    /* jsr */  {
		0x0, ( unsigned char )( 0x1 << OpDirect )
	},
	    /* rts */  {
	0x0, 0x0},
	    /* hlt */  {
0x0, 0x0},};

struct cmd {
	const char *cmd;
	instructionType it;
} cmdTab[] = {
	/* In alphabetical order */
	{
	"add", add}, {
	"bne", bne}, {
	"clr", clr}, {
	"cmp", cmp}, {
	"dec", dec}, {
	"hlt", hlt}, {
	"inc", inc}, {
	"jmp", jmp}, {
	"jsr", jsr}, {
	"lea", lea}, {
	"mov", mov}, {
	"not", not}, {
	"prn", prn}, {
	"red", red}, {
	"rts", rts}, {
"sub", sub},};

int instLookup( const char *cmd ) {
	int i;

	for ( i = 0; i != ( sizeof( cmdTab ) / sizeof( cmdTab[0] ) ); i++ )
		if ( strcmp( cmdTab[i].cmd, cmd ) == 0 )
			return cmdTab[i].it;

	return cmdNotFound;
}

instruction newInstruction( const char *cmd, unsigned short IC ) {
	instructionType it = instLookup( cmd );

	if ( it != cmdNotFound ) {
		instruction inst = malloc( sizeof( *inst ) );

		inst->offset = IC;
		inst->it = it;
		inst->src = NULL;
		inst->dst = NULL;

		return inst;
	}

	return NULL;
}

void deleteInstruction( instruction inst ) {
	if ( inst->src != NULL )
		deleteOp( inst->src );
	if ( inst->dst != NULL )
		deleteOp( inst->dst );

	free( inst );
}

/* return 0 if valid 1 if invalid */
static int notValidSrcOpType( instruction inst, opType type ) {
	return !( cmdValidOpTypesTab[inst->it].src & ( 0x1 << type ) );
}

/* return 0 if valid 1 if invalid */
static int notValidDstOpType( instruction inst, opType type ) {
	return !( cmdValidOpTypesTab[inst->it].dst & ( 0x1 << type ) );
}

/* Adds operand to instruction and checks validity of operand type of the
 * instruction.
 * @returns 0 on success, non-zero on error.
 */
int addSrcOp( instruction inst, const char *opString ) {
	operand op = newOp( opString );

	if ( op == NULL || notValidSrcOpType( inst, getOpType( op ) ) ) {
		deleteOp( op );
		return -1;
	}

	inst->src = op;
	return 0;
}

int addDstOp( instruction inst, const char *opString ) {
	operand op = newOp( opString );

	if ( op == NULL || notValidDstOpType( inst, getOpType( op ) ) ) {
		deleteOp( op );
		return -1;
	}

	inst->dst = op;
	return 0;
}

/* calculates how many machine words the instruction and it's operand take up 
 */
int getInstSize( instruction inst ) {
	return 1 + ( inst->src !=
		     NULL ? getOpSize( inst->src ) : 0 ) + ( inst->dst !=
							     NULL ?
							     getOpSize
							     ( inst->dst ) :
							     0 );
}

/* Causes the instruction to lookup and resolve it's operands via the symTab.
 * Should only be called after the whole file has been read and all symbols
 * added to the symbol table.
 *
 * @returns 0 on success, non-zero on non-existant symbol.
 */
int resolveInstruction( instruction inst, symTabFacade stf ) {
	if ( inst->src != NULL )
		if ( resolveOp( inst->src, stf, inst->offset, 1 ) )
			return -1;

	if ( inst->dst != NULL )
		if ( resolveOp( inst->dst, stf, inst->offset,
				( unsigned short )( 1 +
						    ( inst->src !=
						      NULL ?
						      getOpSize( inst->src ) :
						      0 ) ) ) )
			return -1;

	return 0;
}

/* Outputs the instruction and it's operands in base 12 format to the given
 * buffer. Should only be called after calling resolve.
 */
int printInstruction( instruction inst, char *buf ) {
	unsigned short cmd = 0x0;
	int len = 0;

	/* first lets build up the instruction one piece at a time */
	cmd |= inst->it << 12;
	/* check if the source register is in use */
	if ( inst->src != NULL ) {
		cmd |= getOpType( inst->src ) << 9;
		if ( getOpType( inst->src ) == Op2D
		     || getOpType( inst->src ) == OpRegister )
			cmd |= getOpRegNum( inst->src ) << 6;
	}
	/* check if the source register is in use */
	if ( inst->dst != NULL ) {
		cmd |= getOpType( inst->dst ) << 3;
		if ( getOpType( inst->dst ) == Op2D
		     || getOpType( inst->dst ) == OpRegister )
			cmd |= getOpRegNum( inst->dst );
	}
	len = ustoa( inst->offset, 12, buf, 81, 4 );
	buf[len++] = ' ';
	len += ustoa( cmd, 12, &buf[len], 81, 8 );
	buf[len++] = ' ';
	buf[len++] = 'A';
	buf[len++] = '\n';
	buf[len] = '\0';

	/* Then print the src val and the dst val */
	if ( inst->src != NULL )
		len += printOp( inst->src, &buf[len], inst->offset + 1 );

	if ( inst->dst != NULL )
		len += printOp( inst->dst, &buf[len],
				( unsigned short )( inst->offset + 1 +
						    ( inst->src !=
						      NULL ?
						      getOpSize( inst->src ) :
						      0 ) ) );

	return 0;
}
