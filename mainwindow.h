#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include "../controllers/gerenciador.h"
#include "../models/despesa.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Classe principal da interface do usuário da aplicação de controle financeiro
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);  // Construtor principal
    ~MainWindow();                                   // Destrutor

private slots:
    // Slots conectados a ações da interface (botões e campos)
    void on_pushEntrar_clicked();        // Ação: login do usuário
    void on_pushCadastrar_clicked();     // Ação: ir para tela de cadastro
    void on_pushEnviar_clicked();        // Ação: registrar novo usuário
    void on_pushVoltar_clicked();        // Ação: voltar para a tela de login
    void on_pushConfirmar_clicked();     // Ação: confirmar receita mensal
    void on_pushAdicionar_clicked();     // Ação: adicionar uma nova despesa
    void on_pushLimpar_clicked();        // Ação: remover todas as despesas do mês
    void on_pushSair_clicked();          // Ação: logout do sistema
    void on_mesOuAnoAlterado();          // NOVO: responde à mudança de mês ou ano

private:
    Ui::MainWindow *ui;                  // Interface gerada pelo Qt Designer
    Gerenciador gerenciador;            // Controlador de lógica de negócio e banco

    // Estado atual da aplicação
    double receitaMensal = 0.0;          // Receita do mês selecionado
    int mesSelecionado = 0;              // Mês selecionado (índice de 0 a 11)
    int anoSelecionado = 0;              // Ano selecionado

    QList<Despesa> listaDespesas;        // Lista de despesas do mês/ano atual

    // Funções auxiliares para a interface
    void atualizarTabelaDespesas();      // Atualiza visualmente a tabela de despesas
    void atualizarTotais();              // Atualiza valores e porcentagens do resumo
    void limparCamposDespesa();          // Limpa inputs de valor e categoria
    void carregarNomeUsuario();          // Mostra saudação com nome do usuário
    void configurarLogo(QLabel* label);  // Insere logo ASCII estilizado
};

#endif // MAINWINDOW_H
