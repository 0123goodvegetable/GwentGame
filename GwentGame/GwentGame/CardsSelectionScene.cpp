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
	beforePos = event->scenePos();//获取移动前光标的坐标
	QGraphicsScene::mousePressEvent(event);
}

void CardsSelectionScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	releasePos = event->scenePos();//获取释放时光标的坐标
	QGraphicsScene::mouseReleaseEvent(event);
}