#include "Piano.h"
using namespace std;

int main()
{
	Piano piano;

	// 미를 누름
	piano.putFinger(4);
	piano.putFinger(5);
	piano.putFinger(6);
	// 화면에 출력
	piano.renderPiano();


	waitKey();
	
	return 0;
}