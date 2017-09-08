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
	beforePos = event->scenePos();//获取移动前光标的坐标
	QGraphicsScene::mousePressEvent(event);
}

void CardsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	releasePos = event->scenePos();//获取释放时光标的坐标
	QGraphicsScene::mouseReleaseEvent(event);
}