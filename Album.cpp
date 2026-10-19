#include "Album.h"
#include "MedidorRecursos.h"
#include "Usuario.h"

Album::Album(const std::string& nombre,
             const std::string& ruta,
             Privacidad privacidad)
    : nombre(nombre),
    ruta(ruta),
    privacidad(privacidad),
    fotos(nullptr),
    cantidadFotos(0),
    capacidadFotos(0)
{
}

Album::~Album()
{
    delete[] fotos;
}

const std::string& Album::getNombre() const
{
    return nombre;
}

const std::string& Album::getRuta() const
{
    return ruta;
}

Privacidad Album::getPrivacidad() const
{
    return privacidad;
}

bool Album::agregarFoto(Foto* foto)
{
    if (foto == nullptr)
    {
        return false;
    }

    if (cantidadFotos == capacidadFotos)
    {
        int nuevaCapacidad = (capacidadFotos == 0)
        ? 2
        : capacidadFotos * 2;

        Foto** nuevasFotos = new (std::nothrow) Foto*[nuevaCapacidad];

        if (nuevasFotos == nullptr)
        {
            return false;
        }
        MedidorRecursos::registrarMemoria(sizeof(Foto) * nuevaCapacidad);

        for (int i = 0; i < cantidadFotos; i++)
        {
            MedidorRecursos::getIteraciones();
            nuevasFotos[i] = fotos[i];
        }

        MedidorRecursos::liberarMemoria(sizeof(Foto) * nuevaCapacidad);

        delete[] fotos;
        fotos = nuevasFotos;
        capacidadFotos = nuevaCapacidad;
    }

    fotos[cantidadFotos] = foto;
    cantidadFotos++;

    return true;
}

int Album::getCantidadFotos() const
{
    return cantidadFotos;
}

Foto* Album::getFoto(int indice) const
{
    if (indice < 0 || indice >= cantidadFotos)
    {
        MedidorRecursos::getIteraciones();
        return nullptr;
    }

    return fotos[indice];
}

bool Album::puedeVer(const Usuario* propietario,
                     const Usuario* visitante) const
{
    if (propietario == nullptr || visitante == nullptr) {
        return false;
    }

    if (propietario == visitante) {
        MedidorRecursos::registrarIteracion();
        return true;
    }

    switch (privacidad) {
    case Privacidad::Publico:
        MedidorRecursos::registrarIteracion();
        return true;

    case Privacidad::Privado:
        MedidorRecursos::registrarIteracion();
        return false;

    case Privacidad::Amigos:
        MedidorRecursos::registrarIteracion();
        return propietario->esAmigo(visitante);
    }

    return false;
}
