#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
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
};

#endif // MAINWINDOW_H
