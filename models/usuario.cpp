#include "usuario.h"

Usuario::Usuario(const QString& nome, const QString& login, const QString& senha)
    : nome(nome), login(login), senha(senha) {}

QString Usuario::getNome() const {
    return nome;
}

QString Usuario::getLogin() const {
    return login;
}

bool Usuario::verificarSenha(const QString& tentativa) const {
    return senha == tentativa;
}

void Usuario::adicionarConta(const Conta& conta) {
    contas.push_back(conta);
}

const std::vector<Conta>& Usuario::getContas() const {
    return contas;
}
