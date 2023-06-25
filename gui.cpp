#include "gui.h"

//#include <cairomm/context.h>
#include <iostream>

MainWindow::MainWindow()
: start_button("Start"),
  force_label("0.0"),
  start_point_entry_label("From"),
  end_point_entry_label("To"),
  start_point_entry(),
  end_point_entry(),
  claculate_button("Calculate"),
  min_label("Min"),
  min_out_label("  2.561   "),
  max_label("Max"),
  max_out_label(" 123.5    "),
  std_label("Std"),
  std_out_label("+_2.5"),
  m_Dispatcher(),
  m_Worker(),
  m_WorkerThread(nullptr) 
{
  // Sets the border width of the window.
  set_border_width(10);
  //set_default_size(900,500);
  //start_button.set_default_size(50,20);
  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  start_button.signal_clicked().connect(sigc::mem_fun(*this,
              &MainWindow::on_start_clicked));
 // stop_button.signal_clicked().connect(sigc::mem_fun(*this,
   //           &MainWindow::on_stop_clicked));
  m_Dispatcher.connect(sigc::mem_fun(*this, 
              &MainWindow::on_notification_from_worker_thread));

  // This packs the button into the Window (a container).
  result_ScrolledWindow.add(result_table);
  result_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  result_refTreeModel = Gtk::ListStore::create(result_model);
  result_table.set_model(result_refTreeModel);
  /*
  Gtk::TreeModel::Row row = *(result_refTreeModel->append());
  row[result_model.sample_number] = 1;
  row[result_model.sample_force] = 1.1;
  
  row = *(result_refTreeModel->append());
  row[result_model.sample_number] = 2;
  row[result_model.sample_force] = 2.2;

  row = *(result_refTreeModel->append());
  row[result_model.sample_number] = 3;
  row[result_model.sample_force] = 3.3;
*/
  result_table.append_column("No.", result_model.sample_number);
  result_table.append_column_numeric("Force",result_model.sample_force,"%5.4f");
  
  
  
  
  
  m_grid.set_row_homogeneous(true);
  m_grid.set_column_homogeneous(true);
  add(m_grid);
  m_grid.attach(area,20, 0, 40, 50);
  m_grid.attach(start_button,0, 0, 5, 5);
  start_button.set_margin_start	(20);	

  m_grid.attach(force_label,0, 20, 10, 10);
  
  m_grid.attach(start_point_entry, 46, 51, 10, 5);
  m_grid.attach(start_point_entry_label, 56, 51, 5, 5);
  
  m_grid.attach(end_point_entry,46 , 56, 10, 5);
  m_grid.attach(end_point_entry_label, 56, 56, 5, 5);
  
  m_grid.attach(claculate_button,46 , 61, 10, 5);
 // m_grid.attach(start_point_entry, 30, 78, 10, 5);
  
  
  
 
  
 // start_point_entry.set_margin_start(40);
  
  
  m_grid.attach(min_label, 40, 50, 5, 5);
  m_grid.attach(min_out_label, 40, 55, 5, 5);
  m_grid.attach(max_label, 35, 50, 5, 5);
  m_grid.attach(max_out_label, 35, 55, 5, 5);
  m_grid.attach(std_label, 30, 50, 5, 5);
  m_grid.attach(std_out_label, 30, 55, 5, 5);
  
  result_ScrolledWindow.set_margin_start(20);
  result_ScrolledWindow.set_margin_end(40);
  m_grid.attach(result_ScrolledWindow,0,30,10,30);
  
  //m_box1.pack_start(start_button);
 // m_box1.pack_start(force_label);
 // m_box1.pack_start(stop_button);
  //add(start_button);
  //add(force_label);
 // add(stop_button);

  // The final step is to display this newly created widget...
  //start_button.show();
 // force_label.show();
 // result_ScrolledWindow.show();
  
 // stop_button.show();
 // m_grid.pack_start(area);
//  update_start_stop_buttons();
 // area.show();
 // m_grid.show();
  show_all_children();
  
}
  
   

MainWindow::~MainWindow(){}

