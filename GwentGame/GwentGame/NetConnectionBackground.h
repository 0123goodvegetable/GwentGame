#pragma once

#include <QWidget>
#include "ui_NetConnectionBackground.h"

#include<QComboBox>
#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>
#include<QPushButton>
#include<QRectF>
#include<QPalette>

class NetConnectionBackground : public QWidget
{
	Q_OBJECT

public:
	NetConnectionBackground(QWidget *parent = Q_NULLPTR);
	~NetConnectionBackground();

	void init();//界面初始化函数

private slots:
	void changeUI();//当选择服务器或者客户端不同时，调整控件属性

private:
	Ui::NetConnectionBackground ui;

	QComboBox *chooseClientOrServerBox;
	QLabel *IPLabel;
	QLabel *hostLabel;
	QLineEdit *IPLineEdit;
	QLineEdit *hostLineEdit;
	QPushButton *toConnectButton;
	QGridLayout *layout;
	QWidget *centralWidget;
};
