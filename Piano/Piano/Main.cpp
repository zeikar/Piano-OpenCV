#include "Piano.h"
using namespace std;

int main()
{
	Piano piano;

	for (int i = 0; i < 10; i++)
	{
		piano.putFinger(rand() % PIANO_SIZE);
		piano.renderPiano();
		waitKey(20);
		piano.releaseFinger(rand() % PIANO_SIZE);
	}

	waitKey();
	
	return 0;
}