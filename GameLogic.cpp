#include <vector>
#include <random>
#include <string>
#include "GameLogic.h"

namespace solitaire
{
	void GameLogic::Initialize(HWND hwnd)
	{
		mHwnd = hwnd;
		mpPreviousCard = nullptr;

		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");
		CreateCards();
		mFlipCount = 0;
	}

	void GameLogic::Release()
	{
		mDeck.clear();
		mBackground.reset();
	}
	void GameLogic::OnClick(int curX, int curY)
	{
		Card* pCard{};

		for (auto& e : mDeck)
		{
			if (e.CheckClicked(curX, curY))
			{
				pCard = &e;
				break;
			}
		}

		if (pCard)
		{
			mFlipCount++;
			RECT rc{
				mCountRect.GetLeft(), mCountRect.GetTop(),
				mCountRect.GetRight(), mCountRect.GetBottom()
			};
			InvalidateRect(mHwnd, &rc, false);

			if (mpPreviousCard == nullptr)
			{
				mpPreviousCard = pCard;
			}

			else
			{
				if (mpPreviousCard->GetType() == pCard->GetType())
				{
					pCard->Invalidate();
					mpPreviousCard->Invalidate();

					mDeck.remove_if(
						[&](Card& card){
							return card.GetIndex() == mpPreviousCard->GetIndex() ||
								   card.GetIndex() == pCard->GetIndex();
						}
					);

					mpPreviousCard = nullptr;
				}
				else
				{
					UpdateWindow(mHwnd);
					Sleep(500);
					pCard->Flip(false);
					mpPreviousCard->Flip(false);

					mpPreviousCard = nullptr;
				}
			}
		}
	}
	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

		for (auto& e : mDeck)
		{
			e.Draw(graphics);
		}

		Gdiplus::PointF pos(895.0f, 20.0f);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		Gdiplus::Font font(L"¸¼Àº °íµñ", 20);
		graphics.DrawString(L"Å¬¸¯¼ö: ", -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, mCountRect, &format, &brush);
	}
	void GameLogic::CreateCards()
	{
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_COLUMN * BOARD_ROW))
		{
			int modulator = types.size() % 6;

			switch (modulator)
			{
				case 0:
					types.push_back(Type::Bear);
					types.push_back(Type::Bear);
					break;
				
				case 2:
					types.push_back(Type::Wolf);
					types.push_back(Type::Wolf);
					break;
				
				case 4:
					types.push_back(Type::Dragon);
					types.push_back(Type::Dragon);
					break;
			}
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(types.begin(), types.end(), gen);

		int posX{ 15 }, posY{ 10 };
		int index{};
		for (int x = 0; x < BOARD_COLUMN; ++x)
		{
			posY = 10;
			for (int y = 0; y < BOARD_ROW; ++y)
			{
				mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
				posY += 150;
			}

			posX += 110;
		}
	}
}