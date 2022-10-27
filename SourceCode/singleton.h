#ifndef _SINGLETON_H
#define _SINGLETON_H
#include <queue>
#include <string>
template<typename T>
class Singleton
{
public:
	static T* GetInstance();
	static void destroy();

private:

	Singleton(Singleton const&) {};
	Singleton& operator=(Singleton const&) {};

protected:
	static T* m_instance;

	Singleton() { m_instance = static_cast <T*> (this); };
	~Singleton() {  };

};

template<typename T>
typename T* Singleton<T>::m_instance = 0;

template<typename T>
T* Singleton<T>::GetInstance()
{
	if (!m_instance)
	{
		Singleton<T>::m_instance = new T();
	}

	return m_instance;
}

template<typename T>
void Singleton<T>::destroy()
{
	delete Singleton<T>::m_instance;
	Singleton<T>::m_instance = 0;
}



struct Notifier
{
	std::queue<std::string> queue;
	bool GetRead() { return hasBeenRead; };
	void SetRead(bool value = true) { this->hasBeenRead = value; };
	void Notify(const char* str)
	{
		if (queue.size() >= size)
		{
			queue.pop();
			queue.push(str);
		}
		else
			queue.push(str);

		hasBeenRead = false;
	};
	void Clear() 
	{
		while (!queue.empty()) queue.pop();
	}
	bool hasBeenRead = false;
	uint size = 8;
};

extern Singleton<Notifier> logger;
#endif