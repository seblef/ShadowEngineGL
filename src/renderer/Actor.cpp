
#include "Actor.h"
#include "ActorSub.h"
#include "ActorNullNode.h"
#include "Material.h"
#include "../mediacommon/IVertexBuffer.h"
#include "../mediacommon/IIndexBuffer.h"
#include "../mediacommon/IVideoDevice.h"


Actor::Actor(const ActorCreate_t& ac, IVideoDevice* device) : _skinned(ac.skinned), _material(ac.material),
    _bBox(ac.box), _subCount(ac.subCount), _subs(ac.subs), _boneMatrices(ac.boneMatrices), _invSkinMatrix(ac.invSkinMatrix), _initialPose(*ac.initialPose),
    _nullNodeCount(ac.nullNodeCount), _nullNodes(ac.nullNodes)
{
	memcpy(&_bonesNames, &ac.bonesNames, sizeof(ActorBonesNames_t));

	_vBuffer=device->createVertexBuffer(ac.vxCount,_skinned ? VX_3DSKIN : VX_3D,BU_IMMUTABLE,ac.vertices);
	_iBuffer=device->createIndexBuffer(ac.triCount*3,IDX_16BITS,BU_IMMUTABLE,ac.triangles);
}

Actor::~Actor()
{
	delete _vBuffer;
	delete _iBuffer;

	if(_subs)			delete[] _subs;
	if(_nullNodes)		delete[] _nullNodes;
	if(_boneMatrices)	delete	_boneMatrices;

    for(auto const& a : _animations)
		delete a.second;
}

int Actor::getBoneIndex(const string& boneName) const
{
	if (_boneMatrices)
	{
		for (int i = 0; i < _boneMatrices->getCount(); ++i)
		{
			if (boneName == _bonesNames.bonesNames[i])
				return i;
		}
	}

	return -1;
}

const ActorSub* Actor::getSubs() const
{
    return _subs;
}

const ActorSub& Actor::getSub(int n) const
{
    return _subs[n];
}

const ActorNullNode* Actor::getNullNodes() const
{
    return _nullNodes;
}

const ActorNullNode& Actor::getNullNode(int n) const
{
    return _nullNodes[n];
}

int Actor::getNullNodeIndex(const string& name) const
{
    for(int i=0;i<_nullNodeCount;++i)
        if(_nullNodes[i].getName()==name)
            return i;
    return -1;
}

int Actor::getSubIndex(const string& name) const
{
    for(int i=0;i<_subCount;++i)
        if(_subs[i].getName()==name)
            return i;
    return -1;
}
