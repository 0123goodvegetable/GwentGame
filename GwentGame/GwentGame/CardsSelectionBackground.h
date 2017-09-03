//������ѡ����Ϸ������ص�����
#pragma once
#ifndef CARDSSELECTIONBACKGROUND_H
#define CARDSSELECTIONBACKGROUND_H

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QPointF>
#include"CardsSelectionDirector.h"
#include"CardsSelectionScene.h"
#include<QMap>
#include"CP_CardStack.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	void init();//��ʼ������

	CardsSelectionDirector *selectCard();//ѡȡ���ƣ��ڵ���ʽ��
	bool isCardClicked();//�Ƿ�������
	void cardSizeAdjust();//�������ƴ�С

private slots:
	//�ж����״̬
	void isMoving(QPointF &pos);
	void isPressed();
	void isReleased();

	void selectionChanged();//�ı�ѡ�����

signals:
	void cardClicked(CardsSelectionDirector *card);

private:
	Ui::CardsSelectionBackground ui;

	void GetScreenInfo();//��ȡ��Ļ��Ϣ
	
	QGraphicsView *cardsSelectionView;
	CardsSelectionScene *cardsSelectionScene;
	CardsSelectionDirector *operation_card;
	QPointF cardPos;
	volatile bool Pressed;

	QList<CardsSelectionDirector*> cardsLists;
	QList<QPointF> cardsPosLists;
	QList<QPixmap> cardsPixmapLists;

	//��Ļ�ߴ���Ϣ
	qint16 screenSizeX;
	qint16 screenSizeY;


};

#endif // !CARDSSELECTIONBACKGROUND_H


