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

#include"CP_AllCards.h"

//定义全局变量
const qreal CARD_POS_Y = 100;
const qreal CARD_DIS = 110;
const qreal CARD_STA = 90;
const int   CARD_SIZE = 50;
const int   CARD_SIZE_M = 20;
const int   SCREEN_SIZE = 800;

CardsSelectionBackground::CardsSelectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	init();
}

CardsSelectionBackground::~CardsSelectionBackground()
{
}

void CardsSelectionBackground::init()
{
	//初始化
	Pressed = false;

	//获取屏幕尺寸
	GetScreenInfo();

	view = new QGraphicsView(this);
	scene = new CardsSelectionScene();
	scene->setSceneRect(0, 0, 800, 480);

	//建立图像移动的信号槽
	connect(scene, SIGNAL(selectionChanged()), 
		this, SLOT(selectionChanged()));    
	connect(scene, SIGNAL(isMoving(QPointF&)), 
		this, SLOT(isMoving(QPointF&))); 

	CardsSelectionUI *temp_card;
	QPointF pos;

	//添加卡牌
	//第一张牌：暗影长者
	temp_card = new CardsSelectionUI(Unseen_Elder_No);
	pos = QPointF(CARD_STA, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//第二张牌：贝克尔的扭曲之镜
	temp_card = new CardsSelectionUI(Bekker_Twister_Mirror_No);
	pos = QPointF(CARD_STA + CARD_DIS * 1, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//第三张牌：蔽日浓雾
	temp_card = new CardsSelectionUI(Impenetrable_Fog_No);
	pos = QPointF(CARD_STA + CARD_DIS * 2, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//第四张牌：刺骨冰霜
	temp_card = new CardsSelectionUI(Biting_Frost_No);
	pos = QPointF(CARD_STA + CARD_DIS * 3, CARD_POS_Y);
	temp_card->setPos(pos);
	cardUILists.append(temp_card);
	cardUIPosLists.append(pos);
	cardUIPixmapLists.append(temp_card->pixmap());

	//重新计算UI尺寸
	cardUISizeAdjust();

	int i = 0;
	foreach(CardsSelectionUI* card_temp, cardUILists)
	{

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

		//用于卡牌的点击
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()), 
			this, SLOT(isPressed()));  
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()), 
			this, SLOT(isReleased()));
		//向场景中添加部件
		scene->addItem(card_temp);

		i++;
	}

	//设置视口view的属性
	view->setScene(scene);
	view->setRenderHints(QPainter::Antialiasing);
	view->setContextMenuPolicy(Qt::NoContextMenu);


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

	
}
