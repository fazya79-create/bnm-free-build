#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include "BNM.hpp"

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

static bool get_IsIAP_Hook(void *thiz) {
    LOGI("get_IsIAP called (hooked), forcing true");
    return true;
}

BNM::Coroutine::IEnumerator TestCoroutine() {
    co_yield BNM::Coroutine::WaitForSeconds(1.0f);
    co_yield BNM::Coroutine::WaitForEndOfFrame();
    co_yield BNM::Coroutine::WaitUntil([]() { return true; });
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
    if (BNM::IsLoaded()) {
        LOGI("BNM-Free loaded, testing resolve");
        auto cls = BNM::Class("UnityEngine", "GameObject", BNM::Image("UnityEngine.CoreModule.dll"));
        LOGI("GameObject class: %s", cls.str().c_str());
        auto methods = cls.GetMethods(true);
        LOGI("GameObject methods: %zu", methods.size());

        auto currency = BNM::Class("SYBO.Subway.Core.CommonData", "Currency", BNM::Image("SYBO.Subway.Core.CommonData.dll"));
        LOGI("Currency class: %s", currency.str().c_str());
        auto isIap = currency.GetMethod("get_IsIAP", 0);
        if (isIap.IsValid()) {
            auto info = isIap.GetInfo();
            LOGI("get_IsIAP offset: %p", (void *) isIap.GetOffset());
            static bool (*orig_get_IsIAP)(void *);
            bool hooked = DobbyHook((void *) info->methodPointer, (dobby_dummy_func_t) get_IsIAP_Hook, (dobby_dummy_func_t *) &orig_get_IsIAP) == 0;
            LOGI("get_IsIAP hooked: %s", hooked ? "true" : "false");
        } else {
            LOGI("get_IsIAP not found");
        }

        LOGI("Testing coroutine");
        auto coro = TestCoroutine();
        auto unityCoro = coro.Get();
        if (unityCoro) {
            LOGI("coroutine Get ok");
            bool m1 = unityCoro->MoveNext();
            auto cur1 = unityCoro->Current();
            LOGI("MoveNext 1: %s, current: %p", m1 ? "true" : "false", (void *) cur1);
            bool m2 = unityCoro->MoveNext();
            LOGI("MoveNext 2: %s", m2 ? "true" : "false");
            bool m3 = unityCoro->MoveNext();
            LOGI("MoveNext 3: %s", m3 ? "true" : "false");
            bool m4 = unityCoro->MoveNext();
            LOGI("MoveNext 4 (should be false): %s", m4 ? "true" : "false");
            unityCoro->Finalize();
            LOGI("coroutine test done");
        } else {
            LOGI("coroutine Get failed");
        }
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
