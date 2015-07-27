#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtNetwork/QSsl>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QNetworkSession>
#include "include/cryptoki_ext.h"
#include "include/auxiliary.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool checked;
    CK_RV rv;
    CK_TOKEN_INFO tokenInfo;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
protected:
    void dataManagerScreen();
    void loginScreen();
    void paintEvent(QPaintEvent *e);
    void settingMenu();
    void changePinTokenScreen();
    void changeNameScreen();
    QString getTokenName();
    void AddChild(QTreeWidgetItem *parent, QString name);
    void addRoot(QString name);

    void DeleteSameElementArray(QString strA[], int &length);


private slots:
    void on_loginMenuButton_clicked();
    void on_cerMenuButton_clicked();
    void on_pushButton_clicked();
    void on_settingMenuButton_toggled(bool checked);
    void on_settingMenuPushButton_2_clicked();
    void on_settingMenuPushButton_clicked();
    void on_changePinpushButton_1_clicked();
    void on_changePinpushButton_2_clicked();
    void on_changeNamepushButton_1_clicked();
    void on_changeNamepushButton_2_clicked();
    void on_pushButton_2_clicked();
    void on_refreshButton_clicked();
    void on_lineEdit_returnPressed();
    void on_viewButton_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item);
};

#endif // MAINWINDOW_H
