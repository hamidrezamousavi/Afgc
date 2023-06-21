#ifndef LIB_H
#define LIB_H
#include <vector>
#include <utility>
#include <string>
#include <gtkmm.h>
#include <thread>
#include <mutex>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include <glib.h>
#include <gtkmm/grid.h>
using namespace std;
/*

class Channle{
  private:
    vector<string> container{};
    int last_out{0};
    bool open{true};
  public:
    pair<bool,string> get_next();
    void put(string x);
    bool is_open();
    void close();
};
void read_port(Channle *result);
void read_sin(Channle *result);

*/
class Axes{
  private:
    int area_width;
    int area_height; 
    const int x_margin{20};
    const int y_margin{20};
    double x_max;
  //  double data_max;
  //  double data_min;
    double y_max{1.0};
    pair<int,int> origin{0,0};
  
  public:
    Axes();
    void update(int area_width, int area_height, 
          int sample_number, double data_max /*double data_min*/);
    void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    int x(double num);
    int y(double num);
};
class ForceData{
  public:
    double force{};
    int sample_number{};
    double max_force{};
   // double min_force{};
};

class MainWindow;

class ExampleWorker
{
public:
  ExampleWorker();

  // Thread function.
  void do_work(MainWindow* caller);

  void get_data(ForceData* force_data, Glib::ustring* message) const;
  void stop_work();
  bool has_stopped() const;

private:
  // Synchronizes access to member data.
  mutable std::mutex m_Mutex;

  // Data used by both GUI thread and worker thread.
  bool m_shall_stop;
  bool m_has_stopped;
  ForceData force_data;
  Glib::ustring m_message;
};




#endif