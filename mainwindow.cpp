#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushEntrar_clicked()
{
    QString login = ui->lineLogin->text();
    QString senha = ui->linePassword->text();

    if (gerenciador.autenticarUsuario(login, senha)) {
        ui->stackedWidget->setCurrentIndex(2); // ir para a tela principal
    } else {
        QMessageBox::warning(this, "Erro de login", "Login ou senha incorretos.");
    }
}

void MainWindow::on_pushCadastrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); // ir para a tela de cadastro
}

void MainWindow::on_pushEnviar_clicked()
{
    QString nome = ui->lineNome->text();
    QString login = ui->lineLoginCadastro->text();
    QString senha = ui->lineSenha->text();
    QString confirmar = ui->lineConfirmarSenha->text();

    if (nome.isEmpty() || login.isEmpty() || senha.isEmpty() || confirmar.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Todos os campos devem ser preenchidos.");
        return;
    }

    if (senha != confirmar) {
        QMessageBox::warning(this, "Erro", "As senhas não coincidem.");
        return;
    }

    if (!gerenciador.registrarUsuario(nome, login, senha)) {
        QMessageBox::warning(this, "Erro", "Usuário já existe.");
        return;
    }

    QMessageBox::information(this, "Sucesso", "Usuário cadastrado com sucesso!");

    // Limpar os campos após o cadastro
    ui->lineNome->clear();
    ui->lineLoginCadastro->clear();
    ui->lineSenha->clear();
    ui->lineConfirmarSenha->clear();

    ui->stackedWidget->setCurrentIndex(0); // voltar para tela de login
}

void MainWindow::on_pushVoltar_clicked()
{
    // Limpar os campos ao voltar
    ui->lineNome->clear();
    ui->lineLoginCadastro->clear();
    ui->lineSenha->clear();
    ui->lineConfirmarSenha->clear();

    ui->stackedWidget->setCurrentIndex(0); // voltar para tela de login
}
