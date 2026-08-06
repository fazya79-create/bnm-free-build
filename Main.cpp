#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>

#include "BNM.hpp"

#define LOG_TAG "BNM-Free"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

JavaVM *jvm;

BNM::Coroutine::IEnumerator FullCoroutineTest() {
    LOGI("H2 coroutine body: started");
    co_yield BNM::Coroutine::WaitForSeconds(0.5f);
    LOGI("H2 coroutine body: after WaitForSeconds");
    co_yield BNM::Coroutine::WaitForEndOfFrame();
    LOGI("H2 coroutine body: after WaitForEndOfFrame");
    co_yield BNM::Coroutine::WaitForFixedUpdate();
    LOGI("H2 coroutine body: after WaitForFixedUpdate");
    co_yield BNM::Coroutine::WaitForSecondsRealtime(0.1f);
    LOGI("H2 coroutine body: after WaitForSecondsRealtime");
    co_yield BNM::Coroutine::WaitUntil([]() { return true; });
    LOGI("H2 coroutine body: after WaitUntil");
    co_yield BNM::Coroutine::WaitWhile([]() { return false; });
    LOGI("H2 coroutine body: after WaitWhile");
}

static int32_t getHashCode_Hook(void *thiz) {
    return 0xDEAD;
}

static bool get_IsIAP_Hook(void *thiz) {
    return true;
}

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
    LOGI("A1 load=%s loaded=%s handle=%p", load ? "true" : "false", BNM::IsLoaded() ? "true" : "false", (void *) BNM::GetIl2CppLibraryHandle());
    if (!BNM::IsLoaded()) return nullptr;

    auto currencyClass = BNM::Class("SYBO.Subway.Core.CommonData", "Currency", BNM::Image("SYBO.Subway.Core.CommonData.dll"));
    auto goClass = BNM::Class("UnityEngine", "GameObject", BNM::Image("UnityEngine.CoreModule.dll"));
    auto strClass = BNM::Class("System", "String", BNM::Image("mscorlib.dll"));
    LOGI("A2 Currency=%s | GameObject=%s", currencyClass.str().c_str(), goClass.str().c_str());
    LOGI("A3 methods=%zu fields=%zu properties=%zu events=%zu", currencyClass.GetMethods(true).size(), currencyClass.GetFields(true).size(), currencyClass.GetProperties(true).size(), currencyClass.GetEvents(true).size());
    LOGI("A4 parent=%s image=%s", currencyClass.GetParent().str().c_str(), currencyClass.GetImage().GetInfo()->name);
    LOGI("A5 default int=%s bool=%s monoBehaviour=%s", BNM::Defaults::Get<int>().ToClass().str().c_str(), BNM::Defaults::Get<bool>().ToClass().str().c_str(), BNM::Defaults::Get<BNM::UnityEngine::MonoBehaviour>().ToClass().str().c_str());

    auto inst = currencyClass.CreateNewInstance();
    auto inst2 = currencyClass.CreateNewInstance();
    LOGI("B1 instances=%p %p", (void *) inst, (void *) inst2);
    if (!inst || !inst2) return nullptr;

    auto valueField = currencyClass.GetField("Value").cast<BNM::Field<int>>();
    auto typeField = currencyClass.GetField("CurrencyType").cast<BNM::Field<int>>();
    auto iapIdField = currencyClass.GetField("IAPId").cast<BNM::Field<BNM::Structures::Mono::String *>>();
    auto adField = currencyClass.GetField("AdPlacement").cast<BNM::Field<BNM::Structures::Mono::String *>>();
    if (valueField) { valueField.SetInstance(inst); valueField.Set(777); }
    if (typeField) { typeField.SetInstance(inst); typeField.Set(2); }
    if (iapIdField) { iapIdField.SetInstance(inst); iapIdField.Set(BNM::CreateMonoString("iap_test")); }
    if (adField) { adField.SetInstance(inst); adField.Set(BNM::CreateMonoString("placement")); }
    LOGI("C1 Value=%d type=%d IAPId=%s ad=%s", valueField ? valueField.Get() : -1, typeField ? typeField.Get() : -1, iapIdField && iapIdField.Get() ? iapIdField.Get()->str().c_str() : "(null)", adField && adField.Get() ? adField.Get()->str().c_str() : "(null)");
    LOGI("C2 valueOffset=%llx valuePtr=%p", (unsigned long long) valueField.GetOffset(), valueField.GetFieldPointer());
    auto staticArrField = currencyClass.GetField("AllCurrencyTypes");
    LOGI("C3 staticField=%s valid=%s", staticArrField ? staticArrField.str().c_str() : "(null)", staticArrField ? "true" : "false");

    auto getHash = currencyClass.GetMethod("GetHashCode", 0).cast<BNM::Method<int>>();
    auto getIapId = currencyClass.GetMethod("GetPlatformIAPID", 0).cast<BNM::Method<BNM::Structures::Mono::String *>>();
    auto isTokenType = currencyClass.GetMethod("IsTokenType", 1).cast<BNM::Method<bool>>();
    auto equals = currencyClass.GetMethod("Equals", 1).cast<BNM::Method<bool>>();
    auto isIap = currencyClass.GetMethod("get_IsIAP", 0);
    getHash.SetInstance(inst);
    getIapId.SetInstance(inst);
    equals.SetInstance(inst);
    LOGI("D1 static IsTokenType(0)=%s (1)=%s (2)=%s", isTokenType(0) ? "true" : "false", isTokenType(1) ? "true" : "false", isTokenType(2) ? "true" : "false");
    auto iapIdStr = getIapId();
    LOGI("D2 instance GetPlatformIAPID=%s", iapIdStr ? iapIdStr->str().c_str() : "(null)");
    LOGI("D3 GetHashCode invoke=%d operator()=%d", getHash.Invoke(), getHash());
    LOGI("D4 Equals(inst2)=%s", equals(inst2) ? "true" : "false");
    LOGI("D5 GetHashCode offset=%llx return=%s parent=%s", (unsigned long long) getHash.GetOffset(), getHash.GetReturnType().str().c_str(), getHash.GetParentClass().str().c_str());
    LOGI("D6 get_IsIAP valid=%s rva=%llx", isIap.IsValid() ? "true" : "false", (unsigned long long) ((BNM_PTR) isIap.GetInfo()->methodPointer - (BNM_PTR) BNM::GetIl2CppLibraryHandle()));

    bool isA1 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, currencyClass);
    bool isA2 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, goClass);
    bool isA3 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, (BNM::IL2CPP::Il2CppObject *) inst);
    bool isA4 = BNM::IsA((BNM::IL2CPP::Il2CppObject *) inst, (BNM::IL2CPP::Il2CppClass *) nullptr);
    LOGI("E1 IsA currency=%s gameobject=%s self=%s null=%s", isA1 ? "true" : "false", isA2 ? "true" : "false", isA3 ? "true" : "false", isA4 ? "true" : "false");

    auto arr = BNM::Structures::Mono::Array<int>::Create(5);
    if (arr) {
        for (int i = 0; i < 5; ++i) (*arr)[i] = i * 10;
        auto avec = arr->ToVector();
        LOGI("F1 Array size=%llu cap=%llu data[2]=%d vector=%zu vec[0]=%d", (unsigned long long) arr->Size(), (unsigned long long) arr->GetCapacity(), arr->GetData()[2], avec.size(), avec[0]);
        auto arr2 = currencyClass.NewArray<int>(3);
        if (arr2) {
            (*arr2)[0] = 7;
            (*arr2)[1] = 8;
            LOGI("F2 NewArray size=%llu vals=%d,%d", (unsigned long long) arr2->Size(), (*arr2)[0], (*arr2)[1]);
        }
    }

    auto list = currencyClass.NewListBNM<int>();
    if (list) {
        list->Add(1);
        list->Add(2);
        list->Add(3);
        list->Insert(1, 99);
        LOGI("G1 Add/Insert size=%d ver=%d items=%d,%d,%d,%d", list->GetSize(), list->GetVersion(), list->GetItem(0), list->GetItem(1), list->GetItem(2), list->GetItem(3));
        list->SetItem(3, 33);
        LOGI("G2 SetItem items=%d,%d,%d,%d", list->GetItem(0), list->GetItem(1), list->GetItem(2), list->GetItem(3));
        LOGI("G3 Contains(99)=%s IndexOf(99)=%d", list->Contains(99) ? "true" : "false", list->IndexOf(99));
        list->RemoveAt(2);
        LOGI("G4 RemoveAt size=%d items=%d,%d,%d", list->GetSize(), list->GetItem(0), list->GetItem(1), list->GetItem(2));
        bool removed = list->Remove(99);
        LOGI("G5 Remove(99)=%s size=%d", removed ? "true" : "false", list->GetSize());
        auto lvec = list->ToVector();
        int sum = 0;
        for (auto &v : *list) sum += v;
        LOGI("G6 ToVector=%zu rangeForSum=%d", lvec.size(), sum);
        auto copy = *list;
        LOGI("G7 copyCtor size=%d", copy.GetSize());
        list->Clear();
        LOGI("G8 Clear size=%d", list->GetSize());
        LOGI("G9 syncRoot=%p klass=%p", list->GetSyncRoot(), (void *) list->obj.klass);
    }

    auto coro = FullCoroutineTest();
    auto unityCoro = coro.Get();
    LOGI("H1 coroutine Get=%p", (void *) unityCoro);
    int step = 0;
    while (unityCoro && unityCoro->MoveNext()) {
        LOGI("H3 step%d current=%p", step++, (void *) unityCoro->Current());
    }
    LOGI("H4 coroutine done steps=%d", step);
    unityCoro->Finalize();
    LOGI("H5 finalized, second Finalize (idempotent)");
    unityCoro->Finalize();

    auto info = getHash.GetInfo();
    void *oldMet = nullptr;
    bool vhooked = BNM::VirtualHookImpl(currencyClass, info, (void *) getHashCode_Hook, &oldMet);
    LOGI("I1 VirtualHookImpl=%s oldMet=%p", vhooked ? "true" : "false", oldMet);
    auto viaVtable = ((int32_t (*)(void *)) currencyClass.GetClass()->vtable[info->slot].methodPtr)(inst);
    LOGI("I2 viaVtable hooked=%d", viaVtable);
    BNM::VirtualHookImpl(currencyClass, info, oldMet, nullptr);
    LOGI("I3 restored viaVtable=%d", ((int32_t (*)(void *)) currencyClass.GetClass()->vtable[info->slot].methodPtr)(inst));
    void *oldMet2 = nullptr;
    bool ihooked = BNM::InvokeHookImpl(info, (void *) getHashCode_Hook, &oldMet2);
    LOGI("I4 InvokeHookImpl=%s", ihooked ? "true" : "false");
    LOGI("I5 invoke after hook=%d", getHash.Invoke());
    BNM::InvokeHookImpl(info, oldMet2, nullptr);
    LOGI("I6 invoke after restore=%d", getHash.Invoke());
    void *oldMet3 = nullptr;
    bool dhooked = DobbyHook((void *) isIap.GetInfo()->methodPointer, (dobby_dummy_func_t) get_IsIAP_Hook, (dobby_dummy_func_t *) &oldMet3) == 0;
    LOGI("I7 DobbyHook get_IsIAP=%s", dhooked ? "true" : "false");
    if (dhooked) DobbyHook((void *) isIap.GetInfo()->methodPointer, (dobby_dummy_func_t) oldMet3, (dobby_dummy_func_t *) &oldMet3);

    auto s1 = BNM::CreateMonoString("hello");
    LOGI("J1 String str=%s hash=%u empty=%s", s1->str().c_str(), s1->GetHash(), s1->IsNullOrEmpty() ? "true" : "false");
    LOGI("J2 String::Empty=%p", (void *) BNM::Structures::Mono::String::Empty());
    auto charClass = BNM::Class("System", "Char", BNM::Image("mscorlib.dll"));
    auto indexOf = strClass.GetMethod("IndexOf", {charClass}).cast<BNM::Method<int>>();
    indexOf.SetInstance((BNM::IL2CPP::Il2CppObject *) s1);
    LOGI("J3 IndexOf('l') invoke=%d operator()=%d", indexOf.Invoke((char) 'l'), indexOf((char) 'l'));
    LOGI("J4 skipped: Convert managed-throw path unstable in this game");

    auto v2a = BNM::Structures::Unity::Vector2(3.f, 4.f);
    LOGI("K1 Vector2 mag=%.2f dot=%.1f lerpX=%.1f data[0]=%.1f", BNM::Structures::Unity::Vector2::Magnitude(v2a), BNM::Structures::Unity::Vector2::Dot(v2a, v2a), BNM::Structures::Unity::Vector2::Lerp(v2a, BNM::Structures::Unity::Vector2(1.f, 1.f), 0.5f).x, v2a[0]);
    auto v3a = BNM::Structures::Unity::Vector3(1.f, 2.f, 3.f);
    auto v3b = BNM::Structures::Unity::Vector3(4.f, 5.f, 6.f);
    auto cross = BNM::Structures::Unity::Vector3::Cross(v3a, v3b);
    LOGI("K2 Vector3 cross=(%.1f,%.1f,%.1f) dot=%.1f", cross.x, cross.y, cross.z, BNM::Structures::Unity::Vector3::Dot(v3a, v3b));
    auto q = BNM::Structures::Unity::Quaternion::FromEuler(0.f, 90.f, 0.f);
    LOGI("K3 Quaternion euler=(%.3f,%.3f,%.3f,%.3f) w=%.2f", q.x, q.y, q.z, q.w, q.w);
    LOGI("K4 Matrix4x4 identity m00=%.0f m15=%.0f", BNM::Structures::Unity::Matrix4x4::identity.m_Data[0], BNM::Structures::Unity::Matrix4x4::identity.m_Data[15]);
    auto rect = BNM::Structures::Unity::Rect(1.f, 2.f, 3.f, 4.f);
    LOGI("K5 Rect x=%.1f y=%.1f w=%.1f h=%.1f", rect.x, rect.y, rect.w, rect.h);

    auto boxed = BNM::Defaults::Box(123);
    LOGI("L1 Box(123)=%p IsValid=%s", boxed, BNM::Defaults::Get<int>().IsValid() ? "true" : "false");

    auto ctc = BNM::CompileTimeClassBuilder("UnityEngine", "GameObject", "UnityEngine.CoreModule.dll").Build();
    LOGI("M1 CompileTimeClass ToClass=%s", ctc.ToClass().str().c_str());
    LOGI("M2 GetExternMethod=%p", BNM::GetExternMethod("UnityEngine.Object::FindObjectFromInstanceID"));
    auto mem = BNM::Allocate(64);
    BNM::Free(mem);
    LOGI("M3 Allocate/Free ok");

    LOGI("N1 Image classes=%zu images=%zu assemblies=%zu", goClass.GetImage().GetClasses().size(), BNM::Image::GetImages().size(), BNM::Internal::GetAllAssemblies().size());
    LOGI("N2 GetClassesCount=%zu", goClass.GetImage().GetClassesCount());

    BNM::Utils::ForwardList<int> fl;
    fl.Add(1);
    fl.Add(2);
    fl.Add(3);
    LOGI("O1 ForwardList empty=%s", fl.IsEmpty() ? "true" : "false");
    fl.Clear();
    LOGI("O2 ForwardList cleared empty=%s", fl.IsEmpty() ? "true" : "false");

    auto camClass = BNM::Class("UnityEngine", "Camera", BNM::Image("UnityEngine.CoreModule.dll"));
    auto getMain = camClass.GetMethod("get_main", 0).cast<BNM::Method<BNM::IL2CPP::Il2CppObject *>>();
    auto mainCam = getMain();
    LOGI("P1 Camera.main=%p", (void *) mainCam);

    LOGI("Z ALL TESTS DONE");
    return nullptr;
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
    jvm = vm;
    pthread_t thread;
    pthread_create(&thread, nullptr, MainThread, nullptr);
    return JNI_VERSION_1_6;
}
