# Avoid SDK version warning
CONFIG += sdk_no_version_check

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Release with debug info
QMAKE_CFLAGS_RELEASE -= $$QMAKE_CFLAGS_RELEASE
QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_CXXFLAGS_RELEASE -= $$QMAKE_CXXFLAGS_RELEASE
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE -= $$QMAKE_LFLAGS_RELEASE
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

win32:CONFIG += skip_target_version_ext
!win32:CONFIG += unversioned_libname

# Qt 5.12 supports macOS: 10.12 10.13 10.14
# macos:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12

# Platform Settings
macos:CONFIG   -= app_bundle
win32:DEFINES  += WIN32

CONFIG(debug, debug|release) {
	TOP_OUTDIR=$$shadowed($$PWD)/debug
} else {
	TOP_OUTDIR=$$shadowed($$PWD)/release
        DEFINES += QT_NO_DEBUG_OUTPUT
}

DESTDIR=$$TOP_OUTDIR
LIBRARIES_OUTDIR=$$TOP_OUTDIR/Libraries
MAIN_MODULES_OUTDIR=$$TOP_OUTDIR/MainModules
LIBS += -L$$LIBRARIES_OUTDIR
