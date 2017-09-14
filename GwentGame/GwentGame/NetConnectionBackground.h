//ʵ�����������Ľ���

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

	void init();//�����ʼ������
	void resizeEvent(QResizeEvent*event);//ʹ����ͼƬ�����������

private slots:
	void changeUI();//��ѡ����������߿ͻ��˲�ͬʱ�������ؼ�����
	void createConnection();//�����֮��������

signals:
	void connectToClient(QString IP, int host);//���ӵ��ͻ���
	void connectToServer(QString IP,int host);//���ӵ���������

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
	QString ipAddress;//IP��ַ
};
