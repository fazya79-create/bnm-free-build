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
    for (int i = 0; i < 20 && !BNM::IsLoaded(); i++) sleep(1);
    LOGI("BNM-Free load result: %s, loaded: %s", load ? "true" : "false", BNM::IsLoaded() ? "true" : "false");
    if (!BNM::IsLoaded()) return nullptr;

    auto currencyClass = BNM::Class("SYBO.Subway.Core.CommonData", "Currency", BNM::Image("SYBO.Subway.Core.CommonData.dll"));
    LOGI("Currency class: %s", currencyClass.str().c_str());

    auto inst = currencyClass.CreateNewInstance();
    LOGI("Test instance: %p", (void *) inst);
    if (!inst) return nullptr;

    auto valueField = currencyClass.GetField("Value").cast<BNM::Field<int>>();
    auto typeField = currencyClass.GetField("CurrencyType").cast<BNM::Field<int>>();
    if (valueField) {
        valueField.SetInstance(inst);
        valueField.Set(777);
        LOGI("Test C write: Value = %d", valueField.Get());
        valueField.Set(valueField.Get() + 223);
        LOGI("Test C write2: Value = %d", valueField.Get());
    } else {
        LOGI("Test C: Value field not found");
    }
    if (typeField) {
        typeField.SetInstance(inst);
        typeField.Set(2);
        LOGI("Test C: CurrencyType = %d", typeField.Get());
    }

    auto isTokenType = currencyClass.GetMethod("IsTokenType", 1).cast<BNM::Method<bool>>();
    if (isTokenType) {
        LOGI("Test B static: IsTokenType(0)=%s IsTokenType(1)=%s IsTokenType(2)=%s",
             isTokenType(0) ? "true" : "false",
             isTokenType(1) ? "true" : "false",
             isTokenType(2) ? "true" : "false");
    } else {
        LOGI("Test B static: IsTokenType not found");
    }

    auto toString = currencyClass.GetMethod("ToString", 0).cast<BNM::Method<BNM::Structures::Mono::String *>>();
    if (toString) {
        try {
            toString.SetInstance(inst);
            auto str = toString();
            LOGI("Test B instance: ToString() = %s", str ? str->str().c_str() : "(null)");
        } catch (...) {
            LOGI("Test B instance: ToString threw");
        }
    } else {
        LOGI("Test B instance: ToString not found");
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
