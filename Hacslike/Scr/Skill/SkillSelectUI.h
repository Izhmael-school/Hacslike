#pragma once
#include"Skill.h"

class SkillSelectUI{
private:
    int selectedIndex = 0;  //選択用のインデックス
    bool isActive = false;  //表示非表示
    int cardWidth = 260;    //横
    int cardHeight = 360;   //縦

public:
    /// <summary>
    /// スタート
    /// </summary>
    void StartSelection();
    /// <summary>
    /// 更新
    /// </summary>
    /// <returns></returns>
    int UpdateSelection();
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="skills"></param>
    void Render(const std::vector<std::shared_ptr<Skill>>& skills);
    bool IsActive() const { return isActive; }

};

