
#include "LightOmniRenderer.h"

LightOmniRenderer::LightOmniRenderer(const LightRendererCreate_t& c) :
    LightRenderer(c, "Lights/Omni", "Lights/Omni")
{
	VxBase sOmniVertices[8]={
		VxBase(Vector3(-1.0f, 1.0f,-1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f, 1.0f,-1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f, 1.0f, 1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f, 1.0f, 1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f,-1.0f,-1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f,-1.0f,-1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f,-1.0f, 1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f,-1.0f, 1.0f),Color::White,Vector2::NullVector) };

	unsigned short swOmniIndices[36]={
		4,1,5,
		4,0,1,
		5,2,6,
		5,1,2,
		6,3,7,
		6,2,3,
		7,0,4,
		7,3,0,
		0,2,1,
		0,3,2,
		4,5,6,
		4,6,7 };

    _geo=_device->createGeometryBuffer(8,VX_BASE,sOmniVertices,36,IDX_16BITS,swOmniIndices,BU_IMMUTABLE);
	_indicesCount=36;
}
