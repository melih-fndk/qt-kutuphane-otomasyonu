// melih fındık 22100011064
#include "oduncteslimetmeislem.h"
#include "ui_oduncteslimetmeislem.h"

oduncteslimetmeislem::oduncteslimetmeislem(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::oduncteslimetmeislem)
{
    ui->setupUi(this);
    oduncModel = new QSqlQueryModel(this);
    teslimModel = new QSqlQueryModel(this);

    oduncleriListele();
    teslimleriListele();

    connect(ui->oduncVerPB, &QPushButton::clicked, this, &oduncteslimetmeislem::oduncTeslimEt);
    connect(ui->tableViewOduncAlnan->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &oduncteslimetmeislem::oduncSecildi);

}

oduncteslimetmeislem::~oduncteslimetmeislem()
{
    delete ui;
}

void oduncteslimetmeislem::oduncTeslimEt()
{
    int kitapNo = ui->kitapNoLN->text().toInt();
    int uyeNo = ui->uyeNoLN->text().toInt();
    QDate teslimTarihi = ui->dateEdit->date();
    QString teslimTarihiStr = teslimTarihi.toString("yyyy-MM-dd");

    if (kitapNo == 0 || uyeNo == 0) {
        QMessageBox::warning(this, "Hata", "Lütfen kitap ve üye seçin.");
        return;
    }

    QSqlQuery almaQuery;
    almaQuery.prepare("SELECT odunc_alma_tarihi FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    almaQuery.bindValue(":uye_no", uyeNo);
    almaQuery.bindValue(":kitap_no", kitapNo);
    almaQuery.exec();

    if (almaQuery.next()) {
        QDate oduncAlmaTarihi = almaQuery.value(0).toDate();
        QString oduncAlmaStr = oduncAlmaTarihi.toString("yyyy-MM-dd");
        int borc = hesaplaBorc(oduncAlmaTarihi, teslimTarihi);

        QSqlQuery query;
        query.prepare("INSERT INTO odunc_teslim_edilen (uye_no, kitap_no, alma_tarihi, verme_tarihi, borc) "
                      "VALUES (:uye_no, :kitap_no, :alma_tarihi, :verme_tarihi, :borc)");
        query.bindValue(":uye_no", uyeNo);
        query.bindValue(":kitap_no", kitapNo);
        query.bindValue(":alma_tarihi", oduncAlmaStr);
        query.bindValue(":verme_tarihi", teslimTarihiStr);
        query.bindValue(":borc", borc);


        if (query.exec()) {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
            deleteQuery.bindValue(":uye_no", uyeNo);
            deleteQuery.bindValue(":kitap_no", kitapNo);
            deleteQuery.exec();

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi + 1 WHERE kitap_no = :kitap_no");
            updateQuery.bindValue(":kitap_no", kitapNo);
            updateQuery.exec();

            QMessageBox::information(this, "Başarılı", "Kitap başarıyla teslim edildi.");
            oduncleriListele();
            teslimleriListele();
        } else {
            QMessageBox::critical(this, "Hata", "Kitap teslim edilemedi: " + query.lastError().text());
        }
    } else {
        QMessageBox::critical(this, "Hata", "Kitap ödünç alma kaydı bulunamadı.");
    }
}

void oduncteslimetmeislem::oduncSecildi(const QModelIndex &index)
{
    int row = index.row();
    int uyeNo = oduncModel->index(row, 0).data().toInt();

    ui->uyeNoLN->setText(QString::number(uyeNo));
    ui->kitapNoLN->setText(oduncModel->index(row, 1).data().toString());
}

void oduncteslimetmeislem::oduncleriListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM odunc_alinan");

    oduncModel->setQuery(query);
    ui->tableViewOduncAlnan->setModel(oduncModel);
}

void oduncteslimetmeislem::teslimleriListele()
{
    QSqlQuery query;
    query.exec("SELECT * FROM odunc_teslim_edilen");

    teslimModel->setQuery(query);
    ui->tableViewOduncTeslim->setModel(teslimModel);
}

int oduncteslimetmeislem::hesaplaBorc(const QDate &almaTarihi, const QDate &teslimTarihi)
{
    int gecikme = almaTarihi.daysTo(teslimTarihi) - 15;
    return (gecikme > 0) ? gecikme * 2 : 0;
}
