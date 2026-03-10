CC = gcc
CFLAGS = -Wall -ansi -pedantic

TARGET = assembler
OBJS = assembler.o asm_state.o pass1.o line_reader.o parser.o pass1_directives.o symbols.o ops.o operand.o pass1_instructions.o code_image.o encoder.o data_image.o errors.o mcro.o handle_files.o preassembler_parser.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

assembler.o: assembler.c asm_state.h pass1.h line_reader.h
	$(CC) $(CFLAGS) -c assembler.c

asm_state.o: asm_state.c asm_state.h defs.h
	$(CC) $(CFLAGS) -c asm_state.c

pass1.o: pass1.c pass1.h asm_state.h line_reader.h parser.h
	$(CC) $(CFLAGS) -c pass1.c

line_reader.o: line_reader.c line_reader.h asm_state.h pass1.h
	$(CC) $(CFLAGS) -c line_reader.c

parser.o: parser.c parser.h asm_state.h line_reader.h
	$(CC) $(CFLAGS) -c parser.c

pass1_directives.o: pass1_directives.c pass1_directives.h asm_state.h parser.h
	$(CC) $(CFLAGS) -c pass1_directives.c

symbols.o: symbols.c symbols.h defs.h
	$(CC) $(CFLAGS) -c symbols.c

ops.o: ops.c ops.h defs.h
	$(CC) $(CFLAGS) -c ops.c

operand.o: operand.c operand.h ops.h defs.h parser.h
	$(CC) $(CFLAGS) -c operand.c

pass1_instructions.o: pass1_instructions.c pass1_instructions.h asm_state.h parser.h ops.h operand.h symbols.h
	$(CC) $(CFLAGS) -c pass1_instructions.c

code_image.o: code_image.c code_image.h word.h defs.h
	$(CC) $(CFLAGS) -c code_image.c

encoder.o: encoder.c encoder.h word.h ops.h defs.h
	$(CC) $(CFLAGS) -c encoder.c

data_image.o: data_image.c data_image.h defs.h word.h
	$(CC) $(CFLAGS) -c data_image.c

errors.o: errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c

mcro.o: mcro.c mcro.h errors.h defs.h
	$(CC) $(CFLAGS) -c mcro.c

handle_files.o: defs.h
	$(CC) $(CFLAGS) -c handle_files.c

preassembler_parser.o: preassembler_parser.c preassembler_parser.h ops.h errors.h
	$(CC) $(CFLAGS) -c preassembler_parser.c

clean:
	rm -f $(OBJS) $(TARGET)

