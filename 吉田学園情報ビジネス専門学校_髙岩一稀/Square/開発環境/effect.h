//=============================================================================
//
// �G�t�F�N�g�w�b�_�[ (effect.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "scene2D.h"

//*****************************************************************************
// �G�t�F�N�g�N���X�i�h���N���X�j
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	CEffect(int nPriority);
	~CEffect();

	// �e�N�X�`���̎��
	typedef enum
	{
		EFFECT_TYPE_NORMAL = 0,
		EFFECT_TYPE_EEXPLOSION ,
		EFFECT_TYPE_TILE,
		EFFECT_TYPE_MAX,
	}EFFECT_TYPE;

	static HRESULT Load(void);
	static void Unload(void);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
		float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, EFFECT_TYPE type,int nInterval);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
		float fRemove, D3DXVECTOR2 size, float fCntAlpha, int nLife, int nInterval);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �G�t�F�N�g�̗v�f
	typedef struct
	{
		D3DXVECTOR3 move;		// �ړ���
		D3DXCOLOR col;			// �F
		float fRemove;			// �ړ��ʂ̌�����
		float nCntAlpha;		// ���l�̈ړ�
		int nLife;				// ����
		int nInterval;			// �J�E���g����
	} Effect;

	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_TYPE_MAX];	// �e�N�X�`�����̃|�C���^
	Effect m_effect;										// �G�t�F�N�g���
};

#endif
