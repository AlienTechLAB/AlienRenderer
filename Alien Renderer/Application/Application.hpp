#pragma once
#include <time.h>
#include "eVolution3D/Renderer.hpp"
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/Matrix4x4.hpp"

using namespace eVolution3D;

class Application
{
	//---------------------------------------------------------------------------------------------------------

	private:
	FrameBuffer<Color32>* Frame = NULL;
	Renderer* Rednerer = NULL;
	Vertex3*  Vertices = NULL;
	VertexShaderMVP Shader;

	//---------------------------------------------------------------------------------------------------------

	public: void SetFrameBuffer(FrameBuffer<Color32>* frameBuffer)
	{
		Frame = frameBuffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Start()
	{
		Rednerer = new Renderer();

		Vertices = new Vertex3[4];
		Vertices[0].x =-0.5f; Vertices[0].y = 0.5f;
		Vertices[1].x = 0.5f; Vertices[1].y = 0.5f;
		Vertices[2].x = 0.5f; Vertices[2].y =-0.5f;
		Vertices[3].x =-0.5f; Vertices[3].y =-0.5f;

		Rednerer->SetTargetBuffer(Frame);
		Rednerer->SetVertices(Vertices, 4);
		Rednerer->SetDrawingMode(DrawingMode::LINE_LOOP);
		Rednerer->SetVertexShader(&Shader);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void MainLoopUpdate()
	{
		Matrix4x4 rotation;
		float t = clock() * 0.001f;
		rotation.SetEulerRotation(Vector3(0, t, 0));
		
		Matrix4x4 translation;
		translation.SetTranslation(Vector3::Zero());

		Matrix4x4 scale;
		scale.SetScale(1.0f);

		Matrix4x4 mvp = translation * rotation * scale;
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