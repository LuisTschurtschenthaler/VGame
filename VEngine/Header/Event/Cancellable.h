#ifndef CANCELLABLE_H
#define CANCELLABLE_H

class Cancellable {

private:
	bool _isCancelled = false;


public:
	virtual ~Cancellable() { }
	
	bool const isCancelled() const { return _isCancelled; }
	void const setCancelled(bool cancelled) { _isCancelled = cancelled; }

};

#endif // CANCELLABLE_H
