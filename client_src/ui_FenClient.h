/********************************************************************************
** Form generated from reading UI file 'FenClient.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FENCLIENT_H
#define UI_FENCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FenClient
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *serveurIP;
    QLabel *label_2;
    QSpinBox *serveurPort;
    QPushButton *boutonConnexion;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QTextEdit *listeMessages;
    QLabel *imageView;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *message;
    QPushButton *pushButton;
    QPushButton *btnSend;
    QPushButton *boutonEnvoyer;

    void setupUi(QWidget *FenClient)
    {
        if (FenClient->objectName().isEmpty())
            FenClient->setObjectName(QStringLiteral("FenClient"));
        FenClient->resize(508, 480);
        QIcon icon;
        icon.addFile(QStringLiteral("chat.png"), QSize(), QIcon::Normal, QIcon::Off);
        FenClient->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(FenClient);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(FenClient);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        serveurIP = new QLineEdit(FenClient);
        serveurIP->setObjectName(QStringLiteral("serveurIP"));
        serveurIP->setMaximumSize(QSize(150, 16777215));

        horizontalLayout->addWidget(serveurIP);

        label_2 = new QLabel(FenClient);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        serveurPort = new QSpinBox(FenClient);
        serveurPort->setObjectName(QStringLiteral("serveurPort"));
        serveurPort->setMinimumSize(QSize(60, 0));
        serveurPort->setMinimum(1024);
        serveurPort->setMaximum(65535);
        serveurPort->setValue(4242);

        horizontalLayout->addWidget(serveurPort);

        boutonConnexion = new QPushButton(FenClient);
        boutonConnexion->setObjectName(QStringLiteral("boutonConnexion"));

        horizontalLayout->addWidget(boutonConnexion);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        listeMessages = new QTextEdit(FenClient);
        listeMessages->setObjectName(QStringLiteral("listeMessages"));
        listeMessages->setReadOnly(true);

        verticalLayout_2->addWidget(listeMessages);

        imageView = new QLabel(FenClient);
        imageView->setObjectName(QStringLiteral("imageView"));

        verticalLayout_2->addWidget(imageView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        message = new QLineEdit(FenClient);
        message->setObjectName(QStringLiteral("message"));

        horizontalLayout_2->addWidget(message);

        pushButton = new QPushButton(FenClient);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(false);

        horizontalLayout_2->addWidget(pushButton);

        btnSend = new QPushButton(FenClient);
        btnSend->setObjectName(QStringLiteral("btnSend"));
        btnSend->setEnabled(false);

        horizontalLayout_2->addWidget(btnSend);

        boutonEnvoyer = new QPushButton(FenClient);
        boutonEnvoyer->setObjectName(QStringLiteral("boutonEnvoyer"));
        boutonEnvoyer->setEnabled(false);
        boutonEnvoyer->setIcon(icon);

        horizontalLayout_2->addWidget(boutonEnvoyer);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(FenClient);

        QMetaObject::connectSlotsByName(FenClient);
    } // setupUi

    void retranslateUi(QWidget *FenClient)
    {
        FenClient->setWindowTitle(QApplication::translate("FenClient", "License Plate - Client", 0));
        label->setText(QApplication::translate("FenClient", "IP du serveur :", 0));
        serveurIP->setText(QApplication::translate("FenClient", "127.0.0.1", 0));
        label_2->setText(QApplication::translate("FenClient", "Port du serveur :", 0));
        boutonConnexion->setText(QApplication::translate("FenClient", "Connexion", 0));
        imageView->setText(QString());
        pushButton->setText(QApplication::translate("FenClient", "Browse", 0));
        btnSend->setText(QApplication::translate("FenClient", "Send", 0));
        boutonEnvoyer->setText(QApplication::translate("FenClient", "Status", 0));
    } // retranslateUi

};

namespace Ui {
    class FenClient: public Ui_FenClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FENCLIENT_H
