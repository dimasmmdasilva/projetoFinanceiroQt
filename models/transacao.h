#ifndef TRANSACAO_H
#define TRANSACAO_H

#include <QString>
#include <QDate>

class Transacao {
public:
    Transacao();
    Transacao(const QString& descricao, double valor, const QDate& data);

    QString getDescricao() const;
    double getValor() const;
    QDate getData() const;

private:
    QString descricao;
    double valor;
    QDate data;
};

#endif // TRANSACAO_H
