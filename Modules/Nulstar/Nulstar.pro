TEMPLATE = subdirs
SUBDIRS += Connector \
			NulstarMain \
			ServiceManager \
			Updater \
			Tester

NulstarMain.depends += ServiceManager Connector Updater Tester
