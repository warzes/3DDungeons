#pragma once

#define BUFFER_SIZE_INCREMENT 1048576

class MemoryBuffer
{
public:
	MemoryBuffer() { setDefaults(); }
	~MemoryBuffer() { Empty(); }

	void AddData(void* Data, int DataSize);
	void Empty();
	void FreeUnusedMemory();
	uint8_t* GetData() { return m_buffer; }
	int GetDataSize() { return m_position; }

private:
	void setDefaults();

	uint8_t* m_buffer;
	int m_bufferSize;
	int m_position;
};

void MemoryBuffer::AddData(void* Data, int DataSize)
{
	int Remaining = m_bufferSize - m_position;

	if (DataSize > Remaining)
	{
		uint8_t* OldBuffer = m_buffer;
		int OldBufferSize = m_bufferSize;

		int Needed = DataSize - Remaining;

		m_bufferSize += Needed > BUFFER_SIZE_INCREMENT ? Needed : BUFFER_SIZE_INCREMENT;

		m_buffer = new uint8_t[m_bufferSize];

		memcpy(m_buffer, OldBuffer, OldBufferSize);

		delete[] OldBuffer;
	}

	memcpy(m_buffer + m_position, Data, DataSize);

	m_position += DataSize;
}

void MemoryBuffer::Empty()
{
	delete[] m_buffer;
	setDefaults();
}

void MemoryBuffer::FreeUnusedMemory()
{
	uint8_t* OldBuffer = m_buffer;

	m_bufferSize = m_position;

	m_buffer = new uint8_t[m_bufferSize];

	memcpy(m_buffer, OldBuffer, m_bufferSize);

	delete[] OldBuffer;
}

void MemoryBuffer::setDefaults()
{
	m_buffer = nullptr;
	m_bufferSize = 0;
	m_position = 0;
}

#define RADIUS 0.15625f

class CTriangle
{
public:
	Vector3Old Vertex[3], Edge[3], Normal[4];
	float EdgeLength[3], D[4];
	bool Close;
};

class CObject
{
public:
	Matrix3Old NormalMatrix;
	Matrix4Old ModelMatrix;
	Texture2D Texture;
	MemoryBuffer Buffer;
	GLuint VBO;
	int VertexOffset, Stride, VerticesCount, TrianglesCount;
	CTriangle* Triangles;
	Vector3Old Min, Max;
	bool Close;

public:
	CObject();

	void AddData(void* Data, int DataSize);
	void Destroy();
	void InitVBO(int VertexOffset, int Stride);
	void PrepareTriangles();
	void SetModelMatrix(const Matrix4Old& ModelMatrix);

private:
	void SetDefaults();
};

CObject::CObject()
{
	SetDefaults();
}


void CObject::AddData(void* Data, int DataSize)
{
	Buffer.AddData(Data, DataSize);
}

void CObject::Destroy()
{
	Texture.Destroy();

	Buffer.Empty();

	glDeleteBuffers(1, &VBO);

	delete[] Triangles;

	SetDefaults();
}

