
#include "R2D_StdFrameLoader.h"
#include "R2D_FontLoader.h"
#include "R2D_Line.h"
#include "R2D_Image.h"
#include "R2D_Rect.h"
#include "R2D_Text.h"
#include "Renderer2D.h"
#include "../core/YAMLCore.h"
#include "../mediacommon/ITexture.h"
#include "../loguru.hpp"


R2D_Frame* R2D_StdFrameLoader::loadFrame(const string& frameFile, IVideoDevice* d)
{
    YAML::Node root;
    try
    {
        root = YAML::LoadFile(frameFile);
    }
    catch(const std::exception& e)
    {
        LOG_S(ERROR) << e.what();
    }

	R2D_Frame* f=new R2D_Frame;

    YAML::Node textures = root["textures"];
    if(textures)
        parseTextures(textures, d);
    
    YAML::Node fonts = root["fonts"];
    if(fonts)
        parseFonts(fonts, d);

    YAML::Node layers = root["layers"];
    parseLayers(layers, *f, d);

	return f;
}

void R2D_StdFrameLoader::parseTextures(const YAML::Node& node, IVideoDevice* d) const
{
    for(YAML::const_iterator t=node.begin(); t!=node.end(); ++t)
    {
        const string& texName(t->first.as<string>());
        const string& fileName(t->second.as<string>());

        if(!Renderer2D::getSingletonRef().getTextureDB().getData(texName))
        {
            ITexture* t=d->createTexture(fileName);
            Renderer2D::getSingletonRef().getTextureDB().registerData(texName,t);
        }
    }
}

void R2D_StdFrameLoader::parseFonts(const YAML::Node& node, IVideoDevice* d) const
{
    for(YAML::const_iterator f=node.begin(); f!=node.end(); ++f)
    {
        const string& fontName(f->first.as<string>());
        const string& fileName(f->second.as<string>());

        if(!Renderer2D::getSingletonRef().getTextureDB().getData(fontName))
        {
            R2D_Font* f=R2D_FontLoader::loadFont(fileName, d);
            Renderer2D::getSingletonRef().getFontDB().registerData(fontName, f);
        }
    }
}

void R2D_StdFrameLoader::parseLayers(const YAML::Node& node, R2D_Frame& f, IVideoDevice* d) const
{
    for(YAML::const_iterator l=node.begin(); l!=node.end(); ++l)
    {
        R2D_Layer *layer=new R2D_Layer;
        f.getVector().push_back(layer);

        for(YAML::const_iterator obj=l->begin(); obj!=l->end(); ++obj)
        {
            YAML::Node object = *obj;
            const string& obj_class(object["class"].as<string>());

            if(obj_class == "line")
                parseLine(object, *layer);
            else if(obj_class == "rect")
                parseRect(object, *layer);
            else if(obj_class == "image")
                parseImage(object, *layer);
            else if(obj_class == "text")
                parseText(object, *layer);
        }
    }
}

void R2D_StdFrameLoader::parseLine(const YAML::Node& node, R2D_Layer& l) const
{
	Vector2 p = node["position"].as<Vector2>(Vector2::NullVector);
	Vector2 s = node["size"].as<Vector2>(Vector2::UnitVector);
	Color c = node["color"].as<Color>(Color::White);
	l.getSet().insert(new R2D_Line(p,s,c));
}

void R2D_StdFrameLoader::parseRect(const YAML::Node& node, R2D_Layer& l) const
{
	Vector2 p = node["position"].as<Vector2>(Vector2::NullVector);
	Vector2 s = node["size"].as<Vector2>(Vector2::UnitVector);
	Color c = node["color"].as<Color>(Color::White);
	l.getSet().insert(new R2D_Rect(p,s,c));
}

void R2D_StdFrameLoader::parseImage(const YAML::Node& node, R2D_Layer& l) const
{
	Vector2 p = node["position"].as<Vector2>(Vector2::NullVector);
	Vector2 s = node["size"].as<Vector2>(Vector2::UnitVector);
	Color c = node["color"].as<Color>(Color::White);
	Vector2 uv_start = node["uv_start"].as<Vector2>(Vector2::NullVector);
	Vector2 uv_end = node["uv_end"].as<Vector2>(Vector2::NullVector);
	ITexture* tex = Renderer2D::getSingletonRef().getTextureDB().getData(
        node["texture"].as<string>("")
    );
	l.getSet().insert(new R2D_Image(p,s,c,uv_start,uv_end-uv_start,tex,false));
}

void R2D_StdFrameLoader::parseText(const YAML::Node& node, R2D_Layer& l) const
{
	Vector2 p = node["position"].as<Vector2>(Vector2::NullVector);
	Color c = node["color"].as<Color>(Color::White);
    const string& text = node["text"].as<string>("");
	R2D_Font* f = Renderer2D::getSingletonRef().getFontDB().getData(
        node["font"].as<string>()
    );
	l.getSet().insert(new R2D_Text(f, text, p, Vector2::UnitVector, c));
}
