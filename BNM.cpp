#include "BNM.hpp"

#include <cstdio>
#include <cmath>
#include <cxxabi.h>

using namespace BNM;

namespace BNM::Internal {

Il2CppApi api{};
States states{};
void (*classInitFunc)(IL2CPP::Il2CppClass *){};
void *il2cppLibraryHandle{};
void *currentFinderData = &il2cppLibraryHandle;
VMData vmData{};
std::vector<IL2CPP::Il2CppAssembly *> assembliesCache{};
MethodFinder currentFinderMethod = BasicFinder;

void *BNM_il2cpp_init_origin{};
int (*old_BNM_il2cpp_init)(const char *){};
void *BNM_Class$$FromIl2CppType_origin{};
IL2CPP::Il2CppClass *(*old_BNM_Class$$FromIl2CppType)(IL2CPP::Il2CppReflectionType *){};

std::string_view constructorName = ".ctor";
IL2CPP::Il2CppClass *customListTemplateClass{};
std::map<uint32_t, IL2CPP::Il2CppClass *> customListsMap{};
int32_t finalizerSlot = -1;
std::vector<void (*)()> onLoadedEvents{};

void *BasicFinder(const char *name, void *userData) {
    return BNM_dlsym(*(void **) userData, name);
}

void *GetIl2CppMethod(const char *methodName) {
    return currentFinderMethod(methodName, currentFinderData);
}

#define RESOLVE_API(name) api.name = (decltype(api.name)) GetIl2CppMethod(BNM_OBFUSCATE_TMP(#name))

void ResolveApi() {
    auto &api = BNM::Internal::api;
    RESOLVE_API(il2cpp_init);
    RESOLVE_API(il2cpp_init_utf16);
    RESOLVE_API(il2cpp_shutdown);
    RESOLVE_API(il2cpp_set_config_dir);
    RESOLVE_API(il2cpp_set_data_dir);
    RESOLVE_API(il2cpp_set_temp_dir);
    RESOLVE_API(il2cpp_set_commandline_arguments);
    RESOLVE_API(il2cpp_set_commandline_arguments_utf16);
    RESOLVE_API(il2cpp_set_config_utf16);
    RESOLVE_API(il2cpp_set_config);
    RESOLVE_API(il2cpp_set_memory_callbacks);
    RESOLVE_API(il2cpp_get_corlib);
    RESOLVE_API(il2cpp_add_internal_call);
    RESOLVE_API(il2cpp_resolve_icall);
    RESOLVE_API(il2cpp_alloc);
    RESOLVE_API(il2cpp_free);
    RESOLVE_API(il2cpp_array_class_get);
    RESOLVE_API(il2cpp_array_length);
    RESOLVE_API(il2cpp_array_get_byte_length);
    RESOLVE_API(il2cpp_array_new);
    RESOLVE_API(il2cpp_array_new_specific);
    RESOLVE_API(il2cpp_array_new_full);
    RESOLVE_API(il2cpp_bounded_array_class_get);
    RESOLVE_API(il2cpp_array_element_size);
    RESOLVE_API(il2cpp_assembly_get_image);
    RESOLVE_API(il2cpp_class_for_each);
    RESOLVE_API(il2cpp_class_enum_basetype);
    RESOLVE_API(il2cpp_class_is_generic);
    RESOLVE_API(il2cpp_class_is_inflated);
    RESOLVE_API(il2cpp_class_is_assignable_from);
    RESOLVE_API(il2cpp_class_is_subclass_of);
    RESOLVE_API(il2cpp_class_has_parent);
    RESOLVE_API(il2cpp_class_from_il2cpp_type);
    RESOLVE_API(il2cpp_class_from_name);
    RESOLVE_API(il2cpp_class_from_system_type);
    RESOLVE_API(il2cpp_class_get_element_class);
    RESOLVE_API(il2cpp_class_get_events);
    RESOLVE_API(il2cpp_class_get_fields);
    RESOLVE_API(il2cpp_class_get_nested_types);
    RESOLVE_API(il2cpp_class_get_interfaces);
    RESOLVE_API(il2cpp_class_get_properties);
    RESOLVE_API(il2cpp_class_get_property_from_name);
    RESOLVE_API(il2cpp_class_get_field_from_name);
    RESOLVE_API(il2cpp_class_get_methods);
    RESOLVE_API(il2cpp_class_get_method_from_name);
    RESOLVE_API(il2cpp_class_get_name);
    RESOLVE_API(il2cpp_type_get_name_chunked);
    RESOLVE_API(il2cpp_class_get_namespace);
    RESOLVE_API(il2cpp_class_get_parent);
    RESOLVE_API(il2cpp_class_get_declaring_type);
    RESOLVE_API(il2cpp_class_instance_size);
    RESOLVE_API(il2cpp_class_num_fields);
    RESOLVE_API(il2cpp_class_is_valuetype);
    RESOLVE_API(il2cpp_class_value_size);
    RESOLVE_API(il2cpp_class_is_blittable);
    RESOLVE_API(il2cpp_class_get_flags);
    RESOLVE_API(il2cpp_class_is_abstract);
    RESOLVE_API(il2cpp_class_is_interface);
    RESOLVE_API(il2cpp_class_array_element_size);
    RESOLVE_API(il2cpp_class_from_type);
    RESOLVE_API(il2cpp_class_get_type);
    RESOLVE_API(il2cpp_class_get_type_token);
    RESOLVE_API(il2cpp_class_has_attribute);
    RESOLVE_API(il2cpp_class_has_references);
    RESOLVE_API(il2cpp_class_is_enum);
    RESOLVE_API(il2cpp_class_get_image);
    RESOLVE_API(il2cpp_class_get_assemblyname);
    RESOLVE_API(il2cpp_class_get_rank);
    RESOLVE_API(il2cpp_class_get_data_size);
    RESOLVE_API(il2cpp_class_get_static_field_data);
    RESOLVE_API(il2cpp_class_get_bitmap_size);
    RESOLVE_API(il2cpp_class_get_bitmap);
    RESOLVE_API(il2cpp_stats_dump_to_file);
    RESOLVE_API(il2cpp_stats_get_value);
    RESOLVE_API(il2cpp_domain_get);
    RESOLVE_API(il2cpp_domain_assembly_open);
    RESOLVE_API(il2cpp_domain_get_assemblies);
    RESOLVE_API(il2cpp_raise_exception);
    RESOLVE_API(il2cpp_exception_from_name_msg);
    RESOLVE_API(il2cpp_get_exception_argument_null);
    RESOLVE_API(il2cpp_format_exception);
    RESOLVE_API(il2cpp_format_stack_trace);
    RESOLVE_API(il2cpp_unhandled_exception);
    RESOLVE_API(il2cpp_native_stack_trace);
    RESOLVE_API(il2cpp_field_get_flags);
    RESOLVE_API(il2cpp_field_get_name);
    RESOLVE_API(il2cpp_field_get_parent);
    RESOLVE_API(il2cpp_field_get_offset);
    RESOLVE_API(il2cpp_field_get_type);
    RESOLVE_API(il2cpp_field_get_value);
    RESOLVE_API(il2cpp_field_get_value_object);
    RESOLVE_API(il2cpp_field_has_attribute);
    RESOLVE_API(il2cpp_field_set_value);
    RESOLVE_API(il2cpp_field_static_get_value);
    RESOLVE_API(il2cpp_field_static_set_value);
    RESOLVE_API(il2cpp_field_set_value_object);
    RESOLVE_API(il2cpp_field_is_literal);
    RESOLVE_API(il2cpp_gc_collect);
    RESOLVE_API(il2cpp_gc_collect_a_little);
    RESOLVE_API(il2cpp_gc_start_incremental_collection);
    RESOLVE_API(il2cpp_gc_disable);
    RESOLVE_API(il2cpp_gc_enable);
    RESOLVE_API(il2cpp_gc_is_disabled);
    RESOLVE_API(il2cpp_gc_set_mode);
    RESOLVE_API(il2cpp_gc_get_max_time_slice_ns);
    RESOLVE_API(il2cpp_gc_set_max_time_slice_ns);
    RESOLVE_API(il2cpp_gc_is_incremental);
    RESOLVE_API(il2cpp_gc_get_used_size);
    RESOLVE_API(il2cpp_gc_get_heap_size);
    RESOLVE_API(il2cpp_gc_wbarrier_set_field);
    RESOLVE_API(il2cpp_gc_has_strict_wbarriers);
    RESOLVE_API(il2cpp_gc_set_external_allocation_tracker);
    RESOLVE_API(il2cpp_gc_set_external_wbarrier_tracker);
    RESOLVE_API(il2cpp_gc_foreach_heap);
    RESOLVE_API(il2cpp_stop_gc_world);
    RESOLVE_API(il2cpp_start_gc_world);
    RESOLVE_API(il2cpp_gc_alloc_fixed);
    RESOLVE_API(il2cpp_gc_free_fixed);
    RESOLVE_API(il2cpp_gchandle_new);
    RESOLVE_API(il2cpp_gchandle_new_weakref);
    RESOLVE_API(il2cpp_gchandle_get_target);
    RESOLVE_API(il2cpp_gchandle_free);
    RESOLVE_API(il2cpp_gchandle_foreach_get_target);
    RESOLVE_API(il2cpp_object_header_size);
    RESOLVE_API(il2cpp_array_object_header_size);
    RESOLVE_API(il2cpp_offset_of_array_length_in_array_object_header);
    RESOLVE_API(il2cpp_offset_of_array_bounds_in_array_object_header);
    RESOLVE_API(il2cpp_allocation_granularity);
    RESOLVE_API(il2cpp_unity_liveness_allocate_struct);
    RESOLVE_API(il2cpp_unity_liveness_calculation_from_root);
    RESOLVE_API(il2cpp_unity_liveness_calculation_from_statics);
    RESOLVE_API(il2cpp_unity_liveness_finalize);
    RESOLVE_API(il2cpp_unity_liveness_free_struct);
    RESOLVE_API(il2cpp_unity_liveness_calculation_begin);
    RESOLVE_API(il2cpp_unity_liveness_calculation_end);
    RESOLVE_API(il2cpp_method_get_return_type);
    RESOLVE_API(il2cpp_method_get_declaring_type);
    RESOLVE_API(il2cpp_method_get_name);
    RESOLVE_API(il2cpp_method_get_from_reflection);
    RESOLVE_API(il2cpp_method_get_object);
    RESOLVE_API(il2cpp_method_is_generic);
    RESOLVE_API(il2cpp_method_is_inflated);
    RESOLVE_API(il2cpp_method_is_instance);
    RESOLVE_API(il2cpp_method_get_param_count);
    RESOLVE_API(il2cpp_method_get_param);
    RESOLVE_API(il2cpp_method_get_class);
    RESOLVE_API(il2cpp_method_has_attribute);
    RESOLVE_API(il2cpp_method_get_flags);
    RESOLVE_API(il2cpp_method_get_token);
    RESOLVE_API(il2cpp_method_get_param_name);
    RESOLVE_API(il2cpp_property_get_flags);
    RESOLVE_API(il2cpp_property_get_get_method);
    RESOLVE_API(il2cpp_property_get_set_method);
    RESOLVE_API(il2cpp_property_get_name);
    RESOLVE_API(il2cpp_property_get_parent);
    RESOLVE_API(il2cpp_object_get_class);
    RESOLVE_API(il2cpp_object_get_size);
    RESOLVE_API(il2cpp_object_get_virtual_method);
    RESOLVE_API(il2cpp_object_new);
    RESOLVE_API(il2cpp_object_unbox);
    RESOLVE_API(il2cpp_value_box);
    RESOLVE_API(il2cpp_monitor_enter);
    RESOLVE_API(il2cpp_monitor_try_enter);
    RESOLVE_API(il2cpp_monitor_exit);
    RESOLVE_API(il2cpp_monitor_pulse);
    RESOLVE_API(il2cpp_monitor_pulse_all);
    RESOLVE_API(il2cpp_monitor_wait);
    RESOLVE_API(il2cpp_monitor_try_wait);
    RESOLVE_API(il2cpp_runtime_invoke);
    RESOLVE_API(il2cpp_runtime_invoke_convert_args);
    RESOLVE_API(il2cpp_runtime_class_init);
    RESOLVE_API(il2cpp_runtime_object_init);
    RESOLVE_API(il2cpp_runtime_object_init_exception);
    RESOLVE_API(il2cpp_runtime_unhandled_exception_policy_set);
    RESOLVE_API(il2cpp_string_length);
    RESOLVE_API(il2cpp_string_chars);
    RESOLVE_API(il2cpp_string_new);
    RESOLVE_API(il2cpp_string_new_len);
    RESOLVE_API(il2cpp_string_new_utf16);
    RESOLVE_API(il2cpp_string_new_wrapper);
    RESOLVE_API(il2cpp_string_intern);
    RESOLVE_API(il2cpp_string_is_interned);
    RESOLVE_API(il2cpp_thread_current);
    RESOLVE_API(il2cpp_thread_attach);
    RESOLVE_API(il2cpp_thread_detach);
    RESOLVE_API(il2cpp_thread_get_all_attached_threads);
    RESOLVE_API(il2cpp_is_vm_thread);
    RESOLVE_API(il2cpp_current_thread_walk_frame_stack);
    RESOLVE_API(il2cpp_thread_walk_frame_stack);
    RESOLVE_API(il2cpp_current_thread_get_top_frame);
    RESOLVE_API(il2cpp_thread_get_top_frame);
    RESOLVE_API(il2cpp_current_thread_get_frame_at);
    RESOLVE_API(il2cpp_thread_get_frame_at);
    RESOLVE_API(il2cpp_current_thread_get_stack_depth);
    RESOLVE_API(il2cpp_thread_get_stack_depth);
    RESOLVE_API(il2cpp_override_stack_backtrace);
    RESOLVE_API(il2cpp_type_get_object);
    RESOLVE_API(il2cpp_type_get_type);
    RESOLVE_API(il2cpp_type_get_class_or_element_class);
    RESOLVE_API(il2cpp_type_get_name);
    RESOLVE_API(il2cpp_type_is_byref);
    RESOLVE_API(il2cpp_type_get_attrs);
    RESOLVE_API(il2cpp_type_equals);
    RESOLVE_API(il2cpp_type_get_assembly_qualified_name);
    RESOLVE_API(il2cpp_type_is_static);
    RESOLVE_API(il2cpp_type_is_pointer_type);
    RESOLVE_API(il2cpp_image_get_assembly);
    RESOLVE_API(il2cpp_image_get_name);
    RESOLVE_API(il2cpp_image_get_filename);
    RESOLVE_API(il2cpp_image_get_entry_point);
    RESOLVE_API(il2cpp_image_get_class_count);
    RESOLVE_API(il2cpp_image_get_class);
    RESOLVE_API(il2cpp_capture_memory_snapshot);
    RESOLVE_API(il2cpp_free_captured_memory_snapshot);
    RESOLVE_API(il2cpp_set_find_plugin_callback);
    RESOLVE_API(il2cpp_register_log_callback);
    RESOLVE_API(il2cpp_debugger_set_agent_options);
    RESOLVE_API(il2cpp_is_debugger_attached);
    RESOLVE_API(il2cpp_register_debugger_agent_transport);
    RESOLVE_API(il2cpp_debug_get_method_info);
    RESOLVE_API(il2cpp_unity_install_unitytls_interface);
    RESOLVE_API(il2cpp_custom_attrs_from_class);
    RESOLVE_API(il2cpp_custom_attrs_from_method);
    RESOLVE_API(il2cpp_custom_attrs_get_attr);
    RESOLVE_API(il2cpp_custom_attrs_has_attr);
    RESOLVE_API(il2cpp_custom_attrs_construct);
    RESOLVE_API(il2cpp_custom_attrs_free);
    RESOLVE_API(il2cpp_class_set_userdata);
    RESOLVE_API(il2cpp_class_get_userdata_offset);
    RESOLVE_API(il2cpp_set_default_thread_affinity);
}

#undef RESOLVE_API

namespace AssemblerUtils {

static std::string ReverseHexString(const std::string &hex) {
    std::string out{};
    for (size_t i = 0; i < hex.length(); i += 2) out.insert(0, hex.substr(i, 2));
    return out;
}

static BNM_PTR HexStr2Value(const std::string &hex) {
    return strtoull(hex.c_str(), nullptr, 16);
}

#if defined(__ARM_ARCH_7A__)

static bool IsBranchHex(const std::string &hex) {
    BNM_PTR hexW = HexStr2Value(ReverseHexString(hex));
    return (hexW & 0x0A000000) == 0x0A000000;
}

#elif defined(__aarch64__)

static bool IsBranchHex(const std::string &hex) {
    BNM_PTR hexW = HexStr2Value(ReverseHexString(hex));
    return (hexW & 0xFC000000) == 0x14000000 || (hexW & 0xFC000000) == 0x94000000;
}

#elif defined(__i386__) || defined(__x86_64__)

static bool IsBranchHex(const std::string &hex) {
    return hex.size() >= 2 && hex[0] == 'E' && hex[1] == '8';
}

#else
#error "BNM-Free only supports arm64, arm, x86 and x86_64"
#endif

static const char *hexChars = "0123456789ABCDEF";

template<size_t len>
static std::string ReadMemory(BNM_PTR address) {
    char temp[len];
    memset(temp, 0, len);
    std::string ret{};
    if (memcpy(temp, (void *) address, len) == nullptr) return ret;
    ret.resize(len * 2, 0);
    auto buf = (char *) ret.data();
    for (size_t i = 0; i < len; ++i) {
        *buf++ = hexChars[((uint8_t) temp[i]) >> 4];
        *buf++ = hexChars[((uint8_t) temp[i]) & 0x0F];
    }
    return ret;
}

static bool DecodeBranchOrCall(const std::string &hex, BNM_PTR offset, BNM_PTR &outOffset) {
#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    if (!IsBranchHex(hex)) return false;
#if defined(__aarch64__)
    uint8_t add = 0;
#else
    uint8_t add = 8;
#endif
    outOffset = ((int32_t) (((((HexStr2Value(ReverseHexString(hex))) & (((uint32_t) 1 << 24) - 1) << 0) >> 0) << 2) << (32 - 26)) >> (32 - 26)) + offset + add;
#elif defined(__i386__) || defined(__x86_64__)
    if (!IsBranchHex(hex)) return false;
    outOffset = offset + HexStr2Value(ReverseHexString(hex).substr(0, 8)) + 5;
#else
#error "BNM-Free only supports arm64, arm, x86 and x86_64"
    return false;
#endif
    return true;
}

static BNM_PTR FindNextJump(BNM_PTR start, uint8_t index) {
#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    BNM_PTR offset = 0;
    std::string curHex = ReadMemory<4>(start);
    BNM_PTR outOffset = 0;
    bool out = false;
    while (!(out = DecodeBranchOrCall(curHex, start + offset, outOffset)) || index != 1) {
        offset += 4;
        curHex = ReadMemory<4>(start + offset);
        if (out) index--;
    }
    return outOffset;
#elif defined(__i386__) || defined(__x86_64__)
    BNM_PTR offset = 0;
    std::string curHex = ReadMemory<1>(start);
    BNM_PTR outOffset = 0;
    bool out = false;
    while (!(out = IsBranchHex(curHex)) || index != 1) {
        offset += 1;
        curHex = ReadMemory<1>(start + offset);
        if (out) index--;
    }
    DecodeBranchOrCall(ReadMemory<5>(start + offset), start + offset, outOffset);
    return outOffset;
#else
#error "BNM-Free only supports arm64, arm, x86 and x86_64"
    return 0;
#endif
}

}

}

