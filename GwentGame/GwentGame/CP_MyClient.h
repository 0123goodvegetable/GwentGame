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

	bool isChoosed;//选择客户端
	int turn;//轮次

public slots:
	void tryToConnect(QString ID,int host);//尝试连接服务器
	void sendFile(QString filename);//传输文件
	void MeReady();//我方就位
	void EnemyReady();//敌方就位

signals:
	void changeBackground();//建立连接后切换背景
	void toPlayBackground();//开始游戏
	void toKnowMeReady();
	void toKnowEnemyReady();

private:
	//发出信息
	QTcpSocket *tcpClientConnection;
	qint64 totalWriteBytes;  //数据总大小
	qint64 bytesWritten;  //已经发送数据大小
	qint64 bytesToWrite;   //剩余数据大小
	qint64 loadSize;   //每次发送数据的大小
	QFile *fileToWrite;//将要发送的文件
	QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据

	//接受信息
	qint64 totalReadBytes;  //存放总大小信息
	qint64 bytesReceived;  //已收到数据的大小
	qint64 fileNameSize;  //接收文件名的大小信息
	QString fileName;   //存放文件名
	QFile *fileToRead;  //接收的文件
	QByteArray inBlock;   //数据缓冲区

	bool meReady;//我方选好牌准备开始游戏
	bool enemyReady;//敌方选好牌准备开始游戏

private slots:
	void successToConnection();//成功连接服务器
	void displayError(QAbstractSocket::SocketError); //显示错误
	void updateWriteProgress(qint64 numBytes); //不断更新实现文件的传送
	void receiveFile();//接收文件

};

#endif // !CP_MYCLIENT_H
