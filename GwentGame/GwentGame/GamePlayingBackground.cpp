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
#include<QCursor>
#include<stdlib.h>
#include<time.h>

#define min(x,y) x<y?x:y//最小值
#define max(x,y) x>y?x:y//最大值

//定义全局变量
const qreal CARD_DIS = 80;//卡牌间距

const qreal CHOOSE_CARD_DIS = 250;
const qreal CHOOSE_CARD_HEIGHT = 300;

int SCREEN_WIDTH2 = 1800;//画面宽度
int SCREEN_HEIGHT2 = 960;//画面高度

int CARD_WIDTH;//卡牌宽度
int CARD_HEIGHT;//卡牌高度

int COLUMN_POS_X;//卡牌栏x坐标

int PREPARE_COLUMN_POS_Y;//未打出牌所在卡牌栏y坐标
int E_PREPARE_COLUMN_POS_Y;//敌方未打出的牌

int M_MELEE_COLUMN_POS_Y;//我方近战栏y坐标
int M_ARCHER_COLUMN_POS_Y;//我方远程栏y坐标
int M_SIEGE_COLUMN_POS_Y;//我方攻城栏y坐标

int E_MELEE_COLUMN_POS_Y;//敌方近战栏y坐标
int E_ARCHER_COLUMN_POS_Y;//敌方远程栏y坐标
int E_SIEGE_COLUMN_POS_Y;//敌方攻城栏y坐标

int COLUMN_LENGTH;//卡牌栏长度

//使用技能次数
const int UNSEEN_ELDER_SKILL_TIMES = 2;
const int NORMAL_SKILL_TIMES = 1;

GamePlayingBackground::GamePlayingBackground(int round,QWidget *parent)
	: QWidget(parent),
	my_round(round)
{
	ui.setupUi(this);

	//初始化所有常量
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
	isUsingSkill = false;
	usingSkillTimes = 0;
	useMainScene = true;
	isGoldCardOut = false;
	my_turn = false;
	me_end = false;
	enemy_end = false;

	m_Melee_weather = 0;
	m_Archer_weather = 0;
	m_Siege_weather = 0;
	e_Melee_weather = 0;
	e_Archer_weather = 0;
	e_Siege_weather = 0;


	//从文本中获取牌组
	getFromText();

	view = new QGraphicsView(this);
	main_scene = new CardsScene();
	choose_scene = new CardsScene();
	turnTextLabel = new QLabel(this);
	myAllAttackLabel = new QLabel(this);
	enemyAllAttackLabel = new QLabel(this);
	timeShowLCD = new QLCDNumber(this);
	timeShowLCD->setDigitCount(2);
	clock = new QTimer(this);
	clock->start(1000);

	m_Melee_weather_Label = new QLabel(this);
	m_Archer_weather_Label = new QLabel(this);
	m_Siege_weather_Label = new QLabel(this);
	e_Melee_weather_Label = new QLabel(this);
	e_Archer_weather_Label = new QLabel(this);
	e_Siege_weather_Label = new QLabel(this);


	//设置窗口属性（没有滚动条）
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//建立图像移动的信号槽(游戏主界面）
	connect(main_scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(main_scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	//建立释放技能的信号槽
	connect(this, SIGNAL(toUseSkills(Card *)), this, SLOT(useSkills(Card *)));

	//建立计时的信号槽
	connect(clock, SIGNAL(timeout()), this, SLOT(timing()));

	CardsUI *temp_card;
	QPointF pos;
	//完善卡牌信息
	for (int i = 0,j=0; i < my_cardStackNo.size(); i++,j++)
	{
		temp_card = new CardsUI(my_cardStackNo.at(i));
		pos = QPointF(COLUMN_POS_X + CARD_DIS * j, PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);
		if (temp_card->operating_card->isFriend == true && temp_card->operating_card->isHero == true)//友方领袖牌
		{
			temp_card->setPos(380, PREPARE_COLUMN_POS_Y);
			j--;
		}
		if (temp_card->operating_card->isFriend == false && temp_card->operating_card->isHero == true)//敌方领袖牌
		{
			temp_card->setPos(380, E_PREPARE_COLUMN_POS_Y);
			j--;
		}
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
	}

	for (int i = 0; i < all_cardStackNo.size(); i++)
	{
		temp_card = new CardsUI(all_cardStackNo.at(i));
		allCardUILists.append(temp_card);
	}

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 4;

		//用于卡牌的点击
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()),
			this, SLOT(CardisPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(CardisReleased()));

		//向场景中添加部件
		main_scene->addItem(card_temp);
		i++;
	}

	selected_card = cardUILists[0];//初始化

	//调整卡牌尺寸
	cardUISizeAdjust();

	//添加文本 
	QFont font;
	QPalette palette;
	if (my_turn == true)
	{
		turnTextLabel->setText(tr("My Turn!"));
	}
	else
	{
		turnTextLabel->setText(tr("Not My Turn!"));
	}

	font.setPixelSize(30); // 像素大小 
	turnTextLabel->setFont(font);
	palette.setColor(QPalette::WindowText, Qt::white);
	turnTextLabel->setPalette(palette);
	turnTextLabel->setGeometry(100, 430, 200, 50);

	myAllAttackLabel->setText(tr("0"));
	font.setPixelSize(40); // 像素大小 
	myAllAttackLabel->setFont(font);
	palette.setColor(QPalette::WindowText, Qt::white);
	myAllAttackLabel->setPalette(palette);
	myAllAttackLabel->setGeometry(150, 600, 100, 50);

	enemyAllAttackLabel->setText(tr("0"));
	font.setPixelSize(40); // 像素大小 
	enemyAllAttackLabel->setFont(font);
	palette.setColor(QPalette::WindowText, Qt::white);
	enemyAllAttackLabel->setPalette(palette);
	enemyAllAttackLabel->setGeometry(150, 300, 100, 50);

	//设置天气显示label
	font.setPixelSize(18);
	palette.setColor(QPalette::WindowText, Qt::white);

	m_Melee_weather_Label->setText("sunny");
	m_Archer_weather_Label->setText("sunny");
	m_Siege_weather_Label->setText("sunny");
	e_Melee_weather_Label->setText("sunny");
	e_Archer_weather_Label->setText("sunny");
	e_Siege_weather_Label->setText("sunny");

	m_Melee_weather_Label->setFont(font);
	m_Archer_weather_Label->setFont(font);
	m_Siege_weather_Label->setFont(font);
	e_Melee_weather_Label->setFont(font);
	e_Archer_weather_Label->setFont(font);
	e_Siege_weather_Label->setFont(font);

	m_Melee_weather_Label->setPalette(palette);
	m_Archer_weather_Label->setPalette(palette);
	m_Siege_weather_Label->setPalette(palette);
	e_Melee_weather_Label->setPalette(palette);
	e_Archer_weather_Label->setPalette(palette);
	e_Siege_weather_Label->setPalette(palette);

	e_Siege_weather_Label->setGeometry(409, 165, 90, 90);
	e_Archer_weather_Label->setGeometry(409, 271, 90, 90);
	e_Melee_weather_Label->setGeometry(409, 375, 90, 90);
	m_Melee_weather_Label->setGeometry(409, 503, 90, 90);
	m_Archer_weather_Label->setGeometry(409, 609, 90, 90);
	m_Siege_weather_Label->setGeometry(409, 711, 90, 90);

	//设置计时器位置
	timeShowLCD->setGeometry(1500, 600, 100, 80);

	//设置视口view的属性
	view->setScene(main_scene);//初始化为游戏主界面
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);

}

