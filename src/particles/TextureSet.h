#pragma once

#include <vector>
#include <string>

class ITexture;
class IVideoDevice;

namespace YAML
{
class Node; 
};

namespace Particles
{

class TextureSet
{
	protected:

		std::vector<ITexture*> _textures;
        std::string _folder;
		IVideoDevice* _device;

	public:

		TextureSet(IVideoDevice* d) : _device(d)			{}
		TextureSet(const TextureSet& s);
		TextureSet(const YAML::Node& node, IVideoDevice* d);
		TextureSet(const std::string& dir, IVideoDevice* d);
		~TextureSet();

		void addTexture(const std::string& texFile);
		int getTextureCount() const;
		ITexture* getTexture(const int n) const;
		ITexture* operator[](size_t n) const;

        const std::string& getFolder() const { return _folder; }
};

}