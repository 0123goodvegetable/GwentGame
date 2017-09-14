//�������Ϸ��صĽ���
#pragma once

#include <QWidget>
#include "ui_GamePlayingBackground.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include<QResizeEvent>
#include<QKeyEvent>
#include<QTimerEvent>
#include<QPushButton>
#include<QMouseEvent>
#include<QStringList>
#include<QLabel>
#include<QTimer>
#include<QLCDNumber>


#include"CP_Card.h"
#include"CP_PlayingLogic.h"
#include"CP_AllCards.h"
#include"CardsUI.h"
#include"CardsScene.h"


class GamePlayingBackground : public QWidget
{
	Q_OBJECT

public:
	GamePlayingBackground(int round = 0, QWidget *parent = Q_NULLPTR);
	~GamePlayingBackground();

	void resizeEvent(QResizeEvent*event);//��������¼�
	void keyPressEvent(QKeyEvent *event);//�����¼�

	CardsUI *selectedCardUI();//ѡȡ���ڲ����Ŀ���
	bool isCardUIClicked();//�ж��Ƿ�������ͼƬ
	void cardUISizeAdjust();//���µ���ѡ�ƽ�������

	public slots:
	void putInEnemyText();//���ҷ����ݴ����ļ����д���
	void getFromEnemyText();//���ļ��л�ȡ�з���Ϣ
	void changeMyTurn();//�����ҷ��ִ�
	void enemyEnd() { enemy_end = true; }//�з�����
	void timing();//��ʱ

signals:
	void toUseSkills(Card *card);//�ͷż��ܵ��ź�

	void toSendFile(QString filename);//���洢�ҷ����ݵ��ļ����䵽��һ��

	void chooseEnd();//ѡ�����

	void sendFinal(int i, int me_final_num,int enemy_final_num);//������

private:
	Ui::GamePlayingBackground ui;

	void init();//��ʼ������
	void updateStatus();//������Ϸ״̬
	void updateCoordinate();//������Ϸ���泣��
	void updateStack(QList<CardsUI *> stack);//����������Ϣ

	void useChooseScene(CardsUI *root_card);//����root_card���ó���

	QGraphicsView *view;
	CardsScene *main_scene;//������Ϸ����
	CardsScene *choose_scene;//���Ʋ�����Ϸ����
	QLabel *turnTextLabel;//��ʾ�ִε��ı�
	QLabel *myAllAttackLabel;//�ҷ�ս����ֵ
	QLabel *enemyAllAttackLabel;//�з�ս����ֵ
	QLCDNumber *timeShowLCD;//��ʾ��ʱ����Ļ
	QTimer *clock;//��ʱ��

	QLabel *m_Melee_weather_Label;
	QLabel *m_Archer_weather_Label;
	QLabel *m_Siege_weather_Label;
	QLabel *e_Melee_weather_Label;
	QLabel *e_Archer_weather_Label;
	QLabel *e_Siege_weather_Label;

	volatile bool Pressed;//�����ã���������жϱ���
	int second_number;//����������
	int my_round;//�ڼ��غ�
	bool my_turn;//�ҷ��ִ�
	bool me_end;//�ҷ�ѡ�����
	bool enemy_end;//�з�����
	bool useMainScene;//ʹ��������
	bool operation;//�Ƿ���ĳ�ſ��Ƶ��ƶ���
	bool isUsingSkill;//�Ƿ���ʹ�ü��ܽ׶�
	bool cardExist;//����Ҫѡȡ�Ŀ���
	int usingSkillTimes;//ʹ�ü��ܴ���
	bool isGoldCardOut;//���һ�Ž�ɫ�ƣ��ܲ����ܣ�

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
	QList<CardsUI *> cardUILists;//�����ڳ��Ͽ���ͼƬ�б�
	QList<CardsUI *> allCardUILists;//�ҷ����������п����б�
	QList<QPointF> cardUIPosLists;//����ͼƬλ���б�
	QList<QPixmap> cardUIPixmapLists;//����ͼƬͼ���б�
	QList<int> my_cardStackNo;//��Ϸ���ѷ�������Ϣ
	QList<int> all_cardStackNo;//���������п�����Ϣ

	private slots:
	void isMoving(QPointF &pos);//����ƶ�����
	void CardisPressed();//��갴�¿��ƺ���
	void CardisReleased();//����ͷſ��ƺ���
	void selectionChanged();//ѡ�����ı亯��
	void putInText();//��������Ϣ�洢���ı��ļ���
	void getFromText();//���ı��ļ��л�ȡ������Ϣ
	void useSkills(Card *card);//ʹ�ü���
	void whetherUseActiveSkill();//�ж��Ƿ���Է�����������
	void whetherUseFollowSkill();//�ж��Ƿ���Է�����������

};
