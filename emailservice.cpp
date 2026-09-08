#include "emailservice.h"
#include "smtpclient.h"

#include <QDebug>
#include <QRegularExpression>

// =====================================================
// CONFIGURATION GMAIL SMTP
// =====================================================
static const QString GMAIL_SMTP_HOST = "smtp.gmail.com";
static const int     GMAIL_SMTP_PORT = 465; // Port SSL direct

// (Optionnel) Si vous préférez tester en renseignant directement vos identifiants ici :
static const QString SMTP_EMAIL_DIRECT    = "ali.belkadhi@esprit.tn"; 
static const QString SMTP_PASSWORD_DIRECT = "kkeg bqxq inrp xaim"; // <-- Collez ici votre mot de passe d'application Google (16 lettres)

EmailService::EmailService(QObject *parent)
    : QObject(parent),
      smtpClient(new SmtpClient(this))
{
    connect(smtpClient, &SmtpClient::emailSent, this, &EmailService::onSmtpSuccess);
    connect(smtpClient, &SmtpClient::smtpError, this, &EmailService::onSmtpError);
}

EmailService::~EmailService()
{
}

QString EmailService::getSmtpEmail() const
{
    // 1. Constante directe si renseignée
    if (!SMTP_EMAIL_DIRECT.trimmed().isEmpty()) {
        return SMTP_EMAIL_DIRECT.trimmed();
    }

    // 2. Lecture de la variable d'environnement SMTP_EMAIL avec qEnvironmentVariable
    QString email = qEnvironmentVariable("SMTP_EMAIL").trimmed();

    if (email.isEmpty()) {
        qWarning() << "[EmailService] ⚠️ Variable d'environnement SMTP_EMAIL non définie !";
    }

    return email;
}

QString EmailService::getSmtpPassword() const
{
    // 1. Constante directe si renseignée
    if (!SMTP_PASSWORD_DIRECT.trimmed().isEmpty()) {
        return SMTP_PASSWORD_DIRECT.trimmed();
    }

    // 2. Lecture de la variable d'environnement SMTP_PASSWORD avec qEnvironmentVariable
    QString password = qEnvironmentVariable("SMTP_PASSWORD").trimmed();

    if (password.isEmpty()) {
        qWarning() << "[EmailService] ⚠️ Variable d'environnement SMTP_PASSWORD non définie !";
    }

    return password;
}

QString EmailService::construireCorpsHtml(const QString &prenom,
                                         const QString &nom,
                                         const QString &specialite,
                                         const QDate   &dateEmbauche) const
{
    QString dateStr = dateEmbauche.isValid()
                          ? dateEmbauche.toString("dd/MM/yyyy")
                          : QDate::currentDate().toString("dd/MM/yyyy");

    return QString(
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <style>\n"
        "    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f6f9; margin: 0; padding: 20px; }\n"
        "    .container { max-width: 600px; margin: 0 auto; background: #ffffff; border-radius: 8px; overflow: hidden; box-shadow: 0 4px 12px rgba(0,0,0,0.1); border: 1px solid #e1e8ed; }\n"
        "    .header { background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%); color: #ffffff; padding: 25px; text-align: center; }\n"
        "    .header h1 { margin: 0; font-size: 22px; font-weight: 600; }\n"
        "    .content { padding: 30px; color: #333333; line-height: 1.6; }\n"
        "    .greeting { font-size: 18px; font-weight: bold; color: #1e3c72; margin-bottom: 15px; }\n"
        "    .info-box { background: #f8fafc; border-left: 4px solid #2a5298; padding: 15px 20px; margin: 20px 0; border-radius: 0 6px 6px 0; }\n"
        "    .info-box p { margin: 6px 0; font-size: 15px; }\n"
        "    .label { font-weight: bold; color: #4a5568; }\n"
        "    .footer { background: #edf2f7; color: #718096; padding: 15px; text-align: center; font-size: 13px; border-top: 1px solid #e2e8f0; }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <div class=\"container\">\n"
        "    <div class=\"header\">\n"
        "      <h1>🎓 Centre de Formation</h1>\n"
        "    </div>\n"
        "    <div class=\"content\">\n"
        "      <p class=\"greeting\">Bonjour %1,</p>\n"
        "      <p>Votre profil formateur a été créé avec succès dans notre système de gestion.</p>\n"
        "      <div class=\"info-box\">\n"
        "        <p><span class=\"label\">Nom :</span> %1 %2</p>\n"
        "        <p><span class=\"label\">Spécialité :</span> %3</p>\n"
        "        <p><span class=\"label\">Date d’embauche :</span> %4</p>\n"
        "      </div>\n"
        "      <p>Bienvenue parmi nous.</p>\n"
        "      <br>\n"
        "      <p>Cordialement,<br><strong>Administration du Centre de Formation</strong></p>\n"
        "    </div>\n"
        "    <div class=\"footer\">\n"
        "      <p>Ce message est généré automatiquement, merci de ne pas y répondre directement.</p>\n"
        "    </div>\n"
        "  </div>\n"
        "</body>\n"
        "</html>"
    ).arg(prenom.toHtmlEscaped(),
         nom.toHtmlEscaped(),
         specialite.toHtmlEscaped(),
         dateStr);
}

