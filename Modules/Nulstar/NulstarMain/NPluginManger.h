#ifndef NPLUGINMANGER_H
#define NPLUGINMANGER_H

#include <QObject>

class NPluginManger : public QObject {
    Q_OBJECT
public:
    explicit NPluginManger(QObject* parent = nullptr);

signals:

public slots:
    void scan();
};

#endif // NPLUGINMANGER_H
