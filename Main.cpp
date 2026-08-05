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
    auto iapIdField = currencyClass.GetField("IAPId").cast<BNM::Field<BNM::Structures::Mono::String *>>();
    auto adField = currencyClass.GetField("AdPlacement").cast<BNM::Field<BNM::Structures::Mono::String *>>();
    if (iapIdField) {
        iapIdField.SetInstance(inst);
        iapIdField.Set(BNM::CreateMonoString("iap_test"));
        auto s = iapIdField.Get();
        LOGI("Test C string: IAPId = %s", s ? s->str().c_str() : "(null)");
    } else {
        LOGI("Test C: IAPId field not found");
    }
    if (adField) {
        adField.SetInstance(inst);
        adField.Set(BNM::CreateMonoString("placement"));
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

    auto getIapId = currencyClass.GetMethod("GetPlatformIAPID", 0).cast<BNM::Method<BNM::Structures::Mono::String *>>();
    if (getIapId) {
        try {
            getIapId.SetInstance(inst);
            auto s = getIapId();
            LOGI("Test B instance: GetPlatformIAPID() = %s", s ? s->str().c_str() : "(null)");
        } catch (...) {
            LOGI("Test B instance: GetPlatformIAPID threw");
        }
    } else {
        LOGI("Test B instance: GetPlatformIAPID not found");
    }

    auto getHash = currencyClass.GetMethod("GetHashCode", 0).cast<BNM::Method<int>>();
    if (getHash) {
        try {
            getHash.SetInstance(inst);
            LOGI("Test B instance: GetHashCode() = %d", getHash());
        } catch (...) {
            LOGI("Test B instance: GetHashCode threw");
        }
    } else {
        LOGI("Test B instance: GetHashCode not found");
    }

    auto inst2 = currencyClass.CreateNewInstance();
    auto equals = currencyClass.GetMethod("Equals", 1).cast<BNM::Method<bool>>();
    if (equals) {
        try {
            equals.SetInstance(inst);
            LOGI("Test B instance+arg: Equals(inst2) = %s", equals(inst2) ? "true" : "false");
        } catch (...) {
            LOGI("Test B instance+arg: Equals threw");
        }
    } else {
        LOGI("Test B instance+arg: Equals not found");
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
