//ʵ������ѡ��Ĵ󲿷ֹ���

#pragma once
#ifndef CARDSSELECTIONDIRECTOR_H
#define CARDSSELECTIONDIRECTOR_H

#include<QGraphicsItem>
#include<QStyleOptionGraphicsItem>
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
#include<QPointF>
#include"CP_Card.h"

class CardsSelectionDirector : public QObject, public Card
{
	Q_OBJECT

public:
	CardsSelectionDirector();
	CardsSelectionDirector(QString &file, QString &text, int height=400,int width=300);//ͼƬ�ߴ����

	//���ܺ���
	void setMyPixmap(QString &file, int height,int width);//ͼƬ���ú���
	//void setMyText(QString &text);//�������ú���
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);//���ƺ���
	//QString getMyText();//��ȡ���ֺ���

	//�̳��Ի�����麯��
	QRectF boundingRect() const;
	QPainterPath shape() const;

signals:
	void nodeIsMoving(QPointF &pos);//�ƶ��ڵ�
	void nodeIsPressed();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);//��갴���¼�
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//����ͷ��¼�

//private:
	//QString myText;//���ִ���

private:
	Card operation_card;
};


#endif // !CARDSSELECTIONDIRECTOR_H

