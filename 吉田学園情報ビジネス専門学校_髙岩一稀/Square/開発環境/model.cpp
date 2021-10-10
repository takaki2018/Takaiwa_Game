//=============================================================================
//
// ���f������ [model.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "model.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "debugproc.h"

//=============================================================================
// CModel�̃R���X�g���N�^
//=============================================================================
CModel::CModel(int nPriority) : CSceneX(nPriority)
{
	// �����o�ϐ��̏�����
	m_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCnt = 0;
	m_bAppear = false;
}

//=============================================================================
// CModel�̃f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// ��������
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, char *pModelFileName)
{
	// �|�C���^�ϐ��̐���
	CModel *pModel;
	pModel = new CModel(CScene::PRIORITY_MODEL);

	// NULL�`�F�b�N
	if (pModel != NULL)
	{
		// ���f���t�@�C���l�[���̐ݒ�
		pModel->BindXFileName(pModelFileName);

		// ����������
		pModel->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pModel->SetObjType(OBJTYPE_MODEL);
	}

	return pModel;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CSceneX�̏���������
	CSceneX::Init(pos, size);

	// �����o�ϐ��̏�����
	m_moveRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nCnt = 0;
	m_bAppear = false;

	// �}�e���A�����̎擾
	D3DXMATERIAL *pMat = GetMat();
	int nNumMat = GetNumMat();

	for (int nCnt = 0; nCnt < nNumMat; nCnt++)
	{
		// ���l�̐ݒ�
		pMat[nCnt].MatD3D.Diffuse.a = 0.0f;

		// �}�e���A���̐ݒ�
		SetMat(pMat,nCnt);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	// CSceneX�̏I������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{
	// CSceneX�̍X�V����
	CSceneX::Update();

	// �p�x���̎擾
	D3DXVECTOR3 rot = GetRotate();

	// ����ɉ��
	rot.x += m_moveRot.x + 0.01f;
	rot.y += m_moveRot.y + 0.01f;
	rot.z -= m_moveRot.z + 0.005f;

	// ����
	m_moveRot.x += (0.0f - m_moveRot.x) * 0.03f;
	m_moveRot.y += (0.0f - m_moveRot.y) * 0.03f;
	m_moveRot.z += (0.0f - m_moveRot.z) * 0.03f;

	// �p�x�̐ݒ�
	SetRotate(rot);

	if (m_bAppear == false)
	{
		// �}�e���A�����̎擾
		D3DXMATERIAL *pMat = GetMat();
		int nNumMat = GetNumMat();

		for (int nCnt = 0; nCnt < nNumMat; nCnt++)
		{
			// ���l�̐ݒ�
			pMat[nCnt].MatD3D.Diffuse.a += 0.0005f;

			// �}�e���A���̐ݒ�
			SetMat(pMat, nCnt);

			// ���l��1.0f�𒴂����猩�������Ƃ�`����
			if (pMat[nCnt].MatD3D.Diffuse.a >= 1.0f)
			{
				m_bAppear = true;
			}
		}
	}

#ifdef _DEBUG
	CDebugProc::Print("\n--- ���f���̊p�x ---\n");
	CDebugProc::Print("X�� : %f\n",rot.x);
	CDebugProc::Print("Y�� : %f\n", rot.y);
	CDebugProc::Print("Z�� : %f\n", rot.z);
	CDebugProc::Print("\n--- ���f���̊p�x�ړ��� ---\n");
	CDebugProc::Print("X�� : %f\n", m_moveRot.x);
	CDebugProc::Print("Y�� : %f\n", m_moveRot.y);
	CDebugProc::Print("Z�� : %f\n", m_moveRot.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	// CSceneX�̕`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �p�x�ړ��ʉ��Z����
//=============================================================================
void CModel::AddMoveRotate(D3DXVECTOR3 rot)
{
	// �p�x�̉��Z
	m_moveRot += rot;

	// �ړ��ʂ̐���
	// X��
	if (m_moveRot.x >= 0.2f)
	{
		m_moveRot.x = 0.2f;
	}

	// Y��
	if (m_moveRot.y >= 0.2f)
	{
		m_moveRot.y = 0.2f;
	}

	// Z��
	if (m_moveRot.z >= 0.2f)
	{
		m_moveRot.z = 0.2f;
	}
}