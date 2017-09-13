/********************************************************************************
** Form generated from reading UI file 'GameEndBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEENDBACKGROUND_H
#define UI_GAMEENDBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameEndBackground
{
public:

    void setupUi(QWidget *GameEndBackground)
    {
        if (GameEndBackground->objectName().isEmpty())
            GameEndBackground->setObjectName(QStringLiteral("GameEndBackground"));
        GameEndBackground->resize(400, 300);

        retranslateUi(GameEndBackground);

        QMetaObject::connectSlotsByName(GameEndBackground);
    } // setupUi

    void retranslateUi(QWidget *GameEndBackground)
    {
        GameEndBackground->setWindowTitle(QApplication::translate("GameEndBackground", "GameEndBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GameEndBackground: public Ui_GameEndBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEENDBACKGROUND_H
