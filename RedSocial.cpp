#include "Usuario.h"
#include "RedSocial.h"
#include "MedidorRecursos.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>

RedSocial::RedSocial()
    : usuarios(nullptr),
    cantidadUsuarios(0),
    capacidadUsuarios(0),
    mensajes(nullptr),
    cantidadMensajes(0),
    capacidadMensajes(0),
    solicitudes(nullptr),
    cantidadSolicitudes(0),
    capacidadSolicitudes(0),
    cantidadUltimasFotos(0)
{
}

Usuario* RedSocial::buscarUsuario(const std::string& apodo) const
{
    for (int i = 0; i < cantidadUsuarios; ++i) {
        MedidorRecursos::registrarIteracion();

        if (usuarios[i]->getApodo() == apodo) {
            return usuarios[i];
        }
    }
    return nullptr;
}

bool RedSocial::registrarUsuario(Usuario* usuario)
{
    if (usuario == nullptr) {
        return false;
    }

    if (buscarUsuario(usuario->getApodo()) != nullptr) {
        return false;
    }

    if (cantidadUsuarios == capacidadUsuarios) {
        int nuevaCapacidad = (capacidadUsuarios == 0) ? 4 : capacidadUsuarios * 2;

        Usuario** nuevo = new (std::nothrow) Usuario*[nuevaCapacidad];

        if (nuevo == nullptr) {
            return false;
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Usuario*));

        for (int i = 0; i < cantidadUsuarios; ++i) {
            MedidorRecursos::registrarIteracion();
            nuevo[i] = usuarios[i];
        }

        MedidorRecursos::liberarMemoria(capacidadUsuarios * sizeof(Usuario*));

        delete[] usuarios;
        usuarios = nuevo;
        capacidadUsuarios = nuevaCapacidad;
    }

    usuarios[cantidadUsuarios] = usuario;
    ++cantidadUsuarios;
    return true;
}

int RedSocial::getCantidadUsuarios() const
{
    return cantidadUsuarios;
}

Usuario* RedSocial::getUsuario(int indice) const
{
    if (indice < 0 || indice >= cantidadUsuarios)
        return nullptr;

    return usuarios[indice];
}

RedSocial::~RedSocial()
{
    for (int i = 0; i < cantidadMensajes; ++i)
    {
        MedidorRecursos::liberarMemoria(sizeof(Mensajes));
        delete mensajes[i];
    }

    for (int i = 0; i < cantidadSolicitudes; ++i)
    {
        MedidorRecursos::liberarMemoria(sizeof(Solicitud));
        delete solicitudes[i];
    }

    for (int i = 0; i < cantidadUsuarios; ++i) {
        MedidorRecursos::liberarMemoria(sizeof(Usuario));
        delete usuarios[i];
    }

    MedidorRecursos::liberarMemoria(capacidadUsuarios * sizeof(Usuario*));
    MedidorRecursos::liberarMemoria(capacidadMensajes * sizeof(Mensajes*));
    MedidorRecursos::liberarMemoria(capacidadSolicitudes * sizeof(Solicitud*));

    delete[] usuarios;
    delete[] mensajes;
    delete[] solicitudes;
}

bool RedSocial::agregarMensajeAdministrado(Mensajes* mensaje)
{
    if (mensaje == nullptr)
    {
        return false;
    }

    if (cantidadMensajes == capacidadMensajes)
    {
        int nuevaCapacidad;

        if (capacidadMensajes == 0)
        {
            nuevaCapacidad = 2;
        }
        else
        {
            nuevaCapacidad = capacidadMensajes * 2;
        }

        Mensajes** nuevoArreglo = new (std::nothrow) Mensajes*[nuevaCapacidad];

        if (nuevoArreglo == nullptr){
            return false;
        }

        for (int i = 0; i < cantidadMensajes; ++i)
        {
            MedidorRecursos::registrarIteracion();
            nuevoArreglo[i] = mensajes[i];
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Mensajes*));

        MedidorRecursos::liberarMemoria(capacidadMensajes * sizeof(Mensajes*));
        delete[] mensajes;
        mensajes = nuevoArreglo;
        capacidadMensajes = nuevaCapacidad;
    }

    mensajes[cantidadMensajes] = mensaje;
    cantidadMensajes++;

    return true;
}

bool RedSocial::enviarMensaje(
    Usuario* remitente,
    Usuario* destinatario,
    const std::string& contenido,
    const std::string& fecha)
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(sizeof(remitente) + sizeof(destinatario));

    if (remitente == nullptr ||
        destinatario == nullptr ||
        remitente == destinatario)
    {
        MedidorRecursos::mostrarReporte();
        return false;
    }

    if (buscarUsuario(remitente->getApodo()) != remitente ||
        buscarUsuario(destinatario->getApodo()) != destinatario)
    {
        MedidorRecursos::mostrarReporte();
        return false;
    }

    if (!remitente->esAmigo(destinatario) ||
        contenido.empty() ||
        contenido.length() > 512)
    {
        MedidorRecursos::mostrarReporte();
        return false;
    }

    if (remitente == nullptr ||
        destinatario == nullptr ||
        remitente == destinatario)
    {
        return false;
    }

    Mensajes* mensaje = new Mensajes(
        remitente,
        destinatario,
        contenido,
        fecha
        );

    MedidorRecursos::registrarMemoria(sizeof(Mensajes));

    if (!agregarMensajeAdministrado(mensaje))
    {
        MedidorRecursos::liberarMemoria(sizeof(Mensajes));
        delete mensaje;
        return false;
    }

    if (!remitente->registrarMensajeEnviado(mensaje))
    {
        cantidadMensajes--;
        MedidorRecursos::liberarMemoria(sizeof(Mensajes));
        delete mensaje;
        return false;
    }

    if (!destinatario->agregarMensajeRecibido(mensaje))
    {
        cantidadMensajes--;
        remitente->quitarUltimoMensajeEnviado();
        MedidorRecursos::liberarMemoria(sizeof(Mensajes));
        delete mensaje;
        return false;
    }
    return true;
}

