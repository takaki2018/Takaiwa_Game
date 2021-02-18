//-----------------------------------------------------------------
//
// �E�B���h�E�̕\�� (main.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "main.h"
#include "time.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "title.h"
#include "loadscreen.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"

//-----------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL; // �t�H���g�̃|�C���^
int g_nCountFPS; // FPS�J�E���^
MODE g_mode = MODE_TITLE;

//-----------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);						 // ����������
void Uninit(void);																 // �I������
void Update(void);																 // �X�V����
void Draw(void);																 // �`�揈��
void DrawFPS(void);

//-----------------------------------------------------------------
// ���C���֐�
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		("%s",CLASS_NAME),             // �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),
	};
	HWND hWnd; // �E�B���h�E�n���h��(���ʎq)
	MSG msg;   // ���b�Z�[�W���i�[����

			   // �E�B���h�E�N���X��o�^
	RegisterClassEx(&wcex);

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
		0,
		("%s", CLASS_NAME), // �E�B���h�E�N���X�̖��O
		("%s", WINDOW_NAME), // �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW, // �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right,
		rect.bottom,
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DWORD dwExecLastTime;	// �������s�ŏI����
	DWORD dwFPSLastTime;	// FPS�v���ŏI����
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwFrameCount;		// �t���[���J�E���g

	dwExecLastTime = timeGetTime();
	dwFPSLastTime = timeGetTime();
	dwCurrentTime = timeGetTime();	// ���ݎ����̐ݒ�
	dwFrameCount = 0;

	// ���b�Z�[�W���[�v
	while (1)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) // ���b�Z�[�W���擾���Ȃ������ꍇ0��Ԃ�
		{ // windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��𑗏o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ // DirectX�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			// �t���[���J�E���g��1000/60�b�̎�����
			if((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				// ���
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				// �J�E���g
				dwFrameCount++;
			}
		}
	}

	// �I������
	Uninit();

	// �E�B���h�E�N���X�̓o�^�폜
	UnregisterClass(("%s", CLASS_NAME), wcex.hInstance);

	return (int)msg.wParam;
}