void CObject::InitVBO(int VertexOffset, int Stride)
{
	this->VertexOffset = VertexOffset;
	this->Stride = Stride;

	Buffer.FreeUnusedMemory();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Buffer.GetDataSize(), Buffer.GetData(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	VerticesCount = Buffer.GetDataSize() / Stride;
	TrianglesCount = VerticesCount / 3;

	Triangles = new CTriangle[TrianglesCount];

	PrepareTriangles();
}

void CObject::PrepareTriangles()
{
	uint8_t* Vertices = Buffer.GetData() + VertexOffset;

	CTriangle* Triangle = Triangles;

	for (int t = 0; t < TrianglesCount; t++)
	{
		for (int v = 0; v < 3; v++)
		{
			Vector3Old* Vertex = (Vector3Old*)(Vertices + Stride * v);

			Triangle->Vertex[v].x = ModelMatrix[0] * Vertex->x + ModelMatrix[4] * Vertex->y + ModelMatrix[8] * Vertex->z + ModelMatrix[12];
			Triangle->Vertex[v].y = ModelMatrix[1] * Vertex->x + ModelMatrix[5] * Vertex->y + ModelMatrix[9] * Vertex->z + ModelMatrix[13];
			Triangle->Vertex[v].z = ModelMatrix[2] * Vertex->x + ModelMatrix[6] * Vertex->y + ModelMatrix[10] * Vertex->z + ModelMatrix[14];
		}

		Triangle->Normal[0] = CrossProduct(Triangle->Vertex[1] - Triangle->Vertex[0], Triangle->Vertex[2] - Triangle->Vertex[0]).GetNormalize();
		Triangle->D[0] = -DotProduct(Triangle->Normal[0], Triangle->Vertex[0]);

		for (int e = 0; e < 3; e++)
		{
			Triangle->Edge[e] = Triangle->Vertex[(e + 1) % 3] - Triangle->Vertex[e];
			Triangle->EdgeLength[e] = Triangle->Edge[e].GetLength();
			Triangle->Edge[e] /= Triangle->EdgeLength[e];
			Triangle->Normal[1 + e] = CrossProduct(Triangle->Edge[e], Triangle->Normal[0]);
			Triangle->D[1 + e] = -DotProduct(Triangle->Normal[1 + e], Triangle->Vertex[e]);
		}

		Vertices += Stride * 3;

		Triangle++;
	}

	Min = Max = Triangles[0].Vertex[0];

	Triangle = Triangles;

	for (int t = 0; t < TrianglesCount; t++)
	{
		for (int v = 0; v < 3; v++)
		{
			if (Triangle->Vertex[v].x < Min.x) Min.x = Triangle->Vertex[v].x;
			if (Triangle->Vertex[v].x > Max.x) Max.x = Triangle->Vertex[v].x;
			if (Triangle->Vertex[v].y < Min.y) Min.y = Triangle->Vertex[v].y;
			if (Triangle->Vertex[v].y > Max.y) Max.y = Triangle->Vertex[v].y;
			if (Triangle->Vertex[v].z < Min.z) Min.z = Triangle->Vertex[v].z;
			if (Triangle->Vertex[v].z > Max.z) Max.z = Triangle->Vertex[v].z;
		}

		Triangle++;
	}

	Min -= RADIUS;
	Max += RADIUS;
}

void CObject::SetModelMatrix(const Matrix4Old& ModelMatrix)
{
	this->ModelMatrix = ModelMatrix;

	NormalMatrix = Matrix3Old(ModelMatrix).Inverse().Transpose();

	PrepareTriangles();
}

void CObject::SetDefaults()
{
	NormalMatrix = Matrix3Old();
	ModelMatrix = Matrix4Old();

	VBO = 0;

	VertexOffset = 0;
	Stride = 0;
	VerticesCount = 0;
	TrianglesCount = 0;

	Triangles = NULL;
}

Matrix3Old NormalMatrix;
Matrix4Old ModelMatrix, ProjectionMatrix, ModelViewProjectionMatrix;
Matrix4Old ViewProjectionMatrix;

ICamera* cam = new FlyingCamera;
int LastX = 0;
int LastY = 0;

CObject* Objects = nullptr;
int ObjectsCount = 0;

ShaderProgram Lighting;
int uniformNormalMatrix;
int uniformModelViewProjectionMatrixx;
GLuint VAO;

// SkyBox
ShaderProgram SkyBox;
int uniformCameraPosition;
int uniformViewProjectionMatrix;
TextureCube SkyBoxTexture;
VertexBuffer skyBoxVBO;
VertexArrayBuffer skyBoxVAO;

constexpr Vector3Old SkyBoxVertices[] =
{
	{1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f,  1.0f},
	{1.0f,  1.0f,  1.0f},
	{1.0f,  1.0f,  1.0f},
	{1.0f,  1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f},
	{-1.0f, -1.0f, -1.0f},
	{-1.0f,  1.0f, -1.0f},
	{-1.0f,  1.0f, -1.0f},
	{-1.0f,  1.0f,  1.0f},
	{-1.0f, -1.0f,  1.0f},
	{-1.0f,  1.0f, -1.0f},
	{1.0f,  1.0f, -1.0f},
	{1.0f,  1.0f,  1.0f},
	{1.0f,  1.0f,  1.0f},
	{-1.0f,  1.0f,  1.0f},
	{-1.0f,  1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f},
	{1.0f, -1.0f,  1.0f},
	{1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f},
	{-1.0f, -1.0f,  1.0f},
	{1.0f, -1.0f,  1.0f},
	{-1.0f, -1.0f,  1.0f},
	{-1.0f,  1.0f,  1.0f},
	{-1.0f,  1.0f,  1.0f},
	{1.0f,  1.0f,  1.0f},
	{1.0f, -1.0f,  1.0f},
	{-1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{1.0f,  1.0f, -1.0f},
	{1.0f,  1.0f, -1.0f},
	{-1.0f,  1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f}
};

std::string skyboxVertexShaderText = R"(
#version 330 core

layout(location = 0) in vec3 vert_Position;

uniform vec3 CameraPosition;
uniform mat4x4 ViewProjectionMatrix;

out vec3 TexCoord;

void main()
{
	TexCoord = vec3(vert_Position.x, -vert_Position.yz);
	gl_Position = ViewProjectionMatrix * vec4(vert_Position + CameraPosition, 1.0);
}
)";

std::string skyboxFragmentShaderText = R"(
#version 330

in vec3 TexCoord;

uniform samplerCube CubeMap;

out vec4 frag_Color;

void main()
{
	frag_Color = texture(CubeMap, TexCoord);
}
)";

