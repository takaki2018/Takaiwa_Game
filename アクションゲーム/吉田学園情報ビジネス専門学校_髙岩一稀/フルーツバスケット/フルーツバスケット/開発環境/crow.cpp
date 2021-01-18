//-----------------------------------------------------------------
//
// �J���X (crow.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "crow.h"
#include "sound.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define MAX_CROW			(4)												// �J���X�̍ő吔
#define TEXTURE_X			(4)													// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y			(1)													// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define PLAY_ANIM			((1.0f / TEXTURE_X) * g_aCrow[nCnt].nPatternAnim)	// �e�N�X�`���̓���
#define UV_U				(1.0f / TEXTURE_X + PLAY_ANIM)						// U�̒l
#define UV_V				(1.0f / TEXTURE_Y)									// V�̒l

#define CROW_WIDTH			(20.0f)												// �J���X�̕�
#define CROW_HEIGHT			(20.0f)												// �J���X�̍���

#define ANIMATION_CNT		(10)
#define MOVE_CROW			(2.0f)

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCrow = NULL;	// �w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCrow = NULL;		// �w�i�̃e�N�X�`���̃|�C���^
CROW g_aCrow[MAX_CROW];							// �J���X�̏��
int g_nRandSetCnt;								// �����_���Ŕz�u����J�E���g
int g_nSetCrowCnt;								// �J���X�̐ݒu�J�E���g

//-----------------------------------------------------------------
// �J���X�̏���������
//-----------------------------------------------------------------
HRESULT InitCrow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n���A������
	pDevice = GetDevice();
	g_nRandSetCnt = rand() % 300 + 1000;
	g_nSetCrowCnt = 0;

	// ���̏���������
	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++)
	{
		g_aCrow[nCntCrow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCrow[nCntCrow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCrow[nCntCrow].fWidth = CROW_WIDTH / 2.0f;
		g_aCrow[nCntCrow].fHeight = CROW_HEIGHT / 2.0f;
		g_aCrow[nCntCrow].nCntAnim = 0;
		g_aCrow[nCntCrow].nPatternAnim = 0;
		g_aCrow[nCntCrow].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/crow.png", &g_pTextureCrow);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_CROW,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCrow,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCrow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++)
	{
		// ���_���W�̐ݒ�
		SetVertexCrow(nCntCrow);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCrow->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �J���X�̏I������
//-----------------------------------------------------------------
void UninitCrow(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCrow != NULL)
	{
		g_pVtxBuffCrow->Release();
		g_pVtxBuffCrow = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureCrow != NULL)
	{
		g_pTextureCrow->Release();
		g_pTextureCrow = NULL;
	}
}

//-----------------------------------------------------------------
// �J���X�̍X�V����
//-----------------------------------------------------------------
void UpdateCrow(void)
{
	// �\���̂̃|�C���^�ϐ�
	CROW *pCrow = &g_aCrow[0];

	// �ݒu�J�E���g�̃J�E���g
	g_nSetCrowCnt++;

	// ����؂��Ƃ��ݒu
	if (g_nRandSetCnt == g_nSetCrowCnt)
	{// �J���X�̐ݒu
		SetCrow(D3DXVECTOR3(SCREEN_WIDTH, (float)(rand() % 200 + 200), 0.0f),
			D3DXVECTOR3(-MOVE_CROW, 0.0f, 0.0f));

		// �T�E���h
		PlaySound(SOUND_LABEL_SE_CROW);

		// �ϐ��̏�����
		g_nRandSetCnt = rand() % 300 + 1000;
		g_nSetCrowCnt = 0;
	}

	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++, pCrow++)
	{
		if (pCrow->bUse == true)
		{
			// �ʒu�̍X�V
			pCrow->pos += pCrow->move;

			// ���_���W�̍X�V
			SetVertexCrow(nCntCrow);

			// ��ʊO�ɍs�����Ƃ�
			if (pCrow->pos.x < 0.0f)
			{
				pCrow->bUse = false;
			}
		}
	}
}

//-----------------------------------------------------------------
// �J���X�̕`�揈��
//-----------------------------------------------------------------
void DrawCrow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCrow, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++)
	{
		if (g_aCrow[nCntCrow].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureCrow);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCrow * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVertexCrow(int nCnt)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCrow->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCnt * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x - g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y + g_aCrow[nCnt].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x - g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y - g_aCrow[nCnt].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x + g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y + g_aCrow[nCnt].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aCrow[nCnt].pos.x + g_aCrow[nCnt].fWidth, g_aCrow[nCnt].pos.y - g_aCrow[nCnt].fHeight, 0.0f);

	// �A�j���[�V�����J�E���^�[�̃J�E���g
	g_aCrow[nCnt].nCntAnim++;

	if (0 == g_aCrow[nCnt].nCntAnim % ANIMATION_CNT)
	{// ����؂��Ƃ�����
		g_aCrow[nCnt].nPatternAnim = (g_aCrow[nCnt].nPatternAnim + 1) % TEXTURE_X;
	}
	// �A�j���[�V�����̍X�V
	pVtx[0].tex = D3DXVECTOR2(PLAY_ANIM, UV_V);
	pVtx[1].tex = D3DXVECTOR2(PLAY_ANIM, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(UV_U, UV_V);
	pVtx[3].tex = D3DXVECTOR2(UV_U, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCrow->Unlock();
}

//-----------------------------------------------------------------
// �J���X�̐ݒu
//-----------------------------------------------------------------
void SetCrow(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	// �\���̂̃|�C���^�ϐ�
	CROW *pCrow = &g_aCrow[0];

	// �J���X�̔z�u
	for (int nCntCrow = 0; nCntCrow < MAX_CROW; nCntCrow++, pCrow++)
	{
		if (pCrow->bUse == false)
		{
			// �ʒu�̐ݒ�
			pCrow->pos = pos;

			// �ړ��ʂ̐ݒ�
			pCrow->move = move;

			// �g�p��Ԃ�true�ɂ���
			pCrow->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �J���X���̎擾
//-----------------------------------------------------------------
CROW *GetCrow(void)
{
	return &g_aCrow[0];
}