#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <vector>
#include "../models/usuario.h"

class Gerenciador {
public:
    // Registra um novo usuário, se ainda não existir com o mesmo login
    bool registrarUsuario(const QString& nome, const QString& login, const QString& senha);

    // Autentica usuário e armazena ponteiro em usuarioLogado
    bool autenticarUsuario(const QString& login, const QString& senha);

    // Retorna o ponteiro do usuário autenticado
    Usuario* getUsuarioLogado() const;

    // Finaliza a sessão atual
    void logout();

private:
    std::vector<Usuario> usuarios;
    Usuario* usuarioLogado = nullptr;
};

#endif // GERENCIADOR_H
