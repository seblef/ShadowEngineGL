#pragma once

#include <vector>
#include <string>

using namespace std;

class ITexture;
class IVideoDevice;

namespace YAML
{
class Node; 
};

class ParticleTextureSet
{
	protected:

		typedef std::vector<ITexture*>		TexVector;
		TexVector						_textures;
		IVideoDevice*					_device;

	public:

		ParticleTextureSet(IVideoDevice* d) : _device(d)			{}
		ParticleTextureSet(const ParticleTextureSet& s);
		ParticleTextureSet(const YAML::Node& node, IVideoDevice* d);
		ParticleTextureSet(const string& dir, IVideoDevice* d);
		~ParticleTextureSet();

		void							addTexture(const string& texFile);
		int								getTextureCount() const;
		ITexture*						getTexture(const int n) const;
		ITexture*						operator[](size_t n) const;
};
