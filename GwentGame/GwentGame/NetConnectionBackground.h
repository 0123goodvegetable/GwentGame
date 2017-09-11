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

	void init();//�����ʼ������

private slots:
	void changeUI();//��ѡ����������߿ͻ��˲�ͬʱ�������ؼ�����

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
