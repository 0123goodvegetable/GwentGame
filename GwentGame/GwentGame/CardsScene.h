//����ʵ�����ƻ���
//���ܣ�ʵ�ֿ��Ƶ�����¼�

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
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//����ƶ��¼�
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//��갴���¼�
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//����ͷ��¼�

signals:
	void isMoving(QPointF &pos);//����ƶ��ź�

private:
	QPointF  beforePos;
	QPointF  releasePos;
};

#endif // !CARDSSCENE_H

