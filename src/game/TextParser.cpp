
#include "TextParser.h"
#include "ObjectFlags.h"
#include "GeometryLoader.h"
#include "ResourceDB.h"



const char* const g_PhysicShapes[PSHAPE_COUNT] =
{
	"box",
	"sphere",
	"capsule",
	"mesh",
	"chull"
};

const char * const g_MaterialTexNames[TS_COUNT] = {
	"diffuse",
	"normal",
	"specular",
	"emissive",
	"environment"
};

const char * const g_MaterialBlendNames[] =
{
	"zero",
	"one",
	"src_color",
	"inv_src_color",
	"src_alpha",
	"inv_src_alpha",
	"dest_alpha",
	"inv_dest_alpha",
	"dest_color",
	"inv_dest_color",
	"src_alpha_sat",
	"both_src_alpha",
	"both_inv_src_alpha",
	"blend_factor",
	"inv_blend_factor",
	"src_color2",
	"inv_src_color2"
};

const char* const g_LightTypes[Light::LT_COUNT] =
{
	"omni",
	"spot",
	"area"
};


TextureSlot TextParser::getTextureSlot(const string& b)
{
	TextureSlot t = TS_DIFFUSE;
	for (int i = 0; i<TS_COUNT; ++i)
	{
		if (b == g_MaterialTexNames[i])
			t = (TextureSlot)i;
	}

	return t;
}

BlendMode TextParser::getBlendMode(const string& b)
{
	BlendMode m = BLEND_ZERO;
	for (int i = 0; i<BLEND_COUNT; ++i)
	{
		if (b == g_MaterialBlendNames[i])
			m = (BlendMode)i;
	}

	return m;
}

CullMode TextParser::getCullMode(const string& c)
{
	CullMode cm = CULL_BACK;
	if (c =="none")			cm = CULL_NONE;
	else if (c == "front")		cm = CULL_FRONT;

	return cm;
}

void TextParser::parseVector2(ScriptFile& fl, Vector2& v)
{
	v.x = stof(fl.getToken());
	v.y = stof(fl.getToken());
}

void TextParser::parseMaterialAnimation(MaterialCreate& mc, ScriptFile& sf)
{
	string aname(sf.getToken());
	if (aname == "uv")
	{
		mc._animations |= MA_UV;

		string token(sf.getToken());
		while (sf.good() && token != "end_anim")
		{
			if (token == "rotation")
				mc._uvAnim._rotPerSec = stof(sf.getToken());
			else if (token == "offset")
				parseVector2(sf, mc._uvAnim._offsetPerSec);

			token = sf.getToken();
		}
	}
	else if (aname == "texture")
	{
		mc._animations |= MA_TEXTURE;
		mc._textureAnim._animTime = stof(sf.getToken());
		mc._textureAnim._tileCount = stoi(sf.getToken());
		mc._textureAnim._tilePerRow = stoi(sf.getToken());
		mc._textureAnim._tilePerColumn = stoi(sf.getToken());
	}
	else if (aname == "fade")
	{
		mc._animations |= MA_FADE;
		mc._fadeAnim._startTime = stof(sf.getToken());
		mc._fadeAnim._endTime = stof(sf.getToken());
	}
}

GameMaterial* TextParser::parseMaterial(ScriptFile& sf, string* matName)
{
	string name(sf.getToken());
	MaterialCreate mc;
	Geometry *debris = 0;

	string token(sf.getToken());
	while (sf.good() && token != "end_material")
	{
		if (token == "debris")
			debris = ResourceDB::getSingletonRef().getGeometryDB().getData(sf.getToken());
		else if (token == "renderer")
		{
			token = sf.getToken();
			while (sf.good() && token != "end_renderer" && token != "end_material")
			{
				if (token =="tex")
				{
					TextureSlot stage = getTextureSlot(sf.getToken());
					mc._textures[stage] = sf.getToken();
				}
				else if (token == "ambient")
					sf.parseColor(mc._ambient);
				else if (token == "diffuse")
					sf.parseColor(mc._diffuse);
				else if (token == "specular")
					sf.parseColor(mc._specular);
				else if (token == "shine")
					mc._shininess = stof(sf.getToken());
				else if (token == "spec_intens")
					mc._specularIntensity = stof(sf.getToken());
				else if (token == "blend")
				{
					mc._blendEnable = true;
					mc._srcBlend = getBlendMode(sf.getToken());
					mc._destBlend = getBlendMode(sf.getToken());
				}
				else if (token == "alpha_material")
					mc._flags |= MF_ALPHA;
				else if (token == "animation")
					parseMaterialAnimation(mc, sf);
				else if (token == "cull")
					mc._cull = getCullMode(sf.getToken());
				else if (token == "uv_offset")
					parseVector2(sf, mc._uvOffset);
				else if (token == "uv_scale")
					parseVector2(sf, mc._uvScale);
				else if (token == "uv_rotation")
					mc._uvRot = stof(sf.getToken());
				else if (token == "cast_no_shadows")
					mc._flags |= MF_CASTNOSHADOWS;
				else if (token == "alpha_shadows")
					mc._flags|=MF_ALPHASHADOWS;
				else if (token == "reflection")
					mc._reflectivity=stof(sf.getToken());

				token = sf.getToken();
			}
		}

		token = sf.getToken();
	}

	if (matName)
		*matName = name;

	return new GameMaterial(new Material(mc, Renderer::getSingletonRef().getVideoDevice()), debris);
}

