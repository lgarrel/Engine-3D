#pragma once
#include "pch.h"

class Device;

class ShapeFactory
{
public:
    static std::unique_ptr<Mesh> CreateBox(Device* device, float width, float height, float depth);
    static std::unique_ptr<Mesh> CreateSphere(Device* device, float radius, int sliceCount, int stackCount);
    static std::unique_ptr<Mesh> CreateCylinder(Device* device, float bottomRadius, float topRadius, float height, int sliceCount, int stackCount);
};