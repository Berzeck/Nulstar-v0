TEMPLATE = subdirs
SUBDIRS += Connector \
			NulstarMain \
			ServiceManager

NulstarMain.depends += ServiceManager Connector
