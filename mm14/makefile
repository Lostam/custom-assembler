program_NAME := assembler
program_SRCS := assembler.c instruction.c main.c operand.c stoa.c symbol.c bst.c symTabFacade.c token.c tokenizer.c \
vector.c
program_OBJS := ${program_SRCS:.c=.o}
clean_list += $(program_OBJS) $(program_NAME) *.ob *.ext *.ent

# C Preprocessor Flags
CPPFLAGS += 
# compiler flags for gcc
CFLAGS += -ansi -Wall -Wextra -pedantic-errors
# some flags to make things easier on ValGrind, add to CFLAGS
VGFLAGS := -g -fno-inline-functions -fno-inline-functions-called-once \
	-fno-optimize-sibling-calls

.PHONY: clean indent

$(program_NAME): $(program_OBJS)
	@ $(LINK.c) $(program_SRCS) -o $@

Test: $(program_NAME)
	./$(program_NAME) ps.as ex1

clean:
	@- $(RM) $(clean_list)

indent_SRCS := $(program_SRCS)
indent:
	@ indent -linux -brf -prs $(indent_SRCS)

# Generate dependencies for all files in project
%.d: $(program_SRCS)
	@ $(CC) $(CPPFLAGS) -MM $*.c | sed -e 's@^\(.*\)\.o:@\1.d \1.o:@' > $@

clean_list += ${program_SRCS:.c=.d}

# symTabFacade test
symTabFacadeTest_SRCS := symTabFacadeTest.c symTabFacade.c symbol.c vector.c bst.c
symTabFacadeTest_OBJS := ${symTabFacadeTest_SRCS:.c=.o}
clean_list += symTabFacadeTest $(symTabFacadeTest_OBJS)
symTabFacadeTest: $(symTabFacadeTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(symTabFacadeTest_SRCS)
	@ $(LINK.c) $(symTabFacadeTest_OBJS) -o $@
	./$@

# vector test
vectorTest_SRCS := vectorTest.c vector.c symbol.c
vectorTest_OBJS := ${vectorTest_SRCS:.c=.o}
clean_list += vectorTest $(vectorTest_OBJS)
vectorTest: $(vectorTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(vectorTest_SRCS)
	@ $(LINK.c) $(vectorTest_OBJS) -o $@
	./$@

# bst test
bstTest_SRCS := bstTest.c bst.c symbol.c
bstTest_OBJS := ${bstTest_SRCS:.c=.o}
clean_list += bstTest $(bstTest_OBJS)
bstTest: $(bstTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(bstTest_SRCS)
	@ $(LINK.c) $(bstTest_OBJS) -o $@
	./$@

# operand test
operandTest_SRCS := operandTest.c operand.c stoa.c symTabFacadeMock.c
operandTest_OBJS := ${operandTest_SRCS:.c=.o}
clean_list += operandTest $(operandTest_OBJS)
operandTest: $(operandTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(operandTest_SRCS)
	@ $(LINK.c) $(operandTest_OBJS) -o $@
	./$@

# instruction test
instructionTest_SRCS := instructionTest.c instruction.c operand.c stoa.c symTabFacadeMock.c
instructionTest_OBJS := ${instructionTest_SRCS:.c=.o}
clean_list += instructionTest $(instructionTest_OBJS)
instructionTest: $(instructionTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(instructionTest_SRCS)
	@ $(LINK.c) $(instructionTest_OBJS) -o $@
	./$@

# tokenizer test
tokenizerTest_SRCS := tokenizerTest.c tokenizer.c token.c
tokenizerTest_OBJS := ${tokenizerTest_SRCS:.c=.o}
clean_list += tokenizerTest $(tokenizerTest_OBJS)
tokenizerTest: $(tokenizerTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(tokenizerTest_SRCS)
	@ $(LINK.c) $(tokenizerTest_OBJS) -o $@
	./$@

# stoa test
stoaTest_SRCS := stoaTest.c stoa.c
stoaTest_OBJS := ${stoaTest_SRCS:.c=.o}
clean_list += stoaTest $(stoaTest_OBJS)
stoaTest: $(stoaTest_OBJS)
	@ splint -weak -fcnuse +matchanyintegral $(stoaTest_SRCS)
	@ $(LINK.c) $(stoaTest_OBJS) -o $@
	./$@

# Include the list of dependancies generated for each object file
# unless make was called with target clean
ifneq "$(MAKECMDGOALS)" "clean"
-include ${program_SRCS:.c=.d}
endif
