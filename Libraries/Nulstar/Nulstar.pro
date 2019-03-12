TEMPLATE = subdirs
SUBDIRS += NCore \
           NNetwork \
    NTools
NCore.depends += NNetwork