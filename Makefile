#  makefile

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	TARGET = toot
endif

ifeq ($(UNAME_S),Darwin)
	TARGET = tootosx
endif

OBJS   = login.o util.o post.o main.o upload_file.o asprintf.o fetch.o accounts.o
CC     = cc
CFLAGS += -O2 -MD -std=c11 -Wall -lcurl -g -lreadline -ljson-c


$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CFLAGSDEF) $< -o $@

-include *.d

.PHONY: clean
clean:
	rm -f *.d *.o $(TARGET)
