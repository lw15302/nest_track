# CC := gcc
# RM := bin/rm
#
# CFLAGS := -c -std=c99 -Wall -Wextra -pedantic -O2 -Wfloat-equal
# LFLAGS := -std=c99 -O2
#
# all: server
#
# server:
# 	$(CC) $(LFLAGS) $(CFLAGS) -I/usr/lib/jvm/java-8-oracle/include -o shared/libserver.so -shared src/server.c -fPIC

all:
	gcc -I/usr/lib/jvm/java-8-oracle/include -o shared/libserver.so -shared src/NativeClient.c -fPIC
