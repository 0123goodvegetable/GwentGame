//MainWindow���ڿ��Ʋ�ͬ�����ת��������������ࣩ

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//���������ͷ�ļ�
#include"BeginBackground.h"
#include"GameSelectionBackground.h"
#include"CardsSelectionBackground.h"
#include"CardsEditBackground.h"
#include"GamePlayingBackground.h"
#include"NetConnectionBackground.h"
#include"GameEndBackground.h"
//�����������ͷ�ļ�
#include"CP_MyServer.h"
#include"CP_MyClient.h"

#include<QStackedWidget>
#include<QKeyEvent>
#include <stdlib.h> 
#include <time.h>

class MyServer;
class MyClient;
class NetConnectionBackground;
class GamePlayingBackground;
class GameEndBackground;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();


private slots:
	void updateBackground(int number);//ˢ����Ϸ������и���

	void toBeginBackground();//�������л�����ʼ����
	void toGameSelectionBackground();//�������л�����Ϸѡ�����
	void toCardsSelectionBackground();//�������л�����Ϸ��ʼ����ѡ�����
	void toCardsEditBackground();//�������л�������༭����
	void toGamePlayingBackground();//�������л�����Ϸ������
	void toNetConnectionBackground();//�������л����������ӽ���
	void toGameEndBackground();//�������л�����Ϸ��������

protected:
	void keyPressEvent(QKeyEvent* event);//��������ڵİ�����Ϣ

signals:
	void changeBackgroundNo(int number);//���½�����


private:
	Ui::MainWindowClass ui;
	void init();//��ʼ����������

	QStackedWidget *BackgroundController;//���ƽ����ת��
	int BackgroundNo;//��ѡ����ı�ţ���ʼ����-0����Ϸѡ�����-1������ѡ�����-2������༭����-3����Ϸ������-4���������ӽ���-5����Ϸ��������-6��
	
	BeginBackground *beginBackground;//������ʼ����
	GameSelectionBackground *gameSelectionBackground;//������Ϸѡ�����
	CardsSelectionBackground *cardsSelectionBackground;//������Ϸ��ʼ����ѡ�����
	CardsEditBackground *cardsEditBackground;//��������༭����
	GamePlayingBackground *gamePlayingBackground;//������Ϸ���������
	NetConnectionBackground *netConnectionBackground;//�����������ӽ���
	GameEndBackground *gameEndBackground;//������������

	bool isServer;//�ó����Ƿ����
	bool isClient;//�ó����ǿͻ���
	MyServer *myServer;//�����
	MyClient *myClient;//�ͻ���
};
