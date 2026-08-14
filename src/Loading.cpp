#include "BNM.hpp"
#include <cxxabi.h>

using namespace BNM;
using namespace BNM::Internal::AssemblerUtils;

bool Loading::TryLoadByJNI(JNIEnv *env, jobject context) {
    bool result = false;

    if (!env || Internal::il2cppLibraryHandle || Internal::states.state) return result;

    auto clearException = [env]() -> bool {
        if (!env->ExceptionCheck()) return false;
        env->ExceptionClear();
        return true;
    };

    jobject localContext = nullptr;
    if (context == nullptr) {
        jclass activityThread = env->FindClass(BNM_OBFUSCATE_TMP("android/app/ActivityThread"));
        if (clearException() || !activityThread) {
            BNM_LOG_ERR("TryLoadByJNI: ActivityThread not found");
            return false;
        }
        auto currentMethod = env->GetStaticMethodID(activityThread, BNM_OBFUSCATE_TMP("currentActivityThread"), BNM_OBFUSCATE_TMP("()Landroid/app/ActivityThread;"));
        auto getApplicationMethod = env->GetMethodID(activityThread, BNM_OBFUSCATE_TMP("getApplication"), BNM_OBFUSCATE_TMP("()Landroid/app/Application;"));
        if (clearException() || !currentMethod || !getApplicationMethod) {
            BNM_LOG_ERR("TryLoadByJNI: ActivityThread methods not found");
            env->DeleteLocalRef(activityThread);
            return false;
        }
        auto currentActivityThread = env->CallStaticObjectMethod(activityThread, currentMethod);
        env->DeleteLocalRef(activityThread);
        if (clearException() || !currentActivityThread) {
            BNM_LOG_ERR("TryLoadByJNI: no current ActivityThread");
            if (currentActivityThread) env->DeleteLocalRef(currentActivityThread);
            return false;
        }
        localContext = env->CallObjectMethod(currentActivityThread, getApplicationMethod);
        env->DeleteLocalRef(currentActivityThread);
        if (clearException() || !localContext) {
            BNM_LOG_ERR("TryLoadByJNI: no Application context");
            if (localContext) env->DeleteLocalRef(localContext);
            return false;
        }
        context = localContext;
    }

    struct _LocalRefGuard {
        JNIEnv *env;
        jobject ref;
        ~_LocalRefGuard() { if (ref) env->DeleteLocalRef(ref); }
    } _contextGuard{env, localContext};

    auto contextClass = env->GetObjectClass(context);
    auto getApplicationInfo = contextClass ? env->GetMethodID(contextClass, BNM_OBFUSCATE_TMP("getApplicationInfo"), BNM_OBFUSCATE_TMP("()Landroid/content/pm/ApplicationInfo;")) : nullptr;
    if (contextClass) env->DeleteLocalRef(contextClass);
    if (clearException() || !getApplicationInfo) {
        BNM_LOG_ERR("TryLoadByJNI: getApplicationInfo not found");
        return false;
    }

    auto applicationInfo = env->CallObjectMethod(context, getApplicationInfo);
    if (clearException() || !applicationInfo) {
        BNM_LOG_ERR("TryLoadByJNI: no ApplicationInfo");
        if (applicationInfo) env->DeleteLocalRef(applicationInfo);
        return false;
    }
    auto applicationInfoClass = env->GetObjectClass(applicationInfo);
    if (clearException() || !applicationInfoClass) {
        BNM_LOG_ERR("TryLoadByJNI: no ApplicationInfo class");
        env->DeleteLocalRef(applicationInfo);
        if (applicationInfoClass) env->DeleteLocalRef(applicationInfoClass);
        return false;
    }

    auto flagsField = env->GetFieldID(applicationInfoClass, BNM_OBFUSCATE_TMP("flags"), BNM_OBFUSCATE_TMP("I"));
    if (clearException() || !flagsField) {
        BNM_LOG_ERR("TryLoadByJNI: ApplicationInfo.flags not found");
        env->DeleteLocalRef(applicationInfoClass);
        env->DeleteLocalRef(applicationInfo);
        return false;
    }
    auto flags = env->GetIntField(applicationInfo, flagsField);
    bool isLibrariesExtracted = (flags & 0x10000000) == 0x10000000;

    auto dirField = env->GetFieldID(applicationInfoClass, isLibrariesExtracted ? BNM_OBFUSCATE_TMP("nativeLibraryDir") : BNM_OBFUSCATE_TMP("sourceDir"), BNM_OBFUSCATE_TMP("Ljava/lang/String;"));
    if (clearException() || !dirField) {
        BNM_LOG_ERR("TryLoadByJNI: ApplicationInfo dir field not found");
        env->DeleteLocalRef(applicationInfoClass);
        env->DeleteLocalRef(applicationInfo);
        return false;
    }
    auto jDir = (jstring) env->GetObjectField(applicationInfo, dirField);
    env->DeleteLocalRef(applicationInfoClass);
    env->DeleteLocalRef(applicationInfo);
    if (clearException() || !jDir) {
        BNM_LOG_ERR("TryLoadByJNI: no library dir");
        if (jDir) env->DeleteLocalRef(jDir);
        return false;
    }

    auto rawDir = env->GetStringUTFChars(jDir, nullptr);
    if (clearException() || !rawDir) {
        BNM_LOG_ERR("TryLoadByJNI: GetStringUTFChars failed");
        env->DeleteLocalRef(jDir);
        return false;
    }
    auto cDir = std::string_view(rawDir);

    std::string file = std::string(cDir) + (isLibrariesExtracted ? BNM_OBFUSCATE_TMP("/libil2cpp.so") : BNM_OBFUSCATE_TMP("!/lib/" CURRENT_ARCH "/libil2cpp.so"));
    auto splitBase = cDir.length() >= 8 ? cDir.substr(0, cDir.length() - 8) : cDir;

    auto handle = BNM_dlopen(file.c_str(), RTLD_LAZY);
    if (handle) {
        if (!(result = CheckHandle(handle))) {
            BNM_LOG_ERR("TryLoadByJNI failed for %s", file.c_str());
            BNM_dlclose(handle);
        }
    } else BNM_LOG_ERR("TryLoadByJNI dlopen failed for %s", file.c_str());

    if (!result && !isLibrariesExtracted) {
        file = std::string(splitBase) + BNM_OBFUSCATE_TMP("split_config." CURRENT_ARCH ".apk!/lib/" CURRENT_ARCH "/libil2cpp.so");
        handle = BNM_dlopen(file.c_str(), RTLD_LAZY);
        if (handle) {
            if (!(result = CheckHandle(handle))) {
                BNM_LOG_ERR("TryLoadByJNI split failed for %s", file.c_str());
                BNM_dlclose(handle);
            }
        } else BNM_LOG_ERR("TryLoadByJNI split dlopen failed for %s", file.c_str());
    }

    env->ReleaseStringUTFChars(jDir, rawDir);
    env->DeleteLocalRef(jDir);
    return result;
}

