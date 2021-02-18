//-----------------------------------------------------------------
//
// �G�t�F�N�g (effect.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "effect.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_EFFECT	(8192)						// �G�t�F�N�g�̍ő�l

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	float fRemove;			// �ړ��ʂ̌�����
	float fRandius;			// ���a
	float nCntAlpha;		// ���l�̈ړ�
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EFFECT;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// �G�t�F�N�g�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			// �G�t�F�N�g�̒��_�o�b�t�@�̃|�C���^
EFFECT g_aEffect[MAX_EFFECT];						// �G�t�F�N�g�̏��

//-----------------------------------------------------------------
// �G�t�F�N�g�̏���������
//-----------------------------------------------------------------
HRESULT InitEffect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	// �����̏��̏�����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aEffect[nCntEffect].fRemove = NULL;
		g_aEffect[nCntEffect].fRandius = NULL;
		g_aEffect[nCntEffect].nCntAlpha = NULL;
		g_aEffect[nCntEffect].nLife = NULL;
		g_aEffect[nCntEffect].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_EFFECT, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// �e�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �G�t�F�N�g�̏I������
//-----------------------------------------------------------------
void UninitEffect(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//-----------------------------------------------------------------
// �G�t�F�N�g�̍X�V����
//-----------------------------------------------------------------
void UpdateEffect(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// �����̍X�V
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{// ���S���W�̍X�V
			pEffect->pos.x += pEffect->move.x;
			pEffect->pos.y += pEffect->move.y;

			// �ړ��ʂ̍X�V
			pEffect->move.x *= pEffect->fRemove;
			pEffect->move.y *= pEffect->fRemove;
			
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - pEffect->fRandius, pEffect->pos.y + pEffect->fRandius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - pEffect->fRandius, pEffect->pos.y - pEffect->fRandius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x + pEffect->fRandius, pEffect->pos.y + pEffect->fRandius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x + pEffect->fRandius, pEffect->pos.y - pEffect->fRandius, 0.0f);

			// ���l�̉��Z
 			pEffect->col.a -= pEffect->nCntAlpha;

			// �����̍X�V
			pEffect->nLife--;

			// �F�̍X�V
			pVtx[0].col = pEffect->col;
		  	pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;  
			pVtx[3].col = pEffect->col;

			// �����G�t�F�N�g�̍폜
			if (pEffect->nLife <= 0)
			{
				pEffect->bUse = false;
				pEffect->col.a = 1.0f;
				pEffect->nLife = 0;
			}
		}
		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//-----------------------------------------------------------------
// �G�t�F�N�g�̕`�揈��
//-----------------------------------------------------------------
void DrawEffect(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �����̕`��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �����G�t�F�N�g���g���Ă���Ƃ�����
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// �ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-----------------------------------------------------------------
// �G�t�F�N�g�̐ݒ�
//-----------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove,float fRandius, float fCntAlpha, int nLife)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	EFFECT *pEffect;

	// �ϐ��̎󂯓n��
	pEffect = &g_aEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

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

			// �����̐ݒ�
			pEffect->nLife = nLife;

			// true�ŕԂ�
			pEffect->bUse = true;

			break;
		}
	}
	// ���_�o�b�t�@���A�����b �N����
	g_pVtxBuffEffect->Unlock();
}
