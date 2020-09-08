#pragma once

#include "KeyboardEvent.h"
#include "KeyboardClass.h"
#include <queue>

class KeyboardClass
{
public:
	KeyboardClass();
	bool KeyIsPressed(const unsigned char keycode);	//キーが押されているかの確認
	bool KeyBufferIsEmpty();		//キーバッファが空かどうか
	bool CharBufferIsEmpty();	//整数バッファが空かどうか
	KeyboardEvent ReadKey();	//キーの読み取り
	unsigned char ReadChar();	//整数バッファが空かどうか
	void OnKeyPressed(const unsigned char key);		//押された場合の処理
	void OnKeyReleased(const unsigned char key);	//離された場合の処理
	void OnChar(const unsigned char key);	//ウィンドウプロシージャからキューへ
	void EnableAutoRepeatKeys();	//繰り返しを有効にする
	void DisableAutoRepeatKeys();	//繰り返しを無効にする
	void EnableAutoRepeatChars();		//繰り返しを有効にする
	void DisableAutoRepeatChars();	//繰り返しを無効にする
	bool IsKeysAutoRepeat();		//繰り返しを有効にする
	bool IsCharsAutoRepeat();	//繰り返しを無効にする
private:
	bool autoRepeatKeys = false;//キーの状態を押されている間繰り返す
	bool autoRepeatChars = false;//文字を押されている間繰り返す
	bool KeyStates[256];
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};