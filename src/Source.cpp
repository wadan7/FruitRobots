#include "World.h"

int main()
{
	FSR::World* world = new FSR::World("maps/map0.bmp");

	while (true)
	{
		world->OnTick();
	}
	delete world;

	return 0;
}