#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <string>
#include "RedSocial.h"

class Archivo
{
public:
    static bool cargarUsuarios(
        RedSocial& redSocial,
        const std::string& nombreArchivo);

    static bool guardarUsuarios(
        const RedSocial& redSocial,
        const std::string& nombreArchivo);

    static bool cargarAmistades(RedSocial& redSocial,
                                const std::string& nombreArchivo);

    static bool guardarAmistades(const RedSocial& redSocial,
                                 const std::string& nombreArchivo);

    static bool guardarSolicitudesPendientes(
        const RedSocial& redSocial,
        const std::string& nombreArchivo);

    static bool cargarSolicitudesPendientes(
        RedSocial& redSocial,
        const std::string& nombreArchivo);

    static bool guardarMensajes(const RedSocial& redSocial,
                                const std::string& nombreArchivo);

    static bool cargarMensajes(RedSocial& redSocial,
                               const std::string& nombreArchivo);

    static bool guardarAlbumes(const RedSocial& redSocial,
                               const std::string& nombreArchivo);

    static bool cargarAlbumes(RedSocial& redSocial,
                              const std::string& nombreArchivo);

    static bool guardarFotos(const RedSocial& redSocial,
                             const std::string& nombreArchivo);

    static bool cargarFotos(RedSocial& redSocial,
                            const std::string& nombreArchivo);

    static bool guardarLikes(const RedSocial& redSocial,
                             const std::string& nombreArchivo);

    static bool cargarLikes(RedSocial& redSocial,
                            const std::string& nombreArchivo);
};

#endif
