#include "aurora_launcher.h"
#include <auroraapp.h>
#include <QtQuick>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

#include <SDL.h>
#include <SDL_image.h>

class AuroraLauncherPrivate {
public:
    AuroraLauncherPrivate(AuroraLauncher *parent, int argc, char** argv) 
        : app(Aurora::Application::application(argc, argv))
    {
    }

    ~AuroraLauncherPrivate() {
        app->deleteLater();
    }

    void saveSettings()
    {
        QString config_path = QStringLiteral("%1/%2/%3/config.json")
                .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
                .arg(AURORA_ORGNAME)
                .arg(AURORA_APPNAME);
        qDebug() << "SAVE: Config path: " << config_path;
    }

    void loadSettings()
    {
        QString config_path = QStringLiteral("%1/%2/%3/config.json")
                .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
                .arg(AURORA_ORGNAME)
                .arg(AURORA_APPNAME);
        qDebug() << "LOAD: Config path: " << config_path;
    }

    QGuiApplication *app;
    bool m_close = false;
};

struct _Button {
    SDL_Rect dst_rect;
};
typedef struct _Button Button;

class AuroraDataPrivate
{
public:
    AuroraDataPrivate() {
        m_resourcesPath = QStringLiteral("%1/Fallout2").arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }

    ~AuroraDataPrivate() {}

    QString m_resourcesPath;
    AuroraData::ResolutionMode m_resolution = AuroraData::Default;

    SDL_Texture *m_texture = nullptr;
    SDL_DisplayOrientation m_orientation = SDL_ORIENTATION_UNKNOWN;
    double m_rotate_angle = 90;
    bool m_hideUi = false;
    Button m_save;
    Button m_load;
};

AuroraLauncher::AuroraLauncher(int argc, char** argv) 
    : d(new AuroraLauncherPrivate(this, argc, argv))
{
    // ===
    qDebug() << "AURORA_ORGNAME" << AURORA_ORGNAME;
    qDebug() << "AURORA_APPNAME" << AURORA_APPNAME;
    d->app->setOrganizationName(QStringLiteral(AURORA_ORGNAME));
    d->app->setApplicationName(QStringLiteral(AURORA_APPNAME));
    // read from settings
    d->loadSettings();
    // =====
    qApp->setQuitOnLastWindowClosed(false);
    QSharedPointer<QQuickView> view(new QQuickView());

    qmlRegisterSingletonType<AuroraData>("ru.sashikknox", 1, 0, "AuroraData", AuroraData_singletontype_provider );

    view->setSource(QStringLiteral("/usr/share/%0.%1/qml/main.qml").arg(AURORA_ORGNAME).arg(AURORA_APPNAME));
    view->show();

    QObject::connect(AuroraData::getInstance(), &AuroraData::dataChanged, [this](){
        d->saveSettings();
    });

    QObject::connect(AuroraData::getInstance(), &AuroraData::quit, [this, view](){
        d->m_close = true;
    });
}

AuroraLauncher::~AuroraLauncher() 
{
    d.reset();
}

int AuroraLauncher::exec()
{
    while(!d->m_close) {
        d->app->processEvents(QEventLoop::AllEvents);
    }
    d->app->exit();
    qDebug() << "Launcher closed";
}

AuroraData::AuroraData(QObject *parent)
    : QObject(parent)
    , d(new AuroraDataPrivate())
{

}

AuroraData::~AuroraData()
{
    SDL_free(d->m_texture);
    delete d;
}

AuroraData* AuroraData::getInstance()
{
    return auroraDataGlobalInstance;
}

QString AuroraData::resourcesPath() const
{
    return d->m_resourcesPath;
}

void AuroraData::setResourcesPath(const QString &path)
{
    if (d->m_resourcesPath == path) 
        return;
    d->m_resourcesPath = path;
    emit resourcesPathChanged();
    emit dataChanged();
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
    emit resolutionChanged();
    emit dataChanged();
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
    Q_ASSERT_X(renderer, "Empty SDL_Renderer pointer", "");
    
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

#include "moc_aurora_launcher.cpp"
