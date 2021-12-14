
#include "NavTest.h"
#include "EditorApp.h"
#include "GameApp.h"
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
    std::string dataFolder;
    int verbosityLevel;
    bool editor;

    Options() :
        dataFolder("./"),
        verbosityLevel(defaultVerbosityLevel),
        editor(false)    
    {}
};


void parseCommandLine(int argc, char *argv[], Options& opt)
{
    TCLAP::CmdLine cmdLine("ShadowEngine", ' ', VERSION);
    TCLAP::ValueArg<std::string> dataFolder("d", "data", "Path to data folder, can be relative or absolute.", false, "./", "string");
    TCLAP::ValueArg<int> verbosity("v", "verbosity", "Verbosity level, can be from -3 to 9.", false, defaultVerbosityLevel, "int");
    TCLAP::SwitchArg editor("e", "editor", "Run engine in editor mode");

    cmdLine.add(dataFolder);
    cmdLine.add(verbosity);
    cmdLine.add(editor);
    cmdLine.parse(argc, argv);

    opt.dataFolder = dataFolder.getValue();
    opt.verbosityLevel = verbosity.getValue();
    opt.editor = editor.getValue();
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
    if(opt.editor)
    {
        EditorApp app;
        app.run(opt.dataFolder);
    }
    else
    {
        GameApp app;
        // TestSoundApp app;
    //    TestGLApp app;
        app.run(opt.dataFolder);
    }

    return 1;
}
