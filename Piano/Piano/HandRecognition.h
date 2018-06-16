#pragma once

#include <Leap.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace Leap;

class MyListener : public Leap::Listener
{
private:
	int prevNote = -1;
	int currentNote = 0;

	HandList hands;
	FingerList fingers[2];

	virtual int getThePainoPosition(double _input);

public:
	virtual void onConnect(const Leap::Controller &);
	virtual void onFrame(const Leap::Controller &);
	virtual bool isEnable();
	virtual int pressNotes();
	virtual int realeaseNotes();
	virtual bool isNoteChanged();
};

// connect the leap motion
void MyListener::onConnect(const Leap::Controller &)
{}

// 프레임 받을 때 마다 수행
void MyListener::onFrame(const Leap::Controller & controller)
{
	const Frame frame = controller.frame();

	// get hands
	hands = frame.hands();

	// get fingers from a hand.
	fingers[0] = hands[0].fingers();
	fingers[1] = hands[1].fingers();

	// get the pressed finger position
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (hands[i].isValid())
			{
				// if finger is pressed
				if (abs(hands[0].palmPosition().y - fingers[i][j].tipPosition().y) > 40)
				{
					// get the finger position in 2 dimension
					currentNote = getThePainoPosition(fingers[i][j].tipPosition().x);
				}
			}
		}
	}
}

int MyListener::getThePainoPosition(double _fingerPosition)
{
	double temp = 0;
	
	if (_fingerPosition > 300)
		temp = 600;

	else if (_fingerPosition < -300)
		temp = 0;

	else
		temp = _fingerPosition + 300;

	temp = temp / 600 * 14;

	return int(round(temp));
}

bool MyListener::isEnable()
{
	if (hands[0].isValid() || hands[1].isValid())
		return true;

	return false;
}

int MyListener::pressNotes()
{
	cout << "press " << currentNote << endl;

	return currentNote;
}

int MyListener::realeaseNotes()
{
	int releaseNote = prevNote;

	cout << "realease " << releaseNote << endl;

	prevNote = currentNote;

	return releaseNote;
}

bool MyListener::isNoteChanged()
{		
	if (currentNote != prevNote)
		return true;

	return false;
}