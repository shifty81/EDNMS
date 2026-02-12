#pragma once
#include <unordered_map>
#include <any>
#include <cassert>
#include <vector>
#include "ecs_types.h"
#include "ecs_component_mask.h"

namespace ednms {

class ECSRegistry {
public:
    EntityID CreateEntity() {
        EntityID id = ++m_nextEntity;
        m_entityMasks[id] = {};
        return id;
    }

    void DestroyEntity(EntityID id) {
        m_entityMasks.erase(id);
        for (auto& pool : m_componentPools) {
            pool.second.erase(id);
        }
    }

    bool HasEntity(EntityID id) const {
        return m_entityMasks.find(id) != m_entityMasks.end();
    }

    template<typename T>
    void AddComponent(EntityID id, const T& component) {
        const size_t typeId = GetComponentTypeID<T>();
        m_componentPools[typeId][id] = component;
        m_entityMasks[id].set(typeId);
    }

    template<typename T>
    void RemoveComponent(EntityID id) {
        const size_t typeId = GetComponentTypeID<T>();
        m_componentPools[typeId].erase(id);
        m_entityMasks[id].reset(typeId);
    }

    template<typename T>
    T* GetComponent(EntityID id) {
        const size_t typeId = GetComponentTypeID<T>();
        auto poolIt = m_componentPools.find(typeId);
        if (poolIt == m_componentPools.end()) return nullptr;
        auto it = poolIt->second.find(id);
        return it != poolIt->second.end() ? std::any_cast<T>(&it->second) : nullptr;
    }

    template<typename T>
    const T* GetComponent(EntityID id) const {
        const size_t typeId = GetComponentTypeID<T>();
        auto poolIt = m_componentPools.find(typeId);
        if (poolIt == m_componentPools.end()) return nullptr;
        auto it = poolIt->second.find(id);
        return it != poolIt->second.end() ? std::any_cast<T>(&it->second) : nullptr;
    }

    template<typename T>
    bool HasComponent(EntityID id) const {
        const size_t typeId = GetComponentTypeID<T>();
        const auto& mask = m_entityMasks.at(id);
        return mask.test(typeId);
    }

    const ComponentMask& GetMask(EntityID id) const {
        return m_entityMasks.at(id);
    }

    size_t EntityCount() const {
        return m_entityMasks.size();
    }

    // Collect all entities matching a given component mask
    std::vector<EntityID> GetEntitiesWithMask(const ComponentMask& required) const {
        std::vector<EntityID> result;
        for (const auto& [id, mask] : m_entityMasks) {
            if ((mask & required) == required) {
                result.push_back(id);
            }
        }
        return result;
    }

private:
    EntityID m_nextEntity = 0;
    std::unordered_map<EntityID, ComponentMask> m_entityMasks;
    std::unordered_map<size_t, std::unordered_map<EntityID, std::any>> m_componentPools;

    template<typename T>
    static size_t GetComponentTypeID() {
        static size_t id = s_nextTypeID++;
        return id;
    }

    static inline size_t s_nextTypeID = 0;
};

} // namespace ednms
