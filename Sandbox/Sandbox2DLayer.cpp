#include "Sandbox2DLayer.h"


Sandbox2DLayer::Sandbox2DLayer(const std::string& name) : Layer(name) {};
Sandbox2DLayer::~Sandbox2DLayer() {};


void Sandbox2DLayer::OnAdd()
{
	EnableDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_CameraSystem = new CameraSystem(1280, 720);

	m_ShaderSystem = new ShaderSystem();

	m_ShaderSystem->Add(ShaderType::VERTEX_SHADER, "Sandbox/assets/shaders/test.vert.glsl");
	m_ShaderSystem->Add(ShaderType::FRAGMENT_SHADER, "Sandbox/assets/shaders/test.frag.glsl");

	m_ShaderSystem->Compile();

	// 2D Triangle
	// ==========

	glCreateVertexArrays(1, &m_TrigVA);
	glBindVertexArray(m_TrigVA);

	float vertices[] = {
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  // top 
	};

	uint32_t indices[] = {
		0, 1, 2,
	};

	glCreateBuffers(1, &m_TrigVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_TrigVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glCreateBuffers(1, &m_TrigIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TrigIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

};


void Sandbox2DLayer::OnRemove()
{
	glDeleteVertexArrays(1, &m_TrigVA);
	glDeleteBuffers(1, &m_TrigVB);
	glDeleteBuffers(1, &m_TrigIB);

	delete m_ShaderSystem;
	delete m_CameraSystem;
};


void Sandbox2DLayer::OnUpdate(float deltaTime)
{
	m_CameraSystem->OnUpdate(deltaTime);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ShaderSystem->GetProgramID());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down

	int location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));

	location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetViewMatrix()));

	location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_CameraSystem->GetCamera()->GetProjectionMatrix()));

	location = glGetUniformLocation(m_ShaderSystem->GetProgramID(), "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)));

	glBindVertexArray(m_TrigVA);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
};


void Sandbox2DLayer::OnEvent(Event& event)
{
	m_CameraSystem->OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent& ev) { glViewport(0, 0, ev.GetWidth(), ev.GetHeight()); return true; });
};