bool Loading::TryLoadByDlfcnHandle(void *handle) {
    return CheckHandle(handle);
}

void Loading::SetMethodFinder(BNM::Loading::MethodFinder finderMethod, void *userData) {
    Internal::currentFinderMethod = finderMethod;
    Internal::currentFinderData = userData;
}

bool Loading::TryLoadByUsersFinder() {
    auto init = Internal::currentFinderMethod(BNM_OBFUSCATE_TMP("il2cpp_init"), Internal::currentFinderData);
    if (!init) return false;

    Internal::BNM_il2cpp_init_origin = ::BasicHook(init, (void *) Internal::BNM_il2cpp_init, Internal::old_BNM_il2cpp_init);

    if (Internal::states.lateInitAllowed) Internal::LateInit(Internal::currentFinderMethod(BNM_OBFUSCATE_TMP("il2cpp_class_from_il2cpp_type"), Internal::currentFinderData));

    return true;
}

void Loading::TrySetupByUsersFinder() {
    return Internal::Load();
}

void Loading::AllowLateInitHook() {
    Internal::states.lateInitAllowed = true;
}

void Internal::LateInit(void *il2cpp_class_from_il2cpp_type_addr) {
    if (!il2cpp_class_from_il2cpp_type_addr) return;

#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    const uint8_t count = 1;
#elif defined(__i386__) || defined(__x86_64__)
    const uint8_t count = 2;
#endif

    auto from_il2cpp_type = FindJump(il2cpp_class_from_il2cpp_type_addr, count);

    BNM_Class$$FromIl2CppType_origin = ::BasicHook(from_il2cpp_type, (void *) BNM_Class$$FromIl2CppType, old_BNM_Class$$FromIl2CppType);
}

