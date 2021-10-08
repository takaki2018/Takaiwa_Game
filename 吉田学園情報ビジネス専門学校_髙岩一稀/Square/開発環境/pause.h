//==============================================================================================================
//
// �|�[�Y�w�b�_�[ (pause.h)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene.h"

//--------------------------------------------------------------------------------------------------------------
// �O���錾
//--------------------------------------------------------------------------------------------------------------
class CScene2D;			// �|���S��2D�N���X

//--------------------------------------------------------------------------------------------------------------
// �|�[�Y��ʃN���X(�h���N���X)
//--------------------------------------------------------------------------------------------------------------
class CPause : public CScene
{
public:
	// �|�[�Y�Ŏg�p����UI�̎�ށ@
	typedef enum
	{
		PAUSEUI_NONE = 0,	// �|�[�Y�w�i
		PAUSEUI_MENUBG,		// ���j���[�w�i
		PAUSEUI_CONTINUE,	// �R���e�B�j���[
		PAUSEUI_RETRY,		// ���g���C
		PAUSEUI_QUIT,		// �I��
		PAUSEUI_MAX,
	}PAUSEUI;

	CPause(int nPriolity);
	~CPause();
	static HRESULT Load(void);
	static void Unload(void);
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);				
	void Update(void);				
	void Draw(void);				

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSEUI_MAX];		// �|�[�Y��ʂ̃e�N�X�`���̃|�C���^
	CScene2D *m_apScene2D[PAUSEUI_MAX];						// 2D�|���S�����
	int m_nPauseSelect;										// �|�[�Y���j���[����J�ڂ���ꏊ��ۑ�
};

#endif