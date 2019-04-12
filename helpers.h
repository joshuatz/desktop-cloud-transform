#ifndef HELPERS_H
#define HELPERS_H

#include <QObject>
#include <QtNetwork>

class Helpers
{
public:
    Helpers();
    static void checkInternetConnection(bool *res);
};

#endif // HELPERS_H
