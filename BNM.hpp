#pragma once
#if __cplusplus < 202002L
#error "BNM-Free requires C++20 and above!"
#endif

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <csetjmp>
#include <csignal>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <type_traits>
#include <initializer_list>
#include <limits>
#include <coroutine>
#include <dlfcn.h>
#include <android/log.h>
#include <jni.h>

#if defined(__LP64__)
typedef long BNM_INT_PTR;
typedef unsigned long BNM_PTR;
#else
typedef int BNM_INT_PTR;
typedef unsigned int BNM_PTR;
#endif

#define BNM_OBFUSCATE(str) str
#define BNM_OBFUSCATE_TMP(str) str
#define BNM_VER "1.0.0"

#define BNM_LOG_INFO(...) ((void)__android_log_print(4, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_DEBUG(...) ((void)__android_log_print(3, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_DEBUG_IF(condition, ...) if (condition) ((void)__android_log_print(3, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_ERR(...) ((void)__android_log_print(6, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_ERR_IF(condition, ...) if (condition) ((void)__android_log_print(6, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_WARN(...) ((void)__android_log_print(5, "BNM-Free", __VA_ARGS__))
#define BNM_LOG_WARN_IF(condition, ...) if (condition) ((void)__android_log_print(5, "BNM-Free", __VA_ARGS__))

#ifdef BNM_USE_DOBBY
#include <dobby.h>
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &oldBytes) {
    if ((void *) ptr != nullptr) DobbyHook((void *) ptr, (dobby_dummy_func_t) newMethod, (dobby_dummy_func_t *) &oldBytes);
    return (void *) ptr;
}
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &&oldBytes) {
    if ((void *) ptr != nullptr) DobbyHook((void *) ptr, (dobby_dummy_func_t) newMethod, (dobby_dummy_func_t *) &oldBytes);
    return (void *) ptr;
}
template<typename PTR_T>
inline void Unhook(PTR_T ptr) {
    if ((void *) ptr != nullptr) DobbyDestroy((void *) ptr);
}
#elif defined(BNM_USE_SHADOWHOOK)
#include <shadowhook.h>
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &oldBytes) {
    if ((void *) ptr != nullptr) return shadowhook_hook_func_addr((void *) ptr, (void *) newMethod, (void **) &oldBytes);
    return nullptr;
}
template<typename PTR_T, typename NEW_T, typename T_OLD>
inline void *BasicHook(PTR_T ptr, NEW_T newMethod, T_OLD &&oldBytes) {
    if ((void *) ptr != nullptr) return shadowhook_hook_func_addr((void *) ptr, (void *) newMethod, (void **) &oldBytes);
    return nullptr;
}
template<typename PTR_T>
inline void Unhook(PTR_T ptr) {
    if ((void *) ptr != nullptr) shadowhook_unhook((void *) ptr);
}
#else
#error "No hooking software! Define BNM_USE_DOBBY or BNM_USE_SHADOWHOOK"
#endif

#define BNM_dlopen dlopen
#define BNM_dlsym dlsym
#define BNM_dlclose dlclose
#define BNM_dladdr dladdr

#define BNM_malloc malloc
#define BNM_free free

namespace BNM {

namespace IL2CPP {

typedef void (*Il2CppMethodPointer)();
typedef size_t il2cpp_array_size_t;
typedef int32_t il2cpp_array_lower_bound_t;
typedef uint16_t Il2CppChar;
typedef uint32_t Il2CppGCHandle;
typedef int32_t TypeDefinitionIndex;
typedef int32_t GenericParameterIndex;
typedef int32_t ImageIndex;
typedef int32_t il2cpp_hresult_t;
typedef int32_t Il2CppStat;
typedef int32_t Il2CppGCMode;
typedef int32_t Il2CppRuntimeUnhandledExceptionPolicy;
typedef int32_t Il2CppProfileFlags;
typedef uint32_t Il2CppProfileFunc;
typedef uint32_t Il2CppProfileMethodFunc;
typedef uint32_t Il2CppProfileAllocFunc;
typedef uint32_t Il2CppProfileGCFunc;
typedef uint32_t Il2CppProfileGCResizeFunc;
typedef uint32_t Il2CppProfileFileIOFunc;
typedef uint32_t Il2CppProfileThreadFunc;
typedef uint32_t Il2CppFrameWalkFunc;
typedef uint32_t Il2CppBacktraceFunc;
typedef uint32_t il2cpp_register_object_callback;
typedef uint32_t il2cpp_liveness_reallocate_callback;
typedef uint32_t il2cpp_WorldChangedCallback;
typedef uint32_t Il2CppSetFindPlugInCallback;
typedef uint32_t Il2CppLogCallback;

struct Il2CppMemoryCallbacks;
struct Il2CppProfiler;
struct Il2CppStackFrameInfo;
struct Il2CppMethodDebugInfo;
struct Il2CppCustomAttrInfo;
struct Il2CppManagedMemorySnapshot;
struct Il2CppDebuggerTransport;
struct Il2CppInternalThread;
struct Il2CppAppDomain;
struct Il2CppAppDomainSetup;
struct Il2CppAppContext;
struct Il2CppDelegate;
struct Il2CppVTable;
struct MonitorData;
struct Il2CppNameToTypeHandleHashTable;
struct Il2CppCodeGenModule;
struct Il2CppInteropData;
struct Il2CppRGCTXData;
struct ___Il2CppMetadataTypeHandle;
struct ___Il2CppMetadataGenericParameterHandle;
struct Il2CppMetadataRegistration;
struct Il2CppCodeRegistration;
struct Il2CppGenericClass;
struct Il2CppGenericInst;
struct Il2CppGenericMethod;
struct Il2CppType;
struct Il2CppClass;
struct MethodInfo;
struct FieldInfo;
struct PropertyInfo;
struct EventInfo;
struct Il2CppImage;
struct Il2CppAssembly;
struct Il2CppArray;
struct Il2CppString;
struct Il2CppObject;
struct Il2CppException;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;
struct Il2CppThread;
struct Il2CppDomain;
struct Il2CppArrayBounds;

typedef const ___Il2CppMetadataTypeHandle *Il2CppMetadataTypeHandle;
typedef const ___Il2CppMetadataGenericParameterHandle *Il2CppMetadataGenericParameterHandle;
typedef const void *Il2CppMetadataImageHandle;
typedef const void *Il2CppMetadataGenericContainerHandle;
typedef const void *Il2CppMetadataMethodDefinitionHandle;

enum Il2CppTypeEnum : int {
    IL2CPP_TYPE_END = 0x00,
    IL2CPP_TYPE_VOID = 0x01,
    IL2CPP_TYPE_BOOLEAN = 0x02,
    IL2CPP_TYPE_CHAR = 0x03,
    IL2CPP_TYPE_I1 = 0x04,
    IL2CPP_TYPE_U1 = 0x05,
    IL2CPP_TYPE_I2 = 0x06,
    IL2CPP_TYPE_U2 = 0x07,
    IL2CPP_TYPE_I4 = 0x08,
    IL2CPP_TYPE_U4 = 0x09,
    IL2CPP_TYPE_I8 = 0x0A,
    IL2CPP_TYPE_U8 = 0x0B,
    IL2CPP_TYPE_R4 = 0x0C,
    IL2CPP_TYPE_R8 = 0x0D,
    IL2CPP_TYPE_STRING = 0x0E,
    IL2CPP_TYPE_PTR = 0x0F,
    IL2CPP_TYPE_BYREF = 0x10,
    IL2CPP_TYPE_VALUETYPE = 0x11,
    IL2CPP_TYPE_CLASS = 0x12,
    IL2CPP_TYPE_VAR = 0x13,
    IL2CPP_TYPE_ARRAY = 0x14,
    IL2CPP_TYPE_GENERICINST = 0x15,
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I = 0x18,
    IL2CPP_TYPE_U = 0x19,
    IL2CPP_TYPE_FNPTR = 0x1B,
    IL2CPP_TYPE_OBJECT = 0x1C,
    IL2CPP_TYPE_SZARRAY = 0x1D,
    IL2CPP_TYPE_MVAR = 0x1E,
    IL2CPP_TYPE_CMOD_REQD = 0x1F,
    IL2CPP_TYPE_CMOD_OPT = 0x20,
    IL2CPP_TYPE_INTERNAL = 0x21,
    IL2CPP_TYPE_MODIFIER = 0x40,
    IL2CPP_TYPE_SENTINEL = 0x41,
    IL2CPP_TYPE_PINNED = 0x45,
    IL2CPP_TYPE_ENUM = 0x55,
    IL2CPP_TYPE_IL2CPP_TYPE_INDEX = 0xFF
};

typedef struct Il2CppArrayType {
    const Il2CppType *etype;
    uint8_t rank;
    uint8_t numsizes;
    uint8_t numlobounds;
    int *sizes;
    int *lobounds;
} Il2CppArrayType;

typedef struct Il2CppGenericInst {
    uint32_t type_argc;
    const Il2CppType **type_argv;
} Il2CppGenericInst;

typedef struct Il2CppGenericContext {
    const Il2CppGenericInst *class_inst;
    const Il2CppGenericInst *method_inst;
} Il2CppGenericContext;

typedef struct Il2CppGenericClass {
    const Il2CppType *type;
    Il2CppGenericContext context;
    Il2CppClass *cached_class;
} Il2CppGenericClass;

typedef struct Il2CppGenericMethod {
    const MethodInfo *methodDefinition;
    Il2CppGenericContext context;
} Il2CppGenericMethod;

typedef struct Il2CppType {
    union {
        void *dummy;
        TypeDefinitionIndex __klassIndex;
        Il2CppMetadataTypeHandle typeHandle;
        const Il2CppType *type;
        Il2CppArrayType *array;
        GenericParameterIndex __genericParameterIndex;
        Il2CppMetadataGenericParameterHandle genericParameterHandle;
        Il2CppGenericClass *generic_class;
    } data;
    unsigned int attrs : 16;
    Il2CppTypeEnum type : 8;
    unsigned int num_mods : 5;
    unsigned int byref : 1;
    unsigned int pinned : 1;
    unsigned int valuetype : 1;
} Il2CppType;

typedef struct Il2CppObject {
    union {
        Il2CppClass *klass;
        Il2CppVTable *vtable;
    };
    MonitorData *monitor;
} Il2CppObject;

typedef struct Il2CppArrayBounds {
    IL2CPP::il2cpp_array_size_t length;
    il2cpp_array_lower_bound_t lower_bound;
} Il2CppArrayBounds;

typedef struct Il2CppArray {
    Il2CppObject obj;
    Il2CppArrayBounds *bounds;
    IL2CPP::il2cpp_array_size_t max_length;
} Il2CppArray;

typedef struct Il2CppString {
    Il2CppObject object;
    int32_t length;
    Il2CppChar chars[32];
} Il2CppString;

typedef struct Il2CppReflectionType {
    Il2CppObject object;
    const Il2CppType *type;
} Il2CppReflectionType;

typedef struct Il2CppReflectionMethod {
    Il2CppObject object;
    const MethodInfo *method;
    Il2CppString *name;
    Il2CppReflectionType *reftype;
} Il2CppReflectionMethod;

typedef struct Il2CppRuntimeInterfaceOffsetPair {
    Il2CppClass *interfaceType;
    int32_t offset;
} Il2CppRuntimeInterfaceOffsetPair;

typedef struct VirtualInvokeData {
    Il2CppMethodPointer methodPtr;
    const MethodInfo *method;
} VirtualInvokeData;

typedef struct Il2CppClass {
    const Il2CppImage *image;
    void *gc_desc;
    const char *name;
    const char *namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass *element_class;
    Il2CppClass *castClass;
    Il2CppClass *declaringType;
    Il2CppClass *parent;
    Il2CppGenericClass *generic_class;
    Il2CppMetadataTypeHandle typeMetadataHandle;
    const Il2CppInteropData *interopData;
    Il2CppClass *klass;
    FieldInfo *fields;
    const EventInfo *events;
    const PropertyInfo *properties;
    const MethodInfo **methods;
    Il2CppClass **nestedTypes;
    Il2CppClass **implementedInterfaces;
    Il2CppRuntimeInterfaceOffsetPair *interfaceOffsets;
    void *static_fields;
    const Il2CppRGCTXData *rgctx_data;
    struct Il2CppClass **typeHierarchy;
    void *unity_user_data;
    Il2CppGCHandle initializationExceptionGCHandle;
    uint32_t cctor_started;
    uint32_t cctor_finished_or_no_cctor;
    __attribute__((aligned(8))) size_t cctor_thread;
    Il2CppMetadataGenericContainerHandle genericContainerHandle;
    uint32_t instance_size;
    uint32_t stack_slot_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;
    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;
    uint8_t typeHierarchyDepth;
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t packingSize;
    uint8_t initialized_and_no_error : 1;
    uint8_t initialized : 1;
    uint8_t enumtype : 1;
    uint8_t nullabletype : 1;
    uint8_t is_generic : 1;
    uint8_t has_references : 1;
    uint8_t init_pending : 1;
    uint8_t size_init_pending : 1;
    uint8_t size_inited : 1;
    uint8_t has_finalize : 1;
    uint8_t has_cctor : 1;
    uint8_t is_blittable : 1;
    uint8_t is_import_or_windows_runtime : 1;
    uint8_t is_vtable_initialized : 1;
    uint8_t is_byref_like : 1;
    VirtualInvokeData vtable[0];
} Il2CppClass;

typedef struct FieldInfo {
    const char *name;
    const Il2CppType *type;
    Il2CppClass *parent;
    int32_t offset;
    uint32_t token;
} FieldInfo;

typedef struct PropertyInfo {
    Il2CppClass *parent;
    const char *name;
    const MethodInfo *get;
    const MethodInfo *set;
    uint32_t attrs;
    uint32_t token;
} PropertyInfo;

typedef struct EventInfo {
    const char *name;
    const Il2CppType *eventType;
    Il2CppClass *parent;
    const MethodInfo *add;
    const MethodInfo *remove;
    const MethodInfo *raise;
    uint32_t token;
} EventInfo;

typedef void (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo *, void *, void **, void *);

typedef struct MethodInfo {
    Il2CppMethodPointer methodPointer;
    Il2CppMethodPointer virtualMethodPointer;
    InvokerMethod invoker_method;
    const char *name;
    Il2CppClass *klass;
    const Il2CppType *return_type;
    const Il2CppType **parameters;
    union {
        const Il2CppRGCTXData *rgctx_data;
        Il2CppMetadataMethodDefinitionHandle methodMetadataHandle;
    };
    union {
        const Il2CppGenericMethod *genericMethod;
        Il2CppMetadataGenericContainerHandle genericContainerHandle;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1;
    uint8_t is_inflated : 1;
    uint8_t wrapper_type : 1;
    uint8_t has_full_generic_sharing_signature : 1;
} MethodInfo;

typedef struct Il2CppAssemblyName {
    const char *name;
    const char *culture;
    const uint8_t *public_key;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[8];
} Il2CppAssemblyName;

typedef struct Il2CppImage {
    const char *name;
    const char *nameNoExt;
    Il2CppAssembly *assembly;
    uint32_t typeCount;
    uint32_t exportedTypeCount;
    uint32_t customAttributeCount;
    Il2CppMetadataImageHandle metadataHandle;
    Il2CppNameToTypeHandleHashTable *nameToClassHashTable;
    const Il2CppCodeGenModule *codeGenModule;
    uint32_t token;
    uint8_t dynamic;
} Il2CppImage;

typedef struct Il2CppAssembly {
    Il2CppImage *image;
    uint32_t token;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyName aname;
} Il2CppAssembly;

typedef struct Il2CppDomain {
    Il2CppAppDomain *domain;
    Il2CppAppDomainSetup *setup;
    Il2CppAppContext *default_context;
    Il2CppObject *ephemeron_tombstone;
    const char *friendly_name;
    uint32_t domain_id;
    volatile int threadpool_jobs;
    void *agent_info;
} Il2CppDomain;

typedef struct Il2CppThreadName {
    IL2CPP::Il2CppChar *chars;
    int32_t unused;
    int32_t length;
} Il2CppThreadName;

typedef struct Il2CppInternalThread {
    Il2CppObject obj;
    int lock_thread_id;
    void *handle;
    void *native_handle;
    Il2CppThreadName name;
    uint32_t state;
    Il2CppObject *abort_exc;
    int abort_state_handle;
    uint64_t tid;
    intptr_t debugger_thread;
    void **static_data;
    void *runtime_thread_info;
    Il2CppObject *current_appcontext;
    Il2CppObject *root_domain_thread;
    Il2CppArray *_serialized_principal;
    int _serialized_principal_version;
    void *appdomain_refs;
    int32_t interruption_requested;
    void *longlived;
    uint8_t threadpool_thread;
    uint8_t thread_interrupt_requested;
} Il2CppInternalThread;

typedef struct Il2CppThread {
    Il2CppObject obj;
    Il2CppInternalThread *internal_thread;
    Il2CppObject *start_obj;
    Il2CppException *pending_exception;
    Il2CppObject *principal;
    int32_t principal_version;
    Il2CppDelegate *delegate;
    Il2CppObject *executionContext;
    uint8_t executionContextBelongsToOuterScope;
} Il2CppThread;

typedef struct Il2CppException {
    Il2CppObject object;
    Il2CppString *className;
    Il2CppString *message;
    Il2CppObject *_data;
    struct Il2CppException *inner_ex;
    Il2CppString *_helpURL;
    Il2CppArray *trace_ips;
    Il2CppString *stack_trace;
    Il2CppString *remote_stack_trace;
    int remote_stack_index;
    Il2CppObject *_dynamicMethods;
    il2cpp_hresult_t hresult;
    Il2CppString *source;
    Il2CppObject *safeSerializationManager;
    Il2CppArray *captured_traces;
    Il2CppArray *native_trace_ips;
} Il2CppException;

}

typedef IL2CPP::Il2CppReflectionType MonoType;

struct Class;
struct MethodBase;
struct FieldBase;
struct PropertyBase;
struct EventBase;
struct Image;
struct CompileTimeClass;
struct DelegateBase;
struct MulticastDelegateBase;

namespace PRIVATE_INTERNAL {
    template<typename Ret>
    inline Ret ReturnEmpty() { return {}; }

