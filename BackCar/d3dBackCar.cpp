#include"d3dUtility.h"
#include "Camera.h"
#include "ObjLoader.h"

ID3D11Device *device = NULL;  //D3D11设备接口
IDXGISwapChain *swapChain = NULL;  //交换链接口
ID3D11DeviceContext *immediateContext = NULL;
ID3D11RenderTargetView *renderTargetView = NULL;  //渲染目标视图

//Effect相关全局指针
ID3D11InputLayout *vertexLayout;
ID3DX11Effect *effect;
ID3DX11EffectTechnique *technique;

//声明三个坐标系矩阵
XMMATRIX world; //用于世界变换的矩阵
XMMATRIX view;  //用于观察变换的矩阵
XMMATRIX projection;  //用于投影变换的矩阵

ID3D11BlendState *blendStateAlpha;        //混合状态
ID3D11RasterizerState* noCullRS;          //背面消隐状态

ID3D11DepthStencilView* depthStencilView;  //深度模板视图

ID3D11ShaderResourceView *texture;  //公路的纹理变量
ID3D11ShaderResourceView *textureGrass;  //广场的纹理变量
ID3D11ShaderResourceView *textureCar;  //汽车的纹理变量
ID3D11ShaderResourceView *textureHeart1; //生命值存在的纹理变量
ID3D11ShaderResourceView *textureHeart2; //生命值失去的纹理变量
ID3D11ShaderResourceView *texturehouse1Front;  //房子的纹理变量
ID3D11ShaderResourceView *texturehouse1Right;
ID3D11ShaderResourceView *texturehouse1Left;
ID3D11ShaderResourceView *texturehouse1Top;
ID3D11ShaderResourceView *texturehouse1Behand;
ID3D11ShaderResourceView *textureNumber;//数字的纹理变量
ID3D11ShaderResourceView *textureRoad;
ID3D11ShaderResourceView *textureRoad3;
ID3D11ShaderResourceView *textureTime0;//计时纹理
ID3D11ShaderResourceView *textureTime1;
ID3D11ShaderResourceView *textureTime2;
ID3D11ShaderResourceView *textureTime3;
ID3D11ShaderResourceView *textureTime4;
ID3D11ShaderResourceView *textureTime5;
ID3D11ShaderResourceView *textureTime6;
ID3D11ShaderResourceView *textureTime7;
ID3D11ShaderResourceView *textureTime8;
ID3D11ShaderResourceView *textureTime9;

//定义一个顶点结构，这个顶点包含坐标和法向量
struct Vertex
{
	XMFLOAT3 Pos;  //坐标
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;  //纹理坐标
};

ObjLoader* objLoaderBus;
Camera* camera = new Camera();    //摄像机对象
XMVECTOR Eye;                     //视点位置

