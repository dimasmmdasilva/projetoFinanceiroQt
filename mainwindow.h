#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "gerenciador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Estrutura que representa uma despesa
struct Despesa {
    QString categoria;
    double valor;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushEntrar_clicked();
    void on_pushCadastrar_clicked();
    void on_pushEnviar_clicked();
    void on_pushVoltar_clicked();
    void on_pushConfirmar_clicked();
    void on_pushAdicionar_clicked();
    void on_pushSair_clicked();

private:
    Ui::MainWindow *ui;
    Gerenciador gerenciador;

    double receitaMensal = 0.0;
    std::vector<Despesa> listaDespesas;

    void atualizarTabelaDespesas();
    void atualizarTotais();
    void limparCamposDespesa();
};

#endif // MAINWINDOW_H
