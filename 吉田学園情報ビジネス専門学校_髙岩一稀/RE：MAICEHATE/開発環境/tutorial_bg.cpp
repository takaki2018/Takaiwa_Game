//==============================================================================================================
//
// ���[�h��� (loadscreen.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "tutorial_bg.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_TUTORIALBG		(1)					// �`���[�g���A���w�i�̍ő吔		

#define TUTORIALBG_X		(SCREEN_WIDTH / 2)	// �`���[�g���A���w�i��X���W
#define TUTORIALBG_Y		(SCREEN_HEIGHT / 2)	// �`���[�g���A���w�i��Y���W
#define TUTORIALBG_WIDTH	(SCREEN_WIDTH)		// �`���[�g���A���w�i�̕�
#define TUTORIALBG_HIEGHT	(SCREEN_HEIGHT)		// �`���[�g���A���w�i�̍���

#define MOVE_TUTORISLBG_UV	(0.0002f)			// �`���[�g���A���w�i�̈ړ���

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float fWidth;			// ��
	float fHieght;			// ����
}TutorialBg;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialBg = NULL;			// �`���[�g���A���w�i�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorialBg[MAX_TUTORIALBG] = {};	// �`���[�g���A���w�i�̃e�N�X�`���̃|�C���^
TutorialBg g_TutorialBg;										// �`���[�g���A���w�i���
float g_fMoveTutorialBgUV;										// �`���[�g���A���w�i��UV�l�̈ړ���

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���w�i�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTutorialBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �`���[�g���A���w�i���̏�����
	g_TutorialBg.pos = D3DXVECTOR3(TUTORIALBG_X, TUTORIALBG_Y,0.0f);
	g_TutorialBg.fWidth = TUTORIALBG_WIDTH / 2.0f;
	g_TutorialBg.fHieght = TUTORIALBG_HIEGHT / 2.0f;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/loadbg.png", &g_pTextureTutorialBg[0]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TUTORIALBG, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialBg,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TUTORIALBG; nCnt++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_TutorialBg.pos.x - g_TutorialBg.fWidth, g_TutorialBg.pos.y + g_TutorialBg.fHieght, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutorialBg.pos.x - g_TutorialBg.fWidth, g_TutorialBg.pos.y - g_TutorialBg.fHieght, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutorialBg.pos.x + g_TutorialBg.fWidth, g_TutorialBg.pos.y + g_TutorialBg.fHieght, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutorialBg.pos.x + g_TutorialBg.fWidth, g_TutorialBg.pos.y - g_TutorialBg.fHieght, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		SetTexUVTutorialBg();

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialBg->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���w�i�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTutorialBg(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTutorialBg != NULL)
	{
		g_pVtxBuffTutorialBg->Release();
		g_pVtxBuffTutorialBg = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < MAX_TUTORIALBG; nCntTexture++)
	{
		if (g_pTextureTutorialBg[nCntTexture] != NULL)
		{
			g_pTextureTutorialBg[nCntTexture]->Release();
			g_pTextureTutorialBg[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���w�i�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTutorialBg(void)
{
	// �ϐ������Z
	g_fMoveTutorialBgUV -= MOVE_TUTORISLBG_UV;

	// 1.0f�𒴂����Ƃ�����
	if (g_fMoveTutorialBgUV >= 1.0f)
	{
		g_fMoveTutorialBgUV = 0.0f;
	}
	// �e�N�X�`�����W�̐ݒ�
	SetTexUVTutorialBg();
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���w�i�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawTutorialBg(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialBg, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_TUTORIALBG; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureTutorialBg[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �`���[�g���A���w�i�̃e�N�X�`�����W�̐ݒ�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetTexUVTutorialBg(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialBg->Lock(0, 0, (void**)&pVtx, 0);

	// �w�i�̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f - g_fMoveTutorialBgUV, 1.0f + g_fMoveTutorialBgUV);
	pVtx[1].tex = D3DXVECTOR2(0.0f - g_fMoveTutorialBgUV, 0.0f + g_fMoveTutorialBgUV);
	pVtx[2].tex = D3DXVECTOR2(1.0f - g_fMoveTutorialBgUV, 1.0f + g_fMoveTutorialBgUV);
	pVtx[3].tex = D3DXVECTOR2(1.0f - g_fMoveTutorialBgUV, 0.0f + g_fMoveTutorialBgUV);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialBg->Unlock();
}