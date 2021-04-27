//==============================================================================================================
//
// �R�C���l���� (coin_sheets.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "coin_sheets.h"
#include "sound.h"

//--------------------------------------------------------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------------------------------------------------------
#define MAX_COINSHEETS				(2)						// ��ʂɓ\��e�N�X�`���̍ő吔

#define COIN_POS_X					(60.0f)					// �R�C���e�N�X�`���̈ʒu(X)
#define COIN_POS_Y					(50.0f)					// �R�C���e�N�X�`���̈ʒu(Y)
#define COIN_SIZE					(40.0f)					// �R�C���e�N�X�`���̃T�C�Y

#define NUMBER_POS_X				(160.0f)				// �����e�N�X�`���̈ʒu(X)
#define NUMBER_POS_Y				(50.0f)					// �����e�N�X�`���̈ʒu(Y)
#define NUMBER_SIZE_X				(20.0f)					// �����e�N�X�`���̕�
#define NUMBER_SIZE_Y				(40.0f)					// �����e�N�X�`���̍���

#define MAX_PATTERN					(10)					// �A�j���[�V�����p�^�[��No.�̍ő吔
#define TEXTURE_X					(10)					// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y					(1)						// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define FLASH_INTERBAL_NUM			(20)			// �^�C�}�[�̓_�ŃC���^�[�o��
#define CLEAR_NUMBER_NUM			(10)			// �^�C�}�[�̓����ԍ�
#define LESS_CLEAR_NUMBER_NUM		(0)				// �^�C�}�[�̕s�����ԍ�

//--------------------------------------------------------------------------------------------------------------
// �񋓌^
//--------------------------------------------------------------------------------------------------------------
typedef enum
{
	COINSHEETS_TEXTURETYPE_COIN = 0,	// �R�C���̃e�N�X�`��
	COINSHEETS_TEXTURETYPE_NUMBER,		// �����̃e�N�X�`��
	COINSHEETS_TEXTURETYPE_MAX,
}COINSHEETS_TEXTURETYPE;

//--------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCoinSheets = NULL;							// �R�C���l���󋵂̒��_�o�b�t�@�̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCoinSheets[COINSHEETS_TEXTURETYPE_MAX] = {};		// �R�C���l���󋵂̃e�N�X�`���̃|�C���^
D3DXVECTOR3 g_posCoinTex;														// �R�C���e�N�X�`���̒��S���W
D3DXVECTOR3 g_posNumTex;														// �e�N�X�`���̒��S���W
D3DXCOLOR g_colNumTex;															// �����e�N�X�`���̐F
int g_nCntFlashNum;																// �����e�N�X�`���̓_�ŃJ�E���g
int g_nCntGetCoin;				