bool RedSocial::agregarSolicitudAdministrada(Solicitud* solicitud)
{
    if (solicitud == nullptr)
    {
        return false;
    }

    if (cantidadSolicitudes == capacidadSolicitudes)
    {
        int nuevaCapacidad;

        if (capacidadSolicitudes == 0)
        {
            nuevaCapacidad = 2;
        }
        else
        {
            nuevaCapacidad = capacidadSolicitudes * 2;
        }

        Solicitud** nuevoArreglo = new Solicitud*[nuevaCapacidad];

        for (int i = 0; i < cantidadSolicitudes; ++i)
        {
            MedidorRecursos::registrarIteracion();
            nuevoArreglo[i] = solicitudes[i];
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Solicitud*));
        MedidorRecursos::liberarMemoria(capacidadSolicitudes * sizeof(Solicitud*));

        delete[] solicitudes;
        solicitudes = nuevoArreglo;
        capacidadSolicitudes = nuevaCapacidad;
    }

    solicitudes[cantidadSolicitudes] = solicitud;
    cantidadSolicitudes++;

    return true;
}

void RedSocial::buscarMensajesPorFecha(
    Usuario* usuario,
    const std::string& fechaInicio,
    const std::string& fechaFin) const
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(
        sizeof(usuario) + sizeof(fechaInicio) + sizeof(fechaFin)
        );

    if (usuario == nullptr ||
        fechaInicio.empty() ||
        fechaFin.empty() ||
        fechaInicio > fechaFin ||
        buscarUsuario(usuario->getApodo()) != usuario)
    {
        std::cout << "Usuario o rango de fechas invalido.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int encontrados = 0;

    std::cout << "\n=== MENSAJES RECIBIDOS POR FECHA ===\n";

    for (int i = 0; i < usuario->getCantidadMensajesRecibidos(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Mensajes* mensaje = usuario->getMensajeRecibido(i);

        if (mensaje == nullptr)
        {
            continue;
        }

        const std::string fecha = mensaje->getFecha();

        if (fecha >= fechaInicio && fecha <= fechaFin)
        {
            encontrados++;

            std::cout << "\nRemitente: "
                      << mensaje->getRemitente()->getApodo()
                      << "\nFecha: " << fecha
                      << "\nContenido: " << mensaje->getContenido()
                      << "\nEstado: "
                      << (mensaje->getLeido() ? "Leido" : "No leido")
                      << '\n';
        }
    }

    if (encontrados == 0)
    {
        std::cout << "No hay mensajes en ese rango de fechas.\n";
    }

    MedidorRecursos::mostrarReporte();
}

void RedSocial::mostrarMensajesRecibidos(Usuario* usuario)
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(sizeof(usuario));

    if (usuario == nullptr ||
        buscarUsuario(usuario->getApodo()) != usuario)
    {
        std::cout << "Usuario invalido o no registrado.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int opcion = -1;

    while (opcion != 0)
    {
        int cantidad = usuario->getCantidadMensajesRecibidos();

        std::cout << "\n=== MENSAJES RECIBIDOS ===\n";

        if (cantidad == 0)
        {
            std::cout << "No tienes mensajes recibidos.\n";
            break;
        }

        for (int i = 0; i < cantidad; i++)
        {
            MedidorRecursos::registrarIteracion();

            Mensajes* mensaje = usuario->getMensajeRecibido(i);

            if (mensaje == nullptr)
            {
                continue;
            }

            std::cout << "\nMensaje " << i + 1
                      << "\nRemitente: "
                      << mensaje->getRemitente()->getApodo()
                      << "\nFecha: " << mensaje->getFecha()
                      << "\nContenido: " << mensaje->getContenido()
                      << "\nEstado: "
                      << (mensaje->getLeido() ? "Leido" : "No leido")
                      << '\n';
        }

        std::cout << "\nIngresa el numero del mensaje para marcarlo como leido.\n";
        std::cout << "0. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> opcion;

        if (opcion == 0)
        {
            break;
        }

        if (opcion < 1 || opcion > cantidad)
        {
            std::cout << "Numero de mensaje invalido.\n";
            continue;
        }

        Mensajes* mensaje = usuario->getMensajeRecibido(opcion - 1);

        if (mensaje == nullptr)
        {
            std::cout << "No se pudo consultar ese mensaje.\n";
            continue;
        }

        if (mensaje->getLeido())
        {
            std::cout << "Este mensaje ya estaba marcado como leido.\n";
        }
        else if (usuario->marcarMensajeComoLeido(opcion - 1))
        {
            std::cout << "Mensaje marcado como leido.\n";
        }
        else
        {
            std::cout << "No se pudo marcar el mensaje como leido.\n";
        }
    }

    MedidorRecursos::mostrarReporte();
}

void RedSocial::mostrarMensajesNoLeidosOrdenados(Usuario* usuario)
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(sizeof(usuario));

    if (usuario == nullptr ||
        buscarUsuario(usuario->getApodo()) != usuario)
    {
        std::cout << "Usuario invalido o no registrado.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int totalRecibidos = usuario->getCantidadMensajesRecibidos();

    Mensajes** noLeidos = nullptr;

    if (totalRecibidos > 0)
    {
        noLeidos = new Mensajes*[totalRecibidos];

        MedidorRecursos::registrarMemoria(
            totalRecibidos * sizeof(Mensajes*)
            );
    }

    int cantidad = 0;

    for (int i = 0; i < totalRecibidos; i++)
    {
        MedidorRecursos::registrarIteracion();

        Mensajes* mensaje = usuario->getMensajeRecibido(i);

        if (mensaje != nullptr && !mensaje->getLeido())
        {
            noLeidos[cantidad] = mensaje;
            cantidad++;
        }
    }

    // Ordenar por fecha, de la más antigua a la más reciente.
    for (int i = 0; i < cantidad - 1; i++)
    {
        for (int j = 0; j < cantidad - 1 - i; j++)
        {
            MedidorRecursos::registrarIteracion();

            if (noLeidos[j]->getFecha() >
                noLeidos[j + 1]->getFecha())
            {
                Mensajes* auxiliar = noLeidos[j];
                noLeidos[j] = noLeidos[j + 1];
                noLeidos[j + 1] = auxiliar;
            }
        }
    }

    std::cout << "\n=== MENSAJES NO LEIDOS ===\n";

    if (cantidad == 0)
    {
        std::cout << "No tienes mensajes sin leer.\n";
    }
    else
    {
        for (int i = 0; i < cantidad; i++)
        {
            MedidorRecursos::registrarIteracion();

            std::cout << "\nMensaje " << i + 1
                      << "\nRemitente: "
                      << noLeidos[i]->getRemitente()->getApodo()
                      << "\nFecha: " << noLeidos[i]->getFecha()
                      << "\nContenido: " << noLeidos[i]->getContenido()
                      << "\n";
        }

        int opcion;

        std::cout << "\nNumero del mensaje para marcar como leido"
                  << "\n0. Salir"
                  << "\nOpcion: ";
        std::cin >> opcion;

        if (opcion >= 1 && opcion <= cantidad)
        {
            Mensajes* seleccionado = noLeidos[opcion - 1];

            // Encontrar su posición original en los mensajes recibidos.
            for (int i = 0; i < totalRecibidos; i++)
            {
                MedidorRecursos::registrarIteracion();

                if (usuario->getMensajeRecibido(i) == seleccionado)
                {
                    if (usuario->marcarMensajeComoLeido(i))
                    {
                        std::cout << "Mensaje marcado como leido.\n";
                    }
                    else
                    {
                        std::cout << "No se pudo marcar como leido.\n";
                    }

                    break;
                }
            }
        }
        else if (opcion != 0)
        {
            std::cout << "Opcion invalida.\n";
        }
    }

    delete[] noLeidos;

    if (totalRecibidos > 0)
    {
        MedidorRecursos::liberarMemoria(
            totalRecibidos * sizeof(Mensajes*)
            );
    }

    MedidorRecursos::mostrarReporte();
}

bool RedSocial::administraSolicitud(
    const Solicitud* solicitud) const
{
    if (solicitud == nullptr)
    {
        return false;
    }

    for (int i = 0; i < cantidadSolicitudes; ++i)
    {
        MedidorRecursos::registrarIteracion();

        if (solicitudes[i] == solicitud)
        {
            return true;
        }
    }

    return false;
}

bool RedSocial::aceptarSolicitud(Solicitud* solicitud)
{
    if (!administraSolicitud(solicitud) ||
        solicitud->getEstado() != 0)
    {
        return false;
    }

    Usuario* remitente = solicitud->getRemitente();
    Usuario* destinatario = solicitud->getDestinatario();

    if (remitente == nullptr ||
        destinatario == nullptr ||
        remitente == destinatario)
    {
        return false;
    }

    bool amistadRemitente = remitente->esAmigo(destinatario);
    bool amistadDestinatario = destinatario->esAmigo(remitente);
    bool agregadoRemitente = false;

    if (!amistadRemitente){
        if (!remitente->agregarAmigo(destinatario))
        {
            return false;
        }

        agregadoRemitente = true;
    }

    if (!amistadDestinatario){
        if (!destinatario->agregarAmigo(remitente))
        {
            if (agregadoRemitente)
            {
                remitente->quitarAmigo(destinatario);
            }
            return false;
        }
    }

    solicitud->aceptar();
    return true;
}

bool RedSocial::rechazarSolicitud(Solicitud* solicitud){
    if (!administraSolicitud(solicitud) ||
        solicitud->getEstado() != 0)
    {
        return false;
    }

    solicitud->rechazar();
    return true;
}

bool RedSocial::enviarSolicitud(
    Usuario* remitente,
    Usuario* destinatario)
{
    if (remitente == nullptr ||
        destinatario == nullptr ||
        remitente == destinatario)
    {
        return false;
    }

    if (remitente->esAmigo(destinatario) ||
        destinatario->esAmigo(remitente))
    {
        return false;
    }

    for (int i = 0; i < cantidadSolicitudes; ++i)
    {
        MedidorRecursos::registrarIteracion();

        Solicitud* actual = solicitudes[i];

        if (actual->getRemitente() == remitente &&
            actual->getDestinatario() == destinatario &&
            actual->getEstado() == 0)
        {
            return false;
        }
    }

    Solicitud* solicitud = new Solicitud(
        remitente,
        destinatario
        );

    MedidorRecursos::registrarMemoria(sizeof(Solicitud));

    if (!agregarSolicitudAdministrada(solicitud))
    {
        MedidorRecursos::liberarMemoria(sizeof(Solicitud));
        delete solicitud;
        return false;
    }

    if (!destinatario->agregarSolicitud(solicitud))
    {
        cantidadSolicitudes--;

        MedidorRecursos::liberarMemoria(sizeof(Solicitud));
        delete solicitud;

        return false;
    }

    return true;
}

int RedSocial::contarAmigosEnComun(const Usuario* usuario,const Usuario* candidato) const{
    if (usuario == nullptr || candidato == nullptr)
        return 0;

    int comunes = 0;

    for (int i = 0; i < usuario->getCantidadAmigos(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* amigo = usuario->getAmigo(i);

        if (amigo != nullptr && amigo->esAmigo(candidato))
        {
            comunes++;
        }
    }

    return comunes;
}

bool RedSocial::tieneSolicitudPendiente(const Usuario* remitente,const Usuario* destinatario) const
{
    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        MedidorRecursos::registrarIteracion();

        Solicitud* solicitud = solicitudes[i];

        if (solicitud == nullptr)
            continue;

        if (solicitud->getEstado() != 0)
            continue;

        bool mismoSentido =
            solicitud->getRemitente() == remitente &&
            solicitud->getDestinatario() == destinatario;

        bool sentidoContrario =
            solicitud->getRemitente() == destinatario &&
            solicitud->getDestinatario() == remitente;

        if (mismoSentido || sentidoContrario)
            return true;
    }

    return false;
}

bool RedSocial::puedeVerAlbum(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante,
    const std::string& nombreAlbum) const
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr) {
        return false;
    }

    for (int i = 0; i < propietario->getCantidadAlbumes(); ++i) {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album != nullptr &&
            album->getNombre() == nombreAlbum) {

            MedidorRecursos::registrarMemoria(
                sizeof(Usuario*) * 2 + sizeof(Album*)
                );

            return album->puedeVer(propietario, visitante);
        }
    }

    return false;
}

