#include "Coin.h"
#include <algorithm>
#include"../../Manager/CollisionManager.h"
#include"../../Component/Collider/Collider.h"
#include"../Character/Player/Player.h"
#include "../../Manager/AudioManager.h"

/// 静的メンバ変数（プール本体）
std::vector<std::unique_ptr<Coin>> Coin::coinPool;
Coin* Coin::pInstance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
Coin::Coin(VECTOR _pos, std::string tag, int _value, int _coinValue, int _deleteTime, int _startTime,
    int _maxCoin, int _currentCoin)
    : GameObject(_pos, "Coin")
    , value(_value)
    , coinModelHandle(INVALID)
    , coinValue(_coinValue)
    , deleteTime(_deleteTime)
    , startTime(_startTime)
    , maxCoin(_maxCoin)
    , currentCoin(_currentCoin)
    , active(false)
{
 
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
    coinModelHandle = MV1LoadModel("Res/Model/DropObject/coin.mv1");
    coinValue = 1;
    value = coinValue;
    AudioManager::GetInstance().Load("Res/SE/coin.mp3", "GetCoin", false);
    UpdateMatrix(); // 初期行列更新
}

/// <summary>
/// モデル行列更新
/// </summary>
void Coin::UpdateMatrix()
{
    MATRIX rotY = MGetRotY(Deg2Rad(rotation.y));
    MATRIX trans = MGetTrans(position.x, position.y, position.z);
    matrix = MMult(rotY, trans);
    MV1SetMatrix(coinModelHandle, matrix);
}

/// <summary>
/// 更新
/// </summary>
void Coin::Update()
{
    if (!active) return;

    rotation.y += 10.0f;

    // 上下に少し浮かせるアニメーション
    if (position.y <= 6) {
        position.y += 0.1f;
    }
    else if (position.y >= 6) {
        position.y -= 0.1f;
    }

    // 削除時間チェック
    if (startTime >= deleteTime)
    {
        return;
    }

    // 行列更新
    UpdateMatrix();

    // コライダー更新
    if (pCollider != nullptr)
    {
        pCollider->SetMatrix(matrix);
        pCollider->Update();
    }

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

    //// 当たり判定の描画（デバッグ用）
    //if (pCollider != nullptr) {
    //    pCollider->Render();
    //}
}

/// <summary>
/// コインの生成（プールから再利用 or 新規作成）
/// </summary>
void Coin::SpawnCoin(VECTOR _pos)
{
#if _DEBUG
    DrawFormatString(0, 10, red, "生成");
#endif

    // 現在のアクティブなコイン数
    int activeCount = std::count_if(coinPool.begin(), coinPool.end(),
        [](const std::unique_ptr<Coin>& c) { return c->active; });

    // 再利用可能なコインを探す
    for (auto& coin : coinPool)
    {
        if (!coin->active)
        {
            // 位置・状態を初期化
            coin->position = _pos;
            coin->rotation = VGet(0, 0, 0);
            coin->startTime = 0;
            coin->coinValue = GetRand(4) + 1;
            coin->active = true;

            // 行列・コライダー再設定
            coin->UpdateMatrix();
            if (coin->pCollider)
            {
                coin->pCollider->SetEnable(true);
                coin->pCollider->SetMatrix(coin->matrix);
                coin->pCollider->Update();
            }

            return;
        }
    }

    // 非アクティブなものがない場合は新規生成
    auto newCoin = std::make_unique<Coin>(_pos, "Coin", 1, 1, 5000, 0, 10, activeCount);
    newCoin->Start();
    newCoin->active = true;
    coinPool.push_back(std::move(newCoin));
}

/// <summary>
/// コインの取得処理
/// </summary>
void Coin::ApplyCoin(Player* player)
{
    if (!player || !active) return;
    AudioManager::GetInstance().PlayOneShot("GetCoin");
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

/// <summary>
/// コインの削除
/// </summary>
void Coin::ResetAll()
{
    for(auto& coin : coinPool){
        coin->SetActive(false);
        coin->GetCollider()->SetEnable(false);
    }

}
