
#include "TextParser.h"
#include "ObjectFlags.h"
#include "GeometryLoader.h"
#include "ResourceDB.h"
#include "../core/YAMLCore.h"
#include "../renderer/Material.h"
#include "../renderer/Renderer.h"


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


TextureSlot getTextureSlot(const string& b)
{
	TextureSlot t = TS_DIFFUSE;
	for (int i = 0; i<TS_COUNT; ++i)
	{
		if (b == g_MaterialTexNames[i])
			t = (TextureSlot)i;
	}

	return t;
}

BlendMode getBlendMode(const string& b)
{
	BlendMode m = BLEND_ZERO;
	for (int i = 0; i<BLEND_COUNT; ++i)
	{
		if (b == g_MaterialBlendNames[i])
			m = (BlendMode)i;
	}

	return m;
}

CullMode getCullMode(const string& c)
{
	CullMode cm = CULL_BACK;
	if (c =="none")			cm = CULL_NONE;
	else if (c == "front")		cm = CULL_FRONT;

	return cm;
}

void parseMaterialAnimation(MaterialCreate& mc, const YAML::Node& node)
{
    YAML::Node uv = node["uv"];
	if(uv)
	{
		mc._animations |= MA_UV;
		mc._uvAnim._rotPerSec = uv["rotation"].as<float>(mc._uvAnim._rotPerSec);
		mc._uvAnim._offsetPerSec = uv["offset"].as<Vector2>(mc._uvAnim._offsetPerSec);
	}
    YAML::Node tex = node["texture"];
	if(tex)
	{
		mc._animations |= MA_TEXTURE;
		mc._textureAnim._animTime = tex["time"].as<float>(mc._textureAnim._animTime);
		mc._textureAnim._tileCount = tex["tile_count"].as<int>(mc._textureAnim._tileCount);
		mc._textureAnim._tilePerRow = tex["tiles_per_row"].as<int>(mc._textureAnim._tilePerRow);
		mc._textureAnim._tilePerColumn = tex["tiles_per_col"].as<int>(mc._textureAnim._tilePerColumn);
	}
    YAML::Node fade = node["fade"];
	if(fade)
	{
		mc._animations |= MA_FADE;
		mc._fadeAnim._startTime = fade["start_time"].as<float>(mc._fadeAnim._startTime);
		mc._fadeAnim._endTime = fade["end_time"].as<float>(mc._fadeAnim._endTime);
	}
}

Material* parseRendererMaterial(const YAML::Node& node)
{
	MaterialCreate mc;
    YAML::Node tex(node["textures"]);
    if(tex)
    {
        for(YAML::const_iterator t=tex.begin(); t!=tex.end(); ++t)
        {
            TextureSlot stage = getTextureSlot(t->first.as<string>());
            mc._textures[stage] = t->second.as<string>();
        }
    }

    YAML::Node blend(node["blending"]);
    if(blend)
    {
        mc._srcBlend = getBlendMode(blend["src"].as<string>(""));
        mc._destBlend = getBlendMode(blend["dest"].as<string>(""));
    }

    YAML::Node anim = node["animation"];
    if(anim)
        parseMaterialAnimation(mc, anim);

    if(node["cull"])
        mc._cull = getCullMode(node["cull"].as<string>());

    YAML::Node uv(node["uv"]);
    if(uv)
    {
        mc._uvOffset = uv["offset"].as<Vector2>(mc._uvOffset);
        mc._uvScale = uv["scale"].as<Vector2>(mc._uvScale);
        mc._uvRot = uv["rotation"].as<float>(mc._uvRot);
    }

    mc._ambient = node["ambient_color"].as<Color>(mc._ambient);
    mc._diffuse = node["diffuse_color"].as<Color>(mc._diffuse);
    mc._specular = node["specular_color"].as<Color>(mc._specular);
    mc._shininess = node["shininess"].as<float>(mc._shininess);
    mc._specularIntensity = node["specular_intensity"].as<float>(mc._specularIntensity);
    mc._reflectivity = node["reflectivity"].as<float>(mc._reflectivity);

    if(node["alpha"].as<bool>(false))
        mc._flags |= MF_ALPHA;
    if(node["cast_no_shadows"].as<bool>(false))
        mc._flags |= MF_CASTNOSHADOWS;
    if(node["alpha_shadows"].as<bool>(false))
        mc._flags |= MF_ALPHASHADOWS;

    return new Material(mc, Renderer::getSingletonRef().getVideoDevice());
}

GameMaterial* TextParser::parseMaterial(const YAML::Node& node, const string& name)
{
	Geometry *debris = 0;
    Material *mat = 0;

	debris = ResourceDB::getSingletonRef().getGeometryDB().getData(
		node["debris"].as<string>("")
	);

    YAML::Node r = node["renderer"];
	if(r)
        mat = parseRendererMaterial(r);
    else
        mat = new Material(Renderer::getSingletonRef().getVideoDevice());
	return new GameMaterial(mat, debris);
}

TemplateMesh* TextParser::parseMesh(const YAML::Node& node, const string& name, const string* geoDir)
{
	GameMaterial* gmat = ResourceDB::getSingletonRef().getMaterialDB().getData(node["material"].as<string>());
	Geometry *geo = ResourceDB::getSingletonRef().getGeometryDB().getData(node["geometry"].as<string>());
	unsigned int flags = 0;

	if(node["no_collision"].as<bool>(false))
		flags = OF_NOCOLLISION;

	PhysicShape shape = PSHAPE_BOX;
	const string& s(node["physic_shape"].as<string>(""));
	for (int i = 0; i < PSHAPE_COUNT; ++i)
		if (s == g_PhysicShapes[i])
			shape = (PhysicShape)i;

    assert(geo);
	return new TemplateMesh(geo,gmat,flags,shape,false);
}

TemplateParticleSystem* TextParser::parseParticles(const YAML::Node& node, const string& name)
{
	ParticleSystemTemplate *t = new ParticleSystemTemplate(node);
	return new TemplateParticleSystem(t);
}

void TextParser::parseLight(const YAML::Node& node, LightCreate_t& lc, Light::LightType& type)
{
	const string& t(node["type"].as<string>(""));
	for (int i = 0; i<Light::LT_COUNT; ++i)
		if (t == g_LightTypes[i])
			type = (Light::LightType)i;

	lc._world.createIdentity();
	lc._world = node["position"].as<Vector3>(Vector3::NullVector);
	lc._color = node["color"].as<Color>(Color::White);
	lc._range = node["range"].as<float>(2.f);
	lc._direction = node["direction"].as<Vector3>(-Vector3::YAxisVector);
	lc._nearAngle = node["near_angle"].as<float>(45.f);
	lc._farAngle = node["far_angle"].as<float>(60.f);
	lc._areaWidth = node["area_width"].as<float>(2.f);
	lc._areaHeight = node["area_height"].as<float>(1.f);
	lc._nearZ = node["near_z"].as<float>(.1f);
	lc._castShadows = node["cast_no_shadows"].as<bool>(false);
	lc._shadowMapSize = node["shadowmap_size"].as<int>(256);
	lc._shadowMapFilter = node["shadowmap_filter"].as<int>(4);

	Light::computeMatrix(lc._direction, lc._world);
}
