#include "CardsSelectionBackground.h"
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
#include<stdlib.h>
#include<time.h>


#include"CP_AllCards.h"

//定义全局变量
const qreal CARD_POS_Y = 250;//卡牌y坐标（保持水平所以为常量）
const qreal CARD_DIS = 300;//卡牌间距
const qreal CARD_STA = 100;//第一张卡牌开始时的位置
int   SCREEN_SIZE = 2000;//窗口宽度

CardsSelectionBackground::CardsSelectionBackground(int turn,QWidget *parent)
	: QWidget(parent),
	my_turn(turn)
{
	ui.setupUi(this);

	//初始化窗口
	init();

	
}

CardsSelectionBackground::~CardsSelectionBackground()
{
}

void CardsSelectionBackground::init()
{
	//初始化
	Pressed = false;

	view = new QGraphicsView(this);
	scene = new CardsScene();
	cardsSelectionFinished_button = new QPushButton(this);
	textLabel = new QLabel(this);
	cardsToAdjust_number = 3;

	if (my_turn == 1)
	{
		cardsToAdjust_number = 3;
	}
	if (my_turn == 2)
	{
		cardsToAdjust_number = 2;
	}
	if (my_turn == 3)
	{
		cardsToAdjust_number = 1;
	}

	//初始化按钮图标
	QIcon cardsSelectionFinished_button_icon;
	cardsSelectionFinished_button_icon.addFile(":/buttons/Resources/buttons/cardsSelectionFinished_button.png");
	cardsSelectionFinished_button->setIcon(cardsSelectionFinished_button_icon);
	cardsSelectionFinished_button->setGeometry(855, 895, 155, 55);
	cardsSelectionFinished_button->setIconSize(QSize(150, 80));
	cardsSelectionFinished_button->setFlat(true);

	//从文本中获取牌组
	getFromText();

	//设置窗口属性
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//建立图像移动的信号槽
	connect(scene, SIGNAL(selectionChanged()), 
		this, SLOT(selectionChanged()));    
	connect(scene, SIGNAL(isMoving(QPointF&)), 
		this, SLOT(isMoving(QPointF&))); 

	//建立点击更换卡牌的信号槽
	connect(this, SIGNAL(toChange()), this, SLOT(changeCard()));

	CardsUI *temp_card;
	QPointF pos;
	AllCards all_cards;

	//随机添加卡牌
	for (int i = 0; i < 10; i++)
	{
		srand((unsigned)time(NULL));
		int n = (rand() % cardStackNo.size());

		temp_card = new CardsUI(cardStackNo.at(n));
		pos = QPointF(CARD_STA + CARD_DIS * i, CARD_POS_Y);
		temp_card->setPos(pos);
		cardUILists.append(temp_card);
		cardUIPosLists.append(pos);
		cardUIPixmapLists.append(temp_card->pixmap());
		cardStackNo.removeAt(n);
	}

	putInText();

	int i = 0;
	foreach(CardsUI* card_temp, cardUILists)
	{

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 2;

		//用于卡牌的点击
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()), 
			this, SLOT(isPressed()));  
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()), 
			this, SLOT(isReleased()));
		//向场景中添加部件
		scene->addItem(card_temp);

		i++;
	}

	//添加文本 
	textLabel->setText(tr("remain times to use: %1/3 ").arg(cardsToAdjust_number));
	QFont font;
	font.setPixelSize(30); // 像素大小 
	textLabel->setFont(font);
	QPalette palette;
	palette.setColor(QPalette::WindowText, Qt::white);
	textLabel->setPalette(palette);
	textLabel->setGeometry(750, 110, 400, 50);

	//重新计算UI尺寸
	cardUISizeAdjust();

	//设置视口view的属性
	view->setScene(scene);
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);


}

//槽函数，鼠标拖拽scene时执行
//控制UI水平
void  CardsSelectionBackground::isMoving(QPointF &pos)
{
	int i = 0;

	int min_pos, max_pos;
	min_pos = cardUIPosLists.at(0).x() + pos.x();
	max_pos = cardUIPosLists.at(cardUIPosLists.size() - 1).x() + pos.x() + cardUIPixmapLists.at(cardUIPosLists.size() - 1).width();
	if (Pressed)
	{
		if (min_pos > 40||max_pos < SCREEN_SIZE)
		{
			return;
		}
		else
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				card_temp->setPos(cardUIPosLists.at(i).x() + pos.x(), CARD_POS_Y);
				i++;
			}
		}

		cardUISizeAdjust();
	}
}

