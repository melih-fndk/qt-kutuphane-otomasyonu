// melih fındık 22100011064
#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

#include <QDialog>
#include <QSqlQueryModel>


namespace Ui {
class kitapislemleri;
}

class kitapislemleri : public QDialog
{
    Q_OBJECT

public:
    explicit kitapislemleri(QWidget *parent = nullptr);
    ~kitapislemleri();

private slots:
    void yeniKayit();
    void guncelle();
    void sil();
    void kitapSecildi(const QModelIndex &index);

private:
    Ui::kitapislemleri *ui;
    QSqlQueryModel *model;
    QSqlQueryModel *modelOduncAlinan;
    QSqlQueryModel *modelOduncTeslimEdilen;
    void kitaplariListele();
    void oduncDurumlariniGoster(int kitapNo);
};

#endif // KITAPISLEMLERI_H

