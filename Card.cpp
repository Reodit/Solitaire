#include "Card.h"
#include <iostream>
using namespace solitaire;

Card::Card(HWND hwnd, int index, Type type, int x, int y) : 
	mHwnd{ hwnd }, 
	mIndex { index },
	mX { x }, 
	mY{ y }, 
	mIsFront{ false }, 
	mType{ type }
{
	std::wstring filename;

	switch (mType)
	{
	case Type::Wolf:
		filename = L"Data/card_creature_wolf.png";
		break;

	case Type::Dragon:
		filename = L"Data/card_creature_dragon.png";
		break;

	case Type::Bear:
		filename = L"Data/card_creature_bear.png";
		break;
	}

	mBack = std::make_unique<Gdiplus::Image>(L"Data/card_back.png");
	mFront = std::make_unique<Gdiplus::Image>(filename.c_str());
}

bool Card::CheckClicked(int curX, int curY)
{
	if (curX >= mX && curX <= static_cast<int>(mX + mBack->GetWidth()) &&
		curY >= mY && curY <= static_cast<int>(mY + mBack->GetHeight()))
	{
		Flip(!mIsFront);
		return true;
	}

	return false;
}

void Card::Flip(bool isFront)
{
	mIsFront = isFront;
	Invalidate();
}

void Card::Draw(Gdiplus::Graphics& graphics)
{
	if (mIsFront)
	{
		graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
	}
	else
	{
		graphics.DrawImage(mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
	}
}

void Card::Invalidate()
{
	RECT rct { mX, mY, 
		static_cast<LONG>(mX + mFront->GetWidth()), 
			static_cast<LONG>(mY + mFront->GetHeight()) };
	InvalidateRect(mHwnd, &rct, false);
}
