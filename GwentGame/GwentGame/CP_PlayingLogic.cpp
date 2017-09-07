#include"CP_PlayingLogic.h"
#include<QFile>
#include<QTextStream>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

PlayingLogic::PlayingLogic(QList<Card> &aim_stack)
{
	//��ʼ������״̬��Ϣ
	for(int i=0;i<aim_stack.size();i++)
	{
		Card temp_card(aim_stack[i]);
		cardStack.append(temp_card);
	}

}

QList<Card> PlayingLogic::operateCard(Card &card)
{
	//���ڿ��Ʋ�ͬ��skill���в�ͬ����Ӧ
	switch (card.skill)
	{
	case 1:
		skill1();
		break;
	case 2:
		skill2();
		break;
	case 3:
		skill3();
		break;
	case 4:
		skill4();
		break;
	case 5:
		skill5();
		break;
	case 6:
		skill6();
		break;
	case 7:
		skill7();
		break;
	case 8:
		skill8();
		break;
	case 9:
		skill9();
		break;
	/*case 10:
		skill10();
		break;
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
void PlayingLogic::skill1()
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No== allCards.Unseen_Elder_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isGarbaged = true;//�ÿ��Ʊ�����
			cardStack[num].attack += cardStack[i].attack;//ս��������Ӱ��������
			cardStack[i].isSelected = false;
		}
	}
}

//2�����˶�Ť��֮��
void PlayingLogic::skill2()
{
	//��Ѱ������ƣ�����������Ϊ��Ĺ��
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Bekker_Twister_Mirror_No)
		{
			break;
		}
	}
	cardStack[num].isGarbaged = true;

	//��Ѱ������ƺ�ĺ�������,����ս����
	int max_num, min_num, max_attack = cardStack[0].attack, min_attack = cardStack[0].attack;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isFielded==true)
		{
			if (cardStack[i].attack > max_attack)
			{
				max_attack = cardStack[i].attack;
				max_num = i;
			}
			else if (cardStack[i].attack < min_attack)
			{
				min_attack = cardStack[i].attack;
				min_num = i;
			}
		}
	}

	if (cardStack[max_num].attack <= 10)
	{
		cardStack[min_num].attack += cardStack[max_num].attack;
		cardStack[max_num].isGarbaged = true;
	}
	else
	{
		cardStack[min_num].attack += 10;
		cardStack[max_num].attack -= 10;
	}

}

//3������Ũ��
void PlayingLogic::skill3()
{
	//��Ѱ������ƣ�����������Ϊ��Ĺ��
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Impenetrable_Fog_No)
		{
			break;
		}
	}
	cardStack[num].isGarbaged = true;

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isWeatherControlled = 1;
			cardStack[i].isSelected = false;
		}
	}
}

//4���̹Ǳ�˪
void PlayingLogic::skill4()
{
	//��Ѱ������ƣ�����������Ϊ��Ĺ��
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Biting_Frost_No)
		{
			break;
		}
	}
	cardStack[num].isGarbaged = true;

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isWeatherControlled = 2;
			cardStack[i].isSelected = false;
		}
	}
}

//5��*���(����ɣ�
void PlayingLogic::skill5()
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Dagon_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;
}

//6��*��ʨ��(����ɣ�
void PlayingLogic::skill6()
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Archgriffin_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isFielded==true&&
			cardStack[i].isFriend==true&&
			cardStack[i].genre== cardStack[num].genre)
		{
			cardStack[i].isWeatherControlled = 0;//�������Ӱ��
			
		}
	}

}

//7���Ƕ�
void PlayingLogic::skill7()
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Ge_Els_No)
		{
			break;
		}
	}
	cardStack[num].isFielded = true;

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].isFielded = true;//���һ�Ž�ɫ������ɫ��
		}
	}

}

//8�������أ������ᷨӡ
void PlayingLogic::skill8()
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Geralt_Igni_No)
		{
			break;
		}
	}

	//��Ѱ������ƺ�ĺ�������
	QList<int> record_num;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			record_num.append(i);
		}
	}

	int attack_all = 0;//������ս��
	int max_attack = cardStack[record_num[0]].attack;
	for (int j = 0; j < record_num.size(); j++)
	{
		attack_all += cardStack[record_num[j]].attack;
		max_attack = max(max_attack, cardStack[record_num[j]].attack);
	}
	if (attack_all >= 25)
	{
		for (int j = 0; j < record_num.size(); j++)
		{
			if (cardStack[record_num[j]].attack == max_attack)
			{
				cardStack[record_num[j]].isGarbaged = true;
			}
		}
	}
}

//9������ϣ��
void PlayingLogic::skill9()
{
	//��Ѱ������ƣ�����������Ϊ�ڳ���
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num).No == allCards.Caranthir_No)
		{
			break;
		}
	}

	//��Ѱ������ƺ�ĺ�������
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isSelected == true)
		{
			cardStack[i].genre = cardStack[num].genre;//���з���λ����ͬ��
		}
	}
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i].isFriend == false&& cardStack[i].genre==cardStack[num].genre)
		{
			cardStack[i].isWeatherControlled = 2;//�ڸ��Ž��´̹Ǳ�˪
		}
	}
}