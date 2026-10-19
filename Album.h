#ifndef ALBUM_H
#define ALBUM_H

#include "Foto.h"
#include <string>

class Usuario;

enum class Privacidad
{
    Publico,
    Privado,
    Amigos
};

class Album
{
private:
    std::string nombre;
    std::string ruta;
    Privacidad privacidad;

    Foto** fotos;
    int cantidadFotos;
    int capacidadFotos;
public:
    Album(const std::string& nombre,
          const std::string& ruta,
          Privacidad privacidad = Privacidad::Publico);
    ~Album();
    const std::string& getNombre() const;
    const std::string& getRuta() const;
    Privacidad getPrivacidad() const;

    bool agregarFoto(Foto* foto);
    int getCantidadFotos() const;
    Foto* getFoto(int indice) const;

    bool puedeVer(const Usuario* propietario,const Usuario* visitante) const;

};

#endif // ALBUM_H
