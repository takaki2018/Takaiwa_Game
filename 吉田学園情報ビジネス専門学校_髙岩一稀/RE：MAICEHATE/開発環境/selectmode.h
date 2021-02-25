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
	SELECTMODE_NORMAL = 0,	// �m�[�}�����[�h
	SELECTMODE_ENDLESS,		// �G���h���X���[�h
	SELECTMODE_TUTORIAL,	// �`���[�g���A�����[�h
}SELECTMODE;

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitSelectmode(void);		// ���S�̏���������
void UninitSelectmode(void);		// ���S�̏I������
void UpdateSelectmode(void);		// ���S�̍X�V����
void DrawSelectmode(void);			// ���S�̕`�揈��
void SetVertexLogo(int nCntLogo);	// �^�C�g����ʂɓ\�郍�S�̎w��
int GetSelectMode(void);			// ���̃��[�h��I�񂾂�

#endif
