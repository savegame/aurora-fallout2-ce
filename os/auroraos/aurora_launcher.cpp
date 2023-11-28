#include "aurora_launcher.h"
#include <auroraapp.h>
#include <QtQuick>

class AuroraLauncherPrivate {
public:
    AuroraLauncherPrivate(AuroraLauncher *parent, int argc, char** argv) 
        : app(Aurora::Application::application(argc, argv))
    {
    }

    ~AuroraLauncherPrivate() {}

    QScopedPointer<QGuiApplication> app;
};

AuroraLauncher::AuroraLauncher(int argc, char** argv) 
    : d(new AuroraLauncherPrivate(this, argc, argv))
{
    qDebug() << "AURORA_ORGNAME" << AURORA_ORGNAME;
    qDebug() << "AURORA_APPNAME" << AURORA_APPNAME;
    d->app->setOrganizationName(QStringLiteral(AURORA_ORGNAME));
    d->app->setApplicationName(QStringLiteral(AURORA_APPNAME));

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->setSource(Aurora::Application::pathTo(QStringLiteral("%0.%1/qml/main.qml").arg(AURORA_ORGNAME).arg(AURORA_APPNAME)));
    view->show();

    d->app->exec();
}

AuroraLauncher::~AuroraLauncher() 
{
    d.reset();
}