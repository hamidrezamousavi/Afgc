#ifndef GUI_H
#define GUI_H

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>
#include <glib.h>
#include <vector>
#include "lib.h"

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  std::vector<ForceData> data{};
  virtual ~MyArea();

protected:
  
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  Axes axe{};
};


class MainWindow : public Gtk::Window
{

public:
  MainWindow();
  void notify();
  virtual ~MainWindow();

protected:
  //Signal handlers:
  void on_start_clicked();
 // void on_stop_clicked();
 // void update_start_stop_buttons();
  void update_widgets();

  // Dispatcher handler.
  void on_notification_from_worker_thread();

  //Member widgets:
  Gtk::Button start_button;
  Gtk::Label force_label;
  
 // Gtk::Button stop_button;
  MyArea area{};
  //Gtk::Box m_box1;
  Gtk::Grid m_grid;
  Glib::Dispatcher m_Dispatcher;
  ExampleWorker m_Worker;
  std::thread* m_WorkerThread;
};

#endif