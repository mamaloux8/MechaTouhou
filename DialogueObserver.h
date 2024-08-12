#ifndef DIALOGUE_OBSERVER_H
#define DIALOGUE_OBSERVER_H

class DialogueObserver
{
public:
	//Changer de place ??
	enum State
	{
		hasBegun,
		hasEnded,
	};

	virtual ~DialogueObserver() = default;

	virtual void onNotify(State) = 0;
};

#endif
