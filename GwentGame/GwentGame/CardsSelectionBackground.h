//������ѡ����Ϸ������ص�����
#pragma once

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QMap>
#include<QResizeEvent>

#include"CardsSelectionScene.h"
#include"CardsUI.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	void resizeEvent(QResizeEvent*event);//��������¼�

	CardsUI *selectedCardUI();//ѡȡ���ڲ����Ŀ���
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ
	void cardUISizeAdjust();//���µ���ѡ�ƽ�������

private:
	Ui::CardsSelectionBackground ui;

	void init();//��ʼ������

	QGraphicsView *view;
	CardsSelectionScene *scene;

	volatile bool Pressed;//�����ã���������жϱ���

	QList<CardsUI *> cardUILists;//�����б�
	QList<QPointF> cardUIPosLists;//����λ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬ�б�

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void isPressed();//��갴�º���
	void isReleased();//����ͷź���
	void selectionChanged();//ѡ�����ı亯��

};
