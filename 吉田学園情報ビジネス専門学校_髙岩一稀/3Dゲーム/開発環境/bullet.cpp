//-----------------------------------------------------------------
//
// �e (bullet.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "bullet.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "setparticle.h"
#include "explosion.h"
#include "input.h"
#include "gamepad.h"
#include "Xinput_gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_BULLET	(256)	// �p�ӂ���e�f�[�^�̃T�C�Y
#define BULLET_SIZE	(5.0f)	// �e�̃T�C�Y
#define BULLET_MOVE	(10.0f)	// �e�̈ړ���

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ǂ̒��S���W
	D3DXVECTOR3 rot;			// �ǂ̒��S���W
	D3DXVECTOR3 move;			// �ړ���
	float fWidth;				// ��
	float fHeight;				// ����
	int nIdx;					// �e�̔ԍ�
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Bullet;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;			// �ǂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;				// �ǂ̃e�N�X�`���̃|�C���^
Bullet g_aBullet[MAX_BULLET];							// �r���{�[�h�̏��

//-----------------------------------------------------------------
// �e�̏���������
//-----------------------------------------------------------------
HRESULT InitBullet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	for (int nCntBillboard = 0; nCntBillboard < MAX_BULLET; nCntBillboard++)
	{
		g_aBullet[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBillboard].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBillboard].fWidth = BULLET_SIZE;
		g_aBullet[nCntBillboard].fHeight = BULLET_SIZE;
		g_aBullet[nCntBillboard].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_BULLET,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ���_���W�̐ݒ�
		SetVertexBullet(nCntBullet);

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
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �e�̏I������
//-----------------------------------------------------------------
void UninitBullet(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
}

//-----------------------------------------------------------------
// �e�̍X�V����
//-----------------------------------------------------------------
void UpdateBullet(void)
{
	// �\���̂̃|�C���^�ϐ�
	Bullet *pBullet = &g_aBullet[0];
	Player *pPlayer = GetPlayer();
	XinputGamepad *pXinput = GetXinputGamepad();

	/*if (GetKeyboardTrigger(KEYINFO_SHOOT) == true || pXinput->bPressA == true)
	{
		// �e�̔���
		SetBullet(D3DXVECTOR3(pPlayer->pos.x , pPlayer->pos.y + 28.0f, pPlayer->pos.z),
			D3DXVECTOR3(sinf(pPlayer->rot.y) * -BULLET_MOVE,0.0f, cosf(pPlayer->rot.y) * -BULLET_MOVE),
			BULLET_SIZE,
			BULLET_SIZE);
	}*/

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{// �ʒu�X�V
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.z += pBullet->move.z;

			// �e�̋O��
			//SetOrbitBullet(pBullet->pos);

			// �e�̈ړ�
			SetPositionShadow(pBullet->nIdx, D3DXVECTOR3(pBullet->pos.x, 0.0f, pBullet->pos.z));

			// ��ʊO�`�F�b�N
			if (pBullet->pos.z <= -600.0f || pBullet->pos.z >= 600.0f ||
				pBullet->pos.x <= -600.0f || pBullet->pos.x >= 600.0f)
			{// �e�̏�Ԃ�false�ɂ���
				pBullet->bUse = false;

				// �����A�j���[�V�����̔z�u
				SetExplosion(pBullet->pos);

				// �e���Ȃ���
				DeleteShadow(pBullet->nIdx);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------
// �e�̕`�揈��
//-----------------------------------------------------------------
void DrawBullet(void)
{
	// �ϐ��錾
	Bullet *pBullet = &g_aBullet[0];
	LPDIRECT3DDEVICE9 pDevice;					// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxView;							// �r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���C�g���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pBullet->mtxWorld);

			// �r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 0
			// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&pBillboard->mtxWorld, NULL, &mtxView);
			pBillboard->mtxWorld._41 = 0.0f;
			pBillboard->mtxWorld._42 = 0.0f;
			pBillboard->mtxWorld._43 = 0.0f;
#else
			pBullet->mtxWorld._11 = mtxView._11;
			pBullet->mtxWorld._12 = mtxView._21;
			pBullet->mtxWorld._13 = mtxView._31;
			pBullet->mtxWorld._21 = mtxView._12;
			pBullet->mtxWorld._22 = mtxView._22;
			pBullet->mtxWorld._23 = mtxView._32;
			pBullet->mtxWorld._31 = mtxView._13;
			pBullet->mtxWorld._32 = mtxView._23;
			pBullet->mtxWorld._33 = mtxView._33;
#endif

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pBullet->pos.x, pBullet->pos.y, pBullet->pos.z);
			D3DXMatrixMultiply(&pBullet->mtxWorld, &pBullet->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pBullet->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
	// �A���t�@�e�X�g�����̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	// ���C�g���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------
// �e�̔z�u
//-----------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight)
{
	// �\���̂̃|�C���^�ϐ�
	Bullet *pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			// �ʒu�̐ݒ�
			pBullet->pos = pos;

			// �ړ��ʂ̐ݒ�
			pBullet->move = move;

			// ���̐ݒ�
			pBullet->fWidth = fWidth;

			// �����̐ݒ�
			pBullet->fHeight = fHeight;

			// �g�p��Ԃ�true�ɂ���
			pBullet->bUse = true;

			// �e��t����
			pBullet->nIdx = SetShadow(D3DXVECTOR3(pos.x,0.0f,pos.z),BULLET_SIZE, BULLET_SIZE);

			break;
		}
	}
}

//-----------------------------------------------------------------
// �e�̒��_���W	
//-----------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y + g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y + g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y - g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + g_aBullet[nIdx].fWidth, g_aBullet[nIdx].pos.y - g_aBullet[nIdx].fHeight, g_aBullet[nIdx].pos.z);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}