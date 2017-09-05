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


//定义全局变量
const qreal CARD_DIS = 150;//卡牌间距
const int CARDS_NUM = 29;//所有卡牌数量

int SCREEN_WIDTH = 1800;//画面宽度
int SCREEN_HEIGHT = 1000;//画面高度

int NONSELECTED_CARD_WIDTH;//在下方框内未选择的卡牌宽度
int NONSELECTED_CARD_HEIGHT;//在下方框内未选择的卡牌高度
int SELECTED_CARD_WIDTH;//已选择的卡牌宽度
int SELECTED_CARD_HEIGHT;//已选择卡牌高度

int FIGHT_COLUMN_POS_X;// 近战、远程、攻城三栏的x坐标
int FUNCTION_COLUMN_POS_X;//功能牌栏x坐标
int BASIS_COLUMN_POS_X;//未被选择卡牌栏x坐标
int MELEE_COLUMN_POS_Y;//近战栏y坐标
int ARCHER_COLUMN_POS_Y;//远程栏y坐标
int SIEGE_COLUMN_POS_Y;//攻城栏y坐标
int FUNCTION_COLUMN_POS_Y;//功能牌栏y坐标
int BASIS_COLUMN_POS_Y;//未被选择卡牌栏y坐标

int FIGHT_COLUMN_LENGTH;//近战、远程、攻城三栏长度
int FUNCTION_COLUMN_LENGTH;//功能牌栏长度
int BASIS_COLUMN_LENGTH;//未被选择卡牌栏长度

CardsEditBackground::CardsEditBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	updateCoordinate();//初始化所有坐标

	init();//初始化窗口
}

CardsEditBackground::~CardsEditBackground()
{
}

void CardsEditBackground::init()
{
	//初始化
	Pressed = false;

	view = new QGraphicsView(this);
	scene = new CardsScene();
	stack = new CardStack();

	//建立图像移动的信号槽
	connect(scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	CardsUI *temp_card;
	QPointF pos;
	
	//向牌组编辑器中加入所有卡牌
	for (int i = 1; i <= CARDS_NUM; i++)
	{

	}

}

void  CardsEditBackground::updateCoordinate()
{

	 NONSELECTED_CARD_WIDTH = SCREEN_WIDTH*0.06;//在下方框内未选择的卡牌宽度
	 NONSELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.14;//在下方框内未选择的卡牌高度
	 SELECTED_CARD_WIDTH = SCREEN_WIDTH*0.048;//已选择的卡牌宽度
	 SELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.114;//已选择卡牌高度

	 FIGHT_COLUMN_POS_X = SCREEN_WIDTH*0.231;//近战、远程、攻城三栏的x坐标
	 FUNCTION_COLUMN_POS_X = SCREEN_WIDTH*0.248;//功能牌栏x坐标
	 BASIS_COLUMN_POS_X = SCREEN_WIDTH*0.222;//未被选择卡牌栏x坐标
	 MELEE_COLUMN_POS_Y = SCREEN_HEIGHT*0.1;//近战栏y坐标
	 ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT*0.224;//远程栏y坐标
	 SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT*0.346;//攻城栏y坐标
	 FUNCTION_COLUMN_POS_Y = SCREEN_HEIGHT*0.5;//功能牌栏y坐标
	 BASIS_COLUMN_POS_Y = SCREEN_HEIGHT*0.7405;//未被选择卡牌栏y坐标

	 FIGHT_COLUMN_LENGTH = SCREEN_WIDTH*0.5364;//近战、远程、攻城三栏长度
	 FUNCTION_COLUMN_LENGTH = SCREEN_WIDTH*0.5; //功能牌栏长度
	 BASIS_COLUMN_LENGTH = SCREEN_WIDTH*0.5593;//未被选择卡牌栏长度
}
