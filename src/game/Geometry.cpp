
#include "Geometry.h"
#include "../renderer/Renderer.h"
#include "../loguru.hpp"


Geometry::Geometry(int vxCount, int triCount) :
    _vxCount(vxCount),
    _triCount(triCount),
    _vertices(new Core::Vx3D[vxCount]),
    _indices(new unsigned short[triCount*3]),
    _bBox(Core::BBox3::BoundBox)
{
}

void Geometry::computeBBox()
{
	_bBox = Core::BBox3::InitBox;
	for(int v=0;v<_vxCount;++v)
		_bBox << _vertices[v].pos;
}

void Geometry::computeNormals()
{
	Core::Vector3 n;
	unsigned short *t = _indices.get();
    Core::Vx3D *verts = _vertices.get();
	for(int i=0;i<_triCount;++i)
	{
		n=(verts[t[1]].pos-verts[t[0]].pos) ^ (verts[t[2]].pos - verts[t[0]].pos);
		n.normalize();

		for(int v=0;v<3;++v)
			verts[t[v]].norm=n;

		t+=3;
	}
}

void Geometry::computeBinormalAndTangent()
{
	Core::Vector3 sdir,tdir;
	Core::Vector3 vDeltaPos1,vDeltaPos2;
	Core::Vector2 vDeltaUV1,vDeltaUV2;

	const unsigned short *tri = _indices.get();
	for(int i=0;i<_triCount;++i,tri+=3)
	{
		Core::Vx3D& v0=_vertices[tri[0]];
		Core::Vx3D& v1=_vertices[tri[1]];
		Core::Vx3D& v2=_vertices[tri[2]];

		vDeltaPos1=v1.pos - v0.pos;
		vDeltaPos2=v2.pos - v0.pos;

		vDeltaUV1=v1.uv - v0.uv;
		vDeltaUV2=v2.uv - v0.uv;

		float ir=(vDeltaUV1.x * vDeltaUV2.y - vDeltaUV1.y * vDeltaUV2.x);
		float r=ir==0.0f ? 0.0f : 1.0f / ir;

		sdir.x=(vDeltaUV2.y * vDeltaPos1.x) - (vDeltaUV1.y * vDeltaPos2.x);
		sdir.y=(vDeltaUV2.y * vDeltaPos1.y) - (vDeltaUV1.y * vDeltaPos2.y);
		sdir.z=(vDeltaUV2.y * vDeltaPos1.z) - (vDeltaUV1.y * vDeltaPos2.z);

		tdir.x=(vDeltaUV2.x * vDeltaPos1.x) - (vDeltaUV1.x * vDeltaPos2.x);
		tdir.y=(vDeltaUV2.x * vDeltaPos1.y) - (vDeltaUV1.x * vDeltaPos2.y);
		tdir.z=(vDeltaUV2.x * vDeltaPos1.z) - (vDeltaUV1.x * vDeltaPos2.z);

		sdir*=r;
		tdir*=r;
				
		v0.tangent=(sdir - v0.norm*(v0.norm*sdir));
		v1.tangent=(sdir - v1.norm*(v1.norm*sdir));
		v2.tangent=(sdir - v2.norm*(v2.norm*sdir));

		v0.tangent.normalize();
		v1.tangent.normalize();
		v2.tangent.normalize();

		v0.binorm=v0.tangent ^ v0.norm;
		if((v0.binorm * tdir) > 0.0f)
			v0.binorm*=-1.0f;
		v0.binorm.normalize();

		v1.binorm=v1.tangent ^ v1.norm;
		if((v1.binorm * tdir) > 0.0f)
			v1.binorm*=-1.0f;
		v1.binorm.normalize();
		
		v2.binorm=v2.tangent ^ v2.norm;
		if((v2.binorm * tdir) > 0.0f)
			v2.binorm*=-1.0f;
		v2.binorm.normalize();
	}
}

void Geometry::buildRGeometry()
{
	_rGeo = std::unique_ptr<GeometryData>(
        new GeometryData(
            _vxCount, _vertices.get(),
            _triCount, _indices.get(),
            Renderer::getSingletonRef().getVideoDevice()
        )
    );
}

void Geometry::outputVertices() const
{
	LOG_SCOPE_F(1, "Vertices:");
	for(int i=0;i<_vxCount;++i)
		LOG_S(1) << _vertices[i].pos << "; " << _vertices[i].norm << "; " << _vertices[i].uv;
}

void Geometry::outputTriangles() const
{
	LOG_SCOPE_F(1, "Triangles:");
	for(int i=0;i<_triCount;++i)
		LOG_S(1) << _indices[i*3] << "; " << _indices[i*3+1] << "; " << _indices[i*3+2];
}
