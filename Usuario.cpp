#include "Usuario.h"
#include "MedidorRecursos.h"
#include <new>

/*Constructor que define la estrcutura del usuario, se declara vació por defecto
 */
Usuario::Usuario()
    : apodo(""),
    fechaDeNacimiento(""),
    correo(""),
    fotoDePerfil(""),
    pais(""),
    ciudad(""),
    fechaDeInscripcion(""),
    amigos(nullptr),
    cantidadAmigos(0),
    capacidadAmigos(0),
    mensajesEnviados(nullptr),
    cantidadMensajesEnviados(0),
    capacidadMensajesEnviados(0),
    mensajesRecibidos(nullptr),
    cantidadMensajesRecibidos(0),
    capacidadMensajesRecibidos(0),
    solicitudAmistad(nullptr),
    cantidadSolicitudes(0),
    capacidadSolicitudes(0),
    albumes(nullptr),
    cantidadAlbumes(0),
    capacidadAlbumes(0)
{
}

/*Constructor parametrizado del usuario*/
Usuario::Usuario(const std::string& apodo,
                 const std::string& fechaDeNacimiento,
                 const std::string& correo,
                 const std::string& fotoDePerfil,
                 const std::string& pais,
                 const std::string& ciudad,
                 const std::string& fechaDeInscripcion)
    : apodo(apodo),
    fechaDeNacimiento(fechaDeNacimiento),
    correo(correo),
    fotoDePerfil(fotoDePerfil),
    pais(pais),
    ciudad(ciudad),
    fechaDeInscripcion(fechaDeInscripcion),
    amigos(nullptr),
    cantidadAmigos(0),
    capacidadAmigos(0),
    mensajesEnviados(nullptr),
    cantidadMensajesEnviados(0),
    capacidadMensajesEnviados(0),
    mensajesRecibidos(nullptr),
    cantidadMensajesRecibidos(0),
    capacidadMensajesRecibidos(0),
    solicitudAmistad(nullptr),
    cantidadSolicitudes(0),
    capacidadSolicitudes(0),
    albumes(nullptr),
    cantidadAlbumes(0),
    capacidadAlbumes(0)
{
}

/*Itera sobre los amigos revisando si ya un usuario lo contiene*/
bool Usuario::esAmigo(const Usuario* usuario) const
{
    for (int i = 0; i < cantidadAmigos; ++i) {
        MedidorRecursos::registrarIteracion();
        if (amigos[i] == usuario) {
            return true;
        }
    }
    return false;
}

/*Metodo que recibe un usuario y valida si ya es amigo de otro usuario
 * si no son amigos retorna true
 */

bool Usuario::agregarAmigo(Usuario* usuario){
    if (usuario == nullptr || usuario == this || esAmigo(usuario)) {
        return false;
    }

    if (cantidadAmigos == capacidadAmigos) {
        int nuevaCapacidad = (capacidadAmigos == 0) ? 4 : capacidadAmigos * 2;

        Usuario** nuevo = new (std::nothrow) Usuario*[nuevaCapacidad];

        if (nuevo == nullptr) {
            return false;
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Usuario*));

        for (int i = 0; i < cantidadAmigos; ++i) {
            MedidorRecursos::registrarIteracion();
            nuevo[i] = amigos[i];
        }

        MedidorRecursos::liberarMemoria(capacidadAmigos * sizeof(Usuario*));

        delete[] amigos;
        amigos = nuevo;
        capacidadAmigos = nuevaCapacidad;
    }

    amigos[cantidadAmigos] = usuario;
    ++cantidadAmigos;
    return true;
}

bool Usuario::quitarAmigo(Usuario* usuario)
{
    if (usuario == nullptr)
    {
        return false;
    }

    for (int i = 0; i < cantidadAmigos; ++i)
    {
        MedidorRecursos::registrarIteracion();

        if (amigos[i] == usuario)
        {
            for (int j = i; j < cantidadAmigos - 1; ++j)
            {
                MedidorRecursos::registrarIteracion();
                amigos[j] = amigos[j + 1];
            }

            cantidadAmigos--;
            amigos[cantidadAmigos] = nullptr;

            return true;
        }
    }

    return false;
}

/*Metodo con el que accedemos
 */
int Usuario::getCantidadAmigos() const
{
    return cantidadAmigos;
}

