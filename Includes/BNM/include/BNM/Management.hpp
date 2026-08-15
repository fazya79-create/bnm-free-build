#pragma once

#include "Class.hpp"
#include "Internals.hpp"
namespace BNM {
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

        template <bool IsStatic, typename MET>
        struct GetMethodInvoker;

        namespace PRIVATE_INVOKER {
            template <typename PMF>
            inline PMF RestorePointerToMember(IL2CPP::Il2CppMethodPointer ptr) {
                PMF fn{};
                static_assert(sizeof(PMF) >= sizeof(ptr), "unexpected pointer-to-member layout");
                memcpy((void *) &fn, (const void *) &ptr, sizeof(ptr));
                return fn;
            }

            // il2cpp passes arguments as an array of pointers to the values.
            template <typename T>
            inline T ReadArg(void **params, size_t index) {
                if constexpr (std::is_pointer_v<T>)
                    return (T) params[index];
                else
                    return *(std::remove_cv_t<std::remove_reference_t<T>> *) params[index];
            }
        }  // namespace PRIVATE_INVOKER

        template <typename Ret, typename CLS, typename... Args>
        struct GetMethodInvoker<false, Ret (CLS::*)(Args...)> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, void *obj,
                                   [[maybe_unused]] void **params, std::index_sequence<I...>) {
                auto fn = PRIVATE_INVOKER::RestorePointerToMember<Ret (CLS::*)(Args...)>(ptr);
                ((CLS *) obj->*fn)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *,
                               void *obj, void **params, void *) {
                InvokeImpl(ptr, obj, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename CLS, typename... Args>
        struct GetMethodInvoker<false, Ret (CLS::*)(Args...) const> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, void *obj,
                                   [[maybe_unused]] void **params, std::index_sequence<I...>) {
                auto fn = PRIVATE_INVOKER::RestorePointerToMember<Ret (CLS::*)(Args...) const>(ptr);
                ((const CLS *) obj->*fn)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *,
                               void *obj, void **params, void *) {
                InvokeImpl(ptr, obj, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename CLS, typename... Args>
        struct GetMethodInvoker<true, Ret (CLS::*)(Args...)> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, [[maybe_unused]] void **params,
                                   std::index_sequence<I...>) {
                ((Ret (*)(Args...)) ptr)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *,
                               void **params, void *) {
                InvokeImpl(ptr, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename CLS, typename... Args>
        struct GetMethodInvoker<true, Ret (CLS::*)(Args...) const> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, [[maybe_unused]] void **params,
                                   std::index_sequence<I...>) {
                ((Ret (*)(Args...)) ptr)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *,
                               void **params, void *) {
                InvokeImpl(ptr, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename... Args>
        struct GetMethodInvoker<false, Ret (*)(Args...)> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, [[maybe_unused]] void **params,
                                   std::index_sequence<I...>) {
                ((Ret (*)(Args...)) ptr)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *,
                               void **params, void *) {
                InvokeImpl(ptr, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename... Args>
        struct GetMethodInvoker<true, Ret (*)(Args...)> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, [[maybe_unused]] void **params,
                                   std::index_sequence<I...>) {
                ((Ret (*)(Args...)) ptr)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *,
                               void **params, void *) {
                InvokeImpl(ptr, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename... Args>
        struct GetMethodInvoker<false, Ret(Args...)> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, void *obj,
                                   [[maybe_unused]] void **params, std::index_sequence<I...>) {
                ((Ret (*)(void *, Args...)) ptr)(obj, PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *,
                               void *obj, void **params, void *) {
                InvokeImpl(ptr, obj, params, std::index_sequence_for<Args...>{});
            }
        };

        template <typename Ret, typename... Args>
        struct GetMethodInvoker<true, Ret(Args...)> {
            template <size_t... I>
            static void InvokeImpl(IL2CPP::Il2CppMethodPointer ptr, [[maybe_unused]] void **params,
                                   std::index_sequence<I...>) {
                ((Ret (*)(Args...)) ptr)(PRIVATE_INVOKER::ReadArg<Args>(params, I)...);
            }
            static void Invoke(IL2CPP::Il2CppMethodPointer ptr, const IL2CPP::MethodInfo *, void *,
                               void **params, void *) {
                InvokeImpl(ptr, params, std::index_sequence_for<Args...>{});
            }
        };

        void AddClass(CustomClass *_class);

    }  // namespace MANAGEMENT_STRUCTURES

    namespace ClassesManagement {

        extern std::vector<MANAGEMENT_STRUCTURES::CustomClass *> *classesManagementVector;

        void ProcessCustomClasses();
        void ProcessClassRuntime(MANAGEMENT_STRUCTURES::CustomClass *customClass);
        void ForEachClassInImage(IL2CPP::Il2CppImage *image,
                                 const std::function<bool(IL2CPP::Il2CppClass *)> &func);

        IL2CPP::Il2CppClass *Class$$FromIl2CppType(IL2CPP::Il2CppType *type);
        IL2CPP::Il2CppClass *Type$$GetClassOrElementClass(IL2CPP::Il2CppType *type);
        IL2CPP::Il2CppClass *Class$$FromName(IL2CPP::Il2CppImage *image, const char *namespace_,
                                             const char *name);

        extern IL2CPP::Il2CppClass *(*old_Class$$FromIl2CppType)(IL2CPP::Il2CppType *type);
        extern IL2CPP::Il2CppClass *(*old_Type$$GetClassOrElementClass)(IL2CPP::Il2CppType *type);
        extern IL2CPP::Il2CppClass *(*old_Class$$FromName)(IL2CPP::Il2CppImage *image,
                                                           const char *namespace_,
                                                           const char *name);

    }  // namespace ClassesManagement
}  // namespace BNM
