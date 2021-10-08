//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "sceneX.h"

//-----------------------------------------------------------------------------
// 背景クラス(基本クラス)
//-----------------------------------------------------------------------------
class CModel : public CSceneX
{
public:
	// どこを軸に回転するか
	typedef enum
	{
		ROTATE_X = 0,	// X軸
		ROTATE_Y,		// Y軸
		ROTATE_Z,		// Z軸
		ROTATE_MAX,
	}Rotate;

	CModel(int nPriority);
	~CModel();
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,char *pModelFileName);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddMoveRotate(D3DXVECTOR3 rot);

private:
	D3DXVECTOR3 m_moveRot;		// 回転移動量
	Rotate m_rotate;			// 何軸を中心にまわるか
	int m_nCnt;					// カウント
};
#endif