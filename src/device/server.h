#ifndef SERVER_H
#define SERVER_H

class Server{
    private:
        const char* server = "broker.hivemq.com";
        const String main_channel = "/GDrpD2J3jxvzQEy7vGOn/";
    
    public:
        const char* GetChannel(String pram)
        {
            return (main_channel + pram).c_str();
        }
};



#endif