#include"CP_CardStack.h"
CardStack::CardStack()
{
	game_cardStack_num << 13010530;
}

void CardStack::addGameCard(int No)
{
	game_cardStack_num.append(No);
}

void CardStack::deleteGameCard(int No)
{
	for (int i = 0; i < game_cardStack_num.size(); i++)
	{
		if (game_cardStack_num.at(i) == No)
		{
			game_cardStack_num.removeAt(i);
		}
	}
}