using namespace BNM::Internal::AssemblerUtils;

static void *FindJump(void *start, uint8_t count) {
    if (!start) return nullptr;
    return (void *) FindNextJump((BNM_PTR) start, count);
}

static void EmptyMethod() {}

static void *OffsetInLib(void *offsetInMemory) {
    if (offsetInMemory == nullptr) return nullptr;
    Dl_info info;
    BNM_dladdr(offsetInMemory, &info);
    return (void *) ((BNM_PTR) offsetInMemory - (BNM_PTR) info.dli_fbase);
}

void *Utils::OffsetInLib(void *offsetInMemory) {
    return ::OffsetInLib(offsetInMemory);
}

static bool CheckHandle(void *handle) {
    void *init = BNM_dlsym(handle, BNM_OBFUSCATE_TMP("il2cpp_init"));
    if (!init) return false;

    Internal::BNM_il2cpp_init_origin = ::BasicHook(init, (void *) Internal::BNM_il2cpp_init, Internal::old_BNM_il2cpp_init);

    if (Internal::states.lateInitAllowed) Internal::LateInit(BNM_dlsym(handle, BNM_OBFUSCATE_TMP("il2cpp_class_from_il2cpp_type")));

    Internal::il2cppLibraryHandle = handle;
    return true;
}

#if defined(__ARM_ARCH_7A__)
#define CURRENT_ARCH "armeabi-v7a"
#elif defined(__aarch64__)
#define CURRENT_ARCH "arm64-v8a"
#elif defined(__i386__)
#define CURRENT_ARCH "x86"
#elif defined(__x86_64__)
#define CURRENT_ARCH "x86_64"
#elif defined(__riscv)
#define CURRENT_ARCH "riscv64"
#endif

