#pragma once
#include <time.h>
#include "eVolution3D/GraphicsPipeline.hpp"
#include "eVolution3D/Vertex3.hpp"
#include "eVolution3D/Matrix4x4.hpp"
#include "eVolution3D/TargetBuffer.hpp"

using namespace eVolution3D;

class Application
{
	//---------------------------------------------------------------------------------------------------------

	private:
	TargetBuffer<Color32>* FrameBuffer = nullptr;
	GraphicsPipeline* Pipeline = nullptr;
	Vertex3*  Vertices = nullptr;
	VertexShaderMVP Shader;

	//---------------------------------------------------------------------------------------------------------

	public: void SetFrameBuffer(TargetBuffer<Color32>* frameBuffer)
	{
		FrameBuffer = frameBuffer;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Start()
	{
		Pipeline = new GraphicsPipeline();

		Vertices = new Vertex3[4];
		Vertices[0].x = -0.5f; Vertices[0].y = 0.5f; Vertices[0].z = 0.0f;
		Vertices[1].x =  0.5f; Vertices[1].y = 0.5f; Vertices[1].z = 0.0f;
		Vertices[2].x =  0.5f; Vertices[2].y =-0.5f; Vertices[2].z = 0.0f;
		Vertices[3].x = -0.5f; Vertices[3].y =-0.5f; Vertices[3].z = 0.0f;

		Pipeline->SetTargetBuffer(FrameBuffer);
		Pipeline->SetVertices(Vertices, 4);
		Pipeline->SetDrawingMode(DrawingMode::TRIANGLE_FAN);
		Pipeline->SetVertexShader(&Shader);
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
		Pipeline->SetMVPMatrix(mvp);
	}

	//---------------------------------------------------------------------------------------------------------

	public: void OnKeyPressed(char key)
	{
		std::cout << key << std::endl;
	}

	//---------------------------------------------------------------------------------------------------------

	public: void UpdateFrameBuffer()
	{
		FrameBuffer->Clear(Color32::Black);
		Pipeline->Render();
	}

	//---------------------------------------------------------------------------------------------------------

	public: ~Application()
	{
		Release();
	}

	//---------------------------------------------------------------------------------------------------------

	public: void Release()
	{
		if (Pipeline != nullptr)
		{
			delete Pipeline;
			Pipeline = nullptr;
		}

		if (Vertices != nullptr)
		{
			delete[] Vertices;
			Vertices = nullptr;
		}
	}

	//---------------------------------------------------------------------------------------------------------
};