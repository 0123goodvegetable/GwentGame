//ʵ������ѡ��Ĵ󲿷ֹ���

#pragma once
#ifndef CARDSSELECTIONDIRECTOR_H
#define CARDSSELECTIONDIRECTOR_H

#include<QGraphicsPixmapItem>
#include<QGraphicsItem>
#include<QStyleOptionGraphicsItem>
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
#include<QPointF>

class CardsSelectionDirector : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	CardsSelectionDirector();
	CardsSelectionDirector(QString &file, QString &text, int imagesize = 80);//ͼƬ�ߴ����

	//���ܺ���
};


#endif // !CARDSSELECTIONDIRECTOR_H

