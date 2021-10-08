//-------------------------------------------------------------------
//
// カメラヘッダー (camera.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//-------------------------------------------------------------------
// カメラの種類(列挙型)
//-------------------------------------------------------------------
typedef enum
{
	CAMERATYPE_PLAYER_LOOKDOWN = 0,		// プレイヤー見下ろしカメラ
	CAMERATYPE_PLAYER,				// プレイヤー追従カメラ
	CAMERATYPE_MAX,
}CAMERATYPE;

//-------------------------------------------------------------------
// カメラクラス(基本クラス)
//-------------------------------------------------------------------
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);			
	void Uninit(void);		
	void Update(void);		
	void SetCamera(void);	

	D3DXVECTOR3 GetRotate(void) { return m_rot; }

private:
	void LookAtPointRotate(void);	// カメラの注視点回転処理
	void PointRotate(void);			// カメラの視点回転処理
	void LookAtPointMove(void);		// カメラの注視点移動
	void PointMove(void);			// カメラの視点移動

	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 向き
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9 m_viewport;		// ビューポート
};
#endif