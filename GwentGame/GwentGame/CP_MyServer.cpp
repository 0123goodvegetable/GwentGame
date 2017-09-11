#include"CP_MyServer.h"

MyServer::MyServer()
{
	//初始化成员变量
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

	//已连接后切换场景
	emit changeBackground();
}

void MyServer::displayError(QAbstractSocket::SocketError) //错误处理
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
	totalWriteBytes = fileToWrite->size();//传输文件总大小

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
	//已经发送数据的大小
	if (bytesToWrite > 0) //如果已经发送了数据
	{
		outBlock = fileToWrite->read(qMin(bytesToWrite, loadSize));
		//每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
		//就发送剩余数据的大小
		bytesToWrite -= (int)tcpServerConnection->write(outBlock);
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

void MyServer::receiveFile()
{
	QDataStream in(tcpServerConnection);
	in.setVersion(QDataStream::Qt_4_6);
	if (bytesReceived <= sizeof(qint64) * 2)
	{
		//如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
		if ((tcpServerConnection->bytesAvailable() >= sizeof(qint64) * 2)
			&& (fileNameSize == 0))
		{
			//接收数据总大小信息和文件名大小信息
			in >> totalReadBytes >> fileNameSize;
			bytesReceived += sizeof(qint64) * 2;
		}
		if ((tcpServerConnection->bytesAvailable() >= fileNameSize)
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
		bytesReceived += tcpServerConnection->bytesAvailable();
		inBlock = tcpServerConnection->readAll();
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