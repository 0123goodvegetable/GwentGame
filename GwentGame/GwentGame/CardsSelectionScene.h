//辅助实现手牌选择
//功能：实现鼠标事件

#pragma once
#ifndef CARDSSELECTIONSCENE_H
#define CARDSSELECTIONSCENE_H
#include<QGraphicsScene>

class CardsSelectionScene :public QGraphicsScene
{
	Q_OBJECT

public:
	explicit CardsSelectionScene(QObject *parent = 0);

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

#endif // !CARDSSELECTIONSCENE_H

