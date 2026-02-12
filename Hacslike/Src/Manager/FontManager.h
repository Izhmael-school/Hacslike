#pragma once
#include "../Component/Singleton.h"
#include <vector>
#include <string>
#include "DxLib.h"

struct FontData {
	int fontHandle;
	std::string fontName;
};

class FontManager : public Singleton<FontManager> {
public:
	FontManager();
	~FontManager();


	/// <summary>
	/// フォントを作る
	/// </summary>
	/// <param name="fontName">フォントの名前</param>
	/// <param name="size">サイズ</param>
	/// <param name="thick">太さ</param>
	/// <param name="fontType">フォントタイプ</param>
	/// <param name="anyFontName">任意で付けるフォントの名前</param>
	void CreateFontData(std::string fontName,int size,int thick,int fontType,std::string anyFontName = "");
	int UseFontHandle(std::string _fontName);

	void DeleteFont();

private:
	std::vector<FontData> fontDataArray;

};

