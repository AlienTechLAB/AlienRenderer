#pragma once
#include <time.h>
#include "eVolution3D/eVoRenderer.hpp"
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/eVoMatrix4x4.hpp"

class Application
{
	//---------------------------------------------------------------------------------------------------------

	private:
	FrameBuffer<Color32>* Frame = NULL;
	eVoRenderer* Rednerer = NULL;
	Vertex3*  Vertices = NULL;
	eVoVertexShaderMVP VertexShader;

	//---------------------------------------------------------------------------------------------------------

	public: void SetFrameBuffer(FrameBuffer<Color32>* frameBuffer)
	{
		Frame = frameBuffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Start()
	{
		Rednerer = new eVoRenderer();

		Vertices = new Vertex3[4];
		Vertices[0].x =-100; Vertices[0].y = 100;
		Vertices[1].x = 100; Vertices[1].y = 100;
		Vertices[2].x = 100; Vertices[2].y =-100;
		Vertices[3].x =-100; Vertices[3].y =-100;

		Rednerer->SetTargetBuffer(Frame);
		Rednerer->SetVertices(Vertices, 4);
		Rednerer->SetDrawingMode(DrawingMode::LINE_LOOP);
		Rednerer->SetVertexShader(&VertexShader);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void MainLoopUpdate()
	{
		eVoMatrix4x4 rotation;
		float t = clock() * 0.001f;
		//rotation.SetYRotation(t1 * 0.001f);
		rotation.SetEulerRotation(Vector3(0, 0, t));
		
		eVoMatrix4x4 translation;
		translation.SetTranslation(Vector3(500, 500, 100));

		eVoMatrix4x4 scale;
		scale.SetScale(0.3f);

		eVoMatrix4x4 mvp = translation * rotation * scale;
		Rednerer->SetMVPMatrix(mvp);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void OnKeyPressed(char key)
	{
		std::cout << key << std::endl;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void UpdateFrameBuffer()
	{
		Rednerer->ClearBufferWithColor(Color32::Black);
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