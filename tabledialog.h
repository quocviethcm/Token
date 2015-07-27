#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H
#pragma once
#include "pkcs11.h"
#include <QDialog>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QDebug>
#include <QtCore>
#include <QtGui>

namespace Ui {
class TableDialog;
}

class TableDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TableDialog(QWidget *parent = 0);
    ~TableDialog();
    
private:
    Ui::TableDialog *ui;


public:
    void ReceiveData(QSslCertificate *QsslObject, QString _Pubkey);
    QString getContentQsslObject(QStringList content);
};

#endif // TABLEDIALOG_H
