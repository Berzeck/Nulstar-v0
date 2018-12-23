TEMPLATE = subdirs
SUBDIRS += Libraries \
           NulstarMain \
           ServiceManager \
           Connector

Connector.depends += Libraries
ServiceManager.depends += Libraries
NulstarMain.depends += ServiceManager Connector

OTHER_FILES += AppVersion.h.in \
               common.pri \
               .qmake.conf

