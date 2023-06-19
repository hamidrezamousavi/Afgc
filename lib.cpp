#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include "lib.h"
/*
pair<bool,string> Channle::get_next(){
   // cout << container.size() << last_out << endl;
    if (last_out < container.size()){
      
        pair<bool, string> data{};
        data = {true, container[last_out]};
        last_out++;
        return data;
      }
    else
      return pair{false,"0"};
    }

void Channle::put(string x){
      
      container.push_back(x);
    }

bool Channle::is_open(){
       return open;
    }

void Channle::close(){
       open = false;
    }

void read_port(Channle *result){
  HANDLE h_Serial;
  h_Serial = CreateFile("COM1",GENERIC_READ | GENERIC_WRITE,
                     0,
                     0,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     0);
  if(h_Serial==INVALID_HANDLE_VALUE){
    if(GetLastError()==ERROR_FILE_NOT_FOUND){
        cout << "serial port not found";
    }
    cout << "there is some problem with opening of serial port" << endl;
  }
  DCB dcbSerialParam = {0};
  dcbSerialParam.DCBlength=sizeof(dcbSerialParam);

  if (!GetCommState(h_Serial, &dcbSerialParam)) {
      cout << "serial port not available " << endl;
  }
  
  dcbSerialParam.BaudRate=CBR_9600;
  dcbSerialParam.ByteSize=8;
  dcbSerialParam.StopBits=ONESTOPBIT;
  dcbSerialParam.Parity=NOPARITY;
  
  if(!SetCommState(h_Serial, &dcbSerialParam)){
      cout << "serail port setting error"<< endl;
  }
  COMMTIMEOUTS timeout={0};
  timeout.ReadIntervalTimeout=200;
  timeout.ReadTotalTimeoutConstant=200;
  timeout.ReadTotalTimeoutMultiplier=15;
  timeout.WriteTotalTimeoutConstant=60;
  timeout.WriteTotalTimeoutMultiplier=8; 
  if(!SetCommTimeouts(h_Serial, &timeout)){
      cout << "serail port timeout setting error" << endl;
  }
  int n = 10;
  char sBuff[n+1] = {0};
  DWORD dwRead = 0;
 
  while(result->is_open()){
    if(!ReadFile(h_Serial, sBuff, n, &dwRead, NULL)){
      cout << "error on reading port buffer" <<endl;
    }
    
    result->put(sBuff);
    
  }
  CloseHandle(h_Serial);
  
} 
void read_sin(Channle *result){
  double x{0};
  while(result->is_open()){
    string data = to_string(std::sin(x));  
    x += 0.01;
    
    result->put(data);
    
  }
  
  
} 
 */

#include "gui.h"
#include <sstream>
#include <chrono>

ExampleWorker::ExampleWorker() :
  m_Mutex(),
  m_shall_stop(false),
  m_has_stopped(false),
  force_data{},
  m_message()
{
}

// Accesses to these data are synchronized by a mutex.
// Some microseconds can be saved by getting all data at once, instead of having
// separate get_fraction_done() and get_message() methods.
void ExampleWorker::get_data(ForceData* force_data, Glib::ustring* message) const
{
  std::lock_guard<std::mutex> lock(m_Mutex);

  if (this->force_data.force)
    *force_data = this->force_data;

  if (message)
    *message = m_message;
}

void ExampleWorker::stop_work()
{
  std::lock_guard<std::mutex> lock(m_Mutex);
  m_shall_stop = true;
}

bool ExampleWorker::has_stopped() const
{
  std::lock_guard<std::mutex> lock(m_Mutex);
  return m_has_stopped;
}

void ExampleWorker::do_work(MainWindow* caller)
{
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_has_stopped = false;
   // m_fraction_done = 0.0;
  //  m_message = "";
  } // The mutex is unlocked here by lock's destructor.

  // Simulate a long calculation.
  for (int i = 0; ; ++i) // do until break
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    {
      std::lock_guard<std::mutex> lock(m_Mutex);
      force_data.force = std:: sin(i * 0.01);
      force_data.sample_number = i;
      force_data.max_force = (force_data.max_force < sin(i * 0.01))?
                              sin(i * 0.01) :force_data.max_force;
  
      force_data.min_force = (force_data.min_force > sin(i * 0.01))?
      sin(i * 0.01) :force_data.min_force;
      
      //m_fraction_done =std::sin(i * 0.01);

      if (i % 4 == 3)
      {
        std::ostringstream ostr;
   //     ostr << (m_fraction_done * 100.0) << "% done\n";
        m_message += ostr.str();
      }

      
      if (m_shall_stop)
      {
        m_message += "Stopped";
        break;
      }
    }

    caller->notify();
  }

  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_shall_stop = false;
    m_has_stopped = true;
  }

  caller->notify();
} 
    
