TARGET = DMTool
OBJECTS = main.o alloc_mem.o read_parameter.o read_dump.o distance_matrix.o static_structure.o time_correlation.o output.o

# CC = icc
# CFLAGS = -qopenmp

CC = gcc
CFLAGS = -fopenmp

LDFLAGS = -lm

${TARGET}: ${OBJECTS}
	${CC} -o ${TARGET} ${CFLAGS} ${OBJECTS} ${LDFLAGS}

${OBJECTS}: variables.h functions.h

.PHONY:clean
clean:
	-rm -rf ${TARGET} ${OBJECTS}
