// melih fındık 22100011064
#ifndef ODUNCALMAISLEM_H
#define ODUNCALMAISLEM_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class oduncalmaislem;
}

class oduncalmaislem : public QDialog
{
    Q_OBJECT

public:
    explicit oduncalmaislem(QWidget *parent = nullptr);
    ~oduncalmaislem();

private slots:
    void oduncAl();
    void kitapSecildi(const QModelIndex &index);
    void uyeSecildi(const QModelIndex &index);

private:
    Ui::oduncalmaislem *ui;
    QSqlQueryModel *kitapModel;
    QSqlQueryModel *uyeModel;
    QSqlQueryModel *oduncModel;
    void kitaplariListele();
    void uyeleriListele();
    void oduncleriListele();
};

#endif // ODUNCALMAISLEM_H
