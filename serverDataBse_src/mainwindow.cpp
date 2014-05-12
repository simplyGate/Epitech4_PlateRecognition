#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <iostream>
#include <vector>

#include "ORM/ORM.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  initDbTab();
  initNetwork();


  // QWidget*	test = new QWidget;
  QWidget*	test = &m_img_process_widget;

  ui->mainTab->addTab( test, QString("Image processing"));


}

MainWindow::~MainWindow()
{
  delete ui;
  delete serv;
}

void    MainWindow::initDbTab()
{
    QString dbName = "Database.db";
    QFile testFile(dbName);
    if (!testFile.exists())
    {
        std::cerr << "Path to Database is wrong" << std::endl;
        exit(0);
    }
    else
    {
        ORM::DbManager::OpenDb( dbName.toStdString().c_str());
    }

    // Plate

    QSqlTableModel *modelPlate = new QSqlTableModel(this);
    modelPlate->setTable(QString::fromStdString(ORM::Plate::Table));

    modelPlate->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelPlate->select();

    modelPlate->setHeaderData(0, Qt::Horizontal, QObject::tr(ORM::Plate::Field_id.c_str()));
    modelPlate->setHeaderData(1, Qt::Horizontal, QObject::tr(ORM::Plate::Field_plateNumber.c_str()));
    modelPlate->setHeaderData(2, Qt::Horizontal, QObject::tr(ORM::Plate::Field_userName.c_str()));
    modelPlate->setHeaderData(3, Qt::Horizontal, QObject::tr(ORM::Plate::Field_isWanted.c_str()));

    ui->tableView->setModel(modelPlate);

    //Image

    QSqlTableModel *modelImage = new QSqlTableModel(this);
    modelImage->setTable(QString::fromStdString(ORM::Image::Table));

    modelImage->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelImage->select();

    modelImage->setHeaderData(0, Qt::Horizontal, QObject::tr(ORM::Image::Field_id.c_str()));
    modelImage->setHeaderData(1, Qt::Horizontal, QObject::tr(ORM::Image::Field_pathName.c_str()));
    modelImage->setHeaderData(2, Qt::Horizontal, QObject::tr(ORM::Image::Field_attachedPlate.c_str()));
    modelImage->setHeaderData(3, Qt::Horizontal, QObject::tr(ORM::Image::Field_status.c_str()));

    ui->tableView_2->setModel(modelImage);

    //Log

    QSqlTableModel *modelLog = new QSqlTableModel(this);
    modelLog->setTable(QString::fromStdString(ORM::Log::Table));

    modelLog->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelLog->select();

    modelLog->setHeaderData(0, Qt::Horizontal, QObject::tr(ORM::Log::Field_id.c_str()));
    modelLog->setHeaderData(1, Qt::Horizontal, QObject::tr(ORM::Log::Field_warningLevel.c_str()));
    modelLog->setHeaderData(2, Qt::Horizontal, QObject::tr(ORM::Log::Field_attachedImage.c_str()));
    modelLog->setHeaderData(3, Qt::Horizontal, QObject::tr(ORM::Log::Field_createDate.c_str()));

    ui->tableView_3->setModel(modelLog);
}

void MainWindow::initNetwork()
{
    serv = new Server();
    ui->LayoutNetwork->addWidget(serv);
}

void MainWindow::on_pushButton_7_clicked()
{
  ui->tableView->model()->insertRow(ui->tableView->model()->rowCount());
}

void MainWindow::on_pushButton_6_clicked()
{
  ui->tableView_2->model()->insertRow(ui->tableView_2->model()->rowCount());
}

void MainWindow::on_pushButton_8_clicked()
{
    ((QSqlTableModel *)ui->tableView_2->model())->submitAll();
}

void MainWindow::on_pushButton_9_clicked()
{
    ((QSqlTableModel *)ui->tableView->model())->submitAll();
}

void MainWindow::on_pushButton_10_clicked()
{
    ((QSqlTableModel *)ui->tableView->model())->revertAll();
    ((QSqlTableModel *)ui->tableView->model())->submitAll();
}

