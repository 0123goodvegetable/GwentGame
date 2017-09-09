/*
��7λ��Ч����(abcdefg)����¼������Ϣ��9λ����ǰ��λΪID������ţ�
	��һλ���֣�a����¼�������ࣨ0-��ս��1-Զ�̣�2-���ǣ�3-�ҷ����⣬4-�з����⣬5-�����ƣ�
	����0<=a<=4��ʱ
		�ڶ�����λ���֣�bc����¼���Ƽ���
			01-��������1���Ѿ���λ���������ǵ�ս��Ϊ����Ļ���ս�����ظ����Ρ�����Ӱ���ߣ�
			05-�������ɡ��̹Ǳ�˪����������Ũ����������ꡱ������ԣ�
			06-�����Ƴ������ŵ�����Ч������1��ͭɫ�ƴ�һ��Ĺ��������һ��Ĺ��������ʨ�գ�
			07-���𣺴����鶥�˸���1�Ž�ɫ�ƺ���ɫ�ơ����1�ţ�����1���������鶥�ˡ�(�Ƕ���
			08-�������Է�ͬ����ս��������25����ݻ�����������ǿ�ĵ�λ���������أ������ᷨӡ��
			09-���𣺽�1���о���λ���������ڰ볡ͬ�ţ����ڴ��Ž��´̹Ǳ�˪.(����ϣ��)
			10-����ʹ�Է����ŵġ��̹Ǳ�˪���˺����1�㡣(������ʿ)
			11-���𣺴������������š��͡�֯�š�(�������������)
			12-���𣺴������������š��͡�����š�(��������֯��)
			13-���𣺴���������֯�š��͡�����š�(������������)
			15-��������3ֻ����ŭ���ǡ������ڶԷ�ͬ�Ž��¡�����Ũ����������
			16-�ޣ���ŭ���ǣ�
			17-����ÿ�δ����ƴ����ɫ��ʱ�������Ƴ��⣩���㽫�����еġ��ܲ���������������֮ǰ�ٻ������λ�á����ܲ���
			20-�����������������2����ӥ��Ů����������������ŵӥ��Ů����
			22-���𣮻�û���
			   ��Ը���ڸ���ĩλ����2�����μ���Ԫ�ء�����Ԫ�أ�
			23-���𣺴����鶥�˳�1���ơ�������ս�������ŵ�1����λ���������ͬһ�š���η���ߣ�
			24-���������Ҳ൥λ��ÿ2�غϣ������غϿ�ʼʱ�������Ҳ൥λ����������սʿ��
			25-ÿ������������Ũ���ڵ�����Ч�����������1����С������������ͬ�š�
			   ��������û�С�С���������򸴻�1����С������������ͬ�š�
			   �����ϲ��ٴ��ڡ�����Ũ��Ч������ݻ����С�С��������С������
			26-���𣺴����������С�з֩�롱����з֩�룩
			27-ÿ�����Ѿ���λ����1���ƣ��������������1ֻ��з֩���׳桱������ܵ�1���˺������ӻ��ף���
			   ���𣺻��2�㻤�ס���з֩����ޣ�
			28-ʹ��������ĵ�λ��ö�����4�����档
			   ��Ը�������������1ֻӥ��Ů�����̡���ӥ��Ů������

		���ġ���λ���֣�de����¼���ƹ�����
		����λ���֣�f����¼�����Ƿ�ΪӢ�ۣ�f=9ʱ��Ӣ�ۣ��������ʾͬ�ֿ��Ƹ���

	����a=3��ʱ
		�ڶ�����λ���֣�bc����¼����������
			02-�Գ�����ǿ�ĵ�λ�����ֵ��ͬ����ս�����˺�������಻����10�㣨���ӻ��ף�����ʹ���������ķ�ͬ����λ�����ͬ��ֵ�����档�����˶�Ť��֮����
			03-�ڶԷ����Ž��¡�����Ũ��������Ũ��ÿ�������غϿ�ʼʱ������������ǿ�ĵ�λ���2���˺���������Ũ��
			04-�ڶԷ����Ž��¡��̹Ǳ�˪�����̹Ǳ�˪��ÿ�������غϿ�ʼʱ���������������ĵ�λ���2���˺������̹Ǳ�˪��
			14-ʹ 3 �����ڵ�λ��� 2 �㻤�׺� 3 �����档(����ҩˮ��
			18-���ɡ���ա�������������������
			19-�ڶԷ����Ž��¡�������ꡱ��������꣺ÿ�������غϿ�ʼʱ�������������2�������λ���1���˺���������꣩
			21-�Ե������е�λ���3���˺�����˺�ѣ�
	����λ���֣�g����¼���Ʋ��ʣ�0-��1-����2-ͭ��
*/

