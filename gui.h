#ifndef GUI_H
#define GUI_H

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>
#include <glib.h>
#include <vector>
#include <string>
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
class ResultTableModel : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ResultTableModel()
    { add(sample_number); add(sample_force);}

    Gtk::TreeModelColumn<int> sample_number;
    Gtk::TreeModelColumn<double> sample_force;
    
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
  
  Gtk::Label start_point_entry_label;
  Gtk::Label end_point_entry_label;
  Gtk::Entry start_point_entry;
  Gtk::Entry end_point_entry;
  Gtk::Button claculate_button;
  Gtk::Label min_label;
  Gtk::Label min_out_label;
  Gtk::Label max_label;
  Gtk::Label max_out_label;
  Gtk::Label std_label;
  Gtk::Label std_out_label;
  
 // Gtk::Button stop_button;
  MyArea area{};
  Gtk::ScrolledWindow result_ScrolledWindow;
  ResultTableModel result_model;
  Gtk::TreeView result_table;
  Glib::RefPtr<Gtk::ListStore> result_refTreeModel;
  //Gtk::Box m_box1;
  
  Gtk::Grid m_grid;
  Glib::Dispatcher m_Dispatcher;
  ExampleWorker m_Worker;
  std::thread* m_WorkerThread;
};

#endif