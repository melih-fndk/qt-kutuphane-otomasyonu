// melih fındık 22100011064
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QSqlError>
#include <QMessageBox>
#include "uyeislemleri.h"
#include "kitapislemleri.h"
#include "oduncalmaislem.h"
#include "oduncteslimetmeislem.h"
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(QSqlDatabase::addDatabase("QSQLITE"))
{
    ui->setupUi(this);

    if (createConnection()) {
        statusBar()->showMessage("Veritabanına başarıyla bağlanıldı.");
    } else {
        statusBar()->showMessage("Veritabanına bağlanılamadı.");
    }

    connect(ui->uyeislempushButton, &QPushButton::clicked, this, &MainWindow::UyeIslemleri);
    connect(ui->kitapislempushButton, &QPushButton::clicked, this, &MainWindow::KitapIslemleri);
    connect(ui->oduncalmapushButton, &QPushButton::clicked, this, &MainWindow::OduncAlmaIslemleri);
    connect(ui->oduncteslimpushButton, &QPushButton::clicked, this, &MainWindow::OduncTeslimIslemleri);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::KitapIslemleri()
{
    kitapislemleri *kitapislem = new kitapislemleri();
    kitapislem->exec();
}

void MainWindow::UyeIslemleri()
{
    uyeislemleri *uyeislem = new uyeislemleri();
    uyeislem->exec();
}

void MainWindow::OduncAlmaIslemleri()
{
    oduncalmaislem *oduncislem = new oduncalmaislem();
    oduncislem->exec();
}

void MainWindow::OduncTeslimIslemleri()
{
    oduncteslimetmeislem *teslimislem = new oduncteslimetmeislem();
    teslimislem->exec();
}

bool MainWindow::createConnection() {
    db.setDatabaseName("kutuphane.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Veritabanı Hatası",
                              "Veritabanı bağlantısı kurulamadı:\n" + db.lastError().text());
        return false;
    }

    QFileInfo dbInfo(db.databaseName());
    QString yol = dbInfo.absoluteFilePath();
    QMessageBox::information(this, "Veritabanı yolu", "Kullanılan DB:\n" + yol);

    return true;
}

