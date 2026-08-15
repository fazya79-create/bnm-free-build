#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include <string>
#include <vector>

#include "BNM.hpp"

template struct BNM::Structures::Mono::Dictionary<int, int>;

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

void TestUnityVersion() {
    auto appClass = BNM::Class(BNM_OBFUSCATE("UnityEngine"), BNM_OBFUSCATE("Application"),
                               BNM::Image(BNM_OBFUSCATE("UnityEngine.CoreModule.dll")));
    LOGI("Application class: %s", appClass ? appClass.str().c_str() : "(null)");

    auto getVer = appClass.GetMethod(BNM_OBFUSCATE("get_unityVersion"), 0)
                      .cast<BNM::Method<BNM::Structures::Mono::String *>>();
    LOGI("get_unityVersion resolved: %s", getVer.IsValid() ? "true" : "false");
    auto ver = getVer();
    LOGI("Unity version: %s", ver ? ver->str().c_str() : "(null)");

    auto getVer2 = appClass.GetMethod(BNM_OBFUSCATE("get_version"), 0)
                       .cast<BNM::Method<BNM::Structures::Mono::String *>>();
    auto ver2 = getVer2();
    LOGI("Application version: %s", ver2 ? ver2->str().c_str() : "(null)");
}

void TestSubwaySurferMetadata() {
    struct Target {
        const char *ns;
        const char *name;
        const char *image;
    };

    Target targets[] = {
        {BNM_OBFUSCATE("SYBO.Subway.Core.CommonData"), BNM_OBFUSCATE("Currency"),
         BNM_OBFUSCATE("SYBO.Subway.Core.CommonData.dll")},
        {BNM_OBFUSCATE("SYBO.Subway.Core.ProfileData"), BNM_OBFUSCATE("WalletModel"),
         BNM_OBFUSCATE("SYBO.Subway.Core.ProfileData.dll")},
        {BNM_OBFUSCATE("SYBO.Subway.Core.GameData"), BNM_OBFUSCATE("SubwayGameData"),
         BNM_OBFUSCATE("GameDataAssembly.dll")},
        {BNM_OBFUSCATE("SYBO.Subway"), BNM_OBFUSCATE("CoinPickup"),
         BNM_OBFUSCATE("Assembly-CSharp.dll")},
    };

    for (auto &t : targets) {
        auto cls = BNM::Class(t.ns, t.name, BNM::Image(t.image));
        LOGI("resolve %s.%s [%s]: %s", t.ns, t.name, t.image, cls ? cls.str().c_str() : "(null)");
    }
}

void TestCurrencyStaticMethod() {
    auto currencyExtensions = BNM::Class(
        BNM_OBFUSCATE("SYBO.Subway.Core.CommonData"), BNM_OBFUSCATE("CurrencyExtensions"),
        BNM::Image(BNM_OBFUSCATE("SYBO.Subway.Core.CommonData.dll")));
    if (!currencyExtensions) {
        LOGI("CurrencyExtensions test skipped: class not resolved");
        return;
    }

    auto isExpirable = currencyExtensions.GetMethod(BNM_OBFUSCATE("IsExpirableCurrency"), 1);
    LOGI("CurrencyExtensions.IsExpirableCurrency resolved: %s",
         isExpirable.IsValid() ? "true" : "false");
    if (!isExpirable.IsValid())
        return;

    struct Probe {
        int value;
        const char *name;
    };
    Probe probes[] = {
        {1, "Coins"}, {2, "Keys"}, {6, "EventCoins"}, {10, "IAP"}, {13, "Ads"}, {20, "SprayCan"},
    };
    for (auto &p : probes) {
        auto result = isExpirable.cast<BNM::Method<bool>>()(p.value);
        LOGI("IsExpirableCurrency(%s=%d) = %s", p.name, p.value, result ? "true" : "false");
    }
}

void TestWalletModelReflection() {
    auto wallet =
        BNM::Class(BNM_OBFUSCATE("SYBO.Subway.Core.ProfileData"), BNM_OBFUSCATE("WalletModel"),
                   BNM::Image(BNM_OBFUSCATE("SYBO.Subway.Core.ProfileData.dll")));
    if (!wallet) {
        LOGI("WalletModel test skipped: class not resolved");
        return;
    }

    auto fields = wallet.GetFields();
    LOGI("WalletModel fields: %zu", fields.size());
    for (auto &f : fields)
        if (auto info = f.GetInfo())
            LOGI("  field: %s", info->name ? info->name : "(null)");

    auto methods = wallet.GetMethods();
    LOGI("WalletModel methods: %zu", methods.size());
    for (auto &m : methods)
        if (auto info = m.GetInfo())
            LOGI("  method: %s(%d)", info->name ? info->name : "(null)", info->parameters_count);
}

void TestGenericDictionaryMetadata() {
    auto dictClass =
        BNM::Class(BNM_OBFUSCATE("System.Collections.Generic"), BNM_OBFUSCATE("Dictionary`2"),
                   BNM::Image(BNM_OBFUSCATE("mscorlib.dll")));
    LOGI("Dictionary`2 class: %s", dictClass ? dictClass.str().c_str() : "(null)");
}

void *MainThread(void *) {
    bool load = false;
    for (int i = 0; i < 10; i++) {
        JNIEnv *env = nullptr;
        if (jvm && jvm->AttachCurrentThread(&env, nullptr) == JNI_OK && env) {
            load = BNM::Loading::TryLoadByJNI(env, nullptr);
            jvm->DetachCurrentThread();
            if (load)
                break;
        }
        sleep(1);
    }
    for (int i = 0; i < 20 && !BNM::IsLoaded(); i++) sleep(1);
    LOGI("load=%s loaded=%s", load ? "true" : "false", BNM::IsLoaded() ? "true" : "false");
    if (!BNM::IsLoaded())
        return nullptr;

    bool attached = BNM::AttachIl2Cpp();

    TestUnityVersion();
    TestSubwaySurferMetadata();
    TestCurrencyStaticMethod();
    TestWalletModelReflection();
    TestGenericDictionaryMetadata();

    LOGI("TEST DONE");

    if (attached)
        BNM::DetachIl2Cpp();
    return nullptr;
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    jvm = vm;
    pthread_t thread;
    pthread_create(&thread, nullptr, MainThread, nullptr);
    return JNI_VERSION_1_6;
}
