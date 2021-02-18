//-----------------------------------------------------------------
//
// �|���S�� (polygon.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "polygon.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_POLYGON			(64)			// �p�ӂ���|���S���f�[�^�̐�
#define POLYGON_SIZE		(200.0f)		// �|���S���̃T�C�Y

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;				// �|���S���̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;					// �|���S���̃e�N�X�`���̃|�C���^
Floor g_aFloor[MAX_POLYGON];								// �|���S�����

//-----------------------------------------------------------------
// �|���S���̏���������
//-----------------------------------------------------------------
HRESULT InitPolygon(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		g_aFloor[nCntPolygon].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFloor[nCntPolygon].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aFloor[nCntPolygon].fWidth = 1.0f;
		g_aFloor[nCntPolygon].fDepth = 1.0f;
		g_aFloor[nCntPolygon].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTexturePolygon);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX * MAX_POLYGON,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x - g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z + g_aFloor[nCntPolygon].fDepth / 2.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x + g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z + g_aFloor[nCntPolygon].fDepth / 2.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x - g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z - g_aFloor[nCntPolygon].fDepth / 2.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFloor[nCntPolygon].pos.x + g_aFloor[nCntPolygon].fWidth / 2.0f, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z - g_aFloor[nCntPolygon].fDepth / 2.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		// ���̏���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();

	// ���̔z�u
	for (int nCntPolygon = 0; nCntPolygon < 5; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			SetPolygon(D3DXVECTOR3(-400.0f + POLYGON_SIZE * nCntPolygon, 0.0f, -400.0f + POLYGON_SIZE * nCnt),
				POLYGON_SIZE, POLYGON_SIZE);
		}
	}

	return S_OK;
}

//-----------------------------------------------------------------
// �|���S���̏I������
//-----------------------------------------------------------------
void UninitPolygon(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
}

//-----------------------------------------------------------------
// �|���S���̍X�V����
//-----------------------------------------------------------------
void UpdatePolygon(void)
{

}

//-----------------------------------------------------------------
// �|���S���̕`�揈��
//-----------------------------------------------------------------
void DrawPolygon(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_aFloor[nCntPolygon].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aFloor[nCntPolygon].mtxWorld);

			// �X�P�[���̔��f
			g_aFloor[nCntPolygon].mtxWorld._11 = g_aFloor[nCntPolygon].fWidth;
			g_aFloor[nCntPolygon].mtxWorld._33 = g_aFloor[nCntPolygon].fDepth;

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFloor[nCntPolygon].rot.y, g_aFloor[nCntPolygon].rot.x, g_aFloor[nCntPolygon].rot.z);
			D3DXMatrixMultiply(&g_aFloor[nCntPolygon].mtxWorld, &g_aFloor[nCntPolygon].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aFloor[nCntPolygon].pos.x, g_aFloor[nCntPolygon].pos.y, g_aFloor[nCntPolygon].pos.z);
			D3DXMatrixMultiply(&g_aFloor[nCntPolygon].mtxWorld, &g_aFloor[nCntPolygon].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aFloor[nCntPolygon].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTexturePolygon);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �|���S���̐ݒu
//-----------------------------------------------------------------
void SetPolygon(D3DXVECTOR3 pos,float fWidth,float fDepth)
{
	// �\���̂̃|�C���^�ϐ�
	Floor *pFloor = &g_aFloor[0];

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++,pFloor++)
	{
		if (pFloor->bUse == false)
		{
			// �ʒu�̐ݒ�
			pFloor->pos = pos;

			// ���̐ݒ�
			pFloor->fWidth = fWidth;

			// ���s�̐ݒ�
			pFloor->fDepth = fDepth;

			// �g�p���Ă����Ԃɂ���
			pFloor->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �I�u�W�F�N�g�̓����蔻��
//-----------------------------------------------------------------
bool CollisionFloor(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �\���̂̃|�C���^�ϐ�
	Floor *pFloor = &g_aFloor[0];

	// �ϐ��錾
	bool bGetCoin = false;		// �R�C�����l���������ǂ���

	/*for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++, pFloor++)
	{
		if (pFloor->bUse == true)
		{
			if ((pPos->x - pvtxMin->x) < (pFloor->pos.x + pFloor->vtxMaxCoin.x) &&
				(pPos->x + pvtxMax->x) > (pFloor->pos.x - pFloor->vtxMinCoin.x))
			{// �v���C���[���u���b�N�̒��ɂ���Ƃ�
				if (pPosOld->z + pvtxMax->z <= (pFloor->pos.z - pFloor->vtxMinCoin.z))
				{// �v���C���[���u���b�N�̏�ɂ���Ƃ�
					if (pPos->z + pvtxMax->z >(pFloor->pos.z - pFloor->vtxMinCoin.z))
					{// �߂荞�񂾂珈��
						pPos->z = pFloor->pos.z - pFloor->vtxMinCoin.z - pvtxMax->z;
						bGetCoin = true;
					}
				}
				else if ((pPosOld->z - pvtxMin->z) >= (pFloor->pos.z + pFloor->vtxMaxCoin.z))
				{// �v���C���[���u���b�N�̉��ɂ���Ƃ�
					if ((pPos->z - pvtxMin->z) < (pFloor->pos.z + pFloor->vtxMaxCoin.z))
					{// �߂荞�񂾂�u���b�N�̉��̍��W�ɂ���
						pPos->z = (pFloor->pos.z + pFloor->vtxMaxCoin.z + pvtxMin->z);
						bGetCoin = true;
					}
				}
			}
			if ((pPos->z + pvtxMax->z) > pFloor->pos.z - pFloor->vtxMinCoin.z &&
				(pPos->z - pvtxMin->z) < (pFloor->pos.z + pFloor->vtxMaxCoin.z))
			{// �v���C���[���u���b�N��Y���ɂ���Ƃ�
				if ((pPosOld->x + pvtxMax->x) <= pFloor->pos.x - pFloor->vtxMinCoin.x)
				{// �v���C���[���u���b�N�̍����ɂ���Ƃ�
					if ((pPos->x + pvtxMax->x) > pFloor->pos.x - pFloor->vtxMinCoin.x)
					{// �߂荞�񂾂�u���b�N�̍��̍��W�ɂ���
						pPos->x = pFloor->pos.x - pFloor->vtxMinCoin.x - pvtxMax->x;
						bGetCoin = true;
					}
				}
				else if ((pPosOld->x - pvtxMin->x) >= (pFloor->pos.x + pFloor->vtxMaxCoin.x))
				{// �v���C���[���u���b�N�̉E���ɂ���Ƃ�
					if ((pPos->x - pvtxMin->x) < (pFloor->pos.x + pFloor->vtxMaxCoin.x))
					{// �߂荞�񂾂�u���b�N�̉E�̍��W�ɂ���
						pPos->x = pFloor->pos.x + pFloor->vtxMaxCoin.x + pvtxMin->x;
						bGetCoin = true;
					}
				}
			}
		}
	}
	*/
	return bGetCoin;
}

//-----------------------------------------------------------------
// �|���S�����̎擾
//-----------------------------------------------------------------
Floor *Getpolygon(void)
{
	return &g_aFloor[0];
}