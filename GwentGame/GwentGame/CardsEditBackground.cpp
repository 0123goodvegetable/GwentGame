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
#include<QFile>
#include<QTextStream>

#include"CP_AllCards.h"

#define min(x,y) x<y?x:y//最小值
#define max(x,y) x>y?x:y//最大值

//定义全局变量
const qreal CARD_DIS = 150;//卡牌间距
const int CARDS_NUM = 38;//所有卡牌数量

int SCREEN_WIDTH = 1800;//画面宽度
int SCREEN_HEIGHT = 961;//画面高度

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
	selected_card = new CardsUI();
	saveAndQuit_button = new QPushButton(this);

	//设置窗口属性
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//从文件中读取数据
	getFromText();

	//初始化按钮图标
	QIcon saveAndQuit_button_icon;
	saveAndQuit_button_icon.addFile(":/buttons/Resources/buttons/saveAndQuit_button.png");
	saveAndQuit_button->setIcon(saveAndQuit_button_icon);
	saveAndQuit_button->setGeometry(855, 895, 155, 55);
	saveAndQuit_button->setIconSize(QSize(150, 80));
	saveAndQuit_button->setFlat(true);

	//建立图像移动的信号槽
	connect(scene, SIGNAL(selectionChanged()),
		this, SLOT(selectionChanged()));
	connect(scene, SIGNAL(isMoving(QPointF&)),
		this, SLOT(isMoving(QPointF&)));

	//建立添加卡牌的信号槽
	connect(this, SIGNAL(toEdit()), this, SLOT(editStacks()));

	CardsUI *temp_card;
	QPointF pos;
	AllCards all_cards;
	
	//向牌组编辑器中加入所有卡牌
	for (int i = 1; i <= CARDS_NUM; i++)
	{
		temp_card = new CardsUI(all_cards.AllCardsNo[i]);
		pos = QPointF(BASIS_COLUMN_POS_X+(i-1)*NONSELECTED_CARD_WIDTH, BASIS_COLUMN_POS_Y);
		temp_card->setPos(pos);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());

	}

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{
		for (int i = 0; i < cardStackNo.size(); i++)
		{
			if (cardStackNo[i] == card_temp->operating_card->No)
			{
				card_temp->operating_card->isInGameCardsStack = true;
			}
		}

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 3;

		//用于卡牌的点击
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));

		//向场景中添加部件
		if (card_temp->pos().x() >= BASIS_COLUMN_POS_X&&
			card_temp->pos().x() + NONSELECTED_CARD_WIDTH <= BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH)
		{
			scene->addItem(card_temp);
		}
		i++;
	}

	//初始化卡牌图像
	updatePictures();
	cardUISizeAdjust();

	//设置视口view的属性
	view->setScene(scene);
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);


}

void  CardsEditBackground::updateCoordinate()
{
	//卡牌尺寸
	 NONSELECTED_CARD_WIDTH = SCREEN_WIDTH*0.082;//在下方框内未选择的卡牌宽度
	 NONSELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.175;//在下方框内未选择的卡牌高度
	 SELECTED_CARD_WIDTH = SCREEN_WIDTH*0.07;//已选择的卡牌宽度
	 SELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.145;//已选择卡牌高度

	 //卡牌栏x坐标
	 FIGHT_COLUMN_POS_X = SCREEN_WIDTH*0.235;//近战、远程、攻城三栏的x坐标
	 FUNCTION_COLUMN_POS_X = SCREEN_WIDTH*0.253;//功能牌栏x坐标
	 BASIS_COLUMN_POS_X = SCREEN_WIDTH*0.228;//未被选择卡牌栏x坐标


	
	 BASIS_COLUMN_POS_Y = SCREEN_HEIGHT*0.720;//未被选择卡牌栏y坐标
	 MELEE_COLUMN_POS_Y = SCREEN_HEIGHT*0.095;//近战栏y坐标
	 ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT*0.215;//远程栏y坐标
	 SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT*0.335;//攻城栏y坐标
	 FUNCTION_COLUMN_POS_Y = SCREEN_HEIGHT*0.48;//功能牌栏y坐标


	 //卡牌栏长度
	 FIGHT_COLUMN_LENGTH = SCREEN_WIDTH*0.5364;//近战、远程、攻城三栏长度
	 FUNCTION_COLUMN_LENGTH = SCREEN_WIDTH*0.5; //功能牌栏长度
	 BASIS_COLUMN_LENGTH = SCREEN_WIDTH*0.5593;//未被选择卡牌栏长度
}

