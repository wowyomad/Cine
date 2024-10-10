#include "glash/Cine.hpp"
#include "glash/Core/EntryPoint.hpp"

#include "GameLayer.hpp"

class Game : public Cine::Application
{
public:
	Game()
	{
		PushLayer(new GameLayer());
	}
	~Game()
	{
		
	}
};


Cine::Application* Cine::CreateApplication()
{
	return new Game();
}
