QAIVLIB=qaivlib

win32 {
  contains(QMAKE_HOST.arch, x86_64) {
    message("Building Windows 64-bit")
    OUTARCH=64
    DEFINES += _WINDOWS QT_LARGEFILE_SUPPORT QT_DLL QAIVLIB_SHARED QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2
  } else {
    message("Building Windows 32-bit")
    OUTARCH=32
  }
} else {
  OUTARCH=
}

CONFIG(debug, debug|release) {
    OUTFOLDER=Debug
    win32 {
        QAIVLIB = $$join(QAIVLIB,,,d)
        TARGET = $$join(TARGET,,,d)
    } else {
        QAIVLIB = $$join(QAIVLIB,,,_debug)
        TARGET = $$join(TARGET,,,_debug)
    }
} else {
    OUTFOLDER=Release
}

DESTDIR = $${PWD}/$${OUTFOLDER}$${OUTARCH}
DESIGNER_PLUGINS_DESTDIR = $${DESTDIR}/plugins/designer
PLUGINS_DESTDIR = $${DESTDIR}/plugins

INCLUDEPATH += GeneratedFiles \
               GeneratedFiles/$${OUTFOLDER}
MOC_DIR += GeneratedFiles/$${OUTFOLDER}
OBJECTS_DIR += $${OUTFOLDER}
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
