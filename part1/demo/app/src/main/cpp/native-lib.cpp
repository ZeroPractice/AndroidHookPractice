#include <jni.h>
#include "jwstring.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_org_zero_demo_Candy_CottonCandy(JNIEnv *env, jclass type, jint a, jint b) {

    // TODO
    jwstring result = jwstring::format("%d + %d = %d", a, b, a+ b);

    return env->NewStringUTF(result.c_str());
}

