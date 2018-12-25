# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = Nulstar
VERSION = 0.1.0
APP_DOMAIN=nulstar.com
APP_ROLE=Role_ServiceController
APP_ROLE_VERSION=0.1
APP_NAME=Nulstar Main Controller
APP_VERSION=$$VERSION
APP_VERSION_NAME=OneEye Baby Ninja Egg

DESTDIR=$$MODULES_OUTDIR/$$TARGET/$$VERSION

# Source Files

QMAKE_SUBSTITUTES += $$TOP_SRCDIR/AppVersion.h.in

HEADERS += NMainController.h \
           NPluginManger.h \
           NModulesManager.h \
           NModuleInfo.h

SOURCES += NMainController.cpp \
           NulstarMain.cpp \
           NPluginManger.cpp \
           NModulesManager.cpp \
           NModuleInfo.cpp

OTHER_FILES += Nulstar.cfg \
            Constants.ncf

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/NNetwork $$LIBRARIES_PATH/NCore
LIBS += -lNNetwork -lNCore

# Install Settings

QMAKE_POST_LINK += $$quote($$QMAKE_COPY \"$$PWD/Nulstar.cfg\" \"$$DESTDIR\" $$escape_expand(\n\t))
QMAKE_POST_LINK += $$quote($$QMAKE_COPY \"$$PWD/Constants.ncf\" \"$$DESTDIR\")

# Clean Settings
QMAKE_CLEAN += -r $$DESTDIR

# Deploy Qt libs if it is compiled in release mode and files changed

CONFIG(release,debug|release) {
  unix {
    QT_LIBRARIES = $$[QT_INSTALL_LIBS]/libQt?WebSockets.so.? \
                   $$[QT_INSTALL_LIBS]/libQt?Network.so.? \
                   $$[QT_INSTALL_LIBS]/libQt?Core.so.? \
                   $$[QT_INSTALL_LIBS]/libicui18n.so.?? \
                   $$[QT_INSTALL_LIBS]/libicuuc.so.?? \
                   $$[QT_INSTALL_LIBS]/libicudata.so.??

 # win32:QT_LIBRARIES = $$[QT_INSTALL_LIBS]/Qt5WebSockets.dll \
 #                      $$[QT_INSTALL_LIBS]/Qt5Network.dll \
 #                      $$[QT_INSTALL_LIBS]/Qt5Core.dll
    deploy_files.name = copy large files
    deploy_files.input = QT_LIBRARIES
    deploy_files.output = $${QT_LIBRARIES_OUTDIR}/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
    deploy_files.commands = ${COPY_FILE} ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    deploy_files.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += deploy_files
  }
  win32 {
    #QMAKE_POST_LINK = windeployqt $${QT_LIBRARIES_OUTDIR}/$${TARGET}.exe --no-translations
    QMAKE_POST_LINK = windeployqt -core -network -websockets --no-compiler-runtime --dir $${QT_LIBRARIES_OUTDIR} $${DESTDIR}/$${TARGET}.exe --no-translations
  }

}

