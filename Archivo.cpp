#include "Archivo.h"
#include "Usuario.h"
#include "RedSocial.h"
#include "MedidorRecursos.h"
#include "Album.h"
#include "Foto.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

bool Archivo::cargarUsuarios(
    RedSocial& redSocial,
    const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de usuarios.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    std::string linea;
    int cargados = 0;
    bool correcto = true;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        if (linea.empty())
            continue;

        std::string campos[7];
        std::string restante = linea;
        bool registroValido = true;

        for (int i = 0; i < 6; i++)
        {
            std::size_t separador = restante.find('|');

            if (separador == std::string::npos)
            {
                registroValido = false;
                break;
            }

            campos[i] = restante.substr(0, separador);
            restante = restante.substr(separador + 1);
        }

        if (!registroValido || restante.find('|') != std::string::npos)
        {
            std::cout << "Registro de usuario invalido: "
                      << linea << '\n';
            correcto = false;
            continue;
        }

        campos[6] = restante;

        Usuario* usuario = new Usuario(
            campos[0],
            campos[1],
            campos[2],
            campos[3],
            campos[4],
            campos[5],
            campos[6]
            );

        MedidorRecursos::registrarMemoria(sizeof(Usuario));

        if (redSocial.registrarUsuario(usuario))
        {
            cargados++;
        }
        else
        {
            delete usuario;
            MedidorRecursos::liberarMemoria(sizeof(Usuario));

            std::cout << "No se pudo registrar: "
                      << campos[0] << '\n';

            correcto = false;
        }
    }

    archivo.close();

    std::cout << "Usuarios cargados: " << cargados << '\n';

    MedidorRecursos::mostrarReporte();

    return correcto;
}

bool Archivo::guardarUsuarios(
    const RedSocial& redSocial,
    const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo para guardar usuarios.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    bool correcto = true;

    for (int i = 0; i < redSocial.getCantidadUsuarios(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* usuario = redSocial.getUsuario(i);

        if (usuario == nullptr)
        {
            correcto = false;
            continue;
        }

        archivo
            << usuario->getApodo() << '|'
            << usuario->getFechaDeNacimiento() << '|'
            << usuario->getCorreo() << '|'
            << usuario->getFotoDePerfil() << '|'
            << usuario->getPais() << '|'
            << usuario->getCiudad() << '|'
            << usuario->getFechaDeInscripcion()
            << '\n';
    }

    if (!archivo.good())
        correcto = false;

    archivo.close();

    std::cout << (correcto
                      ? "Usuarios guardados correctamente.\n"
                      : "Hubo errores al guardar los usuarios.\n");

    MedidorRecursos::mostrarReporte();

    return correcto;
}

bool Archivo::guardarAmistades(const RedSocial& redSocial,
                                const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de amistades.\n";
        return false;
    }

    int cantidadAmistadesGuardadas = 0;

    for (int i = 0; i < redSocial.getCantidadUsuarios(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* usuario = redSocial.getUsuario(i);

        if (usuario == nullptr)
        {
            continue;
        }

        for (int j = 0; j < usuario->getCantidadAmigos(); j++)
        {
            MedidorRecursos::registrarIteracion();

            Usuario* amigo = usuario->getAmigo(j);

            if (amigo == nullptr)
            {
                continue;
            }

            if (usuario->getApodo() < amigo->getApodo())
            {
                archivo << usuario->getApodo() << "|"
                        << amigo->getApodo() << "\n";

                if (!archivo.good())
                {
                    archivo.close();
                    MedidorRecursos::mostrarReporte();
                    return false;
                }

                cantidadAmistadesGuardadas++;
            }
        }
    }

    archivo.close();

    std::cout << "Amistades guardadas: "
              << cantidadAmistadesGuardadas << "\n";

    MedidorRecursos::mostrarReporte();

    return true;
}

bool Archivo::cargarAmistades(RedSocial& redSocial,
                               const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de amistades.\n";
        return false;
    }

    std::string linea;
    int cantidadAmistadesCargadas = 0;
    int cantidadLineasInvalidas = 0;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        std::size_t separador = linea.find('|');

        if (separador == std::string::npos ||
            separador == 0 ||
            separador == linea.length() - 1)
        {
            cantidadLineasInvalidas++;
            continue;
        }

        std::string apodo1 = linea.substr(0, separador);
        std::string apodo2 = linea.substr(separador + 1);

        if (apodo1 == apodo2)
        {
            cantidadLineasInvalidas++;
            continue;
        }

        Usuario* usuario1 = redSocial.buscarUsuario(apodo1);
        Usuario* usuario2 = redSocial.buscarUsuario(apodo2);

        if (usuario1 == nullptr || usuario2 == nullptr)
        {
            cantidadLineasInvalidas++;
            continue;
        }

        bool amistad1 = usuario1->esAmigo(usuario2);
        bool amistad2 = usuario2->esAmigo(usuario1);

        if (amistad1 && amistad2)
        {
            continue;
        }

        bool agregado1 = false;
        bool agregado2 = false;

        if (!amistad1)
        {
            agregado1 = usuario1->agregarAmigo(usuario2);

            if (!agregado1)
            {
                cantidadLineasInvalidas++;
                continue;
            }
        }

        if (!amistad2)
        {
            agregado2 = usuario2->agregarAmigo(usuario1);

            if (!agregado2)
            {
                if (agregado1)
                {
                    usuario1->quitarAmigo(usuario2);
                }

                cantidadLineasInvalidas++;
                continue;
            }
        }

        cantidadAmistadesCargadas++;
    }

    bool lecturaCorrecta = !archivo.bad();
    archivo.close();

    std::cout << "Amistades cargadas: "
              << cantidadAmistadesCargadas << "\n";

    if (cantidadLineasInvalidas > 0)
    {
        std::cout << "Lineas de amistades invalidas: "
                  << cantidadLineasInvalidas << "\n";
    }

    MedidorRecursos::mostrarReporte();

    return lecturaCorrecta;
}

