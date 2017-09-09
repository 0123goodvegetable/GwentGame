#include"CP_PlayingLogic.h"
#include<QFile>
#include<QTextStream>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b

PlayingLogic::PlayingLogic(QList<CardsUI*> &aim_stack)
{
	//初始化牌组状态信息
	for(int i=0;i<aim_stack.size();i++)
	{
		CardsUI *temp_card = aim_stack[i];
		cardStack.append(temp_card);
	}

}

QList<CardsUI*> PlayingLogic::operateCard(Card &card,int card_No)
{
	//对于卡牌不同的skill进行不同的响应
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
	//5、不需要达冈技能
	case 6:
		skill6(card_No);
		break;
	//7、盖尔不需要技能
	case 8:
		skill8(card_No);
		break;
	//9、卡兰希尔不需要额外技能
	//10、狂猎骑士不需要额外技能
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

	//返回改变后的牌组状态
	return cardStack;
}

/*
*实现所有卡牌的技能
*/
//1、暗影长者
void PlayingLogic::skill1(int number)
{
	//搜寻打出卡牌，设置其属性为在场上
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

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			cardStack[i]->operating_card->isGarbaged = true;//该卡牌被吞噬
			cardStack[num]->operating_card->attack += cardStack[i]->operating_card->attack;//战斗力被暗影长者吸收
			cardStack[i]->operating_card->isSelected = false;
			cardStack[i]->operating_card->isFielded = false;
		}
	}
}

//2、贝克尔扭曲之镜
void PlayingLogic::skill2(int number)
{

	//搜寻打出卡牌后的后续卡牌,交换战斗力
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

//3、蔽日浓雾
void PlayingLogic::skill3(int number)
{
	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			cardStack[i]->operating_card->isWeatherControlled = 1;
			cardStack[i]->operating_card->isSelected = false;
		}
	}
}

//4、刺骨冰霜
void PlayingLogic::skill4(int number)
{
	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			cardStack[i]->operating_card->isWeatherControlled = 2;
			cardStack[i]->operating_card->isSelected = false;
		}
	}
}


//6、大狮鹫
void PlayingLogic::skill6(int number)
{

	//搜寻打出卡牌后的后续卡牌
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected==true)
		{
			cardStack[i]->operating_card->isWeatherControlled = 0;//清空天气影响
			cardStack[i]->operating_card->isSelected = false;
		}
	}

}

//8、杰洛特：伊格尼法印
void PlayingLogic::skill8(int number)
{
	//搜寻打出卡牌，设置其属性为在场上
	int num = 0;
	for (num = 0; num < cardStack.size(); num++)
	{
		if (cardStack.at(num)->operating_card->skill == 8&&
			cardStack.at(num)->operating_card->number==number)
		{
			break;
		}
	}

	//搜寻打出卡牌后的后续卡牌
	QList<int> record_num;
	for (int i = 0; i < cardStack.size(); i++)
	{
		if (cardStack[i]->operating_card->isSelected == true)
		{
			record_num.append(i);
		}
	}

	int attack_all = 0;//卡牌总战力
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
