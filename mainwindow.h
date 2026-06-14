// melih fındık 22100011064
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase& getDatabase();

private slots:
    void KitapIslemleri();
    void UyeIslemleri();
    void OduncAlmaIslemleri();
    void OduncTeslimIslemleri();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    bool createConnection();
};

#endif // MAINWINDOW_H
