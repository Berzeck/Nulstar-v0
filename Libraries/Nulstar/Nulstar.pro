TEMPLATE = subdirs
SUBDIRS += HttpServer \
           NCore \
           NNetwork \
    NTools
NCore.depends += NNetwork
