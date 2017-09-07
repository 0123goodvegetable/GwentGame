#include "GamePlayingBackground.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPixmap>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QDebug>
#include<QFile>
#include<QTextStream>

//定义全局变量
const qreal CARD_DIS = 80;//卡牌间距

int SCREEN_WIDTH2 = 1800;//画面宽度
int SCREEN_HEIGHT2 = 1000;//画面高度

int CARD_WIDTH;//卡牌宽度
int CARD_HEIGHT;//卡牌高度

int COLUMN_POS_X;//卡牌栏x坐标

int PREPARE_COLUMN_POS_Y;//未打出牌所在卡牌栏y坐标

int M_MELEE_COLUMN_POS_Y;//我方近战栏y坐标
int M_ARCHER_COLUMN_POS_Y;//我方远程栏y坐标
int M_SIEGE_COLUMN_POS_Y;//我方攻城栏y坐标

int E_MELEE_COLUMN_POS_Y;//敌方近战栏y坐标
int E_ARCHER_COLUMN_POS_Y;//敌方远程栏y坐标
int E_SIEGE_COLUMN_POS_Y;//敌方攻城栏y坐标

int COLUMN_LENGTH;//卡牌栏长度



GamePlayingBackground::GamePlayingBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//初始化所有全局变量
	updateCoordinate();

	//初始化窗口
	init();
}

GamePlayingBackground::~GamePlayingBackground()
{
}

void GamePlayingBackground::init()
{
	//初始化各个成员变量
	Pressed = false;
	operation = false;


	//从文本中获取牌组
	getFromText();

	view = new QGraphicsView(this);
	scene1 = new CardsScene();
	scene2 = new CardsScene();

	//设置窗口属性（没有滚动条）
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//建立图像移动的信号槽(游戏主界面）
	connect(scene1, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene1, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	//建立释放技能的信号槽
	connect(this, SIGNAL(toUseSkills()), this, SLOT(useSkills()));

	CardsUI *temp_card;
	QPointF pos;
	//完善卡牌信息
	for (int i = 0; i < my_cardStackNo.size(); i++)
	{
		temp_card = new CardsUI(my_cardStackNo.at(i));
		pos = QPointF(COLUMN_POS_X + CARD_DIS * i, PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);

		PlayingCard.append(*temp_card->operating_card);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
	}

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 4;

		//用于卡牌的点击
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));

		//向场景中添加部件
		scene1->addItem(card_temp);
		i++;
	}

	//调整卡牌尺寸
	cardUISizeAdjust();

	//设置视口view的属性
	view->setScene(scene1);//初始化为游戏主界面
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);

}

void GamePlayingBackground::updateCoordinate()
{


	CARD_HEIGHT = SCREEN_HEIGHT2*0.123;
	CARD_WIDTH = SCREEN_WIDTH2*0.06;

	COLUMN_POS_X = 0.3*SCREEN_WIDTH2;

	PREPARE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.84;

	M_MELEE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.51;
	M_ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT2*0.62;
	M_SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.73;

	E_MELEE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.37;
	E_ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT2*0.26;
	E_SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.16;

	COLUMN_LENGTH = SCREEN_WIDTH2*0.414;
}

void GamePlayingBackground::updateStatus()
{

}

