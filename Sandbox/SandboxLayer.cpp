#include "SandboxLayer.h"


SandboxLayer::SandboxLayer(const std::string& name) : Layer(name) {};
SandboxLayer::~SandboxLayer() {};


void SandboxLayer::OnAdd()
{
	EnableDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_CameraSystem = new CameraSystem(1280, 720);

	m_CameraSystem->SetPerspectiveProjection();
	m_CameraSystem->SetCameraPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	m_ShaderSystem = new ShaderSystem();

	m_ShaderSystem->Add(ShaderType::VERTEX_SHADER, "Sandbox/assets/shaders/test.vert.glsl");
	m_ShaderSystem->Add(ShaderType::FRAGMENT_SHADER, "Sandbox/assets/shaders/test.frag.glsl");

	bool result = m_ShaderSystem->Compile();
	GL_ASSERT(result != false, "Shader compilation failed.");


	// 3D Cube
	// =======

	float cube_vertices[24 * 3] = {
		// positions(vec3)	       
		// --------------

		// back
		-0.5f, -0.5f, -0.5f,     // 0
		 0.5f, -0.5f, -0.5f,     // 1
		 0.5f,  0.5f, -0.5f,     // 2
		-0.5f,  0.5f, -0.5f,     // 3

		// front
		-0.5f, -0.5f,  0.5f,     // 4
		 0.5f, -0.5f,  0.5f,     // 5
		 0.5f,  0.5f,  0.5f,     // 6
		-0.5f,  0.5f,  0.5f,     // 7

		// left
		-0.5f,  0.5f,  0.5f,     // 8
		-0.5f,  0.5f, -0.5f,     // 9
		-0.5f, -0.5f, -0.5f,     // 10
		-0.5f, -0.5f,  0.5f,     // 11

		// right
		 0.5f,  0.5f,  0.5f,     // 12
		 0.5f,  0.5f, -0.5f,     // 13
		 0.5f, -0.5f, -0.5f,     // 14
		 0.5f, -0.5f,  0.5f,     // 15

		 // bottom
		-0.5f, -0.5f, -0.5f,     // 16
		 0.5f, -0.5f, -0.5f,     // 17
		 0.5f, -0.5f,  0.5f,     // 18
		-0.5f, -0.5f,  0.5f,     // 19

		// top
		-0.5f,  0.5f, -0.5f,     // 20 
		 0.5f,  0.5f, -0.5f,     // 21
		 0.5f,  0.5f,  0.5f,     // 22
		-0.5f,  0.5f,  0.5f,     // 23
	};

	unsigned int cube_indices[12 * 3] = {
		// back face
		0, 1, 2,
		2, 3, 0,
		// front face
		4, 5, 6,
		6, 7, 4,
		// left face
		8, 9, 10,
		10, 11, 8,
		// right face
		12, 13, 14,
		14, 15, 12,
		// bottom face
		16, 17, 18,
		18, 19, 16,
		// top face
		20, 21, 22,
		22, 23, 20
	};

	// Batch of cubes
	glm::mat4 modelMatrices[m_MatricesAmount];

	float offsetX = 4.0f;
	float offsetZ = 0.0f;

	for (unsigned int i = 0; i < m_MatricesAmount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(offsetX, 0.0f, offsetZ));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		offsetX -= 1.5f;

		if ((i + 1) % 5 == 0) {
			offsetX = 4.0f;
			offsetZ -= 1.5f;
		}

		modelMatrices[i] = model;
	}

	// Vertex Array Object
	glCreateVertexArrays(1, &m_CubeVA);
	glBindVertexArray(m_CubeVA);

	// Vertex Buffer
	glCreateBuffers(1, &m_CubeVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW); // Arguments - (type, size, data, usage)

	// Index Buffer
	glCreateBuffers(1, &m_CubeIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CubeIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// Instanced Array Buffer
	glCreateBuffers(1, &m_InstancedCubeVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_InstancedCubeVB);
	glBufferData(GL_ARRAY_BUFFER, m_MatricesAmount * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

	// Attribute Index
	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVB);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Arguments - (index, size, type, normalized, stride, offset)

	glBindBuffer(GL_ARRAY_BUFFER, m_InstancedCubeVB);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);
};


void SandboxLayer::OnRemove()
{
	glDeleteVertexArrays(1, &m_CubeVA);
	glDeleteBuffers(1, &m_CubeVB);
	glDeleteBuffers(1, &m_CubeIB);

	delete m_ShaderSystem;
	delete m_CameraSystem;
};


void SandboxLayer::OnUpdate(float deltaTime)
{
	m_CameraSystem->OnUpdate(deltaTime);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ShaderSystem->GetProgramID());

	int location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetCamera()->GetProjectionMatrix()));

	location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetViewMatrix()));

	location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)));

	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		// it's a bit too big for our scene, so scale it down

	//location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_Model");
	//glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));

	//glBindVertexArray(m_CubeVA);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

	// --------------------------------------------------------------

	// Batch Rendering
	glBindVertexArray(m_CubeVA);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, m_MatricesAmount);
};


void SandboxLayer::OnOverlayRender()
{
	ImGui::Begin("Testing Info");
	ImGui::Text("Application (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::Text("Frametime %.3f ms ", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();
};


void SandboxLayer::OnEvent(Event& event)
{
	m_CameraSystem->OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent& ev) { glViewport(0, 0, ev.GetWidth(), ev.GetHeight()); return true; });
};

