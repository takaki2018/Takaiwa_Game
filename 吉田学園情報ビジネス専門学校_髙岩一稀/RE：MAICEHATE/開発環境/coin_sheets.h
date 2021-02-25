//-------------------------------------------------------------------
//
// �R�C���l���󋵃w�b�_�[ (coin_sheets.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _COINSHEETS_H_
#define _COINSHEETS_H_

#include "main.h"

//-------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------
HRESULT InitCoinSheets(void);					// �R�C���l���󋵂̏���������
void UninitCoinSheets(void);					// �R�C���l���󋵂̏I������
void UpdateCoinSheets(void);					// �R�C���l���󋵂̍X�V����
void DrawCoinSheets(void);						// �R�C���l���󋵂̕`�揈��

void SetVertexCoinSheets(int nIdx);				// ���_���W�̐ݒ�
void SetCoinTexInfo(int nIdx);					// �R�C���e�N�X�`���̐ݒu���
void SetNumberTexInfo(int nIdx);				// �����e�N�X�`���̐ݒu���

void AddCoinSheets(int nValue);					// �R�C���l���󋵂̉��Z

#endif