#include <jni.h>
#include <stdio.h>
#include "../include/connect_NativeServer.h"

JNIEXPORT void JNICALL Java_connect_NativeServer_hello(JNIEnv *env, jobject obj)
{
  int i = 0;
  while(1){
    printf("Hello World\n");
    i++;
  }
  return;
}
