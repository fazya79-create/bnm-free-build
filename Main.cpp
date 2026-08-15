#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include <atomic>
#include <string>
#include <vector>

#include "BNM.hpp"

template struct BNM::Structures::Mono::Dictionary<int, int>;

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

const char *CurrencyName(int type) {
    switch (type) {
        case 1:
            return "Coins";
        case 2:
            return "Keys";
        case 3:
            return "Hoverboards";
        case 4:
            return "HeadStarts";
        case 5:
            return "ScoreBoosters";
        case 6:
            return "EventCoins";
        case 7:
            return "Tickets";
        case 8:
            return "SeasonTokens";
        case 9:
            return "SeasonPoints";
        case 10:
            return "IAP";
        case 11:
            return "GoldenKeys";
        case 12:
            return "CollectionPoints";
        case 13:
            return "Ads";
        case 14:
            return "SkipAdTickets";
        case 15:
            return "PvpTicket";
        case 16:
            return "CrewCaps";
        case 20:
            return "SprayCan";
        case 21:
            return "Boombox";
        case 22:
            return "Hat";
        default:
            return "?";
    }
}

using GetCurrencyFn = int (*)(void *, int);
static GetCurrencyFn orig_GetCurrency = nullptr;
static std::atomic<void *> capturedWallet{nullptr};
static std::atomic<int> hookCallCount{0};

static int Hooked_GetCurrency(void *instance, int type) {
    int result = orig_GetCurrency ? orig_GetCurrency(instance, type) : 0;
    int calls = hookCallCount.fetch_add(1);
    if (calls < 5)
        LOGI("GetCurrency(%s=%d) = %d", CurrencyName(type), type, result);
    if (instance && !capturedWallet.load())
        capturedWallet.store(instance);
    return result;
}

void DumpCurrencies(void *walletInstance) {
    auto dict = *(BNM::Structures::Mono::Dictionary<int, int> **) ((char *) walletInstance + 0x30);
    if (!dict) {
        LOGI("Currencies dictionary: null");
        return;
    }
    LOGI("Currencies dictionary: %p count=%d", (void *) dict, dict->GetSize());
    auto keys = dict->GetKeys();
    auto values = dict->GetValues();
    size_t n = keys.size() < values.size() ? keys.size() : values.size();
    for (size_t i = 0; i < n; ++i) LOGI("  %s(%d) = %d", CurrencyName(keys[i]), keys[i], values[i]);
}

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

void SetupWalletCapture() {
    auto wallet =
        BNM::Class(BNM_OBFUSCATE("SYBO.Subway.Core.ProfileData"), BNM_OBFUSCATE("WalletModel"),
                   BNM::Image(BNM_OBFUSCATE("SYBO.Subway.Core.ProfileData.dll")));
    if (!wallet) {
        LOGI("wallet capture skipped: WalletModel not resolved");
        return;
    }

    auto getCurr = wallet.GetMethod(BNM_OBFUSCATE("GetCurrency"), 1);
    if (!getCurr.IsValid()) {
        LOGI("wallet capture skipped: GetCurrency not found");
        return;
    }

    auto info = getCurr.GetInfo();
    if (!info || !info->methodPointer) {
        LOGI("wallet capture skipped: no methodPointer");
        return;
    }

    ::BasicHook((void *) info->methodPointer, (void *) Hooked_GetCurrency, orig_GetCurrency);
    LOGI("WalletModel.GetCurrency hooked at %p, waiting for game to call it...",
         BNM::OffsetInLib((void *) info->methodPointer));
}

void WaitForWalletAndDump() {
    for (int i = 0; i < 60 && !capturedWallet.load(); i++) sleep(1);

    auto inst = capturedWallet.load();
    if (!inst) {
        LOGI("WalletModel instance not captured (no GetCurrency call within 60s)");
        return;
    }

    LOGI("WalletModel instance captured: %p (hook calls so far: %d)", inst, hookCallCount.load());
    DumpCurrencies(inst);
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
    SetupWalletCapture();

    if (attached)
        BNM::DetachIl2Cpp();

    WaitForWalletAndDump();

    LOGI("TEST DONE");
    return nullptr;
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    jvm = vm;
    pthread_t thread;
    pthread_create(&thread, nullptr, MainThread, nullptr);
    return JNI_VERSION_1_6;
}
