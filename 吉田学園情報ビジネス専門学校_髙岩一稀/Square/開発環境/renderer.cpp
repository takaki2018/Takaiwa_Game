//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : 
//
//=============================================================================
#include "renderer.h"
#include "scene.h"	
#include "manager.h"
#include "fade.h"
#include "debugproc.h"
#include "input_keyboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // ���_�t�H�[�}�b�g
#define MAX_VERTEX (4)												// ���_�̐�

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos; // ���_���W
	float rhw;	     // 1.0�ŌŒ�
	D3DCOLOR col;    // ���_�J���[
	D3DXVECTOR2 tex;
} VERTEX_2D;


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_X		(1)							// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)							// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

#define DRAW_TYPE		(1)							// �`����@
#define POLYGON_SIZE	(50.0f + m_fMovementLength)	// �|���S���T�C�Y
#define MOVE_ROT		(0.02f)						// ��]�p�̈ړ���

#define EXPANTION_COUNT	(50)						// �g��J�E���g
#define SHRINK_COUNT	(100)						// �k���J�E���g
#define MOVE_LENGTH		(0.5f)						// �|���S���T�C�Y�̈ړ���

//=============================================================================
// CRenderer�̃R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	
}

//=============================================================================
// CRenderer�̃f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h�̍���
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�̍������@�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����̍������@�̐���

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �k�����ɕ��
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �g�厞�ɕ��
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�l�̌J��Ԃ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�l�̌J��Ԃ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // ���l�̍���
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	// �e�N�X�`�������_�����O�p�e�N�X�`���̏�����
	InitRenderTex();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// �o�b�t�@�j��
		if (m_apBuffMT[nCnt] != NULL)
		{
			m_apBuffMT[nCnt]->Release();
			m_apBuffMT[nCnt] = NULL;
		}

		// �C���^�[�t�F�[�X�j��
		if (m_apRenderMT[nCnt] != NULL)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = NULL;
		}

		// �e�N�X�`���j��
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}
	}

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	// �t�F�[�h�o�b�N�G�t�F�N�g�L�����̂ݏ���
	if (m_bFadeBackEffect == true)
	{
		// �J�E���g�A�b�v
		m_nCnt++;

		if (m_nCnt <= 3)
		{
			// �|���S���g��
			m_size.x += 1.6f * 3.0f;
			m_size.y += 0.9f * 3.0f;
		}
		else if (m_nCnt > 3 && m_nCnt <= 9)
		{
			// �|���S���k��
			m_size.x -= 1.6f * 1.5f;
			m_size.y -= 0.9f * 1.5f;
		}
		else if (m_nCnt > 9)
		{
			// �����l�ɖ߂�
			m_nCnt = 0;
			m_fAlpha = 0.0f;
			m_size.x = SCREEN_WIDTH / 2.0f;
			m_size.y = SCREEN_HEIGHT / 2.0f;

			// �t�F�[�h�o�b�N�G�t�F�N�g���I����
			m_bFadeBackEffect = false;
		}

		// �\���̂̃|�C���^�ϐ�
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuffMT->Unlock();
	}
