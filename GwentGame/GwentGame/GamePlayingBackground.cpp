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

GamePlayingBackground::GamePlayingBackground(QWidget *parent)
	: QWidget(parent)
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

	CardsUI *temp_card;
	QPointF pos;
	//完善卡牌信息
	for (int i = 0; i < my_cardStackNo.size(); i++)
	{
		temp_card = new CardsUI(my_cardStackNo.at(i));
		pos = QPointF(COLUMN_POS_X + CARD_DIS * i, PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);

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

	//设置视口view的属性
	view->setScene(main_scene);//初始化为游戏主界面
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);

}

void GamePlayingBackground::updateCoordinate()
{

	CARD_HEIGHT = SCREEN_HEIGHT2*0.123;
	CARD_WIDTH = SCREEN_WIDTH2*0.06;

	COLUMN_POS_X = 0.3*SCREEN_WIDTH2;

	PREPARE_COLUMN_POS_Y = SCREEN_HEIGHT2*0.87;

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
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, num7 = 0, i = 0;
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
			card_temp->setPos(COLUMN_POS_X + num1*CARD_DIS, PREPARE_COLUMN_POS_Y);
			num1++;
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
	int num = cardUILists.indexOf(selected_card);
	int pos_x, pos_y;
	pos_x = cardUIPosLists[num].x() + pos.x();
	pos_y = cardUIPosLists[num].y() + pos.y();


	if (Pressed == true && operation == true &&
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

//槽函数，当cardUI鼠标按下时执行，设置鼠标按下变量为真
void GamePlayingBackground::CardisPressed()
{
	Pressed = true;
}

//槽函数，当cardUI鼠标释放时执行
//若此时在操作一张卡牌，则释放技能
void GamePlayingBackground::CardisReleased()
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
		
		whetherUseFollowSkill();
		view->viewport()->update();//立即重绘

}

//槽函数，当scene的selectedItem变化时，发送同名信号到此槽
void GamePlayingBackground::selectionChanged()
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

			if (operation == false && isUsingSkill == false&&
				cardUILists[i]->operating_card->isFielded==false)
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
	else if(useMainScene==false)
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

		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(CARD_WIDTH, CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);
		i++;
	}
}

void GamePlayingBackground::getFromText()
{
	QFile file1("playingCardStack.txt");

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
					 !my_cardStackNo.contains(all_cardStackNo[j]))//是金卡或银卡且不在现有手牌中
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
	SCREEN_WIDTH2 = this->width();
	SCREEN_HEIGHT2 = this->height();
	updateCoordinate();

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
				!my_cardStackNo.contains(card->operating_card->No))//不在手牌中的金银卡
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
		break;
	case 17://萝卜，无主动技能
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
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
		}
		break;
	case 22://土元素，无主动技能
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
		break;
	case 23://畏惧者
		num = 0;
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->No == usingSkill_card->operating_card->No)
			{
				cardUILists[num]->operating_card->isFriend = false;
			}
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
		}
		else
		{
			emit toUseSkills(usingSkill_card->operating_card);
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
	i = 0;
	foreach(CardsUI *card, cardUILists)
	{
		if (card->operating_card->skill == 22 &&
			card->operating_card->isFriend == true &&
			card->operating_card->isGarbaged==true&&
			card->operating_card->isUseFollowSkill == false)
		{
			emit toUseSkills(card->operating_card);
			cardUILists[i]->operating_card->isUseFollowSkill = true;
			break;
		}
		i++;
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
		}
		break;

	case 15://林妖
		//生成三只狼
		for (int j = 0; j < 3; j++)
		{
			temp_card = new CardsUI(allCards.Raging_Wolf_No);
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

			my_cardStackNo.append(cardUILists[i]->operating_card->No);
		}
		//降下蔽日浓雾
		if (usingSkill_card->pos().y() == M_MELEE_COLUMN_POS_Y)
		{
			e_Melee_weather = 1;
		}
		if (usingSkill_card->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			e_Archer_weather = 1;
		}
		if (usingSkill_card->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			e_Siege_weather = 1;
		}

		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
		}
		break;

	case 16://暴怒的狼，无技能
		break;

	case 17://萝卜
		foreach(CardsUI *card, allCardUILists)
		{
			if(card->operating_card->skill==17&&
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
		}
		break;

	case 20://赛尔伊诺鹰身女妖
		//生成两个鹰身女妖蛋
		for (int j = 0; j < 2; j++)
		{
			temp_card = new CardsUI(allCards.Harpy_Egg_No);
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
			my_cardStackNo.insert(i,cardUILists[i]->operating_card->No);
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
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
		}
		break;

	case 22://土元素
		//生成两个次级土元素
		for (int j = 0; j < 2; j++)
		{
			temp_card = new CardsUI(allCards.Lesser_Earth_Elemental_No);
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
			my_cardStackNo.insert(i, cardUILists[i]->operating_card->No);
		}
		updateStatus();
		break;

	case 23://畏惧者
	//将一张牌添加到手牌
		i = 0;
		foreach(CardsUI *card, allCardUILists)//抽一张卡牌
		{
			if (!my_cardStackNo.contains(card->operating_card->No))
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

			updateStatus();
		}
		//移动一张牌
		if (cardExist == true)
		{
			i = 0;
			foreach(CardsUI *card, cardUILists)
			{
				if (card->operating_card->No ==
					selected_card->operating_card->No)
				{
					cardUILists[i]->setPos(cardUILists[i]->pos().x(), usingSkill_card->pos().y());
					break;
				}
				i++;
			}
		}
		usingSkillTimes = 1;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = false;
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
		}
		break;
	}

}