bool Archivo::guardarSolicitudesPendientes(
    const RedSocial& redSocial,
    const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de solicitudes.\n";
        return false;
    }

    int guardadas = 0;

    for (int i = 0; i < redSocial.getCantidadSolicitudes(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Solicitud* solicitud = redSocial.getSolicitud(i);

        if (solicitud == nullptr || solicitud->getEstado() != 0)
        {
            continue;
        }

        Usuario* remitente = solicitud->getRemitente();
        Usuario* destinatario = solicitud->getDestinatario();

        if (remitente == nullptr || destinatario == nullptr)
        {
            continue;
        }

        archivo << remitente->getApodo() << "|"
                << destinatario->getApodo() << "\n";

        if (!archivo.good())
        {
            archivo.close();
            MedidorRecursos::mostrarReporte();
            return false;
        }

        guardadas++;
    }

    archivo.close();

    std::cout << "Solicitudes pendientes guardadas: "
              << guardadas << "\n";

    MedidorRecursos::mostrarReporte();

    return true;
}

bool Archivo::cargarSolicitudesPendientes(
    RedSocial& redSocial,
    const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de solicitudes.\n";
        return false;
    }

    std::string linea;
    int cargadas = 0;
    int invalidas = 0;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        std::size_t separador = linea.find('|');

        if (separador == std::string::npos ||
            separador == 0 ||
            separador == linea.length() - 1)
        {
            invalidas++;
            continue;
        }

        std::string apodoRemitente = linea.substr(0, separador);
        std::string apodoDestinatario = linea.substr(separador + 1);

        if (apodoRemitente == apodoDestinatario)
        {
            invalidas++;
            continue;
        }

        Usuario* remitente = redSocial.buscarUsuario(apodoRemitente);
        Usuario* destinatario = redSocial.buscarUsuario(apodoDestinatario);

        if (remitente == nullptr || destinatario == nullptr)
        {
            invalidas++;
            continue;
        }

        if (remitente->esAmigo(destinatario))
        {
            continue;
        }

        if (redSocial.enviarSolicitud(remitente, destinatario))
        {
            cargadas++;
        }
        else
        {
            invalidas++;
        }
    }

    bool lecturaCorrecta = !archivo.bad();
    archivo.close();

    std::cout << "Solicitudes pendientes cargadas: "
              << cargadas << "\n";

    if (invalidas > 0)
    {
        std::cout << "Solicitudes omitidas: " << invalidas << "\n";
    }

    MedidorRecursos::mostrarReporte();

    return lecturaCorrecta;
}


