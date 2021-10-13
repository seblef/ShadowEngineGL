
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

bool LightSpotRenderer::isCameraInVolume(
	const Vector3& eyePos,
	float minZ,
	const Vector3& pos,
	const Light* l
) const
{
	float d = (pos - eyePos).getSquareLength();
	float r = l->getRange() + minZ;
	return d < r * r * 2.0f;
}

void LightSpotRenderer::fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const
{
	const LightSpot *s=(const LightSpot*)l;
	float sfar=s->getFarAngle() * M_PI / 360.0f;
	float snear=s->getNearAngle() * M_PI / 360.0f;

	float cf=cosf(sfar);
	float sf=sinf(sfar);

	b._nearAngle=cosf(snear);
	b._farAngle=cf;
	b._dir=s->getDirection();
	b._range=l->getRange();
	b._invRange=1.0f / l->getRange();

	float xyScale=l->getRange() * sf / cf;
	world.createScale(xyScale,xyScale,l->getRange());
	world*=l->getWorldMatrix();
}
