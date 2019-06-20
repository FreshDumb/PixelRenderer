//#include "../../../external/SDL2-2.0.8/include/SDL.h"
//#include "../../../external/SDL2_net-2.0.1/include/SDL_net.h"

#include<vector>
#include"MathOli.h"
#include <math.h>
#include"Color.h"
#include <sstream>
#include <cstring>

///
/// Method to color a pixel in the window
void DrawPixel(SDL_Window *W, int x, int y, Uint32 color)
{
	SDL_Rect R; R.x = x; R.y = y; R.w = 1; R.h = 1;
	SDL_FillRect(SDL_GetWindowSurface(W), &R, color);
}

#define WIDTH 480
#define HEIGHT 480
#define PI 3.14159265
#define TEXTURED true // Should the object be Textured
#define CAMERALIT true // If true Camera lights the Object
int main(int, char **)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *W = SDL_CreateWindow("SDL-Test", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_FillRect(SDL_GetWindowSurface(W), 0, 0);

	const int GridX = 20;
	const int GridY = 20;
	float Grid[GridX][GridY];

	for (int y = 0; y < GridX; y++)
		for (int x = 0; x < GridY; x++)
			Grid[x][y] = -666;

	Vector3 CameraPosition;
	float CameraDistance = 170;
	Vector3 CameraHeight = Vector3(0, 120, 0);

	/// Camera position
	// Values required for position of the Camera
	// Camera Height is base Vector of the Camera determining height
	// Then a direction Vector is calculated based on a rotation degree
	float CamRotateDegree = 0;
	Vector3 CameraDirection = Vector3(sin(CamRotateDegree*PI / 180), 0 , cos(CamRotateDegree*PI / 180));
	CameraPosition = CameraHeight +  (CameraDirection * CameraDistance);
	Vector3 Direction = Vector3(0, 0, -1);
	Vector3 LookTarget = Vector3(0, 60, 0);


	//
	// Position of Light
	Vector3 LightPos = Vector3(50, 100, 100);

	Vector3 A = Vector3(0, 1, 0);
	Vector3 B = Vector3(1, 0, 0);
	Vector3 C = Vector3(0, 0, 0);

	Triangle Test = Triangle(A, B, C);

	std::ifstream InFile("dat/Teapot.obj");
	if (!InFile.good())
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Import", "Failed to open file", 0);
	Mesh testMesh;
	testMesh.ImportMesh(InFile, testMesh);

	SDL_Surface* Texture;
	if (TEXTURED) {
		Texture = SDL_LoadBMP("dat/Texture.bmp");
		if (Texture == NULL) {
			std::cout << "SHIAT" << "\n";
		}
	}

	//
	// Debug messages for loading the Mesh
	std::cout << testMesh.Faces.size() << "\n";
	std::cout << testMesh.Normals.size() << "\n";
	std::cout << testMesh.TextureCoords.size() << "\n";
	std::cout << testMesh.Vertices.size() << "\n";

	/// RayHit object instance storing the Rays information on hit
	RayHit tempHit = RayHit();
	/// Variable to store bool if a Ray hits the Mesh
	bool outputValue = false;

	/// Used for debugging purposes
	//Vector3 forward = Vector3::normalize(CameraPosition - Vector3(0, 20, 0));
	//Vector3 right = Vector3::crossProduct(Vector3(0, 1, 0), forward);
	//Vector3 up = Vector3::crossProduct(forward, right);

	int xSize = (WIDTH / GridX);
	int ySize = (HEIGHT / GridY);

	float LerpXValues[GridX];
	float LerpYValues[GridY];
	for (int y = 0; y < GridY; y++) {
		LerpYValues[y] = MathOli::lerpValues(1, -1, y / (float)GridY);
	}
	for (int x = 0; x < GridX; x++) {
		LerpXValues[x] = MathOli::lerpValues(1, -1, x / (float)GridX);
	}
	bool Running = true;

	/// Values for rendering .bmps
	/// Z, Y, X are used for numbering the .bmp
	/// ex. Z = 1, Y = 2, X = 5 -> "render125.bmp"
	int countX = 0;
	int countY = 0;
	int countZ = 0;

	SDL_bool done = SDL_FALSE;
	while (!done) {

		/// Prevents the Window from freezing
		//	if event queue is not checked program get's marked as not responding
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = SDL_TRUE;
			}
		}

		/// used for Debugging purposes
		//forward = Vector3::normalize(CameraPosition - LookTarget);
		//right = Vector3::crossProduct(Vector3(0, 1, 0), forward);
		//up = Vector3::crossProduct(forward, right);

		CamRotateDegree += 2;
		Vector3 CameraDirection = Vector3((float)sin(CamRotateDegree*PI / 180), 0.0f, (float)cos(CamRotateDegree*PI / 180));
		CameraPosition = CameraHeight + (CameraDirection * CameraDistance);
		Matrix4x4 lookingMatrix = Matrix4x4::lookAt(CameraPosition, LookTarget);

		if (CAMERALIT)
			LightPos = CameraPosition;

		for (float y = 0; y < GridY; y++) {
			for (float x = 0; x < GridX; x++) {
				Direction = Vector3(LerpXValues[(int)x], LerpYValues[(int)y], -1.0f);

				/// Multiplying the World origin with the lookAt Matrix gives the Camera Position (from O) | Not really needed since based on origin returning CameraPos
				/// Multiplying it with the Direction Vector gives the Direction Vector P
				Vector3 P = lookingMatrix.multVecMatrix(Direction);
				//	Vector3 O = lookingMatrix.multVecMatrix(Vector3(0, 0, 0));
				Direction = P - CameraPosition;
				outputValue = MathOli::shootRay(CameraPosition, Direction, testMesh, tempHit);
				if (outputValue) {
					Vector3 tempV3 = Vector3::normalize(LightPos - LookTarget);
					float tempResult = Vector3::dotProduct(tempV3, tempHit.normal);
					if (tempResult > 0) {
						Grid[(int)x][(int)y] = tempResult;
					}
					else {
						Grid[(int)x][(int)y] = 0;
					}
					float v = Grid[(int)x][(int)y];

					SDL_Color PixelColor = SDL_Color();
					PixelColor.r = 255;
					PixelColor.g = 255;
					PixelColor.b = 255;
					PixelColor.a = 255;
					Uint32 colorToPaint;
					if (TEXTURED) {
						colorToPaint = Color::getpixel(Texture, (/*1 - */tempHit.text.x) * 512, (1 - tempHit.text.y) * 512);
						PixelColor = Color::UintToColour(colorToPaint);
					}
					PixelColor.b *= (float)(20 + 225 * (Grid[(int)x][(int)y])) / 255.0f;
					PixelColor.g *= (float)(20 + 225 * (Grid[(int)x][(int)y])) / 255.0f;
					PixelColor.r *= (float)(20 + 225 * (Grid[(int)x][(int)y])) / 255.0f;

					for (int i = 0; i < ySize; i++) {
						for (int j = 0; j < xSize; j++) {
							DrawPixel(W, x *xSize + i, y * ySize + j, Color::ColourToUint(PixelColor.r, PixelColor.g, PixelColor.b));
						}
					}
				}
				else {
					for (int i = 0; i < ySize; i++) {
						for (int j = 0; j < xSize; j++) {
							Grid[(int)x][(int)y] = 0;
							DrawPixel(W, x *xSize + i, y * ySize + j, 0x00000000);
						}
					}
				}
			}
		}

		/// Renders out .bmp files
		//SDL_LockSurface(SDL_GetWindowSurface(W));
		//std::stringstream ss;
		//char x = countX + '0';
		//char y = countY + '0';
		//char z = countZ + '0';
		//ss << "temp"<< z << y << x << ".bmp";
		//std::string name= ss.str();
		//char CharName[12];
		//strcpy_s(CharName, name.c_str());
		//std::cout << SDL_SaveBMP(SDL_GetWindowSurface(W), CharName);
		//SDL_UnlockSurface(SDL_GetWindowSurface(W));

		//countX++;
		//if (countX == 10) {
		//	countX = 0;
		//	countY++;
		//	if (countY == 10) {
		//		countY = 0;
		//		countZ++;
		//	}
		//}
		//if (countX == 0 && countY == 8 && countZ == 1)
		//	Running = false;
		SDL_UpdateWindowSurface(W);

	}
	SDL_DestroyWindow(W);

	SDL_Quit();

	return 0;
}