extern Vector2 GrassTexCoords[6];
extern Vector3Old GrassVertices[6];
extern Vector2 CubeTexCoords[6];
extern Vector3Old CubeNormals[6];
extern Vector3Old CubeVertices[36];

int GenerateTorus(MemoryBuffer& Buffer, float Radius, float TubeRadius, int SubDivAround, int SubDivTube, const Matrix4Old& ModelMatrix);

std::string lightingVertexShaderText = R"(
#version 330

layout(location = 0) in vec2 vert_TexCoord;
layout(location = 1) in vec3 vert_Normal;
layout(location = 2) in vec3 vert_Position;

uniform mat3x3 NormalMatrix;
uniform mat4x4 ModelViewProjectionMatrix;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
	TexCoord = vert_TexCoord;
	Normal = NormalMatrix * vert_Normal;
	gl_Position = ModelViewProjectionMatrix * vec4(vert_Position, 1.0);
}
)";

std::string lightingFragmentShaderText = R"(
#version 330

in vec2 TexCoord;
in vec3 Normal;

struct DirectionalLight
{
	float Ambient, Diffuse;
	vec3 Direction;
};

uniform sampler2D Texture;
uniform DirectionalLight Light;

out vec4 frag_Color;

void main()
{
	frag_Color = texture(Texture, TexCoord);
	float NdotLD = max(dot(Light.Direction, normalize(Normal)), 0.0); // ламберт
	frag_Color.rgb *= Light.Ambient + Light.Diffuse * NdotLD;
	//float attenuation = saturate(1.0 - DistanceToLight / LightRadius);
	//frag_Color.rgb *= Light.Ambient + Light.Diffuse * NdotLD * attenuation;
}
)";

