#include"CP_PlayingLogic.h"
#include<QFile>
#include<QTextStream>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

PlayingLogic::PlayingLogic(QList<CardsUI*> &aim_stack)
{
	//��ʼ������״̬��Ϣ
	for(int i=0;i<aim_stack.size();i++)
	{
		CardsUI *temp_card = aim_stack[i];
		cardStack.append(temp_card);
	}

}

QList<CardsUI*> PlayingLogic::operateCard(Card &card,int card_No)
{
	//���ڿ��Ʋ�ͬ��skill���в�ͬ����Ӧ
	switch (card.skill)
	{
	case 1:
		skill1(card_No);
		break;
	case 2:
		skill2(card_No);
		break;
	case 3:
		skill3(card_No);
		break;
	case 4:
		skill4(card_No);
		break;
	//5������Ҫ��Լ���
	case 6:
		skill6(card_No);
		break;
	//7���Ƕ�����Ҫ����
	case 8:
		skill8(card_No);
		break;
	//9������ϣ������Ҫ���⼼��
	//10��������ʿ����Ҫ���⼼��
	/*
	case 11:
		skill11();
		break;
	case 12:
		skill12();
		break;
	case 13:
		skill13();
		break;
	case 14:
		skill14();
		break;
	case 15:
		skill15();
		break;
	case 16:
		skill16();
		break;
	case 17:
		skill17();
		break;
	case 18:
		skill18();
		break;
	case 19:
		skill19();
		break;
	case 20:
		skill20();
		break;
	case 21:
		skill21();
		break;
	case 22:
		skill22();
		break;
	case 23:
		skill23();
		break;
	case 24:
		skill24();
		break;
	case 25:
		skill25();
		break;
	case 26:
		skill26();
		break;
	case 27:
		skill27();
		break;
	case 28:
		skill28();
		break;
	case 29:
		skill29();
		break;*/
	}

	//���ظı�������״̬
	return cardStack;
}

/*
*ʵ�����п��Ƶļ���
*/
//1����Ӱ����
void PlayingLogic::skill1(int number)
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num)->operating_card->skill== 1&&
			cardStack.at(num)->operating_card->number==number)
		{
			break;
		}
	}
	cardStack[num]->operating_card->isFielded = true;

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			cardStack[i]->operating_card->isGarbaged = true;//�ÿ��Ʊ�����
			cardStack[num]->operating_card->attack += cardStack[i]->operating_card->attack;//ս��������Ӱ��������
			cardStack[i]->operating_card->isSelected = false;
			cardStack[i]->operating_card->isFielded = false;
		}
	}
}

//2�����˶�Ť��֮��
void PlayingLogic::skill2(int number)
{

	//��Ѱ������ƺ�ĺ�������,����ս����
	int max_num=0, min_num=0, max_attack = cardStack[0]->operating_card->attack, min_attack = cardStack[0]->operating_card->attack;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isFielded==true)
		{
			if (cardStack[i]->operating_card->attack > max_attack)
			{
				max_attack = cardStack[i]->operating_card->attack;
				max_num = i;
			}
			else if (cardStack[i]->operating_card->attack < min_attack)
			{
				min_attack = cardStack[i]->operating_card->attack;
				min_num = i;
			}
		}
	}

	if (cardStack[max_num]->operating_card->attack <= 10)
	{
		cardStack[min_num]->operating_card->attack += cardStack[max_num]->operating_card->attack;
		cardStack[max_num]->operating_card->isGarbaged = true;
	}
	else
	{
		cardStack[min_num]->operating_card->attack += 10;
		cardStack[max_num]->operating_card->attack -= 10;
	}

}

//3������Ũ��
void PlayingLogic::skill3(int number)
{
	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			cardStack[i]->operating_card->isWeatherControlled = 1;
			cardStack[i]->operating_card->isSelected = false;
		}
	}
}

//4���̹Ǳ�˪
void PlayingLogic::skill4(int number)
{
	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			cardStack[i]->operating_card->isWeatherControlled = 2;
			cardStack[i]->operating_card->isSelected = false;
		}
	}
}


//6����ʨ��
void PlayingLogic::skill6(int number)
{

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected==true)
		{
			cardStack[i]->operating_card->isWeatherControlled = 0;//�������Ӱ��
			cardStack[i]->operating_card->isSelected = false;
		}
	}

}

//8�������أ������ᷨӡ
void PlayingLogic::skill8(int number)
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num)->operating_card->skill == 8&&
			cardStack.at(num)->operating_card->number==number)
		{
			break;
		}
	}

	//��Ѱ������ƺ�ĺ�������
	QList<int> record_num;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			record_num.append(i);
		}
	}

	int attack_all = 0;//������ս��
	int max_attack = cardStack[record_num[0]]->operating_card->attack;
	for (int j = 0; j < record_num.size(); j++)
	{
		attack_all += cardStack[record_num[j]]->operating_card->attack;
		max_attack = max(max_attack, cardStack[record_num[j]]->operating_card->attack);
	}
	if (attack_all >= 25)
	{
		for (int j = 0; j < record_num.size(); j++)
		{
			if (cardStack[record_num[j]]->operating_card->attack == max_attack)
			{
				cardStack[record_num[j]]->operating_card->isGarbaged = true;
			}
		}
	}
}
