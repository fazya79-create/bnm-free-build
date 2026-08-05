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

    auto goClass = BNM::Class("UnityEngine", "GameObject", BNM::Image("UnityEngine.CoreModule.dll"));
    bool isA1 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, currencyClass);
    bool isA2 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, goClass);
    bool isA3 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, (BNM::IL2CPP::Il2CppObject *) inst);
    bool isA4 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, (BNM::IL2CPP::Il2CppClass *) nullptr);
    LOGI("Test G IsA: currency=%s gameobject=%s selfobj=%s nullclass=%s", isA1 ? "true" : "false", isA2 ? "true" : "false", isA3 ? "true" : "false", isA4 ? "true" : "false");

    auto intArr = BNM::Structures::Mono::Array<int>::Create(5);
    if (intArr) {
        for (int i = 0; i < 5; ++i) (*intArr)[i] = i * 10;
        LOGI("Test H Array: size=%llu vals=%d,%d,%d,%d,%d", (unsigned long long) intArr->Size(), (*intArr)[0], (*intArr)[1], (*intArr)[2], (*intArr)[3], (*intArr)[4]);
        auto classArr = currencyClass.NewArray<int>(3);
        if (classArr) {
            (*classArr)[0] = 7;
            (*classArr)[1] = 8;
            LOGI("Test H Class::NewArray: size=%llu vals=%d,%d", (unsigned long long) classArr->Size(), (*classArr)[0], (*classArr)[1]);
        }
    } else {
        LOGI("Test H Array: Create failed");
    }

    auto listClass = BNM::Class("System.Collections.Generic", "List`1", BNM::Image("mscorlib.dll")).GetGeneric({BNM::Defaults::Get<int>()});
    LOGI("Test I List class: %s", listClass.str().c_str());
    auto listObj = (BNM::Structures::Mono::List<int> *) listClass.CreateNewInstance();
    if (listObj) {
        BNM::Structures::Mono::PRIVATE_MonoListData::InitMonoListVTable(listObj);
        listObj->Add(1);
        listObj->Add(2);
        listObj->Add(3);
        LOGI("Test I List: after Add Count=%d items=%d,%d,%d", listObj->GetSize(), listObj->GetItem(0), listObj->GetItem(1), listObj->GetItem(2));
        listObj->Insert(1, 99);
        LOGI("Test I List: after Insert(1,99) Count=%d items=%d,%d,%d,%d", listObj->GetSize(), listObj->GetItem(0), listObj->GetItem(1), listObj->GetItem(2), listObj->GetItem(3));
        listObj->RemoveAt(2);
        LOGI("Test I List: after RemoveAt(2) Count=%d items=%d,%d,%d", listObj->GetSize(), listObj->GetItem(0), listObj->GetItem(1), listObj->GetItem(2));
        LOGI("Test I List: Contains(99)=%s IndexOf(99)=%d", listObj->Contains(99) ? "true" : "false", listObj->IndexOf(99));
        LOGI("Test I List: typed klass = %p", (void *) listObj->obj.klass);
    } else {
        LOGI("Test I List: create failed");
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
