//�������Ϸ��صĽ���
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QPushButton>
#include<QMouseEvent>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CP_AllCards.h"
#include"CardsUI.h"
#include"CardsScene.h"


class GamePlayingBackground : public QWidget
{
	Q_OBJECT

public:
	GamePlayingBackground(QWidget *parent = Q_NULLPTR);
	~GamePlayingBackground();

	void resizeEvent(QResizeEvent*event);//��������¼�

	CardsUI *selectedCardUI();//ѡȡ���ڲ����Ŀ���
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ
	void cardUISizeAdjust();//���µ���ѡ�ƽ�������

signals:
	void toUseSkills(Card *card);//�ͷż��ܵ��ź�

private:
	Ui::GamePlayingBackground ui;

	void init();//��ʼ������
	void updateStatus();//������Ϸ״̬
	void updateCoordinate();//������Ϸ���泣��
	void updateStack(QList<CardsUI *> stack);//����������Ϣ

	QGraphicsView *view;
	CardsScene *scene1;//������Ϸ����
	CardsScene *scene2;//���Ʋ�����Ϸ����

	volatile bool Pressed;//�����ã���������жϱ���
	bool operation;//�Ƿ���ĳ�ſ��Ƶ��ִ���
	bool isUsingSkill;//�Ƿ���ʹ�ü��ܽ׶�
	int usingSkillTimes;//ʹ�ü��ܴ���
	PlayingLogic *conductor;//�ͷż���ʱ��ָ��
	AllCards allCards;//���п�����Ϣ
	QPoint cursor_point;//�������

	int m_Melee_weather;//�ҷ���ս���ܵ�����Ӱ�죨0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩
	int m_Archer_weather;//�ҷ�Զ�����ܵ�����Ӱ�죨0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩
	int m_Siege_weather;//�ҷ��������ܵ�����Ӱ�죨0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩
	int e_Melee_weather;//�з���ս���ܵ�����Ӱ�죨0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩
	int e_Archer_weather;//�з�Զ�����ܵ�����Ӱ�죨0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩
	int e_Siege_weather;//�з��������ܵ�����Ӱ�죨0-�ޣ�1-����Ũ��2-�̹Ǳ�˪��3-������꣩

	CardsUI  *selected_card;//��ǰѡ��Ŀ���
	CardsUI *usingSkill_card;//��Ҫ�ͷż��ܵĿ���
	QList<CardsUI *> cardUILists;//����ͼƬ�б�
	QList<QPointF> cardUIPosLists;//����ͼƬλ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬͼ���б�
	QList<int> my_cardStackNo;//��Ϸ���ѷ�������Ϣ

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void CardisPressed();//��갴�¿��ƺ���
	void CardisReleased();//����ͷſ��ƺ���
	void selectionChanged();//ѡ�����ı亯��
	void putInText();//��������Ϣ�洢���ı��ļ���
	void getFromText();//���ı��ļ��л�ȡ������Ϣ
	void useSkills(Card *card);//ʹ�ü���

};
