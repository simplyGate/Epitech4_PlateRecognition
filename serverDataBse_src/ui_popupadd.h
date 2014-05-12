/********************************************************************************
** Form generated from reading UI file 'popupadd.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUPADD_H
#define UI_POPUPADD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PopUpAdd
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PopUpAdd)
    {
        if (PopUpAdd->objectName().isEmpty())
            PopUpAdd->setObjectName(QStringLiteral("PopUpAdd"));
        PopUpAdd->resize(400, 300);
        verticalLayout = new QVBoxLayout(PopUpAdd);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));

        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(PopUpAdd);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(PopUpAdd);
        QObject::connect(buttonBox, SIGNAL(accepted()), PopUpAdd, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PopUpAdd, SLOT(reject()));

        QMetaObject::connectSlotsByName(PopUpAdd);
    } // setupUi

    void retranslateUi(QDialog *PopUpAdd)
    {
        PopUpAdd->setWindowTitle(QApplication::translate("PopUpAdd", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class PopUpAdd: public Ui_PopUpAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUPADD_H
