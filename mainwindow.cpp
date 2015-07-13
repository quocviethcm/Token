#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pkcs11.h"
#include <QtCore>

pkcs11 newToken;

const int buttonW = 70;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    rv = C_Initialize(NULL_PTR);

    checked = false;


    ui->centralWidget->setFixedSize(QSize(600,400));


    ui->loginMenuButton->setGeometry(0,0,100,buttonW);
    ui->loginMenuButton->setIcon(QIcon(QPixmap(":/icons/icons/computer.svg")));
    ui->loginMenuButton->setIconSize(QSize(60,40));
    ui->loginMenuButton->setText("Login");
    ui->loginMenuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->loginMenuButton->setStyleSheet("QToolButton{border: none;outline: none;text-align: center;color : white;}"
                    "QToolButton:hover{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 30% ), stop: 1 rgba( 125, 125, 170, 0% ));border:0px;color : white;}"
                    "QToolButton:pressed{background:QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 80% ), stop: 1 rgba( 125, 125, 170, 30% )); position: relative;top: 1px; left: 1px;color : white;}");
    ui->cerMenuButton->setGeometry(0,70,100,buttonW);
    ui->cerMenuButton->setIcon(QIcon(QPixmap(":/icons/icons/Certificate_Icon.png")));
    ui->cerMenuButton->setIconSize(QSize(60,40));
    ui->cerMenuButton->setText("Certificate");
    ui->cerMenuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->cerMenuButton->setStyleSheet("QToolButton{border: none;outline: none;text-align: center;color : white;}"
                    "QToolButton:hover{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 30% ), stop: 1 rgba( 125, 125, 170, 0% ));border:0px;color : white;}"
                    "QToolButton:pressed{background:QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 80% ), stop: 1 rgba( 125, 125, 170, 30% )); position: relative;top: 1px; left: 1px;color : white;}");
    ui->settingMenuButton->setGeometry(0,140,100,buttonW);
    ui->settingMenuButton->setIcon(QIcon(QPixmap(":/icons/icons/gnome_preferences_system.png")));
    ui->settingMenuButton->setIconSize(QSize(60,40));
    ui->settingMenuButton->setText("Setting");
    ui->settingMenuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->settingMenuButton->setStyleSheet("QToolButton{border: none;outline: none;text-align: center;color : white;}"
                    "QToolButton:hover{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 30% ), stop: 1 rgba( 125, 125, 170, 0% ));border:0px;color : white;}"
                    "QToolButton:pressed{background:QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 80% ), stop: 1 rgba( 125, 125, 170, 30% )); position: relative;top: 1px; left: 1px;color : white;}");
    ui->settingMenuButton->setCheckable(true);

    loginScreen();
    dataManagerScreen();
    settingMenu();
    changePinTokenScreen();
    changeNameScreen();




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
void MainWindow::settingMenu()
{
    ui->settingMenu->setVisible(false);
    ui->settingMenu->setStyleSheet("QWidget{background-color: dimgray;}");
    ui->settingMenu->setGeometry(100,buttonW*2+buttonW/2-200/2,200,200);
    ui->settingMenuLabel->setGeometry(0,0,200,25);
    ui->settingMenuLabel->setText("<b>Configure Token</b>");
    ui->settingMenuLabel->setStyleSheet("QLabel{background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba( 125, 125, 170, 100% ), stop: 1 rgba( 125, 125, 170, 50% ));border:0px;color : white;}");
    ui->settingMenuPushButton->setGeometry(0,25,200,25);
    ui->settingMenuPushButton->setText("Change Token Name");
    ui->settingMenuPushButton->setStyleSheet("QPushButton{color:white;}");
    ui->settingMenuPushButton_2->setGeometry(0,50,200,25);
    ui->settingMenuPushButton_2->setText("Change Token PIN");
    ui->settingMenuPushButton_2->setStyleSheet("QPushButton{color:white;}");
}
void MainWindow::changePinTokenScreen(){
    ui->changePinScreen->setVisible(false);
    ui->changePinScreen->setGeometry(100,0,500,400);
    ui->changePinlabel_1->setGeometry(0,0,300,100);
    ui->changePinlabel_1->setText("CHANGE TOKEN PIN");
    ui->changePinlabel_2->setGeometry(0,150,100,25);
     ui->changePinlabel_2->setText("Old PIN");
    ui->changePinlabel_3->setGeometry(0,200,100,25);
     ui->changePinlabel_3->setText("New PIN");

    ui->changePinlabel_4->setGeometry(0,250,100,25);
     ui->changePinlabel_4->setText("ReType PIN");
    ui->changePinlineEdit_1->setGeometry(120,150,100,25);
    ui->changePinlineEdit_2->setGeometry(120,200,100,25);
    ui->changePinlineEdit_3->setGeometry(120,250,100,25);
    ui->changePinpushButton_1->setGeometry(100,350,100,25);
    ui->changePinpushButton_1->setText("Accept");
    ui->changePinpushButton_2->setGeometry(300,350,100,25);
    ui->changePinpushButton_2->setText("Cancel");
}
void MainWindow::changeNameScreen()
{
    ui->changeNameScreen->setVisible(false);
    ui->changeNameScreen->setGeometry(100,0,500,400);
    ui->changeNamelabel_1->setGeometry(0,0,300,100);
    ui->changeNamelabel_1->setText("CHANGE NAME TOKEN");
    ui->changeNamelabel_2->setGeometry(0,150,100,25);
    ui->changeNamelabel_2->setText("Name");
    ui->changeNamelineEdit_1->setGeometry(120,150,100,25);
    ui->changeNamepushButton_1->setGeometry(100,350,100,25);
    ui->changeNamepushButton_1->setText("Accept");
    ui->changeNamepushButton_2->setGeometry(300,350,100,25);
    ui->changeNamepushButton_2->setText("Cancel");

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
    ui->changePinScreen->setVisible(false);
    ui->changeNameScreen->setVisible(false);
    ui->changePinScreen->setVisible(false);

}

