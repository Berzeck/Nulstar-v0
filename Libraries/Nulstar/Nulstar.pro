TEMPLATE = subdirs
SUBDIRS += NCore \
           NTools \
           NNetwork \
           HttpServer

NCore.depends += NNetwork
NNetwork.depends += NTools
