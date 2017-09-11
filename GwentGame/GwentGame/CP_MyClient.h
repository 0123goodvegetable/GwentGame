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

	bool isChoosed;//ѡ��ͻ���
	int turn;//�ִ�

public slots:
	void tryToConnect(QString ID,int host);//�������ӷ�����
	void sendFile(QString filename);//�����ļ�
	void MeReady();//�ҷ���λ
	void EnemyReady();//�з���λ

signals:
	void changeBackground();//�������Ӻ��л�����
	void toPlayBackground();//��ʼ��Ϸ
	void toKnowMeReady();
	void toKnowEnemyReady();

private:
	//������Ϣ
	QTcpSocket *tcpClientConnection;
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
	void successToConnection();//�ɹ����ӷ�����
	void displayError(QAbstractSocket::SocketError); //��ʾ����
	void updateWriteProgress(qint64 numBytes); //���ϸ���ʵ���ļ��Ĵ���
	void receiveFile();//�����ļ�

};

#endif // !CP_MYCLIENT_H
