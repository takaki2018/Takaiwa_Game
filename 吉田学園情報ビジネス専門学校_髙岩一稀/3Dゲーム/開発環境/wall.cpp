//-----------------------------------------------------------------
//
// �� (wall.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "wall.h"
#include "input.h"
#include "gamepad.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define WIDTH_NUM				(10)										// ��
#define HEIGHT_NUM				(2)											// �s
#define WIDTH					(100.0f)									// ��
#define HEIGHT					(100.0f)									// ���s
#define WIDTH_VEC				(WIDTH_NUM + 1)								// ��̒��_��
#define HEIGHT_VEC				(HEIGHT_NUM + 1)								// �s�̒��_��

#define MAX_WALL_MESH_VERTEX	(HEIGHT_NUM * 2 * (WIDTH_NUM + 2) - 2)
#define MAX_INDEX				(WIDTH_NUM * HEIGHT_NUM * 2 + (4 * (HEIGHT_NUM - 1)))
#define MAX_WALL_MESH			((WIDTH_NUM + 1) * (HEIGHT_NUM + 1))										// �p�ӂ���|���S���f�[�^�̐�


//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;				// �ǂ̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffWall = NULL;				// �ǂ̃C���f�b�N�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;					// �ǂ̃e�N�X�`���̃|�C���^
Wall g_aWall[MAX_WALL];										// �ǂ̏��

//-----------------------------------------------------------------
// �ǂ̏���������
//-----------------------------------------------------------------
HRESULT InitWall(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].nCntWallX = WIDTH_NUM;
		g_aWall[nCntWall].nCntWallZ = HEIGHT_NUM;
		g_aWall[nCntWall].fWidth = WIDTH;
		g_aWall[nCntWall].fDepth = HEIGHT;
		g_aWall[nCntWall].fsizeWidth = g_aWall[nCntWall].nCntWallX * g_aWall[nCntWall].fWidth;
		g_aWall[nCntWall].fsizeDepth = g_aWall[nCntWall].nCntWallZ * g_aWall[nCntWall].fDepth;
		g_aWall[nCntWall].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field000.jpg", &g_pTextureWall);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_WALL_MESH * MAX_WALL,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		for (int nCntZ = 0; nCntZ < HEIGHT_VEC; nCntZ++)
		{
			for (int nCntX = 0; nCntX < WIDTH_VEC; nCntX++)
			{
				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3((-g_aWall[nCntWall].fsizeWidth / 2.0f + (float)nCntX * g_aWall[nCntWall].fWidth) + g_aWall[nCntWall].pos.x,
					(g_aWall[nCntWall].fsizeDepth - (float)nCntZ * g_aWall[nCntWall].fDepth) + g_aWall[nCntWall].pos.y,
					g_aWall[nCntWall].pos.z);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

				// ���̏��
				pVtx++;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * MAX_WALL_MESH_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,						// �t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pIdxBuffWall,
		NULL)))
	{
		return E_FAIL;
	}

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffWall->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < HEIGHT_NUM; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC + 1; nCntX++)
		{
			if (nCntX != 0 && nCntX == WIDTH_VEC && nCntZ != HEIGHT_NUM - 1)
			{// �E�[����܂�Ԃ���
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntZ == HEIGHT_NUM - 1 && nCntX == WIDTH_VEC)
			{// �I�����ɖ�������
				break;
			}
			else
			{// �ʏ�z�u
				pIdx[0] = WIDTH_VEC + (WIDTH_VEC * nCntZ) + nCntX;
				pIdx[1] = pIdx[0] - WIDTH_VEC;
			}
			// ���̏��Ɉړ�
			pIdx += 2;
		}
	}
	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffWall->Unlock();

	// �ǂ̔z�u
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// ���̕�
	SetWall(D3DXVECTOR3(500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, 0.0f));				// ���̕�
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));				// ���̕�
	SetWall(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));				// ���̕�

	return S_OK;
}

