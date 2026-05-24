#include <ui.h>
#include <algorithm>

Rectangle placeRectangleTopRightCorner(Rectangle r, float w)
{
	r.x = w - r.width;
	r.y = 0;
	return r;
}

Rectangle placeRectangleTopLeftCorner(Rectangle r, float w)
{
	r.x = 0;
	r.y = 0;
	return r;
}

Rectangle placeRectangleBottomRightCorner(Rectangle r, float w, float h)
{
	r.x = w - r.width;
	r.y = h - r.height;
	return r;
}

Rectangle placeRectangleBottomLeftCorner(Rectangle r, float w, float h)
{
	r.x = 0;
	r.y = h - r.height;
	return r;
}

Rectangle placeRectangleCenter(Rectangle r, float w, float h)
{
	r.x = (w - r.width) / 2.0f;
	r.y = (h - r.height) / 2.0f;
	return r;
}

Rectangle placeRectangleCenterTop(Rectangle r, float w)
{
	r.x = (w - r.width) / 2.0f;
	r.y = 0;
	return r;
}

Rectangle placeRectangleCenterBottom(Rectangle r, float w, float h)
{
	r.x = (w - r.width) / 2.0f;
	r.y = h - r.height;
	return r;
}

Rectangle placeRectangleCenterLeft(Rectangle r, float h)
{
	r.x = 0;
	r.y = (h - r.height) / 2.0f;
	return r;
}

Rectangle placeRectangleCenterRight(Rectangle r, float w, float h)
{
	r.x = w - r.width;
	r.y = (h - r.height) / 2.0f;
	return r;
}

Rectangle enlargeRectanglePixels(Rectangle r, float pixelsX, float pixelsY)
{
	r.width += pixelsX;
	r.height += pixelsY;

	r.x -= pixelsX / 2.f;
	r.y -= pixelsY / 2.f;

	return r;
}

Rectangle shrinkRectanglePercentage(Rectangle r, float percentageX, float percentageY)
{
	float shrinkX = r.width * percentageX;
	float shrinkY = r.height * percentageY;

	r.width -= shrinkX;
	r.height -= shrinkY;

	r.x += shrinkX / 2.f;
	r.y += shrinkY / 2.f;

	return r;
}

bool UIEngine::addButton(std::string text)
{
	Widget w;
	w.type = BUTTON;
	w.text = text;
	widgets.push_back(w);

	if (lastFrameWidgets.size() < widgets.size())
	{
		return false;
	}
	if (lastFrameWidgets[widgets.size() - 1].type != BUTTON)
	{
		return false;
	}
	return lastFrameWidgets[widgets.size() - 1].isReleased;
}

void UIEngine::addTitle(std::string text)
{
	Widget w;
	w.type = TITLE;
	w.text = text;
	widgets.push_back(w);
}

void UIEngine::updateAndRender()
{
	float w = GetScreenWidth();
	float h = GetScreenHeight();
	Rectangle oneButtonRectangle;
	oneButtonRectangle.width = w * 0.8f;
	oneButtonRectangle.height = h / (widgets.size()+1);
	oneButtonRectangle.height = std::min(oneButtonRectangle.height, oneButtonRectangle.width / 8.f);
	oneButtonRectangle = placeRectangleCenterTop(oneButtonRectangle, w);
	oneButtonRectangle.y += oneButtonRectangle.height / 2.f;

	int fontSize = (int)(oneButtonRectangle.height * 0.5f);

	for (auto& w : widgets)
	{
		Rectangle smallerRect = shrinkRectanglePercentage(oneButtonRectangle, 0.01, 0.1);
		//;

		auto drawText = [&](Rectangle smallerRect, float yOffset = 0)
		{
			int textWidth = MeasureText(w.text.c_str(), fontSize);
			int textHeight = fontSize;

			float textX = smallerRect.x + (smallerRect.width - textWidth) / 2.0f;
			float textY = smallerRect.y + (smallerRect.height - textHeight) / 2.0f;

			Color shadowColor = { 0,0,0,200 };
			DrawText(w.text.c_str(), textX - fontSize * 0.08, textY + fontSize * 0.08 + yOffset, fontSize, shadowColor);
			DrawText(w.text.c_str(), textX, textY, fontSize, WHITE);
		};

		w.isClicked = false;
		w.isHovered = false;
		w.isReleased = false;

		if (CheckCollisionPointRec(GetMousePosition(), smallerRect))
		{
			w.isHovered = true;
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
			{
				w.isClicked = true;
			}
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				w.isReleased = true;
			}
		}

		switch (w.type)
		{
			case BUTTON:
			{
				const float clickOffset = 0.05f;
				Color clickColor = { 120,120,135,205 };
				Color defaultColor = { 90,90,110,205 };

				if (w.isClicked)
				{
					DrawRectangle(smallerRect.x, smallerRect.y + smallerRect.height * clickOffset, 
						smallerRect.width, smallerRect.height, clickColor);
				}
				else
					if (w.isHovered)
					{
						DrawRectangle(smallerRect.x, smallerRect.y, smallerRect.width, smallerRect.height, clickColor);
					}
					else
					{
						DrawRectangle(smallerRect.x, smallerRect.y, smallerRect.width, smallerRect.height, defaultColor);
					}
				if (w.isClicked)
				{
					drawText(smallerRect, smallerRect.height * clickOffset);
				}
				else
				{
					drawText(smallerRect);
				}
				break;
			}
			case TITLE:
			{
				drawText(smallerRect);
				break;
			}
		}

		oneButtonRectangle.y += oneButtonRectangle.height;
	}

	bool disableInputThisFrame = false;

	if (widgets.size() != lastFrameWidgets.size())
	{
		disableInputThisFrame = true;
	}
	else
	{
		for (int i = 0; i < widgets.size(); i++)
		{
			if (widgets[i].type != lastFrameWidgets[i].type)
			{
				disableInputThisFrame = true;
				break;
			}
		}
	}

	lastFrameWidgets = widgets;

	if (disableInputThisFrame)
	{
		for (auto& w : lastFrameWidgets)
		{
			w.isHovered = false;
			w.isReleased = false;
			w.isClicked = false;
		}
	}

	lastFrameWidgets = widgets;
	widgets.clear();
}