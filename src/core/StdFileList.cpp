
#include "StdFileList.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>


using namespace Core;

bool StdFileList::isGood(const string& file, const StringVector& filters) const
{
	if(filters.empty())
		return true;
	else
	{
        for(unsigned int i=0;i<filters.size();++i)
		{
			const string& ext(filters[i]);
			if(file.substr(file.length() - ext.length(),ext.length())==ext)
				return true;
		}

		return false;
	}
}

bool StdFileList::isGood(const string& file, const char** filters, int filterCount) const
{
	for (int i = 0; i<filterCount; ++i)
	{
		int extLength = strlen(filters[i]);
		if (file.substr(file.length() - extLength, extLength) == filters[i])
			return true;
	}

	return false;
}

void StdFileList::reset(const StringVector& filters, bool noDirs)
{
	_files.clear();
	_dirs.clear();

    char tmp[2048];
    getcwd(tmp,2048);

    struct dirent *c_file;
    DIR *dir=opendir(".");

    if(dir==0)
        return;

    string file;

    while((c_file=readdir(dir))!=0)
    {
        if(c_file->d_type==DT_DIR)
        {
            if(!noDirs && strcmp(c_file->d_name,"..")!=0 && strcmp(c_file->d_name,".")!=0)
                _dirs.push_back(c_file->d_name);
        }
        else if(c_file->d_type==DT_REG)
        {
            file=c_file->d_name;
            if(isGood(file,filters))
                _files.push_back(file);
        }
    }

    closedir(dir);
}

void StdFileList::reset(const char** filters, int filterCount, bool noDirs)
{
	_files.clear();
	_dirs.clear();

    char tmp[2048];
    getcwd(tmp, 2048);

    struct dirent *c_file;
    DIR *dir=opendir(".");

    if(dir==0)
        return;

    string file;

    while((c_file=readdir(dir))!=0)
    {
        if(c_file->d_type==DT_DIR)
        {
            if(!noDirs && strcmp(c_file->d_name,"..")!=0 && strcmp(c_file->d_name,".")!=0)
                _dirs.push_back(c_file->d_name);
        }
        else if(c_file->d_type==DT_REG)
        {
            file=c_file->d_name;
            if(isGood(file,filters,filterCount))
                _files.push_back(file);
        }
    }

    closedir(dir);

}

