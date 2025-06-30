#include "usuario.h"

// Construtor utilizado no cadastro de novo usuário
Usuario::Usuario(const QString& nome, const QString& login, const QString& senha)
    : nome(nome), login(login), senha(senha) {}

// Getter para o ID
int Usuario::getId() const {
    return id;
}

// Getter para o nome
QString Usuario::getNome() const {
    return nome;
}

// Getter para o login
QString Usuario::getLogin() const {
    return login;
}

// Getter para a senha
QString Usuario::getSenha() const {
    return senha;
}

// Verifica se a senha fornecida corresponde à armazenada
bool Usuario::verificarSenha(const QString& tentativa) const {
    return senha == tentativa;
}

// Setter para o ID (utilizado ao autenticar o usuário)
void Usuario::setId(int novoId) {
    id = novoId;
}

// Setter para o nome
void Usuario::setNome(const QString& novoNome) {
    nome = novoNome;
}

// Setter para o login
void Usuario::setLogin(const QString& novoLogin) {
    login = novoLogin;
}

// Setter para a senha
void Usuario::setSenha(const QString& novaSenha) {
    senha = novaSenha;
}

// Adiciona uma nova conta à lista de contas
void Usuario::adicionarConta(const Conta& conta) {
    contas.push_back(conta);
}

// Retorna todas as contas associadas ao usuário
const std::vector<Conta>& Usuario::getContas() const {
    return contas;
}
