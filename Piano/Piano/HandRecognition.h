#pragma once

#include <Leap.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define HAND_MIN_POSITION -250
#define HAND_MAX_POSITION 250

#define FINGER_VELOCITY 400

using namespace std;
using namespace Leap;

class MyListener : public Leap::Listener
{
private:
	int prevNote = -1;
	int currentNote = -1;

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
	virtual bool isPressed();

	virtual vector<double> getPositions();
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
				if (fingers[i][j].tipVelocity().y < -FINGER_VELOCITY && (hands[0].palmPosition().y - fingers[i][j].tipPosition().y) > 30)
				{
					int pos = getThePainoPosition(fingers[i][j].tipPosition().x);
					
					// get the finger position in 2 dimension
					if (pos != currentNote)
					{
						prevNote = currentNote;
						currentNote = pos;
					}
				}

				// if finger is released
				else if (fingers[i][j].tipVelocity().y > FINGER_VELOCITY)
				{
					int pos = getThePainoPosition(fingers[i][j].tipPosition().x);

					if (pos == currentNote)
					{
						prevNote = currentNote;
						currentNote = -1;
					}
				}
			}
		}
	}
}

vector<double> MyListener::getPositions()
{
	vector<double> fingerList;

	for (int i = 0; i < 2; i++)
	{
		if (hands[i].isValid())
		{
			for (int j = 0; j < 5; j++)
			{
				if (fingers[i][j].isValid())
				{
					double fingerPos = fingers[i][j].tipPosition().x;
					fingerList.push_back(fingerPos);
				}
			}
		}
	}

	return fingerList;
}

int MyListener::getThePainoPosition(double _fingerPosition)
{
	double temp = 0;
	
	if (_fingerPosition > HAND_MAX_POSITION)
		temp = HAND_MAX_POSITION - HAND_MIN_POSITION;

	else if (_fingerPosition < HAND_MIN_POSITION)
		temp = 0;

	else
		temp = _fingerPosition - HAND_MIN_POSITION;

	temp = temp / (HAND_MAX_POSITION - HAND_MIN_POSITION) * 24;

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
	return currentNote;
}

int MyListener::realeaseNotes()
{
	int releaseNote = prevNote;
	prevNote = currentNote;

	return releaseNote;
}

bool MyListener::isNoteChanged()
{		
	if (currentNote != prevNote)
		return true;

	return false;
}

bool MyListener::isPressed()
{
	if (currentNote == -1)
		return false;

	return true;
}