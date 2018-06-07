#include <Leap.h>
#include <iostream>
#include <ctime>

#define ROCK 1
#define SCISSOR 2
#define PAPER 3

using namespace Leap;
using namespace std;

Leap::HandList hands;
Leap::FingerList fingers[2];

class MyListener : public Leap::Listener
{
public:
	virtual void onConnect(const Leap::Controller &);
	virtual void onFrame(const Leap::Controller &);
};

void MyListener::onConnect(const Leap::Controller &)
{
	cout << "Connected." << endl;
}

// 프레임 받을 때 마다 수행
void MyListener::onFrame(const Leap::Controller & controller)
{
	const Leap::Frame frame = controller.frame();

	// get hands
	hands = frame.hands();

	// get fingers from a hand.
	fingers[0] = hands[0].fingers();
	fingers[1] = hands[1].fingers();
}

int main()
{
	char keyInput;

	srand(time(NULL));

	std::cout << "program started." << std::endl;

	// connect leap motion.
	Leap::Controller controller;

	// paste the listener
	MyListener listener;
	controller.addListener(listener);

	while (true)
	{
		for (int i = 0; i < 2; i++)
		{
			cout << "hand " << i << endl;

			for (Finger finger : fingers[i])
			{
				cout << finger.tipPosition();
				
				if (finger.tipPosition().y < 100)
					cout << "check";

				cout << endl;
			}
		}

		cin.get();
	}

	// remove listener before exit program.
	controller.removeListener(listener);

	return 1;
}