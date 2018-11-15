TEMPLATE = subdirs
SUBDIRS += NCore \
           NNetwork

NCore.depends = NNetwork
