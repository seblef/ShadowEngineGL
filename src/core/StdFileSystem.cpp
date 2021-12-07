#include "StdFileSystem.h"
#include "StdFile.h"
#include "StdFileList.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


using namespace Core;


void StdFileSystem::getCurrentDir(string& dir)
{
    char sz[2048];
    getcwd(sz,2048);
    dir=sz;
}

StdFileSystem::StdFileSystem(const string& dataFolder)
{
    changeWorkingDirectory(dataFolder);
    getCurrentDir(_startDir);
    _workingDir=_startDir;
}

bool StdFileSystem::changeWorkingDirectory(const string& dir)
{
    bool ok=(chdir(dir.c_str())==0);
    getCurrentDir(_workingDir);
    return ok;
}

void StdFileSystem::resetWorkingDirectory()
{
    changeWorkingDirectory(_startDir);
}

bool StdFileSystem::directoryExists(const string& dir)
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

bool StdFileSystem::createDirectory(const string& dir)
{
    return mkdir(dir.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0;
}

IFileList* StdFileSystem::createFileList(const StringVector& filters, bool noDirs)
{
    return new StdFileList(filters,noDirs);
}

IFileList* StdFileSystem::createFileList(const char** filters, int filterCount, bool noDirs)
{
    return new StdFileList(filters, filterCount, noDirs);
}

IFile* StdFileSystem::openFile(const string& fileName, FileAccess fa)
{
    return new StdFile(fileName,fa);
}