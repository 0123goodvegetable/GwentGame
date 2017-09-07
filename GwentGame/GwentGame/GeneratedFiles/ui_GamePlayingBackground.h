/********************************************************************************
** Form generated from reading UI file 'GamePlayingBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEPLAYINGBACKGROUND_H
#define UI_GAMEPLAYINGBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GamePlayingBackground
{
public:

    void setupUi(QWidget *GamePlayingBackground)
    {
        if (GamePlayingBackground->objectName().isEmpty())
            GamePlayingBackground->setObjectName(QStringLiteral("GamePlayingBackground"));
        GamePlayingBackground->resize(400, 300);

        retranslateUi(GamePlayingBackground);

        QMetaObject::connectSlotsByName(GamePlayingBackground);
    } // setupUi

    void retranslateUi(QWidget *GamePlayingBackground)
    {
        GamePlayingBackground->setWindowTitle(QApplication::translate("GamePlayingBackground", "GamePlayingBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GamePlayingBackground: public Ui_GamePlayingBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEPLAYINGBACKGROUND_H
