
#pragma once

#define GROUND_HEADER			(('S' << 24) | ('G' << 16) | ('R' << 8) | ('D'))

namespace File
{
	struct GroundFileHeaderEx
	{
		unsigned int			_header;
		int						_width;
		int						_height;
		int						_matCount;
		int						_gameMatCount;

	};

struct GroundFileHeader
{
	unsigned int			_header;
	unsigned int			_version;
	int						_width;
	int						_height;
	int						_matCount;
};

struct GroundFileMaterial
{
	char					_name[64];
};

};
