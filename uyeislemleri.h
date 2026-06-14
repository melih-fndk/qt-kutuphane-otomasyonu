// melih fındık 22100011064
#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class uyeislemleri;
}

class uyeislemleri : public QDialog
{
    Q_OBJECT

public:
    explicit uyeislemleri(QWidget *parent = nullptr);
    ~uyeislemleri();

private slots:
    void yeniKayit();
    void guncelle();
    void sil();
    void uyeSecildi(const QModelIndex &index);

private:
    Ui::uyeislemleri *ui;
    QSqlQueryModel *model;
    void uyeleriListele();
};

#endif // UYEISLEMLERI_H