bool CheckCollisions(ICamera* cam, CObject* Objects, int ObjectsCount, Vector3Old& Movement, int Depth)
{
	if (Depth < 4)
	{
		Vector3Old Position = cam->position + Movement;

		// check the distance of the camera position from each triangle plane

		for (int o = 0; o < ObjectsCount; o++)
		{
			Objects[o].Close = false;

			if (Position.x < Objects[o].Min.x) continue;
			if (Position.x > Objects[o].Max.x) continue;
			if (Position.y < Objects[o].Min.y) continue;
			if (Position.y > Objects[o].Max.y) continue;
			if (Position.z < Objects[o].Min.z) continue;
			if (Position.z > Objects[o].Max.z) continue;

			Objects[o].Close = true;

			CTriangle* Triangle = Objects[o].Triangles;

			for (int t = 0; t < Objects[o].TrianglesCount; t++)
			{
				float Distance = DotProduct(Triangle->Normal[0], Position) + Triangle->D[0];

				Triangle->Close = Distance > 0.0f && Distance < RADIUS;

				if (Triangle->Close)
				{
					if (DotProduct(Triangle->Normal[1], Position) + Triangle->D[1] < 0.0f)
					{
						if (DotProduct(Triangle->Normal[2], Position) + Triangle->D[2] < 0.0f)
						{
							if (DotProduct(Triangle->Normal[3], Position) + Triangle->D[3] < 0.0f)
							{
								Movement += Triangle->Normal[0] * (RADIUS - Distance);

								CheckCollisions(cam, Objects, ObjectsCount, Movement, Depth + 1);

								return true;
							}
						}
					}
				}

				Triangle++;
			}
		}

		// check the distance of the camera position from each edge

		for (int o = 0; o < ObjectsCount; o++)
		{
			if (!Objects[o].Close) continue;

			CTriangle* Triangle = Objects[o].Triangles;

			for (int t = 0; t < Objects[o].TrianglesCount; t++)
			{
				if (Triangle->Close)
				{
					for (int e = 0; e < 3; e++)
					{
						Vector3Old VCP = Position - Triangle->Vertex[e];

						float EdotVCP = DotProduct(Triangle->Edge[e], VCP);

						if (EdotVCP > 0.0f && EdotVCP < Triangle->EdgeLength[e])
						{
							Vector3Old Normal = VCP - Triangle->Edge[e] * EdotVCP;

							float Distance = Normal.GetLength();

							if (Distance > 0.0f && Distance < RADIUS)
							{
								Movement += Normal * (RADIUS / Distance - 1.0f);

								CheckCollisions(cam, Objects, ObjectsCount, Movement, Depth + 1);

								return true;
							}
						}
					}
				}

				Triangle++;
			}
		}

		// check the distance of the camera position from each vertex

		for (int o = 0; o < ObjectsCount; o++)
		{
			if (!Objects[o].Close) continue;

			CTriangle* Triangle = Objects[o].Triangles;

			for (int t = 0; t < Objects[o].TrianglesCount; t++)
			{
				if (Triangle->Close)
				{
					for (int v = 0; v < 3; v++)
					{
						Vector3Old Normal = Position - Triangle->Vertex[v];

						float Distance = Normal.GetLength();

						if (Distance > 0.0f && Distance < RADIUS)
						{
							Movement += Normal * (RADIUS / Distance - 1.0f);

							CheckCollisions(cam, Objects, ObjectsCount, Movement, Depth + 1);

							return true;
						}
					}
				}

				Triangle++;
			}
		}
	}

	return false;
}


