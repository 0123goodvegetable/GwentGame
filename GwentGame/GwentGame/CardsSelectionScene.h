//����ʵ������ѡ��
//���ܣ�ʵ������¼�

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
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//����ƶ��¼�
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//��갴���¼�
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//����ͷ��¼�

signals:
	void isMoving(QPointF &pos);//����ƶ��ź�

private:
	QPointF  beforePos;
	QPointF  releasePos;
};

#endif // !CARDSSELECTIONSCENE_H