//-----------------------------------------------------------------
// �ǂ̏I������
//-----------------------------------------------------------------
void UninitWall(void)
{
	// �C���f�b�N�X�̊J��
	if (g_pIdxBuffWall != NULL)
	{
		g_pIdxBuffWall->Release();
		g_pIdxBuffWall = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//-----------------------------------------------------------------
// �ǂ̍X�V����
//-----------------------------------------------------------------
void UpdateWall(void)
{

}

//-----------------------------------------------------------------
// �ǂ̕`�揈��
//-----------------------------------------------------------------
void DrawWall(void)
{
	// �ϐ��錾
	Wall *pWall = &g_aWall[0];
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxScaling,mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pWall->mtxWorld);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pWall->rot.y, pWall->rot.x, pWall->rot.z);
			D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pWall->pos.x, pWall->pos.y, pWall->pos.z);
			D3DXMatrixMultiply(&pWall->mtxWorld, &pWall->mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pWall->mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffWall);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureWall);
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_WALL_MESH, 0, MAX_INDEX);
		}
	}
}

//-----------------------------------------------------------------
// �ǂ̔z�u
//-----------------------------------------------------------------
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �\���̂̃|�C���^�ϐ�
	Wall *pWall = &g_aWall[0];

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == false)
		{
			// �ʒu�̐ݒ�
			pWall->pos = pos;

			// �p�x�̐ݒ�
			pWall->rot = rot;

			// �g�p��Ԃ�true�ɂ���
			pWall->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �ǂƂ̓����蔻��
//-----------------------------------------------------------------
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	// �ϐ��錾
	Wall *pWall = &g_aWall[0];
	bool bLand = false;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
#if 0
			// �x�N�g���v�Z
			pWall->aVecA = *pPos - pWall->pos;
			pWall->aVecB = (pWall->aVecA * 1.0f) * 2.0f;

			// �O�ς�p���������蔻��
			pWall->fVec = (pWall->aVecA.x * pWall->aVecB.x) + (pWall->aVecA.z * pWall->aVecB.z);

			if (pWall->fVec > 0.0f)
			{
				if ((pPosOld->x < pWall->pos.x - pWall->fsizeWidth / 2.0f && pPosOld->x > pWall->pos.x + pWall->fsizeWidth / 2.0f) ||
					(pPosOld->z < pWall->pos.z - pWall->fsizeDepth / 2.0f && pPosOld->z > pWall->pos.z + pWall->fsizeDepth / 2.0f))
				{
					if (pPos->x > pPosOld->x && (pPosOld->x < pWall->pos.x))
					{// �v���C���[�������瓖�������Ƃ�
						pPos->x = pWall->pos.x - pvtxMax->x;
					}
					else if (pPos->x < pPosOld->x && (pPosOld->x > pWall->pos.x))
					{// �v���C���[���E���瓖�������Ƃ�
						pPos->x = pWall->pos.x - pvtxMin->x;
					}
					else if (pPos->z > pPosOld->z && pPosOld->z < pWall->pos.z)
					{// �v���C���[����O���瓖�������Ƃ�
						pPos->z = pWall->pos.z - pvtxMax->z;
					}
					else if (pPos->z < pPosOld->z && pPosOld->z > pWall->pos.z)
					{// �v���C���[�������瓖�������Ƃ�
						pPos->z = pWall->pos.z - pvtxMin->z;
					}
				}
			}
#else
			if (pPosOld->x < pWall->pos.x - pWall->fsizeWidth / 2.0f && pPosOld->x > pWall->pos.x + pWall->fsizeWidth / 2.0f &&
				pPosOld->z < pWall->pos.z - pWall->fsizeDepth / 2.0f && pPosOld->z > pWall->pos.z + pWall->fsizeDepth / 2.0f)
			{
				pPos->y = 200.0f;
			}
#endif
		}
	}
}

//-----------------------------------------------------------------
// �Ǐ��̎擾
//-----------------------------------------------------------------
Wall *GetWall(void)
{
	return &g_aWall[0];
}