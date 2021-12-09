#include "Helpers.h"
#include "../core/FileSystemFactory.h"

namespace Editor
{

std::string getRelativePath(const std::string& filename)
{
    std::string relativePath;
    Core::FileSystemFactory::getFileSystem()->extractLocalFileName(filename, relativePath);
    return relativePath;
}

std::string removeExtension(const std::string& filename)
{
    size_t pos = filename.find_last_of('.');
    if(pos == std::string::npos)
        return filename;
    else
        return filename.substr(0, pos);
}

}