Usuario* Usuario::getAmigo(int indice) const
{
    if (indice < 0 || indice >= cantidadAmigos)
    {
        return nullptr;
    }

    return amigos[indice];
}

bool Usuario::agregarMensajeEnviado(Mensajes* mensaje){
    if (mensaje == nullptr)
    {
        return false;
    }

    if (cantidadMensajesEnviados == capacidadMensajesEnviados){
        int nuevaCapacidad = (capacidadMensajesEnviados == 0)
        ? 2
        : capacidadMensajesEnviados * 2;

        Mensajes** nuevosMensajes = new (std::nothrow)Mensajes*[nuevaCapacidad];

        if (nuevosMensajes == nullptr) {
            return false;
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Mensajes*));

        for (int i = 0; i < cantidadMensajesEnviados; i++)
        {
            MedidorRecursos::registrarIteracion();
            nuevosMensajes[i] = mensajesEnviados[i];
        }

        MedidorRecursos::liberarMemoria(capacidadMensajesEnviados * sizeof(Mensajes*));

        delete[] mensajesEnviados;

        mensajesEnviados = nuevosMensajes;
        capacidadMensajesEnviados = nuevaCapacidad;
    }

    mensajesEnviados[cantidadMensajesEnviados] = mensaje;
    cantidadMensajesEnviados++;

    return true;
}

bool Usuario::agregarMensajeRecibido(Mensajes* mensaje)
{
    if (mensaje == nullptr)
    {
        return false;
    }

    if (cantidadMensajesRecibidos == capacidadMensajesRecibidos)
    {
        int nuevaCapacidad = (capacidadMensajesRecibidos == 0)
        ? 2
        : capacidadMensajesRecibidos * 2;

        Mensajes** nuevosMensajes = new (std::nothrow)Mensajes*[nuevaCapacidad];

        if (nuevosMensajes == nullptr) {
            return false;
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Mensajes*));

        for (int i = 0; i < cantidadMensajesRecibidos; i++)
        {
            MedidorRecursos::registrarIteracion();
            nuevosMensajes[i] = mensajesRecibidos[i];
        }

        MedidorRecursos::liberarMemoria(capacidadMensajesRecibidos * sizeof(Mensajes*));

        delete[] mensajesRecibidos;

        mensajesRecibidos = nuevosMensajes;
        capacidadMensajesRecibidos = nuevaCapacidad;
    }

    mensajesRecibidos[cantidadMensajesRecibidos] = mensaje;
    cantidadMensajesRecibidos++;

    return true;
}

Mensajes* Usuario::getMensajeRecibido(int indice) const
{
    if (indice < 0 || indice >= cantidadMensajesRecibidos)
    {
        return nullptr;
    }

    return mensajesRecibidos[indice];
}

bool Usuario::marcarMensajeComoLeido(int indice){
    Mensajes* mensaje = getMensajeRecibido(indice);

    if (mensaje == nullptr)
    {
        return false;
    }

    mensaje->marcarComoLeido();
    return true;
}

int Usuario::getCantidadMensajesEnviados() const
{
    return cantidadMensajesEnviados;
}

int Usuario::getCantidadMensajesRecibidos() const
{
    return cantidadMensajesRecibidos;
}

bool Usuario::registrarMensajeEnviado(Mensajes* mensaje)
{
    return agregarMensajeEnviado(mensaje);
}

bool Usuario::agregarAlbum(Album* album)
{
    if (album == nullptr)
    {
        return false;
    }

    if (cantidadAlbumes == capacidadAlbumes)
    {
        int nuevaCapacidad = (capacidadAlbumes == 0)
        ? 2
        : capacidadAlbumes * 2;

        Album** nuevosAlbumes = new (std::nothrow) Album*[nuevaCapacidad];

        if(nuevosAlbumes == nullptr){
            return false;
        }

        MedidorRecursos::registrarMemoria(nuevaCapacidad * sizeof(Album*));

        for (int i = 0; i < cantidadAlbumes; i++)
        {
            MedidorRecursos::registrarIteracion();
            nuevosAlbumes[i] = albumes[i];
        }

        MedidorRecursos::liberarMemoria(capacidadAlbumes * sizeof(Album*));

        delete[] albumes;

        albumes = nuevosAlbumes;
        capacidadAlbumes = nuevaCapacidad;
    }

    albumes[cantidadAlbumes] = album;
    cantidadAlbumes++;

    return true;
}

