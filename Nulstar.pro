TEMPLATE = subdirs
SUBDIRS += Connector \
           Libraries \
           NulstarMain \
           ServiceManager

Connector.depends += Libraries
ServiceManager.depends += Libraries
NulstarMain.depends += ServiceManager Connector

OTHER_FILES += common.pri \
            .qmake.conf

