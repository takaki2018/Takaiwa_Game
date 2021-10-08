//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "sceneX.h"

//-----------------------------------------------------------------------------
// �w�i�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CModel : public CSceneX
{
public:
	// �ǂ������ɉ�]���邩
	typedef enum
	{
		ROTATE_X = 0,	// X��
		ROTATE_Y,		// Y��
		ROTATE_Z,		// Z��
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
	D3DXVECTOR3 m_moveRot;		// ��]�ړ���
	Rotate m_rotate;			// �����𒆐S�ɂ܂�邩
	int m_nCnt;					// �J�E���g
};
#endif