bool Archivo::guardarMensajes(const RedSocial& redSocial,
                               const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de mensajes.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    int cantidadGuardada = 0;

    for (int i = 0; i < redSocial.getCantidadUsuarios(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* usuario = redSocial.getUsuario(i);

        if (usuario == nullptr)
        {
            continue;
        }

        for (int j = 0; j < usuario->getCantidadMensajesRecibidos(); j++)
        {
            MedidorRecursos::registrarIteracion();

            Mensajes* mensaje = usuario->getMensajeRecibido(j);

            if (mensaje == nullptr ||
                mensaje->getRemitente() == nullptr ||
                mensaje->getDestinatario() == nullptr)
            {
                continue;
            }

            archivo << std::quoted(mensaje->getRemitente()->getApodo()) << ' '
                    << std::quoted(mensaje->getDestinatario()->getApodo()) << ' '
                    << std::quoted(mensaje->getFecha()) << ' '
                    << mensaje->getLeido() << ' '
                    << std::quoted(mensaje->getContenido()) << '\n';

            if (!archivo.good())
            {
                archivo.close();
                MedidorRecursos::mostrarReporte();
                return false;
            }

            cantidadGuardada++;
        }
    }

    archivo.close();

    std::cout << "Mensajes guardados: " << cantidadGuardada << "\n";

    MedidorRecursos::mostrarReporte();

    return true;
}


bool Archivo::cargarMensajes(RedSocial& redSocial,
                              const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de mensajes.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    std::string linea;
    int cantidadCargada = 0;
    int cantidadOmitida = 0;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        std::istringstream registro(linea);

        std::string apodoRemitente;
        std::string apodoDestinatario;
        std::string fecha;
        std::string contenido;
        bool leido;

        if (!(registro >> std::quoted(apodoRemitente)
              >> std::quoted(apodoDestinatario)
              >> std::quoted(fecha)
              >> leido
              >> std::quoted(contenido)))
        {
            cantidadOmitida++;
            continue;
        }

        registro >> std::ws;

        if (!registro.eof())
        {
            cantidadOmitida++;
            continue;
        }

        Usuario* remitente = redSocial.buscarUsuario(apodoRemitente);
        Usuario* destinatario = redSocial.buscarUsuario(apodoDestinatario);

        if (remitente == nullptr || destinatario == nullptr)
        {
            cantidadOmitida++;
            continue;
        }

        if (!redSocial.enviarMensaje(
                remitente, destinatario, contenido, fecha))
        {
            cantidadOmitida++;
            continue;
        }

        int ultimoIndice = destinatario->getCantidadMensajesRecibidos() - 1;

        Mensajes* mensajeCargado =
            destinatario->getMensajeRecibido(ultimoIndice);

        if (mensajeCargado != nullptr && leido)
        {
            mensajeCargado->marcarComoLeido();
        }

        cantidadCargada++;
    }

    bool lecturaCorrecta = !archivo.bad();
    archivo.close();

    std::cout << "Mensajes cargados: " << cantidadCargada << "\n";

    if (cantidadOmitida > 0)
    {
        std::cout << "Mensajes omitidos: " << cantidadOmitida << "\n";
    }

    MedidorRecursos::mostrarReporte();

    return lecturaCorrecta;
}

bool Archivo::guardarAlbumes(const RedSocial& redSocial,
                              const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de albumes.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    int guardados = 0;

    for (int i = 0; i < redSocial.getCantidadUsuarios(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* usuario = redSocial.getUsuario(i);

        if (usuario == nullptr)
        {
            continue;
        }

        for (int j = 0; j < usuario->getCantidadAlbumes(); j++)
        {
            MedidorRecursos::registrarIteracion();

            Album* album = usuario->getAlbum(j);

            if (album == nullptr)
            {
                continue;
            }

            archivo << std::quoted(usuario->getApodo()) << ' '
                    << std::quoted(album->getNombre()) << ' '
                    << std::quoted(album->getRuta()) << ' '
                    << static_cast<int>(album->getPrivacidad()) << '\n';

            if (!archivo.good())
            {
                archivo.close();
                MedidorRecursos::mostrarReporte();
                return false;
            }

            guardados++;
        }
    }

    archivo.close();

    std::cout << "Albumes guardados: " << guardados << "\n";
    MedidorRecursos::mostrarReporte();

    return true;
}

bool Archivo::cargarAlbumes(RedSocial& redSocial,
                             const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de albumes.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    std::string linea;
    int cargados = 0;
    int omitidos = 0;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        std::istringstream registro(linea);

        std::string apodo;
        std::string nombre;
        std::string ruta;
        int privacidad;

        if (!(registro >> std::quoted(apodo)
              >> std::quoted(nombre)
              >> std::quoted(ruta)
              >> privacidad))
        {
            omitidos++;
            continue;
        }

        registro >> std::ws;

        if (!registro.eof() ||
            privacidad < static_cast<int>(Privacidad::Publico) ||
            privacidad > static_cast<int>(Privacidad::Amigos))
        {
            omitidos++;
            continue;
        }

        Usuario* propietario = redSocial.buscarUsuario(apodo);

        if (propietario == nullptr)
        {
            omitidos++;
            continue;
        }

        bool existe = false;

        for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
        {
            MedidorRecursos::registrarIteracion();

            Album* existente = propietario->getAlbum(i);

            if (existente != nullptr &&
                existente->getNombre() == nombre)
            {
                existe = true;
                break;
            }
        }

        if (existe)
        {
            continue;
        }

        Album* album = new Album(
            nombre, ruta, static_cast<Privacidad>(privacidad));

        MedidorRecursos::registrarMemoria(sizeof(Album));

        if (!propietario->agregarAlbum(album))
        {
            delete album;
            MedidorRecursos::liberarMemoria(sizeof(Album));
            omitidos++;
            continue;
        }

        cargados++;
    }

    bool lecturaCorrecta = !archivo.bad();
    archivo.close();

    std::cout << "Albumes cargados: " << cargados << "\n";

    if (omitidos > 0)
    {
        std::cout << "Registros omitidos: " << omitidos << "\n";
    }

    MedidorRecursos::mostrarReporte();

    return lecturaCorrecta;
}


