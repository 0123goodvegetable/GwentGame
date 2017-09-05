//辅助实现手牌绘制
//功能：实现卡牌的鼠标事件

#pragma once
#ifndef CARDSSCENE_H
#define CARDSSCENE_H
#include<QGraphicsScene>

class CardsScene :public QGraphicsScene
{
	Q_OBJECT

public:
	explicit CardsScene(QObject *parent = 0);

private:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//鼠标移动事件
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//鼠标按下事件
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//鼠标释放事件

signals:
	void isMoving(QPointF &pos);//光标移动信号

private:
	QPointF  beforePos;
	QPointF  releasePos;
};

#endif // !CARDSSCENE_H