const char *Internal::GetExceptionTypeName() {
    auto type = abi::__cxa_current_exception_type();
    return type ? type->name() : "unknown";
}

bool Internal::SetupBNM() {
    auto &api = BNM::Internal::api;

    Internal::ResolveApi();

#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    const uint8_t count = 1;
#elif defined(__i386__) || defined(__x86_64__)
    const uint8_t count = 2;
#endif

    if (api.il2cpp_array_new_specific) {
        auto arrayNew = (BNM_PTR) api.il2cpp_array_new_specific;
        auto first = FindNextJump(arrayNew, count);
        auto init = first ? FindNextJump(first, count) : 0;
        if (init) {
            classInitFunc = (void (*)(IL2CPP::Il2CppClass *)) init;
            BNM_LOG_DEBUG("Class::Init resolved at %p", OffsetInLib((void *) init));
        } else BNM_LOG_WARN("Class::Init not resolved");
    }

    if (api.il2cpp_class_from_type) {
        auto from_type_adr = FindJump((void *) api.il2cpp_class_from_type, count);
        if (from_type_adr) {
            ::BasicHook(from_type_adr, (void *) ClassesManagement::Class$$FromIl2CppType, ClassesManagement::old_Class$$FromIl2CppType);
            BNM_LOG_DEBUG("vm::Class::FromIl2CppType hooked at %p", OffsetInLib(from_type_adr));
        }
    }

    if (api.il2cpp_type_get_class_or_element_class) {
        auto type_get_class_adr = FindJump((void *) api.il2cpp_type_get_class_or_element_class, count);
        if (type_get_class_adr) {
            ::BasicHook(type_get_class_adr, (void *) ClassesManagement::Type$$GetClassOrElementClass, ClassesManagement::old_Type$$GetClassOrElementClass);
            BNM_LOG_DEBUG("vm::Type::GetClassOrElementClass hooked at %p", OffsetInLib(type_get_class_adr));
        }
    }

    if (api.il2cpp_class_from_name) {
        auto first = FindJump((void *) api.il2cpp_class_from_name, count);
        auto from_name_adr = first ? FindJump(first, count) : nullptr;
        if (from_name_adr) {
            ::BasicHook(from_name_adr, (void *) ClassesManagement::Class$$FromName, ClassesManagement::old_Class$$FromName);
            BNM_LOG_DEBUG("vm::Image::ClassFromName hooked at %p", OffsetInLib(from_name_adr));
        }
    }

    if (api.il2cpp_image_get_types) {
        auto get_types_adr = FindJump((void *) api.il2cpp_image_get_types, count);
        if (get_types_adr) {
            ::BasicHook(get_types_adr, (void *) Internal::Image$$GetTypes, Internal::orig_Image$$GetTypes);
            BNM_LOG_DEBUG("vm::Image::GetTypes hooked at %p", OffsetInLib(get_types_adr));
        }
    }

    auto corlib = api.il2cpp_get_corlib ? api.il2cpp_get_corlib() : nullptr;
    if (!corlib) {
        BNM_LOG_ERR("SetupBNM failed: no corlib");
        return false;
    }

    auto runtimeMethodInfoClass = TryGetClassInImage(corlib, "System.Reflection", "RuntimeMethodInfo");
    if (runtimeMethodInfoClass) {
        vmData.RuntimeMethodInfo$$MakeGenericMethod_impl = MethodBase(IterateMethods(Class(runtimeMethodInfoClass), [](IL2CPP::MethodInfo *method) {
            return method->name && strcmp(method->name, "MakeGenericMethod_impl") == 0;
        }));
    }
    if (!vmData.RuntimeMethodInfo$$MakeGenericMethod_impl.IsValid())
        vmData.RuntimeMethodInfo$$MakeGenericMethod_impl = Class("System.Reflection", "MonoMethod", corlib).GetMethod("MakeGenericMethod_impl");

    auto runtimeTypeClass = Class("System", "RuntimeType", corlib);
    auto stringClass = Class("System", "String", corlib);
    auto interlockedClass = Class("System.Threading", "Interlocked", corlib);
    auto objectClass = Class("System", "Object", corlib);

    if (!objectClass) {
        BNM_LOG_ERR("SetupBNM failed: System.Object not found");
        return false;
    }

    vmData.Object = objectClass;
    vmData.RuntimeType$$MakeGenericType = runtimeTypeClass.GetMethod("MakeGenericType", 2);
    vmData.RuntimeType$$MakePointerType = runtimeTypeClass.GetMethod("MakePointerType", 1);
    vmData.RuntimeType$$make_byref_type = runtimeTypeClass.GetMethod("make_byref_type", 0);
    vmData.Type$$GetType = Class("System", "Type", corlib).GetMethod("GetType", 1);
    vmData.Interlocked$$CompareExchange = interlockedClass.GetMethod("CompareExchange", {objectClass, objectClass, objectClass});
    auto stringEmpty = stringClass.GetField("Empty");
    if (stringEmpty) vmData.String$$Empty = (IL2CPP::Il2CppString **) stringEmpty.GetFieldPointer();

    auto objectMethods = objectClass.GetMethods(false);
    for (auto &m : objectMethods) {
        auto info = m.GetInfo();
        if (info && info->name && strcmp(info->name, "Finalize") == 0) {
            finalizerSlot = info->slot;
            break;
        }
    }

    auto image = Image("UnityEngine.CoreModule.dll");
    vmData.UnityEngine$$Object = Class("UnityEngine", "Object", image);

    auto listClass = Class("System.Collections.Generic", "List`1");
    if (listClass) {
        auto cls = listClass._data;
        auto size = sizeof(IL2CPP::Il2CppClass) + cls->vtable_count * sizeof(IL2CPP::VirtualInvokeData);
        auto newListClass = (IL2CPP::Il2CppClass *) BNM_malloc(size);
        memcpy(newListClass, cls, size);
        newListClass->has_finalize = 0;
        newListClass->instance_size = newListClass->actualSize = sizeof(Structures::Mono::List<void *>);
        newListClass->has_cctor = 0;
        newListClass->cctor_started = 0;
        newListClass->cctor_finished_or_no_cctor = 1;
        auto ctor = listClass.GetMethod(constructorName, 0);
        if (ctor && newListClass->methods && newListClass->method_count) {
            auto constructor = ctor.GetInfo();
            auto newMethods = (IL2CPP::MethodInfo **) BNM_malloc(sizeof(IL2CPP::MethodInfo *) * newListClass->method_count);
            memcpy(newMethods, newListClass->methods, sizeof(IL2CPP::MethodInfo *) * newListClass->method_count);
            auto newConstructor = (IL2CPP::MethodInfo *) BNM_malloc(sizeof(IL2CPP::MethodInfo));
            *newConstructor = *constructor;
            newConstructor->methodPointer = (decltype(newConstructor->methodPointer)) EmptyMethod;
            newConstructor->virtualMethodPointer = (decltype(newConstructor->virtualMethodPointer)) EmptyMethod;
            newConstructor->invoker_method = (decltype(newConstructor->invoker_method)) EmptyMethod;
            for (uint16_t i = 0; i < newListClass->method_count; ++i) {
                if (newListClass->methods[i] == constructor) newMethods[i] = newConstructor;
            }
            newListClass->methods = (const IL2CPP::MethodInfo **) newMethods;
        }
        customListTemplateClass = newListClass;
        vmData.System$$List = Class(newListClass);
    }

    BNM_LOG_DEBUG("SetupBNM done");
    return true;
}

