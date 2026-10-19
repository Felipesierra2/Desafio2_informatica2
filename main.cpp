#include <iostream>
#include "MedidorRecursos.h"
#include "RedSocial.h"
#include "Usuario.h"
#include "Mensajes.h"
#include "UdeABook.h"
#include "Archivo.h"


using namespace std;

int main()
{
    RedSocial redSocial;

    if (!Archivo::cargarUsuarios(redSocial, "usuarios.txt"))
    {
        std::cout << "Advertencia: la carga tuvo errores.\n";
    }

    if (!Archivo::cargarAmistades(redSocial, "amistades.txt"))
    {
        std::cout << "Advertencia al cargar las amistades.\n";
    }

    if (!Archivo::cargarSolicitudesPendientes(
            redSocial, "solicitudes.txt"))
    {
        std::cout << "Advertencia al cargar las solicitudes.\n";
    }

    if (!Archivo::cargarAlbumes(redSocial, "albumes.txt"))
    {
        std::cout << "Advertencia al cargar los albumes.\n";
    }

    if (!Archivo::cargarFotos(redSocial, "fotos.txt"))
    {
        std::cout << "Advertencia al cargar las fotos.\n";
    }

    if (!Archivo::cargarMensajes(redSocial, "mensajes.txt"))
    {
        std::cout << "Advertencia al cargar los mensajes.\n";
    }

    if (redSocial.getCantidadUsuarios() == 0)
    {
        std::cout << "No hay usuarios disponibles para iniciar sesion.\n";
        return 1;
    }

    if (!Archivo::cargarLikes(redSocial, "likes.txt"))
    {
        std::cout << "Advertencia al cargar los likes.\n";
    }

    {
        UdeABook aplicacion(redSocial);
        aplicacion.ejecutar();
    }

    if (!Archivo::guardarUsuarios(redSocial, "usuarios.txt"))
    {
        std::cout << "No se pudieron guardar todos los datos.\n";
        return 1;
    }

    if (!Archivo::guardarAmistades(redSocial, "amistades.txt"))
    {
        std::cout << "No se pudieron guardar las amistades.\n";
    }

    if (!Archivo::guardarSolicitudesPendientes(
            redSocial, "solicitudes.txt"))
    {
        std::cout << "No se pudieron guardar las solicitudes.\n";
    }

    if (!Archivo::guardarMensajes(redSocial, "mensajes.txt"))
    {
        std::cout << "No se pudieron guardar los mensajes.\n";
    }

    if (!Archivo::guardarAlbumes(redSocial, "albumes.txt"))
    {
        std::cout << "No se pudieron guardar los albumes.\n";
    }

    if (!Archivo::guardarFotos(redSocial, "fotos.txt"))
    {
        std::cout << "No se pudieron guardar las fotos.\n";
    }

    if (!Archivo::guardarLikes(redSocial, "likes.txt"))
    {
        std::cout << "No se pudieron guardar los likes.\n";
    }

    return 0;
}

