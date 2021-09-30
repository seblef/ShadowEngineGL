
#ifndef _STDFILESYSTEM_H_
#define _STDFILESYSTEM_H_

#include "IFileSystem.h"
#include "StdFile.h"
#include "StdFileList.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace Core
{

class StdFileSystem : public IFileSystem
{
protected:

	void				getCurrentDir(string& dir)
	{
		char sz[2048];
        getcwd(sz,2048);
		dir=sz;
	}

public:

	StdFileSystem()
	{
		getCurrentDir(_startDir);
		_workingDir=_startDir;
	}
	~StdFileSystem()				{}

	bool				changeWorkingDirectory(const string& dir)
	{
        bool ok=(chdir(dir.c_str())==0);
		getCurrentDir(_workingDir);
		return ok;
	}
	void				resetWorkingDirectory()												{ changeWorkingDirectory(_startDir); }
	bool				directoryExists(const string& dir)
	{
		string temp(_workingDir);
		if(changeWorkingDirectory(dir))
		{
			changeWorkingDirectory(temp);
			return true;
		}
		else
			return false;
	}

    bool				createDirectory(const string& dir)									{ return mkdir(dir.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0; }

	IFileList*			createFileList(const StringVector& filters, bool noDirs)			{ return new StdFileList(filters,noDirs); }
	IFileList*			createFileList(const char** filters, int filterCount, bool noDirs)	{ return new StdFileList(filters, filterCount, noDirs); }
	IFile*				openFile(const string& fileName, FileAccess fa)						{ return new StdFile(fileName,fa); }
};

}

#endif
