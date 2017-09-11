#include"CP_MyServer.h"

MyServer::MyServer()
{
	//��ʼ����Ա����
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

	tcpServer = new QTcpServer(this);
	connect(tcpServer, SIGNAL(newConnection()),
		this, SLOT(acceptConnection()));
}

void MyServer::startlisten(QString IP,int host)
{
	bytesReceived = 0;
	if (!tcpServer->listen(static_cast<QHostAddress>(IP),host))
	{
		qDebug() << tcpServer->errorString();
		return;
	}

}

void MyServer::acceptConnection()
{
	tcpServerConnection = tcpServer->nextPendingConnection();
	isChoosed = true;
	turn = 1;

	connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)), 
		this,SLOT(displayError(QAbstractSocket::SocketError)));
	connect(tcpServerConnection, &QAbstractSocket::disconnected, 
		tcpServerConnection, &QObject::deleteLater);
	connect(tcpServerConnection, SIGNAL(readyRead()),
		this, SLOT(receiveFile()));
	connect(this, SIGNAL(toKnowEnemyReady()),
		this, SLOT(EnemyReady()));

	//�����Ӻ��л�����
	emit changeBackground();
}

void MyServer::displayError(QAbstractSocket::SocketError) //������
{
	qDebug() << tcpServerConnection->errorString();
	tcpServerConnection->close();

}

void MyServer::sendFile(QString filename)
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

	bytesToWrite = totalWriteBytes - tcpServerConnection->write(outBlock);
	outBlock.resize(0);
}

void MyServer::updateWriteProgress(qint64 numBytes)
{
	bytesWritten += (int)numBytes;
	//�Ѿ��������ݵĴ�С
	if (bytesToWrite > 0) //����Ѿ�����������
	{
		outBlock = fileToWrite->read(qMin(bytesToWrite, loadSize));
		//ÿ�η���loadSize��С�����ݣ���������Ϊ4KB�����ʣ������ݲ���4KB��
		//�ͷ���ʣ�����ݵĴ�С
		bytesToWrite -= (int)tcpServerConnection->write(outBlock);
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

void MyServer::receiveFile()
{
	QDataStream in(tcpServerConnection);
	in.setVersion(QDataStream::Qt_4_6);
	if (bytesReceived <= sizeof(qint64) * 2)
	{
		//������յ�������С��16���ֽڣ���ô�Ǹտ�ʼ�������ݣ����Ǳ��浽//����ͷ�ļ���Ϣ
		if ((tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2)
			&& (fileNameSize == 0))
		{
			//���������ܴ�С��Ϣ���ļ�����С��Ϣ
			in >> totalReadBytes >> fileNameSize;
			bytesReceived += sizeof(qint64) * 2;
		}
		if ((tcpServerConnection->bytesAvailable() >= fileNameSize)
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
		bytesReceived += tcpServerConnection->bytesAvailable();
		inBlock = tcpServerConnection->readAll();
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

void MyServer::EnemyReady()
{
	enemyReady = true;
	if (meReady == true)
	{
		emit toPlayBackground();
		enemyReady = false;
		meReady = false;
	}
}

void MyServer::MeReady()
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