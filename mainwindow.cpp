#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include "huffman.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_selectFile_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_compressWithEncryption_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_decompress_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_selectFile_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select File"), "", tr("All Files (*.*)"));
    if (!fileName.isEmpty()) {
        selectedFilePath = fileName;
        //label to show the file selected
        ui->label->setText("Selected File: " + selectedFilePath);
    } else {
        QMessageBox::warning(this, tr("No File Selected"), tr("Please select a file to proceed."));
    }
}



void MainWindow::on_compressWithEncryption_clicked() {
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("No File Selected"), tr("Please select a file first."));
        return;
    }

    //Ask for encryption type
    QStringList encryptionOptions;
    encryptionOptions << "None" << "Caesar";
    bool ok;
    QString encryptionType = QInputDialog::getItem(
        this,
        tr("Select Encryption Type"),
        tr("Select encryption method:"),
        encryptionOptions,
        0,
        false,
        &ok
        );
    if (!ok || encryptionType.isEmpty()) {
        QMessageBox::warning(this, tr("No Encryption Type Selected"), tr("Please select an encryption type."));
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Compressed File As"),
        "",
        tr("Compressed Files (*.txt);;All Files (*)")
        );
    if (outputFileName.isEmpty()) {
        QMessageBox::warning(this, tr("No Output File"), tr("Please specify an output file."));
        return;
    }

    //output file name has .txt extension
    if (!outputFileName.endsWith(".txt", Qt::CaseInsensitive)) {
        outputFileName += ".txt";
    }

    //compression  using Huffman
    try {

        Huffman huffman(selectedFilePath.toStdString());
        if(encryptionType.toStdString()=="None")
        {
            huffman.Compress("Normal", outputFileName.toStdString());
        }
        else if(encryptionType.toStdString()=="Caesar")
        {
            huffman.Compress("Caesar", outputFileName.toStdString());
        }

        QMessageBox::information(this, tr("Success"), tr("File compressed with encryption successfully."));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), tr("Compression failed: %1").arg(e.what()));
    }
}

void MainWindow::on_decompress_clicked() {
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("No File Selected"), tr("Please select a compressed file first."));
        return;
    }

    QString outputFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Decompressed File As"),
        "",
        tr("Text Files (*.txt);;All Files (*)")
        );
    if (outputFileName.isEmpty()) {
        QMessageBox::warning(this, tr("No Output File"), tr("Please specify an output file."));
        return;
    }

    // Ensure the output file name has a .txt extension
    if (!outputFileName.endsWith(".txt", Qt::CaseInsensitive)) {
        outputFileName += ".txt";
    }

    // Ask for encryption type
    QStringList encryptionOptions;
    encryptionOptions << "None" << "Caesar";
    bool ok;
    QString encryptionType = QInputDialog::getItem(
        this,
        tr("Which encryption was used?"),
        tr("Encryption Type used during compression:"),
        encryptionOptions,
        0,
        false,
        &ok
        );
    if (!ok || encryptionType.isEmpty()) {
        QMessageBox::warning(this, tr("No Encryption Type Selected"), tr("Please select an encryption type."));
        return;
    }

    //decompression using Huffman
    try {
        Huffman huffman(selectedFilePath.toStdString());
        if(encryptionType.toStdString()=="None")
        {
            huffman.Decompress("Normal", outputFileName.toStdString());
        }
        else if(encryptionType.toStdString()=="Caesar")
        {
            huffman.Decompress("Caesar", outputFileName.toStdString());
        }

        QMessageBox::information(this, tr("Success"), tr("File decompressed successfully."));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), tr("Decompression failed: %1").arg(e.what()));
    }
}

