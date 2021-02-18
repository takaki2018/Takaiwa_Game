//-----------------------------------------------------------------
//
// �� (wall.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "billboard.h"
#include "shadow.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_BILLBOARD	(1)		// �p�ӂ���r���{�[�h�f�[�^�̃T�C�Y
#define WALL_SIZE		(50.0f)		// �r���{�[�h�̃T�C�Y

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ǂ̒��S���W
	float fWidth;
	float fHeight;
	int nIdx;					// �e�̔ԍ�
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Billboard;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;				// �ǂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;					// �ǂ̃e�N�X�`���̃|�C���^
Billboard g_aBillboard[MAX_BILLBOARD];							// �r���{�[�h�̏��

//-----------------------------------------------------------------
// �r���{�[�h�̏���������
//-----------------------------------------------------------------
HRESULT InitBillboard(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].fWidth = 100.0f;
		g_aBillboard[nCntBillboard].fHeight = 100.0f ;
		g_aBillboard[nCntBillboard].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field002.jpg", &g_pTextureBillboard);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_BILLBOARD,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x - g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y + g_aBillboard[nCntBillboard].fHeight, g_aBillboard[nCntBillboard].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x + g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y + g_aBillboard[nCntBillboard].fHeight, g_aBillboard[nCntBillboard].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x - g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBillboard[nCntBillboard].pos.x + g_aBillboard[nCntBillboard].fWidth / 2.0f, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���Ԗڂ�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();

	// �r���{�[�h�̐ݒu
	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 0.0f), WALL_SIZE, WALL_SIZE);

	return S_OK;
}

//-----------------------------------------------------------------
// �r���{�[�h�̏I������
//-----------------------------------------------------------------
void UninitBillboard(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//-----------------------------------------------------------------
// �r���{�[�h�̍X�V����
//-----------------------------------------------------------------
void UpdateBillboard(void)
{
	if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{
		g_aBillboard[0].pos.x -= 5.0f;
	}
}

//-----------------------------------------------------------------
// �r���{�[�h�̕`�揈��
//-----------------------------------------------------------------
void DrawBillboard(void)
{
	// �ϐ��錾
	Billboard *pBillboard = &g_aBillboard[0];
	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxScaling,mtxTrans;						// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���C�g���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@��K�p
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pBillboard->mtxWorld);

#if 0
			// �����𔽉f
			D3DXMatrixScaling(&mtxScaling, pBillboard->fWidth, pBillboard->fHeight, 0.0f);
			D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxScaling);
#else
			// �����𔽉f
			//pBillboard->mtxWorld._22 = pBillboard->fHeight;
#endif

			// �r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&pBillboard->mtxWorld, NULL,&mtxView);
			pBillboard->mtxWorld._41 = 0.0f;
			pBillboard->mtxWorld._42 = 0.0f;
			pBillboard->mtxWorld._43 = 0.0f;
#else
			pBillboard->mtxWorld._11 = mtxView._11;
			pBillboard->mtxWorld._12 = mtxView._21;
			pBillboard->mtxWorld._13 = mtxView._31;
			//pBillboard->mtxWorld._21 = mtxView._12;
			//pBillboard->mtxWorld._22 = mtxView._22;
			//pBillboard->mtxWorld._23 = mtxView._32;
			pBillboard->mtxWorld._31 = mtxView._13;
			pBillboard->mtxWorld._32 = mtxView._23;
			pBillboard->mtxWorld._33 = mtxView._33;
#endif
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);
			D3DXMatrixMultiply(&pBillboard->mtxWorld, &pBillboard->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
			
			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureBillboard);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}
	// �W���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// �r���{�[�h�̔z�u
//-----------------------------------------------------------------
void SetBillboard(D3DXVECTOR3 pos,float fWidth,float fDepth)
{
	// �\���̂̃|�C���^�ϐ�
	Billboard *pBillboard = &g_aBillboard[0];

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bUse == false)
		{
			// �ʒu�̐ݒ�
			pBillboard->pos = pos;

			pBillboard->fWidth = fWidth;

			pBillboard->fHeight = fDepth;

			// �g�p��Ԃ�true�ɂ���
			pBillboard->bUse = true;

			break;
		}
	}
}