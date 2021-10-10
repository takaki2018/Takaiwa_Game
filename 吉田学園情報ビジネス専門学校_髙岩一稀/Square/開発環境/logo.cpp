//=============================================================================
//
// ���S���� [logo.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "logo.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "fade.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLogo::m_pTexture[TEXTURETYPE_MAX] = {};	// �e�N�X�`�����̃|�C���^

//=============================================================================
// CLogo�̃R���X�g���N�^
//=============================================================================
CLogo::CLogo()
{
	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_nTextureNumber = -1;
	m_bBlinking = false;
	m_nBlimkingInterval = 0;
	m_nBlinkingCounter = 0;
}

//=============================================================================
// CLogo�̃f�X�g���N�^
//=============================================================================
CLogo::~CLogo()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h����
//=============================================================================
HRESULT CLogo::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo.png", &m_pTexture[TEXTURETYPE_TITLELOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_play.png", &m_pTexture[TEXTURETYPE_PLAY]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_tutorial.png", &m_pTexture[TEXTURETYPE_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_ranking.png", &m_pTexture[TEXTURETYPE_RANKING]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_option.png", &m_pTexture[TEXTURETYPE_OPTION]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/score_logo.png", &m_pTexture[TEXTURETYPE_SCORE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/highscore_logo.png", &m_pTexture[TEXTURETYPE_HIGHSCORE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &m_pTexture[TEXTURETYPE_PRESSENTER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_gamestart.png", &m_pTexture[TEXTURETYPE_GAMESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_gameclear.png", &m_pTexture[TEXTURETYPE_GAMECLEAR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_gameover.png", &m_pTexture[TEXTURETYPE_GAMEOVER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_result.png", &m_pTexture[TEXTURETYPE_RESULTLOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_arcademode.png", &m_pTexture[TEXTURETYPE_ARCADEMODE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_yourscore.png", &m_pTexture[TEXTURETYPE_YOURSCORE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_newrecord.png", &m_pTexture[TEXTURETYPE_NEWRECORD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_maxcombo.png", &m_pTexture[TEXTURETYPE_MAXCOMBO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/logo_killenemy.png", &m_pTexture[TEXTURETYPE_KILLENEMY]);

	return S_OK;
} 

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CLogo::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntLogo = 0; nCntLogo < TEXTURETYPE_MAX; nCntLogo++)
	{
		if (m_pTexture[nCntLogo] != NULL)
		{
			m_pTexture[nCntLogo]->Release();
			m_pTexture[nCntLogo] = NULL;
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CLogo *CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber)
{
	// �|�C���^�ϐ��̐���
	CLogo *pLogo;
	pLogo = new CLogo;

	// NULL�`�F�b�N
	if (pLogo != NULL)
	{
		// ����������
		pLogo->Init(pos, size, nTextureNumber);
	}

	return pLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_size = size;
	m_nTextureNumber = nTextureNumber;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);

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

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLogo::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CLogo::Update(void)
{
	// �_�ł��邩�ǂ���
	if (m_bBlinking == true)
	{
		Blinking();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_nTextureNumber >= 0)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture[m_nTextureNumber]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// ���S�̍��W�w��
//=============================================================================
void CLogo::SetPosition(D3DXVECTOR3 pos)
{
	// ���W�̕ۑ�
	m_pos = pos;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���S�̒��_�J���[�w��
//=============================================================================
void CLogo::SetCol(D3DXCOLOR col)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̍X�V
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �_�ŏ������n�߂�
//=============================================================================
void CLogo::StartBlinking(int nBlimkingInterval)
{
	// �_�ł��n�߂�
	m_bBlinking = true;

	// �_�ŊԊu�̕ۑ�
	m_nBlimkingInterval = nBlimkingInterval;
}

//=============================================================================
// �_�ŏ���
//=============================================================================
void CLogo::Blinking(void)
{
	// �J�E���g�̍X�V
	m_nBlinkingCounter++;

	// �_�ŃJ�E���g�Ɠ_�ŊԊu�������ė]�肪0�̂Ƃ�����
	if (m_nBlinkingCounter % (m_nBlimkingInterval * 2) == 0)
	{
		// �F�̕ύX
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_nBlinkingCounter % (m_nBlimkingInterval * 2) == m_nBlimkingInterval)
	{
		// �F�̕ύX
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}
}