#ifdef _DEBUG
	CDebugProc::Print("\n--- �t�F�[�h�o�b�N�G�t�F�N�g��� ---\n");
	CDebugProc::Print("���l   : %f\n",m_fAlpha);
	CDebugProc::Print("�T�C�Y : (%f,%f)\n", m_size.x,m_size.y);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	// �t�F�[�h���̎擾
	CFade *pFade = CManager::GetFade();

	// �ϐ��錾
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;
	D3DVIEWPORT9 viewportDef;
	LPDIRECT3DSURFACE9 pRenderWk;
	LPDIRECT3DTEXTURE9 pTextureWk;

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// ���݂̏���ۑ�
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);
		m_pD3DDevice->GetViewport(&viewportDef);

		// �����_�����O�^�[�Q�b�g�̐ݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

		// Z�o�b�t�@�̐ݒ�
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[0]);

		// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐ݒ�
		m_pD3DDevice->SetViewport(&m_viewportMT);

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		m_pD3DDevice->Clear(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// �I�u�W�F�N�g�̕`�揈��
		CScene::DrawAll();

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// �t�B�[�h�o�b�N�p�|���S���Ƀe�N�X�`��[1]�𒣂�t����
		m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// �S�Ė߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
		m_pD3DDevice->SetViewport(&viewportDef);

		// ���l��s�����ɂ���
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// �t�B�[�h�o�b�N�p�|���S���Ƀe�N�X�`��[0]�𒣂�t����
		m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// �ݒ��߂�
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// �e�N�X�`��[0]�ƃe�N�X�`��[1]�����ւ���
		pRenderWk = m_apRenderMT[0];
		m_apRenderMT[0] = m_apRenderMT[1];
		m_apRenderMT[1] = pRenderWk;

		pTextureWk = m_apTextureMT[0];
		m_apTextureMT[0] = m_apTextureMT[1];
		m_apTextureMT[1] = pTextureWk;

		// �t�F�[�h�̕`�揈��
		pFade->Draw();

#ifdef _DEBUG
		// FPS�\��
		DrawFPS();

		CDebugProc::Draw();
#endif
		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �t�F�[�h�o�b�N�G�t�F�N�g
//=============================================================================
void CRenderer::SetFadeBackEffect(void)
{
	// �t�B�[�h�o�b�N�G�t�F�N�g�̋���
	m_bFadeBackEffect = true;

	// �ϐ��̏�����
	m_nCnt = 0;
	m_fAlpha = 0.7f;
	m_size.x = SCREEN_WIDTH / 2.0f;
	m_size.y = SCREEN_HEIGHT / 2.0f;
}

//=============================================================================
// �e�N�X�`�������_�����O�p�e�N�X�`���̏�����
//=============================================================================
HRESULT CRenderer::InitRenderTex(void)
{
	// �����o�ϐ��̏�����
	m_fAlpha = 0.0f;
	m_size = D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	m_bFadeBackEffect = false;
	m_nCnt = 0;

	// �ϐ��錾
	LPDIRECT3DSURFACE9 pRenderDef, pBuffDef;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// �����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̐���
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		// �e�N�X�`�������_�����O�p�̃C���^�[�t�F�[�X����
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_apBuffMT[nCnt],
			NULL);

		// ���݂̃����_�����O�^�[�Q�b�g���擾
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		// ���݂�Z�o�b�t�@�̎擾
		m_pD3DDevice->GetDepthStencilSurface(&pBuffDef);

		// �����_�����O�^�[�Q�b�g��ݒ�
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		// Z�o�b�t�@�̐ݒ�
		m_pD3DDevice->SetDepthStencilSurface(m_apBuffMT[nCnt]);

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		m_pD3DDevice->Clear(
			0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		// �����_�����O�^�[�Q�b�g��ݒ�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		// Z�o�b�t�@�̐ݒ�
		m_pD3DDevice->SetDepthStencilSurface(pBuffDef);
	}

	// �e�N�X�`�������_�����O�p�r���[�|�[�g�̐ݒ�
	m_viewportMT.X = 0;
	m_viewportMT.Y = 0;
	m_viewportMT.Width = SCREEN_WIDTH;
	m_viewportMT.Height = SCREEN_HEIGHT;
	m_viewportMT.MinZ = 0.0f;
	m_viewportMT.MaxZ = 1.0f;

	// �t�B�[�h�o�b�N�p�|���S���̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,					// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f + m_size.y - 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + m_size.x - 0.5f, SCREEN_HEIGHT / 2.0f - m_size.y - 0.5f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void CRenderer::DrawFPS(void)
{
	RECT rect = { 1230, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];
	int nCntFPS = GetCountFPS();

	wsprintf(str, "FPS:%d\n", nCntFPS);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0, 0, 0));
}
#endif


