//实现联网操作的界面

#pragma once

#include <QWidget>
#include "ui_NetConnectionBackground.h"

#include<QComboBox>
#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>
#include<QPushButton>
#include<QResizeEvent>
#include<QRectF>
#include<QPalette>
#include<QString>
#include<QHostAddress>
#include<QNetworkInterface>
#include<QHostInfo>

class NetConnectionBackground : public QWidget
{
	Q_OBJECT

public:
	NetConnectionBackground(QWidget *parent = Q_NULLPTR);
	~NetConnectionBackground();

	void init();//界面初始化函数
	void resizeEvent(QResizeEvent*event);//使背景图片填充整个界面

private slots:
	void changeUI();//当选择服务器或者客户端不同时，调整控件属性
	void createConnection();//鼠标点击之后建立连接

signals:
	void connectToClient(QString IP, int host);//连接到客户端
	void connectToServer(QString IP,int host);//连接到服务器端

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
	QString ipAddress;//IP地址
};
