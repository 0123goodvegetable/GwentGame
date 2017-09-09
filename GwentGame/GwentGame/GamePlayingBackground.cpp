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
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0, num6 = 0, num7 = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		if (card_temp->operating_card->isGarbaged == true)//卡牌进入坟墓
		{
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
			num2++;
		}
		else if (card_temp->pos().y() == M_ARCHER_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num3*CARD_DIS, M_ARCHER_COLUMN_POS_Y);
			num3++;
		}
		else if (card_temp->pos().y() == M_SIEGE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num4*CARD_DIS, M_SIEGE_COLUMN_POS_Y);
			num4++;
		}
		else if (card_temp->pos().y() == E_MELEE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num5*CARD_DIS, E_MELEE_COLUMN_POS_Y);
			num5++;
		}
		else if (card_temp->pos().y() == E_ARCHER_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num6*CARD_DIS, E_ARCHER_COLUMN_POS_Y);
			num6++;
		}
		else if (card_temp->pos().y() == E_SIEGE_COLUMN_POS_Y)
		{
			card_temp->setPos(COLUMN_POS_X + num7*CARD_DIS, E_SIEGE_COLUMN_POS_Y);
			num7++;
		}
	}

	int i = 0;
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

			usingSkill_card = selected_card;
			isUsingSkill = true;

			switch (usingSkill_card->operating_card->ID)
			{
			case 2://贝克尔扭曲之镜,不需要选择对象
				emit toUseSkills(usingSkill_card->operating_card);//使用技能
				break;
			case 5://达冈,不需要选择对象
				useChooseScene(usingSkill_card);//选取要使用的天气牌
				break;
			case 6://大狮鹫
				useChooseScene(usingSkill_card);//选取要置入对方墓地的卡牌
				break;
			case 7://盖尔
				useChooseScene(usingSkill_card);//选取一张金卡或银卡
				break;
			}

			updateStatus();
		}
		else if (operation == false && isUsingSkill == true)
		{

			if (usingSkill_card->operating_card->ID == 1 &&
				usingSkillTimes<UNSEEN_ELDER_SKILL_TIMES)//使用两次技能。。写的方法比较麻烦
			{
				emit toUseSkills(usingSkill_card->operating_card);//使用技能
			}
			else
			{
				emit toUseSkills(usingSkill_card->operating_card);//使用技能
			}
		}

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

			if (operation == false && isUsingSkill == false)
			{
				selected_card = cardUILists[i];//选取操作卡牌
				operation = true;
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
	switch (root_card->operating_card->ID)
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
			weather_card[i]->using_background = 4;

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
			garbaged_card[i]->using_background = 4;
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
			two_cards[i]->using_background = 4;

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


//槽函数，用于卡牌施放技能
void GamePlayingBackground::useSkills(Card *card)
{
	int i;
	bool cardExist = false;///存在目标卡牌
	switch (card->skill)
	{
	case 1://暗影长者
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isFriend ==true && 
				//card->operating_card->isFielded == true &&
				card->operating_card->ID != 1)
			{
				cardExist = true;
				break;
			}
		}
		//存在满足条件的卡牌
		if (cardExist)
		{
			if (//selected_card->operating_card->isFielded == true &&
				selected_card->operating_card->isFriend == true &&
				selected_card->operating_card->ID != 1)//选取非自身的友军牌
			{
				int num = cardUILists.indexOf(selected_card);
				cardUILists[num]->operating_card->isSelected = true;
				Card card(allCards.Unseen_Elder_No);
				conductor = new PlayingLogic(cardUILists);
				updateStack(conductor->operateCard(card));
				updateStatus();
				delete conductor;
				usingSkillTimes++;
			}
		}
		//还原
		if (usingSkillTimes == UNSEEN_ELDER_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			cardExist = false;
		}
		break;

	case 2://贝克尔扭曲之镜
		conductor = new PlayingLogic(cardUILists);
		updateStack(conductor->operateCard(*usingSkill_card->operating_card));
		updateStatus();
		delete conductor;
		//还原
		setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
		usingSkillTimes = 0;
		operation = false;
		isUsingSkill = false;
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
		//存在满足条件的卡牌
		if (cardExist)
		{
			if (selected_card->operating_card->isFielded == true&&
				selected_card->operating_card->isFriend == false )
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
				updateStack(conductor->operateCard(*usingSkill_card->operating_card));
				updateStatus();
				delete conductor;
				usingSkillTimes++;
			}
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			cardExist = false;
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
		//存在满足条件的卡牌
		if (cardExist)
		{
			if (selected_card->operating_card->isFielded == true&&
				selected_card->operating_card->isFriend == true )
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
				updateStack(conductor->operateCard(*usingSkill_card->operating_card));
				updateStatus();
				delete conductor;
				usingSkillTimes++;
			}
		}
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			setCursor(QCursor(Qt::ArrowCursor));//恢复原光标
			usingSkillTimes = 0;
			operation = false;
			isUsingSkill = false;
			cardExist = false;
		}
		break;

	case 5://达冈
		//恢复原设定
		if (selected_card->operating_card->ID != usingSkill_card->operating_card->ID)
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
		updateStack(conductor->operateCard(*usingSkill_card->operating_card));
		updateStatus();
		delete conductor;
		//将一张卡牌移至对方墓地
		i = 0;
		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->ID==
				selected_card->operating_card->ID)
			{
				cardUILists[i]->operating_card->isFriend = false;
				usingSkillTimes++;
				break;
			}
			i++;
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
		CardsUI* temp_card = new CardsUI(selected_card->operating_card->No);
		QPointF pos = QPointF(COLUMN_POS_X , PREPARE_COLUMN_POS_Y);
		temp_card->setPos(pos);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
		int No = cardUILists.indexOf(temp_card);
		cardUILists[No]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		cardUILists[No]->using_background = 4;
		//用于卡牌的点击
		connect(cardUILists[No], SIGNAL(cardIsPressed()),
			this, SLOT(CardisPressed()));
		connect(cardUILists[No], SIGNAL(cardIsReleased()),
			this, SLOT(CardisReleased()));
		//向场景中添加部件
		main_scene->addItem(cardUILists[No]);
		cardUISizeAdjust();

		//将卡牌添加到牌场上
		cardUILists[No]->operating_card->isFielded = true;
		switch (cardUILists[No]->operating_card->genre)
		{
		case 0:
			cardUILists[No]->setPos(cardUILists[No]->pos().x(), M_MELEE_COLUMN_POS_Y);
			break;
		case 1:
			cardUILists[No]->setPos(cardUILists[No]->pos().x(), M_ARCHER_COLUMN_POS_Y);
			break;
		case 2:
			cardUILists[No]->setPos(cardUILists[No]->pos().x(), M_SIEGE_COLUMN_POS_Y);
			break;
		case 3:
			cardUILists[No]->setPos(cardUILists[No]->pos().x(), M_ARCHER_COLUMN_POS_Y);
			break;
		case 4:
			cardUILists[No]->setPos(cardUILists[No]->pos().x(), E_ARCHER_COLUMN_POS_Y);
			break;
		case 5:
			cardUILists[No]->operating_card->isGarbaged = true;
			cardUILists[No]->operating_card->isFielded = false;
			break;
		}
		updateStatus();
		usingSkillTimes++;
		if (usingSkillTimes == NORMAL_SKILL_TIMES)
		{
			isUsingSkill = false;
			usingSkillTimes = 0;
			operation = true;
			useMainScene = true;
			usingSkill_card = cardUILists[No];
			view->setScene(main_scene);
			CardisReleased();
		}
	}

}