bool Loading::TryLoadByJNI(JNIEnv *env, jobject context) {
    bool result = false;

    if (!env || Internal::il2cppLibraryHandle || Internal::states.state) return result;

    if (context == nullptr) {
        jclass activityThread = env->FindClass(BNM_OBFUSCATE_TMP("android/app/ActivityThread"));
        auto currentActivityThread = env->CallStaticObjectMethod(activityThread, env->GetStaticMethodID(activityThread, BNM_OBFUSCATE_TMP("currentActivityThread"), BNM_OBFUSCATE_TMP("()Landroid/app/ActivityThread;")));
        context = env->CallObjectMethod(currentActivityThread, env->GetMethodID(activityThread, BNM_OBFUSCATE_TMP("getApplication"), BNM_OBFUSCATE_TMP("()Landroid/app/Application;")));
        env->DeleteLocalRef(currentActivityThread);
    }

    auto applicationInfo = env->CallObjectMethod(context, env->GetMethodID(env->GetObjectClass(context), BNM_OBFUSCATE_TMP("getApplicationInfo"), BNM_OBFUSCATE_TMP("()Landroid/content/pm/ApplicationInfo;")));
    auto applicationInfoClass = env->GetObjectClass(applicationInfo);

    auto flags = env->GetIntField(applicationInfo, env->GetFieldID(applicationInfoClass, BNM_OBFUSCATE_TMP("flags"), BNM_OBFUSCATE_TMP("I")));
    bool isLibrariesExtracted = (flags & 0x10000000) == 0x10000000;

    auto jDir = (jstring) env->GetObjectField(applicationInfo, env->GetFieldID(applicationInfoClass, isLibrariesExtracted ? BNM_OBFUSCATE_TMP("nativeLibraryDir") : BNM_OBFUSCATE_TMP("sourceDir"), BNM_OBFUSCATE_TMP("Ljava/lang/String;")));

    auto cDir = std::string_view(env->GetStringUTFChars(jDir, nullptr));
    env->DeleteLocalRef(applicationInfo);
    env->DeleteLocalRef(applicationInfoClass);

    std::string file = std::string(cDir) + (isLibrariesExtracted ? BNM_OBFUSCATE_TMP("/libil2cpp.so") : BNM_OBFUSCATE_TMP("!/lib/" CURRENT_ARCH "/libil2cpp.so"));
    auto splitBase = cDir.length() >= 8 ? cDir.substr(0, cDir.length() - 8) : cDir;

    auto handle = BNM_dlopen(file.c_str(), RTLD_LAZY);
    if (!(result = CheckHandle(handle))) {
        BNM_LOG_ERR("TryLoadByJNI failed for %s", file.c_str());
    } else goto FINISH;
    if (isLibrariesExtracted) goto FINISH;
    file.clear();

    file = std::string(splitBase) + BNM_OBFUSCATE_TMP("split_config." CURRENT_ARCH ".apk!/lib/" CURRENT_ARCH "/libil2cpp.so");
    handle = BNM_dlopen(file.c_str(), RTLD_LAZY);
    if (!(result = CheckHandle(handle))) BNM_LOG_ERR("TryLoadByJNI split failed for %s", file.c_str());

    FINISH:
    env->ReleaseStringUTFChars(jDir, cDir.data());
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

void Internal::SetupBNM() {
    auto &api = BNM::Internal::api;

    ResolveApi();

#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
    const uint8_t count = 1;
#elif defined(__i386__) || defined(__x86_64__)
    const uint8_t count = 2;
#endif

    if (api.il2cpp_array_new_specific) {
        auto arrayNew = (BNM_PTR) api.il2cpp_array_new_specific;
        auto first = FindNextJump(arrayNew, count);
        auto init = FindNextJump(first, count); (void) init;
        classInitFunc = (void (*)(IL2CPP::Il2CppClass *)) init;
        BNM_LOG_DEBUG("Class::Init resolved at %p", OffsetInLib((void *) init));
    }

    if (api.il2cpp_class_from_type) {
        auto from_type_adr = FindJump((void *) api.il2cpp_class_from_type, count);
        ::BasicHook(from_type_adr, (void *) ClassesManagement::Class$$FromIl2CppType, ClassesManagement::old_Class$$FromIl2CppType);
        BNM_LOG_DEBUG("vm::Class::FromIl2CppType hooked at %p", OffsetInLib(from_type_adr));
    }

    if (api.il2cpp_type_get_class_or_element_class) {
        auto type_get_class_adr = FindJump((void *) api.il2cpp_type_get_class_or_element_class, count);
        ::BasicHook(type_get_class_adr, (void *) ClassesManagement::Type$$GetClassOrElementClass, ClassesManagement::old_Type$$GetClassOrElementClass);
        BNM_LOG_DEBUG("vm::Type::GetClassOrElementClass hooked at %p", OffsetInLib(type_get_class_adr));
    }

    if (api.il2cpp_class_from_name) {
        auto from_name_adr = FindJump(FindJump((void *) api.il2cpp_class_from_name, count), count);
        ::BasicHook(from_name_adr, (void *) ClassesManagement::Class$$FromName, ClassesManagement::old_Class$$FromName);
        BNM_LOG_DEBUG("vm::Image::ClassFromName hooked at %p", OffsetInLib(from_name_adr));
    }

    if (api.il2cpp_image_get_types) {
        auto get_types_adr = FindJump((void *) api.il2cpp_image_get_types, count);
        ::BasicHook(get_types_adr, (void *) Internal::Image$$GetTypes, Internal::orig_Image$$GetTypes);
        BNM_LOG_DEBUG("vm::Image::GetTypes hooked at %p", OffsetInLib(get_types_adr));
    }

    auto corlib = api.il2cpp_get_corlib ? api.il2cpp_get_corlib() : nullptr;
    if (!corlib) {
        BNM_LOG_ERR("SetupBNM failed: no corlib");
        return;
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
        if (info->name && strcmp(info->name, "Finalize") == 0) {
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
        newListClass->instance_size = sizeof(Structures::Mono::List<void *>);
        newListClass->has_cctor = 0;
        newListClass->cctor_started = 0;
        newListClass->cctor_finished_or_no_cctor = 1;
        auto ctor = listClass.GetMethod(constructorName, 0);
        if (ctor) {
            auto constructor = ctor.GetInfo();
            auto newMethods = (IL2CPP::MethodInfo **) BNM_malloc(sizeof(IL2CPP::MethodInfo *) * newListClass->method_count);
            memcpy(newMethods, newListClass->methods, sizeof(IL2CPP::MethodInfo *) * newListClass->method_count);
            auto newConstructor = (IL2CPP::MethodInfo *) BNM_malloc(sizeof(IL2CPP::MethodInfo));
            *newConstructor = *constructor;
            newConstructor->methodPointer = (decltype(newConstructor->methodPointer)) EmptyMethod;
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
}

int Internal::BNM_il2cpp_init(const char *domain_name) {

    if (states.lateInitAllowed) Unhook(BNM_Class$$FromIl2CppType_origin);

    auto ret = old_BNM_il2cpp_init(domain_name);

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

    auto klass = old_BNM_Class$$FromIl2CppType(type);

    if (states.state) return klass;

    if (!api.il2cpp_domain_get) {
        api.il2cpp_domain_get = (decltype(api.il2cpp_domain_get)) GetIl2CppMethod(BNM_OBFUSCATE_TMP("il2cpp_domain_get"));
        api.il2cpp_thread_current = (decltype(api.il2cpp_thread_current)) GetIl2CppMethod(BNM_OBFUSCATE_TMP("il2cpp_thread_current"));
    }

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
    SetupBNM();
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
    if (!vmData.RuntimeType$$MakeGenericType.IsValid()) return {};
    auto monoType = genericType.GetMonoType();
    auto monoGenericsList = Structures::Mono::Array<MonoType *>::Create(templateTypes.size(), true);
    for (IL2CPP::il2cpp_array_size_t i = 0; i < (IL2CPP::il2cpp_array_size_t) templateTypes.size(); ++i)
        (*monoGenericsList)[i] = templateTypes[i].ToClass().GetMonoType();

    using MakeGenericTypeFn = MonoType *(*)(MonoType *, Structures::Mono::Array<MonoType *> *);
    Class typedGenericType = ((MakeGenericTypeFn) vmData.RuntimeType$$MakeGenericType.GetOffset())(monoType, monoGenericsList);

    monoGenericsList->Destroy();

    return typedGenericType;
}

MethodBase Internal::TryMakeGenericMethod(const MethodBase &genericMethod, const std::vector<CompileTimeClass> &templateTypes) {
    if (!vmData.RuntimeMethodInfo$$MakeGenericMethod_impl.IsValid() || !genericMethod.GetInfo()->is_generic) return {};
    IL2CPP::Il2CppReflectionMethod reflectionMethod;
    reflectionMethod.method = genericMethod.GetInfo();

    auto monoGenericsList = Structures::Mono::Array<MonoType *>::Create(templateTypes.size(), true);
    for (IL2CPP::il2cpp_array_size_t i = 0; i < (IL2CPP::il2cpp_array_size_t) templateTypes.size(); ++i) (*monoGenericsList)[i] = templateTypes[i].ToClass().GetMonoType();

    using MakeGenericMethodFn = IL2CPP::Il2CppReflectionMethod *(*)(IL2CPP::Il2CppReflectionMethod *, Structures::Mono::Array<MonoType *> *);
    auto typedGenericMethod = ((MakeGenericMethodFn) vmData.RuntimeMethodInfo$$MakeGenericMethod_impl.GetOffset())(&reflectionMethod, monoGenericsList);

    monoGenericsList->Destroy();

    return typedGenericMethod ? MethodBase(typedGenericMethod->method) : MethodBase{};
}

Class Internal::GetPointer(Class target) {
    if (!vmData.RuntimeType$$MakePointerType.IsValid()) return {};
    using MakePointerTypeFn = MonoType *(*)(MonoType *);
    return ((MakePointerTypeFn) vmData.RuntimeType$$MakePointerType.GetOffset())(target.GetMonoType());
}

Class Internal::GetReference(Class target) {
    if (!vmData.RuntimeType$$make_byref_type.IsValid()) return {};
    using MakeByrefFn = MonoType *(*)(void *);
    return ((MakeByrefFn) vmData.RuntimeType$$make_byref_type.GetOffset())(target.GetMonoType());
}

Class::Class(const IL2CPP::Il2CppObject *object) {
    if (!object) return;
    _data = object->klass;
}

Class::Class(const IL2CPP::Il2CppType *type) {
    if (!type) return;
    _data = Internal::api.il2cpp_class_from_il2cpp_type((IL2CPP::Il2CppType *) type);
}

Class::Class(const MonoType *type) {
    if (!type) return;
    _data = Internal::api.il2cpp_class_from_il2cpp_type((IL2CPP::Il2CppType *) type->type);
}

static IL2CPP::Il2CppClass *TryGetClassWithoutImage(const std::string_view &_namespace, const std::string_view &_name) {
    auto &assemblies = Internal::GetAllAssemblies();

    for (auto assembly : assemblies) {
        if (!assembly) continue;
        auto image = Internal::api.il2cpp_assembly_get_image(assembly);
        if (auto _data = Internal::TryGetClassInImage(image, _namespace, _name); _data) return _data;
    }

    return nullptr;
}

Class::Class(const std::string_view &_namespace, const std::string_view &_name) {
    if (_data = TryGetClassWithoutImage(_namespace, _name); _data) return;
    BNM_LOG_WARN("Class not found: %s.%s", _namespace.data(), _name.data());
}

Class::Class(const std::string_view &_namespace, const std::string_view &_name, const BNM::Image &image) {
    if (!image) {
        BNM_LOG_WARN("Class image invalid: %s for %s.%s", image.str().data(), _namespace.data(), _name.data());
        _data = nullptr;
        return;
    }

    if (_data = Internal::TryGetClassInImage(image, _namespace, _name); _data) return;

    BNM_LOG_WARN("Class not found in image: %s.%s in %s", _namespace.data(), _name.data(), image.str().data());
}

std::vector<Class> Class::GetInnerClasses(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<Class> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto cls = Internal::api.il2cpp_class_get_nested_types(curClass, &iter)) ret.emplace_back(cls);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<FieldBase> Class::GetFields(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<FieldBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto field = Internal::api.il2cpp_class_get_fields(curClass, &iter)) ret.emplace_back((IL2CPP::FieldInfo *) field);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<MethodBase> Class::GetMethods(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<MethodBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto method = Internal::api.il2cpp_class_get_methods(curClass, &iter)) ret.emplace_back((IL2CPP::MethodInfo *) method);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<PropertyBase> Class::GetProperties(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<PropertyBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto prop = Internal::api.il2cpp_class_get_properties(curClass, &iter)) ret.emplace_back((IL2CPP::PropertyInfo *) prop);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

std::vector<EventBase> Class::GetEvents(bool includeParent) const {
    if (!_data) return {};
    TryInit();
    std::vector<EventBase> ret{};
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto eventInfo = Internal::api.il2cpp_class_get_events(curClass, &iter)) ret.emplace_back((IL2CPP::EventInfo *) eventInfo);
        if (includeParent) curClass = Internal::api.il2cpp_class_get_parent(curClass);
        else break;
    } while (curClass);

    return ret;
}

MethodBase Class::GetMethod(const std::string_view &name, int parameters) const {
    if (!_data) return {};
    TryInit();

    auto method = Internal::IterateMethods(*this, [&name, parameters](IL2CPP::MethodInfo *method) {
        return name == (method->name ? method->name : "") && (method->parameters_count == parameters || parameters == -1);
    });

    if (method != nullptr) return method;

    BNM_LOG_WARN("Method not found: %s.%s(%d)", _data->namespaze ? _data->namespaze : "", _data->name ? _data->name : "", parameters);
    return {};
}

MethodBase Class::GetMethod(const std::string_view &name, const std::initializer_list<std::string_view> &parameterNames) const {
    if (!_data) return {};
    TryInit();

    auto parameters = (uint8_t) parameterNames.size();

    auto method = Internal::IterateMethods(*this, [&name, &parameters, &parameterNames](IL2CPP::MethodInfo *method) {
        if (name != (method->name ? method->name : "") || method->parameters_count != parameters) return false;
        for (uint8_t i = 0; i < parameters; ++i) if (Internal::api.il2cpp_method_get_param_name(method, i) != parameterNames.begin()[i]) return false;
        return true;
    });

    if (method != nullptr) return method;

    BNM_LOG_WARN("Method not found by names: %s", name.data());
    return {};
}

MethodBase Class::GetMethod(const std::string_view &name, const std::initializer_list<CompileTimeClass> &parameterTypes) const {
    if (!_data) return {};
    TryInit();
    auto parameters = (uint8_t) parameterTypes.size();

    auto method = Internal::IterateMethods(*this, [&name, parameters, &parameterTypes](IL2CPP::MethodInfo *method) {
        if (name != (method->name ? method->name : "") || method->parameters_count != parameters) return false;
        for (uint8_t i = 0; i < parameters; ++i) {
            auto param = Internal::api.il2cpp_method_get_param(method, i);
            if (!param) return false;
            if (Class(param).GetClass() != parameterTypes.begin()[i].ToIl2CppClass()) return false;
        }
        return true;
    });

    if (method != nullptr) return method;

    BNM_LOG_WARN("Method not found by types: %s", name.data());
    return {};
}

PropertyBase Class::GetProperty(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        auto prop = Internal::api.il2cpp_class_get_property_from_name(curClass, name.data());
        if (prop) return prop;
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Property not found: %s", name.data());
    return {};
}

PropertyBase Class::GetProperty(const std::string_view &name, const CompileTimeClass &type) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    auto typeClass = type.ToIl2CppClass();

    do {
        void *iter = nullptr;
        while (auto currentProperty = Internal::api.il2cpp_class_get_properties(curClass, &iter)) {
            auto propInfo = (IL2CPP::PropertyInfo *) currentProperty;
            if (name == (propInfo->name ? propInfo->name : "") && PropertyBase{propInfo}.GetType() == typeClass) return propInfo;
        }
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Property not found by type: %s", name.data());
    return {};
}

Class Class::GetInnerClass(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto cls = Internal::api.il2cpp_class_get_nested_types(curClass, &iter)) {
            auto clsName = Internal::api.il2cpp_class_get_name(cls);
            if (name == (clsName ? clsName : "")) return cls;
        }
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Inner class not found: %s", name.data());
    return {};
}

FieldBase Class::GetField(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        auto field = Internal::api.il2cpp_class_get_field_from_name(curClass, name.data());
        if (field) return field;
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Field not found: %s", name.data());
    return {};
}

EventBase Class::GetEvent(const std::string_view &name) const {
    if (!_data) return {};
    TryInit();
    auto curClass = _data;

    do {
        void *iter = nullptr;
        while (auto currentEvent = Internal::api.il2cpp_class_get_events(curClass, &iter)) {
            auto eventInfo = (IL2CPP::EventInfo *) currentEvent;
            if (name == (eventInfo->name ? eventInfo->name : "")) return eventInfo;
        }
        curClass = Internal::api.il2cpp_class_get_parent(curClass);
    } while (curClass);

    BNM_LOG_WARN("Event not found: %s", name.data());
    return {};
}

Class Class::GetParent() const {
    if (!_data) return {};
    TryInit();
    return Internal::api.il2cpp_class_get_parent(_data);
}

Class Class::GetArray() const {
    if (!_data) return {};
    TryInit();
    return Internal::api.il2cpp_array_class_get(_data, 1);
}

Class Class::GetPointer() const {
    if (!_data) return {};
    TryInit();
    return Internal::GetPointer(*this);
}

Class Class::GetReference() const {
    if (!_data) return {};
    TryInit();
    return Internal::GetReference(*this);
}

Class Class::GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const {
    if (!_data) return {};
    TryInit();
    return Internal::TryMakeGenericClass(*this, templateTypes);
}

IL2CPP::Il2CppType *Class::GetIl2CppType() const {
    if (!_data) return nullptr;
    TryInit();
    return Internal::api.il2cpp_class_get_type(_data);
}

MonoType *Class::GetMonoType() const {
    if (!_data) return nullptr;
    TryInit();
    return (MonoType *) Internal::api.il2cpp_type_get_object(GetIl2CppType());
}

CompileTimeClass Class::GetCompileTimeClass() const {
    TryInit();
    CompileTimeClass result{};
    result._loadedClass = *this;
    return result;
}

Image Class::GetImage() const {
    if (!_data) return {};
    TryInit();
    return {Internal::api.il2cpp_class_get_image(_data)};
}

IL2CPP::Il2CppObject *Class::CreateNewInstance() const {
    if (!_data) return {};
    TryInit();

    if ((Internal::api.il2cpp_class_get_flags(_data) & (0x00000080 | 0x00000020)))
        BNM_LOG_WARN("CreateNewInstance on abstract/interface: %s", str().c_str());

    auto obj = Internal::api.il2cpp_object_new(_data);
    if (obj) memset((char *) obj + sizeof(IL2CPP::Il2CppObject), 0, Internal::api.il2cpp_class_instance_size(_data) - sizeof(IL2CPP::Il2CppObject));
    return obj;
}

IL2CPP::Il2CppObject *Class::BoxObject(IL2CPP::Il2CppClass *_data, void *data) {
    return Internal::api.il2cpp_value_box(_data, data);
}

IL2CPP::Il2CppArray *Class::ArrayNew(IL2CPP::Il2CppClass *cls, IL2CPP::il2cpp_array_size_t length) {
    return Internal::api.il2cpp_array_new(cls, length);
}

void *Class::NewListInstance() {
    return Internal::customListTemplateClass ? Internal::api.il2cpp_object_new(Internal::customListTemplateClass) : nullptr;
}

Class Class::GetListClass() {
    return Internal::vmData.System$$List;
}

Class CompileTimeClass::ToClass() {
    if (!_autoFree) {
        if (_loadedClass) return _loadedClass;
        if (_stack.empty()) return {};
    }

    if (_stack.empty()) {
        if (_reference) {
            _loadedClass = *_reference;
            _autoFree = false;
            _loadedClass.TryInit();
            return _loadedClass;
        }
        return _loadedClass;
    }

    bool autoFree = _autoFree;
    _autoFree = false;

    for (auto info : _stack) {
        switch (info->_baseType) {
            case _BaseType::Class: {
                auto classInfo = (_ClassInfo *) info;
                auto _namespace = classInfo->_namespace ? classInfo->_namespace : "";
                if (!classInfo->_imageName || !strlen(classInfo->_imageName)) {
                    _loadedClass = TryGetClassWithoutImage(_namespace, classInfo->_name);
                    break;
                }
                BNM::Image image{};
                auto &assemblies = Internal::GetAllAssemblies();
                for (auto assembly : assemblies) {
                    if (!assembly) continue;
                    auto currentImage = Internal::api.il2cpp_assembly_get_image(assembly);
                    if (!Internal::CompareImageName(currentImage, classInfo->_imageName)) continue;
                    image = currentImage;
                    break;
                }
                _loadedClass = Internal::TryGetClassInImage(image, _namespace, classInfo->_name);
            } break;
            case _BaseType::Inner: {
                auto innerInfo = (_InnerInfo *) info;
                if (!_loadedClass) {
                    BNM_LOG_WARN("Inner class parent not resolved: %s", innerInfo->_name);
                    break;
                }
                _loadedClass = _loadedClass.GetInnerClass(innerInfo->_name);
            } break;
            case _BaseType::Modifier: {
                auto modifierInfo = (_ModifierInfo *) info;
                switch (modifierInfo->_modifierType) {
                    case ModifierType::Pointer: _loadedClass = _loadedClass.GetPointer(); break;
                    case ModifierType::Reference: _loadedClass = _loadedClass.GetReference(); break;
                    case ModifierType::Array: _loadedClass = _loadedClass.GetArray(); break;
                    case ModifierType::None: break;
                }
            } break;
            case _BaseType::Generic: {
                auto genericInfo = (_GenericInfo *) info;
                if (!_loadedClass) {
                    BNM_LOG_WARN("Generic parent not resolved");
                    break;
                }
                _loadedClass = Internal::TryMakeGenericClass(_loadedClass, genericInfo->_types);
            } break;
            case _BaseType::None:
            case _BaseType::MaxCount: break;
        }
    }

    if (autoFree) Free();

    _loadedClass.TryInit();
    return _loadedClass;
}

MethodBase::MethodBase(const IL2CPP::MethodInfo *info) {
    if (!info) return;

    _isStatic = (info->flags & 0x0010) == 0x0010;
    _isVirtual = info->slot != 65535;
    _data = (decltype(_data)) info;
}

MethodBase::MethodBase(const IL2CPP::Il2CppReflectionMethod *reflectionMethod) {
    if (!reflectionMethod || !reflectionMethod->method) return;

    auto info = reflectionMethod->method;

    _isStatic = (info->flags & 0x0010) == 0x0010;
    _isVirtual = info->slot != 65535;
    _data = (decltype(_data)) info;
}

MethodBase &MethodBase::SetInstance(IL2CPP::Il2CppObject *val) {
    if (!_data) return *this;
    if (_isStatic) {
        BNM_LOG_WARN("SetInstance on static method: %s", str().c_str());
        return *this;
    }
    _instance = val;
    return *this;
}

MethodBase MethodBase::GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const {
    if (!_data) return {};
    if (!_data->is_generic) {
        BNM_LOG_WARN("GetGeneric on non-generic method: %s", str().c_str());
        return {};
    }
    return Internal::TryMakeGenericMethod(*this, templateTypes);
}

MethodBase MethodBase::GetOverride() const {
    if (!_data || _isStatic || (_data->flags & 0x0040) == 0) return {};
    if (!Utils::IsAllocated(_instance)) {
        BNM_LOG_WARN("GetOverride dead instance: %s", str().c_str());
        return {};
    }

    auto klass = _instance->klass;
    void *iter = nullptr;
    while (auto method = Internal::api.il2cpp_class_get_methods(klass, &iter)) {
        auto vMethod = (IL2CPP::MethodInfo *) method;
        if (strcmp(vMethod->name, _data->name) != 0 || vMethod->parameters_count != _data->parameters_count) continue;
        bool match = true;
        for (uint8_t p = 0; p < vMethod->parameters_count; ++p) {
            auto type = Internal::api.il2cpp_method_get_param(vMethod, p);
            auto type2 = Internal::api.il2cpp_method_get_param(_data, p);
            if (Class(type).GetClass() != Class(type2).GetClass()) {
                match = false;
                break;
            }
        }
        if (!match) continue;
        if (vMethod->slot != 65535 && vMethod->slot < klass->vtable_count) {
            auto &vTable = klass->vtable[vMethod->slot];
            if (vTable.method) return MethodBase(vTable.method)[_instance];
        }
        return MethodBase(vMethod)[_instance];
    }
    return {};
}

BNM::Class MethodBase::GetReturnType() const {
    if (!_data) return {};
    return Internal::api.il2cpp_method_get_return_type(_data);
}

BNM::Class MethodBase::GetParentClass() const {
    if (!_data) return {};
    return Internal::api.il2cpp_method_get_class(_data);
}

FieldBase::FieldBase(IL2CPP::FieldInfo *info) {
    if (!info) return;

    _isConst = Internal::api.il2cpp_field_is_literal(info);
    auto flags = Internal::api.il2cpp_field_get_flags(info);
    auto offset = (int32_t) Internal::api.il2cpp_field_get_offset(info);
    _isStatic = !_isConst && (flags & 0x0010) != 0 && offset != -1;
    _data = info;
    _isThreadStatic = offset == -1;
    auto parent = Internal::api.il2cpp_field_get_parent(info);
    _isInStruct = parent && Internal::api.il2cpp_class_is_valuetype(parent);
}

FieldBase &FieldBase::SetInstance(IL2CPP::Il2CppObject *val) {
    if (!_data) {
        BNM_LOG_ERR("Field::SetInstance dead field");
        return *this;
    }
    if (_isStatic || _isConst) {
        BNM_LOG_WARN("Field::SetInstance on static/const: %s", str().c_str());
        return *this;
    }
    _instance = val;
    return *this;
}

void *FieldBase::GetFieldPointer() const {
    auto &api = Internal::api;
    if (!_data) return nullptr;
    if (!_isStatic && !Utils::IsAllocated(_instance)) {
        BNM_LOG_ERR("Field::GetFieldPointer dead instance: %s", str().c_str());
        return nullptr;
    } else if (_isStatic && !Utils::IsAllocated(_data->parent)) {
        BNM_LOG_ERR("Field::GetFieldPointer dead parent: %s", str().c_str());
        return nullptr;
    } else if (_isThreadStatic) {
        BNM_LOG_ERR("Field::GetFieldPointer thread static unsupported: %s", str().c_str());
        return nullptr;
    } else if (_isConst) {
        BNM_LOG_ERR("Field::GetFieldPointer const impossible: %s", str().c_str());
        return nullptr;
    }
    auto offset = (BNM_PTR) api.il2cpp_field_get_offset(_data);
    if (_isStatic) {
        auto staticData = api.il2cpp_class_get_static_field_data(_data->parent);
        return staticData ? (void *) ((BNM_PTR) staticData + offset) : nullptr;
    }
    return (void *) ((BNM_PTR) _instance + offset - (_isInStruct ? sizeof(IL2CPP::Il2CppObject) : 0));
}

BNM::Class FieldBase::GetType() const {
    if (!_data) return {};
    return Internal::api.il2cpp_field_get_type(_data);
}

BNM::Class FieldBase::GetParentClass() const {
    if (!_data) return {};
    return Internal::api.il2cpp_field_get_parent(_data);
}

PropertyBase::PropertyBase(const IL2CPP::PropertyInfo *info) {
    if (!info) return;

    _data = (IL2CPP::PropertyInfo *) info;

    auto get = Internal::api.il2cpp_property_get_get_method((IL2CPP::PropertyInfo *) info);
    auto set = Internal::api.il2cpp_property_get_set_method((IL2CPP::PropertyInfo *) info);

    if (get && get->methodPointer) {
        _hasGetter = true;
        _getter = get;
    }

    if (set && set->methodPointer) {
        _hasSetter = true;
        _setter = set;
    }
}

PropertyBase &PropertyBase::SetInstance(IL2CPP::Il2CppObject *val) {
    if (_hasGetter) _getter.SetInstance(val);
    if (_hasSetter) _setter.SetInstance(val);
    return *this;
}

BNM::Class PropertyBase::GetType() const {
    if (!_data) return {};
    auto get = Internal::api.il2cpp_property_get_get_method(_data);
    if (get) return Internal::api.il2cpp_method_get_return_type(get);
    auto set = Internal::api.il2cpp_property_get_set_method(_data);
    if (set) return Internal::api.il2cpp_method_get_param(set, 0);
    return {};
}

BNM::Class PropertyBase::GetParentClass() const {
    if (!_data) return {};
    return Internal::api.il2cpp_property_get_parent(_data);
}

EventBase::EventBase(const IL2CPP::EventInfo *info) {
    if (!info) return;
    _hasAdd = _hasRemove = _hasRaise = false;
    if (info->add && info->add->methodPointer) {
        _hasAdd = true;
        _add = info->add;
    }
    if (info->remove && info->remove->methodPointer) {
        _hasRemove = true;
        _remove = info->remove;
    }
    if (info->raise && info->raise->methodPointer) {
        _hasRaise = true;
        _raise = info->raise;
    }
    _data = (IL2CPP::EventInfo *) info;
}

EventBase &EventBase::SetInstance(IL2CPP::Il2CppObject *instance) {
    if (_hasAdd) _add.SetInstance(instance);
    if (_hasRemove) _remove.SetInstance(instance);
    if (_hasRaise) _raise.SetInstance(instance);
    return *this;
}

BNM::Class EventBase::GetParentClass() const {
    if (!_data) return {};
    return _data->parent;
}

Image::Image(const std::string_view &name) {
    _data = Internal::TryGetImage(name);

    BNM_LOG_WARN_IF(!_data, "Image not found: %s", name.data());
}

Image::Image(const BNM::IL2CPP::Il2CppAssembly *assembly) {
    _data = Internal::api.il2cpp_assembly_get_image((IL2CPP::Il2CppAssembly *) assembly);
}

std::vector<BNM::Class> Image::GetClasses(bool includeInner) const {
    auto &api = Internal::api;
    std::vector<IL2CPP::Il2CppClass *> classes{};

    if (_data->nameToClassHashTable != (decltype(_data->nameToClassHashTable)) - 0x424e4d) {
        if (api.il2cpp_image_get_class_count && api.il2cpp_image_get_class) {
            size_t typeCount = api.il2cpp_image_get_class_count(_data);
            for (size_t i = 0; i < typeCount; ++i) {
                auto cls = api.il2cpp_image_get_class(_data, i);
                if (!cls) continue;
                auto declaring = api.il2cpp_class_get_declaring_type(cls);
                if (!includeInner && declaring) continue;
                auto name = api.il2cpp_class_get_name(cls);
                if (!includeInner && (!api.il2cpp_class_get_flags(cls) || (name && strcmp(name, "<Module>") == 0))) continue;
                classes.push_back(cls);
            }
        }
    }

    ClassesManagement::ForEachClassInImage(_data, [&classes, includeInner](IL2CPP::Il2CppClass *cls) -> bool {
        if (!includeInner && Internal::api.il2cpp_class_get_declaring_type(cls)) return false;
        classes.push_back(cls);
        return false;
    });

    std::vector<BNM::Class> ret{};
    ret.reserve(classes.size());
    for (auto cls : classes) ret.emplace_back(cls);
    return ret;
}

std::vector<BNM::Image> Image::GetImages() {
    auto &assemblies = Internal::GetAllAssemblies();

    std::vector<Image> ret{};
    ret.reserve(assemblies.size());
    for (auto assembly : assemblies) {
        if (!assembly) continue;
        ret.emplace_back(Internal::api.il2cpp_assembly_get_image(assembly));
    }

    return ret;
}

namespace BNM::PRIVATE_FieldUtils {
    void GetStaticValue(IL2CPP::FieldInfo *info, void *value) {
        return Internal::api.il2cpp_field_static_get_value(info, value);
    }

    void SetStaticValue(IL2CPP::FieldInfo *info, void *value) {
        return Internal::api.il2cpp_field_static_set_value(info, value);
    }
}

bool BNM::InvokeHookImpl(IL2CPP::MethodInfo *info, void *newMet, void **oldMet) {
    if (!info) return false;
    if (oldMet) *oldMet = (void *) info->methodPointer;
    info->methodPointer = (IL2CPP::Il2CppMethodPointer) newMet;
    return true;
}

bool BNM::VirtualHookImpl(Class targetClass, IL2CPP::MethodInfo *info, void *newMet, void **oldMet) {
    if (!info || !targetClass) return false;

    auto klass = targetClass._data;
    if (!klass || info->slot >= klass->vtable_count) return false;

    auto &vTable = klass->vtable[info->slot];
    if (vTable.method == nullptr) return false;

    if (oldMet) *oldMet = (void *) vTable.methodPtr;
    vTable.methodPtr = (IL2CPP::Il2CppMethodPointer) newMet;
    return true;
}

Structures::Mono::String *BNM::CreateMonoString(const std::string_view &str) {
    return (Structures::Mono::String *) Internal::api.il2cpp_string_new(str.data());
}

namespace BNM::Structures::Mono::PRIVATE_MonoListData {
    static std::map<uint32_t, IL2CPP::Il2CppClass *> customListsMap{};

    IL2CPP::Il2CppClass *TryGetMonoListClass(uint32_t typeHash, MethodData *data, size_t count) {
        auto &klass = customListsMap[typeHash];
        if (klass) return klass;

        auto templateClass = Internal::customListTemplateClass;
        if (!templateClass) return nullptr;
        auto size = sizeof(IL2CPP::Il2CppClass) + templateClass->vtable_count * sizeof(IL2CPP::VirtualInvokeData);
        auto typedClass = (IL2CPP::Il2CppClass *) BNM_malloc(size);
        memcpy(typedClass, templateClass, size);

        std::map<size_t, IL2CPP::MethodInfo *> createdMethods{};
        for (uint16_t i = 4; i < typedClass->vtable_count; ++i) {
            auto &cur = typedClass->vtable[i];
            if (!cur.method || !cur.method->name) continue;
            auto name = std::string_view(cur.method->name);
            auto dot = name.rfind('.');
            if (dot != std::string_view::npos) name = name.substr(dot + 1);

            auto iterator = data;
            size_t c = 0;
            for (; c < count; ++c, ++iterator) if (iterator->name && name == iterator->name) break;
            if (c == count) continue;

            auto &methodInfo = createdMethods[FNV1a(name)];
            if (!methodInfo) {
                methodInfo = (IL2CPP::MethodInfo *) BNM_malloc(sizeof(IL2CPP::MethodInfo));
                *methodInfo = *cur.method;
                methodInfo->methodPointer = (IL2CPP::Il2CppMethodPointer) iterator->ptr;
            }
            cur.method = methodInfo;
            cur.methodPtr = methodInfo->methodPointer;
        }
        klass = typedClass;
        return klass;
    }
}

void *BNM::GetExternMethod(const std::string_view &str) {
    auto ret = Internal::api.il2cpp_resolve_icall(str.data());
    BNM_LOG_WARN_IF(!ret, "GetExternMethod failed: %s", str.data());
    return (void *) ret;
}

template<>
bool BNM::IsA<BNM::IL2CPP::Il2CppObject *>(BNM::IL2CPP::Il2CppObject *object, BNM::IL2CPP::Il2CppClass *_class) {
    if (!object || !_class) return false;
    for (auto cls = object->klass; cls; cls = cls->parent) if (cls == _class) return true;
    return false;
}

bool BNM::IsLoaded() {
    return Internal::states.state;
}

void *BNM::GetIl2CppLibraryHandle() {
    return Internal::il2cppLibraryHandle;
}

bool BNM::AttachIl2Cpp() {
    if (CurrentIl2CppThread()) return false;
    Internal::api.il2cpp_thread_attach(Internal::api.il2cpp_domain_get());
    return true;
}

IL2CPP::Il2CppThread *BNM::CurrentIl2CppThread() {
    return Internal::api.il2cpp_thread_current();
}

void BNM::DetachIl2Cpp() {
    auto thread = CurrentIl2CppThread();
    if (!thread) return;
    Internal::api.il2cpp_thread_detach(thread);
}

void *BNM::Allocate(size_t size) {
    return Internal::api.il2cpp_gc_alloc_fixed(size);
}

void BNM::Free(void *ptr) {
    return Internal::api.il2cpp_gc_free_fixed(ptr);
}

namespace BNM::Defaults::Internal {
    BNM::Class Void{}, Boolean{}, Byte{}, SByte{}, Int16{}, UInt16{}, Int32{}, UInt32{}, IntPtr{}, UIntPtr{}, Int64{}, UInt64{}, Single{}, Double{}, Decimal{}, String{}, Object{};
    BNM::Class Type{};
    BNM::Class Vector2{}, Vector3{}, Vector4{}, Color{}, Color32{}, Ray{}, Quaternion{}, Matrix3x3{}, Matrix4x4{}, RaycastHit{}, RaycastHit2D{};
    BNM::Class UnityObject{}, MonoBehaviour{};
}

void BNM::Internal::LoadDefaults() {
    using namespace BNM::Defaults::Internal;
    auto &api = BNM::Internal::api;

    auto image = api.il2cpp_get_corlib ? api.il2cpp_get_corlib() : nullptr;
    auto SystemStr = "System";
    Void = TryGetClassInImage(image, SystemStr, "Void");
    Boolean = TryGetClassInImage(image, SystemStr, "Boolean");
    Byte = TryGetClassInImage(image, SystemStr, "Byte");
    SByte = TryGetClassInImage(image, SystemStr, "SByte");
    Int16 = TryGetClassInImage(image, SystemStr, "Int16");
    UInt16 = TryGetClassInImage(image, SystemStr, "UInt16");
    Int32 = TryGetClassInImage(image, SystemStr, "Int32");
    UInt32 = TryGetClassInImage(image, SystemStr, "UInt32");
    IntPtr = TryGetClassInImage(image, SystemStr, "IntPtr");
    UIntPtr = TryGetClassInImage(image, SystemStr, "UIntPtr");
    Int64 = TryGetClassInImage(image, SystemStr, "Int64");
    UInt64 = TryGetClassInImage(image, SystemStr, "UInt64");
    Single = TryGetClassInImage(image, SystemStr, "Single");
    Double = TryGetClassInImage(image, SystemStr, "Double");
    Decimal = TryGetClassInImage(image, SystemStr, "Decimal");
    String = TryGetClassInImage(image, SystemStr, "String");
    Object = TryGetClassInImage(image, SystemStr, "Object");
    Type = TryGetClassInImage(image, SystemStr, "Type");

    auto UnityEngineStr = "UnityEngine";
    image = TryGetImage("UnityEngine.CoreModule.dll");
    Vector2 = TryGetClassInImage(image, UnityEngineStr, "Vector2");
    Vector3 = TryGetClassInImage(image, UnityEngineStr, "Vector3");
    Vector4 = TryGetClassInImage(image, UnityEngineStr, "Vector4");
    Color = TryGetClassInImage(image, UnityEngineStr, "Color");
    Color32 = TryGetClassInImage(image, UnityEngineStr, "Color32");
    Ray = TryGetClassInImage(image, UnityEngineStr, "Ray");
    Quaternion = TryGetClassInImage(image, UnityEngineStr, "Quaternion");
    Matrix3x3 = TryGetClassInImage(image, UnityEngineStr, "Matrix3x3");
    Matrix4x4 = TryGetClassInImage(image, UnityEngineStr, "Matrix4x4");

    RaycastHit = TryGetClassInImage(TryGetImage("UnityEngine.PhysicsModule.dll"), UnityEngineStr, "RaycastHit");

    RaycastHit2D = TryGetClassInImage(TryGetImage("UnityEngine.Physics2DModule.dll"), UnityEngineStr, "RaycastHit2D");

    UnityObject = TryGetClassInImage(image, UnityEngineStr, "Object");
    MonoBehaviour = TryGetClassInImage(image, UnityEngineStr, "MonoBehaviour");
}

namespace BNM::Exceptions {
    Exception TryInvoke(const std::function<void()> &func) {
        auto &api = BNM::Internal::api;
        IL2CPP::Il2CppType type;
        memset(&type, 0, sizeof(type));
        type.type = IL2CPP::IL2CPP_TYPE_VOID;
        IL2CPP::Il2CppClass klass;
        memset(&klass, 0, sizeof(klass));
        IL2CPP::MethodInfo info;
        memset(&info, 0, sizeof(info));
        info.return_type = &type;
        PRIVATE_INTERNAL::GetMethodClass(&info) = &klass;
        info.methodPointer = (decltype(info.methodPointer)) &func;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
        info.invoker_method = (IL2CPP::InvokerMethod) +[](std::function<void()> *f) -> void { (*f)(); };
#pragma GCC diagnostic pop
        IL2CPP::Il2CppException *exception = nullptr;
        api.il2cpp_runtime_invoke(&info, nullptr, nullptr, &exception);
        return {exception};
    }
}

Structures::Mono::String *Structures::Mono::String::Empty() {
    return (Structures::Mono::String *) (Internal::vmData.String$$Empty ? *Internal::vmData.String$$Empty : nullptr);
}

template<typename T>
Structures::Mono::Array<T> *Structures::Mono::Array<T>::Create(IL2CPP::il2cpp_array_size_t size, bool zeroed) {
    auto cls = BNM::Defaults::Get<T>().ToClass();
    if (!cls) return nullptr;
    auto arrayClass = cls.GetArray();
    if (!arrayClass) return nullptr;
    auto arr = (Array<T> *) BNM::Internal::api.il2cpp_array_new(arrayClass._data, size);
    if (arr && zeroed) memset((char *) arr + sizeof(IL2CPP::Il2CppArray), 0, size * sizeof(T));
    return arr;
}

template Structures::Mono::Array<MonoType *> *Structures::Mono::Array<MonoType *>::Create(IL2CPP::il2cpp_array_size_t, bool);
template Structures::Mono::Array<int> *Structures::Mono::Array<int>::Create(IL2CPP::il2cpp_array_size_t, bool);

namespace BNM::Structures::Unity {

const Color Color::black = {0.f, 0.f, 0.f};
const Color Color::red = {1.f, 0.f, 0.f};
const Color Color::green = {0.f, 1.f, 0.f};
const Color Color::blue = {0.f, 0.f, 1.f};
const Color Color::white = {1.f, 1.f, 1.f};
const Color Color::orange = {1.f, 0.55f, 0.f};
const Color Color::yellow = {1.f, 0.92156863f, 0.015686275f};
const Color Color::cyan = {0.f, 1.f, 1.f};
const Color Color::magenta = {1.f, 0.f, 1.f};

constexpr float floatInf = std::numeric_limits<float>::infinity();

const Vector2 Vector2::positiveInfinity = {floatInf, floatInf};
const Vector2 Vector2::negativeInfinity = {-floatInf, -floatInf};
const Vector2 Vector2::down = {0.f, -1.f};
const Vector2 Vector2::left = {-1.f, 0.f};
const Vector2 Vector2::one = {1.f, 1.f};
const Vector2 Vector2::right = {1.f, 0.f};
const Vector2 Vector2::up = {0.f, 1.f};
const Vector2 Vector2::zero = {0.f, 0.f};

const Vector3 Vector3::positiveInfinity = {floatInf, floatInf, floatInf};
const Vector3 Vector3::negativeInfinity = {-floatInf, -floatInf, -floatInf};
const Vector3 Vector3::back = {0.f, 0.f, -1.f};
const Vector3 Vector3::down = {0.f, -1.f, 0.f};
const Vector3 Vector3::forward = {0.f, 0.f, 1.f};
const Vector3 Vector3::left = {-1.f, 0.f, 0.f};
const Vector3 Vector3::one = {1.f, 1.f, 1.f};
const Vector3 Vector3::right = {1.f, 0.f, 0.f};
const Vector3 Vector3::up = {0.f, 1.f, 0.f};
const Vector3 Vector3::zero = {0.f, 0.f, 0.f};

const Vector4 Vector4::positiveInfinity = {floatInf, floatInf, floatInf, floatInf};
const Vector4 Vector4::negativeInfinity = {-floatInf, -floatInf, -floatInf, -floatInf};
const Vector4 Vector4::zero = {0.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::one = {1.f, 1.f, 1.f, 1.f};

const Quaternion Quaternion::identity = {0.f, 0.f, 0.f, 1.f};

const Matrix4x4 Matrix4x4::identity(InitIdentity::kIdentity);

void *RaycastHit::GetCollider() const {
    if (!m_Collider || (BNM_PTR) m_Collider < 0) return {};
    static void *(*FromId)(int) {};
    static void *(*FromIdInjected)(int) {};
    if (!FromId) FromId = (decltype(FromId)) BNM::GetExternMethod("UnityEngine.Object::FindObjectFromInstanceID");
    if (!FromIdInjected) FromIdInjected = (decltype(FromIdInjected)) BNM::GetExternMethod("UnityEngine.Object::FindObjectFromInstanceID_Injected");
    if (FromIdInjected) return (void *) BNM::UnmarshalUnityObject<void *>((BNM_INT_PTR) FromIdInjected(m_Collider));
    if (FromId) return FromId(m_Collider);
    return {};
}

void *RaycastHit2D::GetCollider() const {
    if (!m_Collider || (BNM_PTR) m_Collider < 0) return {};
    static void *(*FromId)(int) {};
    static void *(*FromIdInjected)(int) {};
    if (!FromId) FromId = (decltype(FromId)) BNM::GetExternMethod("UnityEngine.Object::FindObjectFromInstanceID");
    if (!FromIdInjected) FromIdInjected = (decltype(FromIdInjected)) BNM::GetExternMethod("UnityEngine.Object::FindObjectFromInstanceID_Injected");
    if (FromIdInjected) return (void *) BNM::UnmarshalUnityObject<void *>((BNM_INT_PTR) FromIdInjected(m_Collider));
    if (FromId) return FromId(m_Collider);
    return {};
}

Matrix3x3::Matrix3x3(const Matrix4x4 &other) {
    m_Data[0] = other.m_Data[0];
    m_Data[1] = other.m_Data[1];
    m_Data[2] = other.m_Data[2];
    m_Data[3] = other.m_Data[4];
    m_Data[4] = other.m_Data[5];
    m_Data[5] = other.m_Data[6];
    m_Data[6] = other.m_Data[8];
    m_Data[7] = other.m_Data[9];
    m_Data[8] = other.m_Data[10];
}

Matrix3x3 &Matrix3x3::operator=(const Matrix4x4 &other) {
    m_Data[0] = other.m_Data[0];
    m_Data[1] = other.m_Data[1];
    m_Data[2] = other.m_Data[2];
    m_Data[3] = other.m_Data[4];
    m_Data[4] = other.m_Data[5];
    m_Data[5] = other.m_Data[6];
    m_Data[6] = other.m_Data[8];
    m_Data[7] = other.m_Data[9];
    m_Data[8] = other.m_Data[10];
    return *this;
}

Matrix3x3 &Matrix3x3::operator*=(const Matrix4x4 &inM) {
    for (int i = 0; i < 3; i++) {
        float v[3] = {Get(i, 0), Get(i, 1), Get(i, 2)};
        Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
        Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
        Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
    }
    return *this;
}

bool Matrix3x3::Invert() {
    auto m = *this;
    bool success = InvertMatrix4x4_Full(m.GetPtr(), m.GetPtr());
    *this = m;
    return success;
}

}

namespace BNM::UnityEngine {
    BNM::Class UnityEventBase::GetArgumentType(PersistentCall *call) {
        auto type = Internal::vmData.UnityEngine$$Object;
        auto typeName = call->m_Arguments->m_ObjectArgumentAssemblyTypeName;
        if (!typeName->IsNullOrEmpty()) if (auto t = Internal::vmData.Type$$GetType.cast<Method<MonoType *>>()(typeName); t != nullptr) type = t;
        return type;
    }
    BNM::Class UnityEventBase::GetTargetType(PersistentCall *call) {
        BNM::Class targetType;
        if (call->m_Target) targetType = call->m_Target;
        else targetType = Internal::vmData.Type$$GetType.cast<Method<MonoType *>>()(call->m_TargetAssemblyTypeName);
        return targetType;
    }
}

MethodBase DelegateBase::GetMethod() const {
    auto method = MethodBase(this->method);
    auto instance = GetInstance();
    if (instance) method.SetInstance(instance);
    return method;
}

DelegateBase *DelegateBase::Create(BNM::MethodBase method) {
    return (DelegateBase *) BNM::Class(object.klass).CreateNewObjectParameters(method._instance, method._data);
}

std::vector<BNM::MethodBase> MulticastDelegateBase::GetMethods() const {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    if (!delegates) return {((DelegateBase *) this)->GetMethod()};

    std::vector<MethodBase> ret{};
    ret.reserve(delegates->max_length);
    for (IL2CPP::il2cpp_array_size_t i = 0; i < delegates->max_length; ++i) ret.push_back((*delegates)[i]->GetMethod());
    return ret;
}

void MulticastDelegateBase::Add(DelegateBase *delegate) {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    auto arr = BNM::Class(delegates->obj.klass->element_class).NewArray<DelegateBase *>(this->delegates->max_length + 1);
    arr->CopyFrom(delegates->GetItems(), delegates->max_length);
    arr->GetItems()[delegates->max_length] = delegate;
    this->delegates = (decltype(this->delegates)) arr;
}

void MulticastDelegateBase::Remove(DelegateBase *delegate) {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    IL2CPP::il2cpp_array_size_t index = 0;
    bool found = false;
    for (IL2CPP::il2cpp_array_size_t i = 0; i < delegates->max_length; ++i) {
        if ((*delegates)[i] != delegate) continue;
        found = true;
        index = i;
        break;
    }
    if (!found) return;
    auto arr = BNM::Class(delegates->obj.klass->element_class).NewArray<DelegateBase *>(this->delegates->max_length - 1);
    auto src = delegates->GetItems();
    auto dst = arr->GetItems();
    memmove(dst + index, src + index + 1, (delegates->max_length - index - 1) * sizeof(void *));
    if (index > 0) memcpy(dst, src, index * sizeof(void *));
    this->delegates = (decltype(this->delegates)) arr;
}

DelegateBase *MulticastDelegateBase::Add(BNM::MethodBase method) {
    auto delegate = ((DelegateBase *) this)->Create(method);
    Add(delegate);
    return delegate;
}

void MANAGEMENT_STRUCTURES::AddClass(CustomClass *_class) {
    if (!ClassesManagement::classesManagementVector)
        ClassesManagement::classesManagementVector = new (BNM_malloc(sizeof(std::vector<MANAGEMENT_STRUCTURES::CustomClass *>))) std::vector<MANAGEMENT_STRUCTURES::CustomClass *>();
    ClassesManagement::classesManagementVector->push_back(_class);
}

namespace BNM::ClassesManagement {

static constexpr BNM_PTR BNM_CLASS_ALLOCATED_METHODS_FLAG = 0x01000000;
static constexpr BNM_PTR BNM_CLASS_ALLOCATED_FIELDS_FLAG = 0x02000000;
static constexpr BNM_PTR BNM_CLASS_ALLOCATED_INNER_FLAG = 0x04000000;
static constexpr BNM_PTR BNM_CLASS_ALLOCATED_HIERARCHY_FLAG = 0x08000000;

std::vector<MANAGEMENT_STRUCTURES::CustomClass *> *classesManagementVector = nullptr;
static std::map<BNM_PTR, std::vector<IL2CPP::Il2CppClass *>> bnmClassesMap{};

static void ForEachClassInImageImpl(IL2CPP::Il2CppImage *image, const std::function<bool(IL2CPP::Il2CppClass *)> &func) {
    auto it = bnmClassesMap.find((BNM_PTR) image);
    if (it == bnmClassesMap.end()) return;
    for (auto cls : it->second) if (func(cls)) break;
}

void ForEachClassInImage(IL2CPP::Il2CppImage *image, const std::function<bool(IL2CPP::Il2CppClass *)> &func) {
    ForEachClassInImageImpl(image, func);
}

static void AddClassToMap(IL2CPP::Il2CppImage *image, IL2CPP::Il2CppClass *cls) {
    bnmClassesMap[(BNM_PTR) image].emplace_back(cls);
}

#define BNM_I2C_NEW(type) (IL2CPP::type *) BNM_malloc(sizeof(IL2CPP::type))

struct CustomClassInfo {
    const char *_namespace{};
    const char *_name{};
    const char *_imageName{};
    Class _class{};
};

static void ModifyClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, Class target);
static void CreateClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, const CustomClassInfo &classInfo);
static CustomClassInfo GetClassInfo(const CompileTimeClass &compileTimeClass);
static IL2CPP::Il2CppImage *MakeImage(std::string_view imageName);
static IL2CPP::MethodInfo *ProcessCustomMethod(MANAGEMENT_STRUCTURES::CustomMethod *method, Class target, bool *hooked = nullptr);
static IL2CPP::MethodInfo *CreateMethod(MANAGEMENT_STRUCTURES::CustomMethod *method);
static void SetupField(IL2CPP::FieldInfo *newField, MANAGEMENT_STRUCTURES::CustomField *field);
static void GetAllInterfaces(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface, std::vector<IL2CPP::Il2CppClass *> &outInterfaces);
static void SetupClassOwner(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *owner);
static void SetupClassParent(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *parent);
static bool HasInterface(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface);
static void SetupTypes(IL2CPP::Il2CppClass *target);

void ProcessCustomClasses() {
    if (classesManagementVector == nullptr) return;

    for (auto customClass : *classesManagementVector) ProcessClassRuntime(customClass);

    classesManagementVector->clear();
    classesManagementVector->shrink_to_fit();
    BNM_free((void *) classesManagementVector);
    classesManagementVector = nullptr;
}

void ProcessClassRuntime(MANAGEMENT_STRUCTURES::CustomClass *customClass) {
    auto &type = customClass->_targetType;
    auto info = GetClassInfo(type);
    if (info._class) ModifyClass(customClass, info._class);
    else if (info._name) CreateClass(customClass, info);
    else {
        BNM_LOG_ERR("ProcessClassRuntime failed to resolve target");
    }
    type.Free();
    customClass->_interfaces.clear();
    customClass->_interfaces.shrink_to_fit();
    customClass->_fields.clear();
    customClass->_fields.shrink_to_fit();
    customClass->_methods.clear();
    customClass->_methods.shrink_to_fit();
}

static void ModifyClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, Class target) {
    BNM_LOG_DEBUG("ModifyClass target: %s", target.str().data());

    auto klass = target._data;

    auto baseType = customClass->_baseType.ToClass();

    IL2CPP::Il2CppClass *owner = customClass->_owner;

    SetupClassParent(klass, baseType._data);
    SetupClassOwner(klass, owner);

    auto newMethodsCount = customClass->_methods.size();
    auto newFieldsCount = customClass->_fields.size();

    if (newMethodsCount) {
        auto oldCount = klass->method_count;
        auto oldMethods = klass->methods;

        std::vector<IL2CPP::MethodInfo *> methodsToAdd{};

        for (size_t i = 0; i < newMethodsCount; ++i) {
            auto method = customClass->_methods[i];

            auto paramCount = method->_parameterTypes.size();
            (void) paramCount;

            bool isHooked = false;
            method->myInfo = ProcessCustomMethod(method, target, &isHooked);

            PRIVATE_INTERNAL::GetMethodClass(method->myInfo) = klass;

            if (!isHooked) methodsToAdd.push_back(method->myInfo);
            BNM_LOG_DEBUG_IF(isHooked, "ModifyClass hooked method: %s (%zu)", method->_name.data(), paramCount);
            BNM_LOG_DEBUG_IF(!isHooked, "ModifyClass added method: %s (%zu)", method->_name.data(), paramCount);
        }

        if (!methodsToAdd.empty()) {
            auto newMethods = (IL2CPP::MethodInfo **) BNM_malloc((oldCount + methodsToAdd.size()) * sizeof(IL2CPP::MethodInfo *));

            auto oldSize = oldCount * sizeof(IL2CPP::MethodInfo *);

            if (oldCount) memcpy(newMethods, oldMethods, oldSize);

            memcpy(newMethods + oldCount, methodsToAdd.data(), methodsToAdd.size() * sizeof(IL2CPP::MethodInfo *));

            if ((klass->flags & BNM_CLASS_ALLOCATED_METHODS_FLAG) == BNM_CLASS_ALLOCATED_METHODS_FLAG) BNM_free((void *) klass->methods);
            klass->flags |= (uint32_t) BNM_CLASS_ALLOCATED_METHODS_FLAG;

            klass->methods = (const IL2CPP::MethodInfo **) newMethods;
            klass->method_count += methodsToAdd.size();
        }
    }

    if (newFieldsCount) {
        auto oldCount = klass->field_count;

        auto newFields = (IL2CPP::FieldInfo *) BNM_malloc((oldCount + newFieldsCount) * sizeof(IL2CPP::FieldInfo));

        if (oldCount) memcpy(newFields, klass->fields, oldCount * sizeof(IL2CPP::FieldInfo));

        auto currentAddress = klass->instance_size;

        IL2CPP::FieldInfo *newField = newFields + oldCount;

        for (auto field : customClass->_fields) {
            SetupField(newField, field);
            newField->parent = klass;

            field->offset = (int32_t) currentAddress;
            newField->offset = (int32_t) currentAddress;
            field->myInfo = newField;

            currentAddress += field->_size;

            newField++;
            BNM_LOG_DEBUG("ModifyClass added field: %s", field->_name.data());
        }

        if ((klass->flags & BNM_CLASS_ALLOCATED_FIELDS_FLAG) == BNM_CLASS_ALLOCATED_FIELDS_FLAG) BNM_free(klass->fields);
        klass->flags |= (uint32_t) BNM_CLASS_ALLOCATED_FIELDS_FLAG;

        klass->actualSize = currentAddress;
        klass->fields = newFields;
        klass->field_count += newFieldsCount;
    }

    customClass->myClass = klass;
    customClass->type = Class(klass);
}