bool Archivo::guardarFotos(const RedSocial& redSocial,
                            const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de fotos.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    int guardadas = 0;

    for (int i = 0; i < redSocial.getCantidadUsuarios(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* usuario = redSocial.getUsuario(i);

        if (usuario == nullptr)
            continue;

        for (int j = 0; j < usuario->getCantidadAlbumes(); j++)
        {
            MedidorRecursos::registrarIteracion();

            Album* album = usuario->getAlbum(j);

            if (album == nullptr)
                continue;

            for (int k = 0; k < album->getCantidadFotos(); k++)
            {
                MedidorRecursos::registrarIteracion();

                Foto* foto = album->getFoto(k);

                if (foto == nullptr)
                    continue;

                archivo << std::quoted(usuario->getApodo()) << ' '
                        << std::quoted(album->getNombre()) << ' '
                        << std::quoted(foto->getFechaCaptura()) << ' '
                        << foto->getTamano() << ' '
                        << std::quoted(foto->getRuta()) << '\n';

                if (!archivo.good())
                {
                    archivo.close();
                    MedidorRecursos::mostrarReporte();
                    return false;
                }

                guardadas++;
            }
        }
    }

    archivo.close();

    std::cout << "Fotos guardadas: " << guardadas << "\n";

    MedidorRecursos::mostrarReporte();

    return true;
}


bool Archivo::cargarFotos(RedSocial& redSocial,
                           const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de fotos.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    std::string linea;
    int cargadas = 0;
    int omitidas = 0;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        std::istringstream registro(linea);

        std::string apodo;
        std::string nombreAlbum;
        std::string fecha;
        std::string ruta;
        unsigned long long tamano;

        if (!(registro >> std::quoted(apodo)
              >> std::quoted(nombreAlbum)
              >> std::quoted(fecha)
              >> tamano
              >> std::quoted(ruta)))
        {
            omitidas++;
            continue;
        }

        registro >> std::ws;

        if (!registro.eof())
        {
            omitidas++;
            continue;
        }

        Usuario* propietario = redSocial.buscarUsuario(apodo);

        if (propietario == nullptr)
        {
            omitidas++;
            continue;
        }

        Album* albumEncontrado = nullptr;

        for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
        {
            MedidorRecursos::registrarIteracion();

            Album* album = propietario->getAlbum(i);

            if (album != nullptr &&
                album->getNombre() == nombreAlbum)
            {
                albumEncontrado = album;
                break;
            }
        }

        if (albumEncontrado == nullptr)
        {
            omitidas++;
            continue;
        }

        bool existe = false;

        for (int i = 0; i < albumEncontrado->getCantidadFotos(); i++)
        {
            MedidorRecursos::registrarIteracion();

            Foto* fotoExistente = albumEncontrado->getFoto(i);

            if (fotoExistente != nullptr &&
                fotoExistente->getRuta() == ruta)
            {
                existe = true;
                break;
            }
        }

        if (existe)
            continue;

        Foto* foto = new Foto(fecha, tamano, ruta);

        MedidorRecursos::registrarMemoria(sizeof(Foto));

        if (!albumEncontrado->agregarFoto(foto))
        {
            delete foto;
            MedidorRecursos::liberarMemoria(sizeof(Foto));
            omitidas++;
            continue;
        }

        cargadas++;
    }

    bool lecturaCorrecta = !archivo.bad();
    archivo.close();

    std::cout << "Fotos cargadas: " << cargadas << "\n";

    if (omitidas > 0)
        std::cout << "Registros omitidos: " << omitidas << "\n";

    MedidorRecursos::mostrarReporte();

    return lecturaCorrecta;
}

