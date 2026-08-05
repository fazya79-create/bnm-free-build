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
        void *handle = dlopen("libil2cpp.so", RTLD_NOW);
        if (handle) {
            load = BNM::Loading::TryLoadByDlfcnHandle(handle);
            break;
        }
        sleep(1);
    }
    LOGI("BNM-Free load result: %s", load ? "true" : "false");
    if (load && BNM::IsLoaded()) {
        LOGI("BNM-Free loaded, testing resolve");
        auto cls = BNM::Class("UnityEngine", "GameObject", BNM::Image("UnityEngine.CoreModule.dll"));
        LOGI("GameObject class: %s", cls.str().c_str());
        auto methods = cls.GetMethods(true);
        LOGI("GameObject methods: %zu", methods.size());
    }
    return nullptr;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    pthread_t thread;
    pthread_create(&thread, nullptr, MainThread, nullptr);
    pthread_detach(thread);
    return JNI_VERSION_1_6;
}
