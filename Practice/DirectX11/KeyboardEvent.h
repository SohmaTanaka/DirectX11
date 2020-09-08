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
	bool IsPress() const;//��������true
	bool IsRelease() const;//��������true
	bool IsValid() const;	//�������ǂ����𔻒f����B�L���Ȃ�true
	unsigned char GetKeyCode() const;//�L�[�R�[�h��Ԃ�

private:
	EventType type;
	unsigned char key;
};