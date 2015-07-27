#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pkcs11.h"
#include "tabledialog.h"
#include <QtCore>
#include <QDebug>

DataCertificate CertificateStore[] = {
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""},
    {"", "", "", "", "", ""}
};

DataPublicKey PublicKeyStore[] = {
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""},
    {"", "", "", "", "", "", ""}
};

DataPrivateKey PrivateKeyStore[] = {
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", "", ""}
};

PositionItem locateItem[] = {
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""},
    {0,0,0,0, "", "", ""}
};


pkcs11 newToken;

QSslCertificate *QsslObj[10];

QSslCertificate *QsslObjFirst[10];

ItemSelect item_select;

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

    ui->settingMenuButton->setEnabled(false);
   // ui->pushButton->setEnabled(false);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddChild(QTreeWidgetItem *parent, QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name);
    //item->setIcon(0, ico);
    parent->addChild(item);
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
    ui->pushButton->setText("Login");
    ui->pushButton_2->setGeometry(320,310,100,50);
    ui->pushButton_2->setText("Logout");
    ui->pushButton_2->setEnabled(false);

    ui->loginScreen->setVisible(true);
    ui->lineEdit->setEchoMode(QLineEdit::Password);

}
void MainWindow::dataManagerScreen()
{

    ui->dataViewScreen->setVisible(false);
    ui->dataViewScreen->setGeometry(100,0,500,400);
    ui->treeWidget->setGeometry(10,10,480,300);
    ui->exportButton->setText("Export");
    ui->exportButton->setGeometry(50,330,100,50);
    ui->viewButton->setGeometry(200,330,100,50);
    ui->viewButton->setText("View");
    ui->refreshButton->setGeometry(350,330,100,50);
    ui->refreshButton->setText("Refresh");

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
    ui->changePinlineEdit_1->setGeometry(120,150,150,25);
    ui->changePinlineEdit_2->setGeometry(120,200,150,25);
    ui->changePinlineEdit_3->setGeometry(120,250,150,25);
    ui->changePinpushButton_1->setGeometry(100,350,100,25);
    ui->changePinpushButton_1->setText("Accept");
    ui->changePinpushButton_2->setGeometry(300,350,100,25);
    ui->changePinpushButton_2->setText("Cancel");

    ui->changePinlineEdit_1->setEchoMode(QLineEdit::Password);
    ui->changePinlineEdit_2->setEchoMode(QLineEdit::Password);
    ui->changePinlineEdit_3->setEchoMode(QLineEdit::Password);
}
void MainWindow::changeNameScreen()
{

    ui->changeNameScreen->setVisible(false);
    ui->changeNameScreen->setGeometry(100,0,500,400);
    ui->changeNamelabel_1->setGeometry(0,0,300,100);
    ui->changeNamelabel_1->setText("CHANGE NAME TOKEN");
    ui->changeNamelabel_2->setGeometry(0,150,100,25);
    ui->changeNamelabel_2->setText("Name");

    ui->changeNamelineEdit_1->setGeometry(120,150,300,25);
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
        ui->lineEdit->setText("");
    }
    rv = newToken.Login(pinCode);
    if (rv != 0)
    {
        QMessageBox::information(this,"Login fail","Wrong PIN number");

    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
        ui->dataViewScreen->setVisible(true);
        ui->loginScreen->setVisible(false);
        ui->settingMenuButton->setEnabled(true);

    }
    ui->lineEdit->setText("");

}
void MainWindow::on_pushButton_2_clicked()
{
   newToken.Logout();
   ui->pushButton->setEnabled(true);
   ui->pushButton_2->setEnabled(false);
   ui->settingMenuButton->setEnabled(false);
   ui->settingMenu->setVisible(false);
   ui->treeWidget->clear();

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
    ui->changeNamelineEdit_1->setText(this->getTokenName().trimmed());

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

QString MainWindow::getTokenName(){
    newToken.Connect();
    //CK_TOKEN_INFO tokenInfo;
    rv = newToken.GetTokenInfo();
    QString a = (const char *)(newToken.tokenInfo.label);
    qDebug() << a;
    return a.split(QRegExp("\\s+Viettel-CA"))[0];

}

void MainWindow::DeleteSameElementArray(QString strA[], int &length)
{
    for(int i=0; i<length; i++)
    {
        for(int t=i+1; t<length; t++)
        {
            while((QString::compare(strA[i], strA[t], Qt::CaseSensitive)) == 0)
            {
                for(int j=t; j<length; j++)
                    strA[j]=strA[j+1];
                length--;
            }
        }
    }
}

void MainWindow::on_refreshButton_clicked()
{
    ui->treeWidget->clear();


      newToken.GetCertificate(CertificateStore,QsslObj);
      newToken.GetPrivateKey(PrivateKeyStore);
    newToken.GetPublicKey(PublicKeyStore);


   QString temp[30];
   QString temp2[30];

   int i=0;
   int index = 0;
   int countNotNull = 0;

   for(i=0; i<30; i++)
   {
       temp[i] = "";
       temp2[i] = "";
   }

   // copy label's object into ObjectLabel
   for(i=0; i<10; i++)
   {
       temp[index] = CertificateStore[i].CER_CKA_ID;
       index++;
   }
   for(i=0; i<10; i++)
   {
       temp[index] = PublicKeyStore[i].KEYPU_CKA_ID;
       index++;
   }
   for(i=0; i<10; i++)
   {
       temp[index] = PrivateKeyStore[i].KEYPR_CKA_ID;
       index++;
   }
   // Delete ""
   for(i=0; i<30; i++)
   {
       if((QString::compare(temp[i], QString(""), Qt::CaseSensitive)) > 0)
       {
           temp2[countNotNull] = temp[i];
           countNotNull++;
       }

   }

   int length = countNotNull;
   // Delete the same elements
   DeleteSameElementArray(temp2, length);

   //testshow2(temp2, length);

   QTreeWidgetItem *TopItem[length];
   // Set Icon for Item
   QIcon ico_TopLevel;
   ico_TopLevel.addFile(QString(":/images/res/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
   QIcon ico_certificate;
   ico_certificate.addFile(QString(":/images/res/chungthuso.png"), QSize(), QIcon::Normal, QIcon::Off);
   QIcon ico_PublicKey;
   ico_PublicKey.addFile(QString(":/images/res/public.png"), QSize(), QIcon::Normal, QIcon::Off);
   QIcon ico_PrivateKey;
   ico_PrivateKey.addFile(QString(":/images/res/private.png"), QSize(), QIcon::Normal, QIcon::Off);

   int j, t;
   QString toplevelname;

   for(i=0; i<length; i++)
   {
       for(t=0; t< 10; t++)
       {
           if((QString::compare(CertificateStore[t].CER_CKA_ID, temp2[i], Qt::CaseSensitive)) == 0)
           {
               toplevelname = CertificateStore[t].CER_COMMOND_NAME.toUpper();
               break;
           }
           if((QString::compare(PublicKeyStore[t].KEYPU_CKA_ID, temp2[i], Qt::CaseSensitive)) == 0)
           {
               toplevelname = PublicKeyStore[t].KEYPU_CKA_CONTAINNER.toUpper();
               break;
           }
           if((QString::compare(PrivateKeyStore[t].KEYPR_CKA_ID, temp2[i], Qt::CaseSensitive)) == 0)
           {
               toplevelname = PrivateKeyStore[t].KEYPR_CKA_CONTAINNER.toUpper();
               break;
           }
       }
       locateItem[i].indexOfRoot = i;
       TopItem[i] = new QTreeWidgetItem(ui->treeWidget);
       TopItem[i]->setText(0, toplevelname);
       //TopItem[i]->setIcon(0, ico_TopLevel);
       ui->treeWidget->addTopLevelItem(TopItem[i]);


       //add certificate
       for(j=0; j<10; j++)
       {
           if((QString::compare(temp2[i], CertificateStore[j].CER_CKA_ID, Qt::CaseSensitive)) == 0)
           {
               AddChild(TopItem[i], "Certificate");
               locateItem[i].indexOfCer = j;
               locateItem[i].CerID = CertificateStore[j].CER_CKA_ID;
               break;
           }
       }
       //add private key
       for(j=0; j<10; j++)
       {
           if((QString::compare(temp2[i], PrivateKeyStore[j].KEYPR_CKA_ID, Qt::CaseSensitive)) == 0)
           {
               AddChild(TopItem[i], "Privatekey");
               locateItem[i].indexOfPri = j;
               locateItem[i].PriID = PrivateKeyStore[j].KEYPR_CKA_ID;
               break;
           }
       }
       //add public key
       for(j=0; j<10; j++)
       {
           if((QString::compare(temp2[i], PublicKeyStore[j].KEYPU_CKA_ID, Qt::CaseSensitive)) == 0)
           {
               AddChild(TopItem[i], "Publickey");
               locateItem[i].indexOfPub = j;
               locateItem[i].PubID = PublicKeyStore[j].KEYPU_CKA_ID;
               break;
           }
       }
   }
   //ui->treeWidget->expandAll();
   //
   //Logined = true;



}

void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void MainWindow::on_viewButton_clicked()
{
    if((QString::compare(item_select.ParentName, QString(""), Qt::CaseSensitive)) == 0
            && (QString::compare(item_select.ItemName, QString(""), Qt::CaseSensitive)) == 0)
    {
        if(QString::compare(item_select.ItemName, "Publickey", Qt::CaseSensitive) != 0
                && QString::compare(item_select.ItemName, "Privatekey", Qt::CaseSensitive) != 0)
        {
            if(item_select.IndexofParent >= 0)
            {
                QString pubkey;
                for(int i=0; i<10; i++)
                {
                    if(CertificateStore[item_select.IndexofParent].CER_CKA_ID.compare(PublicKeyStore[i].KEYPU_CKA_ID) == 0)
                    {
                        pubkey = PublicKeyStore[i].KEYPU_CKA_MODULUS;
                        break;
                    }
                }
                TableDialog TD_Info;
                TD_Info.ReceiveData(QsslObj[item_select.IndexofParent], pubkey);
                TD_Info.setWindowFlags(Qt::WindowStaysOnTopHint);
                TD_Info.setWindowTitle(item_select.ParentName);
                TD_Info.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, TD_Info.size(), qApp->desktop()->availableGeometry()));
                TD_Info.exec();
            }
        }
        else
            QMessageBox::information(this, "title_dialog", "NO Infomation. Contact Development");

    } else
        QMessageBox::information(this,"title_dialog", "NO Infomation. Contact Development");
}




void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent;
    parent = item->parent();

    int root = ui->treeWidget->indexOfTopLevelItem(item);

    QString nameofparent;
    QString nameofisubtem;
    if(root < 0)
    {
        nameofparent = parent->text(0);
        nameofisubtem = ui->treeWidget->currentItem()->text(0);
        item_select.ItemName = nameofisubtem;
        item_select.ParentName = nameofparent;

        item_select.IndexofParent = ui->treeWidget->indexOfTopLevelItem(item->parent());
    } else
    {
        item_select.ItemName = "";
        item_select.ParentName = "";
        item_select.IndexofParent = root;
    }
    item_select.ParentID = root;
    item_select.CurrentItemName = ui->treeWidget->currentItem()->text(0);
}
