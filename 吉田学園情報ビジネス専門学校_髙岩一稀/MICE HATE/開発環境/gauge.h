//-------------------------------------------------------------------
//
// �Q�[�W�w�b�_�[ (gauge.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitGauge(void); // �Q�[�W�̏���������
void UninitGauge(void);  // �Q�[�W�̏I������
void UpdateGauge(void);	 // �Q�[�W�̍X�V����
void DrawGauge(void);	 // �Q�[�W�̕`�揈��
bool GetRemove(void);	 // �Q�[�W�̌����l�̏��

#endif
