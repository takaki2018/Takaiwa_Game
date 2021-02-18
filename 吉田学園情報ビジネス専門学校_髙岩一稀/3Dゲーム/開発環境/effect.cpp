//-----------------------------------------------------------------
//
// �G�t�F�N�g (effect.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "effect.h"
#include "shadow.h"
#include "meshfield.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_EFFECT	(4096)						// �G�t�F�N�g�̍ő�l

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
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
		g_aEffect[nCntEffect].fRandius = 1.0f;
		g_aEffect[nCntEffect].nCntAlpha = NULL;
		g_aEffect[nCntEffect].fGravity = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nIdx = -1;
		g_aEffect[nCntEffect].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_EFFECT, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRandius, g_aEffect[nCntEffect].pos.z);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	VERTEX_3D *pVtx;
	EFFECT *pEffect;
	pEffect = &g_aEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

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

			// ���b�V���t�B�[���h�Ƃ̓����蔻��
			bool bCollision = CollisionMeshField(&pEffect->pos, &pEffect->posOld);

			// �o�E���h
			if (bCollision == true)
			{
				pEffect->move.y *= -pEffect->fBound;
			}
			// ���l�̉��Z
			pEffect->col.a -= pEffect->nCntAlpha;

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

				// �e������
				DeleteShadow(pEffect->nIdx);
			}
			// �e�̈ړ�
			SetPositionShadow(pEffect->nIdx,D3DXVECTOR3(pEffect->pos.x,0.0f,pEffect->pos.z));
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
	EFFECT *pEffect = &g_aEffect[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxScaling,mtxTrans;						// �v�Z�p�}�g���b�N�X

	// ���C�g���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@��K�p
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �����̕`��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{// �����G�t�F�N�g���g���Ă���Ƃ�����
		if (pEffect->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pEffect->mtxWorld);

			// �r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&pEffect->mtxWorld, NULL, &mtxView);
			pEffect->mtxWorld._41 = 0.0f;
			pEffect->mtxWorld._42 = 0.0f;
			pEffect->mtxWorld._43 = 0.0f;
#else
			pEffect->mtxWorld._11 = mtxView._11 * pEffect->fRandius;
			pEffect->mtxWorld._12 = mtxView._21 * pEffect->fRandius;
			pEffect->mtxWorld._13 = mtxView._31 * pEffect->fRandius;
			pEffect->mtxWorld._21 = mtxView._12 * pEffect->fRandius;
			pEffect->mtxWorld._22 = mtxView._22 * pEffect->fRandius;
			pEffect->mtxWorld._23 = mtxView._32 * pEffect->fRandius;
			pEffect->mtxWorld._31 = mtxView._13 * pEffect->fRandius;
			pEffect->mtxWorld._32 = mtxView._23 * pEffect->fRandius;
			pEffect->mtxWorld._33 = mtxView._33 * pEffect->fRandius;
#endif

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);
			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// �ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����̍������@�̐���

	// �W���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// �G�t�F�N�g�̐ݒ�
//-----------------------------------------------------------------
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRemove,float fRandius, float fCntAlpha, float fGravity, float fBound)
{
	// �\���̂̃|�C���^�ϐ�
	EFFECT *pEffect;

	// �ϐ��̎󂯓n��
	pEffect = &g_aEffect[0];

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

			// �e�̐ݒ�
			pEffect->nIdx = SetShadow(D3DXVECTOR3(pos.x,0.0f,pos.z), pEffect->fRandius, pEffect->fRandius);

			break;
		}
	}
}
