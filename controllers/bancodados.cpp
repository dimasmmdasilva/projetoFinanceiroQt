#include "bancodados.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

// Construtor padrão
BancoDados::BancoDados() {}

// Inicializa a conexão com o banco SQLite e cria as tabelas necessárias
bool BancoDados::inicializar() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cf1.0.db");

    if (!db.open()) {
        qDebug() << "[ERRO] Falha ao abrir banco de dados:" << db.lastError().text();
        return false;
    }

    return criarTabelasFinanceiras();
}

// Cria tabelas de usuários, receitas e despesas
bool BancoDados::criarTabelasFinanceiras() {
    QSqlQuery query;

    // Tabela de usuários
    const QString sqlUsuarios = R"(
        CREATE TABLE IF NOT EXISTS usuarios (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nome TEXT NOT NULL,
            login TEXT UNIQUE NOT NULL,
            senha_hash TEXT NOT NULL
        )
    )";

    if (!query.exec(sqlUsuarios)) {
        qDebug() << "[ERRO] Falha ao criar tabela usuarios:" << query.lastError().text();
        return false;
    }

    // Tabela de receitas
    const QString sqlReceitas = R"(
        CREATE TABLE IF NOT EXISTS receitas (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            usuario_id INTEGER NOT NULL,
            mes INTEGER NOT NULL,
            ano INTEGER NOT NULL,
            valor REAL NOT NULL,
            UNIQUE(usuario_id, mes, ano),
            FOREIGN KEY (usuario_id) REFERENCES usuarios(id)
        )
    )";

    if (!query.exec(sqlReceitas)) {
        qDebug() << "[ERRO] Falha ao criar tabela receitas:" << query.lastError().text();
        return false;
    }

    // Tabela de despesas
    const QString sqlDespesas = R"(
        CREATE TABLE IF NOT EXISTS despesas (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            usuario_id INTEGER NOT NULL,
            mes INTEGER NOT NULL,
            ano INTEGER NOT NULL,
            categoria TEXT NOT NULL,
            valor REAL NOT NULL,
            FOREIGN KEY (usuario_id) REFERENCES usuarios(id)
        )
    )";

    if (!query.exec(sqlDespesas)) {
        qDebug() << "[ERRO] Falha ao criar tabela despesas:" << query.lastError().text();
        return false;
    }

    return true;
}

// Insere novo usuário no banco (com hash SHA-256 da senha)
bool BancoDados::inserirUsuario(const QString& nome, const QString& login, const QString& senha) {
    if (usuarioExiste(login))
        return false;

    QString senhaHash = QString(QCryptographicHash::hash(senha.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    query.prepare("INSERT INTO usuarios (nome, login, senha_hash) VALUES (?, ?, ?)");
    query.addBindValue(nome);
    query.addBindValue(login);
    query.addBindValue(senhaHash);

    if (!query.exec()) {
        qDebug() << "[ERRO] Falha ao inserir usuário:" << query.lastError().text();
        return false;
    }

    return true;
}

// Verifica se login já existe
bool BancoDados::usuarioExiste(const QString& login) {
    QSqlQuery query;
    query.prepare("SELECT id FROM usuarios WHERE login = ?");
    query.addBindValue(login);

    return query.exec() && query.next();
}

// Autentica usuário (verifica login + hash da senha) e preenche struct Usuario
bool BancoDados::autenticarUsuario(const QString& login, const QString& senha, Usuario& usuario) {
    QString senhaHash = QString(QCryptographicHash::hash(senha.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    query.prepare("SELECT id, nome FROM usuarios WHERE login = ? AND senha_hash = ?");
    query.addBindValue(login);
    query.addBindValue(senhaHash);

    if (query.exec() && query.next()) {
        usuario.setId(query.value(0).toInt());
        usuario.setNome(query.value(1).toString());
        usuario.setLogin(login);
        usuario.setSenha(senha); // Apenas para memória temporária
        return true;
    }

    return false;
}

// Atualiza ou insere receita mensal do usuário
bool BancoDados::atualizarReceita(int usuarioId, int mes, int ano, double valor) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO receitas (usuario_id, mes, ano, valor)
        VALUES (?, ?, ?, ?)
        ON CONFLICT(usuario_id, mes, ano)
        DO UPDATE SET valor = excluded.valor
    )");

    query.addBindValue(usuarioId);
    query.addBindValue(mes);
    query.addBindValue(ano);
    query.addBindValue(valor);

    if (!query.exec()) {
        qDebug() << "[ERRO] Falha ao atualizar receita:" << query.lastError().text();
        return false;
    }

    return true;
}

// Insere uma nova despesa
bool BancoDados::inserirDespesa(int usuarioId, int mes, int ano, const QString& categoria, double valor) {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO despesas (usuario_id, mes, ano, categoria, valor)
        VALUES (?, ?, ?, ?, ?)
    )");

    query.addBindValue(usuarioId);
    query.addBindValue(mes);
    query.addBindValue(ano);
    query.addBindValue(categoria);
    query.addBindValue(valor);

    if (!query.exec()) {
        qDebug() << "[ERRO] Falha ao inserir despesa:" << query.lastError().text();
        return false;
    }

    return true;
}

// Lista todas as despesas para um usuário em um período
QList<Despesa> BancoDados::listarDespesas(int usuarioId, int mes, int ano) {
    QList<Despesa> lista;

    QSqlQuery query;
    query.prepare("SELECT categoria, valor FROM despesas WHERE usuario_id = ? AND mes = ? AND ano = ?");
    query.addBindValue(usuarioId);
    query.addBindValue(mes);
    query.addBindValue(ano);

    if (query.exec()) {
        while (query.next()) {
            Despesa d;
            d.categoria = query.value(0).toString();
            d.valor = query.value(1).toDouble();
            lista.append(d);
        }
    } else {
        qDebug() << "[ERRO] Falha ao listar despesas:" << query.lastError().text();
    }

    return lista;
}

// Obtém a receita salva para um mês
double BancoDados::obterReceita(int usuarioId, int mes, int ano) {
    QSqlQuery query;
    query.prepare("SELECT valor FROM receitas WHERE usuario_id = ? AND mes = ? AND ano = ?");
    query.addBindValue(usuarioId);
    query.addBindValue(mes);
    query.addBindValue(ano);

    if (query.exec() && query.next())
        return query.value(0).toDouble();

    return 0.0;
}

// Remove todas as despesas do mês/ano do usuário (usado por pushLimpar)
bool BancoDados::removerDespesas(int usuarioId, int mes, int ano) {
    QSqlQuery query;
    query.prepare("DELETE FROM despesas WHERE usuario_id = ? AND mes = ? AND ano = ?");
    query.addBindValue(usuarioId);
    query.addBindValue(mes);
    query.addBindValue(ano);

    if (!query.exec()) {
        qDebug() << "[ERRO] Falha ao remover despesas:" << query.lastError().text();
        return false;
    }

    return true;
}
