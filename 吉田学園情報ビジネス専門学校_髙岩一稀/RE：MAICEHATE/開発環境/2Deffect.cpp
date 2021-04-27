//==============================================================================================================
//
// 2D�G�t�F�N�g (2Deffect.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "2Deffect.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_EFFECT	(4096)						// �G�t�F�N�g�̍ő�l

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posOld;		// �O�̍��W
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fRemove;			// �ړ��ʂ̌�����
	float fRandius;			// ���a
	float nCntAlpha;		// ���l�̈ړ�
	float fGravity;			// �d��
	float fBound;			// ���˕Ԃ�W��
	int nLife;				// ����
	int nIdx;				// �e�̊Ǘ��ԍ�
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EFFECT;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff2DEffect = NULL;	// �G�t�F�N�g�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexture2DEffect = NULL;			// �G�t�F�N�g�̒��_�o�b�t�@�̃|�C���^
EFFECT g_a2DEffect[MAX_EFFECT];						// �G�t�F�N�g�̏��

//--------------------------------------------------------------------------------------------------------------
// �G�t�F�N�g�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT Init2DEffect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTexture2DEffect);

	// �����̏��̏�����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_a2DEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_a2DEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_a2DEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_a2DEffect[nCntEffect].fRemove = NULL;
		g_a2DEffect[nCntEffect].fRandius = 1.0f;
		g_a2DEffect[nCntEffect].nCntAlpha = NULL;
		g_a2DEffect[nCntEffect].fGravity = 0.0f;
		g_a2DEffect[nCntEffect].nLife = 0;
		g_a2DEffect[nCntEffect].nIdx = -1;
		g_a2DEffect[nCntEffect].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_EFFECT, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuff2DEffect,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// �e�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuff2DEffect->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �G�t�F�N�g�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void Uninit2DEffect(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuff2DEffect != NULL)
	{
		g_pVtxBuff2DEffect->Release();
		g_pVtxBuff2DEffect = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTexture2DEffect != NULL)
	{
		g_pTexture2DEffect->Release();
		g_pTexture2DEffect = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �G�t�F�N�g�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void Update2DEffect(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	EFFECT *pEffect;
	pEffect = &g_a2DEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuff2DEffect->Lock(0, 0, (void**)&pVtx, 0);

	// �����̍X�V
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// ���W�̕ۑ�
			pEffect->posOld = pEffect->pos;

			// ���S���W�̍X�V
			pEffect->pos.x += pEffect->move.x;
			pEffect->pos.y += pEffect->move.y;
			pEffect->pos.z += pEffect->move.z;

			// �ړ��ʂ̍X�V
			pEffect->move.x *= pEffect->fRemove;
			pEffect->move.y *= pEffect->fRemove;
			pEffect->move.z *= pEffect->fRemove;

			// �d�͂̉��Z
			pEffect->move.y -= pEffect->fGravity;

			// ���l�̉��Z
			pEffect->col.a -= pEffect->nCntAlpha;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - pEffect->fRandius, pEffect->pos.y - pEffect->fRandius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x + pEffect->fRandius, pEffect->pos.y - pEffect->fRandius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x - pEffect->fRandius, pEffect->pos.y + pEffect->fRandius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + pEffect->fRandius, pEffect->pos.y + pEffect->fRandius, 0.0f);

			// �F�̍X�V
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			// �G�t�F�N�g�̍폜
			if (pEffect->col.a <= 0.0f)
			{
				pEffect->bUse = false;
				pEffect->col.a = 1.0f;
			}
		}
		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuff2DEffect->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �G�t�F�N�g�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void Draw2DEffect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuff2DEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �����̕`��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �����G�t�F�N�g���g���Ă���Ƃ�����
		if (g_a2DEffect[nCntEffect].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexture2DEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// �ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����̍������@�̐���
}

//--------------------------------------------------------------------------------------------------------------
// �G�t�F�N�g�̐ݒ�
// ����		�Fpos		- �ʒu���
//			�Fmove		- �ړ���
//			�Fcol		- �F���
//			�FfRemove	- �ړ��ʌ����l
//			�FfRandius	- ���a�T�C�Y
//			�FfCntAlpha	- ���l�����ʃJ�E���^
//			�FfGravity	- �d��
//			�FfBound	- �o�E���h�l
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void Set2DEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove, float fRandius, float fCntAlpha, float fGravity, float fBound)
{
	// �\���̂̃|�C���^�ϐ�
	EFFECT *pEffect;

	// �ϐ��̎󂯓n��
	pEffect = &g_a2DEffect[0];

	// �G�t�F�N�g�̐ݒ�
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			// �ʒu�ݒ�
			pEffect->pos = pos;

			// �ړ��ʂ̐ݒ�
			pEffect->move = move;

			// �F�ݒ�
			pEffect->col = col;

			// �ړ��ʂ̌����l
			pEffect->fRemove = fRemove;

			// ���a�̐ݒ�
			pEffect->fRandius = fRandius;

			// ���l�̈ړ���
			pEffect->nCntAlpha = fCntAlpha;

			// �d�͂̐ݒ�
			pEffect->fGravity = fGravity;

			// ���˕Ԃ�W���̐ݒ�
			pEffect->fBound = fBound;

			// true�ŕԂ�
			pEffect->bUse = true;

			break;
		}
	}
}