static char forEmptyString = '\0';
static const char *baseImageName = "Assembly-CSharp.dll";

static void CreateClass(MANAGEMENT_STRUCTURES::CustomClass *customClass, const CustomClassInfo &classInfo) {
    auto &api = Internal::api;
    Image image{};
    if (classInfo._imageName) {
        auto &assemblies = Internal::GetAllAssemblies();
        for (auto assembly : assemblies) {
            if (!assembly) continue;
            auto currentImage = api.il2cpp_assembly_get_image(assembly);
            if (!Internal::CompareImageName(currentImage, classInfo._imageName)) continue;
            image = currentImage;
            break;
        }
    } else image = Image(baseImageName);
    if (!image) image = MakeImage(classInfo._imageName);

    BNM_LOG_DEBUG("CreateClass target: %s.%s in %s", classInfo._namespace ? classInfo._namespace : &forEmptyString, classInfo._name, image._data->name);

    IL2CPP::Il2CppClass *parent = customClass->_baseType;
    if (!parent) parent = Internal::vmData.Object;

    IL2CPP::Il2CppClass *owner = customClass->_owner;

    std::vector<IL2CPP::Il2CppRuntimeInterfaceOffsetPair> newInterOffsets{};
    if (parent->interfaceOffsets)
        for (uint16_t i = 0; i < parent->interface_offsets_count; ++i)
            newInterOffsets.push_back(parent->interfaceOffsets[i]);

    auto allInterfaces = customClass->_interfaces;
    std::vector<IL2CPP::Il2CppClass *> interfaces{};
    for (auto &interface : allInterfaces)
        if (auto cls = interface.ToClass(); cls)
            GetAllInterfaces(parent, cls, interfaces);

    auto newVtableSize = parent->vtable_count;
    std::vector<IL2CPP::VirtualInvokeData> newVTable(newVtableSize);
    for (uint16_t i = 0; i < parent->vtable_count; ++i) newVTable[i] = parent->vtable[i];
    for (auto interface : interfaces) {
        newInterOffsets.push_back({interface, (int32_t) newVtableSize});
        void *iter = nullptr;
        while (auto v = api.il2cpp_class_get_methods(interface, &iter)) {
            newVtableSize++;
            newVTable.push_back({nullptr, (const IL2CPP::MethodInfo *) v});
        }
    }

    uint8_t hasFinalize = 0;
    auto methods = (const IL2CPP::MethodInfo **) BNM_malloc(customClass->_methods.size() * sizeof(IL2CPP::MethodInfo *));

    for (size_t i = 0; i < customClass->_methods.size(); ++i) {
        auto method = customClass->_methods[i];

        method->myInfo = ProcessCustomMethod(method, {});

        BNM_LOG_DEBUG("CreateClass added method: %s (%zu)", method->_name.data(), method->_parameterTypes.size());

        if (!method->_isStatic) for (uint16_t v = 0; v < newVtableSize; ++v) {
            auto &vTable = newVTable[v];
            if (!vTable.method) continue;
            auto count = vTable.method->parameters_count;

            if (!strcmp(vTable.method->name, method->myInfo->name) && count == method->myInfo->parameters_count && method->_parameterTypes.size() == count) {
                if (!method->_skipTypeMatch) {
                    bool typesMatch = true;
                    for (uint8_t p = 0; p < count; ++p) {
                        auto type = Internal::api.il2cpp_method_get_param((IL2CPP::MethodInfo *) vTable.method, p);
                        if (Class(type).GetClass() != method->_parameterTypes[p].ToIl2CppClass()) {
                            typesMatch = false;
                            break;
                        }
                    }
                    if (!typesMatch) continue;
                }

                if (!hasFinalize) hasFinalize = v == Internal::finalizerSlot;
                method->_origin = (IL2CPP::MethodInfo *) vTable.method;
                method->_originalAddress = (void *) (vTable.method ? vTable.method->methodPointer : nullptr);
                method->myInfo->slot = v;
                vTable.method = method->myInfo;
                vTable.methodPtr = method->myInfo->methodPointer;

                BNM_LOG_DEBUG("CreateClass overridden virtual: %s", MethodBase(method->_origin).str().c_str());

                break;
            }
        }

        methods[i] = method->myInfo;
    }

    auto klass = customClass->myClass = (IL2CPP::Il2CppClass *) BNM_malloc(sizeof(IL2CPP::Il2CppClass) + newVTable.size() * sizeof(IL2CPP::VirtualInvokeData));
    memset(klass, 0, sizeof(IL2CPP::Il2CppClass) + newVTable.size() * sizeof(IL2CPP::VirtualInvokeData));

    klass->image = image;

    auto len = strlen(classInfo._name);
    klass->name = (char *) BNM_malloc(len + 1);
    memcpy((void *) klass->name, classInfo._name, len);
    ((char *) klass->name)[len] = 0;

    if (!owner && classInfo._namespace) {
        len = strlen(classInfo._namespace);
        klass->namespaze = (char *) BNM_malloc(len + 1);
        memcpy((void *) klass->namespaze, classInfo._namespace, len);
        ((char *) klass->namespaze)[len] = 0;
    } else klass->namespaze = &forEmptyString;

    SetupTypes(klass);

    klass->element_class = klass;
    klass->castClass = klass;

    SetupClassParent(klass, parent);
    SetupClassOwner(klass, owner);

    klass->generic_class = nullptr;
    klass->typeMetadataHandle = nullptr;

    klass->klass = klass;

    klass->field_count = customClass->_fields.size();
    if (klass->field_count > 0) {
        auto fields = (IL2CPP::FieldInfo *) BNM_malloc(klass->field_count * sizeof(IL2CPP::FieldInfo));

        IL2CPP::FieldInfo *newField = fields;
        for (auto field : customClass->_fields) {
            SetupField(newField, field);
            newField->parent = klass;

            newField->offset = (int32_t) field->offset;
            field->myInfo = newField;

            newField++;
        }
        klass->fields = fields;
    } else klass->fields = nullptr;

    if (!interfaces.empty()) {
        klass->interfaces_count = interfaces.size();
        klass->implementedInterfaces = (IL2CPP::Il2CppClass **) BNM_malloc(interfaces.size() * sizeof(IL2CPP::Il2CppClass *));
        memcpy(klass->implementedInterfaces, interfaces.data(), interfaces.size() * sizeof(IL2CPP::Il2CppClass *));
    } else {
        klass->interfaces_count = 0;
        klass->implementedInterfaces = nullptr;
    }

    for (auto method : customClass->_methods) PRIVATE_INTERNAL::GetMethodClass(method->myInfo) = klass;
    klass->method_count = customClass->_methods.size();
    klass->methods = methods;
    klass->has_finalize = hasFinalize;

    klass->flags = (klass->parent->flags & ~(0x00000080 | 0x00000020)) | 0x0000001;

    klass->native_size = -1;
    klass->element_size = 0;
    klass->instance_size = klass->actualSize = customClass->_size;

    klass->vtable_count = newVTable.size();
    for (size_t i = 0; i < newVTable.size(); ++i) klass->vtable[i] = newVTable[i];

    klass->interface_offsets_count = newInterOffsets.size();
    klass->interfaceOffsets = (IL2CPP::Il2CppRuntimeInterfaceOffsetPair *) BNM_malloc(newInterOffsets.size() * sizeof(IL2CPP::Il2CppRuntimeInterfaceOffsetPair));
    memcpy(klass->interfaceOffsets, newInterOffsets.data(), newInterOffsets.size() * sizeof(IL2CPP::Il2CppRuntimeInterfaceOffsetPair));

    klass->interopData = nullptr;
    klass->events = nullptr;
    klass->properties = nullptr;
    klass->nestedTypes = nullptr;
    klass->rgctx_data = nullptr;

    klass->static_fields = nullptr;
    klass->static_fields_size = 0;

    klass->genericRecursionDepth = 0;
    klass->genericContainerHandle = nullptr;

    klass->stack_slot_size = sizeof(void *);

    klass->has_cctor = 0;
    klass->has_references = 1;
    klass->size_inited = 1;
    klass->is_vtable_initialized = 1;

    klass->enumtype = 0;
    klass->token = 0;
    klass->minimumAlignment = 1;
    klass->is_generic = 0;
    klass->rank = 0;
    klass->nested_type_count = 0;
    klass->thread_static_fields_offset = 0;
    klass->thread_static_fields_size = 0;
    klass->cctor_started = 0;
    klass->packingSize = 0;

    klass->size_init_pending = 0;

    klass->cctor_finished_or_no_cctor = 1;
    klass->nullabletype = 0;

    klass->cctor_thread = 0;

    klass->initialized = 0;
    klass->init_pending = 0;
    klass->initialized_and_no_error = 0;
    klass->initializationExceptionGCHandle = 0;

    klass->gc_desc = nullptr;

    api.il2cpp_runtime_class_init(klass);

    AddClassToMap(image.GetInfo(), klass);

    customClass->type = Class(klass);
}