int Internal::BNM_il2cpp_init(const char *domain_name) {

    if (states.lateInitAllowed) Unhook(BNM_Class$$FromIl2CppType_origin);

    auto ret = old_BNM_il2cpp_init ? old_BNM_il2cpp_init(domain_name) : 0;

    Unhook(BNM_il2cpp_init_origin);

    try {
        Load();
    } catch (...) {
        BNM_LOG_ERR("BNM_il2cpp_init Load exception: %s", Internal::GetExceptionTypeName());
    }

    return ret;
}

IL2CPP::Il2CppClass *Internal::BNM_Class$$FromIl2CppType(IL2CPP::Il2CppReflectionType *type) {
    auto &api = BNM::Internal::api;

    auto klass = old_BNM_Class$$FromIl2CppType ? old_BNM_Class$$FromIl2CppType(type) : nullptr;

    if (states.state) return klass;

    if (!api.il2cpp_domain_get) {
        api.il2cpp_domain_get = (decltype(api.il2cpp_domain_get)) GetIl2CppMethod(BNM_OBFUSCATE_TMP("il2cpp_domain_get"));
        api.il2cpp_thread_current = (decltype(api.il2cpp_thread_current)) GetIl2CppMethod(BNM_OBFUSCATE_TMP("il2cpp_thread_current"));
    }

    if (!api.il2cpp_domain_get || !api.il2cpp_thread_current) return klass;

    auto domain = api.il2cpp_domain_get();
    auto thread = api.il2cpp_thread_current();

    if (!domain || !thread || !thread->internal_thread || (void *) domain->default_context != (void *) thread->internal_thread->current_appcontext) return klass;

    Unhook(BNM_il2cpp_init_origin);
    Unhook(BNM_Class$$FromIl2CppType_origin);

    try {
        Load();
    } catch (...) {
        BNM_LOG_ERR("BNM_Class$$FromIl2CppType Load exception: %s", Internal::GetExceptionTypeName());
    }

    return klass;
}

