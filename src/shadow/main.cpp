
#include "NavTest.h"
#include "GameApp.h"
#include "ParticleApp.h"
#include "TestSoundApp.h"
#include "TestGLApp.h"
#include "../loguru.hpp"
#include "../Version.h"
#include <unistd.h>


// TODO: Use proper command line options partser


void setDataFolder(const string& dataFolder)
{
    chdir(dataFolder.c_str());
}



int main(int argc, char* argv[])
{
    loguru::init(argc, argv);
    loguru::add_file("shadow.log", loguru::Truncate, loguru::Verbosity_2);
    loguru::g_stderr_verbosity = 2;

    LOG_S(INFO) << "Starting ShadowEngine v" << VERSION;

    for(int i=0; i<argc; ++i)
        LOG_S(INFO) << "Argument " << i << ": " << argv[i];

    for(int i=0; i<argc; ++i)
    {
        if(strcmp(argv[i], "-d") == 0 && i < argc-1)
            setDataFolder(string(argv[i+1]));
    }

//	NavTest app;
    GameApp app;
//    ParticleApp app;
//	TestSoundApp app;
//    TestGLApp app;

    app.run();

    return 1;
}
