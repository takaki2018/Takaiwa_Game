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
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posVDest;		// �ړI�̎��_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 posRDest;		// �ړI�̒����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 moveV;			// ���_�̈ړ���
	D3DXVECTOR3 moveR;			// �����_�̈ړ���
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9 viewport;		// �r���[�|�[�g
}Camera;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
void InitCamera(void);			// �J�����̏���������
void UninitCamera(void);		// �J�����̏I������
void UpdateCamera(void);		// �J�����̍X�V����
void SetCamera(int nIdx);		// �J�����̔z�u

void KeyboradFollow(void);		// �J�����̃L�[�{�[�h����(�Ǐ])
void LookDownPlayer(void);		// �v���C���[�������낷�J����

Camera GetCamera(void);			// �J�������̎擾

#endif