#include "GameSelectionBackground.h"

GameSelectionBackground::GameSelectionBackground(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	button = new QPushButton(this);
	button->setText(tr("scene2"));
}

GameSelectionBackground::~GameSelectionBackground()
{
}
