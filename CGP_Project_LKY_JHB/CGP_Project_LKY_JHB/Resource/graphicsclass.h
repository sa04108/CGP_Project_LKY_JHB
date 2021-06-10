////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <time.h>
#include <cmath>

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"

#include "textclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
typedef struct Position {
	float x;
	float y;
	float z;
} Pos;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);
	bool Frame(int, int, float);
	bool Render();
	bool Render(float);

	bool ReinitializeBarrel();
	void GainBarrel(float&);
	void SetFuelUp(float&);

	void KnuthShuffle();
	void SetSpaceshipSpeed(float);
	void SetSpaceshipLeft();
	void SetSpaceshipRight();

	void SetCameraPos(float, float, float);
	void SetCameraRotation(float, float, float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model_SolarSystem;
	ModelClass* m_Model_Earth;
	ModelClass* m_Model_Spaceship;
	ModelClass* m_Model_Barrel;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap_Fuel;
	BitmapClass* m_Bitmap_Fuel_Empty;
	BitmapClass* m_Bitmap_Background;
	BitmapClass* m_Bitmap_Gameclear;
	BitmapClass* m_Bitmap_Gameover;

	TextClass* m_Text;
	InstanceType* instances;

	int barrelCount;
	int barrelPosNum[3];

	int modelCount;
	
	int max_barrel_gen;
	int m_frameTime;
	int m_second;
	float deltaTime;

	float spaceshipSpeed;
	float spaceshipSideSpeed;
	float spaceshipMaxPosX;
	float spaceshipMaxPosY;
	float spaceshipMaxRotation;

	float s_trans_x;
	float s_trans_y;
	float s_rotation_y;

	bool isGameover;
};

#endif