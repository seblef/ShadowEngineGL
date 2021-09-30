
#include "ActorSkinnedRenderer.h"
#include "Renderer.h"



ActorSkinnedRenderer::ActorSkinnedRenderer(IVideoDevice* device) : TRenderer<ActorInstance>(
	device,"Base/GBufferSkin","Base/AddSkin","Base/ShadowsSkin","Base/ShadowsAlphaSkin")
{
    _boneCBuffer=device->createConstantBuffer(MAX_ACTOR_BONES*4,4);
}

ActorSkinnedRenderer::~ActorSkinnedRenderer()
{
	delete _boneCBuffer;
}

void ActorSkinnedRenderer::render(bool addPass)
{
	_device->setInputLayer(VX_3DSKIN);
    _boneCBuffer->set();

	if(addPass)		_addShader->set();
	else			_gbufferShader->set();

	Material *m;
	Matrix4 buffer[MAX_ACTOR_BONES];
	const Actor *mesh;
	int ind_count;
	Renderer& r=Renderer::getSingletonRef();
	r.setObjectTransformation(Matrix4::Identity);

	InstanceVector::const_iterator i(_instances.begin());
	while(i!=_instances.end())
	{
		m=(*i)->getModel()->getMaterial();
		if (addPass)
		{
			if (m->getFlag(MF_ADDPASS))
			{
				m->setAdd((*i)->getTime());
				while (i != _instances.end() && (*i)->getModel()->getMaterial() == m)
				{
					mesh = (*i)->getModel();
					mesh->getVertexBuffer()->set();
					mesh->getIndexBuffer()->set();
					ind_count = mesh->getIndexBuffer()->getIndexCount();

					while (i != _instances.end() && (*i)->getModel()->getMaterial() == m && (*i)->getModel() == mesh)
					{
						m = (*i)->getModel()->getMaterial();
						if (m->isAnimated())
							m->setTime((*i)->getTime());

						memcpy(buffer, (*i)->getBoneMatrices()->getItems(), (*i)->getBoneMatrices()->getCount()*sizeof(Matrix4));
						_boneCBuffer->fill((const void*)buffer);
						_device->renderIndexed(ind_count);

						++i;
					}
				}
			}
			else
			{
				while (i != _instances.end() && (*i)->getModel()->getMaterial() == m)
					++i;
			}
		}
		else
		{
			m->setBase((*i)->getTime());

			while (i != _instances.end() && (*i)->getModel()->getMaterial() == m)
			{
				mesh = (*i)->getModel();
				mesh->getVertexBuffer()->set();
				mesh->getIndexBuffer()->set();
				ind_count = mesh->getIndexBuffer()->getIndexCount();

				while (i != _instances.end() && (*i)->getModel()->getMaterial() == m && (*i)->getModel() == mesh)
				{
					m = (*i)->getModel()->getMaterial();
					if (m->isAnimated())
						m->setTime((*i)->getTime());

					memcpy(buffer, (*i)->getBoneMatrices()->getItems(), (*i)->getBoneMatrices()->getCount()*sizeof(Matrix4));
					_boneCBuffer->fill((const void*)buffer);
					_device->renderIndexed(ind_count);

					++i;
				}
			}
		}
	}
}

void ActorSkinnedRenderer::renderShadows(const ViewFrustum& vf)
{
	_device->setInputLayer(VX_3DSKIN);
    _boneCBuffer->set();
	_shadowShader->set();

	Matrix4 buffer[MAX_ACTOR_BONES];
	const Actor *mesh;
	int ind_count;
	Renderer& r=Renderer::getSingletonRef();
	r.setObjectTransformation(Matrix4::Identity);

	InstanceVector::const_iterator i(_shadowQueue.begin());
	while(i!=_shadowQueue.end())
	{
		mesh=(*i)->getModel();

		mesh->getVertexBuffer()->set();
		mesh->getIndexBuffer()->set();
		ind_count=mesh->getIndexBuffer()->getIndexCount();

		while(i!=_shadowQueue.end() && (*i)->getModel()==mesh)
		{
			if (vf.isBoxInside((*i)->getWorldBBox()))
			{
				memcpy(buffer, (*i)->getBoneMatrices()->getItems(), (*i)->getBoneMatrices()->getCount()*sizeof(Matrix4));
				_boneCBuffer->fill((const void*)buffer);
				_device->renderIndexed(ind_count);
			}

			++i;
		}
	}

	if (!_shadowAlphaQueue.empty())
		renderAlphaShadows(vf);
}

void ActorSkinnedRenderer::renderAlphaShadows(const ViewFrustum& vf)
{
	_shadowAlphaShader->set();

	Material *m;
	Matrix4 buffer[MAX_ACTOR_BONES];
	const Actor *mesh;
	int ind_count;

	InstanceVector::const_iterator i(_shadowAlphaQueue.begin());
	while (i != _shadowAlphaQueue.end())
	{
		m = (*i)->getModel()->getMaterial();
		m->setShadow((*i)->getTime());

		while (i != _shadowAlphaQueue.end() && (*i)->getModel()->getMaterial() == m)
		{
			mesh = (*i)->getModel();
			mesh->getVertexBuffer()->set();
			mesh->getIndexBuffer()->set();
			ind_count = mesh->getIndexBuffer()->getIndexCount();

			while (i != _shadowAlphaQueue.end() && (*i)->getModel()->getMaterial() == m && (*i)->getModel() == mesh)
			{
				m = (*i)->getModel()->getMaterial();

				if (vf.isBoxInside((*i)->getWorldBBox()))
				{
					if (m->isAnimated())
						m->setTime((*i)->getTime());

					memcpy(buffer, (*i)->getBoneMatrices()->getItems(), (*i)->getBoneMatrices()->getCount()*sizeof(Matrix4));
					_boneCBuffer->fill((const void*)buffer);
					_device->renderIndexed(ind_count);
				}
				++i;
			}
		}
	}
}
