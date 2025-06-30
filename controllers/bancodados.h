#ifndef BANCODADOS_H
#define BANCODADOS_H

#include <QString>
#include <QSqlDatabase>
#include <QList>
#include "../models/usuario.h"
#include "../models/despesa.h"

// Classe que encapsula toda a comunicação com o banco SQLite
class BancoDados {
public:
    BancoDados();

    // Conecta ao banco e cria tabelas se necessário
    bool inicializar();

    // === Operações com Usuários ===
    bool inserirUsuario(const QString& nome, const QString& login, const QString& senha);
    bool usuarioExiste(const QString& login);
    bool autenticarUsuario(const QString& login, const QString& senha, Usuario& usuario);

    // === Operações com Receita ===
    bool inserirReceita(int usuarioId, int mes, int ano, double valor);                // Receita nova
    bool atualizarReceita(int usuarioId, int mes, int ano, double valor);             // Atualiza valor
    double obterReceita(int usuarioId, int mes, int ano);                             // Busca receita

    // === Operações com Despesas ===
    bool inserirDespesa(int usuarioId, int mes, int ano, const QString& categoria, double valor);
    QList<Despesa> listarDespesas(int usuarioId, int mes, int ano);                   // Carrega lista
    bool removerDespesas(int usuarioId, int mes, int ano);                            // Apaga todas

private:
    QSqlDatabase db;

    // Cria tabelas: usuarios, receitas e despesas
    bool criarTabelasFinanceiras();
};

#endif // BANCODADOS_H
