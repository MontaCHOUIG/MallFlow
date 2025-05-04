#ifndef MAILING_H
#define MAILING_H

#include <QString>

class Mailing {
public:
    static bool envoyerMail(const QString &destinataire, const QString &sujet, const QString &message);
};

#endif // MAILING_H
