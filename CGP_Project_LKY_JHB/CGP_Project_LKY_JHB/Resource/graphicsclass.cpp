////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;

	m_TextureShader = 0;
	m_Bitmap_Fuel = 0;
	m_Bitmap_Fuel_Empty = 0;
	m_Bitmap_Background = 0;

	m_Text = 0;
	m_Timer = 0;
	modelCount = 0;
	m_startTime = 0;
	m_second = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
//	m_Camera->SetPosition(0.0f, 0.5f, -3.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), (char *)"data/mars.obj", (wchar_t *)L"data/mars.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/chair.txt", L"../Engine/data/chair_d.dds");

	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	modelCount++;

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);


	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	
	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	m_Bitmap_Fuel = new BitmapClass;
	if (!m_Bitmap_Fuel)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap_Fuel->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, (wchar_t*)L"data/fuel.png", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap_Fuel_Empty = new BitmapClass;
	if (!m_Bitmap_Fuel_Empty)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap_Fuel_Empty->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, (wchar_t*)L"data/fuel_empty.png", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap_Background = new BitmapClass;
	if (!m_Bitmap_Background)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap_Background->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, (wchar_t*)L"data/spacemap.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
//	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer.", L"Error", MB_OK);
		return false;
	}

	m_startTime = timeGetTime();

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the bitmap object.
	if (m_Bitmap_Fuel)
	{
		m_Bitmap_Fuel->Shutdown();
		delete m_Bitmap_Fuel;
		m_Bitmap_Fuel = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap_Fuel_Empty)
	{
		m_Bitmap_Fuel_Empty->Shutdown();
		delete m_Bitmap_Fuel_Empty;
		m_Bitmap_Fuel_Empty = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap_Background)
	{
		m_Bitmap_Background->Shutdown();
		delete m_Bitmap_Background;
		m_Bitmap_Background = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_Timer)
	{
		m_Timer->~TimerClass();
		delete m_Timer;
		m_Timer = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetObjNum(modelCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetPolygonNum(m_Model->GetPolygonCount(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetScreenSize(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetTime((int)m_Timer->GetTime(), m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	m_Timer->Frame();

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_startTime = timeGetTime();
		m_second++;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	// Set the position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX rotationMatrix, translationMatrix, scaleMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

#pragma region Background Bitmap
	D3DXMatrixScaling(&scaleMatrix, 4.0f, 4.0f, 1.0f);

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap_Background->Render(m_D3D->GetDeviceContext(), 300, 400);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap_Background->GetIndexCount(), scaleMatrix * worldMatrix, viewMatrix, orthoMatrix, m_Bitmap_Background->GetTexture());
	if (!result)
	{
		return false;
	}
#pragma endregion

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

#pragma region Fuel Empty Bitmap
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 0.5f, 1.0f);

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap_Fuel_Empty->Render(m_D3D->GetDeviceContext(), 900, 800);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap_Fuel_Empty->GetIndexCount(), scaleMatrix * worldMatrix, viewMatrix, orthoMatrix, m_Bitmap_Fuel_Empty->GetTexture());
	if (!result)
	{
		return false;
	}
#pragma endregion

#pragma region Fuel Bitmap
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 0.5f, 1.0f);

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap_Fuel->Render(m_D3D->GetDeviceContext(), 900, 800, 0.7f);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap_Fuel->GetIndexCount(), scaleMatrix * worldMatrix, viewMatrix, orthoMatrix, m_Bitmap_Fuel->GetTexture());
	if (!result)
	{
		return false;
	}
#pragma endregion

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
		
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationX(&rotationMatrix, -rotation);

	D3DXMatrixTranslation(&translationMatrix, 0.0f, 0.0f, 50.0f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), rotationMatrix * translationMatrix * worldMatrix, viewMatrix, projectionMatrix, 
								   m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
								   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}