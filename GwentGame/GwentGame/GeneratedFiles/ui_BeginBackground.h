/********************************************************************************
** Form generated from reading UI file 'BeginBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BEGINBACKGROUND_H
#define UI_BEGINBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BeginBackground
{
public:

    void setupUi(QWidget *BeginBackground)
    {
        if (BeginBackground->objectName().isEmpty())
            BeginBackground->setObjectName(QStringLiteral("BeginBackground"));
        BeginBackground->resize(400, 300);

        retranslateUi(BeginBackground);

        QMetaObject::connectSlotsByName(BeginBackground);
    } // setupUi

    void retranslateUi(QWidget *BeginBackground)
    {
        BeginBackground->setWindowTitle(QApplication::translate("BeginBackground", "BeginBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BeginBackground: public Ui_BeginBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BEGINBACKGROUND_H
