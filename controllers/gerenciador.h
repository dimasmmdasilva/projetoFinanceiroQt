#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <QString>
#include <QList>
#include "bancodados.h"
#include "../models/usuario.h"
#include "../models/despesa.h"

// Classe responsável por gerenciar a lógica da aplicação:
// - Cadastro e login de usuários
// - Controle da sessão do usuário logado
// - Encaminhamento das operações financeiras ao BancoDados
class Gerenciador {
public:
    // Construtor: inicializa banco e cria tabelas se necessário
    Gerenciador();

    // ===============================
    // === Autenticação e Cadastro ===
    // ===============================

    // Registra um novo usuário (com nome, login e senha)
    bool registrarUsuario(const QString& nome, const QString& login, const QString& senha);

    // Autentica o usuário e mantém sessão ativa
    bool autenticarUsuario(const QString& login, const QString& senha);

    // =========================
    // === Sessão do Usuário ===
    // =========================

    // Retorna ponteiro para o usuário logado, ou nullptr se não houver
    Usuario* getUsuarioLogado();

    // Encerra a sessão atual (logout)
    void logout();

    // =============================
    // === Operações Financeiras ===
    // =============================

    // Insere ou atualiza receita do mês para o usuário
    bool salvarReceita(int usuarioId, int mes, int ano, double valor);

    // Insere uma nova despesa
    bool salvarDespesa(int usuarioId, int mes, int ano, const QString& categoria, double valor);

    // Recupera todas as despesas registradas para um período
    QList<Despesa> obterDespesas(int usuarioId, int mes, int ano);

    // Remove todas as despesas do mês e ano especificados
    bool removerDespesas(int usuarioId, int mes, int ano);

    // Recupera a receita salva para um mês/ano
    double obterReceita(int usuarioId, int mes, int ano);  // NOVO MÉTODO

private:
    BancoDados banco;          // Interface para acesso ao banco SQLite
    Usuario usuarioLogado;     // Representa o usuário autenticado
    bool logado = false;       // Flag de controle de sessão
};

#endif // GERENCIADOR_H
