//-----------------------------------------------------------------
//
// �J���� (camera.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------
#define LENGTH					(50.0f)		// ���s
#define MOVE_ROT_LookAtPoint	(0.05f)			// �����_��]��
#define MOVE_ROT_Point			(0.05f)			// ���_��]��
#define MOVE_LookAtPoint		(0.5f)			// �����_�ړ���
#define MOVE_Point				(3.0f)			// ���_�ړ���

//-----------------------------------------------------------------
// CCamera�̃R���X�g���N�^
//-----------------------------------------------------------------
CCamera::CCamera()
{

}

//-----------------------------------------------------------------
// CCamera�̃f�X�g���N�^
//-----------------------------------------------------------------
CCamera::~CCamera()
{

}

//-----------------------------------------------------------------
// �J�����̏���������
//-----------------------------------------------------------------
void CCamera::Init(void)
{
	// �ϐ��̏�����
	m_posV = D3DXVECTOR3(0.0f, 0.0f, -LENGTH);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = SCREEN_WIDTH;
	m_viewport.Height = SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
}

//-----------------------------------------------------------------
// �J�����̏I������
//-----------------------------------------------------------------
void CCamera::Uninit(void)
{

}

//-----------------------------------------------------------------
// �J�����̍X�V����
//-----------------------------------------------------------------
void CCamera::Update(void)
{
#ifdef _DEBUG
	// �����_����
	LookAtPointRotate();

	// ���_����
	PointRotate();

	// �����_�ړ�
	LookAtPointMove();

	// ���_�ړ�
	//PointMove();

	// �f�o�b�N�\��
	CDebugProc::Print("\n--- �J������� ---\n");
	CDebugProc::Print("�J�����̎��_  :%f %f %f\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("�J�����̒����_:%f %f %f\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//-----------------------------------------------------------------
// �J�����̐ݒu
//-----------------------------------------------------------------
void CCamera::SetCamera(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �ϐ��̎󂯓n��
	pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		3000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//-----------------------------------------------------------------
// �J�����̒����_��]����
//-----------------------------------------------------------------
void CCamera::LookAtPointRotate(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_LookAtPoint_LEFT) == true)
	{// ���_�̍���]
		m_rot.y -= MOVE_ROT_LookAtPoint;						// ��]��

		if (m_rot.y < -D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			m_rot.y += D3DX_PI * 2.0f;
		}

		// �����_�̍X�V
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_LookAtPoint_RIGHT) == true)
	{// ���_�̉E��]
		m_rot.y += MOVE_ROT_LookAtPoint;						// ��]��

		if (m_rot.y > D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// �����_�̍X�V
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// �J�����̎��_��]����
//-----------------------------------------------------------------
void CCamera::PointRotate(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_Point_LEFT) == true)
	{// ���_�̍���]
		m_rot.y -= MOVE_ROT_Point;						// ��]��

		if (m_rot.y < -D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			m_rot.y += D3DX_PI * 2.0f;
		}

		// ���_�̍X�V
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_Point_RIGHT) == true)
	{// ���_�̉E��]
		m_rot.y += MOVE_ROT_Point;						// ��]��

		if (m_rot.y > D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			m_rot.y -= D3DX_PI * 2.0f;
		}
		// ���_�̍X�V
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// �J�����̒����_�ړ�����
//-----------------------------------------------------------------
void CCamera::LookAtPointMove(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_LookAtPoint_UP) == true)
	{
		// �����_�̏�ړ�
		m_posR.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_LookAtPoint_DOWN) == true)
	{
		// �����_�̉��ړ�
		m_posR.y -= MOVE_Point;
	}
}

//-----------------------------------------------------------------
// �J�����̎��_�ړ�����
//-----------------------------------------------------------------
void CCamera::PointMove(void)
{
	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_UP) == true)
	{
		// ���_�̑O�ړ�
		m_posV.x += MOVE_Point * sinf(m_rot.y);	
		m_posV.z += MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_DOWN) == true)
	{
		// ���_�̌��ړ�
		m_posV.x -= MOVE_Point * sinf(m_rot.y);
		m_posV.z -= MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
	{
		// ���_�̉E�ړ�
		m_posV.x += MOVE_Point * cosf(m_rot.y);
		m_posV.z -= MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_LEFT) == true)
	{
		// ���_�̍��ړ�
		m_posV.x -= MOVE_Point * cosf(m_rot.y);
		m_posV.z += MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Point_UP) == true)
	{
		// ���_�̏�ړ�
		m_posV.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_Point_DOWN) == true)
	{
		// ���_�̉��ړ�
		m_posV.y -= MOVE_Point;
	}
}