#include "KeyboardClass.h"

KeyboardClass::KeyboardClass()
	:KeyStates()	// "()"���������q
{
	for (int i = 0; i < 256; i++)
		this->KeyStates[i] = false;
}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode)
{
	return this->KeyStates[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty()
{
	return this->keyBuffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty()
{
	return this->charBuffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey()
{
	if (this->keyBuffer.empty())
	{
		return KeyboardEvent();	//���Ԃ�
	}
	else
	{
		KeyboardEvent e = this->keyBuffer.front();	//�L���[�̓����擾
		this->keyBuffer.pop();	//�L���[����A�C�e�����폜
		return e;
	}
}

unsigned char KeyboardClass::ReadChar()
{
	if (this->charBuffer.empty())
	{
		return 0u;	//���Ԃ�
	}
	else
	{
		unsigned char e = this->charBuffer.front();	//�L���[�̓����擾
		this->charBuffer.pop();	//�L���[����A�C�e�����폜
		return e;
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key)
{
	this->KeyStates[key] = true;
	this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key)
{
	this->KeyStates[key] = false;
	this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys()
{
	this->autoRepeatKeys = true;
}

void KeyboardClass::DisableAutoRepeatKeys()
{
	this->autoRepeatKeys = false;
}

void KeyboardClass::EnableAutoRepeatChars()
{
	this->autoRepeatChars = true;
}

void KeyboardClass::DisableAutoRepeatChars()
{
	this->autoRepeatChars = false;
}

bool KeyboardClass::IsKeysAutoRepeat()
{
	return this->autoRepeatKeys;
}

bool KeyboardClass::IsCharsAutoRepeat()
{
	return this->autoRepeatChars;
}
