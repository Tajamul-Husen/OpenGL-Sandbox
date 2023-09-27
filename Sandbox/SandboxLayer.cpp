#include "SandboxLayer.h"


SandboxLayer::SandboxLayer(const std::string& name) : Layer(name) {};
SandboxLayer::~SandboxLayer() {};


void SandboxLayer::OnAdd()
{
	EnableDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = new Shader();

	m_Shader->Add(ShaderType::VERTEX_SHADER, "Sandbox/assets/shaders/test.vert.glsl");
	m_Shader->Add(ShaderType::FRAGMENT_SHADER, "Sandbox/assets/shaders/test.frag.glsl");

	m_Shader->Compile();

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


void SandboxLayer::OnRemove()
{
	glDeleteVertexArrays(1, &m_TrigVA);
	glDeleteBuffers(1, &m_TrigVB);
	glDeleteBuffers(1, &m_TrigIB);
	delete m_Shader;
};


void SandboxLayer::OnUpdate(float deltaTime)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_Shader->GetProgramID());

	int location = glGetUniformLocation(m_Shader->GetProgramID(), "u_Color");
	glUniform4fv(location, 1, glm::value_ptr(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)));

	glBindVertexArray(m_TrigVA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
};


void SandboxLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent& ev) { glViewport(0, 0, ev.GetWidth(), ev.GetHeight()); return true; });
};