void RedSocial::mostrarSugerenciasAmistad(Usuario* usuario, int k) const
{
    if (usuario == nullptr || k < 1 || k > 20)
    {
        std::cout << "Usuario o cantidad de sugerencias invalida.\n";
        return;
    }

    if (cantidadUsuarios == 0)
    {
        std::cout << "No hay usuarios registrados.\n";
        return;
    }

    Usuario** sugerencias = new (std::nothrow) Usuario*[cantidadUsuarios];

    if(sugerencias == nullptr){
        return;
    }

    int* amigosEnComun = new (std::nothrow) int[cantidadUsuarios];

    if(amigosEnComun == nullptr){
        delete[] sugerencias;
        return;
    }

    MedidorRecursos::registrarMemoria(
        cantidadUsuarios * sizeof(Usuario*) +
        cantidadUsuarios * sizeof(int));

    int cantidadSugerencias = 0;

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* candidato = usuarios[i];

        if (candidato == nullptr || candidato == usuario)
            continue;

        if (usuario->esAmigo(candidato))
            continue;

        if (tieneSolicitudPendiente(usuario, candidato))
            continue;

        int comunes = contarAmigosEnComun(usuario, candidato);

        if (comunes == 0)
            continue;

        sugerencias[cantidadSugerencias] = candidato;
        amigosEnComun[cantidadSugerencias] = comunes;
        cantidadSugerencias++;
    }

    for (int i = 0; i < cantidadSugerencias - 1; i++)
    {
        MedidorRecursos::registrarIteracion();

        for (int j = 0; j < cantidadSugerencias - i - 1; j++){
            MedidorRecursos::registrarIteracion();

            if (amigosEnComun[j] < amigosEnComun[j + 1]){
                int temporalComun = amigosEnComun[j];
                amigosEnComun[j] = amigosEnComun[j + 1];
                amigosEnComun[j + 1] = temporalComun;

                Usuario* temporalUsuario = sugerencias[j];
                sugerencias[j] = sugerencias[j + 1];
                sugerencias[j + 1] = temporalUsuario;
            }
        }
    }

    std::cout << "\n=== SUGERENCIAS DE AMISTAD ===\n";

    int limite = (k < cantidadSugerencias)? k : cantidadSugerencias;

    if (limite == 0)
    {
        std::cout << "No hay sugerencias disponibles.\n";
    }

    for (int i = 0; i < limite; i++)
    {
        MedidorRecursos::registrarIteracion();

        std::cout << i + 1 << ". "
                  << sugerencias[i]->getApodo()
                  << " | Amigos en comun: "
                  << amigosEnComun[i] << '\n';
    }

    delete[] sugerencias;
    delete[] amigosEnComun;

    MedidorRecursos::liberarMemoria(cantidadUsuarios * sizeof(Usuario*) + cantidadUsuarios * sizeof(int));
}

