//������ѡ����Ϸ������ص�����
#pragma once

#include <QWidget>
#include "ui_CardsSelectionBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>

#include"CardsScene.h"
#include"CardsUI.h"

class CardsSelectionBackground : public QWidget
{
	Q_OBJECT

public:
	CardsSelectionBackground(int turn = 0,QWidget *parent = Q_NULLPTR);
	~CardsSelectionBackground();

	void resizeEvent(QResizeEvent*event);//��������¼�

	CardsUI *selectedCardUI();//ѡȡ���ڲ����Ŀ���
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ
	void cardUISizeAdjust();//���µ���ѡ�ƽ�������

	QPushButton *cardsSelectionFinished_button;//���Ƶ�����ϰ�ť

signals:
	void toChange();//��������

private:
	Ui::CardsSelectionBackground ui;

	void init();//��ʼ������

	QGraphicsView *view;
	CardsScene *scene;

	volatile bool Pressed;//�����ã���������жϱ���

	QList<CardsUI *> cardUILists;//����ͼƬ�б�
	QList<QPointF> cardUIPosLists;//����ͼƬλ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬͼ���б�
	QList<int> cardStackNo;//��ѡ�������

	int my_turn;//�ִ�
	int cardsToAdjust_number;//���Ե����Ŀ�����

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void isPressed();//��갴�º���
	void isReleased();//����ͷź���
	void selectionChanged();//ѡ�����ı亯��
	void getFromText();//���ı��ļ��л�ȡ������Ϣ
	void putInText();//��������Ϣ�洢���ı��ļ���
	void changeCard();//��������

};
