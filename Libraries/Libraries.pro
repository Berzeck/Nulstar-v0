TEMPLATE = subdirs
SUBDIRS += Core \
           Network

Core.depends = Network
