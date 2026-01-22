#include "FontManager.h"

FontManager::FontManager() {
	//フォントの追加
	AddFontResourceEx("Res/Font/ReggaeOne-Regular.ttf", FR_PRIVATE, NULL);
	CreateFontData("Reggae One", 20, -1, DX_FONTTYPE_NORMAL, "MainFont");
}

FontManager::~FontManager() {

	for (auto f : fontDataArray) {
		DeleteFontToHandle(f.fontHandle);
	}

	fontDataArray.clear();
	fontDataArray.shrink_to_fit();
}


void FontManager::CreateFontData(string fontName, int size, int thick, int fontType, string anyFontName) {
	FontData data;

	// ハンドルの作成
	int f = CreateFontToHandle(fontName.c_str(), size, thick, fontType);

	// 失敗なら帰る
	if (f == -1) return;

	data.fontHandle = f;

	if (anyFontName == "")
		data.fontName = fontName;
	else
		data.fontName = anyFontName;

	// 配列に入れる
	fontDataArray.push_back(data);
}

int FontManager::UseFontHandle(string _fontName) {
	for (auto f : fontDataArray) {
		if (f.fontName != _fontName) continue;

		return f.fontHandle;
	}

	return -1;
}
