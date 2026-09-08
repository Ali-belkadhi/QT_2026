#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QTimer>
#include <QSslError>
#include <QList>

class SmtpClient : public QObject
{
    Q_OBJECT

public:
    explicit SmtpClient(QObject *parent = nullptr);
    ~SmtpClient() override;

    // Envoi d'un email en HTML via SMTPS (SSL/TLS direct - port 465)
    void sendMail(const QString &host,
                  int port,
                  const QString &user,
                  const QString &pass,
                  const QString &from,
                  const QString &to,
                  const QString &subject,
                  const QString &htmlBody,
                  int timeoutMs = 30000);

signals:
    void emailSent();
    void smtpError(const QString &errorMessage);

private slots:
    void onReadyRead();
    void onConnected();
    void onEncrypted();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSslErrors(const QList<QSslError> &errors);
    void onTimeout();

private:
    enum SmtpState
    {
        Init,
        HandShake,
        AuthLogin,
        AuthUser,
        AuthPass,
        MailFrom,
        RcptTo,
        Data,
        Body,
        Quit,
        Close
    };

    void sendLine(const QString &line);
    void finishWithError(const QString &msg);
    void finishWithSuccess();
    QString buildMimeMessage() const;

    QSslSocket *socket = nullptr;
    QTimer *timer = nullptr;

    QString host;
    int port = 465;
    QString user;
    QString pass;
    QString from;
    QString to;
    QString subject;
    QString htmlBody;

    SmtpState state = Init;
    QString responseBuffer;
};

#endif // SMTPCLIENT_H
