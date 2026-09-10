#include "smtpclient.h"

#include <QDebug>
#include <QByteArray>
#include <QDateTime>

SmtpClient::SmtpClient(QObject *parent)
    : QObject(parent),
      socket(new QSslSocket(this)),
      timer(new QTimer(this))
{
    timer->setSingleShot(true);

    connect(socket, &QSslSocket::connected, this, &SmtpClient::onConnected);
    connect(socket, &QSslSocket::encrypted, this, &SmtpClient::onEncrypted);
    connect(socket, &QSslSocket::readyRead, this, &SmtpClient::onReadyRead);
    connect(socket, &QAbstractSocket::errorOccurred, this, &SmtpClient::onSocketError);
    connect(socket, &QSslSocket::sslErrors, this, &SmtpClient::onSslErrors);
    connect(timer, &QTimer::timeout, this, &SmtpClient::onTimeout);
}

SmtpClient::~SmtpClient()
{
    if (socket && socket->isOpen()) {
        socket->close();
    }
}

void SmtpClient::sendMail(const QString &host,
                          int port,
                          const QString &user,
                          const QString &pass,
                          const QString &from,
                          const QString &to,
                          const QString &subject,
                          const QString &htmlBody,
                          int timeoutMs)
{
    this->host = host;
    this->port = port;
    this->user = user;
    this->pass = pass;
    this->from = from;
    this->to = to;
    this->subject = subject;
    this->htmlBody = htmlBody;

    this->state = Init;
    this->responseBuffer.clear();

    if (socket->isOpen()) {
        socket->close();
    }

    timer->start(timeoutMs);

    qDebug() << "[SmtpClient] Connexion SSL à" << host << ":" << port << "...";
    socket->connectToHostEncrypted(host, static_cast<quint16>(port));
}

void SmtpClient::onConnected()
{
    qDebug() << "[SmtpClient] Socket connecté (en attente du handshake SSL/TLS).";
}

void SmtpClient::onEncrypted()
{
    qDebug() << "[SmtpClient] Connexion SSL établie avec succès.";
}

void SmtpClient::sendLine(const QString &line)
{
    if (socket && socket->isOpen()) {
        QByteArray data = line.toUtf8() + "\r\n";
        socket->write(data);
        socket->flush();
    }
}

