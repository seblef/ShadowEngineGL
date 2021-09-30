
#include "MeshRenderer.h"
#include "Renderer.h"


void MeshRenderer::render(bool addPass)
{
	Material *m;
	const Mesh *mesh;
	int ind_count;
	Renderer& r=Renderer::getSingletonRef();

	_device->setInputLayer(VX_3D);
	if(addPass)				_addShader->set();
	else					_gbufferShader->set();

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
					mesh->getGeometry()->set();
					ind_count = mesh->getGeometry()->getIndexBuffer()->getIndexCount();

					while (i != _instances.end() && (*i)->getModel()->getMaterial() == m && (*i)->getModel() == mesh)
					{
						m = (*i)->getModel()->getMaterial();
						if (m->isAnimated())
							m->setTime((*i)->getTime());

						r.setObjectTransformation((*i)->getWorldMatrix());
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
				mesh->getGeometry()->set();
				ind_count = mesh->getGeometry()->getIndexBuffer()->getIndexCount();

				while (i != _instances.end() && (*i)->getModel()->getMaterial() == m && (*i)->getModel() == mesh)
				{
					m = (*i)->getModel()->getMaterial();
					if (m->isAnimated())
						m->setTime((*i)->getTime());

					r.setObjectTransformation((*i)->getWorldMatrix());
					_device->renderIndexed(ind_count);

					++i;
				}
			}
		}
	}
}

void MeshRenderer::renderShadows(const ViewFrustum& vf)
{
	_device->setInputLayer(VX_3D);
	_shadowShader->set();

	const Mesh* mesh;
	int ind_count;
	Renderer& r(Renderer::getSingletonRef());

	InstanceVector::const_iterator i(_shadowQueue.begin());
	while(i!=_shadowQueue.end())
	{
		mesh=(*i)->getModel();
		mesh->getGeometry()->set();
		ind_count=mesh->getGeometry()->getIndexBuffer()->getIndexCount();

		while(i!=_shadowQueue.end() && (*i)->getModel()==mesh)
		{
			if(vf.isBoxInside((*i)->getWorldBBox()))
			{
				r.setObjectTransformation((*i)->getWorldMatrix());
				_device->renderIndexed(ind_count);
			}

			++i;
		}
	}

	if (!_shadowAlphaQueue.empty())
		renderShadowAlpha(vf);
}

void MeshRenderer::renderShadowAlpha(const ViewFrustum& vf)
{
	_shadowAlphaShader->set();

	Material *m;
	const Mesh *mesh;
	int ind_count;
	Renderer& r = Renderer::getSingletonRef();

	InstanceVector::const_iterator i(_shadowAlphaQueue.begin());
	while (i != _shadowAlphaQueue.end())
	{
		m = (*i)->getModel()->getMaterial();
		m->setShadow((*i)->getTime());

		while (i != _shadowAlphaQueue.end() && (*i)->getModel()->getMaterial() == m)
		{
			mesh = (*i)->getModel();
			mesh->getGeometry()->set();
			ind_count = mesh->getGeometry()->getIndexBuffer()->getIndexCount();

			while (i != _shadowAlphaQueue.end() && (*i)->getModel()->getMaterial() == m && (*i)->getModel() == mesh)
			{
				m = (*i)->getModel()->getMaterial();

				if (vf.isBoxInside((*i)->getWorldBBox()))
				{
					if (m->isAnimated())
						m->setTime((*i)->getTime());

					r.setObjectTransformation((*i)->getWorldMatrix());
					_device->renderIndexed(ind_count);
				}

				++i;
			}
		}
	}
}
