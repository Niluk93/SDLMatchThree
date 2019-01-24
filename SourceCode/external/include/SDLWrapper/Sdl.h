#pragma once

namespace SDLEngine {
	class Sdl {
	public:
		Sdl(int flags);
		~Sdl();
	private:
		Sdl(const Sdl&) = default;
	};
}
