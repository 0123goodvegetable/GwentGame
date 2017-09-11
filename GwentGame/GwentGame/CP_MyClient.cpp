#include"CP_MyClient.h"

MyClient::MyClient()
{
	//初始化
	loadSize = 4 * 1024;
	totalBytes = 0;
	bytesWritten = 0;
	bytesToWrite = 0;
	tcpClientConnection = new QTcpSocket(this);

	connect(tcpClientConnection, SIGNAL(connected()),
			this, SLOT(successToConnection()));
}

void MyClient::tryToConnect(QString ID, int host)
{
	bytesWritten = 0;
	tcpClientConnection->abort();
	tcpClientConnection->connectToHost(ID,host);//连接
}

void MyClient::successToConnection()
{
	emit changeBackground();
}

void MyClient::displayError(QAbstractSocket::SocketError) //显示错误
{
	qDebug() << tcpClientConnection->errorString();
	tcpClientConnection->close();
}