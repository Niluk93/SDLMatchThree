#include "Engine.h"

#include <stdexcept>
#include <algorithm>
#include <vector>

#define GLM_FORCE_RADIANS 
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <sdl/Sdl.h>

#include "Font.h"
#include "GlContext.h"
#include "Sdl.h"
#include "SdlWindow.h"
#include "SdlSurface.h"
#include "Updater.h"


namespace SDLWrapper
{
	static const int WindowWidth = 640;
	static const int WindowHeight = 960;
	static const float MaxFrameTicks = 300.0f;
	static const float TextScale = 0.5f;

	struct Engine::EngineImplementation
	{
		Sdl mSdl;
		SdlWindow mSdlWindow;
		GlContext mGlContext;
		std::unique_ptr<SdlSurface> mSdlSurfaceContainer[Engine::TEXTURE_MAX];
		std::unique_ptr<SdlSurface> mFontSdlSurface;
		float mElapsedTicks;
		float mLastFrameSeconds;
		Updater* mUpdater;
		bool mQuit;

		float mMouseX;
		float mMouseY;
		bool mMouseButtonDown;

		EngineImplementation()
			: mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE)
			, mSdlWindow(WindowWidth, WindowHeight)
			, mGlContext(mSdlWindow)
			, mLastFrameSeconds(1.0f / 60.0f)
			, mMouseX(WindowWidth * 0.5f)
			, mMouseY(WindowHeight * 0.5f)
			, mMouseButtonDown(false)
			, mQuit(false)
			, mUpdater(nullptr)
			, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
		{
		}

