
#pragma once

#include <vector>
#include <string>

using namespace std;

namespace Core
{

typedef vector<string>			StringVector;

class IFileList
{
	protected:

		StringVector			_files;
		StringVector			_dirs;

	public:

		IFileList() {}
		virtual ~IFileList() {}

		int						getFileCount() const;
		int						getDirCount() const;

		const string&			getFile(int n) const;
		const string&			getDir(int n) const;

		void					sortFiles();
		void					sortDirs();

		virtual void			reset(const StringVector& filter, bool noDirs=false)=0;
		virtual void			reset(const char** filters, int filterCount, bool noDirs = false) = 0;
};

}
