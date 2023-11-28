#ifndef AURORA_LAUNCHER_H
#define AURORA_LAUNCHER_H

#include <memory>

class AuroraLauncherPrivate;

class AuroraLauncher {
public:
    AuroraLauncher(int argc, char **argv);
    ~AuroraLauncher();
private:
    std::unique_ptr<AuroraLauncherPrivate> d;
};
#endif