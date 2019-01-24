#pragma once

#include <glm/fwd.hpp>
#include <memory>

namespace SDLWrapper {
	class Updater;
	class Engine {
	public:
		enum Texture {
			TEXTURE_BACKGROUND,
			TEXTURE_GEMS_START,

			TEXTURE_APPLE = TEXTURE_GEMS_START,
			TEXTURE_BREAD,
			TEXTURE_BROCCOLI,
			TEXTURE_COCONUT,
			TEXTURE_MILK,
			TEXTURE_PURPLE,
			TEXTURE_TANGERINE,

			TEXTURE_GEMS_END = TEXTURE_TANGERINE,
			TEXTURE_MAX
		};

		Engine(const char* assetsDirectory);
		~Engine();

		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Texture texture) const;
		int GetTextureWidth(Texture texture) const;
		void Render(Texture texture, const glm::mat4& transform) const;
		void Render(Texture texture, float x, float y, float rotation = 0.0f) const;


		void Write(const char* text, const glm::mat4& transform) const;
		void Write(const char* text, float x, float y, float rotation = 0.0f) const;

		static float CalculateStringWidth(const char* text);
		static int GetWidth();
		static int GetHeight();

	private:
		struct EngineImplementation;
		std::unique_ptr<EngineImplementation> mPimpl;
	};
}