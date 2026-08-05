#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include "BNM.hpp"

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

static int32_t getHashCode_Hook(void *thiz) {
    return 0xDEAD;
}

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
    for (int i = 0; i < 20 && !BNM::IsLoaded(); i++) sleep(1);
    LOGI("BNM-Free load result: %s, loaded: %s", load ? "true" : "false", BNM::IsLoaded() ? "true" : "false");
    if (!BNM::IsLoaded()) return nullptr;

    auto currencyClass = BNM::Class("SYBO.Subway.Core.CommonData", "Currency", BNM::Image("SYBO.Subway.Core.CommonData.dll"));
    LOGI("Currency class: %s", currencyClass.str().c_str());

    auto inst = currencyClass.CreateNewInstance();
    auto inst2 = currencyClass.CreateNewInstance();
    if (!inst || !inst2) return nullptr;

    auto valueField = currencyClass.GetField("Value").cast<BNM::Field<int>>();
    if (valueField) {
        valueField.SetInstance(inst);
        valueField.Set(777);
        LOGI("Setup: Value = %d", valueField.Get());
    }

    auto getHash = currencyClass.GetMethod("GetHashCode", 0).cast<BNM::Method<int>>();
    auto equals = currencyClass.GetMethod("Equals", 1).cast<BNM::Method<bool>>();

    if (getHash) {
        try {
            getHash.SetInstance(inst);
            LOGI("Test E invoke: GetHashCode() = %d", getHash.Invoke());
        } catch (...) {
            LOGI("Test E invoke: threw");
        }
    } else {
        LOGI("Test E invoke: GetHashCode not found");
    }

    if (equals) {
        try {
            equals.SetInstance(inst);
            LOGI("Test E invoke+arg: Equals(inst2) = %s", equals.Invoke(inst2) ? "true" : "false");
        } catch (...) {
            LOGI("Test E invoke+arg: threw");
        }
    } else {
        LOGI("Test E invoke+arg: Equals not found");
    }

    auto info = currencyClass.GetMethod("GetHashCode", 0).GetInfo();
    auto klass = currencyClass.GetClass();
    void *oldMet = nullptr;
    bool hooked = BNM::VirtualHookImpl(currencyClass, info, (void *) getHashCode_Hook, &oldMet);
    LOGI("Test F: VirtualHookImpl = %s, slot = %d, oldMet = %p", hooked ? "true" : "false", info->slot, oldMet);
    if (hooked && getHash) {
        try {
            getHash.SetInstance(inst);
            LOGI("Test F: invoke after hook = %d", getHash.Invoke());
        } catch (...) {
            LOGI("Test F: hooked invoke threw");
        }
        auto hookedVal = ((int32_t (*)(void *)) klass->vtable[info->slot].methodPtr)(inst);
        LOGI("Test F: via vtable hooked = %d", hookedVal);
        void *restoreOld = nullptr;
        BNM::VirtualHookImpl(currencyClass, info, oldMet, &restoreOld);
        try {
            getHash.SetInstance(inst);
            LOGI("Test F: invoke after restore = %d", getHash.Invoke());
        } catch (...) {
            LOGI("Test F: restored invoke threw");
        }
        auto restoredVal = ((int32_t (*)(void *)) klass->vtable[info->slot].methodPtr)(inst);
        LOGI("Test F: via vtable restored = %d", restoredVal);
    }

    LOGI("ALL TESTS DONE");
    return nullptr;
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    jvm = vm;
    pthread_t thread;
    pthread_create(&thread, nullptr, MainThread, nullptr);
    return JNI_VERSION_1_6;
}
