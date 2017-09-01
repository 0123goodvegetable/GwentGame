/********************************************************************************
** Form generated from reading UI file 'GameSelectionBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMESELECTIONBACKGROUND_H
#define UI_GAMESELECTIONBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameSelectionBackground
{
public:

    void setupUi(QWidget *GameSelectionBackground)
    {
        if (GameSelectionBackground->objectName().isEmpty())
            GameSelectionBackground->setObjectName(QStringLiteral("GameSelectionBackground"));
        GameSelectionBackground->resize(400, 300);

        retranslateUi(GameSelectionBackground);

        QMetaObject::connectSlotsByName(GameSelectionBackground);
    } // setupUi

    void retranslateUi(QWidget *GameSelectionBackground)
    {
        GameSelectionBackground->setWindowTitle(QApplication::translate("GameSelectionBackground", "GameSelectionBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GameSelectionBackground: public Ui_GameSelectionBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMESELECTIONBACKGROUND_H
