#include"CP_MyClient.h"

MyClient::MyClient()
{
	//��ʼ��
	loadSize = 4 * 1024;
	totalWriteBytes = 0;
	bytesWritten = 0;
	bytesToWrite = 0;
	totalReadBytes = 0;
	bytesReceived = 0;
	fileNameSize = 0;
	turn = 0;
	isChoosed = false;
	enemyReady = false;
	meReady = false;

	tcpClientConnection = new QTcpSocket(this);

	connect(tcpClientConnection, SIGNAL(connected()),
			this, SLOT(successToConnection()));

	connect(tcpClientConnection, SIGNAL(bytesWritten(qint64)), 
		this, SLOT(updateWriteProgress(qint64)));

	connect(this, SIGNAL(toKnowEnemyReady()), 
		this, SLOT(EnemyReady()));

	connect(this, SIGNAL(toKnowMeReady()),
		this, SLOT(MeReady()));
}

void MyClient::tryToConnect(QString ID, int host)
{
	bytesWritten = 0;
	tcpClientConnection->abort();
	tcpClientConnection->connectToHost(ID,host);//����
	isChoosed = true;

	connect(tcpClientConnection,SIGNAL(readyRead()),
		this,SLOT(receiveFile()));
}

void MyClient::successToConnection()
{
	turn = 1;
	emit changeBackground();
}

void MyClient::displayError(QAbstractSocket::SocketError) //��ʾ����
{
	qDebug() << tcpClientConnection->errorString();
	tcpClientConnection->close();
}

void MyClient::sendFile(QString filename)
{
	fileToWrite = new QFile(filename);
	if (!fileToWrite->open(QFile::ReadOnly))
	{
		return;
	}
	totalWriteBytes = fileToWrite->size();//�����ļ��ܴ�С

	QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
			sendOut.setVersion(QDataStream::Qt_4_6);

	QString currentFileName = filename.right(filename.size() - filename.lastIndexOf('/') - 1);
	sendOut << qint64(0) << qint64(0) << currentFileName;

	totalWriteBytes += outBlock.size();

	sendOut.device()->seek(0);
	sendOut << totalWriteBytes << qint64((outBlock.size() - sizeof(qint64) * 2));

	bytesToWrite = totalWriteBytes - tcpClientConnection->write(outBlock);
	outBlock.resize(0);
}

void MyClient::updateWriteProgress(qint64 numBytes) 
{
	bytesWritten += (int)numBytes;
	//�Ѿ��������ݵĴ�С
	if (bytesToWrite > 0) //����Ѿ�����������
	{
		outBlock = fileToWrite->read(qMin(bytesToWrite, loadSize));
		//ÿ�η���loadSize��С�����ݣ���������Ϊ4KB�����ʣ������ݲ���4KB��
		//�ͷ���ʣ�����ݵĴ�С
		bytesToWrite -= (int)tcpClientConnection->write(outBlock);
		//������һ�����ݺ�ʣ�����ݵĴ�С
		outBlock.resize(0);
		//��շ��ͻ�����
	}
	else
	{
		fileToWrite->close(); //���û�з����κ����ݣ���ر��ļ�
	}

	if (bytesWritten == totalWriteBytes) //�������
	{		
		fileToWrite->close();
	}
}

void MyClient::receiveFile()
{
	QDataStream in(tcpClientConnection);
	in.setVersion(QDataStream::Qt_4_6);
	if (bytesReceived <= sizeof(qint64) * 2)
	{ 
		//������յ�������С��16���ֽڣ���ô�Ǹտ�ʼ�������ݣ����Ǳ��浽//����ͷ�ļ���Ϣ
		if ((tcpClientConnection->bytesAvailable() >= sizeof(qint64) * 2)
			&& (fileNameSize == 0))
		{ 
			//���������ܴ�С��Ϣ���ļ�����С��Ϣ
			in >> totalReadBytes >> fileNameSize;
			bytesReceived += sizeof(qint64) * 2;
		}
		if ((tcpClientConnection->bytesAvailable() >= fileNameSize)
			&& (fileNameSize != 0))
		{  
			//�����ļ������������ļ�
			in >> fileName;
			bytesReceived += fileNameSize;
			fileToRead = new QFile(fileName);
			if (!fileToRead->open(QFile::WriteOnly))
			{
				qDebug() << "open file error!";
				return;
			}
		}
		else return;
	}
	if (bytesReceived < totalWriteBytes)
	{  
		//������յ�����С�������ݣ���ôд���ļ�
		bytesReceived += tcpClientConnection->bytesAvailable();
		inBlock = tcpClientConnection->readAll();
		fileToRead->write(inBlock);
		inBlock.resize(0);
	}

	if (bytesReceived == totalWriteBytes)
	{ 
		//�����������ʱ
		fileToRead->close();
	}

	//����ļ�����ready.txt����ʼ��Ϸ
	if (fileName == "ready.txt")
	{
		emit toKnowEnemyReady();
	}
}

void MyClient::EnemyReady()
{
	enemyReady = true;
	if (meReady == true)
	{
		emit toPlayBackground();
		enemyReady = false;
		meReady = false;
	}
}

void MyClient::MeReady()
{

	meReady = true;
	QString filename = "ready.txt";
	sendFile(filename);

	if (enemyReady == true)
	{
		emit toPlayBackground();
		enemyReady = false;
		meReady = false;
	}

}