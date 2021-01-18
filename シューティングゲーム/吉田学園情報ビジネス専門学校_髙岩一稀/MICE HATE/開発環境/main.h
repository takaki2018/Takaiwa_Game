//-------------------------------------------------------------------
//
// ���C���w�b�_�[ (main.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include "d3dx9.h"					// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"					// ���͏����ɕK�v
#include "xaudio2.h"				// �T�E���h�Đ��ɕK�v
#include <Xinput.h>



//-------------------------------------------------------------------
// ���C�u�����̃����N
//-------------------------------------------------------------------
#pragma comment(lib,"d3d9.lib")   // �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")  // �g�����C�u����
#pragma comment(lib,"dxguid.lib") // �R���|�[�l���g�g�p�ɕK�v
#pragma comment(lib,"dinput8.lib") // ���͏����ɕK�v
#pragma comment(lib,"winmm.lib")
#pragma comment (lib, "xinput.lib")

//-------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------
#define SCREEN_WIDTH  (1280)										 // �X�N���[���̕�
#define SCREEN_HEIGHT (720)											// �X�N���[���̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define CLASS_NAME "WindowClass"									// �E�B���h�E�N���X�̖��O
#define WINDOW_NAME "2D�V���[�e�B���O�Q�[��"							// �E�B���h�E�̖��O
#define MAX_VERTEX (4)												// ���_�̐�

//-----------------------------------------------------------------
// �\���̂̒�`
//-----------------------------------------------------------------
typedef  struct
{
	D3DXVECTOR3 pos; // ���_���W
	float rhw;	     // 1.0�ŌŒ�
	D3DCOLOR col;    // ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,		// �^�C�g�����
	MODE_LOADSCREEN,	// ���[�h���
	MODE_TUTORIAL,		// �`���[�g���A��
	MODE_GAME,			// �Q�[�����
	MODE_RESULT,		// ���U���g���
	MODE_RANKING,		// �����L���O���
	MODE_MAX,
}MODE;

//-----------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);	// �f�o�C�X�̊l��
void SetMode(MODE mode);			// ���[�h�̐ݒ�
MODE GetMode(void);					// ���[�h�̊l��

#endif

