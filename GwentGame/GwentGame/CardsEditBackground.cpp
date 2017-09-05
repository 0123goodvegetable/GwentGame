#include "CardsEditBackground.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QPixmap>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QDebug>

#include"CP_AllCards.h"


//����ȫ�ֱ���
const qreal CARD_DIS = 150;//���Ƽ��
const int CARDS_NUM = 29;//���п�������

int SCREEN_WIDTH = 1800;//������
int SCREEN_HEIGHT = 1000;//����߶�

int NONSELECTED_CARD_WIDTH;//���·�����δѡ��Ŀ��ƿ��
int NONSELECTED_CARD_HEIGHT;//���·�����δѡ��Ŀ��Ƹ߶�
int SELECTED_CARD_WIDTH;//��ѡ��Ŀ��ƿ��
int SELECTED_CARD_HEIGHT;//��ѡ���Ƹ߶�

int FIGHT_COLUMN_POS_X;// ��ս��Զ�̡�����������x����
int FUNCTION_COLUMN_POS_X;//��������x����
int BASIS_COLUMN_POS_X;//δ��ѡ������x����
int MELEE_COLUMN_POS_Y;//��ս��y����
int ARCHER_COLUMN_POS_Y;//Զ����y����
int SIEGE_COLUMN_POS_Y;//������y����
int FUNCTION_COLUMN_POS_Y;//��������y����
int BASIS_COLUMN_POS_Y;//δ��ѡ������y����

int FIGHT_COLUMN_LENGTH;//��ս��Զ�̡�������������
int FUNCTION_COLUMN_LENGTH;//������������
int BASIS_COLUMN_LENGTH;//δ��ѡ����������

CardsEditBackground::CardsEditBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	updateCoordinate();//��ʼ����������

	init();//��ʼ������
}

CardsEditBackground::~CardsEditBackground()
{
}

void CardsEditBackground::init()
{
	//��ʼ��
	Pressed = false;

	view = new QGraphicsView(this);
	scene = new CardsScene();
	stack = new CardStack();

	//����ͼ���ƶ����źŲ�
	connect(scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	CardsUI *temp_card;
	QPointF pos;
	
	//������༭���м������п���
	for (int i = 1; i <= CARDS_NUM; i++)
	{

	}

}

void  CardsEditBackground::updateCoordinate()
{

	 NONSELECTED_CARD_WIDTH = SCREEN_WIDTH*0.06;//���·�����δѡ��Ŀ��ƿ��
	 NONSELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.14;//���·�����δѡ��Ŀ��Ƹ߶�
	 SELECTED_CARD_WIDTH = SCREEN_WIDTH*0.048;//��ѡ��Ŀ��ƿ��
	 SELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.114;//��ѡ���Ƹ߶�

	 FIGHT_COLUMN_POS_X = SCREEN_WIDTH*0.231;//��ս��Զ�̡�����������x����
	 FUNCTION_COLUMN_POS_X = SCREEN_WIDTH*0.248;//��������x����
	 BASIS_COLUMN_POS_X = SCREEN_WIDTH*0.222;//δ��ѡ������x����
	 MELEE_COLUMN_POS_Y = SCREEN_HEIGHT*0.1;//��ս��y����
	 ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT*0.224;//Զ����y����
	 SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT*0.346;//������y����
	 FUNCTION_COLUMN_POS_Y = SCREEN_HEIGHT*0.5;//��������y����
	 BASIS_COLUMN_POS_Y = SCREEN_HEIGHT*0.7405;//δ��ѡ������y����

	 FIGHT_COLUMN_LENGTH = SCREEN_WIDTH*0.5364;//��ս��Զ�̡�������������
	 FUNCTION_COLUMN_LENGTH = SCREEN_WIDTH*0.5; //������������
	 BASIS_COLUMN_LENGTH = SCREEN_WIDTH*0.5593;//δ��ѡ����������
}