void GamePlayingBackground::changeMyTurn()
{
	if (me_end == false)
	{
		my_turn = true;
		turnTextLabel->setText(tr("My Turn!"));
		second_number = 30;
		clock->start(1000);
	}
	else
	{
		my_turn = false;
		turnTextLabel->setText(tr("End!"));
		putInEnemyText();
	}


	 //检测是否有受天气影响的卡牌
	 if (m_Melee_weather == 1)//近战排有蔽日浓雾
	 {
		 int num = -1,max_attack = 0;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_MELEE_COLUMN_POS_Y&&
				 card->operating_card->attack > max_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 2;
		 }
	 }
	 if (m_Archer_weather == 1)//远程排有蔽日浓雾
	 {
		 int num = -1,max_attack = 0;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_ARCHER_COLUMN_POS_Y&&
				 card->operating_card->attack > max_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 2;
		 }
	 }
	 if (m_Siege_weather == 1)//攻城排有蔽日浓雾
	 {
		 int num = -1,max_attack = 0;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_SIEGE_COLUMN_POS_Y&&
				 card->operating_card->attack > max_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 2;
		 }
	 }

	 if (m_Melee_weather == 2)//近战排有刺骨冰霜
	 {
		 int num = -1,min_attack = 100;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_MELEE_COLUMN_POS_Y&&
				 card->operating_card->attack < min_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 2;
		 }
	 }
	 if (m_Archer_weather == 2)//远程排有刺骨冰霜
	 {
		 int num = -1,min_attack = 100;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_ARCHER_COLUMN_POS_Y&&
				 card->operating_card->attack < min_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 2;
		 }
	 }
	 if (m_Siege_weather == 2)//攻城排有刺骨冰霜
	 {
		 int num = -1,min_attack = 100;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_SIEGE_COLUMN_POS_Y&&
				 card->operating_card->attack < min_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 2;
		 }
	 }

	 if (m_Melee_weather == 3)//近战排有倾盆大雨
	 {
		 int num = 2;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_MELEE_COLUMN_POS_Y)
			 {
				 cardUILists[cardUILists.indexOf(card)]->operating_card->attack--;
			 }
			 num--;
			 if (num <= 0)
			 {
				 break;
			 }
		 }
	 }
	 if (m_Archer_weather == 3)//远程排有倾盆大雨
	 {
		 int num = 2;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_ARCHER_COLUMN_POS_Y)
			 {
				 cardUILists[cardUILists.indexOf(card)]->operating_card->attack--;
			 }
			 num--;
			 if (num <= 0)
			 {
				 break;
			 }
		 }
	 }
	 if (m_Siege_weather == 3)//攻城排有倾盆大雨
	 {
		 int num = 2;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_SIEGE_COLUMN_POS_Y)
			 {
				 cardUILists[cardUILists.indexOf(card)]->operating_card->attack--;
			 }
			 num--;
			 if (num <= 0)
			 {
				 break;
			 }
		 }
	 }

	 if (m_Melee_weather == 4)//近战排有加强版刺骨冰霜（狂猎骑士技能）
	 {
		 int num = -1,min_attack = 100;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_MELEE_COLUMN_POS_Y&&
				 card->operating_card->attack < min_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 3;
		 }
	 }
	 if (m_Archer_weather == 4)//远程排有加强版刺骨冰霜（狂猎骑士技能）
	 {
		 int num = -1,min_attack = 100;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_ARCHER_COLUMN_POS_Y&&
				 card->operating_card->attack < min_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 3;
		 }
	 }
	 if (m_Siege_weather == 4)//攻城排有加强版刺骨冰霜（狂猎骑士技能）
	 {
		 int num = -1,min_attack = 100;
		 foreach(CardsUI *card, cardUILists)
		 {
			 if (card->pos().y() == M_SIEGE_COLUMN_POS_Y&&
				 card->operating_card->attack < min_attack)
			 {
				 num = cardUILists.indexOf(card);
			 }
		 }
		 if (num != -1)
		 {
			 cardUILists[num]->operating_card->attack -= 3;
		 }
	 }

	 updateStatus();
}

void GamePlayingBackground::updateCoordinate()
{

	CARD_HEIGHT = SCREEN_HEIGHT2*0.123;
	CARD_WIDTH = SCREEN_WIDTH2*0.06;

	COLUMN_POS_X = 0.3*SCREEN_WIDTH2;

	PREPARE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.87;
	E_PREPARE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.01;

	M_MELEE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.51;
	M_ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT2*0.62;
	M_SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.73;

	E_MELEE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.37;
	E_ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT2*0.265;
	E_SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.16;

	COLUMN_LENGTH = SCREEN_WIDTH2*0.414;
}

void GamePlayingBackground::updateStatus()
{
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, num7 = 0, num8 = 0, i = 0, my_allAttack = 0, enemy_allAttack = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		if (card_temp->operating_card->isGarbaged == true)//卡牌进入坟墓
		{
			main_scene->removeItem(card_temp);
		}
		else if (card_temp->operating_card->attack <= 0&&
			card_temp->operating_card->isFielded==true)
		{
			cardUILists[i]->operating_card->isGarbaged = true;
			main_scene->removeItem(card_temp);
		}
		else if(card_temp->operating_card->isFielded==false)//卡牌不在场上
		{
			if (card_temp->operating_card->isFriend == true && card_temp->operating_card->isHero==false)
			{
				card_temp->setPos(COLUMN_POS_X + num1*CARD_DIS, PREPARE_COLUMN_POS_Y);
				num1++;
			}
			else if(card_temp->operating_card->isFriend == false && card_temp->operating_card->isHero == false)
			{
				card_temp->setPos(COLUMN_POS_X + num8*CARD_DIS, E_PREPARE_COLUMN_POS_Y);
				num8++;
			}
			else if (card_temp->operating_card->isFriend == true && card_temp->operating_card->isHero == true)//友方领袖牌
			{
				card_temp->setPos(380, PREPARE_COLUMN_POS_Y);
			}
			else if (card_temp->operating_card->isFriend == false && card_temp->operating_card->isHero == true)//敌方领袖牌
			{
				card_temp->setPos(380, E_PREPARE_COLUMN_POS_Y);
			}
		}
		else if(card_temp->pos().y()==M_MELEE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num2*CARD_DIS,M_MELEE_COLUMN_POS_Y);
			cardUILists[i]->operating_card->isWeatherControlled = m_Melee_weather;
			num2++;
		}
		else if (card_temp->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num3*CARD_DIS, M_ARCHER_COLUMN_POS_Y);
			cardUILists[i]->operating_card->isWeatherControlled = m_Archer_weather;
			num3++;
		}
		else if (card_temp->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num4*CARD_DIS, M_SIEGE_COLUMN_POS_Y);
			cardUILists[i]->operating_card->isWeatherControlled = m_Siege_weather;
			num4++;
		}
		else if (card_temp->pos().y() == E_MELEE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num5*CARD_DIS, E_MELEE_COLUMN_POS_Y);
			cardUILists[i]->operating_card->isWeatherControlled = e_Melee_weather;
			num5++;
		}
		else if (card_temp->pos().y() == E_ARCHER_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num6*CARD_DIS, E_ARCHER_COLUMN_POS_Y);
			cardUILists[i]->operating_card->isWeatherControlled = e_Archer_weather;
			num6++;
		}
		else if (card_temp->pos().y() == E_SIEGE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num7*CARD_DIS, E_SIEGE_COLUMN_POS_Y);
			cardUILists[i]->operating_card->isWeatherControlled = e_Siege_weather;
			num7++;
		}

		i++;
	}

	i = 0;
	foreach(CardsUI *card, cardUILists)
	{
		cardUIPosLists[i] = card->pos();
		i++;
	}

	foreach(CardsUI *card, cardUILists)
	{
		if (card->operating_card->isFielded == true &&
			card->operating_card->isFriend == true)
		{
			my_allAttack += card->operating_card->attack;
		}
		if (card->operating_card->isFielded == true &&
			card->operating_card->isFriend == false)
		{
			enemy_allAttack += card->operating_card->attack;
		}
	}

	cardUISizeAdjust();

	//更新天气栏
	if (m_Melee_weather == 1)
	{
		m_Melee_weather_Label->setText("fog");
	}
	if (m_Archer_weather == 1)
	{
		m_Archer_weather_Label->setText("fog");
	}
	if (m_Siege_weather == 1)
	{
		m_Siege_weather_Label->setText("fog");
	}
	if (e_Melee_weather == 1)
	{
		e_Melee_weather_Label->setText("fog");
	}
	if (e_Archer_weather == 1)
	{
		e_Archer_weather_Label->setText("fog");
	}
	if (e_Siege_weather == 1)
	{
		e_Siege_weather_Label->setText("fog");
	}

	if (m_Melee_weather == 2|| m_Melee_weather == 4)
	{
		m_Melee_weather_Label->setText("frost");
	}
	if (m_Archer_weather == 2|| m_Archer_weather==4)
	{
		m_Archer_weather_Label->setText("frost");
	}
	if (m_Siege_weather == 2|| m_Siege_weather==4)
	{
		m_Siege_weather_Label->setText("frost");
	}
	if (e_Melee_weather == 2|| e_Melee_weather==4)
	{
		e_Melee_weather_Label->setText("frost");
	}
	if (e_Archer_weather == 2|| e_Archer_weather==4)
	{
		e_Archer_weather_Label->setText("frost");
	}
	if (e_Siege_weather == 2|| e_Siege_weather==4)
	{
		e_Siege_weather_Label->setText("frost");
	}

	if (m_Melee_weather == 3)
	{
		m_Melee_weather_Label->setText("rain");
	}
	if (m_Archer_weather == 3)
	{
		m_Archer_weather_Label->setText("rain");
	}
	if (m_Siege_weather == 3)
	{
		m_Siege_weather_Label->setText("rain");
	}
	if (e_Melee_weather == 3)
	{
		e_Melee_weather_Label->setText("rain");
	}
	if (e_Archer_weather == 3)
	{
		e_Archer_weather_Label->setText("rain");
	}
	if (e_Siege_weather == 3)
	{
		e_Siege_weather_Label->setText("rain");
	}

	if (my_allAttack < 0)
	{
		my_allAttack = 0;
	}
	if (enemy_allAttack < 0)
	{
		enemy_allAttack = 0;
	}
	myAllAttackLabel->setText(QString::number(my_allAttack));
	enemyAllAttackLabel->setText(QString::number(enemy_allAttack));

	emit sendFinal(my_round, myAllAttackLabel->text().toInt(),enemyAllAttackLabel->text().toInt());

}

