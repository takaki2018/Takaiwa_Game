//-------------------------------------------------------------------
//
// �J�����w�b�_�[ (camera.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//-------------------------------------------------------------------
// �J�����̎��(�񋓌^)
//-------------------------------------------------------------------
typedef enum
{
	CAMERATYPE_PLAYER_LOOKDOWN = 0,		// �v���C���[�����낵�J����
	CAMERATYPE_PLAYER,				// �v���C���[�Ǐ]�J����
	CAMERATYPE_MAX,
}CAMERATYPE;

//-------------------------------------------------------------------
// �J�����N���X(��{�N���X)
//-------------------------------------------------------------------
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);			
	void Uninit(void);		
	void Update(void);		
	void SetCamera(void);	

	D3DXVECTOR3 GetRotate(void) { return m_rot; }

private:
	void LookAtPointRotate(void);	// �J�����̒����_��]����
	void PointRotate(void);			// �J�����̎��_��]����
	void LookAtPointMove(void);		// �J�����̒����_�ړ�
	void PointMove(void);			// �J�����̎��_�ړ�

	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// ����
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;		// �r���[�|�[�g
};
#endif