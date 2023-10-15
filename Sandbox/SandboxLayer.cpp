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

	m_CubeShader = new ShaderSystem();
	m_CubeShader->Add(ShaderType::VERTEX_SHADER, "Sandbox/assets/shaders/instanced.vert.glsl");
	m_CubeShader->Add(ShaderType::FRAGMENT_SHADER, "Sandbox/assets/shaders/basic.frag.glsl");

	m_FloorShader = new ShaderSystem();
	m_FloorShader->Add(ShaderType::VERTEX_SHADER, "Sandbox/assets/shaders/basic.vert.glsl");
	m_FloorShader->Add(ShaderType::FRAGMENT_SHADER, "Sandbox/assets/shaders/basic.frag.glsl");

	bool result = m_CubeShader->Compile();
	GL_ASSERT(result != false, "Cube Shader compilation failed.");

	result = m_FloorShader->Compile();
	GL_ASSERT(result != false, "Floor Shader compilation failed.");

	// Floor
	// =====

	float planeVertices[] = {
		// positions          
		 5.0f, -0.5f,  5.0f,
		-5.0f, -0.5f,  5.0f,
		-5.0f, -0.5f, -5.0f,

		 5.0f, -0.5f,  5.0f,
		-5.0f, -0.5f, -5.0f,
		 5.0f, -0.5f, -5.0f,
	};

	// Vertex Array Object
	glCreateVertexArrays(1, &m_FloorVA);
	glBindVertexArray(m_FloorVA);

	// Vertex Buffer
	glCreateBuffers(1, &m_FloorVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_FloorVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW); // Arguments - (type, size, data, usage)

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Arguments - (index, size, type, normalized, stride, offset)

	glBindVertexArray(0);


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

	// Batches of cube
	glm::mat4 modelMatrices[m_MatricesAmount];

	float posXStart = 2.0f;
	float posX = posXStart,
	float posZ = 0.0f;
	float offset = 1.0f;

	for (unsigned int i = 0; i < m_MatricesAmount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(posX, 0.0f, posZ));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		posX -= offset;

		if ((i + 1) % 5 == 0) {
			posX = posXStart;
			posZ -= offset;
		}

		modelMatrices[i] = model;
	}


	// Vertex Array
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

};


void SandboxLayer::OnRemove()
{
	glDeleteVertexArrays(1, &m_FloorVA);
	glDeleteBuffers(1, &m_FloorVB);

	glDeleteVertexArrays(1, &m_CubeVA);
	glDeleteBuffers(1, &m_CubeVB);
	glDeleteBuffers(1, &m_CubeIB);
	glDeleteBuffers(1, &m_InstancedCubeVB);

	delete m_FloorShader;
	delete m_CubeShader;
	delete m_CameraSystem;
};


void SandboxLayer::OnUpdate(float deltaTime)
{
	m_CameraSystem->OnUpdate(deltaTime);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int program = 0;
	int location = -1;

	// Floor
	program = m_FloorShader->GetProgramID();
	glUseProgram(program);

	location = glGetUniformLocation(program, "u_Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetCamera()->GetProjectionMatrix()));

	location = glGetUniformLocation(program, "u_View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetViewMatrix()));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 0.55f, 1.0f));

	location = glGetUniformLocation(program, "u_Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));

	location = glGetUniformLocation(program, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)));

	glBindVertexArray(m_FloorVA);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// --------------------------------------------------------------

	// Cube (Batch Rendering)
	program = m_CubeShader->GetProgramID();
	glUseProgram(program);

	location = glGetUniformLocation(program, "u_Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetCamera()->GetProjectionMatrix()));

	location = glGetUniformLocation(program, "u_View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetViewMatrix()));

	location = glGetUniformLocation(program, "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)));

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