void ExampleInit()
{
	// Sky box init

	SkyBox.CreateFromMemories(skyboxVertexShaderText, skyboxFragmentShaderText);
	uniformCameraPosition = SkyBox.GetUniformLocation("CameraPosition");
	uniformViewProjectionMatrix = SkyBox.GetUniformLocation("ViewProjectionMatrix");

	SkyBoxTexture.Create(
		"../data/textures/sky/emerald_ft.jpg",
		"../data/textures/sky/emerald_bk.jpg",
		"../data/textures/sky/emerald_dn.jpg",
		"../data/textures/sky/emerald_up.jpg",
		"../data/textures/sky/emerald_rt.jpg",
		"../data/textures/sky/emerald_lf.jpg"
		);

	skyBoxVBO.Create(RenderResourceUsage::Static, 36, sizeof(Vector3Old), SkyBoxVertices);
	skyBoxVAO.Create(&skyBoxVBO, nullptr, &SkyBox);


	LastX = GetCursorPosition().x;
	LastY = GetCursorPosition().y;

	ObjectsCount = 3;
	Objects = new CObject[ObjectsCount];

	Objects[0].Texture.Create("../data/textures/cretewall03.jpg");
	Objects[1].Texture.Create("../data/textures/stone03b.jpg");
	Objects[2].Texture.Create("../data/textures/metalplate.jpg");

	Lighting.CreateFromMemories(lightingVertexShaderText, lightingFragmentShaderText);
	uniformNormalMatrix = Lighting.GetUniformLocation("NormalMatrix");
	uniformModelViewProjectionMatrixx = Lighting.GetUniformLocation("ModelViewProjectionMatrix");

	// set constant uniforms
	Lighting.Bind();
	Lighting.SetUniform(Lighting.GetUniformLocation("Light.Ambient"), 0.333333f);
	Lighting.SetUniform(Lighting.GetUniformLocation("Light.Diffuse"), 0.666666f);
	Vector3Old LightDirection = Vector3Old(0.467757f, 0.424200f, -0.775409f);
	Lighting.SetUniform(Lighting.GetUniformLocation("Light.Direction"), LightDirection);

	// init terrain 
	Vector3Old GrassNormal = Vector3Old(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < 6; i++)
	{
		Objects[0].Buffer.AddData(&GrassTexCoords[i], 8);
		Objects[0].Buffer.AddData(&GrassNormal, 12);
		Objects[0].Buffer.AddData(&GrassVertices[i], 12);
	}

	Objects[0].InitVBO(20, 32);

	// init cubes -------------------------------------------------------------------------------------------------------------

	for (int y = 0; y < 1; y++)
	{
		for (int x = 0; x < 8 - y; x++)
		{
			Vector3Old offset = Vector3Old(-5.5f + x * 1.5f + 0.75f * y, 0.5f + y, -5.0f);

			for (int i = 0; i < 36; i++)
			{
				Objects[1].Buffer.AddData(&CubeTexCoords[i % 6], 8);
				Objects[1].Buffer.AddData(&CubeNormals[i / 6], 12);
				Vector3Old Vertex = CubeVertices[i] + offset;
				Objects[1].Buffer.AddData(&Vertex, 12);
			}
		}
	}

	Objects[1].InitVBO(20, 32);

	// init tori --------------------------------------------------------------------------------------------------------------

	GenerateTorus(Objects[2].Buffer, 1.0f, 0.25f, 32, 16, Matrix4Old());
	GenerateTorus(Objects[2].Buffer, 1.0f, 0.25f, 32, 16, Matrix4Old::Rotate(Vector3Old(0.0f, 1.0f, 0.0f), 90.0f));
	GenerateTorus(Objects[2].Buffer, 1.0f, 0.25f, 32, 16, Matrix4Old::Rotate(Vector3Old(1.0f, 0.0f, 0.0f), 90.0f));

	Objects[2].InitVBO(20, 32);

	// generate VAO 
	glGenVertexArrays(1, &VAO);

	cam->Look(Vector3Old(0.0f, 1.75f, 0.0f), Vector3Old(0.0f, 1.75f, -1.0f));
}

void ExampleClose()
{

}

