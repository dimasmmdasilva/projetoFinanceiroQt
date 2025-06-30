#ifndef USUARIO_H
#define USUARIO_H

#include <QString>
#include <vector>
#include "conta.h"

class Usuario {
public:
    // Construtor padrão
    Usuario() = default;

    // Construtor com parâmetros
    Usuario(const QString& nome, const QString& login, const QString& senha);

    // Getters
    int getId() const;
    QString getNome() const;
    QString getLogin() const;
    QString getSenha() const;
    bool verificarSenha(const QString& tentativa) const;

    // Setters
    void setId(int novoId);
    void setNome(const QString& novoNome);
    void setLogin(const QString& novoLogin);
    void setSenha(const QString& novaSenha);

    // Gerenciamento de contas
    void adicionarConta(const Conta& conta);
    const std::vector<Conta>& getContas() const;

private:
    int id = -1; // -1 por padrão, será definido após autenticação
    QString nome;
    QString login;
    QString senha;
    std::vector<Conta> contas;
};

#endif // USUARIO_H
