#pragma once

#include <map>
#include <vector>

#include "Debug.hpp"
#include "Il2CppStructures.hpp"
#include "MonoStructures.hpp"
#include "Class.hpp"
#include "MethodBase.hpp"

namespace BNM {
    namespace Structures::Mono {
        template <typename TKey, typename TValue>
        struct Dictionary : IL2CPP::Il2CppObject {
#ifdef BNM_DOTNET35
            struct Link {
                int HashCode{};
                int Next{};
            };
            Array<int> *table{};
            Array<Link> *linkSlots{};
            Array<TKey> *keys{};
            Array<TValue> *values{};
            int touchedSlots{};
            int emptySlot{};
            int count{};
            int threshold{};
            void *hcp{};
            void *serialization_info{};
            int version{};

            std::map<TKey, TValue> ToMap() const {
                std::map<TKey, TValue> ret{};
                for (int i = 0; i < touchedSlots; i++)
                    if ((linkSlots->GetItems()[i].HashCode & -2147483648) != 0)
                        ret.insert(std::make_pair(keys->GetItems()[i], values->GetItems()[i]));
                return ret;
            }
            std::vector<TKey> GetKeys() const {
                std::vector<TKey> ret{};
                for (int i = 0; i < touchedSlots; i++)
                    if ((linkSlots->GetItems()[i].HashCode & -2147483648) != 0)
                        ret.push_back(keys->GetItems()[i]);
                return ret;
            }
            std::vector<TValue> GetValues() const {
                std::vector<TValue> ret{};
                for (int i = 0; i < touchedSlots; i++)
                    if ((linkSlots->GetItems()[i].HashCode & -2147483648) != 0)
                        ret.push_back(values->GetItems()[i]);
                return ret;
            }
#else
            struct Entry {
                int hashCode{};
                int next{};
                TKey key{};
                TValue value{};
            };
            Array<int> *buckets{};
            Array<Entry> *entries{};
            int count{};
            int version{};
            int freeList{};
            int freeCount{};
            void *comparer{};
            Array<TKey> *keys{};
            Array<TValue> *values{};
            void *syncRoot{};

            std::map<TKey, TValue> ToMap() const {
                std::map<TKey, TValue> ret{};
                for (int i = 0; i < count; ++i)
                    ret.emplace(entries->GetItems()[i].key, entries->GetItems()[i].value);
                return ret;
            }
            std::vector<TKey> GetKeys() const {
                std::vector<TKey> ret{};
                for (int i = 0; i < count; ++i) ret.emplace_back(entries->GetItems()[i].key);
                return ret;
            }
            std::vector<TValue> GetValues() const {
                std::vector<TValue> ret{};
                for (int i = 0; i < count; ++i) ret.emplace_back(entries->GetItems()[i].value);
                return ret;
            }
#endif
            [[nodiscard]] int GetSize() const { return count; }
            [[nodiscard]] int GetVersion() const { return version; }

            bool TryGet(TKey key, TValue *value) const {
                return Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("TryGetValue"), 2)
                    .template cast<Method<bool>>()[(void *) this](key, value);
            }
            void Add(TKey key, TValue value) {
                Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("Add"), 2)
                    .template cast<Method<void>>()[(void *) this](key, value);
            }
            void Insert(TKey key, TValue value) {
                Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("set_Item"), 2)
                    .template cast<Method<void>>()[(void *) this](key, value);
            }
            bool Remove(TKey key) {
                return Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("Remove"), 1)
                    .template cast<Method<bool>>()[(void *) this](key);
            }
            bool ContainsKey(TKey key) const {
                return Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("ContainsKey"), 1)
                    .template cast<Method<bool>>()[(void *) this](key);
            }
            bool ContainsValue(TValue value) const {
                return Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("ContainsValue"), 1)
                    .template cast<Method<bool>>()[(void *) this](value);
            }
            void Clear() {
                Class((IL2CPP::Il2CppObject *) this)
                    .GetMethod(BNM_OBFUSCATE("Clear"), 0)
                    .template cast<Method<void>>()[(void *) this]();
            }
            TValue Get(TKey key) const {
                TValue ret{};
                if (TryGet(key, &ret))
                    return ret;
                return {};
            }
            TValue operator[](TKey key) const { return Get(key); }
        };
    }  // namespace Structures::Mono
}  // namespace BNM
