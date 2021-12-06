
#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <yaml-cpp/yaml.h>

namespace YAML
{

// Vector2 encoder/decoder
template<>
struct convert<Vector2> {
    static Node encode(const Core::Vector2& v)
    {
        Node node;
        node.push_back(v.x);
        node.push_back(v.y);
        return node;
    }

    static bool decode(const Node& node, Core::Vector2& v)
    {
        if(!node.IsSequence() || node.size() != 2)
            return false;
        
        v.x = node[0].as<float>();
        v.y = node[1].as<float>();
        return true;
    }
};

// Vector3 encoder/decoder
template<>
struct convert<Core::Vector3> {
    static Node encode(const Core::Vector3& v)
    {
        Node node;
        node.push_back(v.x);
        node.push_back(v.y);
        node.push_back(v.z);
        return node;
    }

    static bool decode(const Node& node, Core::Vector3& v)
    {
        if(!node.IsSequence() || node.size() != 3)
            return false;
        
        v.x = node[0].as<float>();
        v.y = node[1].as<float>();
        v.z = node[2].as<float>();
        return true;
    }
};

// Color encoder/decoder
template<>
struct convert<Core::Color> {
    static Node encode(const Core::Color& c)
    {
        Node node;
        node.push_back(c.x);
        node.push_back(c.y);
        node.push_back(c.z);
        node.push_back(c.w);
        return node;
    }

    static bool decode(const Node& node, Color& c)
    {
        if(!node.IsSequence() || node.size() != 4)
            return false;
        
        c.x = node[0].as<float>();
        c.y = node[1].as<float>();
        c.z = node[2].as<float>();
        c.w = node[3].as<float>();
        return true;
    }
};

}