#include "gerenciador.h"

bool Gerenciador::registrarUsuario(const QString& nome, const QString& login, const QString& senha) {
    for (const Usuario& u : usuarios) {
        if (u.getLogin() == login)
            return false; // usuário já existe
    }

    usuarios.emplace_back(nome, login, senha);
    return true;
}

bool Gerenciador::autenticarUsuario(const QString& login, const QString& senha) {
    for (Usuario& u : usuarios) {
        if (u.getLogin() == login && u.verificarSenha(senha)) {
            usuarioLogado = &u;
            return true;
        }
    }
    return false;
}

Usuario* Gerenciador::getUsuarioLogado() const {
    return usuarioLogado;
}