//槽函数，鼠标拖拽卡牌到牌场上
void GamePlayingBackground::isMoving(QPointF &pos)
{
	int num = cardUILists.indexOf(selected_card);
	int pos_x, pos_y;
	pos_x = cardUIPosLists[num].x() + pos.x();
	pos_y = cardUIPosLists[num].y() + pos.y();


	if (Pressed == true && operation == true &&
		pos_x >= COLUMN_POS_X&&pos_x <= COLUMN_POS_X + COLUMN_LENGTH&&
		pos_y >= E_SIEGE_COLUMN_POS_Y&&pos_y < PREPARE_COLUMN_POS_Y)//确保在操作牌且将其拖到牌场中
	{
		//设置卡牌状态为在场上
		PlayingCard[num].isFielded = true;
		cardUILists[num]->operating_card->isFielded = true;
		//根据不同种类安放不同位置
		switch (selected_card->operating_card->genre)
		{
		case 0:
			cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
			break;
		case 1:
			cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
			break;
		case 2:
			cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
			break;
		case 3:
			if (pos_y < M_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
			}
			else if (pos_y >= M_ARCHER_COLUMN_POS_Y&&pos_y < M_SIEGE_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
			}
			else
			{
				cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
			}
			break;
		case 4:
			if (pos_y < E_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, E_SIEGE_COLUMN_POS_Y);
			}
			else if (pos_y >= E_ARCHER_COLUMN_POS_Y&&pos_y < E_MELEE_COLUMN_POS_Y)
			{
				cardUILists[num]->setPos(pos_x, E_ARCHER_COLUMN_POS_Y);
			}
			else
			{
				cardUILists[num]->setPos(pos_x, E_MELEE_COLUMN_POS_Y);
			}
			break;
		case 5:
			cardUILists[num]->operating_card->isGarbaged = true;
		}
	}

}

//槽函数，当cardUI鼠标按下时执行，设置鼠标按下变量为真
void GamePlayingBackground::isPressed()
{
	Pressed = true;
}

//槽函数，当cardUI鼠标释放时执行
//若此时在操作一张卡牌，则释放技能
void GamePlayingBackground::isReleased()
{
	if (operation==true)//正在操作一张牌
	{
		emit toUseSkills();
		operation= false;
	}

	int i = 0;
	foreach(QPointF card_pos, cardUIPosLists)
	{
		Q_UNUSED(card_pos);
		cardUIPosLists[i] = cardUILists[i]->pos();
		i++;
	}

}

//槽函数，当scene的selectedItem变化时，发送同名信号到此槽
void GamePlayingBackground::selectionChanged()
{
	int i = 0;

	QList<QGraphicsItem *> items = scene1->selectedItems();
	if (items.count() == 1)
	{
		//当前所选择的UI图标的坐标
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

		foreach(CardsUI* card, cardUILists)
		{
			if (card == card_temp)
				break;
			i++;
		}

		if (operation == false)//没有在操作卡牌
		{
			selected_card = cardUILists[i];
			operation = true;
		}
	}
	else
	{
		return;
	}
}

//槽函数，用于卡牌施放技能
void GamePlayingBackground::useSkills()
{

}

//判断cardUI接收的是否是单击信号。
//判断依据是当前单击的cardUI对象的pos与存储在cardUIPosListsd的位置比较，相等则为单击。
//用于选取作用的卡牌
bool GamePlayingBackground::isCardUIClicked()
{
	int i = -1;
	QList<QGraphicsItem *> items = scene1->selectedItems();

	if (items.count() == 1)
	{
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());
		i = cardUILists.indexOf(card_temp);
		if (pos == cardUIPosLists.at(i))
		{
			return true;
		}
	}
	return false;
}

void GamePlayingBackground::cardUISizeAdjust()
{
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{

		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(CARD_WIDTH, CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);
		i++;
	}
}

void GamePlayingBackground::getFromText()
{
	QFile file("playingCardStack.txt");

	if (file.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file);
		QString temp_No;
		while (!inPut.atEnd())
		{
			temp_No = inPut.readLine();
			my_cardStackNo.append(temp_No.toInt());
		}
	}

	file.close();
}

void GamePlayingBackground::putInText()
{
	QFile file("playingCardStack.txt");

	if (file.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI *card, cardUILists)
		{
			outPut << card->operating_card->No;
			outPut << "\n";
		}
	}
	file.close();
}

void GamePlayingBackground::resizeEvent(QResizeEvent *event)
{
	//添加背景
	QPixmap background(":/backgrounds/Resources/backgrounds/gamePlaying_background.jpg");//加载背景图片
	QPalette palette;
	view->setGeometry(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());//设置视口尺寸与窗口一样大
	palette.setBrush(view->backgroundRole(),
		QBrush(background.scaled(view->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//设置背景画刷
	view->setPalette(palette);

	//调整画面的大小
	scene1->setSceneRect(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());

	//更新全局变量
	SCREEN_WIDTH2 = this->width();
	SCREEN_HEIGHT2 = this->height();
	updateCoordinate();

}
