#pragma once

#include "IR2D_FrameLoader.h"

namespace YAML
{
    class Node;
}

class R2D_StdFrameLoader : IR2D_FrameLoader
{
protected:

	void									parseTextures(const YAML::Node& node, IVideoDevice* d) const;
	void									parseFonts(const YAML::Node& node, IVideoDevice* d) const;
	void									parseLayers(const YAML::Node& node, R2D_Frame& f, IVideoDevice* d) const;
	void									parseLine(const YAML::Node& node, R2D_Layer& l) const;
	void									parseRect(const YAML::Node& node, R2D_Layer& l) const;
	void									parseImage(const YAML::Node& node, R2D_Layer& l) const;
	void									parseText(const YAML::Node& node, R2D_Layer& l) const;

public:

	R2D_StdFrameLoader()					{}

	R2D_Frame*								loadFrame(const string& frameFile, IVideoDevice* device);
};
