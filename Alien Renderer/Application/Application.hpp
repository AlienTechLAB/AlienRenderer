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
		Vertices[0].x = -0.5f; Vertices[0].y = 0.5f; Vertices[0].z = 0.0f;
		Vertices[1].x =  0.5f; Vertices[1].y = 0.5f; Vertices[1].z = 0.0f;
		Vertices[2].x =  0.5f; Vertices[2].y =-0.5f; Vertices[2].z = 0.0f;
		Vertices[3].x = -0.5f; Vertices[3].y =-0.5f; Vertices[3].z = 0.0f;

		Rednerer->SetTargetBuffer(Frame);
		Rednerer->SetVertices(Vertices, 4);
		Rednerer->SetDrawingMode(DrawingMode::LINE_LOOP);
		Rednerer->SetVertexShader(&Shader);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void MainLoopUpdate()
	{
		// Model transform is to convert from object space to world space.
		Matrix4x4 model;
		model.SetEulerRotation(Vector3(0, clock() * 0.001f, 0));
		Matrix4x4 model2;
		model2.SetTranslation(Vector3(0, 0, 5));
		model = model2 * model;

		Matrix4x4 view;
		view.SetIdentity();

		Matrix4x4 projection;
		projection.SetFrustumProjection(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 20.0f);

		Matrix4x4 mvp = projection * view * model;
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