void MainWindow::on_pushButton_11_clicked()
{
    ((QSqlTableModel *)ui->tableView_2->model())->revertAll();
    ((QSqlTableModel *)ui->tableView_2->model())->submitAll();
}

void MainWindow::on_pushButton_13_clicked()
{
    QItemSelection selection( ui->tableView->selectionModel()->selection() );

    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
    rows.append( index.row() );
    }

    qSort( rows );

    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
    int current = rows[i];
    if( current != prev ) {
    ui->tableView->model()->removeRows( current, 1 );
    prev = current;
    }
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    QItemSelection selection( ui->tableView_2->selectionModel()->selection() );

    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
    rows.append( index.row() );
    }

    qSort( rows );

    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
    int current = rows[i];
    if( current != prev ) {
    ui->tableView_2->model()->removeRows( current, 1 );
    prev = current;
    }
    }
}


void MainWindow::on_pushButton_14_clicked()
{
    QItemSelection selection( ui->tableView_3->selectionModel()->selection() );

    QList<int> rows;
    foreach( const QModelIndex & index, selection.indexes() ) {
    rows.append( index.row() );
    }

    qSort( rows );

    int prev = -1;
    for( int i = rows.count() - 1; i >= 0; i -= 1 ) {
    int current = rows[i];
    if( current != prev ) {
    ui->tableView_3->model()->removeRows( current, 1 );
    prev = current;
    }
    }
}

void MainWindow::on_pushButton_16_clicked()
{
   ((QSqlTableModel *)ui->tableView_3->model())->submitAll();
}

void MainWindow::on_pushButton_15_clicked()
{
    ((QSqlTableModel *)ui->tableView_3->model())->revertAll();
    ((QSqlTableModel *)ui->tableView_3->model())->submitAll();
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (arg1.size() == 0)
    {
        for (int i = 0; i < ui->tableView->model()->rowCount(); i++)
        {
            ui->tableView->showRow(i);
        }
    }
    bool match;
    for (int i = 0; i < ui->tableView->model()->rowCount(); i++)
    {
        match = false;
        for (int j = 0; j < ui->tableView->model()->columnCount(); j++)
        {
            QModelIndex qmi = ui->tableView->model()->index(i, j);
            if (qmi.data().toString().contains(arg1, Qt::CaseInsensitive))
            {
                match = true;
            }
        }
        if (match == true)
            ui->tableView->showRow(i);
        else
            ui->tableView->hideRow(i);
    }
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (arg1.size() == 0)
    {
        for (int i = 0; i < ui->tableView_2->model()->rowCount(); i++)
        {
            ui->tableView_2->showRow(i);
        }
    }
    bool match;
    for (int i = 0; i < ui->tableView_2->model()->rowCount(); i++)
    {
        match = false;
        for (int j = 0; j < ui->tableView_2->model()->columnCount(); j++)
        {
            QModelIndex qmi = ui->tableView_2->model()->index(i, j);
            if (qmi.data().toString().contains(arg1, Qt::CaseInsensitive))
            {
                match = true;
            }
        }
        if (match == true)
            ui->tableView_2->showRow(i);
        else
            ui->tableView_2->hideRow(i);
    }
}

void MainWindow::on_lineEdit_4_textChanged(const QString &arg1)
{
    if (arg1.size() == 0)
    {
        for (int i = 0; i < ui->tableView_3->model()->rowCount(); i++)
        {
            ui->tableView_3->showRow(i);
        }
    }
    bool match;
    for (int i = 0; i < ui->tableView_3->model()->rowCount(); i++)
    {
        match = false;
        for (int j = 0; j < ui->tableView_3->model()->columnCount(); j++)
        {
            QModelIndex qmi = ui->tableView_3->model()->index(i, j);
            if (qmi.data().toString().contains(arg1, Qt::CaseInsensitive))
            {
                match = true;
            }
        }
        if (match == true)
            ui->tableView_3->showRow(i);
        else
            ui->tableView_3->hideRow(i);
    }
}

void MainWindow::on_ButtonUpdateDB_clicked()
{
    ((QSqlTableModel *)ui->tableView->model())->select();
    ((QSqlTableModel *)ui->tableView_2->model())->select();
    ((QSqlTableModel *)ui->tableView_3->model())->select();
}
