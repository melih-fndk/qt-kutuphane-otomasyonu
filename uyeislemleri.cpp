// melih fındık 22100011064
#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

uyeislemleri::uyeislemleri(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uyeislemleri)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    uyeleriListele();
    connect(ui->yenikayitButon, &QPushButton::clicked, this, &uyeislemleri::yeniKayit);
    connect(ui->guncelleButon, &QPushButton::clicked, this, &uyeislemleri::guncelle);
    connect(ui->silButon, &QPushButton::clicked, this, &uyeislemleri::sil);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &uyeislemleri::uyeSecildi);
}

uyeislemleri::~uyeislemleri()
{
    delete ui;
}

void uyeislemleri::uyeleriListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM uye");

    model->setQuery(std::move(query));

    ui->tableView->setModel(model);
}

void uyeislemleri::yeniKayit()
{
    QString uyeAd = ui->uyeadlineEdit->text();
    QString uyeSoyad = ui->uyesoyadlineEdit->text();

    if (uyeAd.isEmpty() || uyeSoyad.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm alanları doldurun.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO uye (uye_ad, uye_soyad) VALUES (:uye_ad, :uye_soyad)");

    query.bindValue(":uye_ad", uyeAd);
    query.bindValue(":uye_soyad", uyeSoyad);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Yeni üye başarıyla eklendi.");
        uyeleriListele();
    } else {
        QMessageBox::critical(this, "Hata", "Üye eklenemedi: " + query.lastError().text());
    }
}

void uyeislemleri::guncelle()
{
    int uyeNo = ui->uyenolineEdit->text().toInt();
    QString uyeAd = ui->uyeadlineEdit->text();
    QString uyeSoyad = ui->uyesoyadlineEdit->text();

    if (uyeAd.isEmpty() || uyeSoyad.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm alanları doldurun.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE uye SET uye_ad = :uye_ad, uye_soyad = :uye_soyad WHERE uye_no = :uye_no");
    query.bindValue(":uye_ad", uyeAd);
    query.bindValue(":uye_soyad", uyeSoyad);
    query.bindValue(":uye_no", uyeNo);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Üye başarıyla güncellendi.");
        uyeleriListele();
    } else {
        QMessageBox::critical(this, "Hata", "Üye güncellenemedi: " + query.lastError().text());
    }
}

void uyeislemleri::sil()
{
    int uyeNo = ui->uyenolineEdit->text().toInt();


    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM odunc_alinan WHERE uye_no = :uye_no");
    checkQuery.bindValue(":uye_no", uyeNo);
    checkQuery.exec();
    checkQuery.next();

    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Hata", "Bu üyenin henüz teslim etmediği kitaplar var. Üye silinemez.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM uye WHERE uye_no = :uye_no");
    query.bindValue(":uye_no", uyeNo);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Üye başarıyla silindi.");
        uyeleriListele();
    } else {
        QMessageBox::critical(this, "Hata", "Üye silinemedi: " + query.lastError().text());
    }
}

void uyeislemleri::uyeSecildi(const QModelIndex &index)
{
    int row = index.row();
    int uyeNo = model->index(row, 0).data().toInt();

    ui->uyenolineEdit->setText(QString::number(uyeNo));
    ui->uyeadlineEdit->setText(model->index(row, 1).data().toString());
    ui->uyesoyadlineEdit->setText(model->index(row, 2).data().toString());
}
