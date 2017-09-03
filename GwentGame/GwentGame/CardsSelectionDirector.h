//实现手牌选择的大部分功能

#pragma once
#ifndef CARDSSELECTIONDIRECTOR_H
#define CARDSSELECTIONDIRECTOR_H

#include<QGraphicsItem>
#include<QStyleOptionGraphicsItem>
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
#include<QPointF>
#include"CP_Card.h"

class CardsSelectionDirector : public QObject, public Card
{
	Q_OBJECT

public:
	CardsSelectionDirector();
	CardsSelectionDirector(QString &file, QString &text, int height=400,int width=300);//图片尺寸待定

	//功能函数
	void setMyPixmap(QString &file, int height,int width);//图片设置函数
	//void setMyText(QString &text);//文字设置函数
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//绘制函数
	//QString getMyText();//获取文字函数

	//继承自基类的虚函数
	QRectF boundingRect() const;
	QPainterPath shape() const;

signals:
	void nodeIsMoving(QPointF &pos);//移动节点
	void nodeIsPressed();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//鼠标按下事件
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//鼠标释放事件

//private:
	//QString myText;//名字待定

private:
	Card operation_card;
};


#endif // !CARDSSELECTIONDIRECTOR_H

