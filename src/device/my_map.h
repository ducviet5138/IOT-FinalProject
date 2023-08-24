#ifndef MY_MAP_H
#define MY_MAP_H

#include <vector>
using namespace std;

template <typename custom>
class MyMap
{
    private:
        vector<String> m_keys;
        vector<custom> m_values;
    public:
        MyMap() 
        {
            m_keys.clear();
            m_values.clear();
        }

        ~MyMap() 
        {
            m_keys.clear();
            m_values.clear();
        }

        void Add(String key, custom value)
        {
            m_keys.push_back(key);
            m_values.push_back(value);
        }

        custom Get(String key)
        {
            int i = 0;
            for ( ; i < m_keys.size(); i++) if (m_keys[i] == key) break;
            return m_values[i]; 
        }
};

#endif
// Notice: 'map' is already appeared in Arduino