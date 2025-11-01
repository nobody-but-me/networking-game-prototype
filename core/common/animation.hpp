
#pragma once
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>
#include <vector>

#include <utils/singleton.hpp>
#include <common/enums.hpp>
#include <utils/log.hpp>

class Animation: public Singleton<Animation>
{
    private:
	bool m_initialized = false;
	bool m_is_playing = false;
	bool m_in_reverse = false;
	bool m_autoplay = false;
	bool m_played = false;
	
	std::vector<unsigned int> m_frames;
	unsigned int m_delay_length = 0;
	unsigned int m_delay = 0;
	
	unsigned int m_index = 0;
	unsigned int m_idx = 0;
	
	AnimationType m_type;
	
    public:
	std::string name;
	
	bool get_initialized() { return this->m_initialized; }
	bool get_is_playing()  { return this->m_is_playing; }
	bool get_in_reverse()  { return this->m_in_reverse; }
	bool get_autoplay()    { return this->m_autoplay; }
	bool get_played()      { return this->m_played; }
	
	void set_initialized(bool new_initialized) { this->m_initialized = new_initialized; }
	void set_is_playing(bool new_is_playing)   { this->m_is_playing = new_is_playing; }
	void set_in_reverse(bool new_in_reverse)   { this->m_in_reverse = new_in_reverse; }
	void set_autoplay(bool new_autoplay)       { this->m_autoplay = new_autoplay; }
	void set_played(bool new_played)           { this->m_played = new_played; }
	
	std::vector<unsigned int> get_frames() { return this->m_frames; }
	AnimationType get_animation_type() { return this->m_type; }
	unsigned int get_delay() { return this->m_delay_length; }
	unsigned int get_index() { return this->m_index; }
	
	void set_frames(std::vector<unsigned int>new_frames) { this->m_frames = new_frames; }
	void set_animation_type(AnimationType new_type) { this->m_type = new_type; }
	void set_delay(unsigned int new_delay) { this->m_delay_length = new_delay; }
	void set_index(unsigned int new_index) { this->m_index = new_index; }
	
	void process() {
	    switch (this->get_animation_type()) {
	    case AnimationType::PING_PONG:
		if (this->m_delay < this->m_delay_length) this->m_delay++;
		else {
		    size_t length = this->get_frames().size();
		    
		    this->m_index = this->m_frames[this->m_idx];
		    if (this->get_in_reverse()) {
			if (this->m_idx > 0) this->m_idx--;
		    } else {
			if (this->m_idx < static_cast<unsigned int>(length)) this->m_idx++;
		    }
		    
		    if (this->m_idx == static_cast<unsigned int>(length)) this->set_in_reverse(true);
		    else if (this->m_idx == 0) this->set_in_reverse(false);
		    
		    this->m_delay = 0;
		}
		break;
	    case AnimationType::LOOP:
		if (this->m_delay < this->m_delay_length) this->m_delay++;
		else {
		    size_t length = this->get_frames().size();
		    if (this->m_idx < static_cast<unsigned int>(length)) {
			this->m_index = this->m_frames[this->m_idx];
			this->m_idx++;
		    } else {
			this->m_index = this->m_frames[0];
			this->m_idx = 0;
		    }
		    this->m_delay = 0;
		}
		break;
	    case AnimationType::ONCE:
		if (this->m_delay < this->m_delay_length) this->m_delay++;
		else {
		    size_t length = this->get_frames().size();
		    if (this->m_idx < static_cast<unsigned int>(length)) {
			this->m_index = this->m_frames[this->m_idx];
			this->m_idx++;
		    } else this->stop();
		    this->m_delay = 0;
		}
		break;
	    }
	}
	void stop() { this->set_is_playing(false); }
	void play() { this->set_is_playing(true);  }
};


#endif//ANIMATION_HPP

