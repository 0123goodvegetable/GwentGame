#include"CP_MyClient.h"

MyClient::MyClient()
{
	//初始化
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
	tcpClientConnection->connectToHost(ID,host);//连接
	isChoosed = true;

	connect(tcpClientConnection,SIGNAL(readyRead()),
		this,SLOT(receiveFile()));
}

void MyClient::successToConnection()
{
	turn = 1;
	emit changeBackground();
}

void MyClient::displayError(QAbstractSocket::SocketError) //显示错误
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
	totalWriteBytes = fileToWrite->size();//传输文件总大小

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
	//已经发送数据的大小
	if (bytesToWrite > 0) //如果已经发送了数据
	{
		outBlock = fileToWrite->read(qMin(bytesToWrite, loadSize));
		//每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
		//就发送剩余数据的大小
		bytesToWrite -= (int)tcpClientConnection->write(outBlock);
		//发送完一次数据后还剩余数据的大小
		outBlock.resize(0);
		//清空发送缓冲区
	}
	else
	{
		fileToWrite->close(); //如果没有发送任何数据，则关闭文件
	}

	if (bytesWritten == totalWriteBytes) //发送完毕
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
		//如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
		if ((tcpClientConnection->bytesAvailable() >= sizeof(qint64) * 2)
			&& (fileNameSize == 0))
		{ 
			//接收数据总大小信息和文件名大小信息
			in >> totalReadBytes >> fileNameSize;
			bytesReceived += sizeof(qint64) * 2;
		}
		if ((tcpClientConnection->bytesAvailable() >= fileNameSize)
			&& (fileNameSize != 0))
		{  
			//接收文件名，并建立文件
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
		//如果接收的数据小于总数据，那么写入文件
		bytesReceived += tcpClientConnection->bytesAvailable();
		inBlock = tcpClientConnection->readAll();
		fileToRead->write(inBlock);
		inBlock.resize(0);
	}

	if (bytesReceived == totalWriteBytes)
	{ 
		//接收数据完成时
		fileToRead->close();
	}

	//如果文件名是ready.txt，则开始游戏
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