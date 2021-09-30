
#include "GroundBinLoader.h"
#include "GroundFileFormat.h"
#include "GameSystem.h"
#include "ResourceDB.h"

void GroundBinLoader::loadGround(Ground& g, const string& groundFile, Map& mp) const
{
	IFile* f=FileSystemFactory::getFileSystem()->openFile(groundFile,FA_READ);
	File::GroundFileHeaderEx h;
	File::GroundFileMaterial m;
	f->read(&h,sizeof(File::GroundFileHeader));

	assert(h._header==GROUND_HEADER && h._width==g.getWidth() && h._height==g.getHeight());

	for(int i=0;i<h._matCount;++i)
	{
		f->read(&m,sizeof(File::GroundFileMaterial));
		g.addGameMaterial(ResourceDB::getSingletonRef().getMaterialDB().getData(m._name));
	}

	for (int i = 0; i < h._gameMatCount; ++i)
	{
		f->read(&m, sizeof(File::GroundFileMaterial));
	}

	f->read(g.getGround(),sizeof(unsigned int) * h._width*h._height);

	delete f;
}
