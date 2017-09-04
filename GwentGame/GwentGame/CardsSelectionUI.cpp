#include"CardsSelectionUI.h"
#include <QPixmap>
#include <iostream>
#include <QDebug>

CardsSelectionUI::CardsSelectionUI()
{
}

CardsSelectionUI::CardsSelectionUI(const Card aim_card)
{
	operating_card = new Card(aim_card.No);

	QPixmap pixmap;
	pixmap.load(operating_card->path);
	pixmap = pixmap.scaled(operating_card->card_width, operating_card->card_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);
}

QRectF CardsSelectionUI::boundingRect() const
{
	QRect rect = this->pixmap().rect();
	//return QRectF(rect);
	return QRectF(0, 0, rect.width(), rect.width() + 15);
}

void CardsSelectionUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	QPixmap pixmap = this->pixmap();
	QRect rect = pixmap.rect();

	painter->drawPixmap(rect, pixmap);


	//print name,calculate the text's heigh & width for center layout
	QPen pen(Qt::black);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::Antialiasing);
	QFont font("Verdana", 8, QFont::Normal);
	painter->setFont(font);
	painter->drawText(QRectF(0, rect.height(), rect.width(), 15), Qt::AlignCenter, operating_card->name);

	if (option->state & QStyle::State_Sunken)
	{
		QRectF rect1 = boundingRect();
		painter->setPen(QPen(Qt::darkGreen));
	}
	else
	{

	}
}

QPainterPath  CardsSelectionUI::shape() const
{
	QRectF rect = boundingRect();

	QPainterPath path;
	path.addRoundRect(rect, 5, 5);
	return path;
}

void  CardsSelectionUI::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit cardIsPressed();
	QGraphicsItem::mousePressEvent(event);
}
void  CardsSelectionUI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update(boundingRect());
	emit cardIsReleased();  
	QGraphicsItem::mouseReleaseEvent(event);
}

void  CardsSelectionUI::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF point = event->pos();
}

