/********************************************************************************
** Form generated from reading UI file 'NetConnectionBackground.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETCONNECTIONBACKGROUND_H
#define UI_NETCONNECTIONBACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NetConnectionBackground
{
public:

    void setupUi(QWidget *NetConnectionBackground)
    {
        if (NetConnectionBackground->objectName().isEmpty())
            NetConnectionBackground->setObjectName(QStringLiteral("NetConnectionBackground"));
        NetConnectionBackground->resize(400, 300);

        retranslateUi(NetConnectionBackground);

        QMetaObject::connectSlotsByName(NetConnectionBackground);
    } // setupUi

    void retranslateUi(QWidget *NetConnectionBackground)
    {
        NetConnectionBackground->setWindowTitle(QApplication::translate("NetConnectionBackground", "NetConnectionBackground", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NetConnectionBackground: public Ui_NetConnectionBackground {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETCONNECTIONBACKGROUND_H
