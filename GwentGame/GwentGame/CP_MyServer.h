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

	bool isChoosed;//选择服务端
	int turn;//轮次

	public slots :
		void startlisten(QString IP,int host);//开始监听
		void sendFile(QString filename);//传输文件
		void MeReady();//我方就位
		void EnemyReady();//敌方就位

	signals:
		void changeBackground();//建立连接后切换背景
		void toPlayBackground();//开始游戏
		void toKnowMeReady();
		void toKnowEnemyReady();
		void receiveFinished();//文件接收完成
		void changeTurn();//改变选手

private:
	QTcpServer *tcpServer;
	QTcpSocket *tcpServerConnection;

	//发出信息
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
		void acceptConnection();//建立连接
		void displayError(QAbstractSocket::SocketError);//错误处理
		void updateWriteProgress(qint64 numBytes); //不断更新实现文件的传送
		void receiveFile();//接收文件
};

#endif // !CP_MYSERVER_H
