//�������Ϸ��صĽ���
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CardsUI.h"
#include"CardsScene.h"


class GamePlayingBackground : public QWidget
{
	Q_OBJECT

public:
	GamePlayingBackground(QWidget *parent = Q_NULLPTR);
	~GamePlayingBackground();

	void resizeEvent(QResizeEvent*event);//��������¼�

	CardsUI *selectedCardUI();//ѡȡ���ڲ����Ŀ���
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ
	void cardUISizeAdjust();//���µ���ѡ�ƽ�������

	QList<int> my_cardStackNo;//��Ϸ���ѷ�������Ϣ

signals:
	void toUseSkills();//�ͷż��ܵ��ź�

public slots:
	void getFromText();//���ı��ļ��л�ȡ������Ϣ


private:
	Ui::GamePlayingBackground ui;

	void init();//��ʼ������
	void updateStatus();//������Ϸ״̬
	void updateCoordinate();//������Ϸ���泣��

	QGraphicsView *view;
	CardsScene *scene1;//������Ϸ����
	CardsScene *scene2;//���Ʋ�����Ϸ����

	volatile bool Pressed;//�����ã���������жϱ���
	bool operation;//�Ƿ���ĳ�ſ��Ƶ��ִ���

	CardsUI  *selected_card;//��ǰѡ��Ŀ���
	QList<CardsUI *> cardUILists;//����ͼƬ�б�
	QList<QPointF> cardUIPosLists;//����ͼƬλ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬͼ���б�
	QList<Card> PlayingCard;//�ų������п��ƣ����ڸ��½��棩



	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void isPressed();//��갴�º���
	void isReleased();//����ͷź���
	void selectionChanged();//ѡ�����ı亯��
	void putInText();//��������Ϣ�洢���ı��ļ���
	void useSkills();//ʹ�ü���

};
