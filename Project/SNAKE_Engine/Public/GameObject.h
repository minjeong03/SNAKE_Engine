#pragma once
#include "Object.h"


class GameObject : public Object
{
public:
    GameObject() : Object(ObjectType::GAME) {}
    ~GameObject() override = default;
};
