
#include "ParticleTextureSet.h"
#include "../core/FileSystemFactory.h"
#include "../mediacommon/IVideoDevice.h"
#include "../mediacommon/ITexture.h"
#include <yaml-cpp/yaml.h>


using namespace Core;


ParticleTextureSet::ParticleTextureSet(const YAML::Node& node, IVideoDevice* d)
	: _device(d)
{
	for(YAML::const_iterator tex=node.begin(); tex!= node.end(); ++tex)
		addTexture(tex->as<string>());
}

ParticleTextureSet::ParticleTextureSet(const ParticleTextureSet& s) : _device(s._device)
{
	for (int i = 0; i < s.getTextureCount(); ++i)
		addTexture(s.getTexture(i)->getName());
}

ParticleTextureSet::ParticleTextureSet(const string& dir, IVideoDevice* d) : _device(d)
{
	IFileSystem *sys = FileSystemFactory::getFileSystem();
	const char * textureExts[] = {
		"dds",
		"bmp",
		"jpg",
		"png",
		"tga"
	};

	sys->resetWorkingDirectory();
	sys->changeWorkingDirectory(dir);

	IFileList *l = sys->createFileList((const char**)textureExts, 5, true);
	l->sortFiles();

	for (int i = 0; i < l->getFileCount(); ++i)
		addTexture(l->getFile(i));

	sys->resetWorkingDirectory();
	delete l;
}

ParticleTextureSet::~ParticleTextureSet()
{
	for(auto& t : _textures)
		t->remRef();
}

void ParticleTextureSet::addTexture(const string& texFile)
{
	ITexture *t=_device->createTexture(texFile);
	if(t)
		_textures.push_back(t);
}

int ParticleTextureSet::getTextureCount() const
{
    return _textures.size();
}

ITexture* ParticleTextureSet::getTexture(const int n) const
{
    return _textures[n];
}

ITexture* ParticleTextureSet::operator[](size_t n) const
{
    return getTexture((int)n);
}
