//=============================================================================
//
// ���f������ [model.cpp]
// Author : 
//
//=============================================================================
#include "model.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "debugproc.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// CModel�̃R���X�g���N�^
//=============================================================================
CModel::CModel(int nPriority) : CSceneX(nPriority)
{
	// �ϐ��̏�����
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
	// CScene2D�̏���������
	CSceneX::Init(pos, size);

	m_moveRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nCnt = 0;

	// �}�e���A�����̎擾
	/*LPD3DXBUFFER pMat = GetMat();
	int nNumMat = GetNumMat();

	for (int nCnt = 0; nCnt < nNumMat; nCnt++)
	{
		pMat[nCnt].GetBufferPointer
	}
	*/
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	// CScene2D�̏I������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{
	// CScene2D�̍X�V����
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