TemplateMesh* TextParser::parseMesh(ScriptFile& sf, string* meshName, const string* geoDir)
{
	string name(sf.getToken());
	string t(sf.getToken());
	GameMaterial* gmat = 0;
	Geometry *geo = 0;
	unsigned int flags = 0;
	PhysicShape shape = PSHAPE_BOX;
	TemplateMesh* mesh = 0;

    while (sf.good() && t != "end_mesh")
	{
		if (t == "geometry")
			geo = ResourceDB::getSingletonRef().getGeometryDB().getData(sf.getToken());
		else if (t == "material")
			gmat = ResourceDB::getSingletonRef().getMaterialDB().getData(sf.getToken());
		else if (t == "no_collision")
			flags |= OF_NOCOLLISION;
		else if (t == "physic_shape")
		{
			t = sf.getToken();
			for (int i = 0; i < PSHAPE_COUNT; ++i)
			if (t == g_PhysicShapes[i])
				shape = (PhysicShape)i;
		}

		t = sf.getToken();
	}

    assert(geo);
	mesh=new TemplateMesh(geo,gmat,flags,shape,false);

	if (meshName)
		*meshName = name;

	return mesh;
}

TemplateParticleSystem* TextParser::parseParticles(ScriptFile& sf, string* particleName)
{
	string name(sf.getToken());
	ParticleSystemTemplate *t = new ParticleSystemTemplate(sf);
	TemplateParticleSystem *tps = new TemplateParticleSystem(t);

	if (particleName)
		*particleName = name;

	return tps;
}

void TextParser::parseLight(ScriptFile& sf, LightCreate_t& lc, Light::LightType& type)
{
	string t(sf.getToken());

	for (int i = 0; i<Light::LT_COUNT; ++i)
	if (t == g_LightTypes[i])
		type = (Light::LightType)i;

	lc._world.createIdentity();
	lc._color = Color::White;
	lc._range = 2;
	lc._direction = -Vector3::YAxisVector;
	lc._nearAngle = 45;
	lc._farAngle = 60;
	lc._areaWidth = 2;
	lc._areaHeight = 1;
	lc._nearZ = 0.1f;
	lc._castShadows = true;
	lc._shadowMapSize = 256;
	lc._shadowMapFilter = 4;

	t = sf.getToken();
	while (sf.good() && t != "end_light")
	{
		if (t == "color")					sf.parseColor(lc._color);
		else if (t == "range")				lc._range = stof(sf.getToken());
        else if (t == "position")
        {
            Vector3 p;
            sf.parseVector(p);
            lc._world=p;
        }
		else if (t == "direction")			sf.parseVector(lc._direction);
		else if (t == "near_angle")			lc._nearAngle = stof(sf.getToken());
		else if (t == "far_angle")			lc._farAngle = stof(sf.getToken());
		else if (t == "area_width")			lc._areaWidth = stof(sf.getToken());
		else if (t == "area_height")		lc._areaHeight = stof(sf.getToken());
		else if (t == "near_z")				lc._nearZ = stof(sf.getToken());
		else if (t == "cast_no_shadows")	lc._castShadows = false;
		else if (t == "shadowmap_size")		lc._shadowMapSize = stoi(sf.getToken());
		else if (t == "shadowmap_filter")	lc._shadowMapFilter = stoi(sf.getToken());

		t = sf.getToken();
	}

	Light::computeMatrix(lc._direction, lc._world);
}
