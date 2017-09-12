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

	bool isChoosed;//ѡ������
	int turn;//�ִ�

	public slots :
		void startlisten(QString IP,int host);//��ʼ����
		void sendFile(QString filename);//�����ļ�
		void MeReady();//�ҷ���λ
		void EnemyReady();//�з���λ

	signals:
		void changeBackground();//�������Ӻ��л�����
		void toPlayBackground();//��ʼ��Ϸ
		void toKnowMeReady();
		void toKnowEnemyReady();
		void receiveFinished();//�ļ��������
		void changeTurn();//�ı�ѡ��

private:
	QTcpServer *tcpServer;
	QTcpSocket *tcpServerConnection;

	//������Ϣ
	qint64 totalWriteBytes;  //�����ܴ�С
	qint64 bytesWritten;  //�Ѿ��������ݴ�С
	qint64 bytesToWrite;   //ʣ�����ݴ�С
	qint64 loadSize;   //ÿ�η������ݵĴ�С
	QFile *fileToWrite;//��Ҫ���͵��ļ�
	QByteArray outBlock;  //���ݻ������������ÿ��Ҫ���͵�����

	//������Ϣ
	qint64 totalReadBytes;  //����ܴ�С��Ϣ
	qint64 bytesReceived;  //���յ����ݵĴ�С
	qint64 fileNameSize;  //�����ļ����Ĵ�С��Ϣ
	QString fileName;   //����ļ���
	QFile *fileToRead;  //���յ��ļ�
	QByteArray inBlock;   //���ݻ�����

	bool meReady;//�ҷ�ѡ����׼����ʼ��Ϸ
	bool enemyReady;//�з�ѡ����׼����ʼ��Ϸ
	
	private slots:
		void acceptConnection();//��������
		void displayError(QAbstractSocket::SocketError);//������
		void updateWriteProgress(qint64 numBytes); //���ϸ���ʵ���ļ��Ĵ���
		void receiveFile();//�����ļ�
};

#endif // !CP_MYSERVER_H
