/********************************************************************************
** Form generated from reading UI file 'CardsEditBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARDSEDITBACKGROUND_H
#define UI_CARDSEDITBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CardsEditBackground
{
public:

    void setupUi(QWidget *CardsEditBackground)
    {
        if (CardsEditBackground->objectName().isEmpty())
            CardsEditBackground->setObjectName(QStringLiteral("CardsEditBackground"));
        CardsEditBackground->resize(400, 300);

        retranslateUi(CardsEditBackground);

        QMetaObject::connectSlotsByName(CardsEditBackground);
    } // setupUi

    void retranslateUi(QWidget *CardsEditBackground)
    {
        CardsEditBackground->setWindowTitle(QApplication::translate("CardsEditBackground", "CardsEditBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CardsEditBackground: public Ui_CardsEditBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARDSEDITBACKGROUND_H
