#include "ShapeModeSelectorMenu.h"
#include "Line.h"

namespace touchpoints { namespace ui {
	ShapeModeSelectorMenu::ShapeModeSelectorMenu() {}
	
	ShapeModeSelectorMenu::ShapeModeSelectorMenu(vec2 startPoint, bool visible, drawing::Brush* brush,
		Shape::Shape shape) : BrushModeSelectorMenu(startPoint, Menu::defaultWidth, Menu::defaultHeight, 
		visible, brush, nullptr), shape(shape)
	{
		setupShapes(shape);
	}
	
	void ShapeModeSelectorMenu::Draw()
	{
		if(!visible)
		{
			return;
		}

		auto whiteSquare = composingShapes.find(0)->second;
		whiteSquare->Draw();

		auto greySquare = composingShapes.find(1)->second;
		greySquare->Draw();

		auto shape = composingShapes.find(2)->second;
		shape->setColor(brush->getColor());
		shape->setFilledShape(brush->getFilledShapes());
		shape->Draw();
	}
	
	bool ShapeModeSelectorMenu::OnTouch(vec2 point)
	{
		bool touchWasHandled = false;
		if(visible && boundingRect.Contains(point))
		{
			touchWasHandled = true;
			brush->changeShape(shape);
		}
		return touchWasHandled;
	}

	void ShapeModeSelectorMenu::setupShapes(Shape::Shape shape)
	{
		auto startX = startPoint.x;
		auto startY = startPoint.y;
		auto endX = startPoint.x + width;
		auto endY = startPoint.y + height;
		auto endPoint = vec2(endX, endY);
		composingShapes.insert(make_pair(0, shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(startX, startY, endX, endY, Menu::white, 0, true))));
		composingShapes.insert(make_pair(1, shared_ptr<drawing::TouchRectangle>
			(new drawing::TouchRectangle(startX, startY, endX, endY, Menu::grey, Menu::defaultBorderThickness, false))));

		switch(shape)
		{
			case Shape::Line:
			{
				//bottom left corner +- offset for padding
				auto startPoint = vec2(startX + defaultImageOffsetX, endY - defaultImageOffsetY);
				//top right corner +- offset for padding
				auto endPoint = vec2(endY - defaultImageOffsetX, startX + defaultImageOffsetY);
				composingShapes.insert(make_pair(2, shared_ptr<drawing::TouchPoint>
					(new drawing::TouchPoint(startPoint, endPoint, brush->getColor(), Menu::defaultLineThickness))));
				break;
			}
			case Shape::Circle:
			{
				//default width of menu panel - 10 for padding
				auto radius = (Menu::defaultWidth - 10) / 2;
				auto center = math::FindMidPoint(startPoint, endPoint);
				composingShapes.insert(make_pair(2, shared_ptr<drawing::TouchCircle>
					(new drawing::TouchCircle(center, radius, brush->getColor(), 
						Menu::defaultLineThickness, brush->getFilledShapes()))));
				break;
			}
			case Shape::Rectangle:
			{
				//adding 10px of padding to rect
				composingShapes.insert(make_pair(2, shared_ptr<drawing::TouchRectangle>
					(new drawing::TouchRectangle(startX + 10, startY + 10, endX - 10, endY - 10, 
						brush->getColor(), Menu::defaultLineThickness, brush->getFilledShapes()))));
				break;
			}
			case Shape::Triangle:
			{
				auto baseVertexLeft = vec2(startX + 10, endY - 10);
				auto baseVertexRight = vec2(endX - 10, endY - 10);
				auto baseCenter = math::FindMidPoint(baseVertexLeft, baseVertexRight);
				auto oppositeBaseVertex = vec2(baseCenter.x, startY + 10);
				composingShapes.insert(make_pair(2, shared_ptr<drawing::TouchVerticalTriangle>
					(new drawing::TouchVerticalTriangle(baseVertexLeft, baseVertexRight, oppositeBaseVertex, 
						baseCenter, brush->getColor(), Menu::defaultLineThickness, brush->getFilledShapes()))));
				break;
			}
			case Shape::Eraser:
			{
				auto imageStartPoint = vec2(startX + Menu::defaultImageOffsetX, startY + Menu::defaultImageOffsetY);
				composingShapes.insert(make_pair(2, shared_ptr<drawing::TouchImage>
					(new drawing::TouchImage(imageStartPoint, Menu::defaultImageWidth, 
						Menu::defaultImageHeight, "Eraser.png"))));
				break;
			}
		}
	}
}}
