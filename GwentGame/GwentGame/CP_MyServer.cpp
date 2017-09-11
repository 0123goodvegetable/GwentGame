#include"CP_MyServer.h"

MyServer::MyServer()
{
	//��ʼ����Ա����
	fileNameSize = 0;
	totalBytes = 0;
	bytesReceived = 0;

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
	connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)), this,
		SLOT(displayError(QAbstractSocket::SocketError)));
	connect(tcpServerConnection, &QAbstractSocket::disconnected, 
		tcpServerConnection, &QObject::deleteLater);

	//�����Ӻ��л�����
	emit changeBackground();
}

void MyServer::displayError(QAbstractSocket::SocketError) //������
{
	qDebug() << tcpServerConnection->errorString();
	tcpServerConnection->close();

}