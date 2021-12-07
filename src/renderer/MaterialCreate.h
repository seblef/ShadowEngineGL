#pragma once

#include "../core/Vector2.h"
#include "../mediacommon/IVideoDevice.h"
#include "../mediacommon/ITexture.h"
#include <string>

using namespace Core;
using namespace std;

enum TextureSlot
{
	TS_DIFFUSE = 0,
	TS_NORMALMAP,
	TS_SPECULAR,
	TS_EMISSIVE,
	TS_ENVIONMENT,

	TS_COUNT
};

//			Material flags

#define MF_NONE						0
#define MF_ALPHA					(1 << 0)
#define MF_ADDPASS					(1 << 1)
#define MF_CASTNOSHADOWS			(1 << 2)
#define MF_ALPHASHADOWS				(1 << 3)

#define MF_ALL						(MF_ALPHA | MF_ADDPASS | MF_CASTNOSHADOWS)

//			Material animations

#define MA_NONE						0
#define MA_TEXTURE					(1 << 0)
#define MA_UV						(1 << 1)
#define MA_FADE						(1 << 2)

class MaterialCreate
{
public:

	class MC_TextureAnim
	{
	public:

		float			_animTime;
		int				_tilePerRow;
		int				_tilePerColumn;
		int				_tileCount;

		MC_TextureAnim() : _animTime(0.1f), _tilePerRow(1), _tilePerColumn(1), _tileCount(1)			{}
		~MC_TextureAnim()			{}
	};

	class MC_UVAnim
	{
	public:

		Vector2		_offsetPerSec;
		float		_rotPerSec;

		MC_UVAnim() : _offsetPerSec(Vector2::NullVector), _rotPerSec(0)		{}
		~MC_UVAnim()														{}
	};

	class MC_FadeAnim
	{
	public:

		float		_startTime;
		float		_endTime;

		MC_FadeAnim() : _startTime(0), _endTime(1)							{}
		~MC_FadeAnim()														{}
	};

	Color			_ambient;
	Color			_diffuse;
	Color			_specular;
	float			_shininess;
	float			_specularIntensity;
	float			_reflectivity;

	Vector2			_uvOffset;
	Vector2			_uvScale;
	float			_uvRot;

	string			_textures[TS_COUNT];

	CullMode		_cull;

	bool			_blendEnable;
	BlendMode		_srcBlend;
	BlendMode		_destBlend;

	unsigned int	_flags;
	unsigned int	_animations;

	MC_TextureAnim	_textureAnim;
	MC_UVAnim		_uvAnim;
	MC_FadeAnim		_fadeAnim;

	MaterialCreate() : _ambient(Color::Black), _diffuse(Color::White), _specular(Color::Black),
		_shininess(200.f), _specularIntensity(0.2), _reflectivity(0),
		_uvOffset(Vector2::NullVector), _uvScale(Vector2::UnitVector), _uvRot(0),
		_cull(CULL_BACK), _blendEnable(false), _srcBlend(BLEND_ONE), _destBlend(BLEND_ZERO),
		_flags(0), _animations(0)
	{}
	~MaterialCreate()			{}
};
