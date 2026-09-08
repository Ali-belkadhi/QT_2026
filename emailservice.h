#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include <QObject>
#include <QString>
#include <QDate>

class SmtpClient;

class EmailService : public QObject
{
    Q_OBJECT

public:
    explicit EmailService(QObject *parent = nullptr);
    ~EmailService() override;

    // Validation stricte du format d'email (syntaxe RFC)
    static bool validerSyntaxeEmail(const QString &email);

    // Envoi asynchrone de l'email de confirmation au formateur
    void envoyerConfirmationFormateur(const QString &prenom,
                                      const QString &nom,
                                      const QString &email,
                                      const QString &specialite,
                                      const QDate   &dateEmbauche);

signals:
    void emailEnvoye();
    void erreurEmail(const QString &message);

private slots:
    void onSmtpSuccess();
    void onSmtpError(const QString &message);

private:
    QString getSmtpEmail() const;
    QString getSmtpPassword() const;

    QString construireCorpsHtml(const QString &prenom,
                                const QString &nom,
                                const QString &specialite,
                                const QDate   &dateEmbauche) const;

    SmtpClient *smtpClient = nullptr;
};

#endif // EMAILSERVICE_H
