#ifndef ELEMBASE
#define ELEMBASE

#include <vector>

namespace player3 { namespace ui
{
	enum ElementType
	{
		Image,
		Label
	};
	struct Style
	{
		public:
			int width;
			int height;
			Box margin;
			Box padding;
			int fontSize;
			const char* bgColor;
			const char* fgColor;
	};
	struct Box
	{
		public:
			int Left;
			int Right;
			int Top;
			int Bottom;
			Box()
			{
				Left = 0;
				Right = 0;
				Top = 0;
				Bottom = 0;
			}
			Box(int left, int right, int top, int bottom)
				: Left(left), Right(right), Top(top), Bottom(bottom)
			{}
	};
	struct Layout
	{
		public:
			Style style;
			const char* name;
			std::vector<ElementBase> childElements;
	};
	class ElementBase
	{
		public:
			virtual Box GetBoundingBox() = 0;
			virtual ElementType GetElementType() = 0;
			virtual void SetElementStyle(Style style) = 0;
			virtual void SetValidPropertyBindings(const char* bindingName, const char* boundProperty) = 0;
	};
}}

#endif