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
const qreal CARD_POS_Y = 150;
const qreal CARD_DIS = 300;
const qreal CARD_STA = 100;
const int   CARD_SIZE = 200;
int   SCREEN_SIZE = 1800;

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
		if (min_pos < 0||max_pos > SCREEN_SIZE)
		{
			return;
		}
		else
		{
			foreach(CardsSelectionUI* card_temp, cardUILists)
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
	if (isCardUIClicked())
		qDebug() << "clicked";
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
		CardsSelectionUI* card_temp = dynamic_cast<CardsSelectionUI*>(items.first());

		foreach(CardsSelectionUI* card, cardUILists)
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
		CardsSelectionUI* card_temp = dynamic_cast<CardsSelectionUI*>(items.first());
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
	foreach(CardsSelectionUI* card, cardUILists)
	{
		QPointF pos = card->pos();

		pos.setX(card->pos().x()+card->pixmap().width());

		//quint16 width = 400;
		//quint16 height = 400;
		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(350, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		cardUILists[i]->setPixmap(pixmap);

		//改变两端卡牌大小
		/*if (pos.x() >= 0 && pos.x() <= SCREEN_SIZE / 2)
		{
			
			quint16 size = pos.x() / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}

		if (pos.x()>SCREEN_SIZE / 2 && pos.x() <= SCREEN_SIZE + 10)
		{
			quint16 size = (SCREEN_SIZE - pos.x()) / 5 + 200;
			QPixmap pixmap = cardUIPixmapLists.at(i);
			pixmap = pixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			cardUILists[i]->setPixmap(pixmap);
		}*/
		i++;
	}
}

//获取设备分辨率信息
void CardsSelectionBackground::GetScreenInfo()
{
	QDesktopWidget* desktopWidget = QApplication::desktop();
	//获取可用桌面大小
	//QRect deskRect = desktopWidget->availableGeometry();
	//获取设备屏幕大小
	QRect screenRect = desktopWidget->screenGeometry();

	sceenSizeX = screenRect.width();
	sceenSizeY = screenRect.height();

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
