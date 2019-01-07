TEMPLATE = subdirs
SUBDIRS += Libraries \
           Modules

Modules.depends = Libraries

OTHER_FILES += common.pri \
               .qmake.conf

QMAKE_SUBSTITUTES += $$TOP_SRCDIR/AppVersion.h.in

