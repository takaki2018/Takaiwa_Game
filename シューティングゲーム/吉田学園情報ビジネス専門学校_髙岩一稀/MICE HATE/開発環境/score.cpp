//-----------------------------------------------------------------
//
// �X�R�A (score.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "Score.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define SCORE_X			(970.0f)				// ���S���W(X)
#define SCORE_Y			(50.0f)					// ���S���W(Y)

#define SCORE_WIDTH		(45.0f)					// �X�R�A�̕�
#define SCORE_HEIGHT	(80.0f)					// �X�R�A�̍���
#define WIDTH_X			(40.0f)					// �X�R�A��z�u����Ԋu

#define MAX_PATTERN     (10)					// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X		(10)					// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)						// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// �X�R�A�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;		// �X�R�A�̒��_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posScore;							// �X�R�A�̒��S���W
int g_nScore;

//-----------------------------------------------------------------
// �X�R�A�̏���������
//-----------------------------------------------------------------
HRESULT InitScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	// �ϐ��̏�����
	g_posScore = D3DXVECTOR3(SCORE_X, SCORE_Y, 0.0f);
	g_nScore = 0;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_SCORE, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// ������
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y + (SCORE_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x - (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y - (SCORE_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y + (SCORE_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (SCORE_WIDTH / 2) + WIDTH_X * nCntScore, g_posScore.y - (SCORE_HEIGHT / 2), 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �X�R�A�̏I������
//-----------------------------------------------------------------
void UninitScore(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//-----------------------------------------------------------------
// �X�R�A�̍X�V����
//-----------------------------------------------------------------
void UpdateScore(void)
{

}

//-----------------------------------------------------------------
// �X�R�A�̕`�揈��
//-----------------------------------------------------------------
void DrawScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		pDevice->SetTexture(0, g_pTextureScore);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//-----------------------------------------------------------------
// �X�R�A�̉��Z����
//-----------------------------------------------------------------
void AddScore(int nValue)
{
	// �ϐ��錾
	int aNumber[MAX_SCORE];			// �X�R�A���i�[
	g_nScore += nValue;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	
	// �X�R�A���i�[
	for (int nCntScore = 0; nCntScore <	MAX_SCORE; nCntScore++)
	{// ��������i�[
		aNumber[nCntScore] = (g_nScore / (int)powf(10.0f, (float)(MAX_SCORE - (nCntScore + 1))));

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + aNumber[nCntScore] * 0.1f, 0.0f);

		// �X�R�A�̏������ɂ���
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//-----------------------------------------------------------------
// �X�R�A�̎擾
//-----------------------------------------------------------------
int GetScore(void)
{
	return g_nScore;
}