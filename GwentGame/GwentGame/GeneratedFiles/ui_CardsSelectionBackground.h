/********************************************************************************
** Form generated from reading UI file 'CardsSelectionBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARDSSELECTIONBACKGROUND_H
#define UI_CARDSSELECTIONBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CardsSelectionBackground
{
public:

    void setupUi(QWidget *CardsSelectionBackground)
    {
        if (CardsSelectionBackground->objectName().isEmpty())
            CardsSelectionBackground->setObjectName(QStringLiteral("CardsSelectionBackground"));
        CardsSelectionBackground->resize(400, 300);

        retranslateUi(CardsSelectionBackground);

        QMetaObject::connectSlotsByName(CardsSelectionBackground);
    } // setupUi

    void retranslateUi(QWidget *CardsSelectionBackground)
    {
        CardsSelectionBackground->setWindowTitle(QApplication::translate("CardsSelectionBackground", "CardsSelectionBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CardsSelectionBackground: public Ui_CardsSelectionBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARDSSELECTIONBACKGROUND_H
