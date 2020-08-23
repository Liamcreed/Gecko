#pragma once

namespace Gecko
{
    struct DeltaTime
    {
    public:
        DeltaTime(float time = 0)
            :m_Time(time)
        {

        }
        float GetSeconds() const {return m_Time;}
        float GetMillieconds() const {return m_Time * 1000;}

        operator float() const {return m_Time; }
    private:
        float m_Time;
    };
    
} // namespace Gecko
