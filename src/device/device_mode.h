#ifndef DEVICE_MODE_H
#define DEVICE_MODE_H

class DeviceMode
{
    private:
        bool working_mode;
        bool person_in_room;
    public:
        DeviceMode()
        {
            working_mode = person_in_room = 1; 
        }

        void UpdateWorkingMode(bool val)
        {
            working_mode = val;
        }

        void UpdatePersonStatus(bool val)
        {
            person_in_room = val;
        }

        bool GetWorkingMode()
        {
            return working_mode;
        }

        bool GetPersonInRoom()
        {
            return person_in_room;
        }
};
#endif