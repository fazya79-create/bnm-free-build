#include "BNM.hpp"

using namespace BNM;

namespace BNM::UnityEngine {
    BNM::Class UnityEventBase::GetArgumentType(PersistentCall *call) {
        auto type = Internal::vmData.UnityEngine$$Object;
        if (!call || !call->m_Arguments)
            return type;
        auto typeName = call->m_Arguments->m_ObjectArgumentAssemblyTypeName;
        if (!typeName || typeName->IsNullOrEmpty())
            return type;
        if (!Internal::vmData.Type$$GetType.IsValid())
            return type;
        if (auto t = Internal::vmData.Type$$GetType.cast<Method<MonoType *>>()(typeName);
            t != nullptr)
            type = t;
        return type;
    }
    BNM::Class UnityEventBase::GetTargetType(PersistentCall *call) {
        BNM::Class targetType;
        if (!call)
            return targetType;
        if (call->m_Target)
            return BNM::Class(call->m_Target);
        if (!call->m_TargetAssemblyTypeName || !Internal::vmData.Type$$GetType.IsValid())
            return targetType;
        targetType = Internal::vmData.Type$$GetType.cast<Method<MonoType *>>()(
            call->m_TargetAssemblyTypeName);
        return targetType;
    }
}  // namespace BNM::UnityEngine

MethodBase DelegateBase::GetMethod() const {
    auto method = MethodBase(this->method);
    auto instance = GetInstance();
    if (instance)
        method.SetInstance(instance);
    return method;
}

DelegateBase *DelegateBase::Create(BNM::MethodBase method) {
    if (!object.klass)
        return nullptr;
    return (DelegateBase *) BNM::Class(object.klass)
        .CreateNewObjectParameters(method._instance, method._data);
}

std::vector<BNM::MethodBase> MulticastDelegateBase::GetMethods() const {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    if (!delegates)
        return {((DelegateBase *) this)->GetMethod()};

    std::vector<MethodBase> ret{};
    ret.reserve(delegates->max_length);
    for (IL2CPP::il2cpp_array_size_t i = 0; i < delegates->max_length; ++i) {
        auto current = (*delegates)[i];
        if (current)
            ret.push_back(current->GetMethod());
    }
    return ret;
}

void MulticastDelegateBase::Add(DelegateBase *delegate) {
    if (!delegate)
        return;

    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    if (!delegates) {
        auto elementClass = object.klass ? BNM::Class(object.klass) : BNM::Class{};
        if (!elementClass) {
            BNM_LOG_ERR("MulticastDelegate::Add failed: unknown delegate class");
            return;
        }
        auto newArr = elementClass.NewArray<DelegateBase *>(1);
        if (!newArr)
            return;
        newArr->GetItems()[0] = delegate;
        this->delegates = (decltype(this->delegates)) newArr;
        return;
    }

    if (!delegates->obj.klass || !delegates->obj.klass->element_class) {
        BNM_LOG_ERR("MulticastDelegate::Add failed: unknown element class");
        return;
    }

    auto arr = BNM::Class(delegates->obj.klass->element_class)
                   .NewArray<DelegateBase *>(delegates->max_length + 1);
    if (!arr)
        return;
    arr->CopyFrom(delegates->GetItems(), delegates->max_length);
    arr->GetItems()[delegates->max_length] = delegate;
    this->delegates = (decltype(this->delegates)) arr;
}

void MulticastDelegateBase::Remove(DelegateBase *delegate) {
    auto delegates = (Structures::Mono::Array<DelegateBase *> *) this->delegates;
    if (!delegates || !delegate)
        return;
    if (!delegates->obj.klass || !delegates->obj.klass->element_class)
        return;

    IL2CPP::il2cpp_array_size_t index = 0;
    bool found = false;
    for (IL2CPP::il2cpp_array_size_t i = 0; i < delegates->max_length; ++i) {
        if ((*delegates)[i] != delegate)
            continue;
        found = true;
        index = i;
        break;
    }
    if (!found)
        return;

    if (delegates->max_length == 1) {
        this->delegates = nullptr;
        return;
    }

    auto arr = BNM::Class(delegates->obj.klass->element_class)
                   .NewArray<DelegateBase *>(delegates->max_length - 1);
    if (!arr)
        return;
    auto src = delegates->GetItems();
    auto dst = arr->GetItems();
    if (index > 0)
        memcpy(dst, src, index * sizeof(void *));
    memmove(dst + index, src + index + 1, (delegates->max_length - index - 1) * sizeof(void *));
    this->delegates = (decltype(this->delegates)) arr;
}

DelegateBase *MulticastDelegateBase::Add(BNM::MethodBase method) {
    auto delegate = ((DelegateBase *) this)->Create(method);
    if (!delegate)
        return nullptr;
    Add(delegate);
    return delegate;
}
