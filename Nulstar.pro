TEMPLATE = subdirs
SUBDIRS += Connector \
           NulstarMain \
           ServiceManager

NulstarMain.depends = ServiceManager
NulstarMain.depends = Connector

