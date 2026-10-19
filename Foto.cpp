#include "Foto.h"
#include "MedidorRecursos.h"
#include <iostream>



Foto::Foto(const std::string& fechaDeCaptura,
           unsigned long long tam,
           const std::string& ruta)
    :fechaDeCaptura(fechaDeCaptura),
    tam(tam),
    ruta(ruta),
    usuariosQueDieronLike(nullptr),
    cantidadUsuariosLike(0),
    capacidadUsuariosLike(0),
    cantidadLikes(0)
{
}

Foto::~Foto()
{
    delete[] usuariosQueDieronLike;
}

const std::string& Foto::getFechaCaptura() const
{
    return fechaDeCaptura;
}

unsigned long long Foto::getTamano() const
{
    return tam;
}

const std::string& Foto::getRuta() const
{
    return ruta;
}

int Foto::getCantidadLikes() const
{
    return cantidadLikes;
}

Usuario* Foto::getUsuarioLike(int indice) const
{
    if (indice < 0 || indice >= cantidadLikes)
    {
        return nullptr;
    }

    return usuariosQueDieronLike[indice];
}

bool Foto::darLike(Usuario* usuario)
{
    MedidorRecursos::registrarIteracion();

    if (usuario == nullptr)
        return false;

    for (int i = 0; i < cantidadUsuariosLike; i++)
    {
        MedidorRecursos::registrarIteracion();

        if (usuariosQueDieronLike[i] == usuario)
            return false;
    }

    if (cantidadUsuariosLike == capacidadUsuariosLike)
    {
        int nuevaCapacidad =
            (capacidadUsuariosLike == 0) ? 2 : capacidadUsuariosLike * 2;

        Usuario** nuevosUsuarios = new Usuario*[nuevaCapacidad];

        for (int i = 0; i < cantidadUsuariosLike; i++)
        {
            MedidorRecursos::registrarIteracion();
            nuevosUsuarios[i] = usuariosQueDieronLike[i];
        }

        delete[] usuariosQueDieronLike;
        usuariosQueDieronLike = nuevosUsuarios;
        capacidadUsuariosLike = nuevaCapacidad;

        MedidorRecursos::registrarMemoria(
            sizeof(Usuario*) * nuevaCapacidad);
    }

    usuariosQueDieronLike[cantidadUsuariosLike] = usuario;
    cantidadUsuariosLike++;
    cantidadLikes++;

    return true;
}

bool Foto::quitarLike(Usuario* usuario)
{
    MedidorRecursos::registrarIteracion();

    if (usuario == nullptr)
        return false;

    for (int i = 0; i < cantidadUsuariosLike; i++)
    {
        MedidorRecursos::registrarIteracion();

        if (usuariosQueDieronLike[i] == usuario)
        {
            for (int j = i; j < cantidadUsuariosLike - 1; j++)
            {
                MedidorRecursos::registrarIteracion();
                usuariosQueDieronLike[j] = usuariosQueDieronLike[j + 1];
            }

            cantidadUsuariosLike--;
            cantidadLikes--;

            return true;
        }
    }

    return false;
}

void Foto::mostrar() const
{
    std::cout << "Ruta de la foto: " << ruta << '\n';
}

