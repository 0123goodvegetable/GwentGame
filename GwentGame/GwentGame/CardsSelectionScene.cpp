#include"CardsSelectionScene.h"
#include <QGraphicsSceneMouseEvent>  
#include <QPointF>  
#include <QDebug> 

CardsSelectionScene::CardsSelectionScene(QObject *parent) :
	QGraphicsScene(parent)
{
}

void CardsSelectionScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF pos = event->scenePos();
	QPointF movePos(pos.x() - beforePos.x(), pos.y() - beforePos.y());
	emit isMoving(movePos);
}

void CardsSelectionScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	beforePos = event->scenePos();//��ȡ�ƶ�ǰ��������
	QGraphicsScene::mousePressEvent(event);
}

void CardsSelectionScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	releasePos = event->scenePos();//��ȡ�ͷ�ʱ��������
	QGraphicsScene::mouseReleaseEvent(event);
}