void GamePlayingBackground::updateStack(QList<CardsUI*> stack)
{
	cardUILists.clear();
	foreach(CardsUI *card, stack)
	{
		cardUILists.append(card);
	}
}

//槽函数，鼠标拖拽卡牌到牌场上
void GamePlayingBackground::isMoving(QPointF &pos)
{
	if (my_turn == true)//是我的回合
	{
		int num = cardUILists.indexOf(selected_card);
		int pos_x, pos_y;
		pos_x = cardUIPosLists[num].x() + pos.x();
		pos_y = cardUIPosLists[num].y() + pos.y();


		if (Pressed == true && operation == true&&cardUILists[num]->operating_card->isFriend == true&&
			pos_x >= COLUMN_POS_X&&pos_x <= COLUMN_POS_X + COLUMN_LENGTH&&
			pos_y >= E_SIEGE_COLUMN_POS_Y&&pos_y < PREPARE_COLUMN_POS_Y)//确保在操作牌且将其拖到牌场中
		{
			//设置卡牌状态为在场上
			cardUILists[num]->operating_card->isFielded = true;
			//根据不同种类安放不同位置
			switch (selected_card->operating_card->genre)
			{
			case 0:
				cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = m_Melee_weather;
				break;
			case 1:
				cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = m_Archer_weather;

				break;
			case 2:
				cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
				cardUILists[num]->operating_card->isWeatherControlled = m_Siege_weather;
				break;
			case 3:
				if (pos_y < M_ARCHER_COLUMN_POS_Y)
				{
					cardUILists[num]->setPos(pos_x, M_MELEE_COLUMN_POS_Y);
					cardUILists[num]->operating_card->isWeatherControlled = m_Melee_weather;
				}
				else if (pos_y >= M_ARCHER_COLUMN_POS_Y&&pos_y < M_SIEGE_COLUMN_POS_Y)
				{
					cardUILists[num]->setPos(pos_x, M_ARCHER_COLUMN_POS_Y);
					cardUILists[num]->operating_card->isWeatherControlled = m_Archer_weather;
				}
				else
				{
					cardUILists[num]->setPos(pos_x, M_SIEGE_COLUMN_POS_Y);
					cardUILists[num]->operating_card->isWeatherControlled = m_Siege_weather;
				}
				break;
			case 4:
				if (pos_y < E_ARCHER_COLUMN_POS_Y)
				{
					cardUILists[num]->setPos(pos_x, E_SIEGE_COLUMN_POS_Y);
					cardUILists[num]->operating_card->isWeatherControlled = e_Siege_weather;
				}
				else if (pos_y >= E_ARCHER_COLUMN_POS_Y&&pos_y < E_MELEE_COLUMN_POS_Y)
				{
					cardUILists[num]->setPos(pos_x, E_ARCHER_COLUMN_POS_Y);
					cardUILists[num]->operating_card->isWeatherControlled = e_Archer_weather;
				}
				else
				{
					cardUILists[num]->setPos(pos_x, E_MELEE_COLUMN_POS_Y);
					cardUILists[num]->operating_card->isWeatherControlled = e_Melee_weather;
				}
				break;
			case 5:
				cardUILists[num]->operating_card->isGarbaged = true;
				cardUILists[num]->operating_card->isFielded = false;
				break;
			}
		}
	}


}

//槽函数，当cardUI鼠标按下时执行，设置鼠标按下变量为真
void GamePlayingBackground::CardisPressed()
{
	Pressed = true;
}

//槽函数，当cardUI鼠标释放时执行
//若此时在操作一张卡牌，则释放技能
void GamePlayingBackground::CardisReleased()
{
	if (my_turn == true)
	{
		if (operation == true && isUsingSkill == false)//正在操作一张牌
		{
			//更换光标图案
			QCursor cursor;
			QPixmap pixmap(":/cursors/Resources/cursor/cursor1_icon.png");
			pixmap.scaled(30, 30);
			cursor = QCursor(pixmap, -1, -1);
			this->setCursor(cursor);

			updateStatus();

			usingSkill_card = selected_card;
			isUsingSkill = true;

			whetherUseActiveSkill();


		}
		else if (operation == false && isUsingSkill == true)
		{

			if (usingSkill_card->operating_card->skill == 1 &&
				usingSkillTimes<UNSEEN_ELDER_SKILL_TIMES)//使用两次技能。。写的方法比较麻烦
			{
				cardExist = false;//再次检索
				foreach(CardsUI *card, cardUILists)
				{
					if (card->operating_card->isFriend == true &&
						card->operating_card->isFielded == true &&
						card->operating_card->skill != 1)
					{
						cardExist = true;
						break;
					}
				}
				if (cardExist == false)
				{
					usingSkillTimes++;
				}
				else
				{
					emit toUseSkills(usingSkill_card->operating_card);//使用技能
				}
			}
			else
			{
				emit toUseSkills(usingSkill_card->operating_card);//使用技能
			}
		}

	}
	whetherUseFollowSkill();

	view->viewport()->update();//立即重绘

}

//槽函数，当scene的selectedItem变化时，发送同名信号到此槽
void GamePlayingBackground::selectionChanged()
{
	if (my_turn == true)
	{
		if (useMainScene)
		{
			int i = 0;
			QList<QGraphicsItem *> items = main_scene->selectedItems();
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

				if (i > cardUILists.size())
				{
					return;
				}

				if (operation == false && isUsingSkill == false &&
					cardUILists[i]->operating_card->isFielded == false)
				{
					selected_card = cardUILists[i];//选取操作卡牌
					operation = true;
					if (selected_card->operating_card->material == 0 &&
						selected_card->operating_card->isHero == false)
					{
						isGoldCardOut = true;
					}
				}
				else if (isCardUIClicked() == true && isUsingSkill == true)//使用技能
				{
					selected_card = cardUILists[i];
					operation = false;
				}

			}
			else
			{
				return;
			}
		}
		else if (useMainScene == false)
		{
			QList<QGraphicsItem *> items = choose_scene->selectedItems();
			CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

			if (isUsingSkill == true)
			{
				selected_card = card_temp;
				operation = false;
			}

		}
	}

}


//判断cardUI接收的是否是单击信号。
//判断依据是当前单击的cardUI对象的pos与存储在cardUIPosListsd的位置比较，相等则为单击。
//用于选取作用的卡牌
bool GamePlayingBackground::isCardUIClicked()
{
	int i = -1;
	if (useMainScene)
	{
		QList<QGraphicsItem *> items = main_scene->selectedItems();

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
	}
	return false;

}

void GamePlayingBackground::cardUISizeAdjust()
{
	//卡牌尺寸调整
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{
		if (card->operating_card->isFriend == false && card->operating_card->isFielded == false &&card->operating_card->isHero==false)
		{
			QPixmap temp_back(":/cards/Resources/card/cardBack.png");
			temp_back = temp_back.scaled(CARD_WIDTH, CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(temp_back);
			cardUILists[i]->using_background = 0;

		}
		else
		{
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(CARD_WIDTH, CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
			cardUILists[i]->using_background = 4;
		}
		i++;
	}
}

void GamePlayingBackground::getFromText()
{
	QFile file1("my_playingCardStack.txt");

	if (file1.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file1);
		QString temp_No;
		while (!inPut.atEnd())
		{
			temp_No = inPut.readLine();
			my_cardStackNo.append(temp_No.toInt());
		}
	}

	file1.close();

	QFile file2("gameCardStack.txt");

	if (file2.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file2);
		QString temp_No;
		while (!inPut.atEnd())
		{
			temp_No = inPut.readLine();
			all_cardStackNo.append(temp_No.toInt());
		}
	}

	file2.close();
}

