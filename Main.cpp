#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include "BNM.hpp"

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

BNM::Coroutine::IEnumerator UnityDrivenTest() {
    LOGI("Test J coroutine body: started by Unity");
    co_yield BNM::Coroutine::WaitForSeconds(1.0f);
    LOGI("Test J coroutine body: resumed after 1s wait");
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
        listObj->_items = intArr;
        listObj->_size = 3;
        LOGI("Test I List: Count=%d items=%d,%d,%d", listObj->Count(), (*listObj)[0], (*listObj)[1], (*listObj)[2]);
    } else {
        LOGI("Test I List: create failed");
    }

    auto cameraClass = BNM::Class("UnityEngine", "Camera", BNM::Image("UnityEngine.CoreModule.dll"));
    auto getMain = cameraClass.GetMethod("get_main", 0).cast<BNM::Method<BNM::IL2CPP::Il2CppObject *>>();
    BNM::IL2CPP::Il2CppObject *mainCam = nullptr;
    for (int i = 0; i < 15 && !mainCam; i++) {
        if (!getMain) break;
        try {
            mainCam = getMain();
        } catch (...) {
            LOGI("Test J: get_main threw");
        }
        if (!mainCam) sleep(1);
    }
    LOGI("Test J: Camera.main = %p", (void *) mainCam);
    if (mainCam) {
        auto mbClass = BNM::Class("UnityEngine", "MonoBehaviour", BNM::Image("UnityEngine.CoreModule.dll"));
        auto startCoro = mbClass.GetMethod("StartCoroutine", 1).cast<BNM::Method<BNM::IL2CPP::Il2CppObject *>>();
        if (startCoro) {
            try {
                startCoro.SetInstance(mainCam);
                auto coro = UnityDrivenTest();
                auto unityCoro = coro.Get();
                startCoro(unityCoro);
                LOGI("Test J: StartCoroutine called, waiting for Unity to drive it");
            } catch (...) {
                LOGI("Test J: StartCoroutine threw");
            }
        } else {
            LOGI("Test J: StartCoroutine not found");
        }
    } else {
        LOGI("Test J: no camera, skipped");
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
