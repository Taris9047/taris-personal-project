#if !defined MODEL_HPP
#define MODEL_HPP

#include <string>
#include <Windows.h>

class Model
{
	enum { TEXT_SIZE = 20 };
public:
	Model(LPCWSTR str)
	{
		SetText(str);
		_text[TEXT_SIZE] = '\0';
	}

	void SetText(LPCWSTR str)
	{
		wcsncpy_s(_text, str, TEXT_SIZE);
	}

	LPCWSTR GetText() const { return _text; }
	int GetLen() const { return wcslen(_text); }

private:

	WCHAR _text[TEXT_SIZE + 1];
};

#endif