
#include "NavTest.h"
#include "GameApp.h"
#include "ParticleApp.h"
#include "TestSoundApp.h"
#include "TestGLApp.h"
#include "../loguru.hpp"
#include "../Version.h"



int main(int argc, char* argv[])
{
    loguru::init(argc, argv);
    loguru::add_file("shadow.log", loguru::Truncate, loguru::Verbosity_2);
    loguru::g_stderr_verbosity = 2;

    LOG_S(INFO) << "Starting ShadowEngine v" << VERSION;

//	NavTest app;
    GameApp app;
//    ParticleApp app;
//	TestSoundApp app;
//    TestGLApp app;

    app.run();

    return 1;
}
