#include "mailing.h"
#include "smtpclient.h"
#include "../smtp/mimemessage.h"
#include "../smtp/mimetext.h"

bool Mailing::envoyerMail(const QString &destinataire, const QString &sujet, const QString &message) {
    SmtpClient smtp("smtp.gmail.com", 587, SmtpClient::TlsConnection); // serveur Gmail
    smtp.setUser("tonemail@gmail.com");
    smtp.setPassword("tonMotDePasseApp"); // mot de passe d'application Gmail

    MimeMessage mail;
    mail.setSender(new EmailAddress("tonemail@gmail.com", "Gestion Magasins"));
    mail.addRecipient(new EmailAddress(destinataire));
    mail.setSubject(sujet);

    MimeText *text = new MimeText(message);
    mail.addPart(text);

    if (!smtp.connectToHost()) return false;
    if (!smtp.login()) return false;
    if (!smtp.sendMail(mail)) return false;

    smtp.quit();
    return true;
}