void RedSocial::mostrarFotosAlbum(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante,
    const std::string& nombreAlbum) const
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr) {
        std::cout << "El propietario o visitante no existe.\n";
        return;
    }

    Album* albumEncontrado = nullptr;

    for (int i = 0; i < propietario->getCantidadAlbumes(); ++i) {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album != nullptr &&
            album->getNombre() == nombreAlbum) {
            albumEncontrado = album;
            break;
        }
    }

    if (albumEncontrado == nullptr) {
        std::cout << "El album no existe.\n";
        return;
    }

    if (!albumEncontrado->puedeVer(propietario, visitante)) {
        std::cout << "No tienes permiso para ver este album.\n";
        return;
    }

    std::cout << "\nFotos del album: "
              << albumEncontrado->getNombre() << '\n';

    if (albumEncontrado->getCantidadFotos() == 0) {
        std::cout << "Este album no tiene fotos.\n";
        return;
    }

    for (int i = 0; i < albumEncontrado->getCantidadFotos(); ++i) {
        MedidorRecursos::registrarIteracion();

        Foto* foto = albumEncontrado->getFoto(i);

        if (foto != nullptr) {
            foto->mostrar();
        }
    }

    MedidorRecursos::registrarMemoria(
        sizeof(Usuario*) * 2 + sizeof(Album*)
        );
}

