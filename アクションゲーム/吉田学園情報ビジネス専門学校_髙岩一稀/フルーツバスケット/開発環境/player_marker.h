//-------------------------------------------------------------------
//
// �v���C���[�}�[�J�[�w�b�_�[ (player_marker.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _PLAYERMARKER_H_
#define _PLAYERMARKER_H_

#include "main.h"

//-------------------------------------------------------------------
// �񋓌^
//-------------------------------------------------------------------
typedef enum
{
	PLAYER_MARKER_TYPE_ONE = 0,		// 1P
	PLAYER_MARKER_TYPE_TWO,			// 2P
	PLAYER_MARKER_TYPE_MAX,			// �l���|�C���g���S�̍ő吔
}PLAYER_MARKER_TYPE;				// �v���C���[�}�[�J�[�̍ő�l

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 move;				// �ړ���
	PLAYER_MARKER_TYPE type;		// �v���C���[�}�[�J�[�̃^�C�v
}PLAYER_MARKER;						// �l���v���C���[�}�[�J�[�̍\����

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitPlayerMarker(void);															// �|�C���g�̏���������
void UninitPlayerMarker(void);															// �|�C���g�̏I������
void UpdatePlayerMarker(void);															// �|�C���g�̍X�V����
void DrawPlayerMarker(void);															// �|�C���g�̕`�揈��
void SetPlayerMarker(int nCntPlayerMarker,D3DXVECTOR3 pos, PLAYER_MARKER_TYPE type);	// �|�C���g�̔z�u
void SetVertexPlayerMarker(int nIdx);													// ���_���W�̐ݒ�

#endif
