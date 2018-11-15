TEMPLATE = subdirs
SUBDIRS += NCore \
           NNetwork

Core.depends = NNetwork
