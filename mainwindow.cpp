#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pkcs11.h"
#include <QtCore>

pkcs11 newToken;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CK_RV rv;
        rv = C_Initialize(NULL_PTR);


    ui->centralWidget->setFixedSize(QSize(600,400));


    ui->loginMenuButton->setGeometry(0,0,100,70);
    ui->loginMenuButton->setIcon(QIcon(QPixmap(":/icons/icons/computer.svg")));
    ui->loginMenuButton->setIconSize(QSize(60,40));
    ui->loginMenuButton->setText("Login");
    ui->loginMenuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->loginMenuButton->setStyleSheet("QToolButton{border: none;outline: none;text-align: center;color : white;}"
                    "QToolButton:hover{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 30% ), stop: 1 rgba( 125, 125, 170, 0% ));border:0px;color : white;}"
                    "QToolButton:pressed{background:QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 80% ), stop: 1 rgba( 125, 125, 170, 30% )); position: relative;top: 1px; left: 1px;color : white;}");
    ui->cerMenuButton->setGeometry(0,70,100,70);
    ui->cerMenuButton->setIcon(QIcon(QPixmap(":/icons/icons/Certificate_Icon.png")));
    ui->cerMenuButton->setIconSize(QSize(60,40));
    ui->cerMenuButton->setText("Certificate");
    ui->cerMenuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->cerMenuButton->setStyleSheet("QToolButton{border: none;outline: none;text-align: center;color : white;}"
                    "QToolButton:hover{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 30% ), stop: 1 rgba( 125, 125, 170, 0% ));border:0px;color : white;}"
                    "QToolButton:pressed{background:QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 80% ), stop: 1 rgba( 125, 125, 170, 30% )); position: relative;top: 1px; left: 1px;color : white;}");



    loginScreen();
    dataManagerScreen();




    //ui->centralWidget->setStyleSheet("QWidget{background-color: rgba( 0, 0, 0, 0% );}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginScreen()
{

    ui->loginScreen->setGeometry(100,0,500,400);
    ui->label_2->setGeometry(100,100,200,100);
    ui->label_2->setText("Wellcome");
    ui->label_2->setStyleSheet("QLabel{font-size:30px;}");
    //ui->widget->setStyleSheet("QWidget{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 80% ), stop: 1 rgba( 125, 125, 170, 30% ));}");
    ui->label->setGeometry(120,250,100,50);
    ui->label->setText("<b> PIN code:</b>");
    ui->lineEdit->setGeometry(230,250,200,50);
    ui->pushButton->setGeometry(120,310,100,50);
    ui->pushButton->setText("LOGIN");
    ui->loginScreen->setVisible(true);

}
void MainWindow::dataManagerScreen()
{
    ui->dataViewScreen->setVisible(false);
    ui->dataViewScreen->setGeometry(100,0,500,400);
    ui->treeView->setGeometry(10,10,480,300);
    ui->exportButton->setText("Export");
    ui->exportButton->setGeometry(100,330,100,50);
    ui->viewButton->setGeometry(300,330,100,50);
    ui->viewButton->setText("View");

}

void MainWindow::paintEvent(QPaintEvent *e){

    QPainter painter(this);

    QLinearGradient gradl(50,300,250,300);
    QColor leftinterface(25,25,70,255);
    gradl.setColorAt(0.0,leftinterface);
    gradl.setColorAt(1.0,Qt::white);
    QRect rec(0,0,100,600);
    painter.fillRect(rec,gradl);
    e->accept();
}

void MainWindow::on_loginMenuButton_clicked()
{
    ui->loginScreen->setVisible(true);
    ui->dataViewScreen->setVisible(false);
}

void MainWindow::on_cerMenuButton_clicked()
{
    ui->loginScreen->setVisible(false);
    ui->dataViewScreen->setVisible(true);
}

void MainWindow::on_pushButton_clicked()
{
    int rv;
    QString pinCode;
    pinCode = ui->lineEdit->text();

    rv = newToken.Connect();
    if(rv != 0)
    {
        QMessageBox::warning(this,"Connect Token Fail","@@ @@ @@");
    }
    rv = newToken.Login(pinCode);
    if (rv != 0)
    {
        QMessageBox::information(this,"Login fail","Wrong PIN number");
    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->dataViewScreen->setVisible(true);
        ui->loginScreen->setVisible(false);
    }
}