    inline IL2CPP::Il2CppClass *&GetMethodClass(IL2CPP::MethodInfo *method) { return method->klass; }
}

namespace Internal {

struct States {
    uint8_t state : 1{};
    uint8_t lateInitAllowed : 1{};
};

#define BNM_DO_API(r, n, p) r(*n) p;
struct Il2CppApi {
    BNM_DO_API(int, il2cpp_init, (const char *domain_name));
    BNM_DO_API(int, il2cpp_init_utf16, (const IL2CPP::Il2CppChar *domain_name));
    BNM_DO_API(void, il2cpp_shutdown, ());
    BNM_DO_API(void, il2cpp_set_config_dir, (const char *config_path));
    BNM_DO_API(void, il2cpp_set_data_dir, (const char *data_path));
    BNM_DO_API(void, il2cpp_set_temp_dir, (const char *temp_path));
    BNM_DO_API(void, il2cpp_set_commandline_arguments, (int argc, const char *const argv[], const char *basedir));
    BNM_DO_API(void, il2cpp_set_commandline_arguments_utf16, (int argc, const IL2CPP::Il2CppChar *const argv[], const char *basedir));
    BNM_DO_API(void, il2cpp_set_config_utf16, (const IL2CPP::Il2CppChar *executablePath));
    BNM_DO_API(void, il2cpp_set_config, (const char *executablePath));
    BNM_DO_API(void, il2cpp_set_memory_callbacks, (IL2CPP::Il2CppMemoryCallbacks *callbacks));
    BNM_DO_API(IL2CPP::Il2CppImage *, il2cpp_get_corlib, ());
    BNM_DO_API(void, il2cpp_add_internal_call, (const char *name, IL2CPP::Il2CppMethodPointer method));
    BNM_DO_API(IL2CPP::Il2CppMethodPointer, il2cpp_resolve_icall, (const char *name));
    BNM_DO_API(void *, il2cpp_alloc, (size_t size));
    BNM_DO_API(void, il2cpp_free, (void *ptr));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_array_class_get, (IL2CPP::Il2CppClass *element_class, uint32_t rank));
    BNM_DO_API(uint32_t, il2cpp_array_length, (IL2CPP::Il2CppArray *array));
    BNM_DO_API(uint32_t, il2cpp_array_get_byte_length, (IL2CPP::Il2CppArray *array));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_array_new, (IL2CPP::Il2CppClass *elementTypeInfo, IL2CPP::il2cpp_array_size_t length));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_array_new_specific, (IL2CPP::Il2CppClass *arrayTypeInfo, IL2CPP::il2cpp_array_size_t length));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_array_new_full, (IL2CPP::Il2CppClass *array_class, IL2CPP::il2cpp_array_size_t *lengths, IL2CPP::il2cpp_array_size_t *lower_bounds));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_bounded_array_class_get, (IL2CPP::Il2CppClass *element_class, uint32_t rank, bool bounded));
    BNM_DO_API(int, il2cpp_array_element_size, (IL2CPP::Il2CppClass *array_class));
    BNM_DO_API(IL2CPP::Il2CppImage *, il2cpp_assembly_get_image, (IL2CPP::Il2CppAssembly *assembly));
    BNM_DO_API(void, il2cpp_class_for_each, (void(*klassReportFunc)(IL2CPP::Il2CppClass *klass, void *userData), void *userData));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_class_enum_basetype, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_generic, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_inflated, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_assignable_from, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *oklass));
    BNM_DO_API(bool, il2cpp_class_is_subclass_of, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *klassc, bool check_interfaces));
    BNM_DO_API(bool, il2cpp_class_has_parent, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *klassc));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_il2cpp_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_name, (IL2CPP::Il2CppImage *image, const char *namespaze, const char *name));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_system_type, (IL2CPP::Il2CppReflectionType *type));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_element_class, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::EventInfo *, il2cpp_class_get_events, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::FieldInfo *, il2cpp_class_get_fields, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_nested_types, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_interfaces, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::PropertyInfo *, il2cpp_class_get_properties, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::PropertyInfo *, il2cpp_class_get_property_from_name, (IL2CPP::Il2CppClass *klass, const char *name));
    BNM_DO_API(IL2CPP::FieldInfo *, il2cpp_class_get_field_from_name, (IL2CPP::Il2CppClass *klass, const char *name));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_class_get_methods, (IL2CPP::Il2CppClass *klass, void **iter));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_class_get_method_from_name, (IL2CPP::Il2CppClass *klass, const char *name, int argsCount));
    BNM_DO_API(const char *, il2cpp_class_get_name, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void, il2cpp_type_get_name_chunked, (IL2CPP::Il2CppType *type, void(*chunkReportFunc)(void *data, void *userData), void *userData));
    BNM_DO_API(char *, il2cpp_class_get_namespace, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_parent, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_get_declaring_type, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int32_t, il2cpp_class_instance_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(size_t, il2cpp_class_num_fields, (IL2CPP::Il2CppClass *enumKlass));
    BNM_DO_API(bool, il2cpp_class_is_valuetype, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int32_t, il2cpp_class_value_size, (IL2CPP::Il2CppClass *klass, uint32_t *align));
    BNM_DO_API(bool, il2cpp_class_is_blittable, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int, il2cpp_class_get_flags, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_abstract, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_interface, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int, il2cpp_class_array_element_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_class_from_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_class_get_type, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(uint32_t, il2cpp_class_get_type_token, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_has_attribute, (IL2CPP::Il2CppClass *klass, IL2CPP::Il2CppClass *attr_class));
    BNM_DO_API(bool, il2cpp_class_has_references, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(bool, il2cpp_class_is_enum, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppImage *, il2cpp_class_get_image, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(const char *, il2cpp_class_get_assemblyname, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(int, il2cpp_class_get_rank, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(uint32_t, il2cpp_class_get_data_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void *, il2cpp_class_get_static_field_data, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(size_t, il2cpp_class_get_bitmap_size, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void, il2cpp_class_get_bitmap, (IL2CPP::Il2CppClass *klass, size_t *bitmap));
    BNM_DO_API(bool, il2cpp_stats_dump_to_file, (const char *path));
    BNM_DO_API(uint64_t, il2cpp_stats_get_value, (IL2CPP::Il2CppStat stat));
    BNM_DO_API(IL2CPP::Il2CppDomain *, il2cpp_domain_get, ());
    BNM_DO_API(IL2CPP::Il2CppAssembly *, il2cpp_domain_assembly_open, (IL2CPP::Il2CppDomain *domain, const char *name));
    BNM_DO_API(IL2CPP::Il2CppAssembly **, il2cpp_domain_get_assemblies, (IL2CPP::Il2CppDomain *domain, size_t *size));
    BNM_DO_API(void, il2cpp_raise_exception, (IL2CPP::Il2CppException *));
    BNM_DO_API(IL2CPP::Il2CppException *, il2cpp_exception_from_name_msg, (IL2CPP::Il2CppImage *image, const char *name_space, const char *name, const char *msg));
    BNM_DO_API(IL2CPP::Il2CppException *, il2cpp_get_exception_argument_null, (char *arg));
    BNM_DO_API(void, il2cpp_format_exception, (IL2CPP::Il2CppException *ex, char *message, int message_size));
    BNM_DO_API(void, il2cpp_format_stack_trace, (IL2CPP::Il2CppException *ex, char *output, int output_size));
    BNM_DO_API(void, il2cpp_unhandled_exception, (IL2CPP::Il2CppException *));
    BNM_DO_API(void, il2cpp_native_stack_trace, (IL2CPP::Il2CppException *ex, uintptr_t **addresses, int *numFrames, char **imageUUID, char **imageName));
    BNM_DO_API(int, il2cpp_field_get_flags, (IL2CPP::FieldInfo *field));
    BNM_DO_API(const char *, il2cpp_field_get_name, (IL2CPP::FieldInfo *field));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_field_get_parent, (IL2CPP::FieldInfo *field));
    BNM_DO_API(size_t, il2cpp_field_get_offset, (IL2CPP::FieldInfo *field));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_field_get_type, (IL2CPP::FieldInfo *field));
    BNM_DO_API(void, il2cpp_field_get_value, (IL2CPP::Il2CppObject *obj, IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_field_get_value_object, (IL2CPP::FieldInfo *field, IL2CPP::Il2CppObject *obj));
    BNM_DO_API(bool, il2cpp_field_has_attribute, (IL2CPP::FieldInfo *field, IL2CPP::Il2CppClass *attr_class));
    BNM_DO_API(void, il2cpp_field_set_value, (IL2CPP::Il2CppObject *obj, IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(void, il2cpp_field_static_get_value, (IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(void, il2cpp_field_static_set_value, (IL2CPP::FieldInfo *field, void *value));
    BNM_DO_API(void, il2cpp_field_set_value_object, (IL2CPP::Il2CppObject *instance, IL2CPP::FieldInfo *field, IL2CPP::Il2CppObject *value));
    BNM_DO_API(bool, il2cpp_field_is_literal, (IL2CPP::FieldInfo *field));
    BNM_DO_API(void, il2cpp_gc_collect, (int maxGenerations));
    BNM_DO_API(int32_t, il2cpp_gc_collect_a_little, ());
    BNM_DO_API(void, il2cpp_gc_start_incremental_collection, ());
    BNM_DO_API(void, il2cpp_gc_disable, ());
    BNM_DO_API(void, il2cpp_gc_enable, ());
    BNM_DO_API(bool, il2cpp_gc_is_disabled, ());
    BNM_DO_API(void, il2cpp_gc_set_mode, (IL2CPP::Il2CppGCMode mode));
    BNM_DO_API(int64_t, il2cpp_gc_get_max_time_slice_ns, ());
    BNM_DO_API(void, il2cpp_gc_set_max_time_slice_ns, (int64_t maxTimeSlice));
    BNM_DO_API(bool, il2cpp_gc_is_incremental, ());
    BNM_DO_API(int64_t, il2cpp_gc_get_used_size, ());
    BNM_DO_API(int64_t, il2cpp_gc_get_heap_size, ());
    BNM_DO_API(void, il2cpp_gc_wbarrier_set_field, (IL2CPP::Il2CppObject *obj, void **targetAddress, void *object));
    BNM_DO_API(bool, il2cpp_gc_has_strict_wbarriers, ());
    BNM_DO_API(void, il2cpp_gc_set_external_allocation_tracker, (void(*func)(void *, size_t, int)));
    BNM_DO_API(void, il2cpp_gc_set_external_wbarrier_tracker, (void(*func)(void **)));
    BNM_DO_API(void, il2cpp_gc_foreach_heap, (void(*func)(void *data, void *userData), void *userData));
    BNM_DO_API(void, il2cpp_stop_gc_world, ());
    BNM_DO_API(void, il2cpp_start_gc_world, ());
    BNM_DO_API(void *, il2cpp_gc_alloc_fixed, (size_t size));
    BNM_DO_API(void, il2cpp_gc_free_fixed, (void *address));
    BNM_DO_API(uint32_t, il2cpp_gchandle_new, (IL2CPP::Il2CppObject *obj, bool pinned));
    BNM_DO_API(uint32_t, il2cpp_gchandle_new_weakref, (IL2CPP::Il2CppObject *obj, bool track_resurrection));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_gchandle_get_target, (uint32_t gchandle));
    BNM_DO_API(void, il2cpp_gchandle_free, (uint32_t gchandle));
    BNM_DO_API(void, il2cpp_gchandle_foreach_get_target, (void(*func)(void *data, void *userData), void *userData));
    BNM_DO_API(uint32_t, il2cpp_object_header_size, ());
    BNM_DO_API(uint32_t, il2cpp_array_object_header_size, ());
    BNM_DO_API(uint32_t, il2cpp_offset_of_array_length_in_array_object_header, ());
    BNM_DO_API(uint32_t, il2cpp_offset_of_array_bounds_in_array_object_header, ());
    BNM_DO_API(uint32_t, il2cpp_allocation_granularity, ());
    BNM_DO_API(void *, il2cpp_unity_liveness_allocate_struct, (IL2CPP::Il2CppClass *filter, int max_object_count, IL2CPP::il2cpp_register_object_callback callback, void *userdata, IL2CPP::il2cpp_liveness_reallocate_callback reallocate));
    BNM_DO_API(void, il2cpp_unity_liveness_calculation_from_root, (IL2CPP::Il2CppObject *root, void *state));
    BNM_DO_API(void, il2cpp_unity_liveness_calculation_from_statics, (void *state));
    BNM_DO_API(void, il2cpp_unity_liveness_finalize, (void *state));
    BNM_DO_API(void, il2cpp_unity_liveness_free_struct, (void *state));
    BNM_DO_API(void *, il2cpp_unity_liveness_calculation_begin, (IL2CPP::Il2CppClass *filter, int max_object_count, IL2CPP::il2cpp_register_object_callback callback, void *userdata, IL2CPP::il2cpp_WorldChangedCallback onWorldStarted, IL2CPP::il2cpp_WorldChangedCallback onWorldStopped));
    BNM_DO_API(void, il2cpp_unity_liveness_calculation_end, (void *state));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_method_get_return_type, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_method_get_declaring_type, (IL2CPP::MethodInfo *method));
    BNM_DO_API(const char *, il2cpp_method_get_name, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_method_get_from_reflection, (IL2CPP::Il2CppReflectionMethod *method));
    BNM_DO_API(IL2CPP::Il2CppReflectionMethod *, il2cpp_method_get_object, (IL2CPP::MethodInfo *method, IL2CPP::Il2CppClass *refclass));
    BNM_DO_API(bool, il2cpp_method_is_generic, (IL2CPP::MethodInfo *method));
    BNM_DO_API(bool, il2cpp_method_is_inflated, (IL2CPP::MethodInfo *method));
    BNM_DO_API(bool, il2cpp_method_is_instance, (IL2CPP::MethodInfo *method));
    BNM_DO_API(uint32_t, il2cpp_method_get_param_count, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppType *, il2cpp_method_get_param, (IL2CPP::MethodInfo *method, uint32_t index));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_method_get_class, (IL2CPP::MethodInfo *method));
    BNM_DO_API(bool, il2cpp_method_has_attribute, (IL2CPP::MethodInfo *method, IL2CPP::Il2CppClass *attr_class));
    BNM_DO_API(uint32_t, il2cpp_method_get_flags, (IL2CPP::MethodInfo *method, uint32_t *iflags));
    BNM_DO_API(uint32_t, il2cpp_method_get_token, (IL2CPP::MethodInfo *method));
    BNM_DO_API(const char *, il2cpp_method_get_param_name, (IL2CPP::MethodInfo *method, uint32_t index));
    BNM_DO_API(uint32_t, il2cpp_property_get_flags, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_property_get_get_method, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_property_get_set_method, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(const char *, il2cpp_property_get_name, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_property_get_parent, (IL2CPP::PropertyInfo *prop));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_object_get_class, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(uint32_t, il2cpp_object_get_size, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_object_get_virtual_method, (IL2CPP::Il2CppObject *obj, IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_object_new, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void *, il2cpp_object_unbox, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_value_box, (IL2CPP::Il2CppClass *klass, void *data));
    BNM_DO_API(void, il2cpp_monitor_enter, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(bool, il2cpp_monitor_try_enter, (IL2CPP::Il2CppObject *obj, uint32_t timeout));
    BNM_DO_API(void, il2cpp_monitor_exit, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_monitor_pulse, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_monitor_pulse_all, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_monitor_wait, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(bool, il2cpp_monitor_try_wait, (IL2CPP::Il2CppObject *obj, uint32_t timeout));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_runtime_invoke, (IL2CPP::MethodInfo *method, void *obj, void **params, IL2CPP::Il2CppException **exc));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_runtime_invoke_convert_args, (IL2CPP::MethodInfo *method, void *obj, IL2CPP::Il2CppObject **params, int paramCount, IL2CPP::Il2CppException **exc));
    BNM_DO_API(void, il2cpp_runtime_class_init, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(void, il2cpp_runtime_object_init, (IL2CPP::Il2CppObject *obj));
    BNM_DO_API(void, il2cpp_runtime_object_init_exception, (IL2CPP::Il2CppObject *obj, IL2CPP::Il2CppException **exc));
    BNM_DO_API(void, il2cpp_runtime_unhandled_exception_policy_set, (IL2CPP::Il2CppRuntimeUnhandledExceptionPolicy value));
    BNM_DO_API(int32_t, il2cpp_string_length, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppChar *, il2cpp_string_chars, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new, (const char *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new_len, (const char *str, uint32_t length));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new_utf16, (const IL2CPP::Il2CppChar *text, int32_t len));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_new_wrapper, (const char *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_intern, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppString *, il2cpp_string_is_interned, (IL2CPP::Il2CppString *str));
    BNM_DO_API(IL2CPP::Il2CppThread *, il2cpp_thread_current, ());
    BNM_DO_API(IL2CPP::Il2CppThread *, il2cpp_thread_attach, (IL2CPP::Il2CppDomain *domain));
    BNM_DO_API(void, il2cpp_thread_detach, (IL2CPP::Il2CppThread *thread));
    BNM_DO_API(IL2CPP::Il2CppThread **, il2cpp_thread_get_all_attached_threads, (size_t *size));
    BNM_DO_API(bool, il2cpp_is_vm_thread, (IL2CPP::Il2CppThread *thread));
    BNM_DO_API(void, il2cpp_current_thread_walk_frame_stack, (IL2CPP::Il2CppFrameWalkFunc func, void *user_data));
    BNM_DO_API(void, il2cpp_thread_walk_frame_stack, (IL2CPP::Il2CppThread *thread, IL2CPP::Il2CppFrameWalkFunc func, void *user_data));
    BNM_DO_API(bool, il2cpp_current_thread_get_top_frame, (IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(bool, il2cpp_thread_get_top_frame, (IL2CPP::Il2CppThread *thread, IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(bool, il2cpp_current_thread_get_frame_at, (int32_t offset, IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(bool, il2cpp_thread_get_frame_at, (IL2CPP::Il2CppThread *thread, int32_t offset, IL2CPP::Il2CppStackFrameInfo *frame));
    BNM_DO_API(int32_t, il2cpp_current_thread_get_stack_depth, ());
    BNM_DO_API(int32_t, il2cpp_thread_get_stack_depth, (IL2CPP::Il2CppThread *thread));
    BNM_DO_API(void, il2cpp_override_stack_backtrace, (IL2CPP::Il2CppBacktraceFunc stackBacktraceFunc));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_type_get_object, (IL2CPP::Il2CppType *type));
    BNM_DO_API(int, il2cpp_type_get_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_type_get_class_or_element_class, (IL2CPP::Il2CppType *type));
    BNM_DO_API(const char *, il2cpp_type_get_name, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_is_byref, (IL2CPP::Il2CppType *type));
    BNM_DO_API(uint32_t, il2cpp_type_get_attrs, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_equals, (IL2CPP::Il2CppType *type, IL2CPP::Il2CppType *otherType));
    BNM_DO_API(const char *, il2cpp_type_get_assembly_qualified_name, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_is_static, (IL2CPP::Il2CppType *type));
    BNM_DO_API(bool, il2cpp_type_is_pointer_type, (IL2CPP::Il2CppType *type));
    BNM_DO_API(IL2CPP::Il2CppAssembly *, il2cpp_image_get_assembly, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(const char *, il2cpp_image_get_name, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(const char *, il2cpp_image_get_filename, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(IL2CPP::MethodInfo *, il2cpp_image_get_entry_point, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(size_t, il2cpp_image_get_class_count, (IL2CPP::Il2CppImage *image));
    BNM_DO_API(IL2CPP::Il2CppClass *, il2cpp_image_get_class, (IL2CPP::Il2CppImage *image, size_t index));
    BNM_DO_API(void, il2cpp_image_get_types, (IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *types));
    BNM_DO_API(IL2CPP::Il2CppManagedMemorySnapshot *, il2cpp_capture_memory_snapshot, ());
    BNM_DO_API(void, il2cpp_free_captured_memory_snapshot, (IL2CPP::Il2CppManagedMemorySnapshot *snapshot));
    BNM_DO_API(void, il2cpp_set_find_plugin_callback, (IL2CPP::Il2CppSetFindPlugInCallback method));
    BNM_DO_API(void, il2cpp_register_log_callback, (IL2CPP::Il2CppLogCallback method));
    BNM_DO_API(void, il2cpp_debugger_set_agent_options, (const char *options));
    BNM_DO_API(bool, il2cpp_is_debugger_attached, ());
    BNM_DO_API(void, il2cpp_register_debugger_agent_transport, (IL2CPP::Il2CppDebuggerTransport *debuggerTransport));
    BNM_DO_API(bool, il2cpp_debug_get_method_info, (IL2CPP::MethodInfo *, IL2CPP::Il2CppMethodDebugInfo *methodDebugInfo));
    BNM_DO_API(void, il2cpp_unity_install_unitytls_interface, (void *unitytlsInterfaceStruct));
    BNM_DO_API(IL2CPP::Il2CppCustomAttrInfo *, il2cpp_custom_attrs_from_class, (IL2CPP::Il2CppClass *klass));
    BNM_DO_API(IL2CPP::Il2CppCustomAttrInfo *, il2cpp_custom_attrs_from_method, (IL2CPP::MethodInfo *method));
    BNM_DO_API(IL2CPP::Il2CppObject *, il2cpp_custom_attrs_get_attr, (IL2CPP::Il2CppCustomAttrInfo *ainfo, IL2CPP::Il2CppClass *attr_klass));
    BNM_DO_API(bool, il2cpp_custom_attrs_has_attr, (IL2CPP::Il2CppCustomAttrInfo *ainfo, IL2CPP::Il2CppClass *attr_klass));
    BNM_DO_API(IL2CPP::Il2CppArray *, il2cpp_custom_attrs_construct, (IL2CPP::Il2CppCustomAttrInfo *cinfo));
    BNM_DO_API(void, il2cpp_custom_attrs_free, (IL2CPP::Il2CppCustomAttrInfo *ainfo));
    BNM_DO_API(void, il2cpp_class_set_userdata, (IL2CPP::Il2CppClass *klass, void *userdata));
    BNM_DO_API(int, il2cpp_class_get_userdata_offset, ());
    BNM_DO_API(void, il2cpp_set_default_thread_affinity, (int64_t affinity_mask));
};
#undef BNM_DO_API

extern Il2CppApi api;

extern States states;
extern void *il2cppLibraryHandle;
extern void *currentFinderData;
extern std::vector<IL2CPP::Il2CppAssembly *> assembliesCache;

typedef void *(*MethodFinder)(const char *name, void *userData);
extern MethodFinder currentFinderMethod;

void *BasicFinder(const char *name, void *userData);

IL2CPP::Il2CppImage *TryGetImage(const std::string_view &_name);
IL2CPP::Il2CppClass *TryGetClassInImage(const IL2CPP::Il2CppImage *image, const std::string_view &_namespace, const std::string_view &_name);
Class TryMakeGenericClass(Class genericType, const std::vector<CompileTimeClass> &templateTypes);
MethodBase TryMakeGenericMethod(const MethodBase &genericMethod, const std::vector<CompileTimeClass> &templateTypes);
Class GetPointer(Class target);
Class GetReference(Class target);
void *GetIl2CppMethod(const char *methodName);
void Load();
void SetupBNM();
void LateInit(void *il2cpp_class_from_il2cpp_type_addr);
void LoadDefaults();

template<class CompareMethod>
IL2CPP::MethodInfo *IterateMethods(Class target, CompareMethod compare);

bool CompareImageName(IL2CPP::Il2CppImage *image, const std::string_view &name);
std::vector<IL2CPP::Il2CppAssembly *> &GetAllAssemblies();

int BNM_il2cpp_init(const char *domain_name);
IL2CPP::Il2CppClass *BNM_Class$$FromIl2CppType(IL2CPP::Il2CppReflectionType *type);

extern void *BNM_il2cpp_init_origin;
extern int (*old_BNM_il2cpp_init)(const char *);
extern void *BNM_Class$$FromIl2CppType_origin;
extern IL2CPP::Il2CppClass *(*old_BNM_Class$$FromIl2CppType)(IL2CPP::Il2CppReflectionType *);

extern std::string_view constructorName;
extern IL2CPP::Il2CppClass *customListTemplateClass;
extern std::map<uint32_t, IL2CPP::Il2CppClass *> customListsMap;
extern int32_t finalizerSlot;
extern std::vector<void (*)()> onLoadedEvents;

void Image$$GetTypes(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *target);

void SetupCoroutine();
void LoadCoroutine();
const char *GetExceptionTypeName();
extern void (*classInitFunc)(IL2CPP::Il2CppClass *);
void Image$$GetTypes(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *target);
extern void (*orig_Image$$GetTypes)(const IL2CPP::Il2CppImage *image, bool exportedOnly, std::vector<IL2CPP::Il2CppClass *> *target);

}

namespace Loading {

void AllowLateInitHook();
bool TryLoadByJNI(JNIEnv *env, jobject context = nullptr);
bool TryLoadByDlfcnHandle(void *handle);
typedef void *(*MethodFinder)(const char *name, void *userData);
void SetMethodFinder(MethodFinder finderMethod, void *userData);
bool TryLoadByUsersFinder();
void TrySetupByUsersFinder();
void AddOnLoadedEvent(void (*event)());
void ClearOnLoadedEvents();

}

namespace PRIVATE_FieldUtils {
    void GetStaticValue(IL2CPP::FieldInfo *info, void *value);
    void SetStaticValue(IL2CPP::FieldInfo *info, void *value);
}

namespace Utils {
    template<typename T>
    inline bool IsAllocated(T x) {
        if (!x) return false;
        volatile char c = *(volatile char *) x;
        (void) c;
        return true;
    }

    template<typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    inline T CheckObj(T obj) {
        if (obj && IsAllocated(obj)) return obj;
        return nullptr;
    }

    template<typename T>
    inline T UnboxObject(T obj) { return (T) (void *) (((char *) obj) + sizeof(IL2CPP::Il2CppObject)); }

    template<typename MET_T, typename PTR_T>
    inline void InitFunc(MET_T &method, PTR_T ptr) {
        method = (MET_T) ptr;
    }

    void *OffsetInLib(void *offsetInMemory);
    void LogCompileTimeClass(const CompileTimeClass &compileTimeClass);
}

namespace Structures {

namespace Mono {

struct String {
    IL2CPP::Il2CppObject object;
    int32_t length;
    IL2CPP::Il2CppChar chars[32];

    inline std::string str() const;
    inline unsigned int GetHash() const;
    static String *Empty();
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-bool-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
    inline bool IsNullOrEmpty() const { return !this || !length; }
#if defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    inline operator std::string() const { return str(); }
};

template<typename T>
struct Array {
    IL2CPP::Il2CppObject obj;
    IL2CPP::Il2CppArrayBounds *bounds;
    IL2CPP::il2cpp_array_size_t max_length;

    inline T *At(IL2CPP::il2cpp_array_size_t index) { return (T *) ((char *) this + sizeof(IL2CPP::Il2CppArray) + sizeof(T) * index); }
    inline T &operator[](IL2CPP::il2cpp_array_size_t index) { return *(T *) ((char *) this + sizeof(IL2CPP::Il2CppArray) + sizeof(T) * index); }
    inline IL2CPP::il2cpp_array_size_t Size() const { return max_length; }
    inline void Destroy() { Internal::api.il2cpp_free(this); }
    inline T *GetItems() { return (T *) ((char *) this + sizeof(IL2CPP::Il2CppArray)); }
    static Array<T> *Create(IL2CPP::il2cpp_array_size_t size, bool zeroed = false);
    template<typename ...Args>
    static Array<T> *CreateWithItems(Args &&...args) {
        auto arr = Create(sizeof...(Args), false);
        T values[] = {static_cast<T>(args)...};
        for (IL2CPP::il2cpp_array_size_t i = 0; i < sizeof...(Args); ++i) (*arr)[i] = values[i];
        return arr;
    }
    inline void CopyFrom(T *source, IL2CPP::il2cpp_array_size_t count) {
        for (IL2CPP::il2cpp_array_size_t i = 0; i < count; ++i) (*this)[i] = source[i];
    }
};

template<typename T>
struct List {
    IL2CPP::Il2CppObject obj;
    Mono::Array<T> *_items;
    int32_t _size;
    int32_t _version;
    IL2CPP::Il2CppObject *_syncRoot;

    inline T &operator[](int32_t index) { return (*_items)[index]; }
    inline T *GetItems() { return _items ? _items->GetItems() : nullptr; }
    inline int32_t Count() const { return _size; }
};

}

namespace Unity {

struct Vector3;
struct Vector4;
struct Color;
struct Matrix4x4;

struct Vector2 {
    float x;
    float y;
    inline Vector2() : x(0), y(0) {}
    inline Vector2(float x, float y) : x(x), y(y) {}
    inline operator Vector3() const;
    static const Vector2 positiveInfinity;
    static const Vector2 negativeInfinity;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 one;
    static const Vector2 right;
    static const Vector2 up;
    static const Vector2 zero;
};

struct Vector3 {
    float x;
    float y;
    float z;
    inline Vector3() : x(0), y(0), z(0) {}
    inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline operator Vector4() const;
    static const Vector3 positiveInfinity;
    static const Vector3 negativeInfinity;
    static const Vector3 back;
    static const Vector3 down;
    static const Vector3 forward;
    static const Vector3 left;
    static const Vector3 one;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 zero;
};

struct Vector4 {
    float x;
    float y;
    float z;
    float w;
    inline Vector4() : x(0), y(0), z(0), w(0) {}
    inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    inline Vector4(Color c);
    static const Vector4 positiveInfinity;
    static const Vector4 negativeInfinity;
    static const Vector4 zero;
    static const Vector4 one;
};

struct Color {
    float r;
    float g;
    float b;
    float a;
    inline Color() : r(0), g(0), b(0), a(1) {}
    inline Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}
    inline Color(Vector4 v) : r(v.x), g(v.y), b(v.z), a(v.w) {}
    static const Color black;
    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color white;
    static const Color orange;
    static const Color yellow;
    static const Color cyan;
    static const Color magenta;
};

struct Color32 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    inline Color32() : r(0), g(0), b(0), a(255) {}
    inline Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
};

struct Ray {
    Vector3 m_Origin;
    Vector3 m_Direction;
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
    inline Quaternion() : x(0), y(0), z(0), w(1) {}
    inline Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    static const Quaternion identity;
};

struct Matrix3x3 {
    float m_Data[9];
    inline Matrix3x3() { memset(m_Data, 0, sizeof(m_Data)); }
    Matrix3x3(const Matrix4x4 &other);
    Matrix3x3 &operator=(const Matrix4x4 &other);
    Matrix3x3 &operator*=(const Matrix4x4 &inM);
    bool Invert();
    inline float *GetPtr() { return m_Data; }
    inline const float *GetPtr() const { return m_Data; }
    inline float &Get(int row, int col) { return m_Data[row * 3 + col]; }
};

struct Matrix4x4 {
    float m_Data[16];
    enum InitIdentity { kIdentity };
    inline Matrix4x4() { memset(m_Data, 0, sizeof(m_Data)); }
    inline Matrix4x4(InitIdentity) { memset(m_Data, 0, sizeof(m_Data)); m_Data[0] = m_Data[5] = m_Data[10] = m_Data[15] = 1; }
    inline float *GetPtr() { return m_Data; }
    inline const float *GetPtr() const { return m_Data; }
    inline float Get(int row, int col) const { return m_Data[row * 4 + col]; }
    static const Matrix4x4 identity;
};

struct RaycastHit {
    Vector3 m_Point;
    Vector3 m_Normal;
    uint32_t m_FaceID;
    float m_Distance;
    Vector2 m_UV;
    int32_t m_Collider;
    void *GetCollider() const;
};

struct RaycastHit2D {
    Vector2 m_Centroid;
    Vector2 m_Point;
    Vector2 m_Normal;
    float m_Distance;
    float m_Fraction;
    int32_t m_Collider;
    void *GetCollider() const;
};

}

}

Structures::Mono::String *CreateMonoString(const std::string_view &str);
void *GetExternMethod(const std::string_view &str);
bool IsLoaded();
void *GetIl2CppLibraryHandle();
bool AttachIl2Cpp();
IL2CPP::Il2CppThread *CurrentIl2CppThread();
void DetachIl2Cpp();
void *Allocate(size_t size);
void Free(void *ptr);

template<typename T>
inline T UnmarshalUnityObject(BNM_INT_PTR ptr) { return (T) ptr; }

struct Class {
    IL2CPP::Il2CppClass *_data{};

    inline constexpr Class() = default;
    inline Class(const Class &) = default;
    inline Class(const IL2CPP::Il2CppClass *_class) : _data((IL2CPP::Il2CppClass *) _class) {}
    inline Class(const CompileTimeClass &compileTimeClass);
    Class(const IL2CPP::Il2CppObject *object);
    Class(const IL2CPP::Il2CppType *type);
    Class(const MonoType *type);
    Class(const std::string_view &_namespace, const std::string_view &name);
    Class(const std::string_view &_namespace, const std::string_view &name, const Image &image);

    std::vector<Class> GetInnerClasses(bool includeParent = false) const;
    std::vector<FieldBase> GetFields(bool includeParent = false) const;
    std::vector<MethodBase> GetMethods(bool includeParent = false) const;
    std::vector<PropertyBase> GetProperties(bool includeParent = false) const;
    std::vector<EventBase> GetEvents(bool includeParent = false) const;

    MethodBase GetMethod(const std::string_view &name, int parameters = -1) const;
    MethodBase GetMethod(const std::string_view &name, const std::initializer_list<std::string_view> &parameterNames) const;
    MethodBase GetMethod(const std::string_view &name, const std::initializer_list<CompileTimeClass> &parameterTypes) const;

    PropertyBase GetProperty(const std::string_view &name) const;
    PropertyBase GetProperty(const std::string_view &name, const CompileTimeClass &type) const;
    Class GetInnerClass(const std::string_view &name) const;
    FieldBase GetField(const std::string_view &name) const;
    EventBase GetEvent(const std::string_view &name) const;

    Class GetParent() const;
    Class GetArray() const;
    Class GetPointer() const;
    Class GetReference() const;
    Class GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const;

    IL2CPP::Il2CppType *GetIl2CppType() const;
    MonoType *GetMonoType() const;
    CompileTimeClass GetCompileTimeClass() const;
    Image GetImage() const;

    template<typename T>
    inline T *GetData() const { return (T *) _data; }
    template<typename T>
    inline Structures::Mono::Array<T> *NewArray(IL2CPP::il2cpp_array_size_t size) const {
        return (Structures::Mono::Array<T> *) Internal::api.il2cpp_array_new(_data, size);
    }
    inline IL2CPP::Il2CppClass *GetClass() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    inline operator IL2CPP::Il2CppClass *() const { return GetClass(); }
    inline operator IL2CPP::Il2CppType *() const { return GetIl2CppType(); }
    inline operator MonoType *() const { return GetMonoType(); }
    inline operator CompileTimeClass() const;

    IL2CPP::Il2CppObject *CreateNewInstance() const;
    template<typename ...Args>
    inline IL2CPP::Il2CppObject *CreateNewObjectParameters(Args &&...args) const;

    inline void TryInit() const { if (_data && Internal::classInitFunc) Internal::classInitFunc(_data); }

    static IL2CPP::Il2CppObject *BoxObject(IL2CPP::Il2CppClass *_data, void *data);
    static IL2CPP::Il2CppArray *ArrayNew(IL2CPP::Il2CppClass *cls, IL2CPP::il2cpp_array_size_t length);
    static void *NewListInstance();
    static Class GetListClass();

    inline std::string str() const {
        if (!_data) return "nullptr";
        std::string result;
        if (_data->namespaze && strlen(_data->namespaze)) {
            result += _data->namespaze;
            result += ".";
        }
        result += _data->name ? _data->name : "nullptr";
        return result;
    }
};

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, IL2CPP::Il2CppClass *_class) { return IsA<BNM::IL2CPP::Il2CppObject *>((IL2CPP::Il2CppObject *) object, _class); }

template<>
bool IsA<IL2CPP::Il2CppObject *>(IL2CPP::Il2CppObject *object, IL2CPP::Il2CppClass *_class);

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, Class _class) { return IsA(object, _class.GetClass()); }

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, IL2CPP::Il2CppObject *_object) { if (!_object) return false; return IsA(object, _object->klass); }

template<typename T, typename = std::enable_if<std::is_pointer_v<T>>>
bool IsA(T object, MonoType *_type) { return IsA(object, Class(_type)); }

struct MethodBase {
    IL2CPP::MethodInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _isStatic : 1 = false;
    uint8_t _isVirtual : 1 = false;

    inline constexpr MethodBase() = default;
    inline MethodBase(const MethodBase &other) = default;
    MethodBase(const IL2CPP::MethodInfo *info);
    MethodBase(const IL2CPP::Il2CppReflectionMethod *reflectionMethod);

    MethodBase &SetInstance(IL2CPP::Il2CppObject *val);
    inline IL2CPP::MethodInfo *GetInfo() const { return _data; }
    inline BNM_PTR GetOffset() const { return _data ? (BNM_PTR) _data->methodPointer : 0; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }

    MethodBase GetGeneric(const std::initializer_list<CompileTimeClass> &templateTypes) const;
    MethodBase GetOverride() const;
    BNM::Class GetReturnType() const;
    BNM::Class GetParentClass() const;

    inline MethodBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline MethodBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }

    inline std::string str() const {
        if (!_data) return "nullptr";
        std::string result;
        result += Class(PRIVATE_INTERNAL::GetMethodClass(_data)).str();
        result += ".";
        result += _data->name ? _data->name : "nullptr";
        result += "(";
        result += std::to_string(_data->parameters_count);
        result += ")";
        return result;
    }
};

template<typename Ret>
struct Method : public MethodBase {
    inline constexpr Method() noexcept = default;
    template<typename OtherType>
    inline Method(const Method<OtherType> &other) : MethodBase(other) {}
    inline Method(const IL2CPP::MethodInfo *info) : MethodBase(info) {}
    inline Method(const IL2CPP::Il2CppReflectionMethod *reflectionMethod) : MethodBase(reflectionMethod) {}
    inline Method(const MethodBase &other) : MethodBase(other) {}

    inline Method<Ret> &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline Method<Ret> &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    template<typename ...Parameters>
    inline Ret Call(Parameters ...parameters) const {
        if (!_data) {
            BNM_LOG_ERR("Method::Call dead method");
            return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        }
        if (sizeof...(Parameters) != _data->parameters_count) {
            BNM_LOG_WARN("Method::Call param count mismatch: %s", str().c_str());
        }
        if (!_isStatic && !Utils::IsAllocated(_instance)) {
            BNM_LOG_ERR("Method::Call dead instance: %s", str().c_str());
            return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        }
        auto method = _data;
        if (!_isStatic) {
            return ((Ret(*)(IL2CPP::Il2CppObject *, Parameters...)) method->methodPointer)(_instance, parameters...);
        }
        if (_isStatic) {
            return ((Ret(*)(Parameters...)) method->methodPointer)(parameters...);
        }
        return ((Ret(*)(void *, Parameters...)) method->methodPointer)(nullptr, parameters...);
    }

    template<typename ...Parameters>
    inline Ret operator()(Parameters ...parameters) const { return Call(parameters...); }

    inline Ret Invoke() const {
        if (!_data) return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        IL2CPP::Il2CppException *exc = nullptr;
        auto ret = Internal::api.il2cpp_runtime_invoke(_data, _instance, nullptr, &exc);
        if (exc) BNM_LOG_ERR("Method::Invoke exception: %s", exc->message ? ((Structures::Mono::String *) exc->message)->str().c_str() : "unknown");
        if constexpr (!std::is_void_v<Ret>) return (Ret) ret;
    }

    template<typename ...Parameters>
    inline Ret Invoke(Parameters ...parameters) const {
        if (!_data) return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        void *args[] = {(void *) parameters...};
        IL2CPP::Il2CppException *exc = nullptr;
        auto ret = Internal::api.il2cpp_runtime_invoke(_data, _instance, args, &exc);
        if (exc) BNM_LOG_ERR("Method::Invoke exception: %s", exc->message ? ((Structures::Mono::String *) exc->message)->str().c_str() : "unknown");
        if constexpr (!std::is_void_v<Ret>) return (Ret) ret;
    }
};

struct FieldBase {
    IL2CPP::FieldInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _isConst : 1 = false;
    uint8_t _isStatic : 1 = false;
    uint8_t _isThreadStatic : 1 = false;
    uint8_t _isInStruct : 1 = false;

    inline constexpr FieldBase() = default;
    inline FieldBase(const FieldBase &other) = default;
    FieldBase(IL2CPP::FieldInfo *info);

    FieldBase &SetInstance(IL2CPP::Il2CppObject *val);
    inline IL2CPP::FieldInfo *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    void *GetFieldPointer() const;
    BNM::Class GetType() const;
    BNM::Class GetParentClass() const;
    inline std::string str() const {
        if (!_data) return "nullptr";
        std::string result;
        result += Class(_data->parent).str();
        result += ".";
        result += _data->name ? _data->name : "nullptr";
        return result;
    }

    inline FieldBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline FieldBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }
};

template<typename T>
struct Field : public FieldBase {
    inline constexpr Field() noexcept = default;
    template<typename OtherType>
    inline Field(const Field<OtherType> &other) : FieldBase(other) {}
    inline Field(IL2CPP::FieldInfo *info) : FieldBase(info) {}
    inline Field(const FieldBase &other) : FieldBase(other) {}

    inline T *GetPointer() const {
        auto ptr = GetFieldPointer();
        BNM_LOG_ERR_IF(ptr == nullptr, "Field::GetPointer dead: %s", _data ? str().c_str() : "null");
        return (T *) ptr;
    }

    inline T Get() const {
        BNM_LOG_ERR_IF(!_data, "Field::Get dead field");
        if (!_data) return {};
        if (_isThreadStatic || _isConst) {
            T val{};
            PRIVATE_FieldUtils::GetStaticValue(_data, (void *) &val);
            return val;
        }
        if (auto ptr = GetPointer(); ptr != nullptr) return *ptr;
        return {};
    }

    inline operator T() const { return Get(); }
    inline T operator()() const { return Get(); }

    inline void Set(T value) const {
        BNM_LOG_ERR_IF(!_data, "Field::Set dead field");
        if (!_data) return;
        if (_isConst) {
            BNM_LOG_ERR("Field::Set const field: %s", str().c_str());
            return;
        }
        if (_isThreadStatic) {
            PRIVATE_FieldUtils::SetStaticValue(_data, (void *) &value);
            return;
        }
        if (auto ptr = GetPointer(); ptr != nullptr) *ptr = value;
    }

    inline Field<T> &operator=(T value) { Set(std::move(value)); return *this; }
    inline Field<T> &operator=(const FieldBase &other) { _data = other._data; _instance = other._instance; _isConst = other._isConst; _isStatic = other._isStatic; _isThreadStatic = other._isThreadStatic; _isInStruct = other._isInStruct; return *this; }
};

struct PropertyBase {
    IL2CPP::PropertyInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _hasGetter : 1 = false;
    uint8_t _hasSetter : 1 = false;
    MethodBase _getter{};
    MethodBase _setter{};

    inline constexpr PropertyBase() = default;
    inline PropertyBase(const PropertyBase &other) = default;
    PropertyBase(const IL2CPP::PropertyInfo *info);
    inline IL2CPP::PropertyInfo *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    PropertyBase &SetInstance(IL2CPP::Il2CppObject *val);
    BNM::Class GetType() const;
    BNM::Class GetParentClass() const;
    inline std::string str() const {
        if (!_data) return "nullptr";
        return _data->name ? _data->name : "nullptr";
    }
    inline PropertyBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline PropertyBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }
    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }
};

template<typename T = bool>
struct Property : public PropertyBase {
    inline constexpr Property() = default;
    template<typename OtherType>
    inline Property(const Property<OtherType> &other) : PropertyBase(other) {}
    inline Property(const IL2CPP::PropertyInfo *info) : PropertyBase(info) {}
    inline Property(const PropertyBase &other) : PropertyBase(other) {}

    inline Property<T> &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline Property<T> &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    inline T Get() const {
        if (_hasGetter) return _getter.cast<Method<T>>()();
        BNM_LOG_ERR("Property::Get no getter: %s", str().c_str());
        return PRIVATE_INTERNAL::ReturnEmpty<T>();
    }
    inline operator T() const { return Get(); }
    inline T operator()() const { return Get(); }

    inline void Set(T value) {
        if (_hasSetter) {
            _setter.cast<Method<void>>()(value);
            return;
        }
        BNM_LOG_ERR("Property::Set no setter: %s", str().c_str());
    }
    inline Property<T> &operator=(T value) { Set(std::move(value)); return *this; }
    inline Property<T> &operator=(const PropertyBase &other) {
        _data = other._data;
        _instance = other._instance;
        _hasGetter = other._hasGetter;
        _hasSetter = other._hasSetter;
        _getter = other._getter;
        _setter = other._setter;
        return *this;
    }
};

struct EventBase {
    IL2CPP::EventInfo *_data{};
    IL2CPP::Il2CppObject *_instance{};
    uint8_t _hasAdd : 1 = false;
    uint8_t _hasRemove : 1 = false;
    uint8_t _hasRaise : 1 = false;
    MethodBase _add{};
    MethodBase _remove{};
    MethodBase _raise{};

    inline constexpr EventBase() = default;
    inline EventBase(const EventBase &other) = default;
    EventBase(const IL2CPP::EventInfo *info);
    inline IL2CPP::EventInfo *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator bool() const { return IsValid(); }
    EventBase &SetInstance(IL2CPP::Il2CppObject *instance);
    BNM::Class GetParentClass() const;
    inline std::string str() const {
        if (!_data) return "nullptr";
        return _data->name ? _data->name : "nullptr";
    }
    inline EventBase &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline EventBase &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }
    template<typename NewType>
    inline NewType &cast() const { return (NewType &) *this; }
};

template<typename Ret = void, typename ...Parameters>
struct Event : public EventBase {
    inline constexpr Event() = default;
    template<typename OtherType>
    inline Event(const Event<OtherType> &other) : EventBase(other) {}
    inline Event(const IL2CPP::EventInfo *info) : EventBase(info) {}
    inline Event(const EventBase &other) : EventBase(other) {}

    inline Event<Ret, Parameters...> &operator[](void *instance) { SetInstance((IL2CPP::Il2CppObject *) instance); return *this; }
    inline Event<Ret, Parameters...> &operator[](IL2CPP::Il2CppObject *instance) { SetInstance(instance); return *this; }

    inline void Add(DelegateBase *delegate) {
        if (_hasAdd) {
            _add.cast<Method<void>>()(delegate);
            return;
        }
        BNM_LOG_ERR("Event::Add no add method: %s", str().c_str());
    }
    inline Event<Ret, Parameters...> &operator+=(DelegateBase *delegate) { Add(delegate); return *this; }

    inline void Remove(DelegateBase *delegate) {
        if (_hasRemove) {
            _remove.cast<Method<void>>()(delegate);
            return;
        }
        BNM_LOG_ERR("Event::Remove no remove method: %s", str().c_str());
    }
    inline Event<Ret, Parameters...> &operator-=(DelegateBase *delegate) { Remove(delegate); return *this; }

    inline Ret Raise(Parameters ...parameters) const {
        if (_hasRaise) return _raise.cast<Method<Ret>>()(parameters...);
        BNM_LOG_ERR("Event::Raise no raise method: %s", str().c_str());
        return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
    }
    inline Ret operator()(Parameters ...parameters) const { return Raise(parameters...); }
    inline Event<Ret> &operator=(const EventBase &other) {
        _data = other._data;
        _instance = other._instance;
        _hasAdd = other._hasAdd;
        _hasRemove = other._hasRemove;
        _hasRaise = other._hasRaise;
        _add = other._add;
        _remove = other._remove;
        _raise = other._raise;
        return *this;
    }
};

struct Image {
    IL2CPP::Il2CppImage *_data{};

    inline constexpr Image() = default;
    inline Image(const Image &) = default;
    inline Image(const IL2CPP::Il2CppImage *image) : _data((IL2CPP::Il2CppImage *) image) {}
    Image(const std::string_view &name);
    Image(const IL2CPP::Il2CppAssembly *assembly);

    inline IL2CPP::Il2CppImage *GetInfo() const { return _data; }
    inline bool IsValid() const { return _data != nullptr; }
    inline operator IL2CPP::Il2CppImage *() const { return GetInfo(); }
    inline operator bool() const { return IsValid(); }
    inline std::string str() const { return _data ? (_data->name ? _data->name : "nullptr") : "nullptr"; }
    inline Class GetClass(const std::string_view &_namespace, const std::string_view &name) const { return Class(_namespace, name, *this); }
    std::vector<Class> GetClasses(bool includeInner = false) const;
    static std::vector<Image> GetImages();
};

template<typename Ret = void, typename ...Parameters>
struct Delegate;

struct DelegateBase {
    IL2CPP::Il2CppObject object;
    IL2CPP::MethodInfo *method;
    IL2CPP::Il2CppObject *target;

    MethodBase GetMethod() const;
    DelegateBase *Create(BNM::MethodBase method);
    inline IL2CPP::Il2CppObject *GetInstance() const { return target; }
};

struct MulticastDelegateBase {
    IL2CPP::Il2CppObject object;
    IL2CPP::Il2CppMethodPointer method_ptr;
    IL2CPP::Il2CppObject *target;
    IL2CPP::Il2CppArray *delegates;

    std::vector<MethodBase> GetMethods() const;
    void Add(DelegateBase *delegate);
    void Remove(DelegateBase *delegate);
    DelegateBase *Add(BNM::MethodBase method);
};

template<typename Ret, typename ...Parameters>
struct Delegate : public MulticastDelegateBase {
    inline Delegate() = default;
    inline Delegate(const Delegate &) = default;
    inline Ret Invoke(Parameters ...parameters) const {
        if (!method_ptr) {
            BNM_LOG_ERR("Delegate::Invoke dead delegate");
            return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        }
        return ((Ret(*)(Parameters...)) method_ptr)(parameters...);
    }
    inline Ret operator()(Parameters ...parameters) const { return Invoke(parameters...); }
};

namespace Exceptions {
    struct Exception {
        IL2CPP::Il2CppException *exception{};
        inline operator bool() const { return exception != nullptr; }
    };
    Exception TryInvoke(const std::function<void()> &func);
}

namespace Defaults {

namespace Internal {
    extern BNM::Class Void, Boolean, Byte, SByte, Int16, UInt16, Int32, UInt32, IntPtr, UIntPtr, Int64, UInt64, Single, Double, Decimal, String, Object, Type;
    extern BNM::Class Vector2, Vector3, Vector4, Color, Color32, Ray, Quaternion, Matrix3x3, Matrix4x4, RaycastHit, RaycastHit2D;
    extern BNM::Class UnityObject, MonoBehaviour;
}

struct DefaultTypeRef {
    BNM::Class *_reference{};
    inline constexpr DefaultTypeRef() = default;
    inline constexpr DefaultTypeRef(BNM::Class *reference) : _reference(reference) {}
    inline operator BNM::CompileTimeClass() const;
    inline operator BNM::Class() const { return _reference ? *_reference : BNM::Class{}; }
    inline BNM::Class ToClass() const { return _reference ? *_reference : BNM::Class{}; }
};

using byte = uint8_t;
using sbyte = int8_t;
using ushort = uint16_t;

template<typename T>
inline DefaultTypeRef Get() {
    if constexpr (std::is_same_v<T, void>) return {&Internal::Void};
    else if constexpr (std::is_same_v<T, bool>) return {&Internal::Boolean};
    else if constexpr (std::is_same_v<T, char>) return {&Internal::Byte};
    else if constexpr (std::is_same_v<T, int8_t>) return {&Internal::SByte};
    else if constexpr (std::is_same_v<T, uint8_t>) return {&Internal::Byte};
    else if constexpr (std::is_same_v<T, int16_t>) return {&Internal::Int16};
    else if constexpr (std::is_same_v<T, uint16_t>) return {&Internal::UInt16};
    else if constexpr (std::is_same_v<T, int32_t>) return {&Internal::Int32};
    else if constexpr (std::is_same_v<T, uint32_t>) return {&Internal::UInt32};
    else if constexpr (std::is_same_v<T, int64_t>) return {&Internal::Int64};
    else if constexpr (std::is_same_v<T, uint64_t>) return {&Internal::UInt64};
    else if constexpr (std::is_same_v<T, float>) return {&Internal::Single};
    else if constexpr (std::is_same_v<T, double>) return {&Internal::Double};
    else if constexpr (std::is_same_v<T, Structures::Mono::String *>) return {&Internal::String};
    else if constexpr (std::is_same_v<T, MonoType *>) return {&Internal::Type};
    else if constexpr (std::is_same_v<T, IL2CPP::Il2CppObject *>) return {&Internal::Object};
    else if constexpr (std::is_same_v<T, Structures::Unity::Vector2>) return {&Internal::Vector2};
    else if constexpr (std::is_same_v<T, Structures::Unity::Vector3>) return {&Internal::Vector3};
    else if constexpr (std::is_same_v<T, Structures::Unity::Vector4>) return {&Internal::Vector4};
    else if constexpr (std::is_same_v<T, Structures::Unity::Color>) return {&Internal::Color};
    else if constexpr (std::is_same_v<T, Structures::Unity::Color32>) return {&Internal::Color32};
    else if constexpr (std::is_same_v<T, Structures::Unity::Ray>) return {&Internal::Ray};
    else if constexpr (std::is_same_v<T, Structures::Unity::Quaternion>) return {&Internal::Quaternion};
    else if constexpr (std::is_same_v<T, Structures::Unity::Matrix3x3>) return {&Internal::Matrix3x3};
    else if constexpr (std::is_same_v<T, Structures::Unity::Matrix4x4>) return {&Internal::Matrix4x4};
    else if constexpr (std::is_same_v<T, Structures::Unity::RaycastHit>) return {&Internal::RaycastHit};
    else if constexpr (std::is_same_v<T, Structures::Unity::RaycastHit2D>) return {&Internal::RaycastHit2D};
    else if constexpr (std::is_same_v<T, IL2CPP::Il2CppArray *>) return {&Internal::Object};
    else return {&Internal::Object};
}

}

namespace UnityEngine {
    inline BNM::Class GetTypeByName(const std::string_view &name) {
        return Class("UnityEngine", name, Image("UnityEngine.CoreModule.dll"));
    }
    inline BNM::Class GetTypeByName(const std::string_view &module, const std::string_view &name) {
        return Class("UnityEngine", name, Image(module));
    }
    inline BNM::Class GetType(const std::string_view &name) { return GetTypeByName(name); }
}

struct CompileTimeClass {
    enum class ModifierType : uint8_t { None = 0, Array = 1, Pointer = 2, Reference = 3 };
    enum class _BaseType : uint8_t { None = 0, Class = 1, Inner = 2, Modifier = 3, Generic = 4, MaxCount = 5 };

    struct _BaseInfo {
        _BaseType _baseType{};
        inline constexpr _BaseInfo(_BaseType type) : _baseType(type) {}
    };

    struct _ClassInfo : _BaseInfo {
        const char *_namespace{};
        const char *_name{};
        const char *_imageName{};
        inline constexpr _ClassInfo() : _BaseInfo(_BaseType::Class) {}
    };

    struct _InnerInfo : _BaseInfo {
        const char *_name{};
        inline constexpr _InnerInfo() : _BaseInfo(_BaseType::Inner) {}
    };

    struct _ModifierInfo : _BaseInfo {
        ModifierType _modifierType{};
        inline constexpr _ModifierInfo() : _BaseInfo(_BaseType::Modifier) {}
    };

    struct _GenericInfo : _BaseInfo {
        std::vector<CompileTimeClass> _types{};
        inline _GenericInfo() : _BaseInfo(_BaseType::Generic) {}
    };

    Class _loadedClass{};
    std::vector<_BaseInfo *> _stack{};
    Class *_reference{};
    uint8_t _autoFree : 1 = false;

    inline CompileTimeClass() = default;
    inline CompileTimeClass(Class *_ref) : _reference(_ref) { _autoFree = true; }
    inline CompileTimeClass(const CompileTimeClass &other) = default;
    inline ~CompileTimeClass() { if (_autoFree) Free(); }

    inline void AddClass(const char *_namespace, const char *name, const char *imageName = nullptr) {
        auto info = new _ClassInfo();
        info->_namespace = _namespace;
        info->_name = name;
        info->_imageName = imageName;
        _stack.push_back(info);
    }
    inline void AddInnerClass(const char *name) {
        auto info = new _InnerInfo();
        info->_name = name;
        _stack.push_back(info);
    }
    inline void AddModifier(ModifierType type) {
        auto info = new _ModifierInfo();
        info->_modifierType = type;
        _stack.push_back(info);
    }
    inline void AddGeneric(const std::vector<CompileTimeClass> &types) {
        auto info = new _GenericInfo();
        info->_types = types;
        _stack.push_back(info);
    }

    Class ToClass();
    inline Class ToClass() const { return ((CompileTimeClass *) this)->ToClass(); }
    inline operator Class() const { return ToClass(); }
    inline IL2CPP::Il2CppType *ToIl2CppType() const { return ToClass().GetIl2CppType(); }
    inline operator IL2CPP::Il2CppType *() const { return ToIl2CppType(); }
    inline IL2CPP::Il2CppClass *ToIl2CppClass() const { return ToClass().GetClass(); }
    inline operator IL2CPP::Il2CppClass *() const { return ToIl2CppClass(); }
    inline BNM::MethodBase operator[](const std::string_view &name) const { return ToClass().GetMethod(name); }
    inline BNM::FieldBase operator[](const char *name) const { return ToClass().GetField(name); }
    inline void Free() {
        for (auto info : _stack) delete info;
        _stack.clear();
        _autoFree = false;
    }
};

struct CompileTimeClassBuilder {
    CompileTimeClass _class{};
    inline CompileTimeClassBuilder(const char *_namespace, const char *name) { _class.AddClass(_namespace, name); }
    inline CompileTimeClassBuilder(const char *_namespace, const char *name, const char *imageName) { _class.AddClass(_namespace, name, imageName); }
    inline CompileTimeClassBuilder &Inner(const char *name) { _class.AddInnerClass(name); return *this; }
    inline CompileTimeClassBuilder &Pointer() { _class.AddModifier(CompileTimeClass::ModifierType::Pointer); return *this; }
    inline CompileTimeClassBuilder &Reference() { _class.AddModifier(CompileTimeClass::ModifierType::Reference); return *this; }
    inline CompileTimeClassBuilder &Array() { _class.AddModifier(CompileTimeClass::ModifierType::Array); return *this; }
    inline CompileTimeClassBuilder &Generic(const std::vector<CompileTimeClass> &types) { _class.AddGeneric(types); return *this; }
    inline CompileTimeClass Build() { return _class; }
};

namespace MANAGEMENT_STRUCTURES {

struct CustomMethod {
    std::string _name{};
    std::string _copyTarget{};
    std::vector<CompileTimeClass> _parameterTypes{};
    CompileTimeClass _returnType{};
    void *_address{};
    void *_invoker{};
    uint8_t _isStatic : 1 = false;
    uint8_t _isInvokeHook : 1 = false;
    uint8_t _isBasicHook : 1 = false;
    uint8_t _skipTypeMatch : 1 = false;
    IL2CPP::MethodInfo *myInfo{};
    IL2CPP::MethodInfo *_origin{};
    void *_originalAddress{};
    uint16_t _slot{};
};

struct CustomField {
    std::string _name{};
    CompileTimeClass _type{};
    int32_t offset{};
    size_t _size{};
    IL2CPP::FieldInfo *myInfo{};
};

struct CustomClass {
    CompileTimeClass _targetType{};
    CompileTimeClass _baseType{};
    IL2CPP::Il2CppClass *_owner{};
    std::vector<CompileTimeClass> _interfaces{};
    std::vector<CustomMethod *> _methods{};
    std::vector<CustomField *> _fields{};
    size_t _size{};
    IL2CPP::Il2CppClass *myClass{};
    Class type{};
    void *userData{};
};

template<bool IsStatic, typename MET>
struct GetMethodInvoker;

template<typename Ret, typename CLS, typename ...Args>
struct GetMethodInvoker<false, Ret (CLS::*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *obj, void **params, void *) {
        Ret (CLS::*fn)(Args...) = nullptr;
        memcpy(&fn, &ptr, sizeof(fn));
        if constexpr (std::is_void_v<Ret>) ((CLS *) obj->*fn)((Args) *params...);
        else ((CLS *) obj->*fn)((Args) *params...);
    }
};

template<typename Ret, typename CLS, typename ...Args>
struct GetMethodInvoker<false, Ret (CLS::*)(Args...) const> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *obj, void **params, void *) {
        Ret (CLS::*fn)(Args...) const = nullptr;
        memcpy(&fn, &ptr, sizeof(fn));
        if constexpr (std::is_void_v<Ret>) ((CLS *) obj->*fn)((Args) *params...);
        else ((CLS *) obj->*fn)((Args) *params...);
    }
};

template<typename Ret, typename CLS, typename ...Args>
struct GetMethodInvoker<true, Ret (CLS::*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<false, Ret (*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<true, Ret (*)(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<false, Ret(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *obj, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

template<typename Ret, typename ...Args>
struct GetMethodInvoker<true, Ret(Args...)> {
    static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *, void **params, void *) {
        if constexpr (std::is_void_v<Ret>) ((Ret (*)(Args...)) ptr)((Args) *params...);
        else ((Ret (*)(Args...)) ptr)((Args) *params...);
    }
};

void AddClass(CustomClass *_class);

}

namespace ClassesManagement {

extern std::vector<MANAGEMENT_STRUCTURES::CustomClass *> *classesManagementVector;

void ProcessCustomClasses();
void ProcessClassRuntime(MANAGEMENT_STRUCTURES::CustomClass *customClass);
void ForEachClassInImage(IL2CPP::Il2CppImage *image, const std::function<bool(IL2CPP::Il2CppClass *)> &func);

IL2CPP::Il2CppClass *Class$$FromIl2CppType(IL2CPP::Il2CppType *type);
IL2CPP::Il2CppClass *Type$$GetClassOrElementClass(IL2CPP::Il2CppType *type);
IL2CPP::Il2CppClass *Class$$FromName(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name);

extern IL2CPP::Il2CppClass *(*old_Class$$FromIl2CppType)(IL2CPP::Il2CppType *type);
extern IL2CPP::Il2CppClass *(*old_Type$$GetClassOrElementClass)(IL2CPP::Il2CppType *type);
extern IL2CPP::Il2CppClass *(*old_Class$$FromName)(IL2CPP::Il2CppImage *image, const char *namespace_, const char *name);

}

namespace UnityEngine {

struct PersistentCall;

struct UnityEventBase {
    IL2CPP::Il2CppObject object;
    static BNM::Class GetArgumentType(PersistentCall *call);
    static BNM::Class GetTargetType(PersistentCall *call);
};

struct PersistentCallArguments {
    IL2CPP::Il2CppObject object;
    Structures::Mono::String *m_ObjectArgumentAssemblyTypeName{};
    void *m_ObjectArgument{};
    int32_t m_Mode{};
};

struct PersistentCall {
    BNM::Class m_Target{};
    Structures::Mono::String *m_TargetAssemblyTypeName{};
    PersistentCallArguments *m_Arguments{};
    IL2CPP::MethodInfo *m_Method{};
};

}

extern bool InvertMatrix4x4_Full(const float *inMatrix, float *dest);

struct CustomWait : BNM::IL2CPP::Il2CppObject {
    std::function<bool()> _func{};
    bool _isUntil = false;
    void Finalize() {
        try {
            _func = {};
            this->~CustomWait();
        } catch (...) {
            BNM_LOG_ERR("CustomWait::Finalize exception");
        }
    }
    bool MoveNext() const {
        try {
            if (!_func) return false;
            return _isUntil == !_func();
        } catch (...) {
            BNM_LOG_ERR("CustomWait::MoveNext exception");
            return false;
        }
    }
    void Reset() {}
    static IL2CPP::Il2CppObject *Current() { return nullptr; }
};

namespace Coroutine {

struct YieldInstruction {
    inline YieldInstruction(BNM::IL2CPP::Il2CppObject *object) : _object(object) {}
    BNM::IL2CPP::Il2CppObject *_object{};
protected:
    inline YieldInstruction() = default;
    friend struct IEnumerator;
};

struct IEnumerator : BNM::IL2CPP::Il2CppObject {
    struct promise_type {
        Coroutine::YieldInstruction _currentValue{};
        inline IEnumerator get_return_object() { return IEnumerator(std::coroutine_handle<promise_type>::from_promise(*this)); }
        inline std::suspend_always initial_suspend() noexcept { return {}; }
        inline std::suspend_always final_suspend() noexcept { return {}; }
        inline void unhandled_exception() {}
        inline std::suspend_always await_transform() = delete;
        [[nodiscard]] inline Coroutine::YieldInstruction value() const noexcept { return _currentValue; }
        inline std::suspend_always yield_value(const Coroutine::YieldInstruction &val) { _currentValue = val; return {}; }
        inline void return_void() {}
    };
    IL2CPP::Il2CppObject *_current{};
    std::coroutine_handle<promise_type> _coroutine{};

    void Finalize();
    bool MoveNext();
    void Reset();
    IL2CPP::Il2CppObject *Current();
    IEnumerator *Get();
    inline IEnumerator *operator()() { return Get(); }
    inline operator IEnumerator *() { return Get(); }
    explicit IEnumerator(std::coroutine_handle<promise_type> handle) : BNM::IL2CPP::Il2CppObject(), _coroutine(handle) {}
    inline constexpr IEnumerator() : BNM::IL2CPP::Il2CppObject() {}
};

struct AsyncOperation : YieldInstruction {
    inline AsyncOperation() = default;
    AsyncOperation(intptr_t operation);
};

struct WaitForEndOfFrame : YieldInstruction {
    WaitForEndOfFrame();
};

struct WaitForFixedUpdate : YieldInstruction {
    WaitForFixedUpdate();
};

struct WaitForSeconds : YieldInstruction {
    inline WaitForSeconds() = default;
    WaitForSeconds(float seconds);
};

struct WaitForSecondsRealtime : YieldInstruction {
    inline WaitForSecondsRealtime() = default;
    WaitForSecondsRealtime(float seconds);
};

struct WaitUntil : YieldInstruction {
    inline WaitUntil() = default;
    WaitUntil(const std::function<bool()> &function);
};

struct WaitWhile : YieldInstruction {
    inline WaitWhile() = default;
    WaitWhile(const std::function<bool()> &function);
};

}

struct BNMClassBuilder {
    static inline CompileTimeClass Class(const char *_namespace, const char *name, const char *imageName = nullptr) {
        return CompileTimeClassBuilder(_namespace, name, imageName).Build();
    }
};

bool InvokeHookImpl(IL2CPP::MethodInfo *info, void *newMet, void **oldMet);
bool VirtualHookImpl(Class targetClass, IL2CPP::MethodInfo *info, void *newMet, void **oldMet);

template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &oldMet) {
    return InvokeHookImpl(targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &&oldMet) {
    return InvokeHookImpl(targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(IL2CPP::MethodInfo *info, T_NEW newMet, T_OLD &oldMet) {
    return InvokeHookImpl(info, (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool InvokeHook(IL2CPP::MethodInfo *info, T_NEW newMet, T_OLD &&oldMet) {
    return InvokeHookImpl(info, (void *) newMet, (void **) &oldMet);
}

template<typename T_NEW, typename T_OLD>
inline bool VirtualHook(BNM::Class targetClass, const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &oldMet) {
    return VirtualHookImpl(targetClass, targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}
template<typename T_NEW, typename T_OLD>
inline bool VirtualHook(BNM::Class targetClass, const BNM::MethodBase &targetMethod, T_NEW newMet, T_OLD &&oldMet) {
    return VirtualHookImpl(targetClass, targetMethod.GetInfo(), (void *) newMet, (void **) &oldMet);
}

namespace Internal {

struct VMData {
    BNM::Class Object{};
    BNM::Class UnityEngine$$Object{};
    BNM::Class System$$List{};
    BNM::MethodBase Type$$GetType{};
    BNM::MethodBase Interlocked$$CompareExchange{};
    BNM::MethodBase RuntimeType$$MakeGenericType{};
    BNM::MethodBase RuntimeType$$MakePointerType{};
    BNM::MethodBase RuntimeType$$make_byref_type{};
    BNM::MethodBase RuntimeMethodInfo$$MakeGenericMethod_impl{};
    IL2CPP::Il2CppString **String$$Empty{};
};

extern VMData vmData;

template<class CompareMethod>
IL2CPP::MethodInfo *IterateMethods(Class target, CompareMethod compare) {
    if (!target) return nullptr;
    void *iter = nullptr;
    while (auto method = api.il2cpp_class_get_methods(target._data, &iter)) {
        if (compare((IL2CPP::MethodInfo *) method)) return (IL2CPP::MethodInfo *) method;
    }
    auto parent = api.il2cpp_class_get_parent(target._data);
    if (parent) return IterateMethods(Class(parent), compare);
    return nullptr;
}

inline bool CompareImageName(IL2CPP::Il2CppImage *image, const std::string_view &name) {
    if (!image) return false;
    if (image->name && name == image->name) return true;
    if (image->nameNoExt && name == image->nameNoExt) return true;
    if (image->name) {
        std::string_view full(image->name);
        if (full.size() > 4 && full.substr(full.size() - 4) == ".dll" && full.substr(0, full.size() - 4) == name) return true;
    }
    return false;
}

inline std::vector<IL2CPP::Il2CppAssembly *> &GetAllAssemblies() {
    if (!assembliesCache.empty()) return assembliesCache;
    if (!api.il2cpp_domain_get) return assembliesCache;
    auto domain = api.il2cpp_domain_get();
    if (!domain) return assembliesCache;
    size_t size = 0;
    auto assemblies = api.il2cpp_domain_get_assemblies(domain, &size);
    for (size_t i = 0; i < size; ++i) if (assemblies[i]) assembliesCache.push_back(assemblies[i]);
    return assembliesCache;
}

}

inline Class::Class(const CompileTimeClass &compileTimeClass) { _data = compileTimeClass.ToClass()._data; }
inline Class::operator CompileTimeClass() const { return GetCompileTimeClass(); }
inline Defaults::DefaultTypeRef::operator BNM::CompileTimeClass() const {
    CompileTimeClass result;
    result._loadedClass = _reference ? *_reference : Class{};
    return result;
}
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-bool-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
inline std::string Structures::Mono::String::str() const {
    if (!this || !length) return {};
    std::string utf8;
    utf8.reserve(length);
    for (int32_t i = 0; i < length; ++i) {
        auto c = chars[i];
        if (c < 0x80) utf8 += (char) c;
        else if (c < 0x800) {
            utf8 += (char) (0xC0 | (c >> 6));
            utf8 += (char) (0x80 | (c & 0x3F));
        } else {
            utf8 += (char) (0xE0 | (c >> 12));
            utf8 += (char) (0x80 | ((c >> 6) & 0x3F));
            utf8 += (char) (0x80 | (c & 0x3F));
        }
    }
    return utf8;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
inline unsigned int Structures::Mono::String::GetHash() const {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-bool-conversion"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
    if (!this) return 0;
#if defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    const IL2CPP::Il2CppChar *p = chars;
    unsigned int h = 0;
    for (int32_t i = 0; i < length; ++i) {
        h = (h << 5) - h + *p;
        ++p;
    }
    return h;
}
inline Structures::Unity::Vector2::operator Vector3() const { return {x, y, 0}; }
inline Structures::Unity::Vector3::operator Vector4() const { return {x, y, z, 0}; }
inline Structures::Unity::Vector4::Vector4(Color c) : x(c.r), y(c.g), z(c.b), w(c.a) {}

template<typename ...Args>
inline IL2CPP::Il2CppObject *Class::CreateNewObjectParameters(Args &&...args) const {
    auto obj = CreateNewInstance();
    if (!obj) return nullptr;
    auto ctor = GetMethod(Internal::constructorName, (int) sizeof...(Args));
    if (!ctor) return obj;
    auto method = ctor.GetInfo();
    ((void (*)(IL2CPP::Il2CppObject *, Args..., IL2CPP::MethodInfo *)) method->methodPointer)(obj, std::forward<Args>(args)..., method);
    return obj;
}

}
