#pragma once

struct MousePoint
{
	int x;
	int y;
};

class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,		//�E�B���h�E��̐�Βl
		RawMove,	//�}�E�X�̈ړ��̓��͒l�i���Βl�j
		Invalid,
	};

private:
	EventType type;
	int x;
	int y;

public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	bool IsValid() const;
	EventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX();
	int GetPosY();
};