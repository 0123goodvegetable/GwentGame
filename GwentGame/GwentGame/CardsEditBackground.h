//ʵ������༭����
#pragma once

#include <QWidget>
#include "ui_CardsEditBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>

#include"CardsScene.h"
#include"CardsUI.h"
#include"CP_CardStack.h"


class CardsEditBackground : public QWidget
{
	Q_OBJECT

public:
	CardsEditBackground(QWidget *parent = Q_NULLPTR);
	~CardsEditBackground();

	void resizeEvent(QResizeEvent *event);//��������¼�
	void cardUISizeAdjust();//���µ������Ʊ༭����
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ

	QPushButton *saveAndQuit_button;//���沢�˳���ť

signals:
	void toEdit();//�༭����

private:
	Ui::CardsEditBackground ui;

	void init();//��ʼ�����Ʊ༭����
	void updateCoordinate();//��������
	void updatePictures();//���¿���ͼ��λ��
	void putInText();//��������Ϣ�洢���ı��ļ���
	void getFromText();//���ı��ļ��л�ȡ������Ϣ

	QGraphicsView *view;//�ӿ�
	CardsScene *scene;//����
	CardStack *stack;//����

	volatile bool Pressed;//�����ã���������жϱ���
	CardsUI* selected_card;//Ŀǰ����ѡ��Ŀ���

	QList<int> cardStackNo;//����Ϸ�����еĿ��ƺ���
	QList<CardsUI *> cardUILists;//����ͼƬ�б�
	QList<QPointF> cardUIPosLists;//����ͼƬλ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬͼ���б�

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void isPressed();//��갴�º���
	void isReleased();//����ͷź���
	void selectionChanged();//ѡ�����ı亯��
	void editStacks();//�༭����


};
