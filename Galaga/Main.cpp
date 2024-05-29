#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Scenes.h"

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(std::function(galaga::LoadMainMenuScene));
	return 0;
}