//ʵ���뿨����ʾ��ؽ���

#pragma once
#ifndef CARDSSELECTIONDIRECTOR_H
#define CARDSSELECTIONDIRECTOR_H

#include"CP_Card.h"
#include<QGraphicsItem>
#include<QStyleOptionGraphicsItem>
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
#include<QPointF>

class CardsUI : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	CardsUI();
	CardsUI(int cardNo);

	//�麯��
	QRectF boundingRect() const;
	QPainterPath shape() const;

signals:
	void cardIsPressed();//�������
	void cardIsReleased();//�ͷſ���

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//�������¼�
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//�ͷ�����¼�
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//����ƶ��¼�
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//���ƺ���

public:
	Card *operating_card;

};


#endif // !CARDSSELECTIONDIRECTOR_H

