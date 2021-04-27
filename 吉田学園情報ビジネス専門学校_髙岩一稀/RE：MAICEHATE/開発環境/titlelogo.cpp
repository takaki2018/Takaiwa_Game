//==============================================================================================================
//
// �^�C�g�����S (titlelogo.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "titlelogo.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define TITLELOGO_X			(SCREEN_WIDTH / 2)	// �^�C�g�����S��X���W
#define TITLELOGO_Y			(-100.0f)			// �^�C�g�����S�̏������SY���W
#define TITLELOGO_WIDTH		(800.0f)			// �^�C�g�����S��
#define TITLELOGO_HEIGHT	(200.0f)			// �^�C�g�����S�̍���

#define TITLELOGO_Y_DEST	(250.0f)			// �ړI�̍���
#define MOVE_TITLELOGO		(5.0f)				// �^�C�g�����S�̈ړ���

//--------------------------------------------------------------------------------------------------------------
// �\���̂̒�`
//--------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	float fWidth;			// ��
	float fHieght;			// ����
	bool bFall;				// �����؂������ǂ���
}TitleLogo;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;		// �^�C�g�����S�̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;			// �^�C�g�����S�̃e�N�X�`���̃|�C���^
TitleLogo g_TitleLogo;									// �^�C�g�����S���

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S�̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitTitlelogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �^�C�g�����S���̏�����
	g_TitleLogo.pos = D3DXVECTOR3(TITLELOGO_X, TITLELOGO_Y, 0.0f);
	g_TitleLogo.fWidth = TITLELOGO_WIDTH / 2.0f;
	g_TitleLogo.fHieght = TITLELOGO_HEIGHT / 2.0f;
	g_TitleLogo.bFall = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo.png", &g_pTextureTitleLogo);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	SetVertexTitleLogo();

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S�̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitTitlelogo(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S�̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateTitlelogo(void)
{
	// �^�C�g�����S����������O�̏���
	FallBefore();

	// ���_���W�̐ݒ�
	SetVertexTitleLogo();
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S�̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawTitlelogo(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, g_pTextureTitleLogo);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S����������O�̏���
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void FallBefore(void)
{
	// �t�F�[�h���̎擾
	int nFade = GetFade();

	if (GetKeyboardTrigger(KEYINFO_OK) == true || GetJoypadTrigger(PLAYER_1, JPINFO_OKorJUMP) == true)
	{// �G���^�[�L�[���������Ƃ�
		if (nFade == FADE_IN)
		{// �t�F�[�h�C���̎�
			SetFade(FADE_NONE, MODE_TITLE);
			g_TitleLogo.pos.y = TITLELOGO_Y_DEST;
			g_TitleLogo.bFall = true;
		}
		else if (g_TitleLogo.pos.y <= TITLELOGO_Y_DEST)
		{// �^�C�g�����S�������Ă��Ă���Ƃ�
			g_TitleLogo.pos.y = TITLELOGO_Y_DEST;
			g_TitleLogo.bFall = true;
		}
	}
	// �X�V����
	if (nFade == FADE_NONE && g_TitleLogo.pos.y != TITLELOGO_Y_DEST)
	{// �^�C�g�����S�̈ړ��ʂ̍X�V
		g_TitleLogo.pos.y += MOVE_TITLELOGO;
	}
	else if (g_TitleLogo.pos.y >= TITLELOGO_Y_DEST)
	{// �^�C�g�����S�������Ă��Ă���Ƃ�
		g_TitleLogo.pos.y = TITLELOGO_Y_DEST;
		g_TitleLogo.bFall = true;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S�̒��_���W�̐ݒ�
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetVertexTitleLogo(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_TitleLogo.pos.x - g_TitleLogo.fWidth, g_TitleLogo.pos.y + g_TitleLogo.fHieght, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_TitleLogo.pos.x - g_TitleLogo.fWidth, g_TitleLogo.pos.y - g_TitleLogo.fHieght, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_TitleLogo.pos.x + g_TitleLogo.fWidth, g_TitleLogo.pos.y + g_TitleLogo.fHieght, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_TitleLogo.pos.x + g_TitleLogo.fWidth, g_TitleLogo.pos.y - g_TitleLogo.fHieght, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �^�C�g�����S���̎擾
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fbool	- �^�C�g�����S���ړI�̏ꏊ�܂ŗ��������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
bool GetbFall(void)
{
	return g_TitleLogo.bFall;
}