//槽函数，鼠标拖拽scene时执行
//控制栏中水平移动
void  CardsEditBackground::isMoving(QPointF &pos)
{
	int i = 0;
	if (Pressed)
	{

		//进行卡牌拖拽(首先判断卡牌在哪一栏）
		if (selected_card->operating_card->isInGameCardsStack == false)
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if (card_temp->operating_card->isInGameCardsStack == false)
				{
					card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(),
						cardUIPosLists.at(i).y());
				}
				i++;
			}
		}
		else if(selected_card->operating_card->isInGameCardsStack == true&&
			(selected_card->operating_card->genre==1|| selected_card->operating_card->genre == 3|| selected_card->operating_card->genre == 4))
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if (card_temp->operating_card->isInGameCardsStack == true&&
					(card_temp->operating_card->genre== 1|| card_temp->operating_card->genre == 3|| card_temp->operating_card->genre == 4))
				{
					card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(),
						cardUIPosLists.at(i).y());
				}
				i++;
			}
		}
		else
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if (card_temp->operating_card->isInGameCardsStack == true &&
					card_temp->operating_card->genre == selected_card->operating_card->genre)
				{
					card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(),
						cardUIPosLists.at(i).y());
				}
				i++;
			}
		}
		

		cardUISizeAdjust();
	}
}

//槽函数，当cardUI鼠标按下时执行，设置鼠标按下变量为真
void CardsEditBackground::isPressed()
{
	Pressed = true;
}

//槽函数，当cardUI鼠标释放时执行
//应当设置标志位，让UI图片停止对鼠标拖动事件的响应
void CardsEditBackground::isReleased()
{
	if (isCardUIClicked())
	{
		emit toEdit();
	}
}

//槽函数，当scene的selectedItem变化时，发送同名信号到此槽
void CardsEditBackground::selectionChanged()
{
	int i = 0, j = 0, k = 0, num = 0;

	QList<QGraphicsItem *> items = scene->selectedItems();
	if (items.count() == 1)
	{
		//当前所选择的卡牌的坐标
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

		foreach(CardsUI* card, cardUILists)
		{
			if (card == card_temp)
			{
				selected_card = card;
				break;
			}
			i++;
		}
		j = i;
		i = 0;

		//设置卡牌位置
		foreach(QPointF card_pos, cardUIPosLists)
		{
			Q_UNUSED(card_pos);
			if (i <= j)
			{
				for (k = i, num = 0; k < j; k++)
				{
					if (cardUILists[k]->pos().y()==selected_card->pos().y())
					{
						num++;
					}
				}
				cardUIPosLists[i].setX(-num*NONSELECTED_CARD_WIDTH + pos.x());
			}
			else
			{
				for (k = j, num = 0; k < i; k++)
				{
					if (cardUILists[k]->pos().y() == selected_card->pos().y())
					{
						num++;
					}
				}
				cardUIPosLists[i].setX(num*NONSELECTED_CARD_WIDTH + pos.x());
			}

			i++;
		}
	}
	else
	{
		return;
	}
}

//槽函数，处理鼠标点击事件
void CardsEditBackground::editStacks()
{
	//选定点击的卡牌
	int No = 0;
	QList<QGraphicsItem *> items = scene->selectedItems();
	if (items.count() == 1)
	{
		//当前所选择的卡牌的坐标
		QPointF pos = items.first()->pos();
		CardsUI* card_temp = dynamic_cast<CardsUI*>(items.first());

		foreach(CardsUI* card, cardUILists)
		{
			if (card == card_temp)
				break;
			No++;
		}
	}

	//当点击卡牌在未选择牌组中
	if (cardUILists[No]->operating_card->isInGameCardsStack == false)
	{
		cardUILists[No]->operating_card->isInGameCardsStack = true;//卡牌设置为在游戏卡组中
	}

	//当卡牌在场上游戏卡组中
	else
	{	
		cardUILists[No]->operating_card->isInGameCardsStack = false;//卡牌退回未选择卡组中
	}
	updatePictures();
	cardUISizeAdjust();

	//将信息输入文本文件中
	putInText();
}

