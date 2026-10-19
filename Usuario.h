#ifndef USUARIO_H
#define USUARIO_H

#include "Album.h"
#include "Mensajes.h"
#include "Solicitud.h"
#include <string>

class Usuario
{
private:
    std::string apodo;
    std::string fechaDeNacimiento;
    std::string correo;
    std::string fotoDePerfil;
    std::string pais;
    std::string ciudad;
    std::string fechaDeInscripcion;

    Usuario** amigos;
    int cantidadAmigos;
    int capacidadAmigos;

    Mensajes** mensajesEnviados;
    int cantidadMensajesEnviados;
    int capacidadMensajesEnviados;

    Mensajes** mensajesRecibidos;
    int cantidadMensajesRecibidos;
    int capacidadMensajesRecibidos;

    Solicitud** solicitudAmistad;
    int cantidadSolicitudes;
    int capacidadSolicitudes;

    Album** albumes;
    int cantidadAlbumes;
    int capacidadAlbumes;

public:
    Usuario();
    Usuario(const std::string& apodo,
            const std::string& fechaDeNacimiento,
            const std::string& correo,
            const std::string& fotoDePerfil,
            const std::string& pais,
            const std::string& ciudad,
            const std::string& fechaDeInscripcion);

    ~Usuario();
    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario& otro);

    bool esAmigo(const Usuario* usuario) const;
    bool agregarAmigo(Usuario* usuario);
    bool agregarSolicitud(Solicitud* solicitud);
    bool quitarAmigo(Usuario* usuario);
    Solicitud* getSolicitud(int indice) const;
    int getCantidadAmigos() const;
    Usuario* getAmigo(int indice) const;

    bool agregarMensajeEnviado(Mensajes* mensaje);
    bool agregarMensajeRecibido(Mensajes* mensaje);
    Mensajes* getMensajeRecibido(int indice) const;
    bool marcarMensajeComoLeido(int indice);
    int getCantidadMensajesEnviados() const;
    int getCantidadMensajesRecibidos() const;
    bool registrarMensajeEnviado(Mensajes* mensaje);
    void quitarUltimoMensajeEnviado();

    bool agregarAlbum(Album* album);
    Album* getAlbum(int indice) const;
    int getCantidadAlbumes() const;

    //getters
    const std::string& getApodo() const;
    const std::string& getFechaDeNacimiento() const;
    const std::string& getCorreo() const;
    const std::string& getFotoDePerfil() const;
    const std::string& getPais() const;
    const std::string& getCiudad() const;
    const std::string& getFechaDeInscripcion() const;

    //Setters
    void setApodo(const std::string& apodo);
    void setFechaDeNacimiento(const std::string& fechaDeNacimiento);
    void setCorreo(const std::string& correo);
    void setFotoDePerfil(const std::string& fotoDePerfil);
    void setPais(const std::string& pais);
    void setCiudad(const std::string& ciudad);
    void setFechaDeInscripcion(const std::string& fechaDeInscripcion);
};


#endif // USUARIO_H
