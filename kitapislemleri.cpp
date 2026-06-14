// melih fındık 22100011064
#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

kitapislemleri::kitapislemleri(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kitapislemleri)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    modelOduncAlinan = new QSqlQueryModel(this);
    modelOduncTeslimEdilen = new QSqlQueryModel(this);
    kitaplariListele();
    connect(ui->yenikayitPB, &QPushButton::clicked, this, &kitapislemleri::yeniKayit);
    connect(ui->guncellePB, &QPushButton::clicked, this, &kitapislemleri::guncelle);
    connect(ui->silPB, &QPushButton::clicked, this, &kitapislemleri::sil);
    connect(ui->tableViewKitaplar->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &kitapislemleri::kitapSecildi);
}

kitapislemleri::~kitapislemleri()
{
    delete ui;
}

void kitapislemleri::kitaplariListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM kitap");

    model->setQuery(query);
    ui->tableViewKitaplar->setModel(model);
}

void kitapislemleri::yeniKayit()
{
    QString kitapAd = ui->lineEditKitapAd->text();
    int kitapSayisi = ui->lineEditKitapSayisi->text().toInt();

    if (kitapAd.isEmpty() || kitapSayisi <= 0) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm alanları doldurun.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO kitap (kitap_ad, kitap_sayisi) VALUES (:kitap_ad, :kitap_sayisi)");
    query.bindValue(":kitap_ad", kitapAd);
    query.bindValue(":kitap_sayisi", kitapSayisi);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Yeni kitap başarıyla eklendi.");
        kitaplariListele();
    } else {
        QMessageBox::critical(this, "Hata", "Kitap eklenemedi: " + query.lastError().text());
    }
}

void kitapislemleri::guncelle()
{
    int kitapNo = ui->lineEditKitapNo->text().toInt();
    QString kitapAd = ui->lineEditKitapAd->text();
    int kitapSayisi = ui->lineEditKitapSayisi->text().toInt();

    if (kitapAd.isEmpty() || kitapSayisi <= 0) {
        QMessageBox::warning(this, "Hata", "Lütfen tüm alanları doldurun.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE kitap SET kitap_ad = :kitap_ad, kitap_sayisi = :kitap_sayisi WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_ad", kitapAd);
    query.bindValue(":kitap_sayisi", kitapSayisi);
    query.bindValue(":kitap_no", kitapNo);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Kitap başarıyla güncellendi.");
        kitaplariListele();
    } else {
        QMessageBox::critical(this, "Hata", "Kitap güncellenemedi: " + query.lastError().text());
    }
}

void kitapislemleri::sil()
{
    int kitapNo = ui->lineEditKitapNo->text().toInt();


    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM odunc_alinan WHERE kitap_no = :kitap_no");
    checkQuery.bindValue(":kitap_no", kitapNo);
    checkQuery.exec();
    checkQuery.next();

    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Hata", "Bu kitap şu anda ödünç alınmış durumda. Kitap silinemez.");
        return;
    }


    QSqlQuery query;
    query.prepare("DELETE FROM kitap WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_no", kitapNo);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Kitap başarıyla silindi.");
        kitaplariListele();
    } else {
        QMessageBox::critical(this, "Hata", "Kitap silinemedi: " + query.lastError().text());
    }
}

void kitapislemleri::kitapSecildi(const QModelIndex &index)
{
    int row = index.row();
    int kitapNo = model->index(row, 0).data().toInt();

    ui->lineEditKitapNo->setText(QString::number(kitapNo));
    ui->lineEditKitapAd->setText(model->index(row, 1).data().toString());
    ui->lineEditKitapSayisi->setText(model->index(row, 2).data().toString());
    oduncDurumlariniGoster(kitapNo);
}

void kitapislemleri::oduncDurumlariniGoster(int kitapNo)
{
    QSqlQuery queryOduncAlinan;
    queryOduncAlinan.prepare("SELECT * FROM odunc_alinan WHERE kitap_no = :kitap_no");
    queryOduncAlinan.bindValue(":kitap_no", kitapNo);
    queryOduncAlinan.exec();

    modelOduncAlinan->setQuery(queryOduncAlinan);
    ui->tableViewOduncDurum->setModel(modelOduncAlinan);

    QSqlQuery queryOduncTeslimEdilen;
    queryOduncTeslimEdilen.prepare("SELECT * FROM odunc_teslim_edilen WHERE kitap_no = :kitap_no");
    queryOduncTeslimEdilen.bindValue(":kitap_no", kitapNo);
    queryOduncTeslimEdilen.exec();

    modelOduncTeslimEdilen->setQuery(queryOduncTeslimEdilen);
    ui->tableViewOduncTeslim->setModel(modelOduncTeslimEdilen);
}
