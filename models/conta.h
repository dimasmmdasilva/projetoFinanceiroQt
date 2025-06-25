#ifndef CONTA_H
#define CONTA_H

#include <QString>
#include <QVector>
#include "transacao.h"

class Conta {
public:
    Conta(const QString& nome);

    void adicionarTransacao(const Transacao& transacao);
    double calcularSaldo() const;
    QString getNome() const;

private:
    QString nome;
    QVector<Transacao> transacoes;
};

#endif // CONTA_H
