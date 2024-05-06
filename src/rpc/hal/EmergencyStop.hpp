#ifndef EMERGENCY_STOP_HPP
#define EMERGENCY_STOP_HPP

class EmergencyStop
{
protected:
    unsigned long lastAlive = 0;

public:
    void alive();
    bool check();

    /**
     * @return `true` iff emergency alive timeout has been reached and the car should be stopped
     */
    void setState(bool enabled);
};

#endif