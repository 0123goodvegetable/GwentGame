//ʵ������༭����
#pragma once

#include <QWidget>
#include "ui_CardsEditBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>

#include"CardsScene.h"
#include"CardsUI.h"
#include"CP_CardStack.h"


class CardsEditBackground : public QWidget
{
	Q_OBJECT

public:
	CardsEditBackground(QWidget *parent = Q_NULLPTR);
	~CardsEditBackground();

	void resizeEvent(QResizeEvent *event);//��������¼�
	void cardUISizeAdjust();//���µ������Ʊ༭����

private:
	Ui::CardsEditBackground ui;

	void init();//��ʼ�����Ʊ༭����
	void updateCoordinate();//��������

	QGraphicsView *view;
	CardsScene *scene;

	CardStack *stack;//����

	volatile bool Pressed;//�����ã���������жϱ���

	QList<CardsUI *> cardUILists;//�����б�
	QList<QPointF> cardUIPosLists;//����λ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬ�б�

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void isPressed();//��갴�º���
	void isReleased();//����ͷź���
	void selectionChanged();//ѡ�����ı亯��

};
