//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 濱崎 小春
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
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"Wonder Alice 3D"		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
//#ifdef _DEBUG
int					g_nCountFPS;			// FPSカウンタ
MODE				g_mode = MODE_TITLE;	// モード
//#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

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

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
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

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
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
				// FPS表示
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定


	//フォグの設定
	FLOAT StartPos = 600;  //開始位置
	FLOAT EndPos = 1000; //終了位置
	FLOAT Density = 0.001f;
	g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
	g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0xFF, 0xCC, 0xBB, 0xAA)); //

	g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos)); //開始位置
	g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));     //終了位置
	g_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));

	g_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP);      //頂点モード
	g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);     //テーブルモード

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// デバッグ表示処理の初期化
	InitDebugProc();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// フェードの初期化
	InitFade(0);

	// サウンドの初期化
	InitSound(hWnd);

	// 最初はタイトル画面に
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// フェードの終了処理
	UninitFade();

	// デバッグ表示の終了処理
	UninitDebugProc();

	// サウンドの終了処理
	UninitSound();

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力更新
	UpdateInput();

	// チュートリアルページの取得
	TUTORIAL cnt = GetTutorial();

	// 画面遷移
	switch (g_mode)
	{
	case MODE_TITLE:
		// タイトルの更新処理
		UpdateTitle();
		break;

	case MODE_TUTORIAL:

		if (cnt == GAMEN)
		{//画面説明ページの更新処理
			// チュートリアルの更新処理
			UpdateTutorial();
		}
		else
		{//操作方法ページの更新処理
			// チュートリアルの更新処理
			UpdateTutorial();

			//ライト処理の更新
			UpdateLight();

			// カメラ更新
			UpdateCamera();

			// フィールドの描画処理
			// 空の描画処理
			UpdateMeshSky();

			// 地面の描画処理
			UpdateMeshField();

			// 壁の描画処理
			UpdateMeshWall();

			// 影の描画処理
			UpdateShadow();

			// プレイヤーの描画処理
			UpdatePlayer();

			// エフェクトの更新処理
			UpdateEffect();
		}
		break;

	case MODE_GAME:
		// ゲームの更新処理
		UpdateGame();
		break;

	case MODE_RESULT:
		// リザルトの更新処理
		UpdateResult();
		break;
	}

	// フェード処理
	UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// チュートリアルページの取得
		TUTORIAL cnt = GetTutorial();

		switch (g_mode)
		{
		case MODE_TITLE:
			pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
			// タイトルの描画処理
			DrawTitle();
			pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
			break;

		case MODE_TUTORIAL:
			if (cnt == GAMEN)
			{//画面説明ページの更新処理
				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
				// チュートリアルの描画処理
				DrawTutorial();
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
			}
			else
			{//操作方法ページの更新処理
				// カメラの設定
				SetCamera();

				// 空の描画処理
				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
				DrawMeshSky();

				// 壁処理の描画
				DrawMeshWall();

				// 地面処理の描画
				DrawMeshField();

				// エフェクトの描画処理
				DrawEffect();

				// 影の描画処理
				DrawShadow();
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

				// プレイヤーの描画処理
				DrawPlayer();

				pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
				// チュートリアルの描画処理
				DrawTutorial();
				pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

			}
			break;

		case MODE_GAME:
			//ゲームの描画処理
			DrawGame();
			break;

		case MODE_RESULT:
			//リザルトの描画処理
			g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：OFF
			DrawResult();
			g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
			break;
		}
		// フェード描画
		DrawFade();

#ifdef _DEBUG
		// デバッグ表示
		DrawDebugProc();
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	g_mode = mode;

	switch (g_mode)
	{
	case MODE_TITLE:
		// リザルト画面の終了処理
		UninitResult();

		// タイトル画面の初期化
		InitTitle(0);

		break;

	case MODE_TUTORIAL:
		// タイトル画面の終了処理
		UninitTitle();

		// チュートリアル画面の初期化
		InitTutorial(0);

		// ライトの初期化
		InitLight();

		// カメラの初期化
		InitCamera();

		// フィールドの初期化
		// 空の初期化
		InitMeshSky(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 8, 8000.0f, true);

		// 地面の初期化
		InitMeshField();

		// 壁の初期化
		InitMeshWall();

		// 影の初期化
		InitShadow();

		// プレイヤーの初期化
		InitPlayer(0);

		// エフェクトの初期化
		InitEffect();

		break;

	case MODE_GAME:

		// カメラの終了処理
		UninitCamera();

		// チュートリアル画面の終了処理
		UninitTutorial();

		// フィールドの終了処理
		// 地面の終了処理
		UninitMeshField();

		// 壁の終了処理
		UninitMeshWall();

		// 影の終了処理
		UninitShadow();

		// プレイヤーの終了処理
		UninitPlayer();

		// エフェクトの終了処理
		UninitEffect();

		// ゲーム画面の初期化
		InitGame();

		break;

	case MODE_RESULT:
		// ゲーム画面の終了処理
		UninitGame();

		// リザルト画面の初期化
		InitResult(0);

		break;
	}
}

//=============================================================================
// モードの取得
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}
