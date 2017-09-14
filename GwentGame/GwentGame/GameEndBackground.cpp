#include "GameEndBackground.h"

GameEndBackground::GameEndBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	init();
}

GameEndBackground::~GameEndBackground()
{
}

void GameEndBackground::init()
{
	WinOrLoseLabel = new QLabel(this);
	my_Final1 = new QLabel(this);
	my_Final2 = new QLabel(this);
	my_Final3 = new QLabel(this);
	enemy_Final1 = new QLabel(this);
	enemy_Final2 = new QLabel(this);
	enemy_Final3 = new QLabel(this);
	close_button = new QPushButton(this);

	//���ð�ťͼ��
	QIcon close_button_icon;
	close_button_icon.addFile(":/buttons/Resources/buttons/close_button.png");
	close_button->setIcon(close_button_icon);
	close_button->setGeometry(855, 880, 90, 45);
	close_button->setIconSize(QSize(100, 40));
	close_button->setFlat(true);


}

void GameEndBackground::updateScore()
{
	//�����ı� 
	QFont font;
	QPalette palette;
	font.setPixelSize(40); // ���ش�С 
	palette.setColor(QPalette::WindowText, Qt::white);//��ɫΪ��ɫ

	my_Final1->setText(QString::number(my_final[0]));
	my_Final1->setFont(font);
	my_Final1->setPalette(palette);
	my_Final1->setGeometry(800, 400, 100, 50);

	my_Final2->setText(QString::number(my_final[1]));
	my_Final2->setFont(font);
	my_Final2->setPalette(palette);
	my_Final2->setGeometry(800, 520, 100, 50);

	my_Final3->setText(QString::number(my_final[2]));
	my_Final3->setFont(font);
	my_Final3->setPalette(palette);
	my_Final3->setGeometry(800, 630, 100, 50);

	enemy_Final1->setText(QString::number(enemy_final[0]));
	enemy_Final1->setFont(font);
	enemy_Final1->setPalette(palette);
	enemy_Final1->setGeometry(950, 400, 100, 50);

	enemy_Final2->setText(QString::number(enemy_final[1]));
	enemy_Final2->setFont(font);
	enemy_Final2->setPalette(palette);
	enemy_Final2->setGeometry(950, 520, 100, 50);

	enemy_Final3->setText(QString::number(enemy_final[2]));
	enemy_Final3->setFont(font);
	enemy_Final3->setPalette(palette);
	enemy_Final3->setGeometry(950, 630, 100, 50);

	WinOrLoseLabel->setText(WinOrLose);
	WinOrLoseLabel->setFont(font);
	WinOrLoseLabel->setPalette(palette);
	WinOrLoseLabel->setGeometry(850, 100, 400, 50);
}

void GameEndBackground::resizeEvent(QResizeEvent*event)
{
	Q_UNUSED(event)//�������������

	this->setAutoFillBackground(true);//�Զ���䱳��

	//��ӱ���
	QImage image;
	QPalette palette;
	image.load(":/backgrounds/Resources/backgrounds/gameEnd_background.jpg");//���ر���ͼƬ

	palette.setBrush(this->backgroundRole(),
		QBrush(image.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//���ñ�����ˢ

	this->setPalette(palette);

}

//�ۺ�������������
void GameEndBackground::receiveFinal(int m_final1, int m_final2, int m_final3, int e_final1, int e_final2, int e_final3)
{
	my_final[0] = m_final1;
	my_final[1] = m_final2;
	my_final[2] = m_final3;
	enemy_final[0] = e_final1;
	enemy_final[1] = e_final2;
	enemy_final[2] = e_final3;

	int number = 0;
	for (int i = 0; i < 3; i++)
	{
		if (my_final[i] > enemy_final[i])
		{
			number++;
		}
		else if (my_final[i] < enemy_final[i])
		{
			number--;
		}
	}
	if (number > 0)
	{
		WinOrLose = "You Win!";
	}
	else if (number < 0)
	{
		WinOrLose = "You Lose!";
	}
	else
	{
		WinOrLose = "Score is Tied!";
	}

	updateScore();
}