void RedSocial::recorrerAlbum(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante,
    const std::string& nombreAlbum)
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr)
    {
        std::cout << "Usuario no encontrado.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    Album* album = nullptr;

    for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Album* actual = propietario->getAlbum(i);

        if (actual != nullptr &&
            actual->getNombre() == nombreAlbum)
        {
            album = actual;
            break;
        }
    }

    if (album == nullptr)
    {
        std::cout << "Album no encontrado.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    if (!album->puedeVer(propietario, visitante))
    {
        std::cout << "No tienes permiso para ver este album.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int totalFotos = album->getCantidadFotos();

    if (totalFotos == 0)
    {
        std::cout << "Este album no tiene fotos.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int indice = 0;
    int opcion = 0;

    while (opcion != 4)
    {
        MedidorRecursos::registrarIteracion();

        Foto* foto = album->getFoto(indice);

        if (foto == nullptr)
        {
            std::cout << "No se pudo obtener la foto.\n";
            break;
        }

        std::cout << "\n=== CARRUSEL DE FOTOS ===\n";
        std::cout << "Album: " << album->getNombre() << '\n';
        std::cout << "Foto " << indice + 1
                  << " de " << totalFotos << '\n';

        foto->mostrar();

        std::cout << "Fecha: " << foto->getFechaCaptura() << '\n';
        std::cout << "Tamano: " << foto->getTamano() << " bytes\n";
        std::cout << "Likes: " << foto->getCantidadLikes() << '\n';

        std::cout << "\n1. Foto siguiente\n";
        std::cout << "2. Foto anterior\n";
        std::cout << "3. Dar o quitar Me gusta\n";
        std::cout << "4. Salir del album\n";
        std::cout << "Opcion: ";

        if (!(std::cin >> opcion))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            opcion = 0;
            std::cout << "Entrada invalida.\n";
            continue;
        }

        switch (opcion)
        {
        case 1:
            indice = (indice + 1) % totalFotos;
            break;

        case 2:
            indice = (indice - 1 + totalFotos) % totalFotos;
            break;

        case 3:
        {
            std::cout << "1. Dar Me gusta\n";
            std::cout << "2. Quitar Me gusta\n";
            std::cout << "Opcion: ";

            int opcionLike;

            if (!(std::cin >> opcionLike))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Entrada invalida.\n";
                break;
            }

            if (opcionLike == 1)
            {
                darLikeFoto(apodoPropietario,
                            apodoVisitante,
                            nombreAlbum,
                            indice);
            }
            else if (opcionLike == 2)
            {
                quitarLikeFoto(apodoPropietario,
                               apodoVisitante,
                               nombreAlbum,
                               indice);
            }
            else
            {
                std::cout << "Opcion invalida.\n";
            }

            break;
        }

        case 4:
            std::cout << "Saliendo del album...\n";
            break;

        default:
            std::cout << "Opcion invalida.\n";
            break;
        }
    }

    MedidorRecursos::mostrarReporte();
}

