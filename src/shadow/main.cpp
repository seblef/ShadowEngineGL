
#include "NavTest.h"
#include "GameApp.h"
#include "ParticleApp.h"
#include "TestSoundApp.h"
#include "TestGLApp.h"
#include "../loguru.hpp"
#include "../Version.h"
#include <tclap/CmdLine.h>


#ifdef _DEBUG
const int defaultVerbosityLevel = 2;
#else
const int defaultVerbosityLevel = 0;
#endif


class Options
{
public:
    string dataFolder;
    int verbosityLevel;

    Options() : dataFolder("./"), verbosityLevel(defaultVerbosityLevel) {}
};


void parseCommandLine(int argc, char *argv[], Options& opt)
{
    TCLAP::CmdLine cmdLine("ShadowEngine", ' ', VERSION);
    TCLAP::ValueArg<string> dataFolder("d", "data", "Path to data folder, can be relative or absolute.", false, "./", "string");
    TCLAP::ValueArg<int> verbosity("v", "verbosity", "Verbosity level, can be from -3 to 9.", false, defaultVerbosityLevel, "int");

    cmdLine.add(dataFolder);
    cmdLine.add(verbosity);
    cmdLine.parse(argc, argv);

    opt.dataFolder = dataFolder.getValue();
    opt.verbosityLevel = verbosity.getValue();
}



int main(int argc, char* argv[])
{
    Options opt;
    parseCommandLine(argc, argv, opt);

    loguru::init(argc, argv);
    loguru::add_file("shadow.log", loguru::Truncate, opt.verbosityLevel);
    loguru::g_stderr_verbosity = 2;

    LOG_S(INFO) << "Starting ShadowEngine v" << VERSION;
    LOG_S(INFO) << "Data folder: " << opt.dataFolder;

//	NavTest app;
    GameApp app;
//    ParticleApp app;
//	TestSoundApp app;
//    TestGLApp app;

    app.run(opt.dataFolder);

    return 1;
}