void Internal::Load() {
    static std::atomic_flag loadStarted = ATOMIC_FLAG_INIT;
    if (loadStarted.test_and_set(std::memory_order_acq_rel)) return;

    if (!SetupBNM()) {
        BNM_LOG_ERR("Load aborted: SetupBNM failed");
        return;
    }
    BNM_LOG_DEBUG("Load stage: SetupBNM ok");

    LoadDefaults();
    BNM_LOG_DEBUG("Load stage: LoadDefaults ok");

    BNM::Internal::SetupCoroutine();
    BNM_LOG_DEBUG("Load stage: SetupCoroutine ok");

    BNM::ClassesManagement::ProcessCustomClasses();
    BNM_LOG_DEBUG("Load stage: ProcessCustomClasses ok");

    BNM::Internal::LoadCoroutine();
    BNM_LOG_DEBUG("Load stage: LoadCoroutine ok");

    states.state = true;

    auto events = onLoadedEvents;
    for (auto event : events) if (event) event();

    BNM_LOG_INFO("BNM-Free loaded");
}

IL2CPP::Il2CppImage *Internal::TryGetImage(const std::string_view &_name) {
    auto &api = BNM::Internal::api;
    auto &assemblies = GetAllAssemblies();

    for (auto assembly : assemblies) {
        if (!assembly) continue;
        auto currentImage = api.il2cpp_assembly_get_image(assembly);
        if (!CompareImageName(currentImage, _name)) continue;
        return currentImage;
    }

    return nullptr;
}

IL2CPP::Il2CppClass *Internal::TryGetClassInImage(const IL2CPP::Il2CppImage *image, const std::string_view &_namespace, const std::string_view &_name) {
    auto &api = BNM::Internal::api;
    if (!image) return nullptr;

    if (image->nameToClassHashTable == (decltype(image->nameToClassHashTable)) - 0x424e4d) goto NEW_CLASSES;

    if (api.il2cpp_image_get_class_count && api.il2cpp_image_get_class) {
        size_t typeCount = api.il2cpp_image_get_class_count((IL2CPP::Il2CppImage *) image);
        for (size_t i = 0; i < typeCount; ++i) {
            auto cls = api.il2cpp_image_get_class((IL2CPP::Il2CppImage *) image, i);
            if (!cls) continue;
            auto declaring = api.il2cpp_class_get_declaring_type(cls);
            auto name = api.il2cpp_class_get_name(cls);
            if (declaring) continue;
            if ((!api.il2cpp_class_get_flags(cls) || (name && strcmp(name, "<Module>") == 0))) continue;
            auto ns = api.il2cpp_class_get_namespace(cls);
            if (_namespace == (ns ? ns : "") && _name == (name ? name : "")) return cls;
        }
    }

    NEW_CLASSES:
    {
        IL2CPP::Il2CppClass *result = nullptr;
        ClassesManagement::ForEachClassInImage((IL2CPP::Il2CppImage *) image, [&_namespace, &_name, &result](IL2CPP::Il2CppClass *cls) -> bool {
            auto ns = api.il2cpp_class_get_namespace(cls);
            auto name = api.il2cpp_class_get_name(cls);
            if (_namespace != (ns ? ns : "") || _name != (name ? name : "")) return false;
            result = cls;
            return true;
        });
        return result;
    }
}

