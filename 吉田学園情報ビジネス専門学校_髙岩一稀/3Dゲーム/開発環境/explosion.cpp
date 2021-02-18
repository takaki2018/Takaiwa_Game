//-----------------------------------------------------------------
//
// ���� (explosion.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "explosion.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define VERTEX_CENTER_X (20)												// ���S���W��X���W
#define VERTEX_CENTER_Y (20)												// ���S���W��Y���W

#define MAX_PATTERN     (8)													// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(8)													// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)													// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM       ((1.0f / TEXTURE_X) * pExprosion->nPatternAnim)		// �e�N�X�`���̓���
#define UV_U			(1.0f / TEXTURE_X + PLAY_ANIM)						// U�̒l
#define UV_V			(1.0f / TEXTURE_Y)									// V�̒l

#define MAX_EXPROSION	(256)												// �e�̍ő�l
#define MOVE_BULLET		(10.0f)												// �e�̈ړ���

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	int nCounterAnim;		// �A�j���[�V�����J�E���^�[
	int nPatternAnim;		// �e�N�X�`���p�^�[���̃J�E���g
	bool bUse;				// �g�p���Ă��邩�ǂ���
} Explosion;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// �e�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// �e�̒��_�o�b�t�@�̃|�C���^
Explosion g_aExplosion[MAX_EXPROSION];				// �e�̏��

//-----------------------------------------------------------------
// �����̏���������
//-----------------------------------------------------------------
HRESULT InitExplosion(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	// �����̏��̏�����
	for (int nCntExprosion = 0; nCntExprosion < MAX_EXPROSION; nCntExprosion++)
	{
		g_aExplosion[nCntExprosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExprosion].nCounterAnim = 0;
		g_aExplosion[nCntExprosion].nPatternAnim = 0;
		g_aExplosion[nCntExprosion].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_EXPROSION, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y + VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y + VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y - VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + VERTEX_CENTER_X, g_aExplosion[nCntExplosion].pos.y - VERTEX_CENTER_Y, g_aExplosion[nCntExplosion].pos.z);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// �e�̏������ɂ���
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �����̏I������
//-----------------------------------------------------------------
void UninitExplosion(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//-----------------------------------------------------------------
// �����̍X�V����
//-----------------------------------------------------------------
void UpdateExplosion(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;
	Explosion *pExprosion;
	pExprosion = &g_aExplosion[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// �����̍X�V
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == true)
		{// �J�E���^�[�A�b�v																																																																		
			pExprosion->nCounterAnim += 1;

			// �w��̒l�̂Ƃ�����
			if ((pExprosion->nCounterAnim % 4) == 0)
			{// �A�j���[�V�����p�^�[��No.���X�V
				pExprosion->nPatternAnim = (pExprosion->nPatternAnim + 1) % MAX_PATTERN;
			}

			// �A�j���[�V�����̍X�V
			pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
			pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

			// �����G�t�F�N�g�̍폜
			if (pExprosion->nPatternAnim == 7)
			{
				pExprosion->bUse = false;
				pExprosion->nPatternAnim = 0;
			}
		}
		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//-----------------------------------------------------------------
// �����̕`�揈��
//-----------------------------------------------------------------
void DrawExplosion(void)
{
	// �ϐ��錾
	Explosion *pExplosion = &g_aExplosion[0];
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxScaling, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���C�g���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@��K�p
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pExplosion->mtxWorld);

			// �r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&pBillboard->mtxWorld, NULL, &mtxView);
			pBillboard->mtxWorld._41 = 0.0f;
			pBillboard->mtxWorld._42 = 0.0f;
			pBillboard->mtxWorld._43 = 0.0f;
#else
			pExplosion->mtxWorld._11 = mtxView._11;
			pExplosion->mtxWorld._12 = mtxView._21;
			pExplosion->mtxWorld._13 = mtxView._31;
			pExplosion->mtxWorld._21 = mtxView._12;
			pExplosion->mtxWorld._22 = mtxView._22;
			pExplosion->mtxWorld._23 = mtxView._32;
			pExplosion->mtxWorld._31 = mtxView._13;
			pExplosion->mtxWorld._32 = mtxView._23;
			pExplosion->mtxWorld._33 = mtxView._33;
#endif
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pExplosion->pos.x, pExplosion->pos.y, pExplosion->pos.z);
			D3DXMatrixMultiply(&pExplosion->mtxWorld, &pExplosion->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureExplosion);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
	// �W���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// �����̐ݒ�
//-----------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos)
{
	// �\���̂̃|�C���^�ϐ�
	Explosion *pExprosion = &g_aExplosion[0];

	// �����̐ݒ�
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == false)
		{
			// �ʒu�ݒ�
			pExprosion->pos = pos;

			// true�ŕԂ�
			pExprosion->bUse = true;

			break;
		}
	}
}