		void Start();
		void ParseEvents();
	};

	Engine::Engine(const char* assetsDirectory)
		: mPimpl(new EngineImplementation)
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetSwapInterval(1);

		glEnable(GL_TEXTURE_2D);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::string assets(assetsDirectory);
		std::string background = assets; background += "/BackGround.png";

		std::string apple = assets; apple += "/Apple.png";
		std::string bread = assets; bread += "/Bread.png";
		std::string broccoli = assets; broccoli += "/Broccoli.png";
		std::string coconut = assets; coconut += "/Coconut.png";
		std::string milk = assets; milk += "/Milk.png";
		std::string purple = assets; purple += "/Purple.png";
		std::string tangerine = assets; tangerine += "/Tangerine.png";

		std::string font = assets; font += "/berlin_sans_demi_72_0.png";

		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BACKGROUND].reset(new SdlSurface(background.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_APPLE].reset(new SdlSurface(apple.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BREAD].reset(new SdlSurface(bread.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BROCCOLI].reset(new SdlSurface(broccoli.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_COCONUT].reset(new SdlSurface(coconut.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_MILK].reset(new SdlSurface(milk.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_PURPLE].reset(new SdlSurface(purple.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_TANGERINE].reset(new SdlSurface(tangerine.c_str()));
		mPimpl->mFontSdlSurface.reset(new SdlSurface(font.c_str()));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, WindowWidth, WindowHeight, 0.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
	}

	Engine::~Engine()
	{
	}

	float Engine::GetLastFrameSeconds() const
	{
		return mPimpl->mLastFrameSeconds;
	}

	float Engine::GetMouseX() const
	{
		return mPimpl->mMouseX;
	}

	float Engine::GetMouseY() const
	{
		return mPimpl->mMouseY;
	}

	bool Engine::GetMouseButtonDown() const
	{
		return mPimpl->mMouseButtonDown;
	}

	void Engine::Quit()
	{
		mPimpl->mQuit = true;
	}

	void Engine::Start(Updater& updater)
	{
		mPimpl->mUpdater = &updater;
		mPimpl->mSdlWindow.Show();
		mPimpl->Start();
	}

	int Engine::GetTextureHeight(Texture texture) const
	{
		return mPimpl->mSdlSurfaceContainer[texture]->Height();
	}

	int Engine::GetTextureWidth(Texture texture) const
	{
		return mPimpl->mSdlSurfaceContainer[texture]->Width();
	}

	void Engine::Render(Engine::Texture texture, const glm::mat4& transform) const
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));

		SdlSurface& surface = *mPimpl->mSdlSurfaceContainer[texture];

		surface.Bind();

		glBegin(GL_QUADS);
		glTexCoord2i(0, 1); glVertex2i(0, surface.Height());
		glTexCoord2i(1, 1); glVertex2i(surface.Width(), surface.Height());
		glTexCoord2i(1, 0); glVertex2i(surface.Width(), 0);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glEnd();
	}

	void Engine::Render(Texture texture, float x, float y, float rotation, float scale) const
	{
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation)
		{
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (scale)
		{
			transformation = glm::scale(transformation, glm::vec3(scale, scale, scale));
		}
		Render(texture, transformation);
	}

	Glyph& FindGlyph(char c)
	{
		auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
		if (found == std::end(Font) || c < *found)
		{
			found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
		}
		return *found;
	}

	float Engine::CalculateStringWidth(const char* text)
	{
		int advance = 0;
		for (; *text; ++text)
		{
			Glyph& g = FindGlyph(*text);
			advance += g.advance;
		}
		return advance * TextScale;
	}

	void Engine::Write(const char* text, const glm::mat4& transform) const
	{
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		int advance = 0;
		for (; *text; ++text)
		{
			Glyph& g = FindGlyph(*text);

			float fontTexWidth = static_cast<float>(mPimpl->mFontSdlSurface->Width());
			float fontTexHeight = static_cast<float>(mPimpl->mFontSdlSurface->Height());

			float uvLeft = static_cast<float>(g.x) / fontTexWidth;
			float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
			float uvBottom = static_cast<float>(g.y) / fontTexHeight;
			float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

			float worldLeft = static_cast<float>(g.xoffset + advance);
			float worldRight = static_cast<float>(g.xoffset + g.width + advance);
			float worldBottom = static_cast<float>(g.yoffset);
			float worldTop = static_cast<float>(g.yoffset + g.height);

			mPimpl->mFontSdlSurface->Bind();

			glBegin(GL_QUADS);
			glTexCoord2f(uvLeft / 2.0f, uvTop / 2.0f); glVertex2f(worldLeft * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvTop / 2.0f); glVertex2f(worldRight * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvBottom / 2.0f); glVertex2f(worldRight * TextScale, worldBottom * TextScale);
			glTexCoord2f(uvLeft / 2.0f, uvBottom / 2.0f); glVertex2f(worldLeft * TextScale, worldBottom * TextScale);
			glEnd();
			advance += g.advance;
		}
	}

	void Engine::Write(const char* text, float x, float y, float rotation, float scale) const
	{
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation)
		{
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (scale)
		{
			transformation = glm::scale(transformation, glm::vec3(scale, scale, scale));
		}
		Write(text, transformation);
	}

	int Engine::GetWidth()
	{
		return WindowWidth;
	}

	int Engine::GetHeight()
	{
		return WindowHeight;
	}

	void Engine::EngineImplementation::Start()
	{
		while (!mQuit)
		{
			SDL_GL_SwapWindow(mSdlWindow);
			glClear(GL_COLOR_BUFFER_BIT);

			ParseEvents();

			float currentTicks = static_cast<float>(SDL_GetTicks());
			float lastFrameTicks = currentTicks - mElapsedTicks;
			mElapsedTicks = currentTicks;

			lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
			mLastFrameSeconds = lastFrameTicks * 0.001f;

			if (mUpdater)
			{
				mUpdater->Update();
			}
		}
	}

	void Engine::EngineImplementation::ParseEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			case SDL_KEYDOWN:
				mQuit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mMouseButtonDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mMouseButtonDown = false;
				break;
			case SDL_MOUSEMOTION:
				mMouseX = static_cast<float>(event.motion.x);
				mMouseY = static_cast<float>(event.motion.y);
				break;
			default:
				break;
			}
		}
	}
}