void GamePlayingBackground::putInText()
{
	QFile file("my_leftCardStack.txt");
	QList<int> leftCardNo;

	if (file.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFielded == false&&
				card->operating_card->isFriend==true&&
				card->operating_card->isGarbaged==false)//未上场余留手牌
			{
				outPut << card->operating_card->No;
				outPut << "\n";
				leftCardNo.append(card->operating_card->No);
			}
		}
	}
	file.close();

	QFile file2("my_usedCardStack.txt");

	if (file2.open(QFile::WriteOnly | QIODevice::Append| QIODevice::Text))
	{
		QTextStream outPut(&file2);

		foreach(int No, my_cardStackNo)
		{
			if (!leftCardNo.contains(No))//已使用手牌
			{
				outPut << No;
				outPut << "\n";
			}
		}
	}
	file2.close();
}

void GamePlayingBackground::putInEnemyText()
{
	QFile file("all_playingCardStack.txt");

	//传输信息顺序：
	//No-number-attack-armer-isShield-isFriend-isFielded-isGarbaged-isUseFollowSkill-isWeatherControlled-position
	
	
	if (file.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI *card, cardUILists)
		{
			QString data = QString::number(card->operating_card->No) + "_" + QString::number(card->operating_card->number) + "_" + QString::number(card->operating_card->attack) 
				+ "_" + QString::number(card->operating_card->armer) + "_" +QString::number(card->operating_card->isShield) + "_" + QString::number(1 - card->operating_card->isFriend) 
				+ "_" + QString::number(card->operating_card->isFielded) + "_" + QString::number(card->operating_card->isGarbaged)
				+ "_" +	QString::number(card->operating_card->isUseFollowSkill) + "_" + QString::number(card->operating_card->isWeatherControlled);
			//确定卡牌坐标信息
			QString position;
			if (card->pos().y() == PREPARE_COLUMN_POS_Y)
			{
				position = QString::number(E_PREPARE_COLUMN_POS_Y);
			}
			if (card->pos().y() == M_MELEE_COLUMN_POS_Y)
			{
				position = QString::number(E_MELEE_COLUMN_POS_Y);
			}
			if (card->pos().y() == M_ARCHER_COLUMN_POS_Y)
			{
				position = QString::number(E_ARCHER_COLUMN_POS_Y);
			}
			if (card->pos().y() == M_SIEGE_COLUMN_POS_Y)
			{
				position = QString::number(E_SIEGE_COLUMN_POS_Y);
			}
			if (card->pos().y() == E_MELEE_COLUMN_POS_Y)
			{
				position = QString::number(M_MELEE_COLUMN_POS_Y);
			}
			if (card->pos().y() == E_ARCHER_COLUMN_POS_Y)
			{
				position = QString::number(M_ARCHER_COLUMN_POS_Y);
			}
			if (card->pos().y() == E_SIEGE_COLUMN_POS_Y)
			{
				position = QString::number(M_SIEGE_COLUMN_POS_Y);
			}
			if (card->pos().y() == E_PREPARE_COLUMN_POS_Y)
			{
				position = QString::number(PREPARE_COLUMN_POS_Y);
			}
			outPut << data + "_" + position;
			outPut << "\n";
		}
	}
	file.close();

	emit toSendFile("all_playingCardStack.txt");

	my_turn = false;

	if (me_end == false)
	{
		turnTextLabel->setText(tr("Not My Turn!"));
		if (clock->isActive())
		{
			clock->stop();
			second_number = 0;
			timeShowLCD->display(0);
		}
	}
	else
	{
		turnTextLabel->setText(tr("End!"));
	}

}

void GamePlayingBackground::getFromEnemyText()
{
	QFile file("all_playingCardStack.txt");

	if (file.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file);
		QString line;//读取一行的数据
		QStringList lineData;//每一行进行数据分析
		//No - number - attack - armer - isShield - isFriend - isFielded - isGarbaged - isUseFollowSkill - isWeatherControlled - position
		int temp_No = 0;
		int temp_number = 0;
		int temp_attack = 0;
		int temp_armer = 0;
		bool temp_isShield = false;
		bool temp_isFriend = false;
		bool temp_isFielded = false;
		bool temp_isGarbaged = false;
		bool temp_isUseFollowSkill = false;
		int temp_isWeatherControlled = 0;
		int temp_position = 0;
		CardsUI *temp_card;
		QPointF pos;
		int num = 0;

		while (!inPut.atEnd())
		{
			line = inPut.readLine();
			lineData = line.split('_');
			if (lineData.size() != 0)
			{
				temp_No = lineData[0].toInt();
				temp_number = lineData[1].toInt();
				temp_attack = lineData[2].toInt();
				temp_armer = lineData[3].toInt();
				temp_isShield = lineData[4].toInt();
				temp_isFriend = lineData[5].toInt();
				temp_isFielded = lineData[6].toInt();
				temp_isGarbaged = lineData[7].toInt();
				temp_isUseFollowSkill = lineData[8].toInt();
				temp_isWeatherControlled = lineData[9].toInt();
				temp_position = lineData[10].toInt();

				if (temp_No == allCards.Frightener_No)
				{
					int num = 0;
					foreach(CardsUI *card, cardUILists)
					{
						if (card->operating_card->No == allCards.Frightener_No)
						{
							num++;
						}
					}
					if (num >= 2)
					{
						continue;
					}
				}

				//更新牌组信息
				int i = 0;
				foreach(CardsUI *card, cardUILists)
				{
					if (card->operating_card->No == temp_No&&
						card->operating_card->isFriend==temp_isFriend&&
						card->operating_card->number == temp_number)
					{
						break;
					}
					i++;
				}

				if (i >= cardUILists.size())//如果卡牌不在牌组中，需要添加卡牌
				{
					temp_card = new CardsUI(temp_No);
					pos = QPointF(COLUMN_POS_X, temp_position);
					temp_card->setPos(pos);

					cardUILists.append(temp_card);
					cardUIPosLists.append(pos);
					cardUIPixmapLists.append(temp_card->pixmap());

					num = cardUILists.indexOf(temp_card);
					cardUILists[num]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
					cardUILists[num]->using_background = 4;
					//用于卡牌的点击
					connect(cardUILists[num], SIGNAL(cardIsPressed()),
						this, SLOT(CardisPressed()));
					connect(cardUILists[num], SIGNAL(cardIsReleased()),
						this, SLOT(CardisReleased()));
					//向场景中添加部件
					main_scene->addItem(cardUILists[num]);
					cardUISizeAdjust();

					//更新卡牌信息
					cardUILists[num]->operating_card->number = temp_number;
					cardUILists[num]->operating_card->attack = temp_attack;
					cardUILists[num]->operating_card->armer = temp_armer;
					cardUILists[num]->operating_card->isShield = temp_isShield;
					cardUILists[num]->operating_card->isFriend = temp_isFriend;
					cardUILists[num]->operating_card->isFielded = temp_isFielded;
					cardUILists[num]->operating_card->isGarbaged = temp_isGarbaged;
					cardUILists[num]->operating_card->isUseFollowSkill = temp_isUseFollowSkill;
					cardUILists[num]->operating_card->isWeatherControlled = temp_isWeatherControlled;
					cardUILists[num]->setPos(COLUMN_POS_X, temp_position);
				}
				else
				{
					//更新卡牌信息
					cardUILists[i]->operating_card->number = temp_number;
					cardUILists[i]->operating_card->attack = temp_attack;
					cardUILists[i]->operating_card->armer = temp_armer;
					cardUILists[i]->operating_card->isShield = temp_isShield;
					cardUILists[i]->operating_card->isFriend = temp_isFriend;
					cardUILists[i]->operating_card->isFielded = temp_isFielded;
					cardUILists[i]->operating_card->isGarbaged = temp_isGarbaged;
					cardUILists[i]->operating_card->isUseFollowSkill = temp_isUseFollowSkill;
					cardUILists[i]->operating_card->isWeatherControlled = temp_isWeatherControlled;
					cardUILists[i]->setPos(COLUMN_POS_X, temp_position);
				}

			}

		}
	}
	file.close();
	foreach(CardsUI *card, cardUILists)//更改每一排的天气设定
	{
		if (card->operating_card->isWeatherControlled == 1 &&
			card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			//近战排蔽日浓雾
			m_Melee_weather = 1;
		}
		if (card->operating_card->isWeatherControlled == 1 &&
			card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			//远程排蔽日浓雾
			m_Archer_weather = 1;
		}
		if (card->operating_card->isWeatherControlled == 1 &&
			card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			//攻城排蔽日浓雾
			m_Siege_weather = 1;
		}

		if (card->operating_card->isWeatherControlled == 2 &&
			card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			//近战排刺骨冰霜
			m_Melee_weather = 2;
		}
		if (card->operating_card->isWeatherControlled == 2 &&
			card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			//远程排刺骨冰霜
			m_Archer_weather = 2;
		}
		if (card->operating_card->isWeatherControlled == 2 &&
			card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			//攻城排刺骨冰霜
			m_Siege_weather = 2;
		}

		if (card->operating_card->isWeatherControlled == 3 &&
			card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			//近战排倾盆大雨
			m_Melee_weather = 3;
		}
		if (card->operating_card->isWeatherControlled == 3 &&
			card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			//远程排倾盆大雨
			m_Archer_weather = 1;
		}
		if (card->operating_card->isWeatherControlled == 3 &&
			card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			//攻城排倾盆大雨
			m_Siege_weather = 1;
		}

		if (card->operating_card->isWeatherControlled == 4 &&
			card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			//近战排强化版刺骨冰霜
			m_Melee_weather = 4;
		}
		if (card->operating_card->isWeatherControlled == 4 &&
			card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			//远程排强化版刺骨冰霜
			m_Archer_weather = 4;
		}
		if (card->operating_card->isWeatherControlled == 4 &&
			card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			//攻城排强化版刺骨冰霜
			m_Siege_weather = 4;
		}

	}

	updateStatus();

	view->viewport()->update();//立即重绘

}

