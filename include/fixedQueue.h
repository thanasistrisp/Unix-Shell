#include <queue>
#include <deque>

// create fixed size queue by inheriting from std::deque
template <typename T>
class fixed_queue : public std::deque<T>
{
public:
	fixed_queue(size_t size) : size_(size) {}

	void push_back(const T &value)
	{
		if (this->size() == size_)
			this->pop_front();
		std::deque<T>::push_back(value);
	}

private:
	size_t size_;
};
