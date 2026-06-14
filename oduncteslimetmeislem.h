// melih fındık 22100011064
#ifndef ODUNCTESLIMETMEISLEM_H
#define ODUNCTESLIMETMEISLEM_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

namespace Ui {
class oduncteslimetmeislem;
}

class oduncteslimetmeislem : public QDialog
{
    Q_OBJECT

public:
    explicit oduncteslimetmeislem(QWidget *parent = nullptr);
    ~oduncteslimetmeislem();

private slots:
    void oduncTeslimEt();
    void oduncSecildi(const QModelIndex &index);


private:
    Ui::oduncteslimetmeislem *ui;
    QSqlQueryModel *oduncModel;
    QSqlQueryModel *teslimModel;

    void oduncleriListele();
    void teslimleriListele();
    int hesaplaBorc(const QDate &almaTarihi, const QDate &teslimTarihi);
};

#endif // ODUNCTESLIMETMEISLEM_H
