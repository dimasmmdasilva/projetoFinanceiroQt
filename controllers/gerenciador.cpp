#include "gerenciador.h"

bool Gerenciador::registrarUsuario(const QString& nome, const QString& login, const QString& senha) {
    // Verifica se já existe usuário com o mesmo login
    for (const Usuario& u : usuarios) {
        if (u.getLogin() == login)
            return false; // Usuário já existe
    }

    // Adiciona novo usuário
    usuarios.emplace_back(nome, login, senha);
    return true;
}

bool Gerenciador::autenticarUsuario(const QString& login, const QString& senha) {
    // Verifica se login e senha correspondem a algum usuário existente
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

void Gerenciador::logout() {
    // Encerra a sessão atual
    usuarioLogado = nullptr;
}
