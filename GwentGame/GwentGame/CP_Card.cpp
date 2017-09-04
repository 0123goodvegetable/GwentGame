#include"CP_Card.h"

Card::Card():
	ID(-1),
	attack(0)
{
}

Card::Card(int num)
{
	Init(num);
}

void Card::Init(int num)
{
	//��¼����
	AddName(num);

	//��¼�����ļ�·��
	path = ":/cards/" + name;

	//��ӿ���ͼƬ
	//AddPicture();

	//��¼����ID
	ID = num / 10000000;
	num %= 10000000;

	//����������Ϣ
	CreateLogic(num);

	//��ʼ�����Ƴ�������
	card_width = 300;
	card_height = 400;

}

void Card::CreateLogic(int num)
{
	/*
	��7λ��Ч����(abcdefg)����¼������Ϣ��9λ����ǰ��λΪID������ţ�
	��һλ���֣�a����¼�������ࣨ0-��ս��1-Զ�̣�2-���ǣ�3-�ҷ����⣬4-�з����⣬5-�����ƣ�
	����0<=a<=4��ʱ
	�ڶ�����λ���֣�bc����¼���Ƽ���
	01-��������1���Ѿ���λ���������ǵ�ս��Ϊ����Ļ���ս�����ظ����Ρ�����Ӱ���ߣ�
	02-�������ɡ��̹Ǳ�˪����������Ũ����������ꡱ������ԣ�
	03-�����Ƴ������ŵ�����Ч������1��ͭɫ�ƴ�һ��Ĺ��������һ��Ĺ��������ʨ�գ�
	���ġ���λ���֣�de����¼���ƹ�����
	����λ���֣�f����¼�����Ƿ�ΪӢ�ۣ�f<5ʱ��Ӣ�ۣ�

	����a=3��ʱ
	�ڶ�����λ���֣�bc����¼����������
	01-�Գ�����ǿ�ĵ�λ�����ֵ��ͬ����ս�����˺�������಻����10�㣨���ӻ��ף�����ʹ���������ķ�ͬ����λ�����ͬ��ֵ�����档�����˶�Ť��֮����
	02-�ڶԷ����Ž��¡�����Ũ��������Ũ��ÿ�������غϿ�ʼʱ������������ǿ�ĵ�λ���2���˺���������Ũ��
	03-�ڶԷ����Ž��¡��̹Ǳ�˪�����̹Ǳ�˪��ÿ�������غϿ�ʼʱ���������������ĵ�λ���2���˺������̹Ǳ�˪��

	����λ���֣�g����¼���Ʋ��ʣ�0-��1-����2-ͭ��
	*/

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;
	number = 1;

	int temp_num = 0;

	No = num;//���Ʊ��

	genre = num / 1000000;//��������

	temp_num = num / 10000;
	skill = temp_num % 100;//���Ƽ���

	temp_num = num / 100;
	attack = num % 100;//����ս����

	temp_num = (num / 10) % 10;
	if (temp_num < 5)
	{
		isHero = true;
	}//�Ƿ�ΪӢ����

	material = num % 10;//���Ʋ���
}

void  Card::AddName(int num)
{
	switch (num)
	{
	case 13010530:
		name = "Unseen_Elder";
		break;
	case 25010091:
		name = "Bekker_Twister_Mirror";
		break;
	case 35020092:
		name = "Impenetrable_Fog";
		break;
	case 45030092:
		name = "Biting_Frost";
		break;
	case 53020630:
		name = "Dagon";
		break;
	case 63030882:
		name = "Archgriffin";
		break;
	}
}

/*void Card::AddPicture()
{
	QPixmap pixmap;
	pixmap.load(path);
	pixmap = pixmap.scaled(card_width, card_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	setPixmap(pixmap);
}*/

QRectF Card::boundingRect() const
{

	return QRectF(0, 0, card_width, card_height);
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPixmap pixmap(path);

	QRect rect = pixmap.rect();
	painter->drawPixmap(rect, pixmap);
}