//-----------------------------------------------------------------
// �E�B���h�E�v���V�[�W��
//-----------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: // �L�[���������Ƃ��\��
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(NULL, "�I�����܂���?", "�m�F", MB_YESNO | MB_TOPMOST);
			if (nID == IDYES)
			{
				nID = MessageBox(NULL, "�I�����܂��B", "", MB_OK | MB_TOPMOST);
				if (nID == IDOK)
				{
					DestroyWindow(hWnd); // ���b�Z�[�W��Ԃ�
				}
			}
			break;
		}
		break;

	case WM_CLOSE: // �E�B���h�E����悤�Ƃ����Ƃ��\��
		nID = MessageBox(NULL, "(  ^ �� ^) < �I������Ⴄ?", "", MB_YESNO);
		if (nID == IDYES)
		{
			nID = MessageBox(NULL, "(  ^ �� ^) < �ق�Ƃ�?", "", MB_YESNO);

			if (nID == IDYES)
			{
				nID = MessageBox(NULL, "(  ; �� ;) < �҂���", "", MB_OK);

				if (nID == IDOK)
				{
					DestroyWindow(hWnd); // ���b�Z�[�W��Ԃ�
				}
			}
			else
			{
				nID = MessageBox(NULL, "(  ^ �� ^) < �����", "", MB_OK);
				return 0;
			}
		}
		else
		{
			return 0;
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------
// ����������
//-----------------------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// �f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h�̍���
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�̍������@�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����̍������@�̐���

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �k�����ɕ��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �g�厞�ɕ��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�l�̌J��Ԃ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�l�̌J��Ԃ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // ���l�̍���
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					
	//FPS�̏���������
	D3DXCreateFont(g_pD3DDevice, 22, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,		
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// �L�[�{�[�h���͂̏���������
	InitKeyboard(hInstance, hWnd);

	// �Q�[���p�b�h���͂̏���������
	InitGamepad(hInstance, hWnd);

	// �T�E���h�̏���������
	InitSound(hWnd);

	// �t�F�[�h�̏�����
	InitFade(g_mode);

	// ��ʃ��[�h�̏���������
	SetMode(g_mode);

	return S_OK;
}

//-----------------------------------------------------------------
// �I������
//-----------------------------------------------------------------
void Uninit(void)
{
	// �L�[�{�[�h���͂̏I������
	UninitKeyboard();

	//�Q�[���p�b�h���͂̏I������
	UninitGamepad();

	// �t�F�[�h�̏I������
	UninitFade();

	// ��ʃ��[�h�̏I������
	switch (g_mode)
	{// �^�C�g����ʂ̏I������
	case MODE_TITLE:
		UninitTitle();
		break;
		// ���[�h��ʂ̏I������
	case MODE_LOADSCREEN:
		UninitLoadScreen();
		break;
		// �Q�[���̏I������
	case MODE_GAME:
		UninitGame();
		break;
		// ���U���g��ʂ̏I������
	case MODE_RESULT:
		UninitResult();
		break;
		// �����L���O�̏I������
	case MODE_RANKING:
		UninitRanking();
		break;
	}

	// �T�E���h�̏I������
	UninitSound();

	// �t�H���g�I�u�W�F�N�g�̊J��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//-----------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------
void Update(void)
{
	// �L�[�{�[�h���͂̍X�V����
	UpdateKeyboard();

	// �Q�[���p�b�h���͂̍X�V����
	UpdateGamepad();

	// ��ʃ��[�h�̍X�V����
	switch (g_mode)
	{// �^�C�g����ʂ̍X�V����
	case MODE_TITLE:
		UpdateTitle();
		break;
		// ���[�h��ʂ̍X�V����
	case MODE_LOADSCREEN:
		UpdateLoadScreen();
		break;
		// �Q�[���̍X�V����
	case MODE_GAME:
		UpdateGame();
		break;
		// ���U���g��ʂ̍X�V����
	case MODE_RESULT:
		UpdateResult();
		break;
		// �����L���O�̍X�V����
	case MODE_RANKING:
		UpdateRanking();
		break;
	}
	// �t�F�[�h�̍X�V����
	UpdateFade();
}

//-----------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------
void Draw(void)
{
	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	// �`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// ��ʃ��[�h�̏C������
		switch (g_mode)
		{// �^�C�g����ʂ̕`�揈��
		case MODE_TITLE:
			DrawTitle();
			break;
			// ���[�h��ʂ̕`�揈��
		case MODE_LOADSCREEN:
			DrawLoadScreen();
			break;
			// �Q�[���̕`�揈��
		case MODE_GAME:
			DrawGame();
			break;
			// ���U���g��ʂ̕`�揈��
		case MODE_RESULT:
			DrawResult();
			break;
			// �����L���O�̕`�揈��
		case MODE_RANKING:
			DrawRanking();
			break;
		}	
		// �t�F�[�h�̕`�揈��
		DrawFade();

#ifdef _DEBUG
		// FPS�̕`�揈��
		DrawFPS();
#endif

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-----------------------------------------------------------------
// �f�o�C�X�̎擾
//-----------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-----------------------------------------------------------------
// FPS�̕`��
//-----------------------------------------------------------------
void DrawFPS(void)
{
	// �ϐ��錾
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	// �e�L�X�g
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// �e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect,
		DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//-----------------------------------------------------------------
// ���[�h�̐ݒ�
//-----------------------------------------------------------------
void SetMode(MODE mode)
{
	// ��ʃ��[�h�̏C������
	switch (g_mode)
	{// �^�C�g����ʂ̏I������
	case MODE_TITLE:
		UninitTitle();
		break;
		// ���[�h��ʂ̏I������
	case MODE_LOADSCREEN:
		UninitLoadScreen();
		break;
		// �Q�[���̏I������
	case MODE_GAME:
		UninitGame();
		break;
		// ���U���g��ʂ̏I������
	case MODE_RESULT:
		UninitResult();
		break;
		// �����L���O�̏I������
	case MODE_RANKING:
		UninitRanking();
		break;
	}
	// ���[�h�̍X�V
	g_mode = mode;

	// ��ʃ��[�h�̏���������
	switch (mode)
	{// �^�C�g����ʂ̏���������
	case MODE_TITLE:
		InitTitle();
		break;
		// ���[�h��ʂ̏���������
	case MODE_LOADSCREEN:
		InitLoadScreen();
		break;
		// �Q�[���̏���������
	case MODE_GAME:
		InitGame();
		break;
		// ���U���g��ʂ̏���������
	case MODE_RESULT:
		InitResult();
		break;
		// �����L���O�̏���������
	case MODE_RANKING:
		InitRanking();
		break;
	}
}

//-----------------------------------------------------------------
// ��ʃ��[�h�̎擾
//-----------------------------------------------------------------
MODE GetMode(void)
{
	return g_mode;
}