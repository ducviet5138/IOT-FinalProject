#ifndef DEVICE_MODE_H
#define DEVICE_MODE_H

class DeviceMode
{
    private:
        bool working_mode;
    public:
        DeviceMode()
        {
            working_mode = 1; 
        }

        ~DeviceMode(){}

        void UpdateMode(bool val)
        {
            working_mode = val;
        }

        bool GetWorkingMode()
        {
            return working_mode;
        }
};
#endif