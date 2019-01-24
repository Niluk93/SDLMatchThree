#pragma once

//interface to be implemented by a class that wants to trigger callbacks
template<typename T>
class CallbackInterface
{
public:
	CallbackInterface(T* _owner) : mOwner(_owner) { }
	virtual ~CallbackInterface() = default;

	//function signature for the callback.
	//will give the object that triggers the callback as a parameter
	typedef void (T::*Callback)(const CallbackInterface*);

protected:
	T* mOwner; //the object that holds the callback function
};