bool Setup()
{
	//****第一步:从.fx文件创建ID3DEffect对象****//
	HRESULT hr = S_OK;  //声明HRESULT的对象用于记录函数调用是否成功
	ID3DBlob *pTechBlob = NULL;  //声明ID3DBlob的对象用于存放从文件读取的信息
	hr = D3DX11CompileFromFile(L"Shader.fx", NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pTechBlob, NULL, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx文件载入失败", L"Error", MB_OK);
		return hr;
	}

	//调用D3DX11CreateEffectFromMemory创建ID3DEffect对象
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(),
		0,
		device,
		&effect
		);

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Effect失败", L"Error", MB_OK);
		return hr;
	}

	//调用D3DX11CreateShaderResourceViewFromFile从BOX.BMP文件创建纹理

	D3DX11CreateShaderResourceViewFromFile(device, L"road.jpg", NULL, NULL, &textureRoad, NULL);
	
	D3DX11CreateShaderResourceViewFromFile(device, L"road3.jpg", NULL, NULL, &textureRoad3, NULL);

	D3DX11CreateShaderResourceViewFromFile(device,L"road2.jpg",NULL,NULL,&texture,NULL);

	D3DX11CreateShaderResourceViewFromFile(device,L"BOX.bmp",NULL,NULL,&textureCar,NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"grass.jpg", NULL, NULL, &textureGrass, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1front.jpg", NULL, NULL, &texturehouse1Front, NULL);
	
	D3DX11CreateShaderResourceViewFromFile(device, L"house1right.jpg", NULL, NULL, &texturehouse1Right, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1left.jpg", NULL, NULL, &texturehouse1Left, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1top.jpg", NULL, NULL, &texturehouse1Top, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"house1behand.jpg", NULL, NULL, &texturehouse1Behand, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"心1.jpg", NULL, NULL, &textureHeart1, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"心2.jpg", NULL, NULL, &textureHeart2, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"数字纹理.jpg", NULL, NULL, &textureNumber, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time0.jpg", NULL, NULL, &textureTime0, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time1.jpg", NULL, NULL, &textureTime1, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time2.jpg", NULL, NULL, &textureTime2, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time3.jpg", NULL, NULL, &textureTime3, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time4.jpg", NULL, NULL, &textureTime4, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time5.jpg", NULL, NULL, &textureTime5, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time6.jpg", NULL, NULL, &textureTime6, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time7.jpg", NULL, NULL, &textureTime7, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time8.jpg", NULL, NULL, &textureTime8, NULL);

	D3DX11CreateShaderResourceViewFromFile(device, L"time9.jpg", NULL, NULL, &textureTime9, NULL);

	//*************第三步创建输入布局****************************************************
	//用GetTechniqueByName获取ID3DX11EffectTechnique的对象
	technique = effect->GetTechniqueByName("TexTech");

	//D3DX11_PASS_DESC结构用于描述一个Effect Pass
	D3DX11_PASS_DESC PassDesc;
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//创建并设置输入布局
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//layout元素个数
	UINT numElements = ARRAYSIZE(layout);

	//调用CreateInputLayout创建输入布局
	hr = device->CreateInputLayout
		(
		layout,
		numElements,
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&vertexLayout
		);
	//设置生成的输入布局到执行上下文中
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Input Layout失败", L"Error", MB_OK);
		return hr;
	}

	//*************第三步创建输入布局****************************************************

	//*************第四步创建顶点缓存****************************************************
static float typr = 2.0f;
static float typgrass = 2.0f;
static float xhouse1 =10.0f;
static float zhouse1 =10.0f;
static float typghouse = 1.0f;
static float tyWidth = 9.0f;
static float tyHeight = 1.0f;
static float typtime = 0.1f;
static float xtime = 3.0f;
static float ytime = 5.0f;
static float ztime = 1.0f;
static float tyHeart = 8.0f;
	Vertex vertices[] =
	{
		

		{ XMFLOAT3(1.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 0.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		//8
		{ XMFLOAT3(1.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(3.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(3.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(4.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(4.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(5.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(5.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(6.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		//19
		{ XMFLOAT3(6.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		//
		{ XMFLOAT3(6.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(8.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(9.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 1.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		//25
		{ XMFLOAT3(8.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(9.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 2.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
//28


		{ XMFLOAT3(9.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 3.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 4.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 5.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 6.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		//44
		{ XMFLOAT3(11.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(11.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(12.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(12.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(12.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(8.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(7.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(6.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(7.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(5.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(6.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(4.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(5.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(3.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(4.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(2.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(1.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(0.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(0.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		//66*6

		//grass
		{ XMFLOAT3(0.0f*typgrass, 0.0f, 0.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typgrass, 0.0f, 15.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 15.0f) },
		{ XMFLOAT3(15.0f*typgrass, 0.0f, 15.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(15.0f, 15.0f) },
		{ XMFLOAT3(0.0f*typgrass, 0.0f, 0.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(15.0f*typgrass, 0.0f, 15.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(15.0f, 15.0f) },
		{ XMFLOAT3(15.0f*typgrass, 0.0f, 0.0f*typgrass), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(15.0f, 0.0f) },

		
		//house1
		//front
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse +  zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse +  zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse , 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//behand
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//left
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//right
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 0.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//top
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 0.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typghouse + xhouse1, 4.0f*typghouse, 4.0f*typghouse + zhouse1), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//car 36
		//car  36
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 3.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		//heart front
		{ XMFLOAT3(10.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
				   				  
		{ XMFLOAT3(10.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(11.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f, 4.0f+tyHeart ,20.0f), XMFLOAT3(0.0f, 0.0f,1.0f), XMFLOAT2(1.0f, 1.0f) },
										  
		{ XMFLOAT3(11.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(12.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(11.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
								
		{ XMFLOAT3(11.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(12.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
									  
		{ XMFLOAT3(12.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(13.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(12.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
								
		{ XMFLOAT3(12.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(13.0f, 5.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(13.0f, 4.0f+tyHeart, 20.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		////heart behind
		//{ XMFLOAT3(10.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		//{ XMFLOAT3(11.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(10.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		//{ XMFLOAT3(10.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		//{ XMFLOAT3(11.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(11.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		//{ XMFLOAT3(11.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		//{ XMFLOAT3(12.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(11.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		//{ XMFLOAT3(11.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		//{ XMFLOAT3(12.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(12.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		//{ XMFLOAT3(12.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		//{ XMFLOAT3(13.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(12.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		//{ XMFLOAT3(12.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		//{ XMFLOAT3(13.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		//{ XMFLOAT3(13.0f, 4.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		// 道路补充：

		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },


		{ XMFLOAT3(9.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(10.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 13.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(10.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(11.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },




		{ XMFLOAT3(13.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 7.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(13.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 8.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(13.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(14.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		

		//停车场
		{ XMFLOAT3(1.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 14.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		//540
		//入口1
		{ XMFLOAT3(2.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 9.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(2.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typr, 0.0f, 10.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		
		//入口2
		{ XMFLOAT3(8.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 11.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(8.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(9.0f*typr, 0.0f, 12.0f*typr), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		//552

		//计时
		{ XMFLOAT3(0.0f*typtime + xtime, 0.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typtime + xtime, 4.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typtime + xtime, 0.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(3.0f*typtime + xtime, 0.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(0.0f*typtime + xtime, 4.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(3.0f*typtime + xtime, 4.0f*typtime + ytime, 0.0f*typtime + ztime), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

	};
	UINT vertexCount = ARRAYSIZE(vertices);

	//创建顶点缓存
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vertexCount;  //定义了36个顶点
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//声明一个数据用于初始化子资源
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	//声明一个D3D11Buffer对象作为顶点缓存
	ID3D11Buffer *vertexBuffer;
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建VertexBuffer失败", L"Error", MB_OK);
		return hr;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//设置顶点缓存
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//指定图元类型
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//*************创建obj对象并载入图像文件***************************************
	/*objLoaderBus = new ObjLoader(effect);
	objLoaderBus->Load(device, "media/", "file.obj", ObjLoader::Left_Hand_System);*/

	Eye = XMVectorSet(0.0f, 3.0f, -5.0f, 0.0f);//相机位置

	camera->SetEye(Eye);   //设置视点位置

}

void Cleanup()
{
	//释放全局指针
	if (renderTargetView)renderTargetView->Release();
	if (immediateContext)immediateContext->Release();
	if (swapChain)swapChain->Release();
	if (device)device->Release();

	if (vertexLayout)vertexLayout->Release();
	if (effect)effect->Release();
	if (texture)texture->Release();
	if (textureCar)textureCar->Release();
}

bool Display(float timeDelta  )
{
	if (device)
	{
		float ClearColor[4] = { 0.0f, 0.5f, 1.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		//先指定混合因子，一般不用它，除非在上面混合因子指定为使用blend factor
		float BlendFactor[] = { 0, 0, 0, 0 }; 

		ID3D11ShaderResourceView *textureT[] = { textureTime0, textureTime1, textureTime2,
			textureTime3, textureTime4, textureTime5, textureTime6, textureTime7,
			textureTime8, textureTime9 };
		/*static float angle = XM_PI;
		static float height = 5.0f;
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 2.0f*timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 2.0f*timeDelta;*/
		/*if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f*timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f*timeDelta;*/

		//if (height < 0.0f) height = 0.f;   //限制镜头最远距离
		//if (height > 12.0f) height = 12.f;     //限制镜头最近距离

		//****第三步，设置变换坐标系****
		//****设置各个坐标系****//
		//初始化世界矩阵
		//world = XMMatrixIdentity();

		////初始化观察矩阵
		//XMVECTOR Eye = XMVectorSet(8.0f, 5.0f, -3.0f, 0.0f);//相机位置
		//XMVECTOR At = XMVectorSet(8.0f, 5.0f, 8.0f, 0.0f);//目标位置
		//XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//相机正上方位置
		//view = XMMatrixLookAtLH(Eye, At, Up);  //设置观察坐标系

		////设置投影矩阵
		//projection = XMMatrixPerspectiveFovLH(
		//	XM_PIDIV2,
		//	800.0f / 600.0f,
		//	0.01f,
		//	100.0f);
		////设置世界坐标系
		//effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);

		////设置观察坐标系
		//effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);

		////设置投影坐标系
		//effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection);

		//通过键盘改变虚拟摄像头方向.
		if (::GetAsyncKeyState('A') & 0x8000f)  //向左移动
		{
			camera->MoveRightBy(-timeDelta * 5.0f);
		}

		if (::GetAsyncKeyState('D') & 0x8000f)  //向右移动
		{
			camera->MoveRightBy(timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('W') & 0x8000f)  //向前移动
		{
			camera->MoveForwardBy(timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('S') & 0x8000f)  //向后移动
		{
			camera->MoveForwardBy(-timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('Q') & 0x8000f)  //向上移动
		{
			camera->MoveUpBy(timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('E') & 0x8000f) //向下移动
		{
			camera->MoveUpBy(-timeDelta* 5.0f);
		}

		if (::GetAsyncKeyState('Z') & 0x8000f) //向右摆动
		{
			camera->Yaw(timeDelta);
		}

		if (::GetAsyncKeyState('X') & 0x8000f) //向左摆动
		{
			camera->Yaw(-timeDelta);
		}

		if (::GetAsyncKeyState('C') & 0x8000f) //上仰
		{
			camera->Pitch(timeDelta);
		}

		if (::GetAsyncKeyState('V') & 0x8000f) //下俯
		{
			camera->Pitch(-timeDelta);
		}
		//重新生成观察矩阵
		camera->Apply();

		//初始化世界矩阵
		world = XMMatrixIdentity();

		//设置投影矩阵
		projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 800.0f / 600.0f, 0.01f, 100.0f);
		//将坐标变换矩阵的常量缓存中的矩阵和坐标设置到Effect框架中---------------------
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		XMMATRIX ViewMATRIX = XMLoadFloat4x4(&camera->GetView());
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&ViewMATRIX);    //设置观察坐标系
		effect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&Eye); //设置视点
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //设置投影坐标系
		//定义一个D3DX11_TECHNIQUE_DESC对象来描述technique
		//D3DX11_TECHNIQUE_DESC techDesc;
		//technique->GetDesc(&techDesc);  //获取technique的描述
		//technique->GetPassByIndex(0)->Apply(0, immediateContext);
		
		static float beginTime = (float)timeGetTime();
		//绘制草地纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureGrass);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 396);
		//绘制公路、纹理
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texture);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(396, 0);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Front);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(12, 402);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Left);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 414);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Right);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 420);

		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texturehouse1Top);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 426);

		//绘制汽车、纹理
		static float transX = 0.0f;
		static float transZ = 0.0f;
		static float angle_car = XM_PI / 2;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		{
			if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
				angle_car -= 0.5f * timeDelta;
			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
				angle_car += 0.5f * timeDelta;
			transX -= cos(angle_car) * 5.0f * timeDelta;
			transZ += sin(angle_car) * 5.0f * timeDelta;

		}

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		{
			if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
				angle_car += 0.5f * timeDelta;
			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
				angle_car -= 0.5f * timeDelta;
			transX += cos(angle_car) * 5.0f * timeDelta;
			transZ -= sin(angle_car) * 5.0f * timeDelta;
		}

		world = XMMatrixIdentity();
		world = XMMatrixRotationY(angle_car)*XMMatrixTranslation(transX + 4.0f, 0.0f, transZ+4.0f)*XMMatrixScaling(0.5f, 0.5f, 0.5f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureCar);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(36, 432);
		//objLoaderBus->RenderEffect(immediateContext, technique);

		world = XMMatrixIdentity();
		//world = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 468);
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 474);
		if (transZ>5.0)
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart2);
		else effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart1);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 480);

		//world = XMMatrixIdentity();
		//world = XMMatrixScaling(0.5f, 0.5f, 0.5f);// *XMMatrixTranslation(0.0f, height - 5.0f, 0.0f);
		//effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		//effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureNumber);
		//technique->GetPassByIndex(0)->Apply(0, immediateContext);
		//immediateContext->Draw(6, 486);

		/*float endTime = (float)timeGetTime();
		float timeInterval = endTime - beginTime;*/
		//if (transZ > 5.0f)
		//{
		//	world = XMMatrixIdentity();
		//	world = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		//	effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		//	effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureHeart2);
		//	technique->GetPassByIndex(0)->Apply(0, immediateContext);
		//	immediateContext->Draw(6, 480);
		//}

		//绘制补充公路
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(texture);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(48, 486);
		//绘制停车场
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad3);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 534);
		//入口1
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 540);
		//入口2
		effect->GetVariableByName("Texture")->AsShaderResource()->SetResource(textureRoad);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 546);

		//绘制计时器
		static int timeboard[4] = { 0, 0, 0, 0 };
		float GameTime = 0.0f;
		GameTime += timeDelta;
		int GameTimer;
		if (GameTime<10000.0f)
		{
			GameTimer = (int)GameTime;
			for (int i = 0; i < 4; i++)
			{

				timeboard[i] = GameTimer % 10;
				GameTimer /= 10;
			}
		}

		world = XMMatrixIdentity();
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[3]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);

		world = world* XMMatrixTranslation(0.3, 0.0f, 0.0f);//
		effect->GetVariableByName("World")->
			AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[2]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);


		world = world* XMMatrixTranslation(0.3, 0.0f, 0.0f);//
		effect->GetVariableByName("World")->
			AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[1]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);


		world = world* XMMatrixTranslation(0.3, 0.0f, 0.0f);//
		effect->GetVariableByName("World")->
			AsMatrix()->SetMatrix((float*)&world);
		effect->GetVariableByName("Texture")->AsShaderResource()
			->SetResource(textureT[timeboard[0]]);
		technique->GetPassByIndex(0)->Apply(0, immediateContext);
		immediateContext->Draw(6, 552);

		swapChain->Present(0, 0);
	}
	return true;
}

//回调函数
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM IParam)
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
	return::DefWindowProc(hwnd, msg, wParam, IParam);
}


int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		800,
		600,
		&renderTargetView,
		&immediateContext,
		&swapChain,
		&device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	return 0;
}