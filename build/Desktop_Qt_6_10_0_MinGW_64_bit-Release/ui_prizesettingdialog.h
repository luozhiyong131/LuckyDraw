/********************************************************************************
** Form generated from reading UI file 'prizesettingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIZESETTINGDIALOG_H
#define UI_PRIZESETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_PrizeSettingDialog
{
public:

    void setupUi(QDialog *PrizeSettingDialog)
    {
        if (PrizeSettingDialog->objectName().isEmpty())
            PrizeSettingDialog->setObjectName("PrizeSettingDialog");
        PrizeSettingDialog->resize(400, 300);

        retranslateUi(PrizeSettingDialog);

        QMetaObject::connectSlotsByName(PrizeSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *PrizeSettingDialog)
    {
        PrizeSettingDialog->setWindowTitle(QCoreApplication::translate("PrizeSettingDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrizeSettingDialog: public Ui_PrizeSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIZESETTINGDIALOG_H
