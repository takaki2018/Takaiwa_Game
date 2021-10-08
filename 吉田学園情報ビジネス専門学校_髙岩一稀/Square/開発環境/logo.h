//=============================================================================
//
// ���S���� [bg.h]
// Author : 
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "scene.h"

//-----------------------------------------------------------------------------
// ���S�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CLogo
{
public:
	// �e�N�X�`���̎��
	typedef enum
	{
		TEXTURETYPE_TITLELOGO = 0,	// �^�C�g�����S
		TEXTURETYPE_PLAY,			// �v���C���S
		TEXTURETYPE_TUTORIAL,		// �`���[�g���A�����S
		TEXTURETYPE_RANKING,		// �����L���O���S
		TEXTURETYPE_OPTION,			// �I�v�V�������S
		TEXTURETYPE_SCORE,			// �X�R�A���S
		TEXTURETYPE_HIGHSCORE,		// �n�C�X�R�A���S
		TEXTURETYPE_PRESSENTER,		// PressEnter
		TEXTURETYPE_GAMESTART,		// GameStart���S
		TEXTURETYPE_GAMECLEAR,		// GameClear���S
		TEXTURETYPE_GAMEOVER,		// GameOver���S
		TEXTURETYPE_RESULTLOGO,		// ���U���g���S
		TEXTURETYPE_ARCADEMODE,		// ArcadeMode���S
		TEXTURETYPE_YOURSCORE,		// YourScore���S
		TEXTURETYPE_NEWRECORD,		// NewRecord���S
		TEXTURETYPE_MAXCOMBO,		// �ő�R���{���S
		TEXTURETYPE_KILLENEMY,		// �G���j�����S
		TEXTURETYPE_MAX,
	}TEXTURETYPE_LOGO;

	CLogo();
	~CLogo();
	static HRESULT Load(void);
	static void Unload(void);
	static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTextureNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void StartBlinking(int nBlimkingInterval);
	void SetPosition(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

private:
	void Blinking(void);										// �_�ŏ���
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURETYPE_MAX];		// �e�N�X�`�����̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_pos;											// �ʒu���
	D3DXVECTOR2 m_size;											// �T�C�Y���
	int m_nTextureNumber;										// �e�N�X�`�����ʔԍ�
	bool m_bBlinking;											// �_�ł��邩�ǂ���
	int m_nBlimkingInterval;									// �_�ŊԊu
	int m_nBlinkingCounter;										// �_�ŃJ�E���g
};
#endif