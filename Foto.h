#ifndef FOTO_H
#define FOTO_H

#include <string>

class Usuario;

class Foto
{
private:
    std::string fechaDeCaptura;
    unsigned long long tam;
    std::string ruta;

    Usuario** usuariosQueDieronLike;
    int cantidadUsuariosLike;
    int capacidadUsuariosLike;

    int cantidadLikes;
public:
    Foto(const std::string& fechaDeCaptura,
         unsigned long long tam,
         const std::string& ruta);

    ~Foto();

    const std::string& getFechaCaptura() const;
    unsigned long long getTamano() const;
    const std::string& getRuta() const;
    int getCantidadLikes() const;

    bool darLike(Usuario* usuario);
    bool quitarLike(Usuario* usuario);
    void mostrar() const;
};

#endif
