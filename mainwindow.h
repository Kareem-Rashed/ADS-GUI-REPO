#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectFile_clicked();
    void on_compressWithEncryption_clicked();
    void on_decompress_clicked();


private:
    Ui::MainWindow *ui;
    QString selectedFilePath; // Stores the selected file path
};

#endif // MAINWINDOW_H