void SmtpClient::onReadyRead()
{
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << "[SmtpClient <<]" << line;

        // Si la ligne est une continuation SMTP (ex: "250-SIZE"), on attend la dernière ligne (ex: "250 OK")
        if (line.length() >= 4 && line.at(3) == '-') {
            continue;
        }

        int code = line.left(3).toInt();

        switch (state) {
        case Init:
            // Attente du message d'accueil 220
            if (code == 220) {
                state = HandShake;
                qDebug() << "[SmtpClient >>] EHLO localhost";
                sendLine("EHLO localhost");
            } else {
                finishWithError(QString("Échec bannière d'accueil SMTP (code %1)").arg(code));
            }
            break;

        case HandShake:
            // Réponse EHLO 250
            if (code == 250) {
                state = AuthLogin;
                qDebug() << "[SmtpClient >>] AUTH LOGIN";
                sendLine("AUTH LOGIN");
            } else {
                finishWithError(QString("Échec EHLO (code %1)").arg(code));
            }
            break;

        case AuthLogin:
            // Attente du prompt Username (334)
            if (code == 334) {
                state = AuthUser;
                qDebug() << "[SmtpClient >>] <Username Base64>";
                sendLine(QString::fromUtf8(user.toUtf8().toBase64()));
            } else {
                finishWithError(QString("Échec demande AUTH LOGIN (code %1)").arg(code));
            }
            break;

        case AuthUser:
            // Attente du prompt Password (334)
            if (code == 334) {
                state = AuthPass;
                qDebug() << "[SmtpClient >>] <Password Base64>";
                sendLine(QString::fromUtf8(pass.toUtf8().toBase64()));
            } else {
                finishWithError(QString("Échec transmission utilisateur (code %1)").arg(code));
            }
            break;

        case AuthPass:
            // Authentification réussie (235)
            if (code == 235) {
                state = MailFrom;
                qDebug() << "[SmtpClient >>] MAIL FROM:<" << from << ">";
                sendLine(QString("MAIL FROM:<%1>").arg(from));
            } else {
                finishWithError(QString("Échec authentification Gmail (code %1 : %2). Vérifiez vos identifiants ou mot de passe d'application.")
                                    .arg(code).arg(line));
            }
            break;

        case MailFrom:
            // MAIL FROM accepté (250)
            if (code == 250) {
                state = RcptTo;
                qDebug() << "[SmtpClient >>] RCPT TO:<" << to << ">";
                sendLine(QString("RCPT TO:<%1>").arg(to));
            } else {
                finishWithError(QString("Adresse expéditeur refusée par le serveur SMTP (code %1 : %2)")
                                    .arg(code).arg(line));
            }
            break;

        case RcptTo:
            // Destinataire accepté (250)
            if (code == 250) {
                state = Data;
                qDebug() << "[SmtpClient >>] DATA";
                sendLine("DATA");
            } else if (code == 550 || code == 551 || code == 553 || code == 501 || code == 554) {
                finishWithError(QString("Adresse destinataire refusée par le serveur SMTP (code %1 : %2)")
                                    .arg(code).arg(line));
            } else {
                finishWithError(QString("Échec lors de la commande RCPT TO (code %1 : %2)")
                                    .arg(code).arg(line));
            }
            break;

        case Data:
            // Prêt à recevoir le contenu (354)
            if (code == 354) {
                state = Body;
                qDebug() << "[SmtpClient >>] Envoi du contenu MIME...";
                QString mime = buildMimeMessage();
                sendLine(mime);
                sendLine(".");
            } else {
                finishWithError(QString("Échec initialisation DATA (code %1 : %2)").arg(code).arg(line));
            }
            break;

        case Body:
            // Message transmis et accepté par le serveur (250)
            if (code == 250) {
                state = Quit;
                qDebug() << "[SmtpClient >>] QUIT";
                sendLine("QUIT");
            } else {
                finishWithError(QString("Échec acceptation corps email (code %1)").arg(code));
            }
            break;

        case Quit:
            // Fin de session (221) ou fermeture
            state = Close;
            finishWithSuccess();
            break;

        case Close:
            break;
        }
    }
}

QString SmtpClient::buildMimeMessage() const
{
    QString mime;

    // Encodage RFC 2047 du sujet pour supporter UTF-8 (accents é, è, etc.)
    QString encodedSubject = QString("=?UTF-8?B?%1?=").arg(QString::fromUtf8(subject.toUtf8().toBase64()));

    mime.append(QString("From: \"SmartSkills\" <%1>\r\n").arg(from));
    mime.append(QString("To: <%1>\r\n").arg(to));
    mime.append(QString("Subject: %1\r\n").arg(encodedSubject));
    mime.append(QString("Date: %1\r\n").arg(QDateTime::currentDateTime().toString(Qt::RFC2822Date)));
    mime.append("MIME-Version: 1.0\r\n");
    mime.append("Content-Type: text/html; charset=UTF-8\r\n");
    mime.append("Content-Transfer-Encoding: 8bit\r\n");
    mime.append("\r\n");
    mime.append(htmlBody);

    return mime;
}

void SmtpClient::finishWithError(const QString &msg)
{
    timer->stop();
    qWarning() << "[SmtpClient] ❌ Erreur SMTP :" << msg;
    emit smtpError(msg);
    if (socket->isOpen()) {
        socket->close();
    }
}

void SmtpClient::finishWithSuccess()
{
    timer->stop();
    qDebug() << "[SmtpClient] ✅ Email envoyé avec succès via Gmail SMTP !";
    emit emailSent();
    if (socket->isOpen()) {
        socket->close();
    }
}

void SmtpClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    if (state != Quit && state != Close) {
        finishWithError(QString("Erreur réseau / socket : %1").arg(socket->errorString()));
    }
}

void SmtpClient::onSslErrors(const QList<QSslError> &errors)
{
    QStringList errorStrings;
    for (const QSslError &err : errors) {
        errorStrings << err.errorString();
    }
    qWarning() << "[SmtpClient] Avertissement SSL :" << errorStrings.join("; ");
    // Ignorer les erreurs SSL de certificat auto-signé ou de chaîne locale si nécessaire :
    socket->ignoreSslErrors();
}

void SmtpClient::onTimeout()
{
    finishWithError("Délai d'attente (timeout) dépassé lors de la communication avec le serveur Gmail SMTP.");
}
