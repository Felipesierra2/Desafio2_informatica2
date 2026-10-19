#include "MedidorRecursos.h"
#include "RedSocial.h"
#include "Usuario.h"
#include "Mensajes.h"
#include "UdeABook.h"
#include <iostream>

using namespace std;

int main()
{
    RedSocial redSocial;

    Usuario* felipe = new Usuario(
        "Felipe",
        "2000-05-15",
        "felipe@udea.edu.co",
        "fotos/felipe.jpg",
        "Colombia",
        "Medellin",
        "2026-10-18"
        );

    Usuario* carlos = new Usuario(
        "Carlos",
        "2001-08-20",
        "carlos@udea.edu.co",
        "fotos/carlos.jpg",
        "Colombia",
        "Bogota",
        "2026-10-18"
        );

    bool felipeRegistrado = redSocial.registrarUsuario(felipe);
    bool carlosRegistrado = redSocial.registrarUsuario(carlos);

    if (!felipeRegistrado || !carlosRegistrado)
    {
        std::cerr << "Error al registrar los usuarios de prueba.\n";

        if (!felipeRegistrado)
            delete felipe;

        if (!carlosRegistrado)
            delete carlos;

        return 1;
    }

    felipe->agregarAmigo(carlos);
    carlos->agregarAmigo(felipe);

    UdeABook aplicacion(redSocial);
    aplicacion.ejecutar();
    return 0;
}

