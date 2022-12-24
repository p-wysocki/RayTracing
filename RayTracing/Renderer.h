#pragma once

#include <memory>

#include "Walnut/Image.h"

class Renderer {
public:
	Renderer() = default;

	std::shared_ptr<Walnut::Image> GetFinalImage() const;
	void OnResize(uint32_t width, uint32_t height);
	void Render();
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};
