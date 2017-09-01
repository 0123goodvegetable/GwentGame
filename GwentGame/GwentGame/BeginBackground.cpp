#include "BeginBackground.h"

BeginBackground::BeginBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	showLabel = new QLabel(this);

	QFont font;//设置文本字体
	font.setPointSize(16);
	font.setFamily("宋体");
	showLabel->setFont(font);
	showLabel->setStyleSheet("color:white");//设置文本颜色

	showLabel->setText(tr("按下任意键继续..."));

}

BeginBackground::~BeginBackground()
{
}

void BeginBackground::resizeEvent(QResizeEvent*event)
{
	Q_UNUSED(event)//避免编译器警告
	this->setAutoFillBackground(true);//自动填充背景

	//添加背景
	QImage image;
	QPalette palette;
	image.load(":/backgrounds/Resources/backgrounds/begin_background.jpg");//加载背景图片

	palette.setBrush(this->backgroundRole(), 
		QBrush(image.scaled(this->size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));//设置背景画刷
		
	this->setPalette(palette);

	//使文本框自适应
	showLabel->setGeometry(geometry().x() + geometry().width() / 2 - 100, geometry().y() + geometry().height() - 35, 250, 25);
}