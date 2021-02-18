//-------------------------------------------------------------------
//
// �v���C���[�}�[�J�[ (player_marker.cpp)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#include "player_marker.h"
#include "player.h"
#include "selectmode.h"
#include "bear.h"
#include "gamepad.h"

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define MAX_PLAYER_MARKER		(4)						// �v���C���[�}�[�J�[�̍ő�l

#define PLAYER_MARKER_SIZE_X	(15.0f)					// �v���C���[�}�[�J�[�̃T�C�Y(��)
#define PLAYER_MARKER_SIZE_Y	(15.0f)					// �v���C���[�}�[�J�[�̃T�C�Y(����)

#define PLAYER_MARKER_HEIGHT	(70.0f)					// �v���C���[�}�[�J�[�̈ʒu����

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerMarker = NULL;				// �v���C���[�}�[�J�[�̃e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayerMarker[MAX_PLAYER_MARKER] = {};	// �v���C���[�}�[�J�[�̒��_�o�b�t�@�̃|�C���^
PLAYER_MARKER g_aPlayerMarker[MAX_PLAYER_MARKER];					// �v���C���[�}�[�J�[�̏��
int g_nCntPlayerMarker;												// �z�u����v���C���[�}�[�J�[�̐�

//-----------------------------------------------------------------
// �G�̏���������
//-----------------------------------------------------------------
HRESULT InitPlayerMarker(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	PLAYER *pPlayer;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();
	pPlayer = GetPlayer();
	PLAYMODE playmode = GetPlayMode();

	switch (playmode)
	{// �V���O�����[�h�̎�����
	case PLAYMODE_SINGLE:
		g_nCntPlayerMarker = 1;
		break;
		// �}���`���[�h�̎�����
	case PLAYMODE_MULTI:
		g_nCntPlayerMarker = GetnCntController();
		break;
	}

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < g_nCntPlayerMarker; nCnt++,pPlayer++)
	{
		g_aPlayerMarker[nCnt].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y - PLAYER_MARKER_HEIGHT, 0.0f);
		g_aPlayerMarker[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayerMarker[nCnt].type = PLAYER_MARKER_TYPE_ONE;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_1.png", &g_pTexturePlayerMarker[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_2.png", &g_pTexturePlayerMarker[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_3.png", &g_pTexturePlayerMarker[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player_4.png", &g_pTexturePlayerMarker[3]);


	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * g_nCntPlayerMarker, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerMarker,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerMarker->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_nCntPlayerMarker; nCnt++)
	{
		// ���_���W�̐ݒ�
		SetVertexPlayerMarker(nCnt);

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

		// ���̏��ɍX�V
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayerMarker->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �G�̏I������
//-----------------------------------------------------------------
void UninitPlayerMarker(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffPlayerMarker != NULL)
	{
		g_pVtxBuffPlayerMarker->Release();
		g_pVtxBuffPlayerMarker = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_PLAYER_MARKER; nCnt++)
	{
		if (g_pTexturePlayerMarker[nCnt] != NULL)
		{
			g_pTexturePlayerMarker[nCnt]->Release();
			g_pTexturePlayerMarker[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// �G�̍X�V����
//-----------------------------------------------------------------
void UpdatePlayerMarker(void)
{
	// �\���̂̃|�C���^�ϐ�
	PLAYER_MARKER *pPlayerMarker = &g_aPlayerMarker[0];
	PLAYER *pPlayer = GetPlayer();

	for (int nCntPlayerMarker = 0; nCntPlayerMarker < g_nCntPlayerMarker; nCntPlayerMarker++, pPlayerMarker++,pPlayer++)
	{
		// �ʒu�̍X�V
		pPlayerMarker->pos.x = pPlayer->pos.x;
		pPlayerMarker->pos.y = pPlayer->pos.y - PLAYER_MARKER_HEIGHT;

		// ���_���W�̍X�V
		SetVertexPlayerMarker(nCntPlayerMarker);
	}
}

//-----------------------------------------------------------------
// �G�̕`�揈��
//-----------------------------------------------------------------
void DrawPlayerMarker(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayerMarker, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �v���C���[�}�[�J�[�̕`��
	for (int nCnt = 0; nCnt < g_nCntPlayerMarker; nCnt++)
	{
		pDevice->SetTexture(0, g_pTexturePlayerMarker[nCnt]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//-----------------------------------------------------------------
// �G�̒��_���W
//-----------------------------------------------------------------
void SetVertexPlayerMarker(int nIdx)
{
	// �ϐ��錾
	float ShaikingMove = GetShaking();

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerMarker->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x - PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y + PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x - PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y - PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x + PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y + PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayerMarker[nIdx].pos.x + PLAYER_MARKER_SIZE_X, g_aPlayerMarker[nIdx].pos.y - PLAYER_MARKER_SIZE_Y + ShaikingMove, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayerMarker->Unlock();
}