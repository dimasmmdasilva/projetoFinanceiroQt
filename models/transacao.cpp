#include "transacao.h"

Transacao::Transacao() : descricao(""), valor(0.0), data(QDate::currentDate()) {}

Transacao::Transacao(const QString& descricao, double valor, const QDate& data)
    : descricao(descricao), valor(valor), data(data) {}

QString Transacao::getDescricao() const {
    return descricao;
}

double Transacao::getValor() const {
    return valor;
}

QDate Transacao::getData() const {
    return data;
}
