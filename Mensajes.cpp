#include "Mensajes.h"

Mensajes::Mensajes(Usuario* remitente,
                   Usuario* destinatario,
                   const std::string& contenido,
                   const std::string& fecha)
    : remitente(remitente),
    destinatario(destinatario),
    contenido(contenido),
    fecha(fecha),
    leido(false)
{
}

Usuario* Mensajes::getRemitente() const
{
    return remitente;
}

Usuario* Mensajes::getDestinatario() const
{
    return destinatario;
}

const std::string& Mensajes::getContenido() const
{
    return contenido;
}

const std::string& Mensajes::getFecha() const
{
    return fecha;
}

bool Mensajes::getLeido() const
{
    return leido;
}

void Mensajes::marcarComoLeido()
{
    leido = true;
}



