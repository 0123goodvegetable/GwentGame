//������ѡ����Ϸ������ص�����
#pragma once

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QMap>

#include"CardsSelectionScene.h"
#include"CardsSelectionUI.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	CardsSelectionUI *selectedCardUI();//ѡȡ���ڲ����Ŀ���
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ
	void cardUISizeAdjust();//���µ���ѡ�ƽ����С

private:
	Ui::CardsSelectionBackground ui;

	void GetScreenInfo();//��ȡ��Ļ��Ϣ
	QGraphicsView *view;
	CardsSelectionScene *scene;

	volatile bool Pressed;//�����ã���������жϱ���

	QList<CardsSelectionUI *> cardUILists;//�����б�
	QList<QPointF> cardUIPosLists;//����λ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬ�б�


	CardsSelectionUI *cardUI;//����
	QPointF cardUIPos;//����λ��

	//��Ļ��Ϣ
	qint16 sceenSizeX;
	qint16 sceenSizeY;

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void isPressed();//��갴�º���
	void isReleased();//����ͷź���
	void selectionChanged();//ѡ�����ı亯��

signals:
	void nodeUiClicked(CardsSelectionUI* node);
};