void ExampleFrame()
{
	auto npos = GetCursorPosition();
	if (npos.x != LastX || npos.y != LastY)
	{
		if (IsMouseButtonDown(1))
		{
			cam->OnMouseMove(LastX - npos.x, LastY - npos.y);
		}
	}
	LastX = npos.x;
	LastY = npos.y;

	short Keys = 0x0000;
	if (IsKeyDown('W')) Keys |= CAMERA_KEY_W;
	if (IsKeyDown('S')) Keys |= CAMERA_KEY_S;
	if (IsKeyDown('A')) Keys |= CAMERA_KEY_A;
	if (IsKeyDown('D')) Keys |= CAMERA_KEY_D;
	if (IsKeyDown('R')) Keys |= CAMERA_KEY_R;
	if (IsKeyDown('F')) Keys |= CAMERA_KEY_F;
	if (IsKeyDown('Q')) Keys |= CAMERA_KEY_Q;
	if (IsKeyDown('E')) Keys |= CAMERA_KEY_E;
	if (IsKeyDown('C')) Keys |= CAMERA_KEY_E;

	if (IsKeyDown(0x20/*VK_SPACE*/)) Keys |= CAMERA_KEY_SPACE;
	if (IsKeyDown(0x10/*VK_SHIFT*/)) Keys |= CAMERA_KEY_SHIFT;
	if (IsKeyDown(0x11/*VK_CONTROL*/)) Keys |= CAMERA_KEY_CONTROL;

	Vector3Old Movement;
	bool MoveCamera = cam->OnKeys(Keys, 0.01f, Movement);
	//Movement.y -= 0.01f;
	if (MoveCamera)
	{
		bool CollisionsDetected = CheckCollisions(cam, Objects, ObjectsCount, Movement, 0);
		if (MoveCamera || CollisionsDetected)
		{
			cam->Move(Movement);
		}
	}

	ProjectionMatrix = Matrix4Old::Perspective(45.0f * DEG2RAD, GetWindowAspectRatio(), 0.01f, 1000.f);
	ViewProjectionMatrix = ProjectionMatrix * cam->GetViewMatrix();

	// render skybox - depth test must be disabled 
	SkyBox.Bind();
	SkyBox.SetUniform(uniformCameraPosition, cam->position);
	SkyBox.SetUniform(uniformViewProjectionMatrix, ViewProjectionMatrix);
	SkyBoxTexture.Bind();

	skyBoxVAO.Draw();
	VertexArrayBuffer::UnBind();
	TextureCube::UnBind();

	// render scene 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Lighting.Bind();
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	static float Angle = 0.0f;
	Angle += 22.5f * 0.001f;
	Objects[2].SetModelMatrix(Matrix4Old::Translate({ 0.0f, 1.75f, 5.0f }) * Matrix4Old::Rotate(Vector3Old(0.0f, 1.0f, 0.0f), Angle * DEG2RAD));


	Objects[0].SetModelMatrix(Matrix4Old::Rotate(Vector3Old(0.0f, 1.0f, 0.1f), 47*DEG2RAD));

	for (int i = 0; i < ObjectsCount; i++)
	{
		ModelViewProjectionMatrix = ViewProjectionMatrix * Objects[i].ModelMatrix;

		Lighting.SetUniform(uniformNormalMatrix, Objects[i].NormalMatrix);
		Lighting.SetUniform(uniformModelViewProjectionMatrixx, ModelViewProjectionMatrix);

		Objects[i].Texture.Bind(0);

		glBindBuffer(GL_ARRAY_BUFFER, Objects[i].VBO);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 32, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*)8);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)20);

		glDrawArrays(GL_TRIANGLES, 0, Objects[i].VerticesCount);
	}

	// unbind and disable what was bound and enabled
	Texture2D::UnBind();
	VertexArrayBuffer::UnBind();
	ShaderProgram::UnBind();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

Vector2 GrassTexCoords[6] =
{
	Vector2(0.0f,  0.0f),
	Vector2(10.0f,  0.0f),
	Vector2(10.0f, 10.0f),
	Vector2(10.0f, 10.0f),
	Vector2(0.0f, 10.0f),
	Vector2(0.0f,  0.0f)
};

