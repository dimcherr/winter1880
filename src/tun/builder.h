#pragma once
#include "tun/entity.h"
#include "Hub.h"

class EntityBuilder;

template <typename T>
struct BaseCompBuilder {
    EntityBuilder* entityBuilder {};
    Entity entity {};
    T* comp {};

    EntityBuilder& Next() {
        return *entityBuilder;
    }
};

template <typename T>
struct CompBuilder : public BaseCompBuilder<T> {};

class EntityBuilder {
public:
    EntityBuilder();
    EntityBuilder(Entity entity);
    Entity GetEntity();

    template<typename... Type>
    [[nodiscard]] decltype(auto) Get() const {
        auto& reg = hub::Reg();
        if constexpr(sizeof...(Type) == 1u) {
            return (reg.assure<std::remove_const_t<Type>>()->get(entity), ...);
        } else {
            return std::forward_as_tuple(reg.get<Type>(entity)...);
        }
    }

    template <typename T>
    EntityBuilder& Tag() {
        hub::AddTag<T>(this->entity);
        return *this;
    }

    template <typename T>
    EntityBuilder& AddComp(const T& comp) {
        hub::AddComp<T>(this->entity, comp);
        return *this;
    }

    template <typename T>
    CompBuilder<T> Add() {
        CompBuilder<T> compBuilder;
        T& comp = hub::AddComp<T>(this->entity);
        compBuilder.comp = &comp;
        compBuilder.entityBuilder = this;
        compBuilder.entity = entity;
        return compBuilder;
    }

    template <typename T>
    CompBuilder<T> Modify() {
        T& comp = hub::Reg().get<T>(this->entity);
        CompBuilder<T> compBuilder;
        compBuilder.comp = &comp;
        compBuilder.entityBuilder = this;
        compBuilder.entity = entity;
        return compBuilder;
    }

private:
    Entity entity {};
};
