
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
  double max = -1.0;
 // double min = 10000000.0;
  double x = 1;
  for (int i = 0; ; ++i) // do until break
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    { 
      std::lock_guard<std::mutex> lock(m_Mutex);
      
    //  double data = sin(i * 0.15707963267948966192313216916398);
      double data = i*2+(100*sin(i * 0.15707));
      force_data.force = data;
      force_data.sample_number = i;
      max = (max > abs(data))?max:abs(data);
    //  min = (min < data)?min:data;
      
      force_data.max_force = max;
  
   //   force_data.min_force = min;
      x = x + 0.1;
      
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
Axes::Axes(){}
void Axes::update(int area_width, int area_height, 
           int sample_number, double data_max /*, 
          double data_min*/){ 
                this->area_width = area_width;
                this->area_height = area_height;
                this->x_max =(sample_number == 0)?1:sample_number; 
                //this->data_max = data_max;
              //  this->data_min = data_min;
              //  double new_y_range = (abs(data_max) > abs(data_min))?
               //             abs(data_max):abs(data_min);
                
                y_max = (y_max > data_max)?y_max:data_max;
               
                }
vector<int> Axes::draw(const Cairo::RefPtr<Cairo::Context>& cr){
  cr->set_line_width(0.5);
  cr->set_source_rgb(0.0, 0.0, 0.8);
  cr->move_to(x_margin, area_height/2);
  cr->line_to(area_width - x_margin, area_height/2);
  cr->move_to(x_margin, y_margin);
  cr->line_to(x_margin, area_height - y_margin);
  cr->stroke();
  int len_x_grid = (area_width - (2*x_margin)) /10;
  int len_y_grid = (area_height - (2*y_margin)) /10;
  cr->set_line_width(0.1);
  cr->set_source_rgb(0.1, 0.1, 0.1);
  cr->move_to(x_margin, area_height/2);
  
 // vector<int> x_grid_cordination{};
  for(int i{0}; i < 11 ; i++){
    cr-> move_to(x_margin + (i* len_x_grid), y_margin);
    cr-> line_to(x_margin + (i* len_x_grid), area_height - y_margin);
  //  x_grid_cordination.push_back(x_margin + (i* len_x_grid));
  }
  
  cr->move_to(x_margin, area_height/2);
 // double grade = y_max / 5;
  
 // vector<int> y_grid_cordination{};
  for(int i{0}; i < 6 ; i++){
    cr-> move_to(x_margin , (area_height/2) - (i*len_y_grid));
   // auto layout = create_pango_layout("Hi there!"); 
    //layout->show_in_cairo_context(cr); 
 //   y_grid_cordination.push_back((area_height/2) - (i*len_y_grid));
    cr-> line_to(area_width - x_margin , (area_height/2) - (i*len_y_grid));
    cr-> move_to(x_margin , (area_height/2) + (i*len_y_grid));
    cr-> line_to(area_width - x_margin , (area_height/2) + (i*len_y_grid));
   // y_grid_cordination.push_back((area_height/2) + (i*len_y_grid));
  }
  cr->stroke();
  vector<int> grid_len{};
  grid_len.push_back(len_x_grid);
  grid_len.push_back(len_y_grid);
  return grid_len;
  
}
int Axes::x(double num){
  
  double pix_factor = (area_width - x_margin)/x_max;
  
    
    
  return static_cast<int>(num * pix_factor + x_margin);
  
}  
int Axes::y(double num){
   
  double pix_factor = (area_height/2 - y_margin)/y_max;
  //std::cout << area_height/2 << '-' <<y_margin  << '-' <<y_max << '-' << pix_factor << std::endl;
  return static_cast<int>(area_height/2 - (num * pix_factor));
  
}  

vector<double> calculate(const vector<ForceData>& data, 
                         int start, int end){
  double min = numeric_limits<double>::infinity();
  double max = -1 * numeric_limits<double>::infinity();
  double sum{0};
  double ave{0};
  double stdv{0};
  double sqrsum{0};
  for(int i{start}; i <= end; i++){
    sum += data[i].force;
    max = (data[i].force > max)?data[i].force:max;
    min = (data[i].force < min)?data[i].force:min;
  }
  ave = sum / (end - start + 1);
  
  for(int i{start}; i <= end; i++){
    sqrsum += pow((data[i].force - ave),2);
  }
  stdv = sqrt(sqrsum / (end - start + 1));                          
  
  return vector<double>{min, max, ave, stdv};
}
    