//--------------------------------------------------------------------------------------------------------------
// �R�C���l���󋵂̏���������
// ����		�Fvoid		- �����Ȃ�
// �Ԃ�l	�FHRESULT	- ���_�o�b�t�@�𐶐��ł������ǂ����Ԃ�
//--------------------------------------------------------------------------------------------------------------
HRESULT InitCoinSheets(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// �ϐ��̏�����
	g_posCoinTex = D3DXVECTOR3(COIN_POS_X, COIN_POS_Y, 0.0f);
	g_posNumTex = D3DXVECTOR3(NUMBER_POS_X, NUMBER_POS_Y,0.0f);
	g_colNumTex = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0);
	g_nCntFlashNum = 0;
	g_nCntGetCoin = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/coin.png", &g_pTextureCoinSheets[COINSHEETS_TEXTURETYPE_COIN]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureCoinSheets[COINSHEETS_TEXTURETYPE_NUMBER]);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_PATTERN, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffCoinSheets,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCoinSheets = 0; nCntCoinSheets < MAX_PATTERN; nCntCoinSheets++)
	{
		// ���_���W�̐ݒ�
		SetVertexCoinSheets(nCntCoinSheets);

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

		// ���̍X�V
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoinSheets->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// �R�C���l���󋵂̏I������
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UninitCoinSheets(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffCoinSheets != NULL)
	{
		g_pVtxBuffCoinSheets->Release();
		g_pVtxBuffCoinSheets = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCntTexture = 0; nCntTexture < COINSHEETS_TEXTURETYPE_MAX; nCntTexture++)
	{
		if (g_pTextureCoinSheets[nCntTexture] != NULL)
		{
			g_pTextureCoinSheets[nCntTexture]->Release();
			g_pTextureCoinSheets[nCntTexture] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
// �R�C���l���󋵂̍X�V����
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void UpdateCoinSheets(void)
{
	if (g_nCntGetCoin == 7)
	{
		if (CLEAR_NUMBER_NUM == g_nCntFlashNum % FLASH_INTERBAL_NUM)
		{// �^�C�}�[��_�ł�����
			g_colNumTex.a = 0.0f;
		}
		else if (LESS_CLEAR_NUMBER_NUM == g_nCntFlashNum % FLASH_INTERBAL_NUM)
		{
			g_colNumTex.a = 1.0f;
		}
		// �_�ŃJ�E���^�̃J�E���g
		g_nCntFlashNum++;
	}
	// ���̍X�V
	for (int nCntTex = 0; nCntTex < COINSHEETS_TEXTURETYPE_MAX; nCntTex++)
	{
		SetVertexCoinSheets(nCntTex);
	}
}

//--------------------------------------------------------------------------------------------------------------
// �R�C���l���󋵂̕`�揈��
// ����		�Fvoid	- �����Ȃ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void DrawCoinSheets(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �ϐ��̎󂯓n��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCoinSheets, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCntCoinSheets = 0; nCntCoinSheets < COINSHEETS_TEXTURETYPE_MAX; nCntCoinSheets++)
	{
		pDevice->SetTexture(0, g_pTextureCoinSheets[nCntCoinSheets]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCoinSheets * 4, 2);
	}
}

//--------------------------------------------------------------------------------------------------------------
// ���_���W�̐ݒ�
// ����		�FnIdx	- ���Ԗڂ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetVertexCoinSheets(int nIdx)
{
	switch (nIdx)
	{// �R�C���e�N�X�`���̏��
	case COINSHEETS_TEXTURETYPE_COIN:
		SetCoinTexInfo(nIdx);
		break;
		// �����e�N�X�`���̏��
	case COINSHEETS_TEXTURETYPE_NUMBER:
		SetNumberTexInfo(nIdx);
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------
// �R�C���e�N�X�`���̐ݒu���
// ����		�FnIdx	- ���Ԗڂ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetCoinTexInfo(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posCoinTex.x - COIN_SIZE, g_posCoinTex.y + COIN_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posCoinTex.x - COIN_SIZE, g_posCoinTex.y - COIN_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posCoinTex.x + COIN_SIZE, g_posCoinTex.y + COIN_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posCoinTex.x + COIN_SIZE, g_posCoinTex.y - COIN_SIZE, 0.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoinSheets->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �����e�N�X�`���̐ݒu���
// ����		�FnIdx	- ���Ԗڂ�
// �Ԃ�l	�Fvoid	- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void SetNumberTexInfo(int nIdx)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	// �e�̏������ɂ���
	pVtx += nIdx * 4;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posNumTex.x - NUMBER_SIZE_X, g_posNumTex.y + NUMBER_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posNumTex.x - NUMBER_SIZE_X, g_posNumTex.y - NUMBER_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posNumTex.x + NUMBER_SIZE_X, g_posNumTex.y + NUMBER_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posNumTex.x + NUMBER_SIZE_X, g_posNumTex.y - NUMBER_SIZE_Y, 0.0f);

	// �e�N�X�`���̍X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 1.0f / TEXTURE_Y);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 1.0f / TEXTURE_Y);
	pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_colNumTex;
	pVtx[1].col = g_colNumTex;
	pVtx[2].col = g_colNumTex;
	pVtx[3].col = g_colNumTex;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoinSheets->Unlock();
}

//--------------------------------------------------------------------------------------------------------------
// �R�C���l���󋵂̉��Z
// ����		�FnValue	- ���Z��
// �Ԃ�l	�Fvoid		- �����Ԃ��Ȃ�
//--------------------------------------------------------------------------------------------------------------
void AddCoinSheets(int nValue)
{
	// �X�R�A���i�[
	g_nCntGetCoin += nValue;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCoinSheets->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���̍X�V
	pVtx[4].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 1.0f / TEXTURE_Y);
	pVtx[5].tex = D3DXVECTOR2(0.0f + g_nCntGetCoin * 0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 1.0f / TEXTURE_Y);
	pVtx[7].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_nCntGetCoin* 0.1f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCoinSheets->Unlock();
}