#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>
#include <strings.h>
#include <malloc.h>
#include <fcntl.h>
#include <linux/types.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include <android/log.h>
#include <iostream>
#include <errno.h>

#include "mylog.h"
#include "MySocketApi.h"

#define MYSOCKET_VERSION   "1.0.1"

#define JNI_EXPORT  extern "C" JNIEXPORT

int g_logLevel = DEBUG;

// C�ַ���תJNI�ַ���
jstring JavaStringFromStdString(JNIEnv* jni, const char* data) {
  return jni->NewStringUTF(data);
}

JNI_EXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) { //load *.so call here
  LogOut(DEBUG, "running");

	JNIEnv* env = nullptr;
	jint result = -1;

	//��ȡJNI�汾
	if(vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    LogOut(ERROR, "GetEnv Version failed");
		return result;
	} 
  LogOut(DEBUG, "GetEnv Version is JNI_VERSION_1_6");

	// g_vm = vm; //decode JavaVM

	return JNI_VERSION_1_6;
}

JNI_EXPORT jstring JNICALL Java_com_MySocket_GetVersion(JNIEnv *env, jobject) {
  return JavaStringFromStdString(env, MYSOCKET_VERSION);
}

JNI_EXPORT jlong JNICALL Java_com_MySocket_ConnectServer(JNIEnv *env, jobject, jstring jip, jint jport) {
  const char *ip = env->GetStringUTFChars(jip, 0);
  LogOut(DEBUG, "ip:%s   jport:%d", ip, jport);
  void* client = CreateSocketClient(ip, jport);
  env->ReleaseStringUTFChars(jip, ip);
  LogOut(DEBUG, "ip:%s   jport:%d  client:%x", ip, jport, client);
  return reinterpret_cast<intptr_t>(client);
}

JNI_EXPORT jint JNICALL Java_com_MySocket_DisconnectServer(JNIEnv *env, jobject, jlong nativaClient) {
  return DeleteSocketClient(reinterpret_cast<void*>(nativaClient));
}

JNI_EXPORT jint JNICALL Java_com_MySocket_PullFromServer(JNIEnv *env, jobject, jlong nativaClient, jstring jpath) {
  const char *path = env->GetStringUTFChars(jpath, 0);
  LogOut(DEBUG, "path:%s ", path);
  int ret = PullFromServer(reinterpret_cast<void*>(nativaClient), path);
  env->ReleaseStringUTFChars(jpath, path);
  LogOut(DEBUG, "ip:%s   ret:%d", path, ret);
  return ret;
}

JNI_EXPORT jint JNICALL Java_com_MySocket_SyncFromServer(JNIEnv *env, jobject, jlong nativaClient, jstring jpath) {
  const char *path = env->GetStringUTFChars(jpath, 0);
  LogOut(DEBUG, "path:%s ", path);
  int ret = SyncFromServer(reinterpret_cast<void*>(nativaClient), path);
  env->ReleaseStringUTFChars(jpath, path);
  LogOut(DEBUG, "ip:%s   ret:%d", path, ret);
  return ret;
}