#define MAX_LINE_LEN 80

struct info{
  char info[MAX_LINE_LEN];
  struct info *next;
};

struct macro{
  char name[MAX_LINE_LEN];
  struct info* info;
  struct macro *next;
};

typedef enum commandNames{
	MOV = 0,
	CMP = 1,
	ADD = 2,
	SUB = 3,
	LEA = 4,
	CLR = 5,
	NOT = 6,
	INC = 7,
	DEC = 8,
	JMP = 9,
	BNE = 10,
	JSR = 11,
	RED = 12,
	PRN = 13,
	RTS = 14,
	STOP = 15
}commandNames;
typedef enum opcodes {
	/* First Group */
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 2,

	LEA_OP = 4,
	/* END First Group */

	/* Second Group */
	CLR_OP = 5,
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,

	JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,

	RED_OP = 12,
	PRN_OP = 13,
	/* END Second Group */

	/* Third Group */
	RTS_OP = 14,
	STOP_OP = 15,
	/* END Third Group */

	/* Failed/Error */
	NONE_OP = -1
} opcode;

typedef enum funct {

	ADD_FUNCT = 10,
	SUB_FUNCT = 11,

	CLR_FUNCT = 10,
	NOT_FUNCT = 11,
	INC_FUNCT = 12,
	DEC_FUNCT = 13,


	JMP_FUNCT = 10,
	BNE_FUNCT = 11,
	JSR_FUNCT = 12,

	/** Default (No need/Error) */
	NONE_FUNCT = 0
} funct;

typedef enum addressing_types {
	/** Immediate addressing (0) */
	IMMEDIATE_ADDR = 0,
	/** Direct addressing (1) */
	DIRECT_ADDR = 1,
	/** Relative addressing (2) */
	RELATIVE_ADDR = 2,
	/** Register addressing */
	REGISTER_ADDR = 3,
	/** Failed/Not detected addressing */
	NONE_ADDR = -1
} addressing_type;
typedef enum attributes{
	EXTERNAL = 0,

	CODE = 1,

	DATA = 2,

	ENTRY = 3

}attributes;
typedef enum boolean{
	FL = 0, TR = 1
} boolean;

typedef struct wordCommand{
	unsigned int A:1;
	unsigned int R:1;
	unsigned int E:1;
	unsigned int opcode:4;
} wordCommand;

typedef struct wordInformation{
	unsigned int A:1;
	unsigned int R:1;
	unsigned int E:1;
	unsigned int funct:4;
	unsigned int source_register:4;
	unsigned int source_addressing:2;
	unsigned int destanation_register:4;
	unsigned int destanation_addressing:2;
} wordInformation;

typedef struct wordExtra{
	unsigned int A:1;
	unsigned int R:1;
	unsigned int E:1;
	boolean address[16];
} wordExtra;

typedef struct wordsList{
	int counter;
	wordCommand* wordCommand;
	wordInformation* wordInformation;
	wordExtra* wordExtra;
	struct wordsList* next;
}wordsList;

typedef struct symbolNode{
	char name[MAX_LINE_LEN];
	int base;
	int offset;
	boolean attributes[4];
	struct symbolNode* next;
}symbolNode;

typedef struct command
{
	char * cmd;
	int code;
	int funct;
}command;
