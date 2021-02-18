//-----------------------------------------------------------------
//
// �����N���S (rank_logo.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "rank_logo.h"
#include "gauge.h"
#include "timer.h"
#include "sound.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_TEXTURE			(5)				// �����N���S�̍ő�l

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankLogo = NULL;			// �����N���S�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureRankLogo[MAX_TEXTURE] = {};	// �����N���S�̒��_�o�b�t�@�̃|�C���^
RANKLOGO g_aRankLogo[RANK_MAX];								// �����N���S�̏��

//-----------------------------------------------------------------
// ���U���g���S�̏���������
//-----------------------------------------------------------------
HRESULT InitRankLogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureRankLogo[RANK_1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureRankLogo[RANK_2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureRankLogo[RANK_3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureRankLogo[RANK_4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_5th.png", &g_pTextureRankLogo[RANK_5]);

	// �ϐ��̏�����
	for (int nCntRankLogo = 0; nCntRankLogo < MAX_TEXTURE; nCntRankLogo++)
	{
		g_aRankLogo[nCntRankLogo].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRankLogo[nCntRankLogo].nTexType = RANK_1;
		g_aRankLogo[nCntRankLogo].fWidth = 0.0f;
		g_aRankLogo[nCntRankLogo].fHeight = 0.0f;
		g_aRankLogo[nCntRankLogo].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRankLogo = 0; nCntRankLogo < MAX_TEXTURE; nCntRankLogo++)
	{
		// ���_���W�̍X�V
		SetVErtexRankLogo(nCntRankLogo);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̍X�V
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// �e�N�X�`���̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���Ԗڂ�
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankLogo->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// ���U���g���S�̏I������
//-----------------------------------------------------------------
void UninitRankLogo(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffRankLogo != NULL)
	{
		g_pVtxBuffRankLogo->Release();
		g_pVtxBuffRankLogo = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureRankLogo[nCnt] != NULL)
		{
			g_pTextureRankLogo[nCnt]->Release();
			g_pTextureRankLogo[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// ���U���g���S�̍X�V����
//-----------------------------------------------------------------
void UpdateRankLogo(void)
{
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aRankLogo[nCntResultLogo].bUse == true)
		{
			// ���_���W�̍X�V
			SetVErtexRankLogo(nCntResultLogo);
		}
	}
}

//-----------------------------------------------------------------
// ���U���g���S�̕`�揈��
//-----------------------------------------------------------------
void DrawRankLogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�ƕ`��
	for (int nCntResultLogo = 0; nCntResultLogo < MAX_TEXTURE; nCntResultLogo++)
	{
		if (g_aRankLogo[nCntResultLogo].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureRankLogo[(int)(g_aRankLogo[nCntResultLogo].nTexType)]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultLogo * 4, 2);
		}
	}
}

//-----------------------------------------------------------------
// ���_���W�̐ݒ�
//-----------------------------------------------------------------
void SetVErtexRankLogo(int nCntRankLogo)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ���Ԗڂ�
	pVtx += nCntRankLogo * 4;

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x - (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y + (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x - (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y - (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x + (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y + (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankLogo[nCntRankLogo].pos.x + (g_aRankLogo[nCntRankLogo].fWidth / 2), g_aRankLogo[nCntRankLogo].pos.y - (g_aRankLogo[nCntRankLogo].fHeight / 2), 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankLogo->Unlock();
}

//-----------------------------------------------------------------
// ���S�̐ݒu
//-----------------------------------------------------------------
void SetRankLogo(D3DXVECTOR3 pos, float fWidth,float fHeight, RANK nTexType)
{
	// �\���̂̃|�C���^�ϐ�
	RANKLOGO *pRankLogo;

	// �ϐ��̎󂯓n��
	pRankLogo = &g_aRankLogo[0];

	// �e�̐ݒ�
	for (int nCntRankLogo = 0; nCntRankLogo < MAX_TEXTURE; nCntRankLogo++, pRankLogo++)
	{
		if (pRankLogo->bUse == false)
		{// �ʒu�ݒ�
			pRankLogo->pos = pos;

			// ��
			pRankLogo->fWidth = fWidth;

			// ����
			pRankLogo->fHeight = fHeight;

			// ���̃e�N�X�`�����g����
			pRankLogo->nTexType = nTexType;

			// true�ŕԂ�
			pRankLogo->bUse = true;

			break;
		}
	}
}

//-----------------------------------------------------------------
// �����N���S�̕ύX
//-----------------------------------------------------------------
void CangeRankLogo(int nCntRankLogo, RANK nTexType)
{
	g_aRankLogo[nCntRankLogo].nTexType = nTexType;
}