
#include "ScriptFile.h"
#include <assert.h>

using namespace Core;


ScriptFile::ScriptFile(const string& fileName) : _ifs(fileName,fstream::in), _fileName(fileName)
{
	_ifs.unsetf(fstream::skipws);
	assert(_ifs.good());
}

const string& ScriptFile::getToken()
{
	_token.clear();
	if(_ifs.good())
	{
        char c=_ifs.get();


        while((c==' ' || c=='	' || c=='\n') && _ifs.good())
            c=_ifs.get();
			
		if(!_ifs.good())
			return _token;

		if(c=='\"')
		{
			getText();
			return _token;
		}
			
		if(c=='{' || c=='}')
		{
			_token=c;
			return _token;
		}
		
        while(c!='\n' && c!=' ' && c!='	' && c!='{' && c!='}' && _ifs.good())
		{
			_token+=c;
            c=_ifs.get();
		}
		
		// Commentaire
		if(_token=="//")
		{
			do
			{
                c=_ifs.get();
            } while(c!='\n' && _ifs.good());
	
   			return getToken();
		}
	}

	return _token;
}

void ScriptFile::getText()
{
	_token.clear();
    char c=_ifs.get();
	while(_ifs.good() && c!='\"')
	{
		_token+=c;
        c=_ifs.get();
	}
}

void ScriptFile::parseColor(Color &c)
{
	for(int i=0;i<4;++i)
		c[i]=stof(getToken());
}

void ScriptFile::parseVector(Vector3& v)
{
	for(int i=0;i<3;++i)
		v[i]=stof(getToken());
}

void ScriptFile::parseVector(Vector2& v)
{
	v.x=stof(getToken());
	v.y=stof(getToken());
}
