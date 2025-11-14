#pragma once
#include"../GameObject/Artifact/ArtifactBase.h"
#include "../Component/Singleton.h"
#include <vector>
#include <memory>
#include <random>
class Player;

class ArtifactManager : public Singleton<ArtifactManager>
{
public:

	ArtifactManager();

	~ArtifactManager();

private:
	std::vector<std::shared_ptr<ArtifactBase>> activeArtifact; //現在有効なアーティファクト
	std::vector<std::shared_ptr<ArtifactBase>> artifacrPool;  //アーティファクトの候補
	std::vector<std::shared_ptr<ArtifactBase>> obtainedArtifacts;  // 獲得済みアーティファクト
public:

	/// <summary>
	/// ボスを討伐後の選択式アーチファクト
	/// </summary>
	/// <param name="artifact"></param>
	std::vector<std::shared_ptr<ArtifactBase>> GenerateArtifactChoices();

	void ApplySelectedArtifact(Player* _player, std::shared_ptr<ArtifactBase> _artifact);

	/// <summary>
	/// ダンジョン開始時にランダムで一つだけ選出するアーチファクト
	/// </summary>
	/// <param name="_artifact"></param>
	std::vector<std::shared_ptr<ArtifactBase>> ApplyArtifact();

	void Update(Player* player);


	/// <summary>
	/// 削除
	/// </summary>
	void ClearArtifact(Player* player);

	/// <summary>
	/// ゲッター
	/// </summary>
	/// <returns></returns>
	const inline std::vector<std::shared_ptr<ArtifactBase>>& GetActiveArtifact() const { return activeArtifact; }

	const std::vector<std::shared_ptr<ArtifactBase>>& GetObtainedArtifacts() const { return obtainedArtifacts; }
};

