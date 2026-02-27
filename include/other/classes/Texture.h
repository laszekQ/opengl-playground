#pragma once
class Texture2D
{
    unsigned int ID;
    int width, height, nrChannels;
public:
    Texture2D(const char* path, int desired_channels);
    unsigned int getId();
    void setParameteri(unsigned int option, unsigned int mode);
};
