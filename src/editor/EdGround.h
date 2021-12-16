#pragma once

#include "../renderer/GroundData.h"

namespace Editor
{

class EdMaterial;

class EdGround : public GroundData
{
protected:
    int _matCount;
    EdMaterial** _materials;
    unsigned int* _flags;

public:
    EdGround(
        int width,
        int height,
        int matCount,
        EdMaterial** materials,
        Material** rMaterials,
        unsigned int *flags,
        int *materialIds
    );
    ~EdGround();

};

}