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
	//��¼�����ļ�·��
	QString temp_string = QString::number(num, 10);
	path = "Resource\\Cards\\" + temp_string + ".jpg";

	//��¼����ID
	ID = num / 10000000;

	num %= 10000000;

	CreateLogic(num);

}

void Card::CreateLogic(int num)
{
	/*
	��7λ��Ч����(abcdefg)����¼������Ϣ��9λ����ǰ��λΪID������ţ�
	��һλ���֣�a����¼�������ࣨ0-��ս��1-Զ�̣�2-���ǣ�3-�ҷ����⣬4-�з����⣬5-�����ƣ�
	����0<=a<=4��ʱ
		�ڶ�����λ���֣�bc����¼���Ƽ��ܣ�00-�޼��ܣ�01-����ͨ��λ��������...)
		���ġ���λ���֣�de����¼���ƹ�����
		����λ���֣�f����¼�����Ƿ�ΪӢ�ۣ�f<5ʱ��Ӣ�ۣ�
		����λ���֣�g����¼���Ʋ��ʣ�0-��1-����2-ͭ��
	����a=3��ʱ
		�ڶ�����λ���֣�bc����¼���������ࣨ0-���ʣ�1-�꣬2-��3-��˪��4-�����ˣ�5-�ջٹ�������ߵĵ�λ...��
	*/

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;

	int temp_num = 0;

	No = num;

	genre = num / 1000000;

	temp_num = num / 10000;
	skill = temp_num % 100;

	temp_num = num / 100;
	attack = num % 100;

	temp_num = (num/10) % 10;
	if (temp_num < 5)
	{
		isHero = true;
	}

	material = num % 10;
}
