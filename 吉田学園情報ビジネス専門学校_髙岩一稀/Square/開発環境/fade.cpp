//---------------------------------------------------------------- -
//
// �t�F�[�h (fade.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "fade.h"
#include "renderer.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define REMOVE_ALFHA		(0.01f)				// ���l�̑�����

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define MAX_VERTEX (4)												// ���_�̐�

//-----------------------------------------------------------------
// �\���̒�`
//-----------------------------------------------------------------
typedef  struct
{
	D3DXVECTOR3 pos; // ���_���W
	float rhw;	     // 1.0�ŌŒ�
	D3DCOLOR col;    // ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_2D;

//-----------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-----------------------------------------------------------------
CFade::FADE CFade::m_fade = CFade::FADE_IN;
CManager::MODE CFade::m_modeNext = CManager::MODE_TITLE;

//-----------------------------------------------------------------
// �t�F�[�h�̏���������
//-----------------------------------------------------------------
HRESULT CFade::Init(CManager::MODE modeNext)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_fade = CFade::FADE_IN;
	m_modeNext = modeNext;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffFade->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// �t�F�[�h�̏I������
//-----------------------------------------------------------------
void CFade::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuffFade != NULL)
	{
		m_pVtxBuffFade->Release();
		m_pVtxBuffFade = NULL;
	}
}

//-----------------------------------------------------------------
// �t�F�[�h�̍X�V����
//-----------------------------------------------------------------
void CFade::Update(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffFade->Unlock();

	// �t�F�[�h���̕���
	if (m_fade == FADE_IN)
	{// �t�F�[�h�C���̎�
		m_colorFade.a -= REMOVE_ALFHA;

		if (m_colorFade.a <= 0.0f)
		{// 0.0f�ɂȂ����Ƃ��������Ȃ�
			m_fade = FADE_NONE;
		}
	}
	else if (m_fade == FADE_OUT)
	{// �t�F�[�h�A�E�g�̂Ƃ�
		m_colorFade.a += REMOVE_ALFHA;

		if (m_colorFade.a >= 1.0f)
		{// 1.0f�ɂȂ����Ƃ��t�F�[�h�C��
			m_fade = FADE_IN;

			CManager::SetMode(m_modeNext);
		}
	}
	else if (m_fade == FADE_NONE)
	{// �������Ă��Ȃ��Ƃ�
		m_colorFade.a = 0.0f;
	}
}

//-----------------------------------------------------------------
// �t�F�[�h�̕`�揈��
//-----------------------------------------------------------------
void CFade::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-----------------------------------------------------------------
// �t�F�[�h�ݒ�
//-----------------------------------------------------------------
void CFade::SetFade(FADE fade, CManager::MODE modeNext)
{
	m_fade = fade;
	m_modeNext = modeNext;
}

//-----------------------------------------------------------------
// �t�F�[�h���̎擾
//-----------------------------------------------------------------
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}