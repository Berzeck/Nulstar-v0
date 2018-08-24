#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include "NConnectionController.h"

int main(int argc, char *argv[])
{    
  QCoreApplication lApp(argc, argv);
  lApp.setApplicationName(QString(APP_NAME).replace("_"," "));
  lApp.setApplicationVersion(APP_VERSION);
  lApp.setOrganizationDomain(QStringLiteral("nulstar.com"));
  lApp.setOrganizationName(QStringLiteral("Nulstar"));

  QStringList lOptions;
  for(int i1 = 1; i1 < argc; ++i1)
    lOptions << QString::fromUtf8 (argv[i1]);
  QTextStream lOut(stdout);
  if((lOptions.isEmpty()) || ((lOptions.at(0) == "--help"))) {
    lOut << lApp.applicationName() << "\n"
            "-----------------\n"
            "  Parameters:\n\n"
            "  --version : Displays current version\n"
            "  --help : Shows this help\n"
            "  --debuglevel <Level> : Sets the level of internal information to be available\n"
            "    <Level> :  1 [Critical]\n"
            "    <Level> :  2 [Error]\n"
            "    <Level> :  3 [Warnings]\n"
            "    <Level> :  4 [Information]\n"
            "    <Level> :  5 [Everything]\n"
            "  --port [Port Number] : Port that will be opened for user connections\n"
            "  --adminport [Port Number] : Port that will be opened for system adminitration\n"
            "  --commport [Port Number] : Port that will be opened for internal communication with other components\n"
            "  --secmode <Mode> : Sets the security model that will be used for communication\n"
            "    <Mode> :  0 [No encryption]\n"
            "    <Mode> :  1 [Ssl]\n\n" << endl;
            return 0;
  }
  if(lOptions.contains(QStringLiteral("--version")))  {
    lOut <<  lApp.applicationName() << " - " << lApp.applicationVersion() << " [" << QString(APP_VERSION_NAME).replace("_"," ") << "]" <<  endl;
    return 0;
  }
  return lApp.exec();
}
