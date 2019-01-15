TEMPLATE = subdirs
SUBDIRS += Libraries \
           Modules

Modules.depends = Libraries

OTHER_FILES += common.pri \
               .qmake.conf
