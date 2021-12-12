
#include "GeoGEOLoader.h"
#include "GeoFileFormat.h"
#include "Geometry.h"
#include "../core/FileSystemFactory.h"
#include "../loguru.hpp"
#include <memory>



Geometry* GeoGEOLoader::loadGeometry(const string& geoFile)
{
	Geometry* geo=0;

	std::unique_ptr<IFile> fl(FileSystemFactory::getFileSystem()->openFile(geoFile,FA_READ));
	if(fl->good())
	{
		File::GeoFileHeader h;
		fl->read(&h,sizeof(File::GeoFileHeader));

		if(h._header==GEO_HEADER)
		{
			geo=new Geometry(h._vxCount,h._triCount);
			fl->read(geo->getVertices(),sizeof(Vx3D)*h._vxCount);
			fl->read(geo->getTriangles(),sizeof(unsigned short)*3*h._triCount);
		}
		else
        {
            LOG_S(ERROR) << "Wrong header in geometry file " << geoFile;
			return 0;
        }
	}
	else
    {
        LOG_S(ERROR) << "Failed opening geometry file " << geoFile;
		return 0;
    }

	geo->computeBBox();
	geo->computeBinormalAndTangent();
	return geo;
}