// =====================================================
// VALIDATION SYNTAXIQUE EMAIL
// =====================================================

bool EmailService::validerSyntaxeEmail(const QString &email)
{
    QString clean = email.trimmed();

    if (clean.isEmpty()) {
        return false;
    }

    // 1. Rejeter les doubles points consécutifs (ex: ali..test@gmail.com ou ali@gmail..com)
    if (clean.contains("..")) {
        return false;
    }

    // 2. Rejeter si commence ou finit par un point ou arobase
    if (clean.startsWith('.') || clean.endsWith('.') ||
        clean.startsWith('@') || clean.endsWith('@')) {
        return false;
    }

    // 3. Regex stricte RFC : local-part @ domain . TLD (au moins 2 lettres)
    static const QRegularExpression regexStrict(
        "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"
    );

    return regexStrict.match(clean).hasMatch();
}


void EmailService::envoyerConfirmationFormateur(const QString &prenom,
                                                const QString &nom,
                                                const QString &email,
                                                const QString &specialite,
                                                const QDate   &dateEmbauche)
{
    QString destEmail = email.trimmed();

    // 1. Validation syntaxique stricte de l'email destinataire
    if (!validerSyntaxeEmail(destEmail)) {
        QString msg = QString("Format de l'adresse email invalide : '%1'").arg(destEmail);
        qWarning() << "[EmailService] ❌" << msg;
        emit erreurEmail(msg);
        return;
    }

    // NOTE TECHNIQUE SUR LA VALIDATION EMAIL :
    // Une adresse comme 'VEEE@gmail.com' est syntaxiquement valide (elle respecte la regex).
    // Cependant, il est impossible de savoir si cette boîte Gmail existe réellement sans lui envoyer un mail :
    // - La commande 'SMTP VRFY' est désactivée par Gmail/Microsoft/Yahoo pour protéger la vie privée et bloquer le spam.
    // - Si l'adresse n'existe pas, Gmail accepte d'abord la transaction SMTP puis génère un message asynchrone (Bounce / NDR 550 5.1.1).

    // 2. Récupération des identifiants SMTP
    QString smtpUser = getSmtpEmail();
    QString smtpPass = getSmtpPassword();

    if (smtpUser.isEmpty() || smtpPass.isEmpty()) {
        QString msg = "Identifiants SMTP manquants (variables SMTP_EMAIL ou SMTP_PASSWORD non définies).";
        qWarning() << "[EmailService] ❌" << msg;
        emit erreurEmail(msg);
        return;
    }

    // 3. Préparation du sujet et du corps HTML
    QString subject = "Bienvenue au Centre de Formation";
    QString htmlBody = construireCorpsHtml(prenom, nom, specialite, dateEmbauche);

    qDebug() << "[EmailService] Préparation de l'envoi vers :" << destEmail;

    // 4. Déclenchement de l'envoi SMTP asynchrone
    smtpClient->sendMail(
        GMAIL_SMTP_HOST,
        GMAIL_SMTP_PORT,
        smtpUser,
        smtpPass,
        smtpUser,   // Expéditeur
        destEmail,  // Destinataire
        subject,
        htmlBody
    );
}

void EmailService::onSmtpSuccess()
{
    qDebug() << "[EmailService] ✅ Email de confirmation envoyé avec succès !";
    emit emailEnvoye();
}

void EmailService::onSmtpError(const QString &message)
{
    qWarning() << "[EmailService] ❌ Échec envoi email :" << message;
    emit erreurEmail(message);
}
