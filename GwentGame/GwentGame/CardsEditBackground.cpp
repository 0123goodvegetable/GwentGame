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
	change_size = false;

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

		card_temp->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
		card_temp->using_background = 3;

		//用于卡牌的点击
		connect(cardUILists.at(i), SIGNAL(cardIsPressed()),
			this, SLOT(isPressed()));
		connect(cardUILists.at(i), SIGNAL(cardIsReleased()),
			this, SLOT(isReleased()));
		//向场景中添加部件
		scene->addItem(card_temp);

		i++;
	}

	//重新计算UI尺寸
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
	 NONSELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.17;//在下方框内未选择的卡牌高度
	 SELECTED_CARD_WIDTH = SCREEN_WIDTH*0.048;//已选择的卡牌宽度
	 SELECTED_CARD_HEIGHT = SCREEN_HEIGHT*0.114;//已选择卡牌高度

	 //卡牌栏x坐标
	 FIGHT_COLUMN_POS_X = SCREEN_WIDTH*0.231;//近战、远程、攻城三栏的x坐标
	 FUNCTION_COLUMN_POS_X = SCREEN_WIDTH*0.248;//功能牌栏x坐标
	 BASIS_COLUMN_POS_X = SCREEN_WIDTH*0.222;//未被选择卡牌栏x坐标


	
	 BASIS_COLUMN_POS_Y = (SCREEN_HEIGHT*0.72)+geometry().y();//未被选择卡牌栏y坐标
	 MELEE_COLUMN_POS_Y = SCREEN_HEIGHT*0.1;//近战栏y坐标
	 ARCHER_COLUMN_POS_Y = SCREEN_HEIGHT*0.224;//远程栏y坐标
	 SIEGE_COLUMN_POS_Y = SCREEN_HEIGHT*0.346;//攻城栏y坐标
	 FUNCTION_COLUMN_POS_Y = SCREEN_HEIGHT*0.5;//功能牌栏y坐标


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
		//找到未选择卡组中第一张与最后一张牌,控制卡牌位置
		int j, k;
		for (j = 0; j < cardUIPosLists.size(); j++)
		{
			if (cardUILists[j]->operating_card->isInGameCardsStack == false)
			{
				break;
			}
		}
		for (k = cardUIPosLists.size()-1; k>=0; k--)
		{
			if (cardUILists[k]->operating_card->isInGameCardsStack == false)
			{
				break;
			}
		}
		int min_pos, max_pos;
		min_pos = cardUIPosLists.at(j).x() + pos.x();
		max_pos = cardUIPosLists.at(k).x()
			+ pos.x() + cardUIPixmapLists.at(k).width();
		if (min_pos > BASIS_COLUMN_POS_X || max_pos < BASIS_COLUMN_POS_X+ BASIS_COLUMN_LENGTH)
		{
			return;
		}

		//进行卡牌移动
		else
		{
			foreach(CardsUI* card_temp, cardUILists)
			{
				if(card_temp->operating_card->isInGameCardsStack==false)
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
		qDebug() << "clicked";
}

//槽函数，当scene的selectedItem变化时，发送同名信号到此槽
void CardsEditBackground::selectionChanged()
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
			cardUIPosLists[i].setX((i - j)*NONSELECTED_CARD_WIDTH + pos.x());
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

		QPixmap pixmap = cardUIPixmapLists.at(i);
		pixmap = pixmap.scaled(NONSELECTED_CARD_WIDTH, NONSELECTED_CARD_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
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
