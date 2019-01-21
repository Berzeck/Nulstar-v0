#ifndef NMESSAGEUNSUBSCRIBE_H
#define NMESSAGEUNSUBSCRIBE_H

#include "NMessage.h"
#include "NNetwork.h"

namespace NulstarNS {
  class NETWORKSHARED_EXPORT NMessageUnsubscribe : public NMessage {
    Q_OBJECT

  public:
    explicit NMessageUnsubscribe(const QString& lConnectionName, const QString& lMessageID = QString(), const QStringList& lUnscribeMethods = QStringList(), QObject* rParent = nullptr);
    ~NMessageUnsubscribe() override {}

    QStringList fUnsubscribeMethods() const { return mUnsubscribeMethods; }

  protected:
    QVariantMap fMessageData() const override;
    QString fMessageType() const override { return QString("Unsubscribe"); }
    static bool fValidateMessageObject(const QJsonObject& lMessageObject);

  private:
    QStringList mUnsubscribeMethods;
  };
} // namespace NulstarNS

#endif // NMESSAGEUNSUBSCRIBE_H
