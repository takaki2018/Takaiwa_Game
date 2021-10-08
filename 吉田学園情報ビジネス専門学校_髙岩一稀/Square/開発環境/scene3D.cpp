//=============================================================================
//
// �V�[��3D���� [scene2D.cpp]
// Author : 
//
//=============================================================================
#include "scene3D.h"
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
CScene3D::CScene3D(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fDepth = 0.0f;
}

//=============================================================================
// CScene�̃f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = size.x;
	m_fDepth = size.y;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - m_fWidth / 2.0f, pos.y, pos.z + m_fDepth / 2.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_fWidth / 2.0f, pos.y, pos.z + m_fDepth / 2.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_fWidth / 2.0f, pos.y, pos.z - m_fDepth / 2.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_fWidth / 2.0f, pos.y, pos.z - m_fDepth / 2.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y,m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, m_pTexture);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �e�N�X�`�����̊��蓖��
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// �e�N�X�`���������蓖�Ă�
	m_pTexture = pTexture;
}

//=============================================================================
// ���W�ݒ菈��
//=============================================================================
void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
	// ���W�ݒ�
	m_pos = pos;

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CScene3D::SetSize(D3DXVECTOR2 size)
{
	// ���W�ݒ�
	m_fWidth = size.x;
	m_fDepth = size.y;

	// �p�x�ƑΊp���̒������Čv�Z����
	m_fAngle = atan2f(m_fWidth / 2.0f, m_fDepth / 2.0f);
	m_fLength = sqrtf(m_fWidth / 2.0f * m_fWidth / 2.0f + m_fDepth / 2.0f * m_fDepth / 2.0f);

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// ���_���W�̐ݒ菈��
//=============================================================================
void CScene3D::SetVertex(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

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
void CScene3D::SetCol(D3DXCOLOR col)
{
	// �ϐ��̍X�V
	m_col = col;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

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
void CScene3D::SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

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