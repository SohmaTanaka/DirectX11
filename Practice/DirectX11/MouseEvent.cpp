#include "MouseEvent.h"

MouseEvent::MouseEvent() :
	type(EventType::Invalid), x(0), y(0)
{
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) :
	type(type), x(x), y(y)
{
}

//—LŒø‚Å‚ ‚ê‚Îtrue
bool MouseEvent::IsValid() const
{
	return this->type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return this->type;
}

MousePoint MouseEvent::GetPos() const
{
	//\‘¢‘Ì‚ð—ªŽ®éŒ¾‚Å•Ô‚·
	return { this->x,this->y };
}

int MouseEvent::GetPosX()
{
	return this->x;
}

int MouseEvent::GetPosY()
{
	return this->y;
}
