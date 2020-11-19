//添加纹理的卢浮宫金字塔
//Pyramid.cpp

#include "d3dUtility.h"
#include <string>
#include <iostream>

//Globals
//
const int Width = 640;
const int Height = 480;
const bool g_bIsWindowed = true;
std::string python_message = "";
bool quit = false;



//
D3DXMATRIX matWorld;
IDirect3DDevice9 *Device = nullptr;
IDirect3DVertexBuffer9 *Pyramid = nullptr;
IDirect3DTexture9 *tex = nullptr;

//define vertex format
struct NormalVertex
{
	float _x, _y, _z;
	float nx, ny, nz;
	float u, v;
	static const DWORD FVF;
};

const DWORD NormalVertex::FVF = { D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };

bool Setup()
{
	//create vertex buffer
	Device->CreateVertexBuffer(
		12 * sizeof(NormalVertex),
		D3DUSAGE_WRITEONLY,
		NormalVertex::FVF,
		D3DPOOL_MANAGED,
		&Pyramid,
		NULL
	);

	//fill the buffer with data
	NormalVertex *v;
	Pyramid->Lock(0, 0, (void**)&v, 0);
	// front face
	v[0] = NormalVertex{ -1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f, 0.1f, 0.6f };
	v[1] = NormalVertex{ 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f, 0.5f, 0.2f };
	v[2] = NormalVertex{ 1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f, 0.9f, 0.6f };
	// left face
	v[3] = NormalVertex{ -1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f , 0.1f, 0.6f };
	v[4] = NormalVertex{ 0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f, .5f, 0.2f };
	v[5] = NormalVertex{ -1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f ,0.9f, 0.6f };
	// right face
	v[6] = NormalVertex{ 1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f,  0.1f, 0.6f };
	v[7] = NormalVertex{ 0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f , .5f, 0.2f };
	v[8] = NormalVertex{ 1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f , 0.9f, 0.6f };
	// back face
	v[9] = NormalVertex{ 1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.1f, 0.6f };
	v[10] = NormalVertex{ 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f , .5f, 0.2f };
	v[11] = NormalVertex{ -1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f , 0.9f, 0.6f };

	Pyramid->Unlock();

	//set the material
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient = d3d::PYRAMID;
	mtrl.Diffuse = d3d::PYRAMID;
	mtrl.Specular = d3d::PYRAMID;
	mtrl.Emissive = d3d::BLACK;
	mtrl.Power = 5.0f;
	Device->SetMaterial(&mtrl);

	//set the light
	D3DLIGHT9 dir;
	ZeroMemory(&dir, sizeof(D3DLIGHT9));
	dir.Type = D3DLIGHT_POINT;
	dir.Diffuse = d3d::PYRAMID;
	dir.Specular = d3d::PYRAMID * 0.3f;
	dir.Ambient = d3d::PYRAMID * 0.6f;
	//dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	dir.Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	dir.Range = 1.7f;
	dir.Attenuation0 = .7f;
	//dir.Falloff = 0.5f;
	//dir.Theta = 0.25f;
	//dir.Phi = 4.0f;
	Device->SetLight(0, &dir);
	Device->LightEnable(0, true);

	//texture
	D3DXCreateTextureFromFile(Device, "C:\\Users\\Administrator\\Pictures\\Saved Pictures\\pyramid.jpg", &tex);
	Device->SetTexture(0, tex);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	//set the projection
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)Width / (float)Height, 1.0f, 1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//set the camara
	D3DXVECTOR3 position(0.0f, 1.0f, 2.0f);
	D3DXVECTOR3 target(0.0f, 0.25f, 1.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

	//turn off the culling
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//光源操作
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);//规格化
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);//开启镜面光

													   //灯光开关
	Device->SetRenderState(D3DRS_LIGHTING, false);


	return true;
}

bool Display(float timeDelta)
{
	if (Device)
	{

		//clear buffer
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);//0到f的组合可以产生不同颜色的背景
																					 //begin scene
		Device->BeginScene();
		//prepare
		Device->SetFVF(NormalVertex::FVF);
		Device->SetStreamSource(0, Pyramid, 0, sizeof(NormalVertex));

		std::cin >> python_message;
		//std::cout << python_message;
		//rolation 
		if (python_message == "OK")
		{
			
			static float y = 0.0f;
			y += timeDelta * 3.0f;
			if (y >= 6.28f)
				y = 0.0f;
			D3DXMatrixRotationY(&matWorld, y);
			Device->SetTransform(D3DTS_WORLD, &matWorld);
			
		}
		else if (python_message == "NOTHING")
		{
			//std::cout << python_message;
			static float y = 0.0f;
			D3DXMatrixRotationY(&matWorld, y);
			Device->SetTransform(D3DTS_WORLD, &matWorld);
		}

	 //   static float y = 0.0f;
		//y += timeDelta * 3.0f;
		//if (y >= 6.28f)
		//y = 0.0f;
		//D3DXMatrixRotationY(&matWorld, y);
		//Device->SetTransform(D3DTS_WORLD, &matWorld);

		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

		Device->EndScene();
		Device->Present(NULL, NULL, NULL, NULL);

	}
	return true;
}

void Cleanup()
{
	Device->Release();
}





LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain主函数
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, g_bIsWindowed, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	/*std::cin >> python_message;*/
	d3d::EnterMsgLoop(Display);
	
	Cleanup();

	Device->Release();

	return 0;
}