Album* Usuario::getAlbum(int indice) const
{
    if (indice < 0 || indice >= cantidadAlbumes)
    {
        return nullptr;
    }

    return albumes[indice];
}

int Usuario::getCantidadAlbumes() const
{
    return cantidadAlbumes;
}

//Getters
const std::string& Usuario::getApodo() const
{
    return apodo;
}

const std::string& Usuario::getFechaDeNacimiento() const
{
    return fechaDeNacimiento;
}

const std::string& Usuario::getCorreo() const
{
    return correo;
}

const std::string& Usuario::getFotoDePerfil() const
{
    return fotoDePerfil;
}

const std::string& Usuario::getPais() const
{
    return pais;
}

const std::string& Usuario::getCiudad() const
{
    return ciudad;
}

const std::string& Usuario::getFechaDeInscripcion() const
{
    return fechaDeInscripcion;
}

//Setters
void Usuario::setApodo(const std::string& apodo)
{
    this->apodo = apodo;
}

void Usuario::setFechaDeNacimiento(const std::string& fechaDeNacimiento)
{
    this->fechaDeNacimiento = fechaDeNacimiento;
}

void Usuario::setCorreo(const std::string& correo)
{
    this->correo = correo;
}

void Usuario::setFotoDePerfil(const std::string& fotoDePerfil)
{
    this->fotoDePerfil = fotoDePerfil;
}

void Usuario::setPais(const std::string& pais)
{
    this->pais = pais;
}

void Usuario::setCiudad(const std::string& ciudad)
{
    this->ciudad = ciudad;
}

void Usuario::setFechaDeInscripcion(const std::string& fechaDeInscripcion)
{
    this->fechaDeInscripcion = fechaDeInscripcion;
}

/* libera el arreglo, pero no los Usuario* que contiene
 */
Usuario::~Usuario(){
    MedidorRecursos::liberarMemoria(capacidadAmigos * sizeof(Usuario*));
    delete[] amigos;

    MedidorRecursos::liberarMemoria(capacidadMensajesEnviados * sizeof(Mensajes*));
    delete[] mensajesEnviados;

    MedidorRecursos::liberarMemoria(capacidadMensajesRecibidos * sizeof(Mensajes*));
    delete[] mensajesRecibidos;

    MedidorRecursos::liberarMemoria(capacidadSolicitudes * sizeof(Solicitud*));
    delete[] solicitudAmistad;

    for (int i = 0; i < cantidadAlbumes; i++){
        delete albumes[i];
    }

    MedidorRecursos::liberarMemoria(capacidadAlbumes * sizeof(Album*));
    delete[] albumes;
}
/*Realiza una copia técnica de los datos del usuario y sus referencias
 */
Usuario::Usuario(const Usuario& otro)
    : apodo(otro.apodo),
    fechaDeNacimiento(otro.fechaDeNacimiento),
    correo(otro.correo),
    fotoDePerfil(otro.fotoDePerfil),
    pais(otro.pais),
    ciudad(otro.ciudad),
    fechaDeInscripcion(otro.fechaDeInscripcion),
    amigos(nullptr),
    cantidadAmigos(otro.cantidadAmigos),
    capacidadAmigos(otro.capacidadAmigos),
    mensajesEnviados(nullptr),
    cantidadMensajesEnviados(otro.cantidadMensajesEnviados),
    capacidadMensajesEnviados(otro.capacidadMensajesEnviados),
    mensajesRecibidos(nullptr),
    cantidadMensajesRecibidos(otro.cantidadMensajesRecibidos),
    capacidadMensajesRecibidos(otro.capacidadMensajesRecibidos),
    solicitudAmistad(nullptr),
    cantidadSolicitudes(otro.cantidadSolicitudes),
    capacidadSolicitudes(otro.capacidadSolicitudes),
    albumes(nullptr),
    cantidadAlbumes(otro.cantidadAlbumes),
    capacidadAlbumes(otro.capacidadAlbumes)
{
    try {
        if (capacidadAmigos > 0) {
            amigos = new Usuario*[capacidadAmigos];
            for (int i = 0; i < cantidadAmigos; ++i) {
                amigos[i] = otro.amigos[i];
            }
        }

        if (capacidadMensajesEnviados > 0) {
            mensajesEnviados = new Mensajes*[capacidadMensajesEnviados];
            for (int i = 0; i < cantidadMensajesEnviados; ++i) {
                mensajesEnviados[i] = otro.mensajesEnviados[i];
            }
        }

        if (capacidadMensajesRecibidos > 0) {
            mensajesRecibidos = new Mensajes*[capacidadMensajesRecibidos];
            for (int i = 0; i < cantidadMensajesRecibidos; ++i) {
                mensajesRecibidos[i] = otro.mensajesRecibidos[i];
            }
        }

        if (capacidadSolicitudes > 0) {
            solicitudAmistad = new Solicitud*[capacidadSolicitudes];
            for (int i = 0; i < cantidadSolicitudes; ++i) {
                solicitudAmistad[i] = otro.solicitudAmistad[i];
            }
        }

        if (capacidadAlbumes > 0) {
            albumes = new Album*[capacidadAlbumes];
            for (int i = 0; i < cantidadAlbumes; ++i) {
                albumes[i] = otro.albumes[i];
            }
        }
    } catch (...) {
        delete[] amigos;
        delete[] mensajesEnviados;
        delete[] mensajesRecibidos;
        delete[] solicitudAmistad;
        delete[] albumes;
        throw;
    }
}


