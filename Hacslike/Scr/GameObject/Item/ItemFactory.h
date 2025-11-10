#pragma once
#include "ItemBase.h"
#include <unordered_map>
#include <functional>
#include <memory>


class ItemFactory
{
private://アイテムを格納するコンテナ
	std::unordered_map<std::string, std::function<std::unique_ptr<ItemBase>()>> itemTemplates;

public: //メンバ関数
	static ItemFactory& Instance();

	/// <summary>
	/// アイテムの設定
	/// </summary>
	/// <param name="id"></param>
	/// <param name="generator"></param>
	void RegisterItem(const std::string& _id, std::function<std::unique_ptr<ItemBase>()>_generator);

	/// <summary>
	/// アイテムの生成
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	std::unique_ptr<ItemBase> CreateItem(const std::string& _id);
	// 登録処理をまとめる関数を追加
	void InitializeDefaultItems();

	// シングルトンなのでコンストラクタ等は非公開
	ItemFactory() = default;
	ItemFactory(const ItemFactory&) = delete;
	ItemFactory& operator=(const ItemFactory&) = delete;
};