void GamePlayingBackground::useChooseScene(CardsUI *root_card)
{
	choose_scene->clear();
	useMainScene = false;
	int i;

	QPointF pos;
	int No[3] = { allCards.Biting_Frost_No,allCards.Impenetrable_Fog_No,allCards.Torrential_Rain_No };//三张天气牌信息
	QList<CardsUI *>garbaged_card;//墓地中的牌
	switch (root_card->operating_card->skill)
	{
	case 5://达冈
		CardsUI *weather_card[3];
		//设置卡牌
		for (int i = 0; i < 3; i++)
		{
			weather_card[i] = new CardsUI(No[i]);
			pos = QPointF(300 + CHOOSE_CARD_DIS * i, 200);
			weather_card[i]->setPos(pos);
			weather_card[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			//weather_card[i]->using_background = 4;

			//用于卡牌的点击
			connect(weather_card[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(weather_card[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));

			//向场景中添加部件
			choose_scene->addItem(weather_card[i]);

			QPixmap pixmap = weather_card[i]->pixmap();
			pixmap = pixmap.scaled(CHOOSE_CARD_HEIGHT*0.85, CHOOSE_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			weather_card[i]->setPixmap(pixmap);
		}
		break;

	case 6://大狮鹫
		CardsUI *tempcard1;
		i = 0;
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isGarbaged == true &&
				card->operating_card->isFriend == true&&
				card->operating_card->genre!=5)
			{
				tempcard1 = new CardsUI(card->operating_card->No);
				pos = QPointF(CHOOSE_CARD_DIS * i, 200);
				tempcard1->setPos(pos);
				garbaged_card.append(tempcard1);
				i++;
			}
		}
		for(i=0;i<garbaged_card.size();i++)
		{

			garbaged_card[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			//garbaged_card[i]->using_background = 4;
			//用于卡牌的点击
			connect(garbaged_card[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(garbaged_card[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));
			//向场景中添加部件
			choose_scene->addItem(garbaged_card[i]);

			QPixmap pixmap = garbaged_card[i]->pixmap();
			pixmap = pixmap.scaled(CHOOSE_CARD_HEIGHT*0.85, CHOOSE_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			garbaged_card[i]->setPixmap(pixmap);
			
		}
		break;

	case 7://盖尔
		CardsUI *two_cards[2];
		int j = 0, num = 0;
		for (int i = 0; i < 2; i++)
		{
			 while (true)
			{
				 if((allCardUILists[j]->operating_card->material==0||
					 allCardUILists[j]->operating_card->material == 1)&&
					 !my_cardStackNo.contains(all_cardStackNo[j])&&
					 allCardUILists[j]->operating_card->isHero==false)//是金卡或银卡且不在现有手牌中
				 {
					 num = all_cardStackNo[j];
					 j++;
					 break;
				 }
				 j++;
			}
			two_cards[i] = new CardsUI(num);
			pos = QPointF(300 + CHOOSE_CARD_DIS * i, 200);
			two_cards[i]->setPos(pos);
			two_cards[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			//two_cards[i]->using_background = 4;

			//用于卡牌的点击
			connect(two_cards[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(two_cards[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));

			//向场景中添加部件
			choose_scene->addItem(two_cards[i]);

			QPixmap pixmap = two_cards[i]->pixmap();
			pixmap = pixmap.scaled(CHOOSE_CARD_HEIGHT*0.85, CHOOSE_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			two_cards[i]->setPixmap(pixmap);
		}
		break;
	}
	//建立图像移动的信号槽(游戏主界面）
	connect(choose_scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	view->setScene(choose_scene);

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
	main_scene->setSceneRect(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());

	//更新全局变量
	//SCREEN_WIDTH2 = this->width();
	//SCREEN_HEIGHT2 = this->height();
	//updateCoordinate();

}

void GamePlayingBackground::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Space:
		emit chooseEnd();
		me_end = true;
		turnTextLabel->setText(tr("End!"));

		if (enemy_end == false)
		{
			putInEnemyText();
		}
		if (clock->isActive())
			clock->stop();
		putInText();
		break;
	}
}

void GamePlayingBackground::timing()
{
	if (second_number > 0)
	{
		timeShowLCD->display(second_number);
		second_number--;
	}
	else
	{
		second_number = 0;
		timeShowLCD->display(0);
		putInEnemyText();
	}
}

//判断是否可以发动主动技能
void  GamePlayingBackground::whetherUseActiveSkill()
{
	cardExist = false;
	int num = 0;
	switch (usingSkill_card->operating_card->skill)
	{
	case 1://暗影长者
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == true &&
				card->operating_card->isFielded == true &&
				card->operating_card->skill != 1)
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 2://贝克尔扭曲之镜,不需要选择对象
		emit toUseSkills(usingSkill_card->operating_card);//使用技能
		break;
	case 3://蔽日浓雾
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//在场上的敌方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 4://刺骨冰霜
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//在场上的敌方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 5://达冈,不需要选择对象
		useChooseScene(usingSkill_card);//选取要使用的天气牌
		break;
	case 6://大狮鹫
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == true &&
				card->operating_card->isGarbaged == true &&
				card->operating_card->genre != 5)//在墓地的我方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == true)
		{
			useChooseScene(usingSkill_card);//选取要置入对方墓地的卡牌
		}
		else
		{
			operation = false;
			isUsingSkill = true;
			CardisReleased();
		}
		break;
	case 7://盖尔
		num = 0;
		foreach(CardsUI *card, allCardUILists)
		{
			if ((card->operating_card->genre == 0 ||
				card->operating_card->genre == 1) &&
				card->operating_card->isHero==false&&
				!my_cardStackNo.contains(card->operating_card->No))//不在手牌中的非英雄金银卡
			{
				num++;
			}
		}
		if (num >= 2)
		{
			useChooseScene(usingSkill_card);//选取一张金卡或银卡

		}
		else
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = false;
			useMainScene = true;
			putInEnemyText();
		}
		break;
	case 8://杰洛特：伊格尼法印
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//在场上的敌方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			Card *temp_card = new Card(allCards.Roach_No);
			useSkills(temp_card);
			putInEnemyText();
		}
		break;
	case 9://卡兰希尔
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//在场上的敌方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 10://狂猎骑士
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true &&
				card->operating_card->isWeatherControlled == 2)//在场上的敌方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 11://老巫妪：呢喃婆
		foreach(CardsUI *card, allCardUILists)
		{
			if (card->operating_card->skill == 12 ||
				card->operating_card->skill == 13)
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		else
		{
			emit toUseSkills(usingSkill_card->operating_card);
		}
		break;
	case 12://老巫妪：织婆
		foreach(CardsUI *card, allCardUILists)
		{
			if (card->operating_card->skill == 11 ||
				card->operating_card->skill == 13)
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		else
		{
			emit toUseSkills(usingSkill_card->operating_card);
		}
		break;
	case 13://老巫妪：煮婆
		foreach(CardsUI *card, allCardUILists)
		{
			if (card->operating_card->skill == 12 ||
				card->operating_card->skill == 13)
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		else
		{
			emit toUseSkills(usingSkill_card->operating_card);
		}
		break;
	case 14://雷霆药水
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFielded == true)//在场上存在单位
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 15://林妖
		emit toUseSkills(usingSkill_card->operating_card);//使用技能
		break;
	case 16://暴怒的狼，无技能
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
		putInEnemyText();
		break;
	case 17://萝卜，无主动技能
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
		putInEnemyText();
		break;
	case 19://倾盆大雨
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true)//在场上的敌方英雄
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 20://赛尔伊诺鹰身女妖
		emit toUseSkills(usingSkill_card->operating_card);//使用技能
		break;
	case 21://撕裂
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFielded == true)//在场上有卡牌
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	case 22://土元素，无主动技能
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
		putInEnemyText();
		break;
	case 23://畏惧者
		num = 0;
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend == false &&
				card->operating_card->isFielded == true &&
				card->operating_card->No!=usingSkill_card->operating_card->No)//对家英雄
			{
				cardExist = true;
				break;
			}
			num++;
		}
		if (cardExist == false)//无需选择对象卡牌
		{
			operation = false;
			isUsingSkill = true;
			CardisReleased();
		}
		break;
	case 26://蟹蜘蛛
		foreach(CardsUI *card, allCardUILists)
		{
			if (card->operating_card->skill == 26)
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		else
		{
			emit toUseSkills(usingSkill_card->operating_card);
		}
		break;
	case 29://指挥号角
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFielded == true)//在场上有卡牌
			{
				cardExist = true;
				break;
			}
		}
		if (cardExist == false)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	}
}

//判断是否可以发动被动技能
void GamePlayingBackground::whetherUseFollowSkill()
{
	int i = 0;
	if (isGoldCardOut)//萝卜技能
	{
		Card *temp_card;
		foreach(CardsUI *card, allCardUILists)
		{
			if (card->operating_card->skill == 17 &&
				!my_cardStackNo.contains(card->operating_card->No))
			{
				temp_card = new Card(card->operating_card->No);
				emit toUseSkills(temp_card);
				break;
			}
		}	
	}

	//土元素技能
	int num = 0;
	foreach(CardsUI *card, cardUILists)
	{
		if (card->operating_card->No==allCards.Lesser_Earth_Elemental_No||
			card->operating_card->No == allCards.Lesser_Earth_Elemental2_No)
		{
			break;
		}
		num++;
	}
	if (num >= cardUILists.size())
	{
		i = 0;
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->skill == 22 &&
				card->operating_card->isFriend == true &&
				card->operating_card->isGarbaged == true &&
				card->operating_card->isUseFollowSkill == false)
			{
				emit toUseSkills(card->operating_card);
				cardUILists[i]->operating_card->isUseFollowSkill = true;
				break;
			}
			i++;
		}
	}
}

