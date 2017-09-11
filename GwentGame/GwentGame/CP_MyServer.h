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
		void startlisten(QString IP,int host);//开始监听

	signals:
		void changeBackground();//建立连接后切换背景

private:
	QTcpServer *tcpServer;
	QTcpSocket *tcpServerConnection;
	qint64 totalBytes;  //存放总大小信息
	qint64 bytesReceived;  //已收到数据的大小
	qint64 fileNameSize;  //文件名的大小信息
	QString fileName;   //存放文件名
	QFile *localFile;   //本地文件
	QByteArray inBlock;   //数据缓冲区

	private slots:
		void acceptConnection();//建立连接
		void displayError(QAbstractSocket::SocketError);//错误处理
};

#endif // !CP_MYSERVER_H
