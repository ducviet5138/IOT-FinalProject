#ifndef DEVICE_CONNECTION_H
#define DEVICE_CONNECTION_H

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


//WiFiClient m_espClient;
//PubSubClient av_client(m_espClient);
#endif