#pragma once
#include <list>
#include "Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW = 5;
		const int BOARD_COLUMN = 8;
		const Gdiplus::RectF mCountRect{ 885.0f, 60.0f, 120.0f, 30.0f };

	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;

		HWND mHwnd;
		int mFlipCount{};
		Card* mpPreviousCard;

	public:
		void Initialize(HWND hwnd);
		void Release();
		void OnClick(int curX, int curY);
		void Draw(Gdiplus::Graphics& graphics);
		void CreateCards();
	};
}

