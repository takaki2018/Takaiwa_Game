//=============================================================================
//
// �V�[��2D���� [scene2D.cpp]
// Author : 
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_X		(1)							// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)							// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

//=============================================================================
// CScene�̃R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fHeight / 2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth / 2.0f + m_fHeight / 2.0f * m_fHeight / 2.0f);
}

//=============================================================================
// CScene�̃f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos , D3DXVECTOR2 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = size.x;
	m_fHeight = size.y;
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fHeight /2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth /2.0f + m_fHeight /2.0f * m_fHeight / 2.0f);

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
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
	pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	if (m_bDeath == true)
	{
		// ���_�o�b�t�@�̊J��
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �e�N�X�`�����̊��蓖��
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	 // �e�N�X�`���������蓖�Ă�
	m_pTexture = pTexture;
}

//=============================================================================
// ���W�ݒ菈��
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	// ���W�ݒ�
	m_pos = pos;

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR2 size)
{
	// ���W�ݒ�
	m_fWidth = size.x;
	m_fHeight = size.y;

	// �p�x�ƑΊp���̒������Čv�Z����
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fHeight / 2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth / 2.0f + m_fHeight / 2.0f * m_fHeight / 2.0f);

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// ��]�ݒ菈��
//=============================================================================
void CScene2D::SetRotateVertex(float fAngle)
{
	// ��]�ݒ�
	m_rot.z = fAngle;

	D3DXVECTOR3 pos[4];
	pos[0].x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pos[0].y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
			   
	pos[1].x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
			   
	pos[2].y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;

	float x = pos[1].x - pos[0].x;
	float y = pos[0].y - pos[2].y;

	if (x < 0.0f)
	{
		x *= -1.0f;
	}
	if (y < 0.0f)
	{
		y *= -1.0f;
	}
	m_fWidth = x;
	m_fHeight = y;

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// ���_���W�̐ݒ菈��
//=============================================================================
void CScene2D::SetVertex(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�J���[�̐ݒ菈��
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	// �ϐ��̍X�V
	m_col = col;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ菈��
//=============================================================================
void CScene2D::SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(texUV.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[1].tex = D3DXVECTOR2(texUV.x, texUV.y);
	pVtx[2].tex = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[3].tex = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}