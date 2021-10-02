
#pragma once

#include "IFileSystem.h"

namespace Core
{

class FileSystemFactory
{
protected:

	static IFileSystem*			_fileSys;

public:

	static void					createFileSystem(const string& fileSysName, const string& dataFolder);
	static IFileSystem*			getFileSystem()									{ return _fileSys; }
};

}

#define g_FileSystem			Core::FileSystemFactory::getFileSystem()
