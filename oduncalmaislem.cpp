// melih fındık 22100011064
#include "oduncalmaislem.h"
#include "ui_oduncalmaislem.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

oduncalmaislem::oduncalmaislem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oduncalmaislem)
{
    ui->setupUi(this);
    kitapModel = new QSqlQueryModel(this);
    oduncModel = new QSqlQueryModel(this);
    uyeModel = new QSqlQueryModel(this);

    kitaplariListele();
    uyeleriListele();
    oduncleriListele();

    connect(ui->odunc_almabuton, &QPushButton::clicked, this, &oduncalmaislem::oduncAl);
    connect(ui->tableViewKitaplar->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &oduncalmaislem::kitapSecildi);
    connect(ui->tableViewUyeler->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &oduncalmaislem::uyeSecildi);
}

oduncalmaislem::~oduncalmaislem()
{
    delete ui;
}

void oduncalmaislem::kitaplariListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM kitap");

    kitapModel->setQuery(query);
    ui->tableViewKitaplar->setModel(kitapModel);
}

void oduncalmaislem::uyeleriListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM uye");

    uyeModel->setQuery(query);
    ui->tableViewUyeler->setModel(uyeModel);
}

void oduncalmaislem::oduncleriListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM odunc_alinan");

    oduncModel->setQuery(query);
    ui->tableViewOduncAlinanKitap->setModel(oduncModel);
}


void oduncalmaislem::oduncAl()
{
    int kitapNo = ui->kitapNoLN->text().toInt();
    int uyeNo = ui->uyeNoLN->text().toInt();
    QDate oduncAlmaTarihi = ui->dateEdit->date();
    QString oduncAlmaStr = oduncAlmaTarihi.toString("yyyy-MM-dd");

    if (kitapNo == 0 || uyeNo == 0) {
        QMessageBox::warning(this, "Hata", "Lütfen kitap ve üye seçin.");
        return;
    }

    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    query.bindValue(":uye_no", uyeNo);
    query.bindValue(":kitap_no", kitapNo);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Hata", "Veritabanı hatası: " + query.lastError().text());
        return;
    }
    if (query.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Hata", "Bu kitap zaten ödünç alınmış.");
        return;
    }

    query.prepare("SELECT kitap_sayisi FROM kitap WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_no", kitapNo);
    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Hata", "Veritabanı hatası: " + query.lastError().text());
        return;
    }
    if (query.value(0).toInt() <= 0) {
        QMessageBox::warning(this, "Hata", "Bu kitap kütüphanede mevcut değil.");
        return;
    }

    query.prepare("INSERT INTO odunc_alinan (uye_no, kitap_no, odunc_alma_tarihi) VALUES (:uye_no, :kitap_no, :odunc_alma_tarihi)");
    query.bindValue(":uye_no", uyeNo);
    query.bindValue(":kitap_no", kitapNo);
    query.bindValue(":odunc_alma_tarihi", oduncAlmaStr);

    if (query.exec()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi - 1 WHERE kitap_no = :kitap_no");
        updateQuery.bindValue(":kitap_no", kitapNo);
        updateQuery.exec();

        QMessageBox::information(this, "Başarılı", "Kitap başarıyla ödünç alındı.");
        oduncleriListele();
        kitaplariListele();
    } else {
        QMessageBox::critical(this, "Hata", "Kitap ödünç alınamadı: " + query.lastError().text());
    }
}


void oduncalmaislem::kitapSecildi(const QModelIndex &index)
{
    int row = index.row();
    ui->kitapNoLN->setText(kitapModel->index(row, 0).data().toString());
}

void oduncalmaislem::uyeSecildi(const QModelIndex &index)
{
    int row = index.row();
    ui->uyeNoLN->setText(uyeModel->index(row, 0).data().toString());

}
