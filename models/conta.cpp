#include "conta.h"

Conta::Conta(const QString& nome) : nome(nome) {}

void Conta::adicionarTransacao(const Transacao& transacao) {
    transacoes.append(transacao);
}

double Conta::calcularSaldo() const {
    double saldo = 0.0;
    for (const Transacao& t : transacoes)
        saldo += t.getValor();
    return saldo;
}

QString Conta::getNome() const {
    return nome;
}
