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
	QPointF pos(event->scenePos().x() - beforePos.x(), 
		event->scenePos().y() - beforePos.y());

	emit isMoving(pos);
}

void CardsSelectionScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	beforePos = event->scenePos();//��ȡ�ƶ�ǰ��������
}

void CardsSelectionScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	releasePos = event->scenePos();//��ȡ�ͷ�ʱ��������
}