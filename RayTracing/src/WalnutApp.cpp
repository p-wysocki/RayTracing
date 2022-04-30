#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Viewport");

		// get dimensions of current window
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		// what's this sorcery with {} and (float)?
		if (m_Image)
		{
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });
		}

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		// tried calling it as "Timer timer();", why didn't that work?
		Timer timer;

		// render scene if image doesn't exist or window size changes
		if (!m_Image || m_ViewportHeight != m_Image->GetHeight() || m_ViewportWidth != m_Image->GetWidth())
		{
			// create a shared pointer to an Image object
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);

			// we use brackets because m_ImageData is an array of data
			delete[] m_ImageData;

			// does it create an array (vector in memory?) of given length? Is it a pointer or just an integer?
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		// iterate over pixels in array and set them to a value
		for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
		{
			m_ImageData[i] = Random::UInt();

			// set alpha channel
			m_ImageData[i] |= 0xff000000;
		}

		// upload image data
		m_Image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	std::shared_ptr<Image> m_Image;

	// does it matter if they're 32bit?
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	// buffer for image data
	uint32_t* m_ImageData = nullptr;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}