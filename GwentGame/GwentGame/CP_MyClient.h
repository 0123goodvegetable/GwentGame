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
	void tryToConnect(QString ID,int host);//尝试连接服务器

signals:
	void changeBackground();//建立连接后切换背景

private:
	QTcpSocket *tcpClientConnection;
	QFile *localFile;  //要发送的文件
	qint64 totalBytes;  //数据总大小
	qint64 bytesWritten;  //已经发送数据大小
	qint64 bytesToWrite;   //剩余数据大小
	qint64 loadSize;   //每次发送数据的大小
	QString fileName;  //保存文件路径
	QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据

private slots:
	void successToConnection();//成功连接服务器
	void displayError(QAbstractSocket::SocketError); //显示错误

};

#endif // !CP_MYCLIENT_H
