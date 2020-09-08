#pragma once

#include "KeyboardEvent.h"
#include "KeyboardClass.h"
#include <queue>

class KeyboardClass
{
public:
	KeyboardClass();
	bool KeyIsPressed(const unsigned char keycode);	//�L�[��������Ă��邩�̊m�F
	bool KeyBufferIsEmpty();		//�L�[�o�b�t�@���󂩂ǂ���
	bool CharBufferIsEmpty();	//�����o�b�t�@���󂩂ǂ���
	KeyboardEvent ReadKey();	//�L�[�̓ǂݎ��
	unsigned char ReadChar();	//�����o�b�t�@���󂩂ǂ���
	void OnKeyPressed(const unsigned char key);		//�����ꂽ�ꍇ�̏���
	void OnKeyReleased(const unsigned char key);	//�����ꂽ�ꍇ�̏���
	void OnChar(const unsigned char key);	//�E�B���h�E�v���V�[�W������L���[��
	void EnableAutoRepeatKeys();	//�J��Ԃ���L���ɂ���
	void DisableAutoRepeatKeys();	//�J��Ԃ��𖳌��ɂ���
	void EnableAutoRepeatChars();		//�J��Ԃ���L���ɂ���
	void DisableAutoRepeatChars();	//�J��Ԃ��𖳌��ɂ���
	bool IsKeysAutoRepeat();		//�J��Ԃ���L���ɂ���
	bool IsCharsAutoRepeat();	//�J��Ԃ��𖳌��ɂ���
private:
	bool autoRepeatKeys = false;//�L�[�̏�Ԃ�������Ă���ԌJ��Ԃ�
	bool autoRepeatChars = false;//������������Ă���ԌJ��Ԃ�
	bool KeyStates[256];
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};