//槽函数，当cardUI鼠标按下时执行，设置鼠标按下变量为真
void CardsSelectionBackground::isPressed()
{
	Pressed = true;
}

//槽函数，当cardUI鼠标释放时执行
//应当设置标志位，让UI图片停止对鼠标拖动事件的响应
void CardsSelectionBackground::isReleased()
{
	if (isCardUIClicked()&&cardsToAdjust_number>0)
	{
		emit toChange();
	}
}

//槽函数，当scene的selectedItem变化时，发送同名信号到此槽
void CardsSelectionBackground::selectionChanged()
{
	int i = 0, j = 0;

	QList<QGraphicsItem *> items = scene->selectedItems();
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
		j = i;
		i = 0;

		foreach(QPointF card_pos, cardUIPosLists)
		{
			Q_UNUSED(card_pos);
			cardUIPosLists[i].setX((i - j)*CARD_DIS + pos.x());
			cardUIPosLists[i].setY(CARD_POS_Y);
			i++;
		}

	}
	else
	{
		return;
	}
}

//槽函数，当点击卡牌时更换卡牌
void CardsSelectionBackground::changeCard()
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

	if (cardsToAdjust_number > 0)
	{
		CardsUI *temp_card;

		srand((unsigned)time(NULL));
		int n = (rand() % cardStackNo.size());
		temp_card = new CardsUI(cardStackNo.at(n));
		temp_card->setPos(cardUILists[No]->pos());
		cardStackNo.removeAt(n);

		//去掉原来的牌
		scene->removeItem(cardUILists[No]);
		cardUILists.removeAt(No);
		cardUIPosLists.removeAt(No);
		cardUIPixmapLists.removeAt(No);

		//插入新牌
		cardUIPosLists.insert(No, temp_card->pos());
		cardUIPixmapLists.insert(No, temp_card->pixmap());
		cardUILists.insert(No, temp_card);

		putInText();

		//设置新牌的参数
		cardUILists[No]->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		cardUILists[No]->using_background = 2;

		//用于卡牌的点击
		connect(cardUILists[No], SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists[No], SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));

		//将新牌插入场景
		scene->addItem(cardUILists[No]);

		cardsToAdjust_number--;

		textLabel->setText(tr("remain times to use: %1/3 ").arg(cardsToAdjust_number));
	}

	cardUISizeAdjust();

}

//判断cardUI接收的是否是单击信号。
//判断依据是当前单击的cardUI对象的pos与存储在cardUIPosListsd的位置比较，相等则为单击。
//用于更换卡牌
bool CardsSelectionBackground::isCardUIClicked()
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

void CardsSelectionBackground::cardUISizeAdjust()
{
	quint16 i = 0;
	foreach(CardsUI* card, cardUILists)
	{
		QPointF pos = card->pos();

		pos.setX(card->pos().x()+card->pixmap().width());

		//改变两端卡牌大小
		if ( pos.x()<= SCREEN_SIZE / 2)
		{			
			quint16 width = pos.x() / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(width, width*1.1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}

		if (pos.x()> SCREEN_SIZE / 2)
		{
			quint16 width = (SCREEN_SIZE - pos.x()) / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(width, width*1.1, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}
		i++;
	}
}

void CardsSelectionBackground::getFromText()
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

void CardsSelectionBackground::putInText()
{
	QFile file("my_playingCardStack.txt");

	if (file.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		QTextStream outPut(&file);

		foreach(CardsUI* temp_card, cardUILists)
		{
			outPut << temp_card->operating_card->No;
			outPut << "\n";
		}
	}
	file.close();
}

void CardsSelectionBackground::resizeEvent(QResizeEvent *event)
{
	//添加背景
	QPixmap background(":/backgrounds/Resources/backgrounds/cardsSelection_background.jpg");//加载背景图片
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
	SCREEN_SIZE = this->width();
	
}

