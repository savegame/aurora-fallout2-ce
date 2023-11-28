#include "win32.h"

#include <stdlib.h>

#include <SDL.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "main.h"
#include "svga.h"
#include "window_manager.h"

#if __APPLE__ && TARGET_OS_IOS
#include "platform/ios/paths.h"
#endif

#ifdef AURORAOS
#include <QString>
#include <QScopedPointer>
#include <QStandardPaths>
#include <QDebug>
#include <aurora_launcher.h>
#endif

namespace fallout {

#ifdef _WIN32
// 0x51E444
bool gProgramIsActive = false;

// GNW95MUTEX
HANDLE _GNW95_mutex = NULL;

// 0x4DE700
int main(int argc, char* argv[])
{
    _GNW95_mutex = CreateMutexA(0, TRUE, "GNW95MUTEX");
    if (GetLastError() == ERROR_SUCCESS) {
        SDL_ShowCursor(SDL_DISABLE);

        gProgramIsActive = true;
        falloutMain(argc, argv);

        CloseHandle(_GNW95_mutex);
    }
    return 0;
}
#else
bool gProgramIsActive = false;

int main(int argc, char* argv[])
{
#if __APPLE__ && TARGET_OS_IOS
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    chdir(iOSGetDocumentsPath());
#endif

#if __APPLE__ && TARGET_OS_OSX
    char* basePath = SDL_GetBasePath();
    chdir(basePath);
    SDL_free(basePath);
#endif

#if __ANDROID__
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    chdir(SDL_AndroidGetExternalStoragePath());
#endif
#if AURORAOS
    AuroraLauncher launcher(argc, argv);

    if (argc > 1) {
        chdir(argv[1]);
    } else {
        QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        path += QStringLiteral("/Fallout2");
        chdir(path.toLocal8Bit().data());
        qDebug() << "Change dir to" << path;
    }
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "0");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
#endif

    SDL_ShowCursor(SDL_DISABLE);
    gProgramIsActive = true;
    return falloutMain(argc, argv);
}
#endif

} // namespace fallout

int main(int argc, char* argv[])
{
    return fallout::main(argc, argv);
}
