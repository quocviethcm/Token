#include "tabledialog.h"
#include "ui_tabledialog.h"

TableDialog::TableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
}

TableDialog::~TableDialog()
{
    delete ui;
}

void TableDialog::ReceiveData(QSslCertificate *QsslObject, QString _Pubkey)
{
    if(QsslObject->isNull())
    {
        QMessageBox::warning(this, "Token Manager", "Không thể nhận được đối tượng QSslCertificate");
    } else
    {
        //subject info
        QString subject_cn = QsslObject->subjectInfo(QSslCertificate::CommonName);
        QString subject_o = QsslObject->subjectInfo(QSslCertificate::Organization);
        QString subject_l = QsslObject->subjectInfo(QSslCertificate::LocalityName);
        QString subject_c = QsslObject->subjectInfo(QSslCertificate::CountryName);
        //issue info
        QString issue_cn = QsslObject->issuerInfo(QSslCertificate::CommonName);
        QString issue_ou = QsslObject->issuerInfo(QSslCertificate::OrganizationalUnitName);
        QString issue_o = QsslObject->issuerInfo(QSslCertificate::Organization);
        QString issue_c = QsslObject->issuerInfo(QSslCertificate::CountryName);
        //issue certificate
        QString issue_cer_version = QString::fromLatin1(QsslObject->version());
        pkcs11 hex_convert;
        QString issue_cer_serial = hex_convert.hexFormat(QString::fromLatin1(QsslObject->serialNumber()), 1);
        QString issue_cer_datevalid = QsslObject->effectiveDate().toString(QString("dd/MM/yyyy"));
        QString issue_cer_dateexpire = QsslObject->expiryDate().toString(QString("dd/MM/yyyy"));
        //public key info
        QString pubkey_size;
        QString pubkey_alg;
        QSslKey publickey = QsslObject->publicKey();
        if(!publickey.isNull())
        {
                //size
            int length = publickey.length();
            if(length == -1)
                pubkey_size = "Key is null";
            else
                pubkey_size = QString::number(length, 10);
                //alg
            pubkey_alg = (publickey.algorithm()==1)?"DSA":"RSA";
        } else {
            pubkey_size = "Key is null";
            pubkey_alg = "Key is null";
        }

        //set GUI
        ui->lb_subject_cn->setText(subject_cn);
        ui->lb_subject_o->setText(subject_o);
        ui->lb_subject_l->setText(subject_l);
        ui->lb_subject_c->setText(subject_c);

        ui->lb_issue_name_c->setText(issue_c);
        ui->lb_issue_name_o->setText(issue_o);
        ui->lb_issue_name_ou->setText(issue_ou);
        ui->lb_issue_name_cn->setText(issue_cn);

        ui->lb_issue_cer_version->setText(issue_cer_version);
        ui->editT_pubkey_serial->setText(issue_cer_serial);
        ui->lb_issue_cer_validbefore->setText(issue_cer_datevalid);
        ui->lb_issue_cer_validafter->setText(issue_cer_dateexpire);

        ui->lb_pubkey_keysz->setText(pubkey_size);
        ui->lb_pubkey_alg->setText(pubkey_alg);
        ui->editT_pubkey_value->setText(_Pubkey);

    }
}

QString TableDialog::getContentQsslObject(QStringList content)
{
    QString temp = content.join(" ");
    return temp;
}
