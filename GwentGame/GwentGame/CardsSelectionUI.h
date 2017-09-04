//实现手牌选择界面的大部分功能

#pragma once
#ifndef CARDSSELECTIONDIRECTOR_H
#define CARDSSELECTIONDIRECTOR_H

#include"CP_Card.h"
#include<QGraphicsItem>
#include<QStyleOptionGraphicsItem>
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
#include<QPointF>

class CardsSelectionUI : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	CardsSelectionUI();
	CardsSelectionUI(const Card aim_card);

	//虚函数
	QRectF boundingRect() const;
	QPainterPath shape() const;

signals:
	void cardIsPressed();//点击卡牌
	void cardIsReleased();//释放卡牌

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//点击鼠标事件
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//释放鼠标事件
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//鼠标移动事件
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//绘制函数

public:
	Card *operating_card;

};


#endif // !CARDSSELECTIONDIRECTOR_H

