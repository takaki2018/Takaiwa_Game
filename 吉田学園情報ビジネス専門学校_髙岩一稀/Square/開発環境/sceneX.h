//-------------------------------------------------------------------
//
// モデルヘッダー (model.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "scene.h"

//-------------------------------------------------------------------
// シーンXクラス(派生クラス)
//-------------------------------------------------------------------
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority);
	~CSceneX();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);			
	void Update(void);			
	void Draw(void);			
	void BindXFileName(char *pModelFileName) { m_pModelFileName = pModelFileName; }
	D3DXVECTOR3 GetPosision(void) { return m_pos; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotate(void) { return m_rot; }
	void SetRotate(D3DXVECTOR3 rot);
	LPD3DXBUFFER GetMat(void) { return m_pBuffMat; }
	int GetNumMat(void) { return m_nNumMat; }
	void SetMat(D3DXMATERIAL *pMat, int nCntMat);

private:
	char *m_pModelFileName;				// モデルのファイル名
	LPDIRECT3DTEXTURE9 m_pTexture[8];	// テクスチャへのポインタ
	LPD3DXMESH m_pMesh;					// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアル(材質情報)へのポインタ
	DWORD m_nNumMat;					// マテリアルの数
	D3DXVECTOR3 m_pos;					// 中心座標
	D3DXVECTOR3 m_rot;					// 回転角
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_vtxMinModel;			// 各頂点座標の最小値
	D3DXVECTOR3 m_vtxMaxModel;			// 各頂点座標の最大値
};

#endif