Vector3Old GrassVertices[6] =
{
	Vector3Old(-10.0f, 0.0f,  10.0f),
	Vector3Old(10.0f, 0.0f,  10.0f),
	Vector3Old(10.0f, 0.0f, -10.0f),
	Vector3Old(10.0f, 0.0f, -10.0f),
	Vector3Old(-10.0f, 0.0f, -10.0f),
	Vector3Old(-10.0f, 0.0f,  10.0f)
};

Vector2 CubeTexCoords[6] =
{
	Vector2(0.0f, 0.0f),
	Vector2(1.0f, 0.0f),
	Vector2(1.0f, 1.0f),
	Vector2(1.0f, 1.0f),
	Vector2(0.0f, 1.0f),
	Vector2(0.0f, 0.0f)
};

Vector3Old CubeNormals[6] =
{
	Vector3Old(1.0f,  0.0f,  0.0f),
	Vector3Old(-1.0f,  0.0f,  0.0f),
	Vector3Old(0.0f,  1.0f,  0.0f),
	Vector3Old(0.0f, -1.0f,  0.0f),
	Vector3Old(0.0f,  0.0f,  1.0f),
	Vector3Old(0.0f,  0.0f, -1.0f)
};

Vector3Old CubeVertices[36] =
{
	Vector3Old(0.5f, -0.5f,  0.5f), Vector3Old(0.5f, -0.5f, -0.5f), Vector3Old(0.5f,  0.5f, -0.5f), Vector3Old(0.5f,  0.5f, -0.5f), Vector3Old(0.5f,  0.5f,  0.5f), Vector3Old(0.5f, -0.5f,  0.5f),
	Vector3Old(-0.5f, -0.5f, -0.5f), Vector3Old(-0.5f, -0.5f,  0.5f), Vector3Old(-0.5f,  0.5f,  0.5f), Vector3Old(-0.5f,  0.5f,  0.5f), Vector3Old(-0.5f,  0.5f, -0.5f), Vector3Old(-0.5f, -0.5f, -0.5f),
	Vector3Old(-0.5f,  0.5f,  0.5f), Vector3Old(0.5f,  0.5f,  0.5f), Vector3Old(0.5f,  0.5f, -0.5f), Vector3Old(0.5f,  0.5f, -0.5f), Vector3Old(-0.5f,  0.5f, -0.5f), Vector3Old(-0.5f,  0.5f,  0.5f),
	Vector3Old(-0.5f, -0.5f, -0.5f), Vector3Old(0.5f, -0.5f, -0.5f), Vector3Old(0.5f, -0.5f,  0.5f), Vector3Old(0.5f, -0.5f,  0.5f), Vector3Old(-0.5f, -0.5f,  0.5f), Vector3Old(-0.5f, -0.5f, -0.5f),
	Vector3Old(-0.5f, -0.5f,  0.5f), Vector3Old(0.5f, -0.5f,  0.5f), Vector3Old(0.5f,  0.5f,  0.5f), Vector3Old(0.5f,  0.5f,  0.5f), Vector3Old(-0.5f,  0.5f,  0.5f), Vector3Old(-0.5f, -0.5f,  0.5f),
	Vector3Old(0.5f, -0.5f, -0.5f), Vector3Old(-0.5f, -0.5f, -0.5f), Vector3Old(-0.5f,  0.5f, -0.5f), Vector3Old(-0.5f,  0.5f, -0.5f), Vector3Old(0.5f,  0.5f, -0.5f), Vector3Old(0.5f, -0.5f, -0.5f)
};

// ----------------------------------------------------------------------------------------------------------------------------

