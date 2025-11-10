#include "Coin.h"
#include <algorithm>
#include"../../Manager/CollisionManager.h"
#include"../../Component/Collider/Collider.h"
#include"../Character/Player/Player.h"

/// 静的メンバ変数（プール本体）
std::vector<std::unique_ptr<Coin>> Coin::coinPool;
Coin* Coin::pInstance = nullptr;

/// <summary>
/// デストラクタ
/// </summary>
Coin::Coin(VECTOR _pos, std::string tag, int _value, int _coinValue, int _deleteTime, int _startTime
    , int _maxCoin, int _currentCoin)
    :GameObject(_pos, "Coin")
    , value(_value)
    , coinModelHandle(INVALID)
    , coinValue(_coinValue)
    , deleteTime(_deleteTime)
    , startTime(_startTime)
    , maxCoin(_maxCoin)
    , currentCoin(_currentCoin)
    , active(false) {

}

/// <summary>
/// デストラクタ
/// </summary>
Coin::~Coin()
{
    if (coinModelHandle != INVALID)
    {
        MV1DeleteModel(coinModelHandle);
        coinModelHandle = INVALID;
    }
}

void Coin::CreateInstance()
{
    pInstance = new Coin();
}

Coin* Coin::GetInstance()
{
    if (pInstance == nullptr)
        CreateInstance();

    return pInstance;
}

void Coin::DestroyInstance()
{
    if (pInstance != nullptr) {
        delete pInstance;
        pInstance = nullptr;
    }
}

/// <summary>
/// 初期化
/// </summary>
void Coin::Start()
{
    SetCollider(new SphereCollider(this, VZero, 30));
    CollisionManager::GetInstance().Register(pCollider);
    coinModelHandle = MV1LoadModel("Res/Model/DropObject/coin.mv1");
    coinValue = 1;
    value = coinValue;
}

/// <summary>
/// 更新
/// </summary>
void Coin::Update()
{
    if (!active) return;

   

    rotation.y += 10.0f;
    if (position.y <= 6) {
        position.y += 0.1f;
    }
    else if (position.y >= 6) {
        position.y -= 0.1f;
    }

    if (startTime >= deleteTime)
    {

        return;
    }
    if (pCollider != nullptr)
    {
        pCollider->SetMatrix(matrix);
        pCollider->Update();
    }
    MV1SetMatrix(coinModelHandle, matrix);
    GameObject::Update();
}

/// <summary>
/// 描画
/// </summary>
void Coin::Render()
{
    if (!active) return;

#if _DEBUG
    DrawFormatString(100, 10, red, "startTime%d", startTime);
#endif

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    MV1SetMatrix(coinModelHandle, matrix);
    MV1DrawModel(coinModelHandle);
    //当たり判定の描画
   /* if (pCollider != nullptr) {
        pCollider->Render();
    }*/
}

/// <summary>
/// コインの生成（プールから再利用 or 新規作成）
/// </summary>
void Coin::SpawnCoin(VECTOR _pos)
{
#if _DEBUG
    DrawFormatString(0, 10, red, "生成");
#endif
    // 最大数チェック
    int activeCount = std::count_if(coinPool.begin(), coinPool.end(),
        [](const std::unique_ptr<Coin>& c) { return c->active; });

    /* if (activeCount >= maxCoin) return;*/

     // 非アクティブなコインを探して再利用
    for (auto& coin : coinPool)
    {
        if (!coin->active)
        {
            coin->position = _pos;
            coin->startTime = 0;
            coin->active = true;
            return;
        }
    }

    // 見つからなければ新規作成
    auto newCoin = std::make_unique<Coin>(_pos, "Coin", 1, 1, 5000, 0, 10, activeCount);
    newCoin->Start();
    newCoin->active = true;
    coinPool.push_back(std::move(newCoin));
}

/// <summary>
/// ステージ上のコインの削除（非アクティブ化）
/// </summary>
void Coin::RemoveCoin(Coin* target)
{
    coinPool.erase(
        std::remove_if(coinPool.begin(), coinPool.end(),
            [target](const std::unique_ptr<Coin>& coin) {
                if (coin.get() == target) {
                    if (coin->GetCollider()) {
                        CollisionManager::GetInstance().UnRegister(coin->GetCollider());

                    }
                    return true;
                }
                return false;
            }),
        coinPool.end());

}


/// <summary>
/// コインの取得処理
/// </summary>
void Coin::ApplyCoin(Player* player)
{

    if (!player || !active) return;
    player->SetCoinValue(player->GetCoinValue() + player->GetCoinAcquisitionValue() + coinValue);

    player->SetIsCoin(false);


}

/// <summary>
/// 全コイン更新
/// </summary>
void Coin::UpdateAll()
{
    for (auto& coin : coinPool)
    {
        coin->Update();
    }
}

/// <summary>
/// 全コイン描画
/// </summary>
void Coin::RenderAll()
{
    for (auto& coin : coinPool)
    {
        coin->Render();
    }
}