void MainWindow::on_cerMenuButton_clicked()
{
    ui->loginScreen->setVisible(false);
    ui->dataViewScreen->setVisible(true);
    ui->changePinScreen->setVisible(false);
    ui->changeNameScreen->setVisible(false);
    ui->changePinScreen->setVisible(false);

}

void MainWindow::on_pushButton_clicked()
{
    //int rv;
    QString pinCode;
    pinCode = ui->lineEdit->text();

    rv = newToken.Connect();
    if(rv != 0)
    {
        QMessageBox::warning(this,"Connect Token Fail","Are you plug token yet?");
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





void MainWindow::on_settingMenuButton_toggled(bool checked)
{

    if(checked == true)
    {
        ui->settingMenu->setVisible(true);
        ui->settingMenu->raise();
    }

    else
    {
        ui->settingMenu->setVisible(false);
    }
    //checked = ~checked;

}

void MainWindow::on_settingMenuPushButton_clicked()
{
    ui->settingMenu->setVisible(false);
    //checked = ~checked;
    ui->changeNameScreen->setVisible(true);
    ui->loginScreen->setVisible(false);
    ui->dataViewScreen->setVisible(false);
    ui->changePinScreen->setVisible(false);
    ui->changePinScreen->setVisible(false);

}

void MainWindow::on_settingMenuPushButton_2_clicked()
{
    ui->settingMenu->setVisible(false);
    //checked = ~checked;
    ui->changeNameScreen->setVisible(false);
    ui->loginScreen->setVisible(false);
    ui->dataViewScreen->setVisible(false);

    ui->changePinScreen->setVisible(true);
}

void MainWindow::on_changePinpushButton_1_clicked()
{

    rv = newToken.Connect();
    QString oldPin, newPin, confirmPin;
    oldPin = ui->changePinlineEdit_1->text();
    newPin = ui->changePinlineEdit_2->text();
    confirmPin = ui->changePinlineEdit_3->text();

    if (newPin.compare(confirmPin) != 0)
    {
        QMessageBox::warning(this,"Sorry", "New Password not match");
        ui->changePinlineEdit_2->setText("");
        ui->changePinlineEdit_3->setText("");
        ui->changePinlineEdit_1->setText("");
    }
    else
    {

        int i = newToken.ChangePin(oldPin,newPin);

        if (i == CKR_OK)
        QMessageBox::information(this,"Change PIN","Success");
        else         QMessageBox::information(this,"Change PIN","Fail");

        ui->changePinlineEdit_2->setText("");
        ui->changePinlineEdit_3->setText("");
        ui->changePinlineEdit_1->setText("");

    }
}

void MainWindow::on_changePinpushButton_2_clicked()
{
    ui->changePinlineEdit_2->setText("");
    ui->changePinlineEdit_3->setText("");
    ui->changePinlineEdit_1->setText("");
}



void MainWindow::on_changeNamepushButton_1_clicked()
{

    QString newName = ui->changeNamelineEdit_1->text();
    rv = newToken.SetTokenName(newName);
    if (rv != CKR_OK) QMessageBox::information(this,"Change Name","Fail");
    else QMessageBox::information(this,"Change Name","Success");

    ui->changeNamelineEdit_1->setText("");
}

void MainWindow::on_changeNamepushButton_2_clicked()
{
    ui->changeNamelineEdit_1->setText("");

}
