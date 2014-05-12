/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *mainTab;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    QVBoxLayout *LayoutNetwork;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout_2;
    QTabWidget *PlateTab;
    QWidget *Plate;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_5;
    QTableView *tableView;
    QPushButton *pushButton_7;
    QPushButton *pushButton_13;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QLineEdit *lineEdit_2;
    QWidget *Users;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout_6;
    QPushButton *pushButton_6;
    QTableView *tableView_2;
    QPushButton *pushButton_12;
    QPushButton *pushButton_8;
    QPushButton *pushButton_11;
    QLineEdit *lineEdit_3;
    QWidget *Log;
    QVBoxLayout *verticalLayout_7;
    QGridLayout *gridLayout_7;
    QTableView *tableView_3;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QPushButton *pushButton_14;
    QLineEdit *lineEdit_4;
    QPushButton *ButtonUpdateDB;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(489, 427);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        mainTab = new QTabWidget(centralWidget);
        mainTab->setObjectName(QStringLiteral("mainTab"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        LayoutNetwork = new QVBoxLayout();
        LayoutNetwork->setSpacing(6);
        LayoutNetwork->setObjectName(QStringLiteral("LayoutNetwork"));

        gridLayout_4->addLayout(LayoutNetwork, 0, 0, 1, 1);

        mainTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        PlateTab = new QTabWidget(tab_2);
        PlateTab->setObjectName(QStringLiteral("PlateTab"));
        Plate = new QWidget();
        Plate->setObjectName(QStringLiteral("Plate"));
        verticalLayout_5 = new QVBoxLayout(Plate);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        tableView = new QTableView(Plate);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setSortingEnabled(true);

        gridLayout_5->addWidget(tableView, 1, 0, 1, 5);

        pushButton_7 = new QPushButton(Plate);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        gridLayout_5->addWidget(pushButton_7, 2, 0, 1, 3);

        pushButton_13 = new QPushButton(Plate);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));

        gridLayout_5->addWidget(pushButton_13, 3, 0, 1, 3);

        pushButton_9 = new QPushButton(Plate);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        gridLayout_5->addWidget(pushButton_9, 2, 3, 2, 1);

        pushButton_10 = new QPushButton(Plate);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));

        gridLayout_5->addWidget(pushButton_10, 2, 4, 2, 1);

        lineEdit_2 = new QLineEdit(Plate);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        gridLayout_5->addWidget(lineEdit_2, 0, 0, 1, 5);


        verticalLayout_5->addLayout(gridLayout_5);

        PlateTab->addTab(Plate, QString());
        Users = new QWidget();
        Users->setObjectName(QStringLiteral("Users"));
        verticalLayout_6 = new QVBoxLayout(Users);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        pushButton_6 = new QPushButton(Users);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        gridLayout_6->addWidget(pushButton_6, 2, 0, 1, 3);

        tableView_2 = new QTableView(Users);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setSortingEnabled(true);

        gridLayout_6->addWidget(tableView_2, 1, 0, 1, 5);

        pushButton_12 = new QPushButton(Users);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        gridLayout_6->addWidget(pushButton_12, 3, 0, 1, 3);

        pushButton_8 = new QPushButton(Users);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        gridLayout_6->addWidget(pushButton_8, 2, 3, 2, 1);

        pushButton_11 = new QPushButton(Users);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));

        gridLayout_6->addWidget(pushButton_11, 2, 4, 2, 1);

        lineEdit_3 = new QLineEdit(Users);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        gridLayout_6->addWidget(lineEdit_3, 0, 0, 1, 5);


        verticalLayout_6->addLayout(gridLayout_6);

        PlateTab->addTab(Users, QString());
        Log = new QWidget();
        Log->setObjectName(QStringLiteral("Log"));
        verticalLayout_7 = new QVBoxLayout(Log);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        tableView_3 = new QTableView(Log);
        tableView_3->setObjectName(QStringLiteral("tableView_3"));
        tableView_3->setSortingEnabled(true);

        gridLayout_7->addWidget(tableView_3, 1, 0, 1, 4);

        pushButton_15 = new QPushButton(Log);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));

        gridLayout_7->addWidget(pushButton_15, 2, 3, 1, 1);

        pushButton_16 = new QPushButton(Log);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));

        gridLayout_7->addWidget(pushButton_16, 2, 2, 1, 1);

        pushButton_14 = new QPushButton(Log);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));

        gridLayout_7->addWidget(pushButton_14, 2, 0, 1, 2);

        lineEdit_4 = new QLineEdit(Log);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        gridLayout_7->addWidget(lineEdit_4, 0, 0, 1, 4);


        verticalLayout_7->addLayout(gridLayout_7);

        PlateTab->addTab(Log, QString());

        gridLayout_2->addWidget(PlateTab, 0, 0, 1, 1);

        ButtonUpdateDB = new QPushButton(tab_2);
        ButtonUpdateDB->setObjectName(QStringLiteral("ButtonUpdateDB"));

        gridLayout_2->addWidget(ButtonUpdateDB, 1, 0, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        mainTab->addTab(tab_2, QString());

        verticalLayout_2->addWidget(mainTab);


        verticalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 489, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        mainTab->setCurrentIndex(1);
        PlateTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        mainTab->setTabText(mainTab->indexOf(tab), QApplication::translate("MainWindow", "Network", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "Add Row", 0));
        pushButton_13->setText(QApplication::translate("MainWindow", "Remove Rows", 0));
        pushButton_9->setText(QApplication::translate("MainWindow", "Submit", 0));
        pushButton_10->setText(QApplication::translate("MainWindow", "Cancel", 0));
        PlateTab->setTabText(PlateTab->indexOf(Plate), QApplication::translate("MainWindow", "Plate / User", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "Add Row", 0));
        pushButton_12->setText(QApplication::translate("MainWindow", "Delete Rows", 0));
        pushButton_8->setText(QApplication::translate("MainWindow", "Submit", 0));
        pushButton_11->setText(QApplication::translate("MainWindow", "Cancel", 0));
        PlateTab->setTabText(PlateTab->indexOf(Users), QApplication::translate("MainWindow", "Image", 0));
        pushButton_15->setText(QApplication::translate("MainWindow", "Cancel", 0));
        pushButton_16->setText(QApplication::translate("MainWindow", "Submit", 0));
        pushButton_14->setText(QApplication::translate("MainWindow", "Delete Rows", 0));
        PlateTab->setTabText(PlateTab->indexOf(Log), QApplication::translate("MainWindow", "Log", 0));
        ButtonUpdateDB->setText(QApplication::translate("MainWindow", "Update Database", 0));
        mainTab->setTabText(mainTab->indexOf(tab_2), QApplication::translate("MainWindow", "DataBase", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
