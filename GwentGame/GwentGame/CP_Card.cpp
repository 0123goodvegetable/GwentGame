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

int Card::Init(int num)
{
	//��¼�����ļ�·��
	QString temp_string = QString::number(num, 10);
	path = "Resource\\Cards\\" + temp_string + ".jpg";

	return CreateLogic(num);
}

int Card::CreateLogic(int num)
{
	/*
	��6λ����(abcdef)����¼������Ϣ
	��һλ���֣�a����¼�������ࣨ0-��ս��1-Զ�̣�2-���ǣ�3-�����ƣ�
	����0<=a<=2��ʱ
		�ڶ�����λ���֣�bc����¼���Ƽ��ܣ�00-�޼��ܣ�01-����ͨ��λ��������...)
		���ġ���λ���֣�de����¼���ƹ�����
		����λ���֣�f����¼�����Ƿ�ΪӢ��
	����a=3��ʱ
		�ڶ�λ���ּ�¼���������ࣨ0-���ʣ�1-�꣬2-��3-��˪��4-�����ˣ�5-�ջٹ�������ߵĵ�λ...��
	*/

	isHero = false;
	isPickerActive = false;
	isSelected = false;
	isFielded = false;
	isGarbaged = false;

	int temp_num = 0;

	No = num;
}