/*Sobrecarga de operadores, metodo con el que almacenamos el nuevo usuario
 */
Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this == &otro) {
        return *this;
    }

    Usuario copia(otro);

    std::swap(apodo, copia.apodo);
    std::swap(fechaDeNacimiento, copia.fechaDeNacimiento);
    std::swap(correo, copia.correo);
    std::swap(fotoDePerfil, copia.fotoDePerfil);
    std::swap(pais, copia.pais);
    std::swap(ciudad, copia.ciudad);
    std::swap(fechaDeInscripcion, copia.fechaDeInscripcion);

    std::swap(amigos, copia.amigos);
    std::swap(cantidadAmigos, copia.cantidadAmigos);
    std::swap(capacidadAmigos, copia.capacidadAmigos);

    std::swap(mensajesEnviados, copia.mensajesEnviados);
    std::swap(cantidadMensajesEnviados, copia.cantidadMensajesEnviados);
    std::swap(capacidadMensajesEnviados, copia.capacidadMensajesEnviados);

    std::swap(mensajesRecibidos, copia.mensajesRecibidos);
    std::swap(cantidadMensajesRecibidos, copia.cantidadMensajesRecibidos);
    std::swap(capacidadMensajesRecibidos, copia.capacidadMensajesRecibidos);

    std::swap(solicitudAmistad, copia.solicitudAmistad);
    std::swap(cantidadSolicitudes, copia.cantidadSolicitudes);
    std::swap(capacidadSolicitudes, copia.capacidadSolicitudes);

    std::swap(albumes, copia.albumes);
    std::swap(cantidadAlbumes, copia.cantidadAlbumes);
    std::swap(capacidadAlbumes, copia.capacidadAlbumes);

    return *this;
}

void Usuario::quitarUltimoMensajeEnviado()
{
    if (cantidadMensajesEnviados > 0)
    {
        cantidadMensajesEnviados--;
        mensajesEnviados[cantidadMensajesEnviados] = nullptr;
    }
}

bool Usuario::agregarSolicitud(Solicitud* solicitud)
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

        Solicitud** nuevoArreglo = new (std::nothrow)Solicitud*[nuevaCapacidad];

        if (nuevoArreglo == nullptr) {
            return false;
        }

        for (int i = 0; i < cantidadSolicitudes; ++i)
        {
            MedidorRecursos::registrarIteracion();
            nuevoArreglo[i] = solicitudAmistad[i];
        }

        MedidorRecursos::registrarMemoria(
            nuevaCapacidad * sizeof(Solicitud*)
            );

        MedidorRecursos::liberarMemoria(
            capacidadSolicitudes * sizeof(Solicitud*)
            );

        delete[] solicitudAmistad;
        solicitudAmistad = nuevoArreglo;
        capacidadSolicitudes = nuevaCapacidad;
    }

    solicitudAmistad[cantidadSolicitudes] = solicitud;
    cantidadSolicitudes++;

    return true;
}

Solicitud* Usuario::getSolicitud(int indice) const
{
    if (indice < 0 || indice >= cantidadSolicitudes)
    {
        return nullptr;
    }

    return solicitudAmistad[indice];
}



