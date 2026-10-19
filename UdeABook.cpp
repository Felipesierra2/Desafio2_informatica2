#include "UdeABook.h"
#include "Usuario.h"
#include <iostream>
#include <string>
#include <limits>

UdeABook::UdeABook(RedSocial& redSocial)
    : redSocial(redSocial),
    actualizacionFotosRealizada(false)
{
}

void UdeABook::ejecutar()
{
    std::string apodo;

    std::cout << "\n================================\n";
    std::cout << "     BIENVENIDO A UdeA@Book\n";
    std::cout << "================================\n";
    std::cout << "Ingresa tu apodo: ";

    if (!(std::cin >> apodo))
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada invalida.\n";
        return;
    }

    Usuario* usuario = redSocial.iniciarSesion(apodo);

    if (usuario == nullptr)
    {
        std::cout << "No fue posible iniciar sesion.\n";
        return;
    }

    redSocial.mostrarFotosAleatoriasAmigos(usuario);
    menuUsuario(usuario);
}

void UdeABook::menuUsuario(Usuario* usuario)
{
    int opcion = -1;

    while (opcion != 0)
    {
        std::cout << "\n========== UdeA@Book ==========\n";
        std::cout << "Sesion: " << usuario->getApodo() << "\n\n";

        std::cout << "--- Inicio ---\n";
        std::cout << "1. Actualizar fotos de amigos\n";

        std::cout << "\n--- Amistades ---\n";
        std::cout << "2. Gestionar solicitudes recibidas\n";
        std::cout << "3. Enviar solicitud de amistad\n";
        std::cout << "4. Ver sugerencias de amistad\n";

        std::cout << "\n--- Perfiles y fotos ---\n";
        std::cout << "5. Visitar perfil\n";
        std::cout << "6. Mostrar albumes visibles\n";
        std::cout << "7. Recorrer fotos de un album\n";
        std::cout << "8. Dar like a una foto\n";
        std::cout << "9. Quitar like a una foto\n";

        std::cout << "\n--- Mensajes ---\n";
        std::cout << "10. Enviar mensaje\n";
        std::cout << "11. Mostrar mensajes recibidos\n";
        std::cout << "12. Mostrar mensajes no leidos ordenados\n";
        std::cout << "13. Buscar mensajes por fechas\n";

        std::cout << "\n0. Cerrar sesion\n";
        std::cout << "Opcion: ";

        if (!(std::cin >> opcion))
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Debes ingresar una opcion numerica.\n";
            continue;
        }

        std::string apodo;
        std::string nombreAlbum;
        std::string contenido;
        std::string fechaMensaje;
        std::string fechaInicio;
        std::string fechaFin;
        int k;
        int indice;
        bool resultado;

        switch (opcion)
        {
        case 1:
            if (!actualizacionFotosRealizada)
            {
                redSocial.mostrarFotosAleatoriasAmigos(usuario);
                actualizacionFotosRealizada = true;
            }
            else
            {
                std::cout << "Ya actualizaste las fotos una vez en esta sesion.\n";
            }
            break;

        case 2:
            redSocial.gestionarSolicitudesPendientes(usuario);
            break;

        case 3:
            std::cout << "Apodo de la persona: ";
            std::cin >> apodo;

            resultado = redSocial.enviarSolicitud(
                usuario, redSocial.buscarUsuario(apodo));

            if (!resultado)
                std::cout << "No se pudo enviar la solicitud.\n";
            break;

        case 4:
            std::cout << "Cantidad de sugerencias (1-20): ";
            if (std::cin >> k)
                redSocial.mostrarSugerenciasAmistad(usuario, k);
            else
            {
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Cantidad invalida.\n";
            }
            break;

        case 5:
            std::cout << "Apodo del perfil que deseas visitar: ";
            std::cin >> apodo;
            redSocial.visitarPerfil(apodo, usuario->getApodo());
            break;

        case 6:
            std::cout << "Apodo del propietario: ";
            std::cin >> apodo;
            redSocial.mostrarAlbumesVisibles(
                apodo, usuario->getApodo());
            break;

        case 7:
            std::cout << "Apodo del propietario: ";
            std::cin >> apodo;
            std::cout << "Nombre del album: ";
            std::getline(std::cin >> std::ws, nombreAlbum);

            redSocial.recorrerAlbum(
                apodo, usuario->getApodo(), nombreAlbum);
            break;

        case 8:
        case 9:
            std::cout << "Apodo del propietario: ";
            std::cin >> apodo;
            std::cout << "Nombre del album: ";
            std::getline(std::cin >> std::ws, nombreAlbum);
            std::cout << "Indice de la foto: ";

            if (!(std::cin >> indice))
            {
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Indice invalido.\n";
                break;
            }

            if (opcion == 8)
            {
                resultado = redSocial.darLikeFoto(
                    apodo, usuario->getApodo(),
                    nombreAlbum, indice);
            }
            else
            {
                resultado = redSocial.quitarLikeFoto(
                    apodo, usuario->getApodo(),
                    nombreAlbum, indice);
            }

            if (!resultado)
                std::cout << "No se pudo completar la operacion.\n";
            break;

        case 10:
            std::cout << "Apodo del destinatario: ";
            std::cin >> apodo;

            std::cout << "Contenido del mensaje: ";
            std::getline(std::cin >> std::ws, contenido);

            std::cout << "Fecha (AAAA-MM-DD): ";
            std::cin >> fechaMensaje;

            resultado = redSocial.enviarMensaje(
                usuario,
                redSocial.buscarUsuario(apodo),
                contenido,
                fechaMensaje);

            if (!resultado)
                std::cout << "No se pudo enviar el mensaje.\n";
            break;

        case 11:
            redSocial.mostrarMensajesRecibidos(usuario);
            break;

        case 12:
            redSocial.mostrarMensajesNoLeidosOrdenados(usuario);
            break;

        case 13:
            std::cout << "Fecha inicial (AAAA-MM-DD): ";
            std::cin >> fechaInicio;
            std::cout << "Fecha final (AAAA-MM-DD): ";
            std::cin >> fechaFin;

            redSocial.buscarMensajesPorFecha(
                usuario, fechaInicio, fechaFin);
            break;

        case 0:
            std::cout << "Sesion cerrada.\n";
            break;

        default:
            std::cout << "Opcion inexistente.\n";
            break;
        }
    }
}


