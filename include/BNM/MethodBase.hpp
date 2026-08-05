#pragma once

#include "Class.hpp"
#include "Internals.hpp"
namespace BNM {
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
        if constexpr (!std::is_void_v<Ret>) {
            if constexpr (std::is_pointer_v<Ret>) return (Ret) ret;
            Ret val{};
            memcpy(&val, &ret, sizeof(Ret));
            return val;
        }
    }

    template<typename ...Parameters>
    inline Ret Invoke(Parameters ...parameters) const {
        if (!_data) return PRIVATE_INTERNAL::ReturnEmpty<Ret>();
        void *args[] = {(void *) parameters...};
        IL2CPP::Il2CppException *exc = nullptr;
        auto ret = Internal::api.il2cpp_runtime_invoke(_data, _instance, args, &exc);
        if (exc) BNM_LOG_ERR("Method::Invoke exception: %s", exc->message ? ((Structures::Mono::String *) exc->message)->str().c_str() : "unknown");
        if constexpr (!std::is_void_v<Ret>) {
            if constexpr (std::is_pointer_v<Ret>) return (Ret) ret;
            Ret val{};
            memcpy(&val, &ret, sizeof(Ret));
            return val;
        }
    }
};
}
