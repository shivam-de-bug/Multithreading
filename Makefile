CC = gcc
CFLAG = -Wall -pthread
EXEC = q1 q2 q3 q4_1 q4_2
SRCS = q1.c q2.c q3.c q4_1.c q4_2.c
OBJS = $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC): %: %.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAG) -c $<

clean:
	rm -f $(OBJS) $(EXEC)
