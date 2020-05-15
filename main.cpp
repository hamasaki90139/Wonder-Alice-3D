//=============================================================================
//
// ���C������ [main.cpp]
// Author : �_�� ���t
//
//=============================================================================
#include "main.h"
#include "debugproc.h"
#include "input.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "particle.h"
#include "enemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "meshsky.h"
#include "collision.h"
#include "particle.h"
#include "effect.h"
#include "score.h"
#include "title.h"
#include "tutorial.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "pose.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Wonder Alice 3D"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
//#ifdef _DEBUG
int					g_nCountFPS;			// FPS�J�E���^
MODE				g_mode = MODE_TITLE;	// ���[�h
//#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				// FPS�\��
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��


	//�t�H�O�̐ݒ�
	FLOAT StartPos = 600;  //�J�n�ʒu
	FLOAT EndPos = 1000; //�I���ʒu
	FLOAT Density = 0.001f;
	g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
	g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0xFF, 0xCC, 0xBB, 0xAA)); //

	g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos)); //�J�n�ʒu
	g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));     //�I���ʒu
	g_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));

	g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP);      //���_���[�h
	g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);     //�e�[�u�����[�h

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// �f�o�b�O�\�������̏�����
	InitDebugProc();

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �t�F�[�h�̏�����
	InitFade(0);

	// �T�E���h�̏�����
	InitSound(hWnd);

	// �ŏ��̓^�C�g����ʂ�
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// �t�F�[�h�̏I������
	UninitFade();

	// �f�o�b�O�\���̏I������
	UninitDebugProc();

	// �T�E���h�̏I������
	UninitSound();

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͍X�V
	UpdateInput();

	// �`���[�g���A���y�[�W�̎擾
	TUTORIAL cnt = GetTutorial();

	// ��ʑJ��
	switch (g_mode)
	{
	case MODE_TITLE:
		// �^�C�g���̍X�V����
		UpdateTitle();
		break;

	case MODE_TUTORIAL:

		if (cnt == GAMEN)
		{//��ʐ����y�[�W�̍X�V����
			// �`���[�g���A���̍X�V����
			UpdateTutorial();
		}
		else
		{//������@�y�[�W�̍X�V����
			// �`���[�g���A���̍X�V����
			UpdateTutorial();

			//���C�g�����̍X�V
			UpdateLight();

			// �J�����X�V
			UpdateCamera();

			// �t�B�[���h�̕`�揈��
			// ��̕`�揈��
			UpdateMeshSky();

			// �n�ʂ̕`�揈��
			UpdateMeshField();

			// �ǂ̕`�揈��
			UpdateMeshWall();

			// �e�̕`�揈��
			UpdateShadow();

			// �v���C���[�̕`�揈��
			UpdatePlayer();

			// �G�t�F�N�g�̍X�V����
			UpdateEffect();
		}
		break;

	case MODE_GAME:
		// �Q�[���̍X�V����
		UpdateGame();
		break;

	case MODE_RESULT:
		// ���U���g�̍X�V����
		UpdateResult();
		break;
	}

	// �t�F�[�h����
	UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// �`���[�g���A���y�[�W�̎擾
		TUTORIAL cnt = GetTutorial();

		switch (g_mode)
		{
		case MODE_TITLE:
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
			// �^�C�g���̕`�揈��
			DrawTitle();
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
			break;

		case MODE_TUTORIAL:
			if (cnt == GAMEN)
			{//��ʐ����y�[�W�̍X�V����
				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
				// �`���[�g���A���̕`�揈��
				DrawTutorial();
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
			}
			else
			{//������@�y�[�W�̍X�V����
				// �J�����̐ݒ�
				SetCamera();

				// ��̕`�揈��
				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
				DrawMeshSky();

				// �Ǐ����̕`��
				DrawMeshWall();

				// �n�ʏ����̕`��
				DrawMeshField();

				// �G�t�F�N�g�̕`�揈��
				DrawEffect();

				// �e�̕`�揈��
				DrawShadow();
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

				// �v���C���[�̕`�揈��
				DrawPlayer();

				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
				// �`���[�g���A���̕`�揈��
				DrawTutorial();
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

			}
			break;

		case MODE_GAME:
			//�Q�[���̕`�揈��
			DrawGame();
			break;

		case MODE_RESULT:
			//���U���g�̕`�揈��
			g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FOFF
			DrawResult();
			g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
			break;
		}
		// �t�F�[�h�`��
		DrawFade();

#ifdef _DEBUG
		// �f�o�b�O�\��
		DrawDebugProc();
#endif

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode)
{
	g_mode = mode;

	switch (g_mode)
	{
	case MODE_TITLE:
		// ���U���g��ʂ̏I������
		UninitResult();

		// �^�C�g����ʂ̏�����
		InitTitle(0);

		break;

	case MODE_TUTORIAL:
		// �^�C�g����ʂ̏I������
		UninitTitle();

		// �`���[�g���A����ʂ̏�����
		InitTutorial(0);

		// ���C�g�̏�����
		InitLight();

		// �J�����̏�����
		InitCamera();

		// �t�B�[���h�̏�����
		// ��̏�����
		InitMeshSky(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 8, 8000.0f, true);

		// �n�ʂ̏�����
		InitMeshField();

		// �ǂ̏�����
		InitMeshWall();

		// �e�̏�����
		InitShadow();

		// �v���C���[�̏�����
		InitPlayer(0);

		// �G�t�F�N�g�̏�����
		InitEffect();

		break;

	case MODE_GAME:

		// �J�����̏I������
		UninitCamera();

		// �`���[�g���A����ʂ̏I������
		UninitTutorial();

		// �t�B�[���h�̏I������
		// �n�ʂ̏I������
		UninitMeshField();

		// �ǂ̏I������
		UninitMeshWall();

		// �e�̏I������
		UninitShadow();

		// �v���C���[�̏I������
		UninitPlayer();

		// �G�t�F�N�g�̏I������
		UninitEffect();

		// �Q�[����ʂ̏�����
		InitGame();

		break;

	case MODE_RESULT:
		// �Q�[����ʂ̏I������
		UninitGame();

		// ���U���g��ʂ̏�����
		InitResult(0);

		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}
