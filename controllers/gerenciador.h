#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <vector>
#include "../models/usuario.h"

class Gerenciador {
public:
    bool registrarUsuario(const QString& nome, const QString& login, const QString& senha);
    bool autenticarUsuario(const QString& login, const QString& senha);
    Usuario* getUsuarioLogado();

private:
    std::vector<Usuario> usuarios;
    Usuario* usuarioLogado = nullptr;
};

#endif // GERENCIADOR_H
