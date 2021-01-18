//-------------------------------------------------------------------
//
// �^�C�g���w�b�_�[ (title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SELECTMODE_H_
#define _SELECTMODE_H_

#include "main.h"

//-------------------------------------------------------------------
// �񋓌^
//-------------------------------------------------------------------
typedef enum
{
	PLAYMODE_SINGLE = 0,	// �V���O��
	PLAYMODE_MULTI,			// �}���`
	PLAYMODE_MAX,			// �v���C���[�h�̍ő吔
}PLAYMODE;

//-------------------------------------------------------------------
// �\���̂̒�`
//-------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	float fWidth;			// ��
	float fHeight;			// ����
}SELECTMODE;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitSelectmode(void);			// �Z���N�g���[�h�̏���������
void UninitSelectmode(void);			// �Z���N�g���[�h�̏I������
void UpdateSelectmode(void);			// �Z���N�g���[�h�̍X�V����
void DrawSelectmode(void);				// �Z���N�g���[�h�̕`�揈��

void SelectMode(void);					// ���[�h�I��
void DecideMode(void);					// ���[�h�I������

void SetVertexLogo(int nCntLogo);		// ���_���W�̐ݒ�
PLAYMODE GetPlayMode(void);				// ���̃��[�h��I�񂾂�

#endif