void RedSocial::mostrarAlbumesVisibles(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante) const
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr)
    {
        std::cout << "Usuario no encontrado.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    std::cout << "\n=== ALBUMES DEL PERFIL DE "
              << apodoPropietario << " ===\n";

    int albumesVisibles = 0;

    for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album == nullptr)
            continue;

        if (album->getPrivacidad() == Privacidad::Privado)
            continue;

        if (album->puedeVer(propietario, visitante))
        {
            albumesVisibles++;

            std::cout << albumesVisibles << ". "
                      << album->getNombre() << '\n';
        }
    }

    if (albumesVisibles == 0)
    {
        std::cout << "Este perfil no tiene albumes disponibles para ti.\n";
    }

    MedidorRecursos::mostrarReporte();
}

bool RedSocial::darLikeFoto(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante,
    const std::string& nombreAlbum,
    int indiceFoto)
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr) {
        std::cout << "El propietario o visitante no existe.\n";
        return false;
    }

    Album* albumEncontrado = nullptr;

    for (int i = 0; i < propietario->getCantidadAlbumes(); ++i) {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album != nullptr &&
            album->getNombre() == nombreAlbum) {
            albumEncontrado = album;
            break;
        }
    }

    if (albumEncontrado == nullptr) {
        std::cout << "El album no existe.\n";
        return false;
    }

    if (!albumEncontrado->puedeVer(propietario, visitante)) {
        std::cout << "No tienes permiso para interactuar con este album.\n";
        return false;
    }

    Foto* foto = albumEncontrado->getFoto(indiceFoto);

    if (foto == nullptr) {
        std::cout << "El indice de la foto no es valido.\n";
        return false;
    }

    if (!foto->darLike(visitante))
    {
        std::cout << "Este usuario ya dio Me gusta a esta foto.\n";
        return false;
    }

    std::cout << "Me gusta registrado correctamente.\n";
    std::cout << "Cantidad de likes: "
              << foto->getCantidadLikes() << '\n';

    MedidorRecursos::mostrarReporte();
    return true;
}

bool RedSocial::quitarLikeFoto(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante,
    const std::string& nombreAlbum,
    int indiceFoto)
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr)
    {
        std::cout << "Usuario no encontrado.\n";
        return false;
    }

    Album* album = nullptr;

    for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Album* actual = propietario->getAlbum(i);

        if (actual != nullptr &&
            actual->getNombre() == nombreAlbum)
        {
            album = actual;
            break;
        }
    }

    if (album == nullptr)
    {
        std::cout << "Album no encontrado.\n";
        return false;
    }

    if (!album->puedeVer(propietario, visitante))
    {
        std::cout << "No tienes permiso para interactuar con este album.\n";
        return false;
    }

    Foto* foto = album->getFoto(indiceFoto);

    if (foto == nullptr)
    {
        std::cout << "Indice de foto invalido.\n";
        return false;
    }

    if (!foto->quitarLike(visitante))
    {
        std::cout << "Este usuario no habia dado Me gusta a esta foto.\n";
        return false;
    }

    std::cout << "Me gusta eliminado correctamente.\n";
    std::cout << "Cantidad de likes: "
              << foto->getCantidadLikes() << '\n';

    MedidorRecursos::mostrarReporte();
    return true;
}

bool RedSocial::agregarFotoAlbum(
    const std::string& apodoPropietario,
    const std::string& nombreAlbum,
    Foto* foto)
{
    MedidorRecursos::iniciarMedicion();

    if (foto == nullptr) {
        std::cout << "La foto no es valida.\n";
        return false;
    }

    Usuario* propietario = buscarUsuario(apodoPropietario);

    if (propietario == nullptr) {
        std::cout << "El usuario no existe.\n";
        return false;
    }

    for (int i = 0; i < propietario->getCantidadAlbumes(); ++i) {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album != nullptr &&
            album->getNombre() == nombreAlbum) {

            if (album->agregarFoto(foto)) {
                MedidorRecursos::registrarMemoria(sizeof(Foto*));
                std::cout << "Foto agregada correctamente.\n";
                return true;
            }

            std::cout << "No fue posible agregar la foto.\n";
            return false;
        }
    }

    std::cout << "El album no existe.\n";
    return false;
}

