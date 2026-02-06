#pragma once
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <DxLib.h>
#include"../../Manager/ArtifactManager.h"
#include"../../Definition.h"

class ArtifactUI
{
public:
    const int baseX = 250;               // インベントリウィンドウ左上X
    const int baseY = 16;               // インベントリウィンドウ左上Y
    const int boxW = 400;              // ウィンドウ幅
    const int boxH = 440;              // ウィンドウ最大高さ（表示領域）
    const int padding = 6;
    const int titleHeight = 22;
    const int lineHeight = 28;          // 1行あたりの高さ
    const int iconSize = 20;            // アイコン表示サイズ（アイコンはロード時にこのサイズに合わせるか、DrawExtendGraphで拡縮）
    const int iconX = baseX + padding;
    const int textX = iconX + iconSize + 8;

    int scrollOffset = 0;   // 現在のスクロール位置
    int scrollSpeed = 16;   // スクロール量（ホイール1回 or キー1回で動く距離）

    int displayStartIndex = 0;   // 現在表示の先頭インデックス
    const int displayCount = 5;  // 一度に表示できる数

public:
    ArtifactUI() = default;
    ~ArtifactUI() = default;

    void Update();
    void Render();
};

