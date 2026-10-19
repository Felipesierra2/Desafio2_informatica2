#ifndef UDEABOOK_H
#define UDEABOOK_H

#include "RedSocial.h"
class UdeABook
{
private:
    RedSocial& redSocial;

    void menuUsuario(Usuario* usuario);

    bool actualizacionFotosRealizada;

public:
    UdeABook(RedSocial& redSocial);
    void ejecutar();
};

#endif // UDEABOOK_H