//槽函数，用于卡牌施放技能
void GamePlayingBackground::useSkills(Card *card)
{
	int i;
	QPointF pos;
	CardsUI* temp_card;
	switch (card->skill)
	{
	case 1://暗影长者

		//存在满足条件的卡牌
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == true &&
			selected_card->operating_card->skill != 1)//选取非自身的友军牌
		{
			int num = cardUILists.indexOf(selected_card);
			cardUILists[num]->operating_card->isSelected = true;
			Card card(allCards.Unseen_Elder_No);
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(card,1));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
			//CardisReleased();
		}
		//还原
		if (usingSkillTimes == UNSEEN_ELDER_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 2://贝克尔扭曲之镜
		conductor = new PlayingLogic(cardUILists);
		updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
		updateStatus();
		delete conductor;
		//还原
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
		putInEnemyText();
		break;

	case 3://蔽日浓雾
		//存在满足条件的卡牌
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == false)
		{
			//设置该排天气情况
			if (selected_card->pos().y() == E_MELEE_COLUMN_POS_Y)
			{
				e_Melee_weather = 1;
			}
			else if (selected_card->pos().y() == E_ARCHER_COLUMN_POS_Y)
			{
				e_Archer_weather = 1;
			}
			else if (selected_card->pos().y() == E_SIEGE_COLUMN_POS_Y)
			{
				e_Siege_weather = 1;
			}
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 4://刺骨冰霜
		//存在满足条件的卡牌
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == false)
		{
			//设置该排天气情况
			if (selected_card->pos().y() == E_MELEE_COLUMN_POS_Y)
			{
				e_Melee_weather = 2;
			}
			else if (selected_card->pos().y() == E_ARCHER_COLUMN_POS_Y)
			{
				e_Archer_weather = 2;
			}
			else if (selected_card->pos().y() == E_SIEGE_COLUMN_POS_Y)
			{
				e_Siege_weather = 2;
			}

			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 5://达冈
		//恢复原设定
		if (selected_card->operating_card->No != usingSkill_card->operating_card->No)
		{
			usingSkillTimes++;
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = true;
			useMainScene = true;
			usingSkill_card = selected_card;
			view->setScene(main_scene);
			CardisReleased();
		}
		break;

	case 6://大狮鹫
		//清除天气效果
		i = 0;
		foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
		{
			if (card->pos().y() == usingSkill_card->pos().y() &&
				card->operating_card->isFielded == true)
			{
				cardUILists[i]->operating_card->isSelected = true;
			}
			i++;
		}
		conductor = new PlayingLogic(cardUILists);
		updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
		updateStatus();
		delete conductor;
		if (usingSkill_card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			m_Melee_weather = 0;
		}
		if (usingSkill_card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			m_Archer_weather = 0;
		}
		if (usingSkill_card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			m_Siege_weather = 0;
		}
		//将一张卡牌移至对方墓地
		if (cardExist == true)
		{
			i = 0;
			foreach(CardsUI *card, cardUILists)
			{
				if (card->operating_card->No ==
					selected_card->operating_card->No)
				{
					cardUILists[i]->operating_card->isFriend = false;
					my_cardStackNo.removeOne(cardUILists[i]->operating_card->No);
					usingSkillTimes++;
					break;
				}
				i++;
			}
		}
		else
		{
			usingSkillTimes++;
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = false;
			useMainScene = true;
			view->setScene(main_scene);
			putInEnemyText();
		}
		break;

	case 7://盖尔
		//将选中的牌插入牌组中
		temp_card = new CardsUI(selected_card->operating_card->No);
		pos = QPointF(COLUMN_POS_X, PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
		i = cardUILists.indexOf(temp_card);
		cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		cardUILists[i]->using_background = 4;
		//用于卡牌的点击
		connect(cardUILists[i], SIGNAL(cardIsPressed()),
			this, SLOT(CardisPressed()));
		connect(cardUILists[i], SIGNAL(cardIsReleased()),
			this, SLOT(CardisReleased()));
		//向场景中添加部件
		main_scene->addItem(cardUILists[i]);
		cardUISizeAdjust();

		//将卡牌添加到牌场上
		cardUILists[i]->operating_card->isFielded = true;
		switch (cardUILists[i]->operating_card->genre)
		{
		case 0:
			cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_MELEE_COLUMN_POS_Y);
			break;
		case 1:
			cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_ARCHER_COLUMN_POS_Y);
			break;
		case 2:
			cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);
			break;
		case 3:
			cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_ARCHER_COLUMN_POS_Y);
			break;
		case 4:
			cardUILists[i]->setPos(cardUILists[i]->pos().x(), E_ARCHER_COLUMN_POS_Y);
			break;
		case 5:
			cardUILists[i]->operating_card->isGarbaged = true;
			cardUILists[i]->operating_card->isFielded = false;
			break;
		}
		updateStatus();
		my_cardStackNo.append(cardUILists[i]->operating_card->No);
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = true;
			useMainScene = true;
			usingSkill_card = cardUILists[i];
			view->setScene(main_scene);
			CardisReleased();
		}
		break;

	case 8://杰洛特：伊格尼法印
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == false)
		{
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 9://卡兰希尔
		//存在满足条件的卡牌
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == false)
		{
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->operating_card->No == selected_card->operating_card->No)
				{
					break;
				}
				i++;
			}
			if (usingSkill_card->pos().y() == M_MELEE_COLUMN_POS_Y)
			{
				cardUILists[i]->setPos(cardUILists[i]->pos().x(), E_MELEE_COLUMN_POS_Y);
				e_Melee_weather = 2;
			}
			else if (usingSkill_card->pos().y() == M_ARCHER_COLUMN_POS_Y)
			{
				cardUILists[i]->setPos(cardUILists[i]->pos().x(), E_ARCHER_COLUMN_POS_Y);
				e_Archer_weather = 2;
			}
			else
			{
				cardUILists[i]->setPos(cardUILists[i]->pos().x(), E_SIEGE_COLUMN_POS_Y);
				e_Siege_weather = 2;
			}
			i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			Card temp_card(allCards.Biting_Frost_No);//降下刺骨冰霜
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(temp_card,1));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 10://狂猎骑士
		//存在满足条件的卡牌
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == false &&
			selected_card->operating_card->isWeatherControlled == 2)
		{
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y())
				{
					cardUILists[i]->operating_card->isWeatherControlled = 4;
				}
				i++;
			}
			if (selected_card->pos().y() == E_MELEE_COLUMN_POS_Y)
			{
				e_Melee_weather = 4;
			}
			else if (selected_card->pos().y() == M_ARCHER_COLUMN_POS_Y)
			{
				e_Archer_weather = 4;
			}
			else
			{
				e_Siege_weather = 4;
			}
			usingSkillTimes++;
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 11://老巫妪：呢喃婆
		foreach(CardsUI* card1, allCardUILists)
		{
			if (card1->operating_card->skill == 11 ||
				card1->operating_card->skill == 12 ||
				card1->operating_card->skill == 13)//是老巫妪的牌
			{
				if (my_cardStackNo.contains(card1->operating_card->No))//该牌在手牌中
				{
					i = 0;
					foreach(CardsUI *card2, cardUILists)
					{
						if (card2->operating_card->No ==
							card1->operating_card->No)
						{
							break;
						}
						i++;
					}
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);
				}
				else//创建新牌
				{
					temp_card = new CardsUI(card1->operating_card->No);
					pos = QPointF(COLUMN_POS_X, M_SIEGE_COLUMN_POS_Y);
					temp_card->setPos(pos);
					cardUILists.append(temp_card);
					cardUIPosLists.append(pos);
					cardUIPixmapLists.append(temp_card->pixmap());
					i = cardUILists.indexOf(temp_card);
					cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
					cardUILists[i]->using_background = 4;
					//用于卡牌的点击
					connect(cardUILists[i], SIGNAL(cardIsPressed()),
						this, SLOT(CardisPressed()));
					connect(cardUILists[i], SIGNAL(cardIsReleased()),
						this, SLOT(CardisReleased()));
					//向场景中添加部件
					main_scene->addItem(cardUILists[i]);
					cardUISizeAdjust();

					//将卡牌添加到牌场上
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);

					my_cardStackNo.append(cardUILists[i]->operating_card->No);
				}
			}
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 12://老巫妪：织婆
		foreach(CardsUI* card1, allCardUILists)
		{
			if (card1->operating_card->skill == 11 ||
				card1->operating_card->skill == 12 ||
				card1->operating_card->skill == 13)//是老巫妪的牌
			{
				if (my_cardStackNo.contains(card1->operating_card->No))//该牌在手牌中
				{
					i = 0;
					foreach(CardsUI *card2, cardUILists)
					{
						if (card2->operating_card->No ==
							card1->operating_card->No)
						{
							break;
						}
						i++;
					}
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);
				}
				else//创建新牌
				{
					temp_card = new CardsUI(card1->operating_card->No);
					pos = QPointF(COLUMN_POS_X, M_SIEGE_COLUMN_POS_Y);
					temp_card->setPos(pos);
					cardUILists.append(temp_card);
					cardUIPosLists.append(pos);
					cardUIPixmapLists.append(temp_card->pixmap());
					i = cardUILists.indexOf(temp_card);
					cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
					cardUILists[i]->using_background = 4;
					//用于卡牌的点击
					connect(cardUILists[i], SIGNAL(cardIsPressed()),
						this, SLOT(CardisPressed()));
					connect(cardUILists[i], SIGNAL(cardIsReleased()),
						this, SLOT(CardisReleased()));
					//向场景中添加部件
					main_scene->addItem(cardUILists[i]);
					cardUISizeAdjust();

					//将卡牌添加到牌场上
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);

					my_cardStackNo.append(cardUILists[i]->operating_card->No);
				}
			}
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 13://老巫妪：煮婆
		foreach(CardsUI* card1, allCardUILists)
		{
			if (card1->operating_card->skill == 11 ||
				card1->operating_card->skill == 12 ||
				card1->operating_card->skill == 13)//是老巫妪的牌
			{
				if (my_cardStackNo.contains(card1->operating_card->No))//该牌在手牌中
				{
					i = 0;
					foreach(CardsUI *card2, cardUILists)
					{
						if (card2->operating_card->No ==
							card1->operating_card->No)
						{
							break;
						}
						i++;
					}
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);
				}
				else//创建新牌
				{
					temp_card = new CardsUI(card1->operating_card->No);
					pos = QPointF(COLUMN_POS_X, M_SIEGE_COLUMN_POS_Y);
					temp_card->setPos(pos);
					cardUILists.append(temp_card);
					cardUIPosLists.append(pos);
					cardUIPixmapLists.append(temp_card->pixmap());
					i = cardUILists.indexOf(temp_card);
					cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
					cardUILists[i]->using_background = 4;
					//用于卡牌的点击
					connect(cardUILists[i], SIGNAL(cardIsPressed()),
						this, SLOT(CardisPressed()));
					connect(cardUILists[i], SIGNAL(cardIsReleased()),
						this, SLOT(CardisReleased()));
					//向场景中添加部件
					main_scene->addItem(cardUILists[i]);
					cardUISizeAdjust();

					//将卡牌添加到牌场上
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_SIEGE_COLUMN_POS_Y);

					my_cardStackNo.append(cardUILists[i]->operating_card->No);
				}
			}
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 14://雷霆药水
		if (selected_card->operating_card->isFielded == true)
		{
			int left_x = 0, right_x = COLUMN_LENGTH + COLUMN_POS_X + 1;
			foreach(CardsUI *card, cardUILists)
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)//同一排
				{
					if (card->pos().x() < selected_card->pos().x())
					{
						left_x = max(left_x, card->pos().x());
					}
					if(card->pos().x() > selected_card->pos().x())
					{
						right_x = min(right_x, card->pos().x());
					}
				}
			}
			i = 0;
			foreach(CardsUI *card, cardUILists)//选中相邻卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true&&
					(card->pos().x()==left_x|| card->pos().x()==right_x|| card->pos().x()==selected_card->pos().x()))//相邻卡牌
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 15://林妖
		//生成三只狼
		for (int j = 0; j < 3; j++)
		{
			if (j == 0)
			{
				temp_card = new CardsUI(allCards.Raging_Wolf_No);
			}
			if (j == 1)
			{
				temp_card = new CardsUI(allCards.Raging_Wolf2_No);
			}
			if (j == 2)
			{
				temp_card = new CardsUI(allCards.Raging_Wolf3_No);
			}
			pos = QPointF(COLUMN_POS_X, M_MELEE_COLUMN_POS_Y);
			temp_card->setPos(pos);
			cardUILists.append(temp_card);
			cardUIPosLists.append(pos);
			cardUIPixmapLists.append(temp_card->pixmap());
			i = cardUILists.indexOf(temp_card);
			cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			cardUILists[i]->using_background = 4;
			//用于卡牌的点击
			connect(cardUILists[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(cardUILists[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));
			//向场景中添加部件
			main_scene->addItem(cardUILists[i]);
			cardUISizeAdjust();
			//将卡牌添加到牌场上
			cardUILists[i]->operating_card->isFielded = true;

		}
		//降下蔽日浓雾
		if (usingSkill_card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			e_Melee_weather = 1;
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == E_MELEE_COLUMN_POS_Y &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isWeatherControlled = 1;
				}
				i++;
			}
		}
		if (usingSkill_card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			e_Archer_weather = 1;
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == E_ARCHER_COLUMN_POS_Y &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isWeatherControlled = 1;
				}
				i++;
			}
		}
		if (usingSkill_card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			e_Siege_weather = 1;
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == E_SIEGE_COLUMN_POS_Y &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isWeatherControlled = 1;
				}
				i++;
			}
		}

		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			Card *temp_card = new Card(allCards.Roach_No);
			useSkills(temp_card);
			putInEnemyText();
		}
		break;

	case 16://暴怒的狼，无技能
		break;

	case 17://萝卜
		foreach(CardsUI *card, allCardUILists)
		{
			if(isGoldCardOut==true&&
				card->operating_card->skill==17&&
				!my_cardStackNo.contains(card->operating_card->No))
			{
				temp_card = new CardsUI(card->operating_card->No);
				//将萝卜放在随机行
				srand((unsigned)time(NULL));
				int n = (rand() % 3);
				switch (n)
				{
				case 0:
					pos = QPointF(COLUMN_POS_X, M_MELEE_COLUMN_POS_Y);
					break;
				case 1:
					pos = QPointF(COLUMN_POS_X, M_ARCHER_COLUMN_POS_Y);
					break;
				case 2:
					pos = QPointF(COLUMN_POS_X, M_SIEGE_COLUMN_POS_Y);
					break;
				}
				temp_card->setPos(pos);
				cardUILists.append(temp_card);
				cardUIPosLists.append(pos);
				cardUIPixmapLists.append(temp_card->pixmap());

				i = cardUILists.indexOf(temp_card);
				cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
				cardUILists[i]->using_background = 4;
				//用于卡牌的点击
				connect(cardUILists[i], SIGNAL(cardIsPressed()),
					this, SLOT(CardisPressed()));
				connect(cardUILists[i], SIGNAL(cardIsReleased()),
					this, SLOT(CardisReleased()));
				//向场景中添加部件
				main_scene->addItem(cardUILists[i]);
				cardUISizeAdjust();
				//将卡牌添加到牌场上
				cardUILists[i]->operating_card->isFielded = true;

				my_cardStackNo.append(cardUILists[i]->operating_card->No);
				updateStatus();
				break;
			}
		}
		isGoldCardOut = false;

		break;


	case 19://倾盆大雨
		if (selected_card->operating_card->isFielded == true &&
			selected_card->operating_card->isFriend == false)
		{
			//设置该排天气情况
			if (selected_card->pos().y() == E_MELEE_COLUMN_POS_Y)
			{
				e_Melee_weather = 3;
			}
			else if (selected_card->pos().y() == E_ARCHER_COLUMN_POS_Y)
			{
				e_Archer_weather = 3;
			}
			else if (selected_card->pos().y() == E_SIEGE_COLUMN_POS_Y)
			{
				e_Siege_weather = 3;
			}

			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 20://赛尔伊诺鹰身女妖
		//生成两个鹰身女妖蛋
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				temp_card = new CardsUI(allCards.Harpy_Egg_No);
			}
			else
			{
				temp_card = new CardsUI(allCards.Harpy_Egg2_No);
			}
			pos = QPointF(COLUMN_POS_X, usingSkill_card->pos().y());
			temp_card->setPos(pos);
			cardUILists.insert(cardUILists.indexOf(usingSkill_card) ,temp_card);
			i = cardUILists.indexOf(temp_card);
			cardUIPosLists.insert(i , pos);
			cardUIPixmapLists.insert(i , temp_card->pixmap());
			cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			cardUILists[i]->using_background = 4;
			//用于卡牌的点击
			connect(cardUILists[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(cardUILists[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));
			//向场景中添加部件
			main_scene->addItem(cardUILists[i]);
			cardUISizeAdjust();
			//将卡牌添加到牌场上
			cardUILists[i]->operating_card->isFielded = true;
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 21://撕裂
		if (selected_card->operating_card->isFielded == true)		
		{
			int i = 0;
			foreach(CardsUI *card, cardUILists)//选中同排其他卡牌
			{
				if (card->pos().y() == selected_card->pos().y() &&
					card->operating_card->isFielded == true)
				{
					cardUILists[i]->operating_card->isSelected = true;
				}
				i++;
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 22://土元素
		//生成两个次级土元素
		for (int j = 0; j < 2; j++)
		{
			if (j == 0)
			{
				temp_card = new CardsUI(allCards.Lesser_Earth_Elemental_No);
			}
			if (j == 1)
			{
				temp_card = new CardsUI(allCards.Lesser_Earth_Elemental2_No);
			}
			pos = QPointF(COLUMN_POS_X,M_MELEE_COLUMN_POS_Y);
			temp_card->setPos(pos);
			cardUILists.append(temp_card);
			cardUIPosLists.append(pos);
			cardUIPixmapLists.append(temp_card->pixmap());

			i = cardUILists.indexOf(temp_card);
			cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			cardUILists[i]->using_background = 4;
			//用于卡牌的点击
			connect(cardUILists[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(cardUILists[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));
			//向场景中添加部件
			main_scene->addItem(cardUILists[i]);
			cardUISizeAdjust();
			//将卡牌添加到牌场上
			cardUILists[i]->operating_card->isFielded = true;
		}
		updateStatus();
		break;

	case 23://畏惧者
	//将一张牌添加到手牌
		i = 0;
		foreach(CardsUI *card, allCardUILists)//抽一张卡牌
		{
			if (!my_cardStackNo.contains(card->operating_card->No)&&
				card->operating_card->isHero==false&&
				card->operating_card->No!=usingSkill_card->operating_card->No)
			{
				break;
			}
			i++;
		}
		if (i != allCardUILists.size())
		{
			temp_card = new CardsUI(allCardUILists[i]->operating_card->No);
			pos = QPointF(COLUMN_POS_X+COLUMN_LENGTH, PREPARE_COLUMN_POS_Y);//插到尾部
			temp_card->setPos(pos);
			cardUILists.append(temp_card);
			cardUIPosLists.append(pos);
			cardUIPixmapLists.append(temp_card->pixmap());

			i = cardUILists.indexOf(temp_card);
			cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
			cardUILists[i]->using_background = 4;
			//用于卡牌的点击
			connect(cardUILists[i], SIGNAL(cardIsPressed()),
				this, SLOT(CardisPressed()));
			connect(cardUILists[i], SIGNAL(cardIsReleased()),
				this, SLOT(CardisReleased()));
			//向场景中添加部件
			main_scene->addItem(cardUILists[i]);
			cardUISizeAdjust();
			//将卡牌添加到牌场上
			my_cardStackNo.insert(i, cardUILists[i]->operating_card->No);

		}
		//移动一张牌
		if (cardExist == true)
		{
			i = 0;
			foreach(CardsUI *card, cardUILists)
			{
				if (card->operating_card->No ==
					selected_card->operating_card->No&&
					card->operating_card->isFriend==false&&
					card->operating_card->isFielded==true)
				{
					cardUILists[i]->setPos(cardUILists[i]->pos().x()+10, usingSkill_card->pos().y());
					break;
				}
				i++;
			}
		}
		//设置畏惧者的属性
		i = 0;
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->No == usingSkill_card->operating_card->No&&
				card->operating_card->isFriend == true)
			{
				cardUILists[i]->operating_card->isFielded = true;
				cardUILists[i]->operating_card->isFriend = false;
			}
			i++;
		}
		usingSkillTimes = 1;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = false;
			updateStatus();
			putInEnemyText();
		}
		break;

	case 26://蟹蜘蛛
		foreach(CardsUI* card1, allCardUILists)
		{
			if (card1->operating_card->skill == 26)
			{
				if (my_cardStackNo.contains(card1->operating_card->No))//该牌在手牌中
				{
					i = 0;
					foreach(CardsUI *card2, cardUILists)
					{
						if (card2->operating_card->No ==
							card1->operating_card->No)
						{
							break;
						}
						i++;
					}
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_ARCHER_COLUMN_POS_Y);
				}
				else//创建新牌
				{
					temp_card = new CardsUI(card1->operating_card->No);
					pos = QPointF(COLUMN_POS_X, M_ARCHER_COLUMN_POS_Y);
					temp_card->setPos(pos);
					cardUILists.append(temp_card);
					cardUIPosLists.append(pos);
					cardUIPixmapLists.append(temp_card->pixmap());
					i = cardUILists.indexOf(temp_card);
					cardUILists[i]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
					cardUILists[i]->using_background = 4;
					//用于卡牌的点击
					connect(cardUILists[i], SIGNAL(cardIsPressed()),
						this, SLOT(CardisPressed()));
					connect(cardUILists[i], SIGNAL(cardIsReleased()),
						this, SLOT(CardisReleased()));
					//向场景中添加部件
					main_scene->addItem(cardUILists[i]);
					cardUISizeAdjust();

					//将卡牌添加到牌场上
					cardUILists[i]->operating_card->isFielded = true;
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), M_ARCHER_COLUMN_POS_Y);

					my_cardStackNo.append(cardUILists[i]->operating_card->No);
				}
			}
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;

	case 29://指挥号角
		if (selected_card->operating_card->isFielded == true)
		{
			for (int num = 0; num < 5; num++)
			{
				i = 0;
				foreach(CardsUI *card, cardUILists)
				{
					if (card->pos().y() == selected_card->pos().y() &&
						card->operating_card->isFielded == true&&
						card->pos().x()>=selected_card->pos().x())//选中卡牌右侧五张卡牌
					{
						cardUILists[i]->operating_card->isSelected = true;
					}
					i++;
				}
				if (i >= cardUILists.size())
				{
					break;
				}
			}
			conductor = new PlayingLogic(cardUILists);
			updateStack(conductor->operateCard(*usingSkill_card->operating_card, usingSkill_card->operating_card->number));
			updateStatus();
			delete conductor;
			usingSkillTimes++;
		}

		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			putInEnemyText();
		}
		break;
	}

}