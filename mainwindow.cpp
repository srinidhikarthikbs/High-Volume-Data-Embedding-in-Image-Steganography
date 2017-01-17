#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "embedding.h"
#include "extraction.h"
#include<QFileDialog>
#include<qdebug.h>
#include<QMessageBox>
#include<QDir>
//QString path="Please select an image file";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->label->setText(path);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    embedding *e=new embedding();
    e->show();

}

void MainWindow::on_pushButton_2_clicked()
{
    extraction *e=new extraction();
    e->show();
}
