#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gerenciador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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


private:
    Ui::MainWindow *ui;
    Gerenciador gerenciador; // instância do gerenciador
};
#endif // MAINWINDOW_H
