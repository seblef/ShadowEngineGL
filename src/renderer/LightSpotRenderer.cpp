
#include "LightSpotRenderer.h"

LightSpotRenderer::LightSpotRenderer(const LightRendererCreate_t& c) :
LightRenderer(c, "Lights/Spot", "Lights/SpotShadows")
{
	VxBase sSpotVertices[5]={
		VxBase(Vector3::NullVector,Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f,-1.0f,1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f,-1.0f,1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f, 1.0f,1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f, 1.0f,1.0f),Color::White,Vector2::NullVector) };

	unsigned short swSpotIndices[18]={
		0,2,1,
		0,3,2,
		0,4,3,
		0,1,4,
		1,2,4,
		2,3,4 };

    _geo=_device->createGeometryBuffer(5,VX_BASE,sSpotVertices,18,IDX_16BITS,swSpotIndices,BU_IMMUTABLE);
    _indicesCount=18;
}
