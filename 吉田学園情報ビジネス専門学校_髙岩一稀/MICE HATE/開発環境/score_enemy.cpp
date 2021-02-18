//-----------------------------------------------------------------
//
// �G�X�R�A (score_enemy.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "score_enemy.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define SCORE_X			(45.0f)				// ��X���W
#define SCORE_Y			(80.0f)				// ��Y���W
#define WIDTH_X			(40.0f)				// �X�R�A�̕�

#define MAX_PATTERN     (10)				// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(10)				// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)					// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define MAX_SCORE		(256)				// �X�R�A�e�N�X�`���̍ő�l
#define MAX_NUM			(8)					// UI�ɕ\������X�R�A�̌���

//-----------------------------------------------------------------
// �\���̂̐錾
//-----------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���S���W
	D3DXCOLOR col;		// ���_�J���[
	int nScore;			// �X�R�A
	bool bUse;			// �g�p����
}SCORE;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreEnemy = NULL;	// �X�R�A�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScoreEnemy = NULL;		// �X�R�A�̒��_�o�b�t�@�̃|�C���^
int g_nScoreEnemy;
SCORE g_aScoreEnemy[MAX_SCORE];						// �X�R�A

//-----------------------------------------------------------------
// �X�R�A�̏���������
//-----------------------------------------------------------------
HRESULT InitScoreEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScoreEnemy);

	// �ϐ��̏�����
	g_nScoreEnemy = 0;

	// �X�R�A���̏�����
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aScoreEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aScoreEnemy[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aScoreEnemy[nCnt].nScore = 0;
		g_aScoreEnemy[nCnt].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		// ���_���W�̐ݒ�
		SetVertexScoreEnemy(nCntScore);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreEnemy->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �X�R�A�̏I������
//-----------------------------------------------------------------
void UninitScoreEnemy(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScoreEnemy != NULL)
	{
		g_pVtxBuffScoreEnemy->Release();
		g_pVtxBuffScoreEnemy = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureScoreEnemy != NULL)
	{
		g_pTextureScoreEnemy->Release();
		g_pTextureScoreEnemy = NULL;
	}
}

//-----------------------------------------------------------------
// �X�R�A�̍X�V����
//-----------------------------------------------------------------
void UpdateScoreEnemy(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	SCORE *pScoreEnemy;
	pScoreEnemy = &g_aScoreEnemy[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += MAX_NUM * 4;
	// �X�R�A�̍X�V
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pScoreEnemy++)
	{
		if (pScoreEnemy->bUse == true)
		{// ���_���W�̍X�V
			SetVertexScoreEnemy(nCntScore);
		}
		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreEnemy->Unlock();
}

//-----------------------------------------------------------------
// �X�R�A�̕`�揈��
//-----------------------------------------------------------------
void DrawScoreEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScoreEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		if (g_aScoreEnemy[nCntScore].bUse == true)
		{// �X�R�A�̕\��
			pDevice->SetTexture(0, g_pTextureScoreEnemy);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �X�R�A�̐ݒu
//-----------------------------------------------------------------
void SetScoreEnemy(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;
	SCORE *pScoreEnemy;

	// �ϐ��̎󂯓n��
	pScoreEnemy = &g_aScoreEnemy[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// �����̐ݒ�
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pScoreEnemy++)
	{
		if (pScoreEnemy->bUse == false)
		{
			// �ʒu�ݒ�
			pScoreEnemy->pos = pos;

			// �F�ݒ�
			pScoreEnemy->col = col;

			// true�ŕԂ�
			pScoreEnemy->bUse = true;

			break;
		}
		// ���̏��ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreEnemy->Unlock();
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexScoreEnemy(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x - (40.0f / 2), g_aScoreEnemy[nCnt].pos.y + (40.0f / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x - (40.0f / 2), g_aScoreEnemy[nCnt].pos.y - (40.0f / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x + (40.0f / 2), g_aScoreEnemy[nCnt].pos.y + (40.0f / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aScoreEnemy[nCnt].pos.x + (40.0f / 2), g_aScoreEnemy[nCnt].pos.y - (40.0f / 2), 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_aScoreEnemy[nCnt].col;
	pVtx[1].col = g_aScoreEnemy[nCnt].col;
	pVtx[2].col = g_aScoreEnemy[nCnt].col;
	pVtx[3].col = g_aScoreEnemy[nCnt].col;

	// ���_�o�b�t�@���A�����b �N����
	g_pVtxBuffScoreEnemy->Unlock();
}