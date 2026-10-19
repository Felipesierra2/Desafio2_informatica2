cmake_minimum_required(VERSION 3.16)

project(Desafio_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Desafio_2 main.cpp
    Usuario.h Usuario.cpp
    UdeABook.h UdeABook.cpp
    Mensajes.h Mensajes.cpp
    Solicitud.h Solicitud.cpp
    Album.h Album.cpp
    Foto.h Foto.cpp
    README.md
    .gitignore
    RedSocial.h RedSocial.cpp
    MedidorRecursos.h MedidorRecursos.cpp
    Archivo.h Archivo.cpp

)

include(GNUInstallDirs)
install(TARGETS Desafio_2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
