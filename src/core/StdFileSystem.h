
#pragma once

#include "IFileSystem.h"

namespace Core
{

class StdFileSystem : public IFileSystem
{
protected:
	void				getCurrentDir(string& dir);

public:

	StdFileSystem(const string& dataFolder);
	~StdFileSystem()				{}

	bool				changeWorkingDirectory(const string& dir);
	void				resetWorkingDirectory();
	bool				directoryExists(const string& dir);
    bool				createDirectory(const string& dir);

	IFileList*			createFileList(const StringVector& filters, bool noDirs);
	IFileList*			createFileList(const char** filters, int filterCount, bool noDirs);
	IFile*				openFile(const string& fileName, FileAccess fa);
};

}
