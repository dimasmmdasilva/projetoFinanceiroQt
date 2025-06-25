#include "gerenciador.h"

bool Gerenciador::registrarUsuario(const QString& nome, const QString& login, const QString& senha) {
    // Verifica se já existe usuário com esse login
    for (const Usuario& u : usuarios) {
        if (u.getLogin() == login)
            return false; // já existe
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

Usuario* Gerenciador::getUsuarioLogado() {
    return usuarioLogado;
}
