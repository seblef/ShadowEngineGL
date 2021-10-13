
#include "LightAreaRenderer.h"

LightAreaRenderer::LightAreaRenderer(const LightRendererCreate_t& c) :
    LightRenderer(c, "Lights/Area", "Lights/AreaShadows")
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

void LightAreaRenderer::fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const
{
	const LightArea *s=(const LightArea*)l;
	float sfar=s->getFarAngle() * M_PI / 360.0f;
	float snear=s->getNearAngle() * M_PI / 360.0f;

	float cf=cosf(sfar);
	float sf=sinf(sfar);

	b._nearAngle=cosf(snear);
	b._farAngle=cf;
	b._dir=s->getDirection();

	float aspect=s->getAreaWidth() / s->getAreaHeight();
	float xscale,yscale;
	float div=cf / sf;

	float dist=s->getAreaHeight() * div * 0.5f;
	float range=s->getRange() + dist;
	yscale=range / div;
	xscale=yscale * aspect;

	const Matrix4& mw(s->getWorldMatrix());
	Vector3 pos(mw);
	pos-=s->getDirection() * dist;

	Vector3 up(Vector3::YAxisVector);
	up.transformNoTranslation(mw);
	up.normalize();

	world.createScale(xscale,yscale,range);
	world*=mw;
	world=pos;

	Camera cam(
		pos,
		pos + l->getDirection(),
		dist, range,
		Vector2(s->getAreaWidth()*0.5f,s->getAreaHeight()*0.5f),
		s->getFarAngle(),
		false);
	cam.setUpVector(up);
	cam.buildMatrices();
	b._areaViewProj=cam.getViewProjMatrix();
}

bool LightAreaRenderer::isCameraInVolume(
	const Vector3& eyePos,
	float minZ,
	const Vector3& pos,
	const Light *l
) const
{
	float d = (pos - eyePos).getSquareLength();
	float r = l->getRange() + minZ;
	return d < r * r * 5.0f;
}
