#include "TextureSet.h"
#include "../core/FileSystemFactory.h"
#include "../mediacommon/IVideoDevice.h"
#include "../mediacommon/ITexture.h"
#include <yaml-cpp/yaml.h>
#include <memory>


namespace Particles
{

TextureSet::TextureSet(const YAML::Node& node, IVideoDevice* d)
	: _device(d)
{
	for(YAML::const_iterator tex=node.begin(); tex!= node.end(); ++tex)
		addTexture(tex->as<string>());
}

TextureSet::TextureSet(const TextureSet& s) : _device(s._device)
{
	for (int i = 0; i < s.getTextureCount(); ++i)
		addTexture(s.getTexture(i)->getName());
}

TextureSet::TextureSet(const std::string& dir, IVideoDevice* d) : _device(d)
{
	Core::IFileSystem *sys = FileSystemFactory::getFileSystem();
	const char * textureExts[] = {
		"dds",
		"bmp",
		"jpg",
		"png",
		"tga"
	};

	sys->resetWorkingDirectory();
	sys->changeWorkingDirectory(dir);

	std::unique_ptr<IFileList> l(sys->createFileList((const char**)textureExts, 5, true));
	l->sortFiles();

	for (int i = 0; i < l->getFileCount(); ++i)
		addTexture(l->getFile(i));

	sys->resetWorkingDirectory();
}

TextureSet::~TextureSet()
{
	for(auto& t : _textures)
		t->remRef();
}

void TextureSet::addTexture(const string& texFile)
{
	ITexture *t=_device->createTexture(texFile);
	if(t)
		_textures.push_back(t);
}

int TextureSet::getTextureCount() const
{
    return _textures.size();
}

ITexture* TextureSet::getTexture(const int n) const
{
    return _textures[n];
}

ITexture* TextureSet::operator[](size_t n) const
{
    return getTexture((int)n);
}

}