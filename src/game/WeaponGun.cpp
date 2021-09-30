
#include "WeaponGun.h"

WeaponGun::WeaponGun(ScriptFile& sf) : _trace(0)
{
	string t(sf.getToken());
    while (sf.good() && t != "end_weapon")
	{
        if (t == "trace")
			_trace = new TraceTemplate(sf);
		else
			parseToken(t, sf);

		t = sf.getToken();
	}

}

WeaponGun::~WeaponGun()
{
	if (_trace)
		delete _trace;
}
