
#ifndef DELTA_HPP
#define DELTA_HPP


namespace Math
{
    namespace Delta
    {
	void calculate_delta();
	float get_delta_time();
	bool is_frametiming(); // NOTE: horrible name, couldn't find a better one.
	void update();
    }
}


#endif//DELTA_HPP