void MainWindow::on_start_clicked()
{ if(start_button.get_label() == "Start"){
    std::cout << "Hello World" << std::endl;
    if (m_WorkerThread)
      {
        std::cout << "Can't start a worker thread while another one is running." << std::endl;
    }
    else
    {
    // Start a new worker thread.
      start_button.set_label("Stop");
      m_WorkerThread = new std::thread(
            [this]
                  {
                    m_Worker.do_work(this);
                  });
    }
  }
  else{
    std::cout << "bye World" << std::endl;
    start_button.set_label("Start");
    if (!m_WorkerThread){
      std::cout << "Can't stop a worker thread. None is running." << std::endl;
    }
    else{
   // Order the worker thread to stop.
    m_Worker.stop_work();
    if (m_WorkerThread->joinable())
      m_WorkerThread->join();
   // stop_button.set_sensitive(false);
    }
     
  }
 // update_start_stop_buttons();
}
/*
void MainWindow::on_stop_clicked()
{
  std::cout << "bye World" << std::endl;
  if (!m_WorkerThread)
  {
    std::cout << "Can't stop a worker thread. None is running." << std::endl;
  }
  else
  {
   // Order the worker thread to stop.
    m_Worker.stop_work();
    if (m_WorkerThread->joinable())
      m_WorkerThread->join();
    stop_button.set_sensitive(false);
  }
  
 // hide();
}
void MainWindow::update_start_stop_buttons()
{
  const bool thread_is_running = m_WorkerThread != nullptr;

  start_button.set_sensitive(!thread_is_running);
  stop_button.set_sensitive(thread_is_running);
}*/
void MainWindow::update_widgets()
{
  ForceData force_data{};
  Glib::ustring message_from_worker_thread;
  m_Worker.get_data(&force_data, &message_from_worker_thread);
 // std::cout << fraction_done<< std::endl;
  force_label.set_text(std::to_string(force_data.force)); 
  area.data.push_back(force_data);
  area.queue_draw();
  Gtk::TreeModel::Row row = *(result_refTreeModel->append());
  row[result_model.sample_number] = force_data.sample_number;
  row[result_model.sample_force] = force_data.force;
  
  
  
  //m_ProgressBar.set_fraction(fraction_done);
/*
  if (message_from_worker_thread != m_TextView.get_buffer()->get_text())
  {
    auto buffer = m_TextView.get_buffer();
    buffer->set_text(message_from_worker_thread);

    // Scroll the last inserted line into view. That's somewhat complicated.
    Gtk::TextIter iter = buffer->end();
    iter.set_line_offset(0); // Beginning of last line
    auto mark = buffer->get_mark("last_line");
    buffer->move_mark(mark, iter);
    m_TextView.scroll_to(mark);
    // TextView::scroll_to(iter) is not perfect.
    // We do need a TextMark to always get the last line into view.
  }*/
}
void MainWindow::notify()
{
  m_Dispatcher.emit();
}
void MainWindow::on_notification_from_worker_thread()
{
  if (m_WorkerThread && m_Worker.has_stopped())
  {
    // Work is done.
    if (m_WorkerThread->joinable())
      m_WorkerThread->join();
    delete m_WorkerThread;
    m_WorkerThread = nullptr;
  // update_start_stop_buttons();
  }
  update_widgets();
}


MyArea::MyArea()
{
}

MyArea::~MyArea()
{
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
 // std::cout << y << std::endl;
  // coordinates for the center of the window
  

  cr->set_line_width(0.2);

  // draw red lines out from the center of the window
  cr->set_source_rgb(0.8, 0.0, 0.0);
 // cr->move_to(0, 100);
 if(data.size()>0)
  std::cout << data.back().sample_number << "---"<< data.back().force<< std::endl;  
 for(ForceData point:data){
   //below line should put out off loop
   
   axe.update(width, height,data.size(),point.max_force/*, 
                point.min_force*/);
  
  //  std::cout << point.max_force << "----" << endl;
    //std::cout <<point.sample_number<< '-' << point.force << std::endl;
   // std::cout << axe.x(point.sample_number)<<'-'<< axe.y(point.force);
   // std::cout << std::endl;
    cr->line_to(axe.x(point.sample_number), axe.y(point.force));
    cr->move_to(axe.x(point.sample_number), axe.y(point.force));
    
     
 
  }
  
  cr->stroke();
  auto grid_len = axe.draw(cr);

  int len_x_grid = grid_len[0];
  if (data.size()>0)
  for(int i{0}; i < 11; i++){
    cr->move_to(20 +(len_x_grid *i),height/2 );
    std::string x_grid_label = std::to_string((data.back().sample_number/10) * i); 
    auto layout = create_pango_layout(x_grid_label); 
    layout->show_in_cairo_context(cr);
   
    
  }
  
  
  int len_y_grid = grid_len[1];
  if (data.size()>0)
  for(int i{0}; i < 6; i++){
    cr->move_to(20, (height/2)- len_y_grid * i);
    std::string y_grid_label = std::to_string((data.back().max_force/5.0) * static_cast<double>(i)); 
   // std::cout << data.back().max_force << "*****" << endl;
    auto layout = create_pango_layout(y_grid_label); 
    layout->show_in_cairo_context(cr);
 
    cr->move_to(20, (height/2) + len_y_grid * i);
    y_grid_label = std::to_string((data.back().max_force/-5.0) * static_cast<double>(i)); 
  
    layout = create_pango_layout(y_grid_label); 
  
    layout->show_in_cairo_context(cr);
    
  }
 // Pango::FontDescription font;
//  font.set_family("Monospace");
 // font.set_weight(Pango::WEIGHT_BOLD);
  //auto layout = create_pango_layout("Hi there!");
 // layout->set_font_description(font);

 // int text_width;
  //int text_height;
  //layout->get_pixel_size(text_width, text_height);

  // Position the text in the middle
 // cr->move_to((rectangle_width-text_width)/2, (rectangle_height-text_height)/2);

 // layout->show_in_cairo_context(cr);
  

  return true;
}

