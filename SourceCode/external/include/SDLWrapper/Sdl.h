#pragma once

namespace SDLWrapper
{
	class Sdl
	{
	public:
		Sdl(int flags);
		~Sdl();
	private:
		Sdl(const Sdl&) = default;
	};
}
