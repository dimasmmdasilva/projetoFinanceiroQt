#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QTableWidgetItem>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configura o logo ASCII
    QString logo = R"(╔══════════════════════╗
║       CF 1.0         ║
║ Controle Financeiro  ║
╚══════════════════════╝)";

    ui->labelLogo->setText(logo);

    QFont fonte("Courier New");
    fonte.setStyleHint(QFont::Monospace);
    fonte.setPointSize(10);
    ui->labelLogo->setFont(fonte);
    ui->labelLogo->setAlignment(Qt::AlignCenter);
    ui->labelLogo->setWordWrap(true);

    // Aplica cor verde clara ao texto do logo
    ui->labelLogo->setStyleSheet("color: #90ee90;");

    // Configura o logo ASCII da tela de cadastro
    QString logoCadastro = R"(╔══════════════════════╗
║       CF 1.0         ║
║ Controle Financeiro  ║
╚══════════════════════╝)";

    QFont fonteLogo("Courier New");
    fonteLogo.setStyleHint(QFont::Monospace);
    fonteLogo.setPointSize(10);

    ui->labelLogoCadastro->setText(logoCadastro);
    ui->labelLogoCadastro->setFont(fonteLogo);
    ui->labelLogoCadastro->setAlignment(Qt::AlignCenter);
    ui->labelLogoCadastro->setWordWrap(true);
    ui->labelLogoCadastro->setStyleSheet("color: lightgreen;");


    // Configura a tabela de despesas
    ui->tableDespesas->setColumnCount(3);
    ui->tableDespesas->setHorizontalHeaderLabels({"Valor", "Categoria", "%"});
    ui->tableDespesas->horizontalHeader()->setStretchLastSection(true);
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
        Usuario* u = gerenciador.getUsuarioLogado();
        ui->labelSaudacao->setText("Olá, " + u->getNome());
        ui->stackedWidget->setCurrentIndex(2); // Vai para tela principal
    } else {
        QMessageBox::warning(this, "Erro de login", "Login ou senha incorretos.");
    }
}

void MainWindow::on_pushCadastrar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1); // Vai para tela de cadastro
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

    // Limpa campos
    ui->lineNome->clear();
    ui->lineLoginCadastro->clear();
    ui->lineSenha->clear();
    ui->lineConfirmarSenha->clear();

    ui->stackedWidget->setCurrentIndex(0); // Retorna para tela de login
}

void MainWindow::on_pushVoltar_clicked()
{
    // Limpa campos
    ui->lineNome->clear();
    ui->lineLoginCadastro->clear();
    ui->lineSenha->clear();
    ui->lineConfirmarSenha->clear();

    ui->stackedWidget->setCurrentIndex(0); // Retorna para login
}

void MainWindow::on_pushSair_clicked()
{
    receitaMensal = 0.0;
    listaDespesas.clear();
    ui->tableDespesas->clearContents();
    ui->tableDespesas->setRowCount(0);
    gerenciador.logout();
    ui->stackedWidget->setCurrentIndex(0); // Volta ao login
}

void MainWindow::on_pushConfirmar_clicked()
{
    receitaMensal = ui->lineReceita->text().toDouble();
    atualizarTotais();
}

void MainWindow::on_pushAdicionar_clicked()
{
    double valor = ui->lineValor->text().toDouble();
    QString categoria = ui->comboBoxCategoria->currentText();

    if (valor <= 0.0 || categoria.isEmpty()) {
        QMessageBox::warning(this, "Erro", "Informe um valor e categoria válidos.");
        return;
    }

    listaDespesas.push_back({categoria, valor});
    atualizarTabelaDespesas();
    atualizarTotais();
    limparCamposDespesa();
}

void MainWindow::limparCamposDespesa()
{
    ui->lineValor->clear();
    ui->comboBoxCategoria->setCurrentIndex(0);
}

void MainWindow::atualizarTabelaDespesas()
{
    ui->tableDespesas->setRowCount(static_cast<int>(listaDespesas.size()));

    for (int i = 0; i < static_cast<int>(listaDespesas.size()); ++i) {
        const auto& despesa = listaDespesas[i];
        double porcentagem = receitaMensal > 0 ? (despesa.valor / receitaMensal) * 100.0 : 0.0;

        ui->tableDespesas->setItem(i, 0, new QTableWidgetItem(QString::number(despesa.valor, 'f', 2)));
        ui->tableDespesas->setItem(i, 1, new QTableWidgetItem(despesa.categoria));
        ui->tableDespesas->setItem(i, 2, new QTableWidgetItem(QString::number(porcentagem, 'f', 1) + "%"));
    }
}

void MainWindow::atualizarTotais()
{
    double totalGasto = 0.0;
    for (const auto& d : listaDespesas)
        totalGasto += d.valor;

    double saldo = receitaMensal - totalGasto;
    double percGasto = receitaMensal > 0 ? (totalGasto / receitaMensal) * 100.0 : 0.0;
    double percSaldo = receitaMensal > 0 ? (saldo / receitaMensal) * 100.0 : 0.0;

    ui->labelValorTotalGasto->setText("R$ " + QString::number(totalGasto, 'f', 2));
    ui->labelPorcentagemTotalGasto->setText(QString::number(percGasto, 'f', 1) + "%");

    ui->labelSaldoAtual->setText("R$ " + QString::number(saldo, 'f', 2));
    ui->labelPorcentagemSaldoAtual->setText(QString::number(percSaldo, 'f', 1) + "%");
}
