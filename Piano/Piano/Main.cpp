#include "Piano.h"
using namespace std;

int main()
{
	Piano piano;

	for (int i = 0; i < PIANO_SIZE; i++)
	{
		piano.putFinger(i);
		piano.renderPiano();
		waitKey();
		piano.releaseFinger(i);
	}

	waitKey();
	
	return 0;
}