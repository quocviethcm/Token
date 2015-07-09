#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
protected:
    void dataManagerScreen();
    void loginScreen();
    void paintEvent(QPaintEvent *e);
private slots:
    void on_loginMenuButton_clicked();
    void on_cerMenuButton_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
