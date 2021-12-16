#pragma once

#include <string>

namespace Editor
{

std::string getRelativePath(const std::string& filename);
std::string getPathOnly(const std::string& filename);
std::string removeExtension(const std::string& filename);

}