void CardsEditBackground::updatePictures()
{
	int num1 = 0, num2 = 0, num3 = 0, num4 = 0, num5 = 0;
	//更新每一个卡牌栏的图像位置
	foreach(CardsUI *card, cardUILists)
	{
		if (card->operating_card->isInGameCardsStack == false)
		{
			card->setPos(BASIS_COLUMN_POS_X + num1*NONSELECTED_CARD_WIDTH, BASIS_COLUMN_POS_Y);
			num1++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 0)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num2*SELECTED_CARD_WIDTH, MELEE_COLUMN_POS_Y);
			num2++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 1)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num3*SELECTED_CARD_WIDTH, ARCHER_COLUMN_POS_Y);
			num3++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 2)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num4*SELECTED_CARD_WIDTH, SIEGE_COLUMN_POS_Y);
			num4++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 3)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num3*SELECTED_CARD_WIDTH, ARCHER_COLUMN_POS_Y);
			num3++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 4)
		{
			card->setPos(FIGHT_COLUMN_POS_X + num3*SELECTED_CARD_WIDTH, ARCHER_COLUMN_POS_Y);
			num3++;
		}
		if (card->operating_card->isInGameCardsStack == true &&
			card->operating_card->genre == 5)
		{
			card->setPos(FUNCTION_COLUMN_POS_X + num5*SELECTED_CARD_WIDTH, FUNCTION_COLUMN_POS_Y);
			num5++;
		}
	}

	int i = 0;
	foreach(CardsUI *card, cardUILists)
	{
		cardUIPosLists[i] = card->pos();
		i++;
	}
}

void CardsEditBackground::putInText()
{
	QFile file("gameCardStack.txt");

	if (file.open(QFile::WriteOnly | QIODevice::Truncate| QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI *card, cardUILists)
		{
			if (card->operating_card->isInGameCardsStack == true)
			{
				outPut << card->operating_card->No;
				outPut << "\n";
			}
		}
	}
	file.close();
}

void CardsEditBackground::getFromText()
{
	QFile file("gameCardStack.txt");

	if (file.open(QFile::ReadOnly))
	{
		QTextStream inPut(&file);
		QString temp_No;
		while (!inPut.atEnd())
		{
			temp_No = inPut.readLine();
			cardStackNo.append(temp_No.toInt());
		}
	}

	file.close();
}

//判断cardUI接收的是否是单击信号。
//判断依据是当前单击的cardUI对象的pos与存储在cardUIPosListsd的位置比较，相等则为单击。
bool CardsEditBackground::isCardUIClicked()
{
	int i = -1;
	QList<QGraphicsItem *> items = scene->selectedItems();

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

void CardsEditBackground::cardUISizeAdjust()
{
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{
		QPointF pos = card->pos();

		pos.setX(card->pos().x() + card->pixmap().width());
		pos.setY(card->pos().y());


		//向场景中添加卡牌
		if (card->operating_card->isInGameCardsStack == false)
		{
			if (card->pos().x() >= BASIS_COLUMN_POS_X  &&
				card->pos().x() + NONSELECTED_CARD_WIDTH <= BASIS_COLUMN_POS_X + BASIS_COLUMN_LENGTH )
			{
				scene->addItem(card);
			}
			else
			{
				scene->removeItem(card);
			}
		}
		else
		{
			if (card->pos().x() >= FIGHT_COLUMN_POS_X  &&
				card->pos().x() + SELECTED_CARD_WIDTH <= FIGHT_COLUMN_POS_X + FIGHT_COLUMN_LENGTH )
			{
				scene->addItem(card);
			}
			else
			{
				scene->removeItem(card);
			}

		}


		QPixmap pixmap = cardUIPixmapLists.at(i);
		if (card->operating_card->isInGameCardsStack == false)
		{
			pixmap = pixmap.scaled(NONSELECTED_CARD_WIDTH, NONSELECTED_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		}
		else
		{
			pixmap = pixmap.scaled(SELECTED_CARD_WIDTH, SELECTED_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		}
		cardUILists[i]->setPixmap(pixmap);
		i++;
	}
}

void CardsEditBackground::resizeEvent(QResizeEvent *event)
{

	//添加背景
	QPixmap background(":/backgrounds/Resources/backgrounds/cardsEdit_background.jpg");//加载背景图片
	QPalette palette;
	view->setGeometry(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());//设置视口尺寸与窗口一样大
	palette.setBrush(view->backgroundRole(),
		QBrush(background.scaled(view->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//设置背景画刷
	view->setPalette(palette);

	//调整画面的大小
	scene->setSceneRect(geometry().x(), geometry().y(),
		geometry().width(), geometry().height());

	//获取窗口宽度
	SCREEN_WIDTH = this->width();
	SCREEN_HEIGHT = this->height();
	updateCoordinate();


}
