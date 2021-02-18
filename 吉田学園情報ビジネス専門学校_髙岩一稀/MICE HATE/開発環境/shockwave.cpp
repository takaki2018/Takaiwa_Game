//-----------------------------------------------------------------
//
// �Ռ��g (shockwave.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "shockwave.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_SHOCKWAVE	(256)						// �Ռ��g�̍ő�l

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	D3DXVECTOR3 size;		// �T�C�Y
	float fSize;			// �T�C�Y�̔{��
	float fNB;				// �m�b�N�o�b�N�l
	float fNB_MAG;			// �m�b�N�o�b�N�{��
	float nCntAlpha;		// ���l�̈ړ�
	int nCntState;			// ��ԃJ�E���^
	bool bUse;				// �g�p���Ă��邩�ǂ���
} SHOCKWAVE;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShockwave = NULL;	// �Ռ��g�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShockwave = NULL;		// �Ռ��g�̒��_�o�b�t�@�̃|�C���^
SHOCKWAVE g_aShockwave[MAX_SHOCKWAVE];				// �Ռ��g�̏��
float g_fKnockBack;									// �m�b�N�o�b�N�l

//-----------------------------------------------------------------
// �Ռ��g�̏���������
//-----------------------------------------------------------------
HRESULT InitShockwave(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect100.jpg", &g_pTextureShockwave);

	g_fKnockBack = 0.0f;

	// �����̏��̏�����
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++)
	{
		g_aShockwave[nCntShockwave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShockwave[nCntShockwave].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShockwave[nCntShockwave].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
		g_aShockwave[nCntShockwave].fSize = NULL;
		g_aShockwave[nCntShockwave].fNB = NULL;
		g_aShockwave[nCntShockwave].fNB_MAG = NULL;
		g_aShockwave[nCntShockwave].nCntAlpha = NULL;
		g_aShockwave[nCntShockwave].nCntState = NULL;
		g_aShockwave[nCntShockwave].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SHOCKWAVE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffShockwave,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++)
	{
		// ���_���W�̐ݒ�
		SetVertexShockwave(nCntShockwave);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// �e�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShockwave->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �Ռ��g�̏I������
//-----------------------------------------------------------------
void UninitShockwave(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShockwave != NULL)
	{
		g_pVtxBuffShockwave->Release();
		g_pVtxBuffShockwave = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureShockwave != NULL)
	{
		g_pTextureShockwave->Release();
		g_pTextureShockwave = NULL;
	}
}

//-----------------------------------------------------------------
// �Ռ��g�̍X�V����
//-----------------------------------------------------------------
void UpdateShockwave(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	SHOCKWAVE *pShockwave;
	pShockwave = &g_aShockwave[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// �����̍X�V
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++, pShockwave++)
	{
		if (pShockwave->bUse == true)
		{
			// �T�C�Y�̊m��
			pShockwave->size.x += 1.0f * pShockwave->fSize;
			pShockwave->size.y += 1.0f * pShockwave->fSize;

			// �m�b�N�o�b�N�l
			pShockwave->fNB *= pShockwave->fNB_MAG;

			if (pShockwave->fNB > 0.0f)
			{
				g_fKnockBack = pShockwave->fNB;
			}

			// ���_���W�̐ݒ�
			SetVertexShockwave(nCntShockwave);

			// ���l�̉��Z
			pShockwave->col.a -= pShockwave->nCntAlpha;

			// �Ռ��g�̍폜
			if (pShockwave->col.a <= 0.0f)
			{
				pShockwave->bUse = false;
				pShockwave->fNB = 0.0f;
				pShockwave->size = D3DXVECTOR3(0.0f,0.0f,0.0f);
				pShockwave->col.a = 1.0f;
			}
		}
		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShockwave->Unlock();
}

//-----------------------------------------------------------------
// �Ռ��g�̕`�揈��
//-----------------------------------------------------------------
void DrawShockwave(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	// �f�X�e�B�l�[�V�����̍������@�̐���

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffShockwave, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �����̕`��
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++)
	{// �����G�t�F�N�g���g���Ă���Ƃ�����
		if (g_aShockwave[nCntShockwave].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShockwave);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShockwave * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�̍������@�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����̍������@�̐���
}

//-----------------------------------------------------------------
// �Ռ��g�̐ݒ�
//-----------------------------------------------------------------
void SetShockwave(D3DXVECTOR3 pos,D3DXVECTOR3 size, D3DXCOLOR col, float fSize, float fNB, float fNB_MAG, float nCntAlpha)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	SHOCKWAVE *pShockwave;

	// �ϐ��̎󂯓n��
	pShockwave = &g_aShockwave[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// �����̐ݒ�
	for (int nCntShockwave = 0; nCntShockwave < MAX_SHOCKWAVE; nCntShockwave++, pShockwave++)
	{
		if (pShockwave->bUse == false)
		{
			// �ʒu�ݒ�
			pShockwave->pos = pos;

			// �T�C�Y
			pShockwave->size = size;

			// �F�ݒ�
			pShockwave->col = col;

			// �g��T�C�Y�̎w��
			pShockwave->fSize = fSize;

			// ���_���W�̐ݒ�
			SetVertexShockwave(nCntShockwave);

			// �m�b�N�o�b�N�l�̐ݒ�
			pShockwave->fNB = fNB;

			// �m�b�N�o�b�N�{��
			pShockwave->fNB_MAG = fNB_MAG;

			// ���l�̈ړ���
			pShockwave->nCntAlpha = nCntAlpha;

			// true�ŕԂ�
			pShockwave->bUse = true;

			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShockwave->Unlock();
}

//-----------------------------------------------------------------
// �Ռ��g�̒��_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexShockwave(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShockwave->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ��w��
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x - g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y + g_aShockwave[nIdx].size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x - g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y - g_aShockwave[nIdx].size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x + g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y + g_aShockwave[nIdx].size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aShockwave[nIdx].pos.x + g_aShockwave[nIdx].size.x, g_aShockwave[nIdx].pos.y - g_aShockwave[nIdx].size.y, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_aShockwave[nIdx].col;
	pVtx[1].col = g_aShockwave[nIdx].col;
	pVtx[2].col = g_aShockwave[nIdx].col;
	pVtx[3].col = g_aShockwave[nIdx].col;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShockwave->Unlock();
}

//-----------------------------------------------------------------
// �m�b�N�o�b�N�̎w��
//-----------------------------------------------------------------
float ShockwaveMove(void)
{
	return g_fKnockBack;
}