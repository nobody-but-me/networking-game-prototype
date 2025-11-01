
#pragma once
#ifndef SINGLETON_HPP
#define SINGLETON_HPP


template <typename T>
class Singleton
{
protected:
    Singleton() {}
public:
    Singleton(const Singleton *singleton) = delete;
    Singleton *operator = (const Singleton *) = delete;
    
    static T *get_instance() {
	static T instance;
	return &instance;
    }
};

#endif//SINGLETON_HPP
