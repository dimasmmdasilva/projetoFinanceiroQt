#include "gerenciador.h"

// ==========================================================
// === Construtor ===========================================
// ==========================================================

// Inicializa o banco de dados ao instanciar o gerenciador
Gerenciador::Gerenciador() {
    banco.inicializar(); // Cria tabelas e abre conexão, se necessário
}

// ==========================================================
// === Sessão do Usuário ====================================
// ==========================================================

// Registra um novo usuário no banco (verifica se o login já existe)
bool Gerenciador::registrarUsuario(const QString& nome, const QString& login, const QString& senha) {
    if (banco.usuarioExiste(login)) {
        return false; // Impede duplicidade de logins
    }
    return banco.inserirUsuario(nome, login, senha);
}

// Autentica o usuário com login e senha
// Se sucesso, armazena sessão ativa
bool Gerenciador::autenticarUsuario(const QString& login, const QString& senha) {
    Usuario temp;

    if (banco.autenticarUsuario(login, senha, temp)) {
        usuarioLogado = temp;
        logado = true;
        return true;
    }

    logado = false;
    return false;
}

// Retorna ponteiro para o usuário logado, ou nullptr se não autenticado
Usuario* Gerenciador::getUsuarioLogado() {
    return logado ? &usuarioLogado : nullptr;
}

// Finaliza a sessão atual
void Gerenciador::logout() {
    logado = false;
}


// ==========================================================
// === Operações Financeiras ================================
// ==========================================================

// Salva ou atualiza receita mensal do usuário no banco
bool Gerenciador::salvarReceita(int usuarioId, int mes, int ano, double valor) {
    return banco.atualizarReceita(usuarioId, mes, ano, valor);
}

// Adiciona uma nova despesa ao banco de dados
bool Gerenciador::salvarDespesa(int usuarioId, int mes, int ano, const QString& categoria, double valor) {
    return banco.inserirDespesa(usuarioId, mes, ano, categoria, valor);
}

// Obtém a lista de despesas registradas para um mês e ano
QList<Despesa> Gerenciador::obterDespesas(int usuarioId, int mes, int ano) {
    return banco.listarDespesas(usuarioId, mes, ano);
}

// Remove todas as despesas para um dado mês e ano
bool Gerenciador::removerDespesas(int usuarioId, int mes, int ano) {
    return banco.removerDespesas(usuarioId, mes, ano);
}

// Obtém a receita registrada para um mês e ano específicos
double Gerenciador::obterReceita(int usuarioId, int mes, int ano) {
    return banco.obterReceita(usuarioId, mes, ano);
}