void RedSocial::visitarPerfil(
    const std::string& apodoPropietario,
    const std::string& apodoVisitante)
{
    MedidorRecursos::iniciarMedicion();

    Usuario* propietario = buscarUsuario(apodoPropietario);
    Usuario* visitante = buscarUsuario(apodoVisitante);

    if (propietario == nullptr || visitante == nullptr)
    {
        std::cout << "Usuario no encontrado.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    std::cout << "\n=== VISITANDO PERFIL DE "
              << apodoPropietario << " ===\n";

    int cantidadVisibles = 0;

    for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album == nullptr)
            continue;

        if (album->getPrivacidad() == Privacidad::Privado)
            continue;

        if (album->puedeVer(propietario, visitante))
        {
            cantidadVisibles++;

            std::cout << cantidadVisibles << ". "
                      << album->getNombre() << '\n';
        }
    }

    if (cantidadVisibles == 0)
    {
        std::cout << "No hay albumes disponibles para ti.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    std::cout << "0. Salir del perfil\n";

    int opcion;

    while (true)
    {
        std::cout << "Selecciona un album: ";

        if (!(std::cin >> opcion))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Entrada invalida.\n";
            continue;
        }

        if (opcion == 0)
        {
            std::cout << "Saliendo del perfil...\n";
            MedidorRecursos::mostrarReporte();
            return;
        }

        if (opcion < 1 || opcion > cantidadVisibles)
        {
            std::cout << "Seleccion invalida.\n";
            continue;
        }

        break;
    }

    Album* albumSeleccionado = nullptr;
    int posicionVisible = 0;

    for (int i = 0; i < propietario->getCantidadAlbumes(); i++)
    {
        MedidorRecursos::registrarIteracion();

        Album* album = propietario->getAlbum(i);

        if (album == nullptr)
            continue;

        if (album->getPrivacidad() == Privacidad::Privado)
            continue;

        if (album->puedeVer(propietario, visitante))
        {
            posicionVisible++;

            if (posicionVisible == opcion)
            {
                albumSeleccionado = album;
                break;
            }
        }
    }

    if (albumSeleccionado == nullptr)
    {
        std::cout << "No se pudo seleccionar el album.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    MedidorRecursos::mostrarReporte();

    recorrerAlbum(apodoPropietario,
                  apodoVisitante,
                  albumSeleccionado->getNombre());
}

void RedSocial::mostrarSolicitudesPendientes(Usuario* usuario) const
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(sizeof(usuario));

    if (usuario == nullptr)
    {
        std::cout << "Usuario invalido.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int numero = 0;

    std::cout << "\n=== SOLICITUDES DE AMISTAD PENDIENTES ===\n";

    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        MedidorRecursos::registrarIteracion();

        Solicitud* solicitud = solicitudes[i];

        if (solicitud != nullptr &&
            solicitud->getDestinatario() == usuario &&
            solicitud->getEstado() == 0)
        {
            numero++;

            std::cout << numero << ". "
                      << solicitud->getRemitente()->getApodo()
                      << " quiere ser tu amigo.\n";
        }
    }

    if (numero == 0)
    {
        std::cout << "No tienes solicitudes pendientes.\n";
    }

    MedidorRecursos::mostrarReporte();
}

void RedSocial::gestionarSolicitudesPendientes(Usuario* usuario)
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(sizeof(usuario));

    if (usuario == nullptr)
    {
        std::cout << "Usuario invalido.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int opcion = -1;

    while (opcion != 0)
    {
        int cantidadPendientes = 0;

        std::cout << "\n=== GESTIONAR SOLICITUDES ===\n";

        for (int i = 0; i < cantidadSolicitudes; i++)
        {
            MedidorRecursos::registrarIteracion();

            Solicitud* solicitud = solicitudes[i];

            if (solicitud != nullptr &&
                solicitud->getDestinatario() == usuario &&
                solicitud->getEstado() == 0)
            {
                cantidadPendientes++;

                std::cout << cantidadPendientes << ". "
                          << solicitud->getRemitente()->getApodo()
                          << "\n";
            }
        }

        if (cantidadPendientes == 0)
        {
            std::cout << "No tienes solicitudes pendientes.\n";
            break;
        }

        std::cout << "0. Salir\n";
        std::cout << "Selecciona una solicitud: ";
        std::cin >> opcion;

        if (opcion == 0)
        {
            break;
        }

        if (opcion < 1 || opcion > cantidadPendientes)
        {
            std::cout << "Opcion invalida.\n";
            continue;
        }

        int numeroActual = 0;
        Solicitud* seleccionada = nullptr;

        for (int i = 0; i < cantidadSolicitudes; i++)
        {
            MedidorRecursos::registrarIteracion();

            Solicitud* solicitud = solicitudes[i];

            if (solicitud != nullptr &&
                solicitud->getDestinatario() == usuario &&
                solicitud->getEstado() == 0)
            {
                numeroActual++;

                if (numeroActual == opcion)
                {
                    seleccionada = solicitud;
                    break;
                }
            }
        }

        if (seleccionada == nullptr)
        {
            std::cout << "No se encontro la solicitud.\n";
            continue;
        }

        int accion;

        std::cout << "\n1. Aceptar solicitud\n";
        std::cout << "2. Rechazar solicitud\n";
        std::cout << "0. Cancelar\n";
        std::cout << "Elige una accion: ";
        std::cin >> accion;

        if (accion == 1)
        {
            if (aceptarSolicitud(seleccionada))
            {
                std::cout << "Solicitud aceptada correctamente.\n";
            }
            else
            {
                std::cout << "No fue posible aceptar la solicitud.\n";
            }
        }
        else if (accion == 2)
        {
            if (rechazarSolicitud(seleccionada))
            {
                std::cout << "Solicitud rechazada correctamente.\n";
            }
            else
            {
                std::cout << "No fue posible rechazar la solicitud.\n";
            }
        }
        else if (accion != 0)
        {
            std::cout << "Accion invalida.\n";
        }
    }

    MedidorRecursos::mostrarReporte();
}

