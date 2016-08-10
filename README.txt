BUILD INSTRUCTIONS:

TIDY UP LATER

make sure you  have java/javac/javah 1.8

build java project
cd out/production/java
run javah -d ../../../../c/include/ connect.NativeClient


compile C code:
cd into c/src
run gcc -I/usr/lib/jvm/java-8-oracle/include -o ../shared/libserver.so -shared server.c -fPIC