bool Archivo::guardarLikes(const RedSocial& redSocial,
                            const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ofstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de likes.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    int guardados = 0;

    for (int i = 0; i < redSocial.getCantidadUsuarios(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* propietario = redSocial.getUsuario(i);

        if (propietario == nullptr)
            continue;

        for (int j = 0; j < propietario->getCantidadAlbumes(); j++)
        {
            MedidorRecursos::registrarIteracion();

            Album* album = propietario->getAlbum(j);

            if (album == nullptr)
                continue;

            for (int k = 0; k < album->getCantidadFotos(); k++)
            {
                MedidorRecursos::registrarIteracion();

                Foto* foto = album->getFoto(k);

                if (foto == nullptr)
                    continue;

                for (int l = 0; l < foto->getCantidadLikes(); l++)
                {
                    MedidorRecursos::registrarIteracion();

                    Usuario* usuarioLike = foto->getUsuarioLike(l);

                    if (usuarioLike == nullptr)
                        continue;

                    archivo << std::quoted(propietario->getApodo()) << ' '
                            << std::quoted(album->getNombre()) << ' '
                            << std::quoted(foto->getRuta()) << ' '
                            << std::quoted(usuarioLike->getApodo()) << '\n';

                    if (!archivo.good())
                    {
                        archivo.close();
                        MedidorRecursos::mostrarReporte();
                        return false;
                    }

                    guardados++;
                }
            }
        }
    }

    archivo.close();

    std::cout << "Likes guardados: " << guardados << "\n";
    MedidorRecursos::mostrarReporte();

    return true;
}

bool Archivo::cargarLikes(RedSocial& redSocial,
                           const std::string& nombreArchivo)
{
    MedidorRecursos::iniciarMedicion();

    std::ifstream archivo(nombreArchivo.c_str());

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir el archivo de likes.\n";
        MedidorRecursos::mostrarReporte();
        return false;
    }

    std::string linea;
    int cargados = 0;
    int omitidos = 0;

    while (std::getline(archivo, linea))
    {
        MedidorRecursos::registrarIteracion();

        std::istringstream registro(linea);

        std::string apodoPropietario;
        std::string nombreAlbum;
        std::string rutaFoto;
        std::string apodoLike;

        if (!(registro >> std::quoted(apodoPropietario)
              >> std::quoted(nombreAlbum)
              >> std::quoted(rutaFoto)
              >> std::quoted(apodoLike)))
        {
            omitidos++;
            continue;
        }

        registro >> std::ws;

        if (!registro.eof())
        {
            omitidos++;
            continue;
        }

        Usuario* propietario =
            redSocial.buscarUsuario(apodoPropietario);

        Usuario* usuarioLike =
            redSocial.buscarUsuario(apodoLike);

        if (propietario == nullptr || usuarioLike == nullptr)
        {
            omitidos++;
            continue;
        }

        Album* albumEncontrado = nullptr;

        for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
        {
            MedidorRecursos::registrarIteracion();

            Album* album = propietario->getAlbum(i);

            if (album != nullptr &&
                album->getNombre() == nombreAlbum)
            {
                albumEncontrado = album;
                break;
            }
        }

        if (albumEncontrado == nullptr)
        {
            omitidos++;
            continue;
        }

        int indiceFoto = -1;

        for (int i = 0; i < albumEncontrado->getCantidadFotos(); i++)
        {
            MedidorRecursos::registrarIteracion();

            Foto* foto = albumEncontrado->getFoto(i);

            if (foto != nullptr && foto->getRuta() == rutaFoto)
            {
                indiceFoto = i;
                break;
            }
        }

        if (indiceFoto == -1)
        {
            omitidos++;
            continue;
        }

        if (redSocial.darLikeFoto(
                apodoPropietario,
                apodoLike,
                nombreAlbum,
                indiceFoto))
        {
            cargados++;
        }
        else
        {
            omitidos++;
        }
    }

    bool lecturaCorrecta = !archivo.bad();
    archivo.close();

    std::cout << "Likes cargados: " << cargados << "\n";

    if (omitidos > 0)
        std::cout << "Likes omitidos: " << omitidos << "\n";

    MedidorRecursos::mostrarReporte();

    return lecturaCorrecta;
}

