#pragma once


namespace _3DE
{
	template <typename S>
	int& GetStateID() { static int _id = -1; return _id; }


    static int GenerateRandomNumber(int _min = -1, int _max = 1)
    {
        int _range = _max - _min + 1;

        static int _value = rand() % _range + _min;

        return _value;
    }
}

