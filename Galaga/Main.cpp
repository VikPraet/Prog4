#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <ctime>

#include "Minigin.h"
#include "Scenes.h"

int main(int, char* []) {
	srand(static_cast<unsigned int>(time(nullptr)));

	dae::Minigin engine("../Data/");
	engine.Run(std::function(galaga::LoadMainScene));
	return 0;
}