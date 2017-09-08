#include"CardsScene.h"
#include <QGraphicsSceneMouseEvent>  
#include <QPointF>  
#include <QDebug> 

CardsScene::CardsScene(QObject *parent) :
	QGraphicsScene(parent)
{
}

void CardsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF pos = event->scenePos();
	QPointF movePos(pos.x() - beforePos.x(), pos.y() - beforePos.y());
	emit isMoving(movePos);
}

void CardsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	beforePos = event->scenePos();//��ȡ�ƶ�ǰ��������
	QGraphicsScene::mousePressEvent(event);
}

void CardsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	releasePos = event->scenePos();//��ȡ�ͷ�ʱ��������
	QGraphicsScene::mouseReleaseEvent(event);
}