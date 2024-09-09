#include "aurora_launcher.h"
#include <string>

#include <SDL.h>
#include <SDL_image.h>

class AuroraLauncherPrivate {
public:
    AuroraLauncherPrivate(AuroraLauncher *parent, int argc, char** argv) 
    { 
        // Aurora::Application::application(argc, argv);
    }

    ~AuroraLauncherPrivate() {}

    void saveSettings()
    {
        // QString config_path = QStringLiteral("%1/%2/%3/config.json")
        //         .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
        //         .arg(AURORA_ORGNAME)
        //         .arg(AURORA_APPNAME);
        // qDebug() << "SAVE: Config path: " << config_path;
    }

    void loadSettings()
    {
        // QString config_path = QStringLiteral("%1/%2/%3/config.json")
        //         .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
        //         .arg(AURORA_ORGNAME)
        //         .arg(AURORA_APPNAME);
        // qDebug() << "LOAD: Config path: " << config_path;
    }

    bool m_close = false;
    // QGuiApplication *app;
    // QSharedPointer<QQuickView> m_view;
};

struct _Button {
    SDL_Rect dst_rect;
    SDL_FingerID fingerId;
};
typedef struct _Button Button;

class AuroraDataPrivate
{
public:
    AuroraDataPrivate() {
        //m_resourcesPath = QStringLiteral("%1/Fallout2").arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }

    ~AuroraDataPrivate() {
        // if (qApp)
            // qApp->deleteLater();
    }

    std::string m_resourcesPath;
    AuroraData::ResolutionMode m_resolution = AuroraData::Default;

    SDL_Texture *m_texture = nullptr;
    SDL_DisplayOrientation m_orientation = SDL_ORIENTATION_UNKNOWN;
    double m_rotate_angle = 90;
    bool m_hideUi = false;
    Button m_save;
    Button m_load;
};

// AuroraDataHelper::AuroraDataHelper(QObject *parent)
//     : QObject(parent)
// {}

// AuroraDataHelper::~AuroraDataHelper() 
// {}

// AuroraData *AuroraDataHelper::auroraData() 
// {
//     return AuroraData::getInstance();
// }

AuroraLauncher::AuroraLauncher(int argc, char** argv) 
    : d(new AuroraLauncherPrivate(this, argc, argv))
{
    // ===
    // qDebug() << "AURORA_ORGNAME" << AURORA_ORGNAME;
    // qDebug() << "AURORA_APPNAME" << AURORA_APPNAME;
    // d->app->setOrganizationName(QStringLiteral(AURORA_ORGNAME));
    // d->app->setApplicationName(QStringLiteral(AURORA_APPNAME));
    // // read from settings
    // d->loadSettings();
    // =====
    // qApp->setQuitOnLastWindowClosed(false);
    // d->m_view.reset(new QQuickView());

    // qmlRegisterSingletonType<AuroraData>("ru.sashikknox", 1, 0, "AuroraData", AuroraData_singletontype_provider );
    // qmlRegisterType<AuroraDataHelper>("ru.sashikknox", 1, 0, "AuroraDataHelper");   

    // d->m_view->setSource(QStringLiteral("/usr/share/%0.%1/qml/main.qml").arg(AURORA_ORGNAME).arg(AURORA_APPNAME));
    // d->m_view->show();

    // QObject::connect(AuroraData::getInstance(), &AuroraData::dataChanged, [this](){
        // d->saveSettings();
    // });

    // QObject::connect(AuroraData::getInstance(), &AuroraData::quit, [this](){
        // qDebug() << "Quit signal!";
        // d->m_clo/se = true;
        // qApp->quit();
    // });
}

AuroraLauncher::~AuroraLauncher() 
{
    // d.reset();
    delete d;
}

int AuroraLauncher::exec()
{
    // while(true) {
    //     d->app->processEvents(QEventLoop::AllEvents, 1);
    //     if(d->m_close) {
    //         qDebug() << "Quit from";
    //         break;
    //     }
    // }
    // d->app->exit();

    fprintf(stderr, "Launcher closed\n");
}

AuroraData::AuroraData()
    : d(new AuroraDataPrivate())
{

}

AuroraData::~AuroraData()
{
    SDL_free(d->m_texture);
    delete d;
}

AuroraData* AuroraData::getInstance()
{
    static AuroraData* static_auroraData = new AuroraData();
    return static_auroraData;
}

void AuroraData::qProcessEvents() 
{
    // if (d->m_view->visible()) {
    //     d->m_view->hide();
    // }
    // qApp->processEvents(QEventLoop::AllEvents, 1);
}

std::string AuroraData::resourcesPath() const
{
    // return d->m_resourcesPath;
    std::string path;
    path += std::string(getenv("HOME"));
    path += std::string("/Documents/Fallout2");
    return path;
}

void AuroraData::setResourcesPath(const std::string &path)
{
    if (d->m_resourcesPath == path) 
        return;
    d->m_resourcesPath = path;
    // emit resourcesPathChanged();
    // emit dataChanged();
}

AuroraData::ResolutionMode AuroraData::resolution() const
{
    return d->m_resolution;
}

void AuroraData::setResolution(ResolutionMode mode)
{
    if (mode == d->m_resolution)
        return;
    d->m_resolution;
    // emit resolutionChanged();
    // emit dataChanged();
}

void AuroraData::setOrientation(SDL_DisplayOrientation orientation)
{
    d->m_orientation = orientation;
    bool _nativeLandscape = false;

    if (_nativeLandscape) {
        // NativeLandscape
        if (d->m_orientation == SDL_ORIENTATION_PORTRAIT) 
        {
            d->m_rotate_angle = 0;
        }
        else if (d->m_orientation == SDL_ORIENTATION_PORTRAIT_FLIPPED)
        {
            d->m_rotate_angle = 180;
        }
    } else {

        if (d->m_orientation == SDL_ORIENTATION_LANDSCAPE) {
            d->m_rotate_angle = -90;

            d->m_save.dst_rect = {
                10, 52,
                32, 32
            };
            d->m_load.dst_rect = {
                10, 10,
                32, 32
            };
        } else if (d->m_orientation == SDL_ORIENTATION_LANDSCAPE_FLIPPED) {
            d->m_rotate_angle = 90;

            d->m_load.dst_rect = {
                480 - 42, 640 - 42,
                32, 32
            };
            d->m_save.dst_rect = {
                480 - 42, 640 - 84,
                32, 32
            };
        }
    }
}

SDL_DisplayOrientation AuroraData::orientation() const
{
    return d->m_orientation;
}

void AuroraData::initSDLPart(SDL_Renderer *renderer) 
{
    // Q_ASSERT_X(renderer, "Empty SDL_Renderer pointer", "");
    
    int r = IMG_Init(IMG_INIT_PNG);
    if (r == 0) {
        return;
    }
    d->m_texture = IMG_LoadTexture(renderer, "/usr/share/ru.sashikknox.fallout2ce/ui.png");
}

void AuroraData::hideSDLUi(bool hide) 
{
    d->m_hideUi = hide;
}

#define containsValue(value, _min, _max) (_min) <= (value) && (_max) >= (value)
#define containsPoint(rect, _x, _y) containsValue((_x), (rect).x, (rect).x + (rect).w) && containsValue((_y), (rect).y, (rect).y + (rect).h)

bool AuroraData::fingerDown(const SDL_TouchFingerEvent &finger)
{
    int x = finger.x * 480;
    int y = finger.y * 640;

    if (containsPoint((d->m_save.dst_rect), x, y)) {
        d->m_save.fingerId = finger.fingerId;
        fprintf(stderr, "SAVE button pressed\n");
        
        SDL_Event event;
        event.type = SDL_KEYDOWN;
        event.key.state = SDL_PRESSED;
        event.key.keysym.scancode = SDL_SCANCODE_F6;
        SDL_PushEvent(&event);

        return true;
    } else if (containsPoint((d->m_load.dst_rect), x, y)) {
        d->m_load.fingerId = finger.fingerId;
        fprintf(stderr, "LOAD button pressed\n");

        SDL_Event event;
        event.type = SDL_KEYDOWN;
        event.key.state = SDL_PRESSED;
        event.key.keysym.scancode = SDL_SCANCODE_F7;
        SDL_PushEvent(&event);

        return true;
    }
    return false;
}
bool AuroraData::fingerMotion(const SDL_TouchFingerEvent &finger)
{
    return finger.fingerId == d->m_save.fingerId || finger.fingerId == d->m_load.fingerId;
}

bool AuroraData::fingerUp(const SDL_TouchFingerEvent &finger)
{
    if (finger.fingerId == d->m_save.fingerId) {
        d->m_save.fingerId = -1;
        fprintf(stderr, "SAVE button up\n");

        SDL_Event event;
        event.type = SDL_KEYUP;
        event.key.state = SDL_RELEASED;
        event.key.keysym.scancode = SDL_SCANCODE_F6;
        SDL_PushEvent(&event);

        return true;
    } else if (finger.fingerId == d->m_load.fingerId) {
        d->m_load.fingerId = -1;
        fprintf(stderr, "LOAD button up\n");

        SDL_Event event;
        event.type = SDL_KEYUP;
        event.key.state = SDL_RELEASED;
        event.key.keysym.scancode = SDL_SCANCODE_F7;
        SDL_PushEvent(&event);

        return true;
    }
    return false;
}

void AuroraData::drawSDLUi(SDL_Renderer *renderer)
{
    if (!d->m_texture || d->m_hideUi)
        return;

    SDL_Rect src_rect = {
        0, 0,
        32, 32
    };
    // save
    SDL_RenderCopyEx(renderer, d->m_texture, &src_rect, &d->m_save.dst_rect, d->m_rotate_angle, NULL, SDL_FLIP_NONE);
    // load
    src_rect.x = 32;
    SDL_RenderCopyEx(renderer, d->m_texture, &src_rect, &d->m_load.dst_rect, d->m_rotate_angle, NULL, SDL_FLIP_NONE);
}