#include"CP_Card.h"

Card::Card():
	ID(-1),
	attack(0)
{
}

Card::Card(const Card &root_card)
{

	ID = root_card.ID;
	No = root_card.No;
	number = root_card.number;
	explainText = root_card.explainText;
	name = root_card.name;

	genre = root_card.genre;
	attack = root_card.attack;
	skill = root_card.skill;
	material = root_card.material;
	armer = root_card.armer;

	isHero = root_card.isHero;
	isFriend = root_card.isFriend;
	isInGameCardsStack = root_card.isInGameCardsStack;
	isPickerActive = root_card.isPickerActive;
	isSelected = root_card.isSelected;
	isFielded = root_card.isFielded;
	isGarbaged = root_card.isGarbaged;
	isShield = root_card.isShield;

	isWeatherControlled = root_card.isWeatherControlled;

	card_height = root_card.card_height;
	card_width = root_card.card_width;

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

	//��¼����ID
	ID = num / 10000000;

	//���Ʊ��
	No = num;

	num %= 10000000;

	//����������Ϣ
	CreateLogic(num);

	//���ÿ��Ƶĳ���
	card_height = 400; 
	card_width = 300;

}

void Card::CreateLogic(int num)
{

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;
	isInGameCardsStack = false;
	isFriend = true;
	isShield = false;
	armer = 0;
	isWeatherControlled = 0;
	number = 1;

	int temp_num = 0;

	genre = num / 1000000;//��������

	temp_num = num / 10000;
	skill = temp_num % 100;//���Ƽ���

	temp_num = num / 100;
	attack = num % 100;//����ս����

	temp_num = (num / 10) % 10;
	if (temp_num ==9 )
	{
		isHero = true;
	}//�Ƿ�ΪӢ����
	else
	{
		isHero = false;
		number = temp_num;
	}

	material = num % 10;//���Ʋ���
}

void  Card::AddName(int num)
{
	switch (num)
	{
	case 13010590:
		name = "Unseen_Elder";
		break;
	case 25020011:
		name = "Bekker_Twister_Mirror";
		break;
	case 35030012:
		name = "Impenetrable_Fog";
		break;
	case 45040012:
		name = "Biting_Frost";
		break;
	case 53050690:
		name = "Dagon";
		break;
	case 63060812:
		name = "Archgriffin";
		break;
	case 63060822:
		name = "Archgriffin";
		break;
	case 63060832:
		name = "Archgriffin";
		break;
	case 73070110:
		name = "Ge_Els";
		break;
	case 83080510:
		name = "Geralt_Igni";
		break;
	case 93090810:
		name = "Caranthir";
		break;
	case 103100812:
		name = "Wild_Hunt_Rider";
		break;
	case 112110611:
		name = "Crone_Whispess";
		break;
	case 122120611:
		name = "Crone_Weavess";
		break;
	case 132130811:
		name = "Crone_Brewess";
		break;
	case 145140012:
		name = "Thunderbolt_potion";
		break;
	case 153150510:
		name = "Woodland_Spirit";
		break;
	case 160160112:
		name = "Raging_Wolf";
		break;
	case 173170411:
		name = "Roach";
		break;
	case 185180012:
		name = "First_Light";
		break;
	case 195190012:
		name = "Torrential_Rain";
		break;
	case 203200512:
		name = "Celaeno_Harpy";
		break;
	case 215210012:
		name = "Lacerate";
		break;
	case 220220612:
		name = "Earth_Elemental";
		break;
	case 234231211:
		name = "Frightener";
		break;
	case 243240612:
		name = "Vran_Warrior";
		break;
	case 253250412:
		name = "Foglet";
		break;
	case 261260312:
		name = "Arachas";
		break;
	case 272270612:
		name = "Arachas_Behemoth";
		break;
	case 283280112:
		name = "Harpy_Egg";
		break;
	case 295290011:
		name = "Commander_Horn";
		break;
	}
}


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