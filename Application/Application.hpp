#pragma once
#include <time.h>
#include "eVolution3D/eVoRenderer.hpp"
#include "eVolution3D/eVoVector3.hpp"

class Application
{
	//---------------------------------------------------------------------------------------------------------

	private:
	eVoFrameBuffer<eVoColor32>* FrameBuffer = NULL;
	eVoRenderer* Rednerer = NULL;
	eVoVector3*  Vertices = NULL;

	//---------------------------------------------------------------------------------------------------------

	public: void SetFrameBuffer(eVoFrameBuffer<eVoColor32>* frameBuffer)
	{
		FrameBuffer = frameBuffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Start()
	{
		Rednerer = new eVoRenderer();

		Vertices = new eVoVector3[5];
		Vertices[0].x = 100; Vertices[0].y = 200;
		Vertices[1].x = 200; Vertices[1].y = 200;
		Vertices[2].x = 200; Vertices[2].y = 100;
		Vertices[3].x = 100; Vertices[3].y = 100;
		Vertices[4].x = 100; Vertices[4].y = 0;

		Rednerer->SetTargetBuffer(FrameBuffer);
		Rednerer->SetVertices(Vertices, 5);
		Rednerer->SetDrawingMode(eVoDrawingMode::WIRE_TRIANGLES);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void MainLoopUpdate()
	{
	}

	//---------------------------------------------------------------------------------------------------------

	public: void OnKeyPressed(char key)
	{
		std::cout << key << std::endl;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void UpdateFrameBuffer()
	{
		Rednerer->Render();
	}

	//---------------------------------------------------------------------------------------------------------

	public: ~Application()
	{
		Release();
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Release()
	{
		if (Rednerer != NULL)
		{
			delete Rednerer;
			Rednerer = NULL;
		}

		if (Vertices != NULL)
		{
			delete[] Vertices;
			Vertices = NULL;
		}
	}

	//---------------------------------------------------------------------------------------------------------
};