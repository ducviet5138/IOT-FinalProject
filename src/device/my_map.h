#ifndef MY_MAP_H
#define MY_MAP_H

#include <vector>
#include <string>
using namespace std;

template <typename custom>
class MyMap
{
    private:
        vector<string> m_keys;
        vector<custom> m_values;
    public:
        void Add(string key, custom value)
        {
            m_keys.push_back(key);
            m_values.push_back(value);
        }

        custom Get(string key)
        {
            int i = 0;
            for ( ; i < m_keys.size(); i++) if (m_keys[i] == key) break;
            return m_values[i]; 
        }
};

#endif