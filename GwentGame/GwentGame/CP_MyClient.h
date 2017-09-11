#pragma once
#ifndef CP_MYCLIENT_H
#define CP_MYCLIENT_H

#include <QWidget>
#include<QFile>
#include<QString>
#include<QByteArray>
#include<QTcpServer>
#include<QTcpSocket>
#include<QObject>
#include<QtNetwork>

class MyClient:public QObject
{
	Q_OBJECT

public:

	MyClient();
	~MyClient() {}

public slots:
	void tryToConnect(QString ID,int host);//�������ӷ�����

signals:
	void changeBackground();//�������Ӻ��л�����

private:
	QTcpSocket *tcpClientConnection;
	QFile *localFile;  //Ҫ���͵��ļ�
	qint64 totalBytes;  //�����ܴ�С
	qint64 bytesWritten;  //�Ѿ��������ݴ�С
	qint64 bytesToWrite;   //ʣ�����ݴ�С
	qint64 loadSize;   //ÿ�η������ݵĴ�С
	QString fileName;  //�����ļ�·��
	QByteArray outBlock;  //���ݻ������������ÿ��Ҫ���͵�����

private slots:
	void successToConnection();//�ɹ����ӷ�����
	void displayError(QAbstractSocket::SocketError); //��ʾ����

};

#endif // !CP_MYCLIENT_H
