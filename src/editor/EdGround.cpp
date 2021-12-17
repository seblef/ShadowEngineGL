#include "EdGround.h"
#include "../renderer/Renderer.h"

namespace Editor
{

EdGround::EdGround(
    int width,
    int height,
    int matCount,
    EdMaterial** materials,
    Material** rMaterials,
    unsigned int *flags,
    int *materialIds
) :
    GroundData(width, height, matCount, rMaterials, materialIds),
    _matCount(matCount),
    _materials(materials),
    _flags(flags)
{
    Renderer::getSingletonRef().setGroundData(this);
}

EdGround::~EdGround()
{
    Renderer::getSingletonRef().setGroundData(0);
    delete[] _materials;
    delete[] _flags;
}

}