static CustomClassInfo GetClassInfo(const CompileTimeClass &compileTimeClass) {
    CompileTimeClass tmp{};

    auto &stack = compileTimeClass._stack;

    for (auto info : stack) {
        if (info->_baseType != CompileTimeClass::_BaseType::Class) continue;

        auto classInfo = (CompileTimeClass::_ClassInfo *) info;

        auto ns = classInfo->_namespace ? classInfo->_namespace : "";
        auto name = classInfo->_name;

        auto it = std::find(stack.begin(), stack.end(), info);
        bool isLastClass = true;
        for (auto jt = it + 1; jt != stack.end(); ++jt) {
            if ((*jt)->_baseType == CompileTimeClass::_BaseType::Class) {
                isLastClass = false;
                break;
            }
        }

        auto resolved = TryGetClassWithoutImage(ns, name);
        if (resolved) continue;

        if (isLastClass) return {ns, name, classInfo->_imageName};
        return {};
    }
    return {._class = compileTimeClass.ToClass()};
}

static IL2CPP::Il2CppImage *MakeImage(std::string_view imageName) {
    auto newImg = BNM_I2C_NEW(Il2CppImage);
    memset(newImg, 0, sizeof(IL2CPP::Il2CppImage));

    if (imageName.ends_with(".dll")) imageName.remove_suffix(4);

    auto nameLen = imageName.size();
    newImg->nameNoExt = (char *) BNM_malloc(nameLen + 1);
    memcpy((void *) newImg->nameNoExt, (void *) imageName.data(), nameLen);
    ((char *) newImg->nameNoExt)[nameLen] = 0;

    newImg->name = (char *) BNM_malloc(nameLen + 5);
    memcpy((void *) newImg->name, (void *) imageName.data(), nameLen);
    auto nameEnd = ((char *) (newImg->name + nameLen));
    nameEnd[0] = '.';
    nameEnd[1] = 'd';
    nameEnd[2] = 'l';
    nameEnd[3] = 'l';
    nameEnd[4] = 0;

    newImg->assembly = nullptr;
    newImg->customAttributeCount = 0;
    newImg->typeCount = 0;
    newImg->exportedTypeCount = 0;
    newImg->token = 1;

    auto newAsm = BNM_I2C_NEW(Il2CppAssembly);
    memset(newAsm, 0, sizeof(IL2CPP::Il2CppAssembly));
    newAsm->image = newImg;
    newImg->assembly = newAsm;
    auto &aName = newAsm->aname;
    aName.name = newImg->name;
    aName.culture = nullptr;
    aName.public_key = nullptr;
    aName.public_key_token[0] = 0;
    aName.hash_len = 0;
    aName.hash_alg = 0;
    aName.flags = 0;
    newAsm->referencedAssemblyStart = -1;
    newAsm->referencedAssemblyCount = 0;

    newImg->nameToClassHashTable = (decltype(newImg->nameToClassHashTable)) - 0x424e4d;

    Internal::assembliesCache.push_back(newAsm);

    BNM_LOG_INFO("MakeImage added: %s", imageName.data());

    return newImg;
}

