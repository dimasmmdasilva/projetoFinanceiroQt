#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFont>

// Construtor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configura o logotipo ASCII nos labels
    configurarLogo(ui->labelLogo);
    configurarLogo(ui->labelLogoCadastro);

    // Assinatura do autor
    ui->labelAutor->setText("Desenvolvido por DimasMMS");
    QFont fonteAutor("Arial", 8);
    ui->labelAutor->setFont(fonteAutor);
    ui->labelAutor->setStyleSheet("color: yellow;");
    ui->labelAutor->setAlignment(Qt::AlignCenter);

    // Configura tabela de despesas
    ui->tableDespesas->setColumnCount(3);
    ui->tableDespesas->setHorizontalHeaderLabels({"Valor", "Categoria", "%"});
    ui->tableDespesas->horizontalHeader()->setStretchLastSection(true);

    // Oculta a senha ao digitar
    ui->linePassword->setEchoMode(QLineEdit::Password);

    // Conecta mudanças de mês e ano
    connect(ui->comboBoxMes, &QComboBox::currentIndexChanged, this, &MainWindow::on_mesOuAnoAlterado);
    connect(ui->spinAno, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::on_mesOuAnoAlterado);
}

// Destrutor
MainWindow::~MainWindow() {
    delete ui;
}

// ==============================
// === Sessão e Navegação =======
// ==============================

// Login
void MainWindow::on_pushEntrar_clicked()
{
    QString login = ui->lineLogin->text();
    QString senha = ui->linePassword->text();

    if (gerenciador.autenticarUsuario(login, senha)) {
        carregarNomeUsuario();
        ui->stackedWidget->setCurrentIndex(2);
        on_mesOuAnoAlterado();  // Carrega automaticamente receita e despesas
    } else {
        QMessageBox::warning(this, "Erro de login", "Login ou senha incorretos.");
    }
}

// Logout
void MainWindow::on_pushSair_clicked()
{
    receitaMensal = 0.0;
    listaDespesas.clear();
    ui->tableDespesas->clearContents();
    ui->tableDespesas->setRowCount(0);
    gerenciador.logout();

    // Limpa campos de login
    ui->lineLogin->clear();
    ui->linePassword->clear();

    ui->stackedWidget->setCurrentIndex(0);
}

// Ir para tela de cadastro
void MainWindow::on_pushCadastrar_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
}

// Voltar da tela de cadastro para login
void MainWindow::on_pushVoltar_clicked()
{
    ui->lineNome->clear();
    ui->lineLoginCadastro->clear();
    ui->lineSenha->clear();
    ui->lineConfirmarSenha->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

// ==============================
// === Cadastro de Usuário ======
// ==============================

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
    on_pushVoltar_clicked();
}

// ==============================
// === Receita e Despesas =======
// ==============================

void MainWindow::on_pushConfirmar_clicked()
{
    receitaMensal = ui->lineReceita->text().toDouble();
    mesSelecionado = ui->comboBoxMes->currentIndex();
    anoSelecionado = ui->spinAno->value();

    Usuario* u = gerenciador.getUsuarioLogado();
    if (!u) return;

    if (!gerenciador.salvarReceita(u->getId(), mesSelecionado, anoSelecionado, receitaMensal)) {
        QMessageBox::warning(this, "Erro", "Erro ao salvar receita.");
        return;
    }

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

    Usuario* u = gerenciador.getUsuarioLogado();
    if (!u) return;

    if (!gerenciador.salvarDespesa(u->getId(), mesSelecionado, anoSelecionado, categoria, valor)) {
        QMessageBox::warning(this, "Erro", "Erro ao salvar despesa.");
        return;
    }

    listaDespesas = gerenciador.obterDespesas(u->getId(), mesSelecionado, anoSelecionado);
    atualizarTabelaDespesas();
    atualizarTotais();
    limparCamposDespesa();
}

void MainWindow::on_pushLimpar_clicked()
{
    Usuario* u = gerenciador.getUsuarioLogado();
    if (!u) return;

    if (!gerenciador.removerDespesas(u->getId(), mesSelecionado, anoSelecionado)) {
        QMessageBox::warning(this, "Erro", "Erro ao apagar despesas.");
        return;
    }

    listaDespesas.clear();
    ui->tableDespesas->clearContents();
    ui->tableDespesas->setRowCount(0);
    atualizarTotais();
}

// Ao alterar mês ou ano: carrega nova receita e despesas
void MainWindow::on_mesOuAnoAlterado()
{
    Usuario* u = gerenciador.getUsuarioLogado();
    if (!u) return;

    mesSelecionado = ui->comboBoxMes->currentIndex();
    anoSelecionado = ui->spinAno->value();

    receitaMensal = gerenciador.obterReceita(u->getId(), mesSelecionado, anoSelecionado);
    ui->lineReceita->setText(QString::number(receitaMensal, 'f', 2));

    listaDespesas = gerenciador.obterDespesas(u->getId(), mesSelecionado, anoSelecionado);
    atualizarTabelaDespesas();
    atualizarTotais();
}

// ==============================
// === Atualizações Visuais =====
// ==============================

void MainWindow::atualizarTabelaDespesas()
{
    ui->tableDespesas->setRowCount(listaDespesas.size());

    for (int i = 0; i < listaDespesas.size(); ++i) {
        const Despesa& d = listaDespesas[i];
        double perc = receitaMensal > 0 ? (d.valor / receitaMensal) * 100.0 : 0.0;

        ui->tableDespesas->setItem(i, 0, new QTableWidgetItem(QString::number(d.valor, 'f', 2)));
        ui->tableDespesas->setItem(i, 1, new QTableWidgetItem(d.categoria));
        ui->tableDespesas->setItem(i, 2, new QTableWidgetItem(QString::number(perc, 'f', 1) + "%"));
    }
}

void MainWindow::atualizarTotais()
{
    double total = 0.0;
    for (const auto& d : listaDespesas)
        total += d.valor;

    double saldo = receitaMensal - total;
    double percGasto = receitaMensal > 0 ? (total / receitaMensal) * 100.0 : 0.0;
    double percSaldo = receitaMensal > 0 ? (saldo / receitaMensal) * 100.0 : 0.0;

    ui->labelValorTotalGasto->setText("R$ " + QString::number(total, 'f', 2));
    ui->labelPorcentagemTotalGasto->setText(QString::number(percGasto, 'f', 1) + "%");

    ui->labelValorSaldoAtual->setText("R$ " + QString::number(saldo, 'f', 2));
    ui->labelPorcentagemSaldoAtual->setText(QString::number(percSaldo, 'f', 1) + "%");
}

// ==============================
// === Utilitários ==============
// ==============================

void MainWindow::limparCamposDespesa()
{
    ui->lineValor->clear();
    ui->comboBoxCategoria->setCurrentIndex(0);
}

void MainWindow::carregarNomeUsuario()
{
    Usuario* u = gerenciador.getUsuarioLogado();
    if (u)
        ui->labelSaudacao->setText("Olá, " + u->getNome());
}

void MainWindow::configurarLogo(QLabel* label)
{
    QString logo = R"(╔══════════════════════╗
║       CF 1.0         ║
║ Controle Financeiro  ║
╚══════════════════════╝)";

    QFont fonte("Courier New");
    fonte.setStyleHint(QFont::Monospace);
    fonte.setPointSize(10);

    label->setText(logo);
    label->setFont(fonte);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setStyleSheet("color: lightgreen;");
}

