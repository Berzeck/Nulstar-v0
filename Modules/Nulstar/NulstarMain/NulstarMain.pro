# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings
QT -= gui
QT += websockets
CONFIG += c++11 console

# Target Settings

TARGET = Nulstar
VERSION = 0.1.0
APP_DOMAIN=Nulstar
APP_ROLE=Role_MainController
APP_ROLE_VERSION=0.1
APP_NAME=$$TARGET
APP_VERSION=$$VERSION
APP_VERSION_NAME=OneEye Baby Ninja Egg

DESTDIR=$$MODULES_OUTDIR/$$APP_DOMAIN/$$TARGET/$$VERSION

# Source Files

QMAKE_SUBSTITUTES += AppVersion.h.in

HEADERS += NMainController.h \
           NPluginManger.h \
           NModuleManager.h \
           NModuleInfo.h \
    NModuleParameter.h

SOURCES += NMainController.cpp \
           NulstarMain.cpp \
           NPluginManger.cpp \
           NModuleManager.cpp \
           NModuleInfo.cpp \
    NModuleParameter.cpp

OTHER_FILES += Module.ncf

# Dependencies

INCLUDEPATH += $$LIBRARIES_PATH/Nulstar/NNetwork \
            $$LIBRARIES_PATH/Nulstar/NCore
LIBS += -lNNetwork -lNCore

# Install Settings

#COPYFILE1 = \"$$PWD/Constants.ncf\" \"$$DESTDIR\"
COPYFILE2 = \"$$PWD/Module.ncf\" \"$$DESTDIR\"
win32: {
#  COPYFILE1 ~= s,/,\\,g
  COPYFILE2 ~= s,/,\\,g
}
#QMAKE_POST_LINK += $$QMAKE_COPY $$COPYFILE1 & $$QMAKE_COPY $$COPYFILE2
QMAKE_POST_LINK += $$QMAKE_COPY $$COPYFILE2

# Clean Settings
QMAKE_CLEAN += -r $$DESTDIR

# Deploy Qt libs if it is compiled in release mode and files changed

CONFIG(release,debug|release) {
  unix:!mac {
    QT_LIBRARIES = $$[QT_INSTALL_LIBS]/libQt?WebSockets.so.? \
                   $$[QT_INSTALL_LIBS]/libQt?Network.so.? \
                   $$[QT_INSTALL_LIBS]/libQt?Core.so.? \
                   $$[QT_INSTALL_LIBS]/libicui18n.so.?? \
                   $$[QT_INSTALL_LIBS]/libicuuc.so.?? \
                   $$[QT_INSTALL_LIBS]/libicudata.so.??

    deploy_files.name = copy large files
    deploy_files.input = QT_LIBRARIES
    deploy_files.output = $${QT_LIBRARIES_OUTDIR}/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
    deploy_files.commands = ${COPY_FILE} ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    deploy_files.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += deploy_files
  }
  win32 {
    QMAKE_POST_LINK = $$QMAKE_POST_LINK & windeployqt -core -network -websockets --no-compiler-runtime --dir $${QT_LIBRARIES_OUTDIR} $${DESTDIR}/$${TARGET}.exe --no-translations
  }
}

