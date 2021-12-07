#include "IFileList.h"
#include <algorithm>

using namespace Core;


int IFileList::getFileCount() const
{
    return _files.size();
}

int IFileList::getDirCount() const
{
    return _dirs.size();
}

const string& IFileList::getFile(int n) const
{
    return _files[n];
}

const string& IFileList::getDir(int n) const
{
    return _dirs[n];
}

void IFileList::sortFiles()
{
    sort(_files.begin(), _files.end());
}

void IFileList::sortDirs()
{
    sort(_dirs.begin(), _dirs.end());
}
