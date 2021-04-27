//==============================================================================================================
//
// �e (shadow.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "shadow.h"
#include "input.h"
#include "gamepad.h"
#include "model_set.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_SHADOW		(256)			// �p�ӂ���e�f�[�^�̐�

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;			// �e�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;					// �e�̃e�N�X�`���̃|�C���^
Shadow g_aShadow[MAX_SHADOW];								// �e�̏��

//--------------------------------------------------------------------------------------------------------------
// �e�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitShadow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	// �\���̗̂v�f�̏�����
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].fWidth = 1.0f;
		g_aShadow[nCntShadow].fDepth = 1.0f;
		g_aShadow[nCntShadow].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_SHADOW,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);
		pVtx[1].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);
		pVtx[2].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);
		pVtx[3].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.9f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̏���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �e�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitShadow(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �e�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateShadow(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// �e�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawShadow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxScaling,mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	Shadow *pShadow = &g_aShadow[0];

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// �����𔽉f
			pShadow->mtxWorld._11 = pShadow->fWidth;
			pShadow->mtxWorld._33 = pShadow->fDepth;

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureShadow);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	// �ʏ�̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//--------------------------------------------------------------------------------------------------------------
// �e�̔z�u
// ����		�Fpos		- �ʒu���
//			�FfWidth	- ��
//			�FfDepth	- ���s
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// �ϐ��錾
	int nIdx = -1;						// ���Ԗڂ̉e���ۑ����邽�߂̕ϐ�
	Shadow *pShadow = &g_aShadow[0];	// �e�̍\���̂̃|�C���^�ϐ�

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if (pShadow->bUse == false)
		{
			// �ʒu�̐ݒ�
			pShadow->pos = pos;

			// ���̐ݒ�
			pShadow->fWidth = fWidth;

			// ���s�̐ݒ�
			pShadow->fDepth = fDepth;

			// �g�p�󋵂�true�ɂ���
			pShadow->bUse = true;

			// ���ԖڂɎg�p���Ă���e�Ȃ̂��ۑ�����
			nIdx = nCntShadow;

			break;
		}
	}

	return nIdx;
}

//--------------------------------------------------------------------------------------------------------------
// �e�̈ʒu
// ����		�FnIdx	- �e�ԍ������炤
//			�Fpos	- �ʒu���
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;
}

//--------------------------------------------------------------------------------------------------------------
// �e�̏���
// ����		�FnIdx	- �e�ԍ������炤
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DeleteShadow(int nIdx)
{	
	// �g�p��Ԃ�false�ɂ���
	g_aShadow[nIdx].bUse = false;
}

//--------------------------------------------------------------------------------------------------------------
// �e���̎擾
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FShadow	- �e���̐擪�A�h���X��n��
//--------------------------------------------------------------------------------------------------------------
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}

