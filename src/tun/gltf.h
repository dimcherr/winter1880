#pragma once
#include "tun/file.h"
#include "tun/list.h"
#include "cgltf.h"

namespace gltf {

struct Data {
    Bytes fileData {};
    cgltf_data* data {};
    List<float> vertexBuffer {};
    List<uint16_t> indexBuffer {};
    int elementCount {};
};

void Load(StringView path);
void LoadPrims(StringView path);
void Close(cgltf_data* gltf);

}