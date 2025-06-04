#pragma once
#include <cstdint>

struct RenderBuffer {
    std::tuple<uint32_t, uint32_t, uint32_t>* arr; // 0x0
    uint32_t arrSize; // 0x8
    uint32_t padding; // 0xc
    uint64_t* ptr; // 0x10
    uint32_t counter; // 0x18
    uint32_t unk1; // 0x1c
    uint32_t unk2; // 0x20
    uint32_t unk3; // 0x24
}; // Size: 0x28

class WorldContainer {
private: char pad_0[0x28]; public:
    RenderBuffer buffers[0x3]; // 0x28
    uint32_t index; // 0xa0

    RenderBuffer* NextRenderBuffer() {
        return &buffers[(index + 1) % 3];
    }

    RenderBuffer* LastRenderBuffer() {
        return &buffers[(index + 2) % 3];
    }

    RenderBuffer* GetRenderBuffer() {
        return &buffers[index];
    }
}; // Size: 0xa4

class WorldRenderComponent {
private: char pad_0[0x7e8]; public:
	WorldContainer* container; // 0x7e8
}; // Size: 0x7e8

class WorldComponent : public Component<TYPE_WORLD>
{
public:
	// Constructor
	WorldComponent(uint64_t ComponentParent = 0) : Component<TYPE_WORLD>(ComponentParent) {}

};