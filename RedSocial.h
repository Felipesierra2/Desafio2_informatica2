#ifndef REDSOCIAL_H
#define REDSOCIAL_H
#include "Mensajes.h"
#include "Solicitud.h"
#include <string>
#include <new>

class Usuario;

class RedSocial
{
private:
    Usuario** usuarios;
    int cantidadUsuarios;
    int capacidadUsuarios;

    Mensajes** mensajes;
    int cantidadMensajes;
    int capacidadMensajes;

    Solicitud** solicitudes;
    int cantidadSolicitudes;
    int capacidadSolicitudes;

    bool agregarSolicitudAdministrada(Solicitud* solicitud);
    bool administraSolicitud(const Solicitud* solicitud) const;
    bool agregarMensajeAdministrado(Mensajes* mensaje);
    int contarAmigosEnComun(const Usuario* usuario,const Usuario* candidato) const;
    bool tieneSolicitudPendiente(const Usuario* remitente,const Usuario* destinatario) const;
    int contarFotosVisiblesAmigos(Usuario* usuario) const;
    std::string rutasUltimasFotos[3];
    int cantidadUltimasFotos;

public:
    RedSocial();
    ~RedSocial();

    Usuario* buscarUsuario(const std::string& apodo) const;
    bool registrarUsuario(Usuario* usuario);
    bool enviarMensaje(Usuario* remitente,Usuario* destinatario,const std::string& contenido,
        const std::string& fecha);
    void buscarMensajesPorFecha(
        Usuario* usuario,
        const std::string& fechaInicio,
        const std::string& fechaFin) const;
    void mostrarMensajesRecibidos(Usuario* usuario);
    void mostrarMensajesNoLeidosOrdenados(Usuario* usuario);
    bool enviarSolicitud(Usuario* remitente, Usuario* destinatario);
    bool aceptarSolicitud(Solicitud* solicitud);
    bool rechazarSolicitud(Solicitud* solicitud);
    bool puedeVerAlbum(const std::string& apodoPropietario,
                       const std::string& apodoVisitante,
                       const std::string& nombreAlbum) const;
    void mostrarSugerenciasAmistad(Usuario* usuario, int k) const;
    void mostrarAlbumesVisibles(const std::string& apodoPropietario,
                                const std::string& apodoVisitante) const;
    void mostrarFotosAlbum(const std::string& apodoPropietario,
                           const std::string& apodoVisitante,
                           const std::string& nombreAlbum) const;
    void recorrerAlbum(const std::string& apodoPropietario,
                       const std::string& apodoVisitante,
                       const std::string& nombreAlbum);
    bool darLikeFoto(const std::string& apodoPropietario,
                     const std::string& apodoVisitante,
                     const std::string& nombreAlbum,
                     int indiceFoto);
    bool quitarLikeFoto(const std::string& apodoPropietario,
                        const std::string& apodoVisitante,
                        const std::string& nombreAlbum,
                        int indiceFoto);
    bool agregarFotoAlbum(const std::string& apodoPropietario,
                          const std::string& nombreAlbum,
                          Foto* foto);
    void visitarPerfil(const std::string& apodoPropietario,
                       const std::string& apodoVisitante);
    void mostrarSolicitudesPendientes(Usuario* usuario) const;
    void gestionarSolicitudesPendientes(Usuario* usuario);
    int getCantidadUsuarios() const;
    Usuario* getUsuario(int indice) const;

    Usuario* iniciarSesion(const std::string& apodo) const;
    void mostrarFotosAleatoriasAmigos(Usuario* usuario);

    int getCantidadSolicitudes() const;
    Solicitud* getSolicitud(int indice) const;

};

#endif // REDSOCIAL_H
