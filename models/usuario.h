#ifndef USUARIO_H
#define USUARIO_H

#include <QString>
#include <vector>
#include "conta.h"

class Usuario {
public:
    Usuario(const QString& nome, const QString& login, const QString& senha);

    QString getNome() const;
    QString getLogin() const;
    bool verificarSenha(const QString& tentativa) const;

    void adicionarConta(const Conta& conta);
    const std::vector<Conta>& getContas() const;

private:
    QString nome;
    QString login;
    QString senha; // OBS: versão simples, idealmente deve ser hash
    std::vector<Conta> contas;
};

#endif // USUARIO_H
