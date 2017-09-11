#pragma once
#ifndef CP_MYSERVER_H
#define CP_MYSERVER_H

#include<QTcpSocket>
#include<QTcpServer>
#include<QString>
#include<QFile>
#include<QByteArray>
#include<QObject>
#include<QtNetwork>

class MyServer:public QObject
{
	Q_OBJECT

public:
	MyServer();
	~MyServer() {}

	public slots :
		void startlisten(QString IP,int host);//��ʼ����

	signals:
		void changeBackground();//�������Ӻ��л�����

private:
	QTcpServer *tcpServer;
	QTcpSocket *tcpServerConnection;
	qint64 totalBytes;  //����ܴ�С��Ϣ
	qint64 bytesReceived;  //���յ����ݵĴ�С
	qint64 fileNameSize;  //�ļ����Ĵ�С��Ϣ
	QString fileName;   //����ļ���
	QFile *localFile;   //�����ļ�
	QByteArray inBlock;   //���ݻ�����

	private slots:
		void acceptConnection();//��������
		void displayError(QAbstractSocket::SocketError);//������
};

#endif // !CP_MYSERVER_H
