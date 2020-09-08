#pragma once


class KeyboardEvent
{
public:
	enum EventType
	{
		Press,
		Release,
		Invalid,
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPress() const;//押したらtrue
	bool IsRelease() const;//離したらtrue
	bool IsValid() const;	//無効かどうかを判断する。有効ならtrue
	unsigned char GetKeyCode() const;//キーコードを返す

private:
	EventType type;
	unsigned char key;
};