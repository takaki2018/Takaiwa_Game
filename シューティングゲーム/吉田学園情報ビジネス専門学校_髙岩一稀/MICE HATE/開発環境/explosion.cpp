//-----------------------------------------------------------------
//
// �e (bullet.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "explosion.h"
#include "particle.h"

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
	D3DXVECTOR3 pos;		// �ʒu
	D3DCOLOR col;			// �F
	int nCounterAnim;		// �A�j���[�V�����J�E���^�[
	int nPatternAnim;		// �e�N�X�`���p�^�[���̃J�E���g
	bool bUse;				// �g�p���Ă��邩�ǂ���
} EXPLOSION;

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// �e�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// �e�̒��_�o�b�t�@�̃|�C���^
EXPLOSION g_aExplosion[MAX_EXPROSION];				// �e�̏��

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
		g_aExplosion[nCntExprosion].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_aExplosion[nCntExprosion].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_EXPROSION, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++)
	{
		// ���_���W�̐ݒ�
		SetVertexExplosion(nCntExplosion);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

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
	VERTEX_2D *pVtx;
	EXPLOSION *pExprosion;
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

			// ���_���W�̐ݒ�
			SetVertexExplosion(nCntExplosion);

			// �F�̍X�V
			pVtx[0].col = pExprosion->col;
			pVtx[1].col = pExprosion->col;
			pVtx[2].col = pExprosion->col;
			pVtx[3].col = pExprosion->col;

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
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �����̕`��
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++)
	{// �����G�t�F�N�g���g���Ă���Ƃ�����
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// �����̐ݒ�
//-----------------------------------------------------------------
void SetExplosion(D3DXVECTOR3 pos,D3DCOLOR col)
{
	// �\���̂̃|�C���^�ϐ�
	EXPLOSION *pExprosion;

	// �ϐ��̎󂯓n��
	pExprosion = &g_aExplosion[0];

	// �����̐ݒ�
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPROSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == false)
		{
			// �ʒu�ݒ�
			pExprosion->pos = pos;

			// ���_���W�̐ݒ�
			SetVertexExplosion(nCntExplosion);

			// �F�ݒ�
			pExprosion->col = col;

			// true�ŕԂ�
			pExprosion->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �����̒��_���W
//-----------------------------------------------------------------
void SetVertexExplosion(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y + VERTEX_CENTER_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + VERTEX_CENTER_X, g_aExplosion[nIdx].pos.y - VERTEX_CENTER_Y, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}
