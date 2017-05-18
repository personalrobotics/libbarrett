/**
 * @file logger.h
 */

#ifndef BARRETT_LOGGER_H_
#define BARRETT_LOGGER_H_

#include <string>
#include <boost/thread/thread.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <fstream>
#include <queue>

#include <native/timer.h>


namespace barrett {

struct LogData {
    enum Type {
        TYPE_SENT = 0,
        TYPE_RECEIVED = 1,
    };

    Type type;
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t can_data[8];
    RTIME timestamp;
};

inline std::ostream& operator <<(std::ostream& stream, const LogData& logData)
{
    stream << logData.type << ' '
           << logData.timestamp << ' '
           << static_cast<uint32_t>(logData.can_dlc) << ' ';

    std::ios state(NULL);
    state.copyfmt(stream);

    stream << std::hex << std::setfill('0') << std::setw(sizeof(uint32_t) * 2) << logData.can_id << ' ';

    for (uint8_t data_index = 0; data_index < logData.can_dlc; ++data_index)
    {
        stream << std::hex << std::setfill('0') << std::setw(2)
               << static_cast<uint32_t>(logData.can_data[data_index]);
    }

    stream.copyfmt(state);

    return stream;
}

class Logger {
public:
    Logger(double printCycle, const std::string& filename)
    : mLogBuffer()
    , mPrintCycle(printCycle)
    , mLogfile()
    , mThread(boost::thread(&Logger::print, this))
    {
      mLogfile.open(filename.c_str(), std::ofstream::out);
      mLogfile << "Logging begins here" << std::endl;
      std::cout << "Logger created here" << std::endl;
    }

    void log(LogData log){
        mLogBuffer.push(log);
    }

    ~Logger()
    {
        mLogfile.close();
    }

protected:
    std::queue<LogData> mLogBuffer;
    double mPrintCycle;
    std::ofstream mLogfile;
    boost::thread mThread;

    void print()
    {
        while(1)
        {
            while (!mLogBuffer.empty())
            {
                LogData& log = mLogBuffer.front();
                mLogfile << log << '\n';
                mLogBuffer.pop();
            }

            mLogfile << std::flush;
            boost::this_thread::sleep(boost::posix_time::seconds(mPrintCycle));            
        }        

        
    }
};
}


#endif /* BARRETT_OS_H_ */