static IL2CPP::VirtualInvokeData *TryFindVirtualMethod(Class target, IL2CPP::MethodInfo *targetMethod, uint16_t *outSlot = nullptr) {
    for (uint16_t i = 0; i < target._data->vtable_count; ++i) {
        auto &it = target._data->vtable[i];
        if (it.method != targetMethod) continue;
        if (outSlot) *outSlot = i;
        return &it;
    }
    if (outSlot) *outSlot = 65535;
    return nullptr;
}

static IL2CPP::MethodInfo *ProcessCustomMethod(MANAGEMENT_STRUCTURES::CustomMethod *method, Class target, bool *hooked) {
    if (!target) return CreateMethod(method);

    struct _ClearMethod {
        MANAGEMENT_STRUCTURES::CustomMethod *method;
        ~_ClearMethod() {
            method->_parameterTypes.clear();
            method->_parameterTypes.shrink_to_fit();
        }
    } _clearMethod{method};

    auto parameters = (uint8_t) method->_parameterTypes.size();

    auto originalMethod = Internal::IterateMethods(target, [method, parameters](IL2CPP::MethodInfo *klassMethod) {
        if (method->_name != (klassMethod->name ? klassMethod->name : "") || klassMethod->parameters_count != parameters) return false;
        if (method->_skipTypeMatch) return true;

        for (uint8_t i = 0; i < parameters; ++i) {
            auto param = Internal::api.il2cpp_method_get_param(klassMethod, i);
            if (Class(param).GetClass() != method->_parameterTypes[i].ToIl2CppClass()) return false;
        }
        return true;
    });

    if (!originalMethod || PRIVATE_INTERNAL::GetMethodClass(originalMethod) != target._data) {
        bool isVirtual = originalMethod != nullptr && (originalMethod->flags & 0x0040) == 0x0040;
        auto parent = originalMethod;
        originalMethod = CreateMethod(method);

        if (!method->_copyTarget.empty()) {
            auto copyTarget = Internal::IterateMethods(target, [targetName = method->_copyTarget](IL2CPP::MethodInfo *method) { return targetName == (method->name ? method->name : ""); });
            if (copyTarget) originalMethod->token = copyTarget->token;
        }

        bool hasNonVirtualParent = !isVirtual && parent;

        if (hasNonVirtualParent) {
            method->_origin = parent;
            method->_originalAddress = (void *) parent->methodPointer;
            return originalMethod;
        }

        bool canDoVirtualHook = hooked && isVirtual;

        if (!canDoVirtualHook) return originalMethod;

        uint16_t slot{};
        if (auto vTable = TryFindVirtualMethod(target, parent, &slot); vTable != nullptr) {
            method->_origin = parent;
            method->_originalAddress = (void *) parent->methodPointer;
            originalMethod->slot = slot;
            vTable->methodPtr = (void (*)()) method->_address;
            vTable->method = originalMethod;
            return originalMethod;
        }
    }

    if (method->_isInvokeHook) {
        method->_origin = originalMethod;
        method->_originalAddress = (void *) originalMethod->methodPointer;
        originalMethod->methodPointer = (void (*)()) method->_address;
        if (hooked) *hooked = true;
        return originalMethod;
    }

    if ((originalMethod->flags & 0x0040) == 0 || method->_isBasicHook) goto SKIP_NON_VIRTUAL;

    if (auto vTable = TryFindVirtualMethod(target, originalMethod); vTable != nullptr) {
        method->_origin = (IL2CPP::MethodInfo *) vTable->method;
        method->_originalAddress = (void *) method->_origin->methodPointer;
        vTable->methodPtr = (void (*)()) method->_address;
        if (hooked) *hooked = true;
        return originalMethod;
    }

    SKIP_NON_VIRTUAL:

    method->_origin = originalMethod;
    ::BasicHook((void *) originalMethod->methodPointer, method->_address, method->_originalAddress);

    if (hooked) *hooked = true;

    return originalMethod;
}

