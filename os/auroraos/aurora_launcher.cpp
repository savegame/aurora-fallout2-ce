#include "aurora_launcher.h"
#include <auroraapp.h>
#include <QtQuick>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

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

class AuroraDataPrivate
{
public:
    AuroraDataPrivate() {
        m_resourcesPath = QStringLiteral("%1/Fallout2").arg(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }

    ~AuroraDataPrivate() {}

    QString m_resourcesPath;
    AuroraData::ResolutionMode m_resolution = AuroraData::Default;
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

#include "moc_aurora_launcher.cpp"