int GenerateTorus(MemoryBuffer& Buffer, float Radius, float TubeRadius, int SubDivAround, int SubDivTube, const Matrix4Old& ModelMatrix)
{
	Matrix3Old NormalMatrix = Matrix3Old(ModelMatrix).Inverse().Transpose();

	int VerticesCount = 0;

	Vector2* TexCoords = new Vector2[4];
	Vector3Old* Normals = new Vector3Old[4];
	Vector3Old* Vertices = new Vector3Old[4];

	int Indices[] = { 0, 1, 2, 2, 3, 0 };

	float pim2 = 6.283185f;

	float AddAngleAround = pim2 / (float)SubDivAround;
	float AddAngleTube = pim2 / (float)SubDivTube;

	float CurAngleAround = 0.0f;
	int StepsAround = 0;

	while (StepsAround < SubDivAround)
	{
		float NextAngleAround = CurAngleAround + AddAngleAround;

		Vector3Old Dir1(sin(CurAngleAround), cos(CurAngleAround), 0.0f);
		Vector3Old Dir2(sin(NextAngleAround), cos(NextAngleAround), 0.0f);

		float CurAngleTube = 0.0f;
		int StepsTube = 0;

		while (StepsTube < SubDivTube)
		{
			float SineTube = sin(CurAngleTube);
			float CosineTube = cos(CurAngleTube);

			float NextAngleTube = CurAngleTube + AddAngleTube;

			float NextSineTube = sin(NextAngleTube);
			float NextCosineTube = cos(NextAngleTube);

			Vector3Old Mid1 = Dir1 * (Radius - TubeRadius / 2.0f), Mid2 = Dir2 * (Radius - TubeRadius / 2.0f);

			TexCoords[0] = Vector2(CurAngleAround / pim2, NextAngleTube / pim2);
			TexCoords[1] = Vector2(CurAngleAround / pim2, CurAngleTube / pim2);
			TexCoords[2] = Vector2(NextAngleAround / pim2, CurAngleTube / pim2);
			TexCoords[3] = Vector2(NextAngleAround / pim2, NextAngleTube / pim2);

			Normals[0] = Vector3Old(0.0f, 0.0f, -NextSineTube) + Dir1 * NextCosineTube;
			Normals[1] = Vector3Old(0.0f, 0.0f, -SineTube) + Dir1 * CosineTube;
			Normals[2] = Vector3Old(0.0f, 0.0f, -SineTube) + Dir2 * CosineTube;
			Normals[3] = Vector3Old(0.0f, 0.0f, -NextSineTube) + Dir2 * NextCosineTube;

			Vertices[0] = Mid1 + Vector3Old(0.0f, 0.0f, -NextSineTube * TubeRadius) + Dir1 * NextCosineTube * TubeRadius;
			Vertices[1] = Mid1 + Vector3Old(0.0f, 0.0f, -SineTube * TubeRadius) + Dir1 * CosineTube * TubeRadius;
			Vertices[2] = Mid2 + Vector3Old(0.0f, 0.0f, -SineTube * TubeRadius) + Dir2 * CosineTube * TubeRadius;
			Vertices[3] = Mid2 + Vector3Old(0.0f, 0.0f, -NextSineTube * TubeRadius) + Dir2 * NextCosineTube * TubeRadius;

			for (int i = 0; i < 6; i++)
			{
				int Index = Indices[i];

				Buffer.AddData(&TexCoords[Index], 8);
				Vector3Old Normal = (NormalMatrix * Normals[Index]).GetNormalize();
				Buffer.AddData(&Normal, 12);
				Vector4Old Vertex = ModelMatrix * Vector4Old(Vertices[Index].x, Vertices[Index].y, Vertices[Index].z, 1.0f);
				Buffer.AddData(&Vertex, 12);

				VerticesCount++;
			}

			CurAngleTube += AddAngleTube;
			StepsTube++;
		}

		CurAngleAround += AddAngleAround;
		StepsAround++;
	}

	delete[] TexCoords;
	delete[] Normals;
	delete[] Vertices;

	return VerticesCount;
}
