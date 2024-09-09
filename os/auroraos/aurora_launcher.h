#ifndef AURORA_LAUNCHER_H
#define AURORA_LAUNCHER_H

// #include <QObject>
// #include <QScopedPointer>
#include <memory>
#include <string>
#include <SDL.h>

class AuroraLauncherPrivate;
class AuroraDataPrivate;
class AuroraData;
class QQmlEngine;
class QJSEngine;
class SDL_Renderer;

class AuroraLauncher
{
public:
    AuroraLauncher(int argc, char **argv);
    ~AuroraLauncher();

    int exec();
private:
    AuroraLauncherPrivate *d;
};

class AuroraData
{
public:
    enum ResolutionMode {
        Default,
        Large
    };
    // Q_ENUM(ResolutionMode);

    AuroraData();
    ~AuroraData();

    static AuroraData* getInstance();

    static void qProcessEvents();

    std::string resourcesPath() const;
    void setResourcesPath(const std::string &path);

    ResolutionMode resolution() const;
    void setResolution(ResolutionMode mode);

    void setOrientation(SDL_DisplayOrientation orientation);
    SDL_DisplayOrientation orientation() const;

    void initSDLPart(SDL_Renderer *renderer);
    void drawSDLUi(SDL_Renderer *renderer);
    void hideSDLUi(bool hide);

    bool fingerDown(const SDL_TouchFingerEvent &finger);
    bool fingerMotion(const SDL_TouchFingerEvent &finger);
    bool fingerUp(const SDL_TouchFingerEvent &finger);
// signals:
//     void quit();
//     void dataChanged();
//     void resourcesPathChanged();
//     void resolutionChanged();
private:
    AuroraDataPrivate *d;
};

// class AuroraDataHelper : public QObject class AuroraDataHelper : public QObject 
// {
//     Q_OBJECT
//     Q_PROPERTY(AuroraData* auroraData READ auroraData CONSTANT FINAL)
// public: 
//     AuroraDataHelper(QObject *parent = nullptr);
//     ~AuroraDataHelper();
    
//     AuroraData *auroraData();
// };
// {
//     Q_OBJECT
//     Q_PROPERTY(AuroraData* auroraData READ auroraData CONSTANT FINAL)
// public: 
//     AuroraDataHelper(QObject *parent = nullptr);
//     ~AuroraDataHelper();
    
//     AuroraData *auroraData();
// };

// Q_GLOBAL_STATIC(AuroraData, auroraDataGlobalInstance);

// // Second, define the singleton type provider function (callback).
// static QObject *AuroraData_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
// {
//     Q_UNUSED(engine)
//     Q_UNUSED(scriptEngine)
//     return AuroraData::getInstance();
// }

#endif
