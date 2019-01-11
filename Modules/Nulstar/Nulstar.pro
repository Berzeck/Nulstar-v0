TEMPLATE = subdirs
SUBDIRS += Connector \
			NulstarMain \
			ServiceManager \
			Updater

NulstarMain.depends += ServiceManager Connector Updater
