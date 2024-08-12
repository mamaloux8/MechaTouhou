#ifndef OBSERVER_H
#define OBSERVER_H

template <typename T>
class Observer
{
public:
	//Changer de place ??
	enum State
	{
		isSpawn,
		isHit,
		isDead,
		isEmpty,
	};

	virtual ~Observer() = default;

	virtual void onNotify(const T*, State) = 0;
};

#endif