Class Internal::TryMakeGenericClass(Class genericType, const std::vector<CompileTimeClass> &templateTypes) {
    if (!vmData.RuntimeType$$MakeGenericType.IsValid() || !genericType) return {};
    auto monoType = genericType.GetMonoType();
    if (!monoType) return {};

    auto monoGenericsList = Structures::Mono::Array<MonoType *>::Create(templateTypes.size(), true);
    if (!monoGenericsList) {
        BNM_LOG_ERR("TryMakeGenericClass failed: cannot allocate Type[]");
        return {};
    }
    for (IL2CPP::il2cpp_array_size_t i = 0; i < (IL2CPP::il2cpp_array_size_t) templateTypes.size(); ++i)
        (*monoGenericsList)[i] = templateTypes[i].ToClass().GetMonoType();

    using MakeGenericTypeFn = MonoType *(*)(MonoType *, Structures::Mono::Array<MonoType *> *);
    Class typedGenericType = ((MakeGenericTypeFn) vmData.RuntimeType$$MakeGenericType.GetOffset())(monoType, monoGenericsList);

    return typedGenericType;
}

MethodBase Internal::TryMakeGenericMethod(const MethodBase &genericMethod, const std::vector<CompileTimeClass> &templateTypes) {
    if (!vmData.RuntimeMethodInfo$$MakeGenericMethod_impl.IsValid() || !genericMethod.GetInfo() || !genericMethod.GetInfo()->is_generic) return {};

    auto monoGenericsList = Structures::Mono::Array<MonoType *>::Create(templateTypes.size(), true);
    if (!monoGenericsList) {
        BNM_LOG_ERR("TryMakeGenericMethod failed: cannot allocate Type[]");
        return {};
    }
    for (IL2CPP::il2cpp_array_size_t i = 0; i < (IL2CPP::il2cpp_array_size_t) templateTypes.size(); ++i) (*monoGenericsList)[i] = templateTypes[i].ToClass().GetMonoType();

    IL2CPP::Il2CppReflectionMethod reflectionMethod;
    memset(&reflectionMethod, 0, sizeof(reflectionMethod));
    reflectionMethod.method = genericMethod.GetInfo();
    reflectionMethod.object.klass = vmData.RuntimeMethodInfo$$MakeGenericMethod_impl.GetParentClass()._data;

    using MakeGenericMethodFn = IL2CPP::Il2CppReflectionMethod *(*)(IL2CPP::Il2CppReflectionMethod *, Structures::Mono::Array<MonoType *> *);
    auto typedGenericMethod = ((MakeGenericMethodFn) vmData.RuntimeMethodInfo$$MakeGenericMethod_impl.GetOffset())(&reflectionMethod, monoGenericsList);

    return typedGenericMethod ? MethodBase(typedGenericMethod->method) : MethodBase{};
}

Class Internal::GetPointer(Class target) {
    if (!vmData.RuntimeType$$MakePointerType.IsValid() || !target) return {};
    auto monoType = target.GetMonoType();
    if (!monoType) return {};
    using MakePointerTypeFn = MonoType *(*)(MonoType *);
    return ((MakePointerTypeFn) vmData.RuntimeType$$MakePointerType.GetOffset())(monoType);
}

Class Internal::GetReference(Class target) {
    if (!vmData.RuntimeType$$make_byref_type.IsValid() || !target) return {};
    auto monoType = target.GetMonoType();
    if (!monoType) return {};
    using MakeByrefFn = MonoType *(*)(void *);
    return ((MakeByrefFn) vmData.RuntimeType$$make_byref_type.GetOffset())(monoType);
}
