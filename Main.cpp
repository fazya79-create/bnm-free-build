#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include "BNM.hpp"

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

void *MainThread(void *) {
    bool load = false;
    for (int i = 0; i < 10; i++) {
        JNIEnv *env = nullptr;
        if (jvm && jvm->AttachCurrentThread(&env, nullptr) == JNI_OK && env) {
            load = BNM::Loading::TryLoadByJNI(env, nullptr);
            jvm->DetachCurrentThread();
            if (load) break;
        }
        sleep(1);
    }
    for (int i = 0; i < 20 && !BNM::IsLoaded(); i++) sleep(1);
    LOGI("load=%s loaded=%s", load ? "true" : "false", BNM::IsLoaded() ? "true" : "false");
    if (!BNM::IsLoaded()) return nullptr;
    
    bool attached = BNM::AttachIl2Cpp();

    auto appClass = BNM::Class(BNM_OBFUSCATE("UnityEngine"), BNM_OBFUSCATE("Application"), BNM::Image(BNM_OBFUSCATE("UnityEngine.CoreModule.dll")));
    LOGI("Application class: %s", appClass ? appClass.str().c_str() : "(null)");

    auto getVer = appClass.GetMethod(BNM_OBFUSCATE("get_unityVersion"), 0).cast<BNM::Method<BNM::Structures::Mono::String *>>();
    LOGI("get_unityVersion resolved: %s", getVer.IsValid() ? "true" : "false");
    auto ver = getVer();
    LOGI("Unity version: %s", ver ? ver->str().c_str() : "(null)");

    auto getVer2 = appClass.GetMethod(BNM_OBFUSCATE("get_version"), 0).cast<BNM::Method<BNM::Structures::Mono::String *>>();
    auto ver2 = getVer2();
    LOGI("Application version: %s", ver2 ? ver2->str().c_str() : "(null)");

    LOGI("TEST DONE");

    if (attached) BNM::DetachIl2Cpp();
    return nullptr;
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    jvm = vm;
    pthread_t thread;
    pthread_create(&thread, nullptr, MainThread, nullptr);
    return JNI_VERSION_1_6;
}