Usuario* RedSocial::iniciarSesion(const std::string& apodo) const
{
    MedidorRecursos::iniciarMedicion();
    MedidorRecursos::registrarMemoria(sizeof(apodo));

    Usuario* usuario = buscarUsuario(apodo);

    if (usuario == nullptr)
    {
        std::cout << "Apodo no encontrado.\n";
    }
    else
    {
        std::cout << "Bienvenido, "
                  << usuario->getApodo() << ".\n";
    }

    MedidorRecursos::mostrarReporte();
    return usuario;
}



void RedSocial::mostrarFotosAleatoriasAmigos(Usuario* usuario)
{
    MedidorRecursos::iniciarMedicion();

    if (usuario == nullptr)
    {
        std::cout << "Usuario invalido.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    int cantidadAmigos = usuario->getCantidadAmigos();

    if (cantidadAmigos == 0)
    {
        std::cout << "No tienes amigos registrados.\n";
        MedidorRecursos::mostrarReporte();
        return;
    }

    static bool semillaInicializada = false;

    if (!semillaInicializada)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        semillaInicializada = true;
    }

    Usuario** candidatos = new Usuario*[cantidadAmigos];
    int* pesos = new int[cantidadAmigos];

    MedidorRecursos::registrarMemoria(
        sizeof(Usuario*) * cantidadAmigos +
        sizeof(int) * cantidadAmigos);

    int cantidadCandidatos = 0;

    for (int i = 0; i < cantidadAmigos; i++)
    {
        MedidorRecursos::registrarIteracion();

        Usuario* amigo = usuario->getAmigo(i);

        if (amigo == nullptr || amigo->getFotoDePerfil().empty())
        {
            continue;
        }

        const std::string& ruta = amigo->getFotoDePerfil();

        bool rutaRepetida = false;

        for (int j = 0; j < cantidadCandidatos; j++)
        {
            MedidorRecursos::registrarIteracion();

            if (candidatos[j]->getFotoDePerfil() == ruta)
            {
                rutaRepetida = true;
                break;
            }
        }

        if (rutaRepetida)
        {
            continue;
        }

        bool yaMostrada = false;

        for (int j = 0; j < cantidadUltimasFotos; j++)
        {
            MedidorRecursos::registrarIteracion();

            if (rutasUltimasFotos[j] == ruta)
            {
                yaMostrada = true;
                break;
            }
        }

        if (yaMostrada)
        {
            continue;
        }

        int amigosEnComun = contarAmigosEnComun(usuario, amigo);
        int peso;

        if (amigosEnComun <= 10)
        {
            peso = 2;
        }
        else if (amigosEnComun <= 20)
        {
            peso = 5;
        }
        else
        {
            peso = 7;
        }

        candidatos[cantidadCandidatos] = amigo;
        pesos[cantidadCandidatos] = peso;
        cantidadCandidatos++;
    }

    if (cantidadCandidatos < 3)
    {
        std::cout
            << "No hay suficientes fotos de perfil distintas "
            << "para mostrar un grupo nuevo de tres.\n";

        delete[] candidatos;
        delete[] pesos;

        MedidorRecursos::liberarMemoria(
            sizeof(Usuario*) * cantidadAmigos +
            sizeof(int) * cantidadAmigos);

        MedidorRecursos::mostrarReporte();
        return;
    }

    std::string rutasSeleccionadas[3];

    std::cout << "\n=== FOTOS DE PERFIL DE TUS AMIGOS ===\n";

    for (int seleccion = 0; seleccion < 3; seleccion++)
    {
        int pesoTotal = 0;

        for (int i = 0; i < cantidadCandidatos; i++)
        {
            MedidorRecursos::registrarIteracion();
            pesoTotal += pesos[i];
        }

        int numeroAleatorio = std::rand() % pesoTotal;
        int acumulado = 0;
        int indiceElegido = 0;

        for (int i = 0; i < cantidadCandidatos; i++)
        {
            MedidorRecursos::registrarIteracion();
            acumulado += pesos[i];

            if (numeroAleatorio < acumulado)
            {
                indiceElegido = i;
                break;
            }
        }

        Usuario* elegido = candidatos[indiceElegido];

        rutasSeleccionadas[seleccion] =
            elegido->getFotoDePerfil();

        std::cout << "\nAmigo: " << elegido->getApodo() << "\n";
        std::cout << "Foto de perfil: "
                  << elegido->getFotoDePerfil() << "\n";

        for (int i = indiceElegido; i < cantidadCandidatos - 1; i++)
        {
            MedidorRecursos::registrarIteracion();
            candidatos[i] = candidatos[i + 1];
            pesos[i] = pesos[i + 1];
        }

        cantidadCandidatos--;
    }

    for (int i = 0; i < 3; i++)
    {
        rutasUltimasFotos[i] = rutasSeleccionadas[i];
    }

    cantidadUltimasFotos = 3;

    delete[] candidatos;
    delete[] pesos;

    MedidorRecursos::liberarMemoria(
        sizeof(Usuario*) * cantidadAmigos +
        sizeof(int) * cantidadAmigos);

    MedidorRecursos::mostrarReporte();
}


int RedSocial::getCantidadSolicitudes() const
{
    return cantidadSolicitudes;
}

Solicitud* RedSocial::getSolicitud(int indice) const
{
    if (indice < 0 || indice >= cantidadSolicitudes)
    {
        return nullptr;
    }

    return solicitudes[indice];
}
