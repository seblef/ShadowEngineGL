
#include "FileSystemFactory.h"
#include "StdFileSystem.h"

using namespace Core;

IFileSystem* FileSystemFactory::_fileSys=0;

void FileSystemFactory::createFileSystem(const string& fileSysName, const string& dataFolder)
{
	if(_fileSys)		delete _fileSys;

	_fileSys=new StdFileSystem(dataFolder);
}