static IL2CPP::MethodInfo *CreateMethod(MANAGEMENT_STRUCTURES::CustomMethod *method) {
    auto *myInfo = BNM_I2C_NEW(MethodInfo);
    memset(myInfo, 0, sizeof(IL2CPP::MethodInfo));
    myInfo->methodPointer = (decltype(myInfo->methodPointer)) method->_address;
    myInfo->invoker_method = (decltype(myInfo->invoker_method)) method->_invoker;
    myInfo->parameters_count = method->_parameterTypes.size();

    myInfo->virtualMethodPointer = (decltype(myInfo->virtualMethodPointer)) method->_address;

    auto name = (char *) BNM_malloc(method->_name.size() + 1);
    memcpy((void *) name, method->_name.data(), method->_name.size());
    name[method->_name.size()] = 0;
    myInfo->name = name;

    myInfo->flags = 0x0006 | 0x0080;
    if (method->_isStatic) myInfo->flags |= 0x0010;
    if (method->_name == Internal::constructorName) myInfo->flags |= 0x0800 | 0x1000;

    myInfo->is_generic = false;
    myInfo->is_inflated = false;

    myInfo->slot = 65535;

    myInfo->token = 0;

    auto methodType = method->_returnType.ToClass();
    if (!methodType) methodType = Internal::vmData.Object;
    myInfo->return_type = methodType.GetIl2CppType();

    auto argsCount = myInfo->parameters_count;
    if (argsCount) {
        auto &types = method->_parameterTypes;
        auto parameters = (IL2CPP::Il2CppType **) BNM_malloc(argsCount * sizeof(IL2CPP::Il2CppType *));

        myInfo->parameters = (const IL2CPP::Il2CppType **) parameters;
        for (uint8_t p = 0; p < argsCount; ++p) {
            auto parameter = BNM_I2C_NEW(Il2CppType);

            auto type = p < types.size() ? types[p].ToClass() : Internal::vmData.Object;
            if (!type) type = Internal::vmData.Object;
            *parameter = *type.GetIl2CppType();

            parameters[p] = parameter;
        }
    }
    myInfo->rgctx_data = nullptr;
    myInfo->genericMethod = nullptr;

    method->_parameterTypes.clear();
    method->_parameterTypes.shrink_to_fit();

    return myInfo;
}

static void SetupField(IL2CPP::FieldInfo *newField, MANAGEMENT_STRUCTURES::CustomField *field) {
    auto name = field->_name;
    auto len = name.size();
    newField->name = (char *) BNM_malloc(len + 1);
    memcpy((void *) newField->name, name.data(), len);
    ((char *) newField->name)[len] = 0;

    newField->type = BNM_I2C_NEW(Il2CppType);
    auto fieldType = field->_type.ToClass();
    if (!fieldType) fieldType = Internal::vmData.Object;

    auto newType = (IL2CPP::Il2CppType *) newField->type;
    *newType = *fieldType.GetIl2CppType();
    newType->attrs = newField->token = 0x0006;
}

static void SetupClassOwner(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *owner) {
    if (!owner) return;

    auto oldOwner = target->declaringType;
    auto oldInnerList = owner->nestedTypes;

    target->declaringType = owner;

    auto newInnerList = (IL2CPP::Il2CppClass **) BNM_malloc(sizeof(IL2CPP::Il2CppClass) * (owner->nested_type_count + 1));
    if (owner->nested_type_count) memcpy(newInnerList, owner->nestedTypes, sizeof(IL2CPP::Il2CppClass) * owner->nested_type_count);
    newInnerList[owner->nested_type_count++] = target;
    owner->nestedTypes = newInnerList;

    if ((owner->flags & BNM_CLASS_ALLOCATED_INNER_FLAG) == BNM_CLASS_ALLOCATED_INNER_FLAG) BNM_free(oldInnerList);
    owner->flags |= (uint32_t) BNM_CLASS_ALLOCATED_INNER_FLAG;

    if (oldOwner) {
        oldInnerList = oldOwner->nestedTypes;
        newInnerList = (IL2CPP::Il2CppClass **) BNM_malloc(sizeof(IL2CPP::Il2CppClass) * (oldOwner->nested_type_count - 1));
        uint8_t skipped = 0;
        for (uint16_t i = 0; i < oldOwner->nested_type_count; ++i) {
            if (skipped == 0) if (skipped = (oldInnerList[i] == target); skipped) continue;
            newInnerList[i - skipped] = oldInnerList[i];
        }
        oldOwner->nestedTypes = newInnerList;
        --oldOwner->nested_type_count;

        if ((oldOwner->flags & BNM_CLASS_ALLOCATED_INNER_FLAG) == BNM_CLASS_ALLOCATED_INNER_FLAG) BNM_free(oldInnerList);
        oldOwner->flags |= (uint32_t) BNM_CLASS_ALLOCATED_INNER_FLAG;
    }
}

static void SetupClassParent(IL2CPP::Il2CppClass *target, IL2CPP::Il2CppClass *parent) {
    if (!parent) [[unlikely]] return;

    if ((target->flags & BNM_CLASS_ALLOCATED_HIERARCHY_FLAG) == BNM_CLASS_ALLOCATED_HIERARCHY_FLAG) BNM_free(target->typeHierarchy);
    target->flags |= (uint32_t) BNM_CLASS_ALLOCATED_HIERARCHY_FLAG;

    target->typeHierarchyDepth = parent->typeHierarchyDepth + 1;
    target->typeHierarchy = (IL2CPP::Il2CppClass **) BNM_malloc(target->typeHierarchyDepth * sizeof(IL2CPP::Il2CppClass *));
    if (parent->typeHierarchyDepth) memcpy(target->typeHierarchy, parent->typeHierarchy, parent->typeHierarchyDepth * sizeof(IL2CPP::Il2CppClass *));
    target->typeHierarchy[parent->typeHierarchyDepth] = target;
    target->parent = parent;
}

static void GetAllInterfaces(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface, std::vector<IL2CPP::Il2CppClass *> &outInterfaces) {
    Internal::api.il2cpp_runtime_class_init(interface);
    if (!HasInterface(parent, interface)) outInterfaces.push_back(interface);
    if (!interface->interfaces_count || interface->interfaces_count == (uint16_t) - 1) return;

    for (uint16_t i = 0; i < interface->interfaces_count; ++i) GetAllInterfaces(parent, interface->implementedInterfaces[i], outInterfaces);
}

static bool HasInterface(IL2CPP::Il2CppClass *parent, IL2CPP::Il2CppClass *interface) {
    if (!parent || !interface) return false;
    for (uint16_t i = 0; i < parent->interfaces_count; ++i) if (parent->implementedInterfaces[i] == interface) return true;
    if (parent->parent) return HasInterface(parent->parent, interface);
    return false;
}

static void SetupTypes(IL2CPP::Il2CppClass *target) {
    IL2CPP::Il2CppType classType;
    memset(&classType, 0, sizeof(IL2CPP::Il2CppType));
    classType.type = IL2CPP::IL2CPP_TYPE_CLASS;
    classType.attrs = 0x0;
    classType.pinned = 0;
    classType.byref = 0;
    classType.num_mods = 31;
    classType.data.dummy = target;

    target->this_arg = target->byval_arg = classType;
    target->this_arg.byref = 1;
}

IL2CPP::Il2CppClass *(*old_Class$$FromIl2CppType)(IL2CPP::Il2CppType *type){};
IL2CPP::Il2CppClass *(*old_Type$$GetClassOrElementClass)(IL2CPP::Il2CppType *type){};
IL2CPP::Il2CppClass *(*old_Class$$FromName)(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name){};

IL2CPP::Il2CppClass *Class$$FromIl2CppType(IL2CPP::Il2CppType *type) {
    if (!type) return nullptr;
    if (type->num_mods == 31) return (IL2CPP::Il2CppClass *) type->data.dummy;
    return old_Class$$FromIl2CppType ? old_Class$$FromIl2CppType(type) : nullptr;
}

IL2CPP::Il2CppClass *Type$$GetClassOrElementClass(IL2CPP::Il2CppType *type) {
    if (!type) return nullptr;
    if (type->num_mods == 31) return (IL2CPP::Il2CppClass *) type->data.dummy;
    return old_Type$$GetClassOrElementClass ? old_Type$$GetClassOrElementClass(type) : nullptr;
}

IL2CPP::Il2CppClass *Class$$FromName(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name) {
    auto &api = Internal::api;
    if (!image) return nullptr;

    IL2CPP::Il2CppClass *ret = nullptr;

    if (image->nameToClassHashTable != (decltype(image->nameToClassHashTable)) - 0x424e4d)
        ret = old_Class$$FromName ? old_Class$$FromName(image, namespace_, name) : api.il2cpp_class_from_name(image, namespace_, name);

    if (!ret) ForEachClassInImageImpl(image, [namespace_, name, &ret](IL2CPP::Il2CppClass *cls) -> bool {
        auto ns = api.il2cpp_class_get_namespace(cls);
        auto clsName = api.il2cpp_class_get_name(cls);
        if (!strcmp(namespace_, ns ? ns : "") && !strcmp(name, clsName ? clsName : "")) {
            ret = cls;
            return true;
        }
        return false;
    });

    return ret;
}

}

namespace BNM::Internal {

void (*orig_Image$$GetTypes)(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<BNM::IL2CPP::Il2CppClass *> *target){};

void Image$$GetTypes(const IL2CPP::Il2CppImage *image, bool, std::vector<BNM::IL2CPP::Il2CppClass *> *target) {
    if (!image || !target) return;
    if (image->nameToClassHashTable != (decltype(image->nameToClassHashTable)) - 0x424e4d && orig_Image$$GetTypes)
        orig_Image$$GetTypes(image, false, target);
    auto it = ClassesManagement::bnmClassesMap.find((BNM_PTR) image);
    if (it == ClassesManagement::bnmClassesMap.end()) return;
    for (auto cls : it->second) target->push_back(cls);
}

MANAGEMENT_STRUCTURES::CustomClass coroutineIEClass{};
MANAGEMENT_STRUCTURES::CustomClass coroutineWaitClass{};
BNM::Class coroutineAsyncOperation{}, coroutineWaitForEndOfFrame{}, coroutineWaitForFixedUpdate{}, coroutineWaitForSeconds{}, coroutineWaitForSecondsRealtime{};

}

void BNM::Internal::SetupCoroutine() {
    using namespace BNM::MANAGEMENT_STRUCTURES;

    static CustomMethod ieMoveNext{}, ieReset{}, ieCurrent{};
    coroutineIEClass._size = sizeof(BNM::Coroutine::IEnumerator);
    coroutineIEClass._targetType = BNM::CompileTimeClassBuilder("BNM.Coroutine", "IEnumerator").Build();
    coroutineIEClass._baseType = {};
    coroutineIEClass._owner = {};
    coroutineIEClass._interfaces = {BNM::CompileTimeClassBuilder("System.Collections", "IEnumerator", "mscorlib.dll").Build()};
    AddClass(&coroutineIEClass);

    {
        constexpr auto p = &BNM::Coroutine::IEnumerator::MoveNext;
        ieMoveNext._address = *(void **) &p;
        ieMoveNext._invoker = (void *) &GetMethodInvoker<false, decltype(&BNM::Coroutine::IEnumerator::MoveNext)>::Invoke;
        ieMoveNext._name = "MoveNext";
        ieMoveNext._returnType = BNM::Defaults::Get<bool>();
        ieMoveNext._isStatic = false;
        coroutineIEClass._methods.push_back(&ieMoveNext);
    }
    {
        constexpr auto p = &BNM::Coroutine::IEnumerator::Reset;
        ieReset._address = *(void **) &p;
        ieReset._invoker = (void *) &GetMethodInvoker<false, decltype(&BNM::Coroutine::IEnumerator::Reset)>::Invoke;
        ieReset._name = "Reset";
        ieReset._returnType = BNM::Defaults::Get<void>();
        ieReset._isStatic = false;
        coroutineIEClass._methods.push_back(&ieReset);
    }
    {
        constexpr auto p = &BNM::Coroutine::IEnumerator::Current;
        ieCurrent._address = *(void **) &p;
        ieCurrent._invoker = (void *) &GetMethodInvoker<false, decltype(&BNM::Coroutine::IEnumerator::Current)>::Invoke;
        ieCurrent._name = "get_Current";
        ieCurrent._returnType = BNM::Defaults::Get<BNM::IL2CPP::Il2CppObject *>();
        ieCurrent._isStatic = false;
        coroutineIEClass._methods.push_back(&ieCurrent);
    }

    static CustomMethod cwMoveNext{}, cwReset{}, cwCurrent{};
    coroutineWaitClass._size = sizeof(CustomWait);
    coroutineWaitClass._targetType = BNM::CompileTimeClassBuilder("BNM.Coroutine", "CustomWait").Build();
    coroutineWaitClass._baseType = {};
    coroutineWaitClass._owner = {};
    coroutineWaitClass._interfaces = {BNM::CompileTimeClassBuilder("System.Collections", "IEnumerator", "mscorlib.dll").Build()};
    AddClass(&coroutineWaitClass);

    {
        constexpr auto p = &CustomWait::MoveNext;
        cwMoveNext._address = *(void **) &p;
        cwMoveNext._invoker = (void *) &GetMethodInvoker<false, decltype(&CustomWait::MoveNext)>::Invoke;
        cwMoveNext._name = "MoveNext";
        cwMoveNext._returnType = BNM::Defaults::Get<bool>();
        cwMoveNext._isStatic = false;
        coroutineWaitClass._methods.push_back(&cwMoveNext);
    }
    {
        constexpr auto p = &CustomWait::Reset;
        cwReset._address = *(void **) &p;
        cwReset._invoker = (void *) &GetMethodInvoker<false, decltype(&CustomWait::Reset)>::Invoke;
        cwReset._name = "Reset";
        cwReset._returnType = BNM::Defaults::Get<void>();
        cwReset._isStatic = false;
        coroutineWaitClass._methods.push_back(&cwReset);
    }
    {
        constexpr auto p = &CustomWait::Current;
        cwCurrent._address = *(void **) &p;
        cwCurrent._invoker = (void *) &GetMethodInvoker<true, decltype(&CustomWait::Current)>::Invoke;
        cwCurrent._name = "get_Current";
        cwCurrent._returnType = BNM::Defaults::Get<BNM::IL2CPP::Il2CppObject *>();
        cwCurrent._isStatic = false;
        coroutineWaitClass._methods.push_back(&cwCurrent);
    }
}

void BNM::Internal::LoadCoroutine() {
    auto image = BNM::Image("UnityEngine.CoreModule.dll");
    auto _namespace = "UnityEngine";

    coroutineAsyncOperation = BNM::Class(_namespace, "AsyncOperation", image);
    coroutineWaitForEndOfFrame = BNM::Class(_namespace, "WaitForEndOfFrame", image);
    coroutineWaitForFixedUpdate = BNM::Class(_namespace, "WaitForFixedUpdate", image);
    coroutineWaitForSeconds = BNM::Class(_namespace, "WaitForSeconds", image);
    coroutineWaitForSecondsRealtime = BNM::Class(_namespace, "WaitForSecondsRealtime", image);
}

void BNM::Coroutine::IEnumerator::Finalize() {
    try {
        if (_coroutine) {
            _coroutine.destroy();
            _coroutine = nullptr;
        }
        this->~IEnumerator();
    } catch (...) {
        BNM_LOG_ERR("IEnumerator::Finalize exception: %s", Internal::GetExceptionTypeName());
    }
}

bool BNM::Coroutine::IEnumerator::MoveNext() {
    try {
        if (!_coroutine) return false;
        _coroutine.resume();
        if (_coroutine.done()) {
            _coroutine.destroy();
            _coroutine = nullptr;
            return false;
        }
        _current = _coroutine.promise().value()._object;
        return true;
    } catch (...) {
        if (_coroutine) {
            _coroutine.destroy();
            _coroutine = nullptr;
        }
        BNM_LOG_ERR("IEnumerator::MoveNext exception: %s", Internal::GetExceptionTypeName());
        return false;
    }
}

BNM::Coroutine::IEnumerator *BNM::Coroutine::IEnumerator::Get() {
    try {
        auto inst = (BNM::Coroutine::IEnumerator *) BNM::Class(Internal::coroutineIEClass.myClass).CreateNewInstance();
        if (!inst) return nullptr;
        inst->_current = nullptr;
        inst->_coroutine = nullptr;
        std::swap(this->_coroutine, inst->_coroutine);
        return inst;
    } catch (...) {
        BNM_LOG_ERR("IEnumerator::Get exception: %s", Internal::GetExceptionTypeName());
        return nullptr;
    }
}

void BNM::Coroutine::IEnumerator::Reset() {}

BNM::IL2CPP::Il2CppObject *BNM::Coroutine::IEnumerator::Current() {
    return _current;
}

BNM::Coroutine::AsyncOperation::AsyncOperation(intptr_t operation) {
    _object = Internal::coroutineAsyncOperation.CreateNewObjectParameters(operation);
}

BNM::Coroutine::WaitForEndOfFrame::WaitForEndOfFrame() {
    _object = Internal::coroutineWaitForEndOfFrame.CreateNewInstance();
}

BNM::Coroutine::WaitForFixedUpdate::WaitForFixedUpdate() {
    _object = Internal::coroutineWaitForFixedUpdate.CreateNewInstance();
}

BNM::Coroutine::WaitForSeconds::WaitForSeconds(float seconds) {
    _object = Internal::coroutineWaitForSeconds.CreateNewObjectParameters(seconds);
}

BNM::Coroutine::WaitForSecondsRealtime::WaitForSecondsRealtime(float seconds) {
    _object = Internal::coroutineWaitForSecondsRealtime.CreateNewObjectParameters(seconds);
}

BNM::Coroutine::WaitUntil::WaitUntil(const std::function<bool()> &function) {
    auto obj = (CustomWait *) BNM::Class(Internal::coroutineWaitClass.myClass).CreateNewInstance();
    if (obj) {
        obj->_func = function;
        obj->_isUntil = true;
    }
    _object = obj;
}

BNM::Coroutine::WaitWhile::WaitWhile(const std::function<bool()> &function) {
    auto obj = (CustomWait *) BNM::Class(Internal::coroutineWaitClass.myClass).CreateNewInstance();
    if (obj) {
        obj->_func = function;
        obj->_isUntil = false;
    }
    _object = obj;
}

void Utils::LogCompileTimeClass(const CompileTimeClass &compileTimeClass) {
    for (auto info : compileTimeClass._stack) {
        switch (info->_baseType) {
            case CompileTimeClass::_BaseType::Class: {
                auto classInfo = (CompileTimeClass::_ClassInfo *) info; (void) classInfo;
                BNM_LOG_ERR("\tClass( imageName: \"%s\", namespace: \"%s\", name: \"%s\") - %s", classInfo->_imageName, classInfo->_namespace, classInfo->_name, compileTimeClass._loadedClass.str().data());
            } break;
            case CompileTimeClass::_BaseType::Inner: {
                auto innerInfo = (CompileTimeClass::_InnerInfo *) info; (void) innerInfo;
                BNM_LOG_ERR("\tInner( name: \"%s\") - %s", innerInfo->_name, compileTimeClass._loadedClass.str().data());
            } break;
            case CompileTimeClass::_BaseType::Modifier:
            case CompileTimeClass::_BaseType::Generic:
            case CompileTimeClass::_BaseType::None:
            case CompileTimeClass::_BaseType::MaxCount: break;
        }
    }
}

void Loading::AddOnLoadedEvent(void (*event)()) {
    if (event) Internal::onLoadedEvents.push_back(event);
}

void Loading::ClearOnLoadedEvents() {
    